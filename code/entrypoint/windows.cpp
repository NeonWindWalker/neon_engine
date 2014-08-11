#ifdef _WIN32

#include "include.h"
#include "../drawer/platformspecific.h"
#include "../reflection/include.h"


#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shlobj.h>
#include <Shellapi.h>

#ifdef DEBUG
Base::boolean gLeaveMouse = true;
#else
Base::boolean gLeaveMouse = false;
#endif
Base::boolean gMouseModeChanged = false;
Base::boolean gFillScrean = false;
Base::boolean gFillScreanChanged = false;
Base::boolean gWindowResized = false;
Base::boolean gWindowRecreatedRequest = false;
HWND ghWnd = 0;
HWND ghOldhWnd = 0;
HINSTANCE ghInstance;

struct TouchInfo
{
	Base::uint id;
	Base::Vector2f pos;
};
Base::HashArray< Base::Pair<Base::uint, TouchInfo> > gTouchEventsRemaping;
int gMuteMouseEventsFromTouch;

const char * gcWindowHelpText = " [F1 - leave mouse, F2 - fullscreen, F4 - hide, F12 - video record]";

extern Base::boolean initDirectInput();
extern void directInputSetCooperativeLevel();
extern void releaseDirectInput();
extern void directInputUpdate();
extern Base::Array<Input::EventBase*> gEvents;

namespace EntryPoint
{
	Base::Delegate<Base::boolean (FileStorageLib::IStorage* packedge, FileStorageLib::IStorage* homedir)> gInitDelegate;
	Base::Delegate<Base::boolean (Base::ConstRange< Input::EventBase* const> events)> gStepDelegate;
	Base::Delegate<void ()> gStepFinishDelegate;
	Base::Delegate<void ()> gDeInitDelegate;

	float gPixelSize;
	Base::boolean gYUp = true;

	float getDensity() { return 1.0f; }
	Base::boolean setDensity(float f) { return false; }
	float getPixelSize() { return gPixelSize; }
	void leaveInput(Base::uint channel)
	{
		gLeaveMouse = true;
		gMouseModeChanged = true;
	}
}

class WindowsInitParams
{
public:
	rl_class(WindowsInitParams)

	rl_field(Base::Vector2i, windowPosition)
	rl_field(Base::Vector2i, windowSize)
	rl_field(Base::boolean, fullscreen)
	rl_field(Base::boolean, maximized)
	rl_field(Base::Vector2i, clientRectSize)

	WindowsInitParams()
	{
		int screedFullWidth = GetSystemMetrics(SM_CXFULLSCREEN);
		int screedFullHeight = GetSystemMetrics(SM_CYFULLSCREEN);
		windowPosition.set(screedFullWidth / 6, screedFullHeight / 6);
		windowSize.set(screedFullWidth * 2/ 3, screedFullHeight * 2 / 3);
		fullscreen = false;
		maximized = false;
		clientRectSize.setZero();
	}

	void load(FileStorageLib::IStorage& storage)
	{
		Base::BinaryData data;
		if(!storage.read(data, _CS("windows.ini")))
			return;
		Base::ConstString str((const Base::utf8*)data.first(), data.byteSize());
		ReflectionLib::reflectionOf(this).deserialize(str);
		fullscreen = Base::boolean01FromBoolean(fullscreen);

		//if(gFillScrean != fullscreen){
		//	gFillScrean = fullscreen;
		//	gFillScreanChanged = true;
		//}
	}

	void save(FileStorageLib::IStorage& storage)
	{
		clientRectSize.setZero();
		fullscreen = Base::boolean01FromBoolean(fullscreen);
		Base::String str;
		ReflectionLib::reflectionOf(this).serialize(str);
		storage.write(_CS("windows.ini"), Base::ConstBinaryData(str.first(), str.size()));
	}
};

class LogListener
{
public:
	void onLog(Base::CodePoint cp, Base::ConstString trace, Base::ConstString ch, Base::ConstString msg)
	{
		Base::String str;
		Base::LogPrinter::genStandardFormatMessage(str, cp, trace, ch, msg);
		str << _CS("\n");
		printf("%s", str.c_str());
		OutputDebugStringA(str.c_str());
		FILE* f = fopen("home/log.txt", "at");
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

/*
unsigned char *to_utf8(unsigned long *unicode)
{
  unsigned char *utf8 = NULL;
  unsigned long *s = unicode;
  unsigned char *u;
  unsigned long ch;
  int x = 0;
  while (*s)
    {
      ++s;
      ++x;
    }
  if (x == 0)
  {
      return NULL;
  }
  utf8 = (unsigned char *)malloc(x*4);
 
  s = unicode;
  u = utf8;
  x = 0;

  while (*s)
    {
      ch = *s;
      if (*s < 0x80)
{
  x = 1;
  *u = *s;
  u++;
}
      else if (*s < 0x800)
{
  x = 2;
  *u = 0xc0 | (ch >> 6);
  u++;
}
      else if (*s < 0x10000)
{
  x = 3;
  *u = 0xe0 | (ch >> 12);
  u++;
}
      else if (*s < 0x200000)
{
  x = 4;
  *u = 0xf0 | (ch >> 18);
  u++;
}
      if (x > 1)
{
  int y;
  for (y = x; y > 1; y--)
    {
      
      //unsigned long mask = 0x3f << ((y-2)*6);
      //*u = 0x80 | (ch & mask);
      
      *u = 0x80 | (ch & (0x3f << ((y-2)*6)));
      ++u;
    }
}
      ++s;
    }
  return utf8;
}
*/

void ClipCursorByClientRect()
{
	RECT clientRect;
	GetClientRect(ghWnd, &clientRect);
	POINT hiPt;
	hiPt.x = clientRect.right - clientRect.left;
	hiPt.y = clientRect.bottom - clientRect.top;
	ClientToScreen(ghWnd, &hiPt);
	POINT lowPt;
	lowPt.x = 0;
	lowPt.y = 0;
	ClientToScreen(ghWnd, &lowPt);

	RECT clipRect;
	clipRect.left = lowPt.x;
	clipRect.top = lowPt.y;
	clipRect.right = hiPt.x;
	clipRect.bottom = hiPt.y;

	SetCursorPos((clipRect.left + clipRect.right) / 2, (clipRect.top + clipRect.bottom) / 2);
	ClipCursor(&clipRect);
}


LONG WINAPI MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(hWnd != ghWnd)
		return DefWindowProc(hWnd, uMsg, wParam, lParam); 

	switch(uMsg)
	{
	case WM_CLOSE:
		{
			Input::EventKey* ev = new Input::EventKey();
			ev->up = true;
			ev->key = Input::EKeyType_CloseApplication;
			gEvents.push(ev);
		}
		return 0;

	case WM_DESTROY:
		if(!gFillScreanChanged)
			PostQuitMessage(0);
		return 0;

	case WM_CHAR:
		if(EntryPoint::gInputState.text[0].printing)
		{
			EntryPoint::addCharacterToText(EntryPoint::gInputState.text[0], wParam);
		}
		return 0;

	/*case WM_UNICHAR:
		if(EntryPoint::gInputState.text[0].printing){
			unsigned long ch[2];
			ch[0] = wParam;
			ch[1] = 0;
			char* c = (char*)to_utf8(ch);
			EntryPoint::gInputState.text[0].string<<Base::constString(c);
			free(c);
		}
		return 1;*/

	case WM_SIZE:

		gWindowResized = true;
		//if(gRenderWindow)
		//	gRenderWindow->resize();

		if(!gLeaveMouse)
			ClipCursorByClientRect();
		

		return 0;

#ifndef _USING_V110_SDK71_
	case WM_TOUCH:
		{
			UINT cInputs = LOWORD(wParam);
			PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
			if (GetTouchInputInfo((HTOUCHINPUT)lParam,
				cInputs,
				pInputs,
				sizeof(TOUCHINPUT)))
			{
				// process pInputs
				RECT childRect;
				GetClientRect(ghWnd, &childRect);
				for(UINT i=0; i < cInputs; ++i)
				{
					TOUCHINPUT& src = pInputs[i];
					Input::EventTouch* ev = new Input::EventTouch();
					POINT pt;
					pt.x = TOUCH_COORD_TO_PIXEL(src.x);
					pt.y = TOUCH_COORD_TO_PIXEL(src.y);
					ScreenToClient(ghWnd, &pt);
					ev->x = float(pt.x);
					ev->y = EntryPoint::gYUp ? float(childRect.bottom - childRect.top - 1 - pt.y) : float(pt.y);
					ev->down = (src.dwFlags & TOUCHEVENTF_DOWN) != 0;
					ev->up = (src.dwFlags & TOUCHEVENTF_UP) != 0;

					Base::boolean bNew;
					auto it = gTouchEventsRemaping.insert(bNew, (Base::uint)src.dwID);
					if(bNew)
					{
						const int maxChannel = 4;
						for(int i = 0; i < (int)gTouchEventsRemaping.size() - maxChannel; ++i)
							gTouchEventsRemaping.eraseByIndex(0);

						bool b[maxChannel] = { false, false, false, false };

						for(auto i = gTouchEventsRemaping.first(); i != it; ++i)
							b[i->val.id] = true;
							
						for(int n = 0; n < maxChannel; ++n)
							if(!b[n]){
								it->val.id = n;
								break;
							}
					}
					else
					{
						ev->dx = ev->x - it->val.pos.x;
						ev->dy = ev->y - it->val.pos.y;
					}
					it->val.pos.x = ev->x;
					it->val.pos.y = ev->y;

					ev->channel = it->val.id;
					Assert(ev->channel < Input::EventBase::gMaxChannels);
					gEvents.push(ev);

					if(ev->up)
						gTouchEventsRemaping.erase(it);
				}

				if (!CloseTouchInputHandle((HTOUCHINPUT)lParam))
				{
					// error handling
				}
			}
			delete [] pInputs;
		}
		break;
#endif

	case WM_SETFOCUS:
		
		break;

	case WM_KILLFOCUS:
		if(!gLeaveMouse){
			gLeaveMouse = true;
			gMouseModeChanged = true;
		}
		break;

	case WM_LBUTTONDOWN:
		if(gLeaveMouse){
			gLeaveMouse = false;
			gMouseModeChanged = true;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); 
}

BOOL SetClientRect(HWND hWnd, int x, int y)
{
	RECT rect = {0,0,x,y}, rect2;
	AdjustWindowRectEx(&rect, GetWindowLong(hWnd,GWL_STYLE), (BOOL)GetMenu(hWnd), GetWindowLong(hWnd, GWL_EXSTYLE));
	GetWindowRect(hWnd, &rect2);
	return MoveWindow(hWnd, rect2.left, rect2.top, rect.right-rect.left,rect.bottom-rect.top, TRUE);
}

#ifdef _UNICODE
#define strT(x) L##x
#else
#define strT(x) x
#endif

extern void CalcEntryPointPixelSize(HWND hWnd);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ghInstance = hInstance;

	fclose(fopen("home/log.txt", "wt"));

	LogListener ll;

	logInfo << "Starting";

	srand(0);

	ReflectionLib::initReflectionLibrary();

	Base::String packdir = _CS("data");
	Base::String homedir = _CS("home");
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
	
	WindowsInitParams initialParams;
	initialParams.load(*gHomeFileSystem);
	gFillScrean = initialParams.fullscreen;

	WNDCLASS    wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)MainWindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = strT("MainWindow");
	if (!RegisterClass(&wc)){
		logError << "RegisterClass";
		return 0;
	}

	Base::String title;
	title << Base::constString(EntryPoint::gcGameName) << Base::constString(gcWindowHelpText);

#ifdef _UNICODE
	WCHAR titleStr[2048];
	MultiByteToWideChar(CP_ACP, 0, title.c_str(), -1, titleStr, 2048);
#else
	CHAR* titleStr = (CHAR*)title.c_str(); 
#endif
	
	ghWnd = CreateWindow(strT("MainWindow"), titleStr, gFillScrean ? WS_POPUP : WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, initialParams.windowPosition.x, initialParams.windowPosition.y, initialParams.windowSize.x, initialParams.windowSize.y, NULL, NULL, hInstance, NULL);
	if (ghWnd == 0){
		logError << "CreateWindow";
		return 0;
	}

#ifndef _USING_V110_SDK71_
	RegisterTouchWindow(ghWnd, TWF_WANTPALM | TWF_FINETOUCH);
#endif

	if(initialParams.clientRectSize.x && initialParams.clientRectSize.y)
		SetClientRect(ghWnd, initialParams.clientRectSize.x, initialParams.clientRectSize.y);

	if(!initDirectInput()){
		releaseDirectInput();
		return 0;
	}

	logInfo << "ShowWindow";
	if(gFillScrean || initialParams.maximized)
		ShowWindow( ghWnd, SW_SHOWMAXIMIZED);
	else
		ShowWindow( ghWnd, SW_SHOW);

	logInfo << "UpdateWindow";
	UpdateWindow( ghWnd );

	CalcEntryPointPixelSize(ghWnd);
	/*{
		HDC screen = GetDC(NULL);
		int hSize = GetDeviceCaps(screen, HORZSIZE);
		int hRes = GetDeviceCaps(screen, HORZRES);
		EntryPoint::gcPixelSize = (float)hSize / (float)hRes * 0.001f;
	}*/

	

	EntryPoint::registerDelegates();

	logInfo << "Starting init game";
	if(!EntryPoint::gInitDelegate(gPackFileSystem, gHomeFileSystem)){
		logError << "Init game, do SHUTDOWN";
		releaseDirectInput();
		DestroyWindow(ghWnd);
		return 0;
	}
	else
		logInfo << "Succses init game";

	gMouseModeChanged = !gLeaveMouse;

	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	logInfo << "Statring message cycle";
	int needUpdatePixelSizeCounter = 0;
	bool stepFinishCalled = false;
	while(msg.message != WM_QUIT)
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if(gFillScreanChanged && ghOldhWnd == 0)
			{
				logInfo << "Changing fullscreen/windowed mode";
				EntryPoint::gStepFinishDelegate();

				ghOldhWnd = ghWnd;

				if(gFillScrean)
				{
					RECT r;
					GetWindowRect(ghOldhWnd, &r);
					//save windowed params
					initialParams.windowPosition.x = r.left;
					initialParams.windowPosition.y = r.top;
					initialParams.windowSize.x = r.right - r.left;
					initialParams.windowSize.y = r.bottom - r.top;
					WINDOWPLACEMENT  wp;
					GetWindowPlacement(ghOldhWnd, &wp);
					initialParams.maximized = wp.showCmd == SW_MAXIMIZE;
				}

				ghWnd = CreateWindow( strT("MainWindow"), titleStr, gFillScrean ? WS_POPUP : WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, initialParams.windowPosition.x, initialParams.windowPosition.y, initialParams.windowSize.x, initialParams.windowSize.y, NULL, NULL, hInstance, NULL);
#ifndef _USING_V110_SDK71_
				RegisterTouchWindow(ghWnd, TWF_WANTPALM | TWF_FINETOUCH);
#endif
				//SetClientRect(ghWnd, EntryPoint::gInitialWindowWidth, EntryPoint::gInitialWindowHeight);
				
				if(gFillScrean)
					ShowWindow(ghWnd, SW_SHOWMAXIMIZED);
				else
					ShowWindow(ghWnd, SW_SHOW);

				directInputSetCooperativeLevel();

				//DestroyWindow(oldwnd);
				//ShowWindow(oldwnd, HIDE_WINDOW);

				gFillScreanChanged = false;
				gWindowRecreatedRequest = true;
				initialParams.fullscreen = gFillScrean;
				initialParams.save(*gHomeFileSystem);
			}

			if(gTouchEventsRemaping.size())
				gMuteMouseEventsFromTouch = 10;

			directInputUpdate();

			if(gMuteMouseEventsFromTouch)
				gMuteMouseEventsFromTouch --;

			if(gMouseModeChanged)
			{
				if(gLeaveMouse){
					ClipCursor(0);
					RECT r;
					GetWindowRect(ghWnd, &r);
					SetCursorPos((r.left + r.right) / 2, r.top + 16);
					for(int n=0; n < 16 && ShowCursor(true) < 0; n++);
				}
				else{
					Input::EventCursorAccure* ev = new Input::EventCursorAccure();

					POINT cur;
					GetCursorPos(&cur);
					ScreenToClient(ghWnd, &cur);
					RECT clientRect;
					GetClientRect(ghWnd, &clientRect);
					ev->x = (float)Base::clampt<int>(cur.x, 0, clientRect.right - clientRect.left - 1);
					ev->y = (float)Base::clampt<int>(EntryPoint::gYUp ? clientRect.bottom - clientRect.top - 1 - cur.y : cur.y, 0, clientRect.bottom - clientRect.top - 1);
					gEvents.clearSaveReserve();
					gEvents.push(ev);

					ClipCursorByClientRect();
					for(int n=0; n < 16 && ShowCursor(false) >= 0; n++);
				}
			
				gMouseModeChanged = false;
			}

			

			if(++needUpdatePixelSizeCounter > 100){
				CalcEntryPointPixelSize(ghWnd);
				needUpdatePixelSizeCounter = 0;
			}

			if(GetWindowLong(ghWnd, GWL_STYLE) & WS_MINIMIZE)
			{
				if(!stepFinishCalled)
				{
					if(EntryPoint::gStepFinishDelegate)
						EntryPoint::gStepFinishDelegate();
					stepFinishCalled = true;
				}
				Sleep(100);
			}
			else
			{
				if(!EntryPoint::gStepDelegate(Base::ConstRange< Input::EventBase* const>(gEvents.first(), gEvents.size())))
					PostQuitMessage(0);

				stepFinishCalled = false;
			}
			
			for(Base::uint i=0; i < gEvents.size(); ++i)
				delete gEvents[i];
			gEvents.clear();

			if(ghOldhWnd != 0){
				DestroyWindow(ghOldhWnd);
				ghOldhWnd = 0;
			}
			//Sleep(100);
		}
	}
	if(!gLeaveMouse){
		ClipCursor(0);
		ShowCursor(true);
	}

	if(!gFillScrean)
	{
		RECT r;
		GetWindowRect(ghWnd, &r);
		initialParams.windowPosition.x = r.left;
		initialParams.windowPosition.y = r.top;
		initialParams.windowSize.x = r.right - r.left;
		initialParams.windowSize.y = r.bottom - r.top;
		WINDOWPLACEMENT  wp;
		GetWindowPlacement(ghWnd, &wp);
		initialParams.maximized = wp.showCmd == SW_MAXIMIZE;
	}
	initialParams.fullscreen = gFillScrean;
	initialParams.save(*gHomeFileSystem);

	logInfo << "Out form message cycle";
	logInfo << "Deinit game";
	if(!stepFinishCalled && EntryPoint::gStepFinishDelegate)
		EntryPoint::gStepFinishDelegate();
	EntryPoint::gDeInitDelegate();
	logInfo << "Delete direct input";
	releaseDirectInput();
	logInfo << "Destroy window";
	DestroyWindow(ghWnd);
	if(ghOldhWnd != 0)
		DestroyWindow(ghOldhWnd);
	delete gHomeFileSystem;
	delete gPackFileSystem;
	gTouchEventsRemaping.clear();
	ReflectionLib::deinitReflectionLibrary();
	logInfo << "Done";
	return 0;
}


Base::boolean EntryPoint::renderInit(DrawerLib::IDevice*& rDevice, DrawerLib::IFrameWindow*& rWindow)
{
	DrawerLib::WindowHandle wh;
	wh.hWnd = ghWnd;
	wh.hDC = GetDC(ghWnd);

	logInfo << "Starting create render device";
	//gRenderDevice = DrawerLib::createDeviceWithMainWindow(gRenderWindow, wh, gFillScrean);
	rDevice = DrawerLib::createDevice(DrawerLib::DisplayHandle());
	if(!rDevice){
		logError << "Create render device";
		return false;
	}
	else
		logInfo << "Succses create render device";

	logInfo << "Starting create render window";
	rWindow = rDevice->createFrameWindow();
	if(!rWindow){
		logError << "Create render window, do SHUTDOWN";
		delete rDevice;
		return false;
	}
	else
		logInfo << "Succses create render window";

	logInfo << "Starting init redner window";
	if(!rWindow->init(wh, gFillScrean)){
		logError << "Init redner window, do SHUTDOWN";
		//DrawerLib::safe_release(rWindow);
		delete rDevice;
		return false;
	}
	else
		logInfo << "Succses init redner window";

	rDevice->setSyncInterval(0);
	return true;
}

Base::boolean EntryPoint::renderWindowPrepare(DrawerLib::IFrameWindow* rWindow)
{
	if(!rWindow)
		return false;

	if(gWindowRecreatedRequest || gWindowResized){
		DrawerLib::WindowHandle wh;
		wh.hWnd = ghWnd;
		wh.hDC = GetDC(ghWnd);

		logInfo << "Starting reinit redner window";
		if(!rWindow->init(wh, gFillScrean)){
			logError << "Reinit redner window";
			return false;
		}
		else
			logInfo << "Succses reinit redner window";

		gWindowRecreatedRequest = false;
		gWindowResized = false;
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
	//DrawerLib::safe_release(rWindow);
	delete rDevice;
	return true;
}

Base::Vector2i EntryPoint::getScreenSize()
{
	return Vector2i(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
}

Base::String EntryPoint::getDeviceName()
{
	return _CS("WindowsPC");
}

void EntryPoint::openURL(Base::ConstString url)
{
	WCHAR str[2048];
	MultiByteToWideChar(CP_ACP, 0, Base::String(url).c_str(), -1, str, 2048);
	ShellExecute(ghWnd, L"open", str, NULL, NULL, SW_NORMAL);
}

void EntryPoint::openRateUs()
{

}

Base::String EntryPoint::getLanguage()
{
	WCHAR wstr[LOCALE_NAME_MAX_LENGTH];
	uint len = GetUserDefaultLocaleName(wstr, LOCALE_NAME_MAX_LENGTH);
	if(!len)
		return _CS("");
	Base::String s;
	s.resize(len * 3);
	WideCharToMultiByte(CP_ACP, 0, wstr, len, (CHAR*)s.first(), s.size(), 0, 0);
	//s.resizeToCStr();
	s.resize(2);
	return s;
}

#endif