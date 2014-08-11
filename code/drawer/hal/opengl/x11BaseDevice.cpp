#include "device.h"

#if defined(DRAWER_API_OPENGL) && defined(__linux__) && !defined(__ANDROID__)

namespace DrawerLib
{ 

PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI = 0;

static boolean gLoadExtensionsDone = false;

extern void loadExtensions()
{
	if(gLoadExtensionsDone)
		return;
	gLoadExtensionsDone = true;

	loadCrossplatformExtensions();

	LOAD_EXTENSION(PFNGLXSWAPINTERVALSGIPROC, glXSwapIntervalSGI)
}

XVisualInfo* getXVisualInfo(Colormap& outColorMap, const DisplayHandle& display)
{
	loadExtensions();

	int attrList[] =
	{
		GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24,
		None
	};

	int attrListMini[] =
	{
		GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16,
		None
	};

	XVisualInfo* vi = glXChooseVisual(display.display, display.screen, attrList);
	//if(vi == 0)
	//	vi = glXChooseVisual(display.display, display.screen, attrListMini);
	if (vi == 0){
		logError << "glXChooseVisual";
		return 0;
	}

	outColorMap =  XCreateColormap(display.display, RootWindow(display.display, vi->screen), vi->visual, AllocNone);
	return vi;
}

BaseOSDevice::BaseOSDevice(const DisplayHandle& display)
{
	_window.display = display.display;
	_window.screen = display.screen;
	_externalCreatedWindow = false;
	_fullscreanWindow = false;
	_glxContext = 0;

	Colormap colormap;
	XVisualInfo *vi = getXVisualInfo(colormap, display);
	if (vi == 0)
		return;

	int glxMajor=0, glxMinor=0;
	glXQueryVersion(_window.display, &glxMajor, &glxMinor);
	logInfo << "glXQueryVersion " << glxMajor << "." << glxMinor;

	_glxContext = glXCreateContext(_window.display, vi, 0, GL_TRUE);
	if(!_glxContext){
		logError << "glXCreateContext";
		return;
	}

	XSetWindowAttributes windowAttributes;
	windowAttributes.colormap = colormap;
	windowAttributes.border_pixel = 0;

	_window.window = XCreateWindow(_window.display, RootWindow(_window.display, vi->screen),
		0, 0, 32, 32, 0, vi->depth, InputOutput, vi->visual,
		CWBorderPixel | CWColormap, &windowAttributes);

	if(!_window.window){
		logError << "Cant create internal main window";
		return;
	}

	XSelectInput(_window.display, _window.window, 0);
	//XMapRaised(_window.display, _window.window);

	if(makeCurrentContext())
		logInfo << "Initialization OpenGL with internal window - OK";
	else{
		logError << "Cant create valid context";
		glXDestroyContext(_window.display, _glxContext);
		_glxContext = 0;
	}
}

BaseOSDevice::BaseOSDevice(const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo)
{
	_window = srcWainWindowHandle;
	_externalCreatedWindow = true;
	_fullscreanWindow = fullscrean;
	_glxContext = 0;

	Colormap colormap;
	XVisualInfo *vi = getXVisualInfo(colormap, _window);
	if (vi == 0)
		return;

	int glxMajor=0, glxMinor=0;
	glXQueryVersion(_window.display, &glxMajor, &glxMinor);
	logInfo << "glXQueryVersion " << glxMajor << "." << glxMinor;

	_glxContext = glXCreateContext(_window.display, vi, 0, GL_TRUE);
	if(!_glxContext){
		logError << "glXCreateContext";
		return;
	}

	XSetWindowAttributes windowAttributes;
	windowAttributes.colormap = colormap;
	windowAttributes.border_pixel = 0;
	windowAttributes.override_redirect = True;
	XChangeWindowAttributes(_window.display, _window.window, CWBorderPixel | CWColormap | (fullscrean ? CWOverrideRedirect : 0), &windowAttributes);

	if(fullscrean)
		XMoveResizeWindow(_window.display, _window.window, 0, 0, DisplayWidth(_window.display, _window.screen), DisplayHeight(_window.display, _window.screen));

	//XMapRaised(_window.display, _window.window);

	if(makeCurrentContext())
	{
		logInfo << "Initialization OpenGL with main window - OK";

		if (glXIsDirect(_window.display, _glxContext))
			logInfo << "DRI enabled\n";
		else
			logInfo << "no DRI available\n";
	}
	else{
		logError << "Cant create valid context";
		glXDestroyContext(_window.display, _glxContext);
		_glxContext = 0;
	}
}

BaseOSDevice::~BaseOSDevice()
{
	if( _glxContext){
		glXMakeCurrent(_window.display, None, NULL);
		glXDestroyContext(_window.display, _glxContext);
		_glxContext = 0;
	}

	if(_window.window)
	{
		if(!_externalCreatedWindow)
			XDestroyWindow(_window.display, _window.window);
		_window.window = 0;
	}
}

boolean BaseOSDevice::makeCurrentContext()
{
	if(!glXMakeCurrent(_window.display, _window.window, _glxContext)){
		logWarning << "glXMakeCurrent fail";
		return false;
	}
	return true;
}

boolean BaseOSDevice::setSyncInterval(uint synk)
{
	if(glXSwapIntervalSGI){
		glXSwapIntervalSGI(synk);
		return true;
	}
	return false;
}

BaseOSFrameWindow::BaseOSFrameWindow(BaseOSDevice& dev, boolean isMainWindowHack) : IFrameWindow(dev), _osdevice(dev)
{
	if(isMainWindowHack)
	{
		_window = _osdevice.window();
		_glxContext = _osdevice.glxContext();
		_noDeleteContextOnDeestroy = true;
		XWindowAttributes attr;
		XGetWindowAttributes(_window.display, _window.window, &attr);
		_size = Vector2i(attr.width, attr.height);
	}
	else
	{
		_glxContext = 0;
		_noDeleteContextOnDeestroy = false;
		_size.set(0,0);
	}
}

BaseOSFrameWindow::~BaseOSFrameWindow()
{
	deinit();
}

void BaseOSFrameWindow::deinit()
{
	if(!_noDeleteContextOnDeestroy && _glxContext){
		_osdevice.makeCurrentContext();
		glXDestroyContext(_window.display, _glxContext);
	}
	_glxContext = 0;
	_window.display = 0;
	_window.screen = 0;
	_window.window = 0;
	_size.set(0,0);
}

boolean BaseOSFrameWindow::init(const WindowHandle& wnd, boolean fullscrean, boolean stereo)
{
	deinit();

	_window = wnd;

	Colormap colormap;
	XVisualInfo *vi = getXVisualInfo(colormap, _window);
	if (vi == 0)
		return false;

	_glxContext = glXCreateContext(_window.display, vi, _osdevice.glxContext(), GL_TRUE);
	if(!_glxContext){
		logError << "glXCreateContext";
		deinit();
		return false;
	}

	XSetWindowAttributes windowAttributes;
	windowAttributes.colormap = colormap;
	windowAttributes.border_pixel = 0;
	windowAttributes.override_redirect = fullscrean;
	XChangeWindowAttributes(_window.display, _window.window, CWBorderPixel | CWColormap | (fullscrean ? CWOverrideRedirect : 0), &windowAttributes);

	XMapRaised(_window.display, _window.window);

	if(fullscrean)
		XMoveResizeWindow(_window.display, _window.window, 0, 0, DisplayWidth(_window.display, _window.screen), DisplayHeight(_window.display, _window.screen));


	XWindowAttributes attr;
	XGetWindowAttributes(_window.display, _window.window, &attr);
	_size = Vector2i(attr.width, attr.height);

	if(makeCurrentContext())
	{
		logInfo << "Initialization OpenGL window - OK";

		if (glXIsDirect(_window.display, _glxContext))
			logInfo << "DRI enabled\n";
		else
			logInfo << "no DRI available\n";

		_osdevice.makeCurrentContext();
		return true;
	}
	else{
		logError << "Cant create valid context";
		deinit();
		return false;
	}
}

boolean BaseOSFrameWindow::makeCurrentContext()
{
	if(!glXMakeCurrent(_window.display, _window.window, _glxContext)){
		logWarning << "glXMakeCurrent fail";
		return false;
	}
	return true;
}

void BaseOSFrameWindow::resize()
{
	if(!isValid())
		return;
	makeCurrentContext();
	XWindowAttributes attr;
	XGetWindowAttributes(_window.display, _window.window, &attr);
	_size = Vector2i(attr.width, attr.height);
	glViewport(0, 0, _size.x, _size.y);
	_osdevice.makeCurrentContext();
}

/*
void BaseOSFrameWindow::resize(uint width, uint height)
{
	if(!isValid())
		return;
	makeCurrentContext();
	_size = Vector2i(width, height);
	glViewport(0, 0, _size.x, _size.y);
	_osdevice.makeCurrentContext();
}
*/

void BaseOSFrameWindow::present()
{
	//if(!makeCurrentContext())
	//	return;
	//glClearColor(0,1,0,1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glXSwapBuffers(_window.display, _window.window);
	glFinish();
}

}

#endif

