#include "include.h"

#if defined(__linux__) && !defined(__ANDROID__)

#include "../drawer/platformspecific.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

USING_BASE_LIB

extern Array<Input::EventBase*> gEvents;
Display* gDisplay;
int gScreen;
Window gWindow = 0;
Window gOldWindow = 0;
Window gWindowAwaitingDestroy = 0;
boolean gFullScreen = false;
boolean gNeedRenderResize = true;
int gRenderWindowPrevWidth = -1;
int gRenderWindowPrevHeight = -1;
boolean gMuteConfigureNotify = false;
Cursor gHiddenCursor;
const char * gcWindowHelpText = " [F1 - leave mouse, F2 - fullscreen, F3 - force text input(experimetnal)]";

extern boolean gLockMouse;

void reciveInputBegin();
void reciveInput(XEvent& we);
void reciveInputEnd();
void lockMouse();
void unlockMouse();

namespace EntryPoint
{
Base::Delegate<Base::boolean (FileStorageLib::IStorage* packedge, FileStorageLib::IStorage* homedir)> gInitDelegate;
Base::Delegate<Base::boolean (Base::ConstRange< Input::EventBase* const> events)> gStepDelegate;
Base::Delegate<void ()> gStepFinishDelegate;
Base::Delegate<void ()> gDeInitDelegate;
}


class LogListener
{
public:
	void onLog(Base::CodePoint codePoint, Base::ConstString trace, Base::ConstString ch, Base::ConstString msg)
	{
		Base::String str;
		Base::LogPrinter::genStandardFormatMessage(str, codePoint, trace, ch, msg);
		str << _CS("\n");
		printf("%s", str.c_str());
		FILE* f = fopen("log.txt", "at");
		if(f){
			fprintf(f, "%s", str.c_str());
			fclose(f);
		}
	}

	LogListener()
	{
		Base::LogPrinter::addListener(Base::makeDelegate(this, &LogListener::onLog));
	}

	~LogListener()
	{
		Base::LogPrinter::removeListener(Base::makeDelegate(this, &LogListener::onLog));
	}
};

int main(int argc, char* argv[])
{
	fclose(fopen("log.txt", "wt"));

	LogListener ll;
	
	Base::String title;
	title << Base::constString(EntryPoint::gcGameName) << Base::constString(gcWindowHelpText);

	gDisplay = XOpenDisplay(0);
	gScreen = DefaultScreen(gDisplay);

	Colormap cmap;
	XVisualInfo* vi = DrawerLib::getXVisualInfo(cmap, DrawerLib::DisplayHandle(gDisplay, gScreen));
	XSetWindowAttributes windowAttributes;
	windowAttributes.colormap = cmap;
	gWindow = XCreateWindow(gDisplay, RootWindow(gDisplay, gScreen),
			0, 0, EntryPoint::gInitialWindowWidth, EntryPoint::gInitialWindowHeight, 0, vi->depth, InputOutput, vi->visual, CWColormap, &windowAttributes);
	//XSelectInput(wh.display, wh.window, ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask);
	XMapWindow(gDisplay, gWindow);
	
	XStoreName(gDisplay, gWindow, title.c_str());
	XSetIconName(gDisplay, gWindow, title.c_str());

	Atom		pnProtocol[2];
	Atom		nWMProtocols;

	pnProtocol[0] = XInternAtom (gDisplay, "WM_DELETE_WINDOW", True);
	pnProtocol[1] = XInternAtom (gDisplay, "WM_SAVE_YOURSELF", True);
	nWMProtocols = XInternAtom (gDisplay, "WM_PROTOCOLS", True);
	XSetWMProtocols (gDisplay, gWindow, pnProtocol, 2);

	Pixmap hiddenCursorPixmap = XCreatePixmap(gDisplay, gWindow, 1, 1, 1);
	XColor hiddenCursorColor;
	memset(&hiddenCursorColor, 0, sizeof(hiddenCursorColor));
	gHiddenCursor = XCreatePixmapCursor(gDisplay, hiddenCursorPixmap, hiddenCursorPixmap, &hiddenCursorColor, &hiddenCursorColor, 0, 0 );

	Base::String packdir = _CS("data/");
	Base::String homedir = _CS("home/");
	FileStorageLib::IStorage* gPackFileSystem = 0;
	FileStorageLib::IStorage* gHomeFileSystem = 0;

	gPackFileSystem = FileStorageLib::openStorage(packdir);
	if(gPackFileSystem == 0){
		logError << "cant file system, path: ", homedir;
		return 0;
	}
	gHomeFileSystem = FileStorageLib::openStorage(homedir);
	if(gHomeFileSystem == 0){
		delete gPackFileSystem;
		logError << "cant file system, path: ", homedir;
		return 0;
	}

	EntryPoint::registerDelegates();

	logInfo << "Starting init game";
	if(!EntryPoint::gInitDelegate(gPackFileSystem, gHomeFileSystem)){
		logError << "Init game, do SHUTDOWN";
		XCloseDisplay(gDisplay);
		return 0;
	}
	else
		logInfo << "Succses init game";

#ifndef DEBUG
	lockMouse();
#endif

	Base::boolean done = false;
	XEvent we;
	while (!done)
	{
		if(gFullScreen)
		{
			XGrabKeyboard(gDisplay, gWindow, false, GrabModeAsync, GrabModeAsync, CurrentTime);
			XGrabPointer(gDisplay, gWindow, true, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, RootWindow(gDisplay, gScreen), None, CurrentTime);
		}
		else
		{
			XUngrabKeyboard(gDisplay, CurrentTime);
			XUngrabPointer(gDisplay, CurrentTime);
			XSelectInput(gDisplay, gWindow, ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask);
		}

		reciveInputBegin();

		while (!done && XPending(gDisplay) > 0)
		{

			XNextEvent(gDisplay, &we);

			if(we.type == ClientMessage)
			{
				if (we.xclient.message_type == nWMProtocols)
				{
					if (we.xclient.data.l[0] == pnProtocol[0])
					{
						logInfo << "To Destroy Window";
						if(we.xany.window == gWindow)
							done = true;
					}
					else if (we.xclient.data.l[0] == pnProtocol[1])
					{
						logInfo << "To Save Window";
						if(we.xany.window == gWindow)
							done = true;
					}
				}
			}

			if(we.xany.window != gWindow)
				continue;

			reciveInput(we);

			switch(we.type)
			{
			case ConfigureNotify:
			{
				XConfigureEvent xce = we.xconfigure;
				
				if(!gMuteConfigureNotify && (gRenderWindowPrevWidth != xce.width || gRenderWindowPrevHeight != xce.height ))
				{
					gRenderWindowPrevWidth = xce.width;
					gRenderWindowPrevHeight = xce.height;
					gNeedRenderResize = true;
				}
				
					
				break;
			}

			case ButtonPress:
				if(!gLockMouse)
					lockMouse();
				break;

			case KeyPress:
				{
					switch(we.xkey.keycode)
					{
					case 67:
						if(gLockMouse)
							unlockMouse();
						break;
					case 68:
						{
							if(gOldWindow)
								break;

							gFullScreen = !gFullScreen;
							gOldWindow = gWindow;

							if(gFullScreen){
								XWindowAttributes attr;
								XGetWindowAttributes(gDisplay, gOldWindow, &attr);
								EntryPoint::gInitialWindowWidth = attr.width;
								EntryPoint::gInitialWindowHeight = attr.height;
							}

							XSetWindowAttributes attr;
							attr.colormap = cmap;
							gWindow = XCreateWindow(gDisplay, RootWindow(gDisplay, gScreen),
									0, 0, EntryPoint::gInitialWindowWidth, EntryPoint::gInitialWindowHeight, 0, vi->depth, InputOutput, vi->visual, CWColormap, &attr);

							XSetWMProtocols(gDisplay, gWindow, pnProtocol, 2);
							
							XStoreName(gDisplay, gWindow, title.c_str());
							XSetIconName(gDisplay, gWindow, title.c_str());
	

							if(gLockMouse)
								lockMouse();
							else
								unlockMouse();
						}
						break;
					}
				}
				break;
			}
		}

		if(done)
			break;

		reciveInputEnd();

		if(!EntryPoint::gStepDelegate(gEvents))
			done = true;

		if(gWindowAwaitingDestroy){
			XDestroyWindow(gDisplay, gWindowAwaitingDestroy);
			gWindowAwaitingDestroy = 0;
		}
	}
	logInfo << "Exit form message loop";
	if(gFullScreen){
		XUngrabKeyboard(gDisplay, CurrentTime);
		XUngrabPointer(gDisplay, CurrentTime);
	}
	EntryPoint::gDeInitDelegate();
	XFreeCursor(gDisplay, gHiddenCursor);
	XFreePixmap(gDisplay, hiddenCursorPixmap);
	XCloseDisplay(gDisplay);
	logInfo << "Done";
	return 0;
}

Base::boolean EntryPoint::renderInit(DrawerLib::IDevice*& rDevice, DrawerLib::IFrameWindow*& rWindow)
{
	logInfo << "Starting create render device";
	rDevice = DrawerLib::createDevice(DrawerLib::DisplayHandle(gDisplay, gScreen));
	if(!rDevice){
		logError << "Create render device";
		return false;
	}
	logInfo << "Succses create render device";

	DrawerLib::WindowHandle wh;
	wh.display = gDisplay;
	wh.screen = gScreen;
	wh.window = gWindow;

	logInfo << "Starting create render window";
	rWindow = rDevice->createFrameWindow();
	if(!rWindow->init(wh)){
		logError << "Create render window";
		delete rDevice;
		return false;
	}
	logInfo << "Succses create render window";

	rDevice->setSyncInterval(1);
	return true;
}

Base::boolean EntryPoint::renderWindowPrepare(DrawerLib::IFrameWindow* rWindow)
{
	gMuteConfigureNotify = false;
	
	if(!rWindow)
		return false;

	if(gOldWindow || gNeedRenderResize)
	{
		DrawerLib::WindowHandle wh;
		wh.display = gDisplay;
		wh.screen = gScreen;
		wh.window = gWindow;

		logInfo << "Starting reinit render window";
		if(!rWindow->init(wh, gFullScreen)){
			logError << "Reinit render window";
			return false;
		}
		else
			logInfo << "Succses reinit render window";

		gWindowAwaitingDestroy = gOldWindow;
		gOldWindow = 0;
		gNeedRenderResize = false;
		gMuteConfigureNotify = true;
	}

	return true;
}

Base::boolean EntryPoint::renderWindowPresent(DrawerLib::IFrameWindow* rWindow)
{
	if(!rWindow)
		return false;
	
	rWindow->present();
	return true;
}

Base::boolean EntryPoint::renderDeInit(DrawerLib::IDevice* rDevice, DrawerLib::IFrameWindow* rWindow)
{
	//rWindow->selfDelete();
	delete rDevice;
	return true;
}

#endif
