#include "device.h"

#if defined(DRAWER_API_OPENGL) && defined(_WIN32)

#include "../../platformspecific.h"

namespace DrawerLib
{ 

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;

static boolean gLoadExtensionsDone = false;

extern void loadExtensions()
{
	if(gLoadExtensionsDone)
		return;
	gLoadExtensionsDone = true;

	loadCrossplatformExtensions();

	LOAD_EXTENSION(PFNWGLSWAPINTERVALEXTPROC, wglSwapIntervalEXT)
}

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 

#ifdef _UNICODE
#define strT(x) L##x
#else
#define strT(x) x
#endif

BaseOSDevice::BaseOSDevice(const DisplayHandle& displayHandle)
{
	_externalCreatedWindow = false;
	_fullscreanWindow = false;
	_glContext = 0;

	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASS    wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = strT("OpenGLMainWindow");
	if (!RegisterClass(&wc)){
		logError << "RegisterClass";
		return;
	}

	_window.hWnd = CreateWindow(strT("OpenGLMainWindow"), strT("OpenGLMainWindow"), WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 32, 32, NULL, NULL, hInstance, NULL);
	if (_window.hWnd == 0){
		logError << "CreateWindow";
		return;
	}
	_window.hDC = (GetDC((_window.hWnd)));

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	int pf = ChoosePixelFormat((_window.hDC), &pfd);
	if(pf == 0){
		logError << "ChoosePixelFormat";
		return;
	}
	if(SetPixelFormat((_window.hDC), pf, &pfd) == FALSE){
		logError << "SetPixelFormat";
		return;
	} 
	DescribePixelFormat((_window.hDC), pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	_glContext = wglCreateContext((_window.hDC));
	if(!_glContext){
		logError << "wglCreateContext";
		return;
	}
	makeCurrentContext();

	ShowWindow((_window.hWnd), SW_HIDE);
	//UdateWindow(_window.hWnd);

	logInfo << "Sucsses init device";
	loadExtensions();
}

BaseOSDevice::BaseOSDevice(const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo)
{
	_externalCreatedWindow = true;
	_fullscreanWindow = false;
	_window = srcWainWindowHandle;
	_glContext = 0;

	if(fullscrean)
		MoveWindow((_window.hWnd), 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), false);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	int pf = ChoosePixelFormat((_window.hDC), &pfd);
	if(pf == 0){
		logError << "ChoosePixelFormat";
		return;
	}
	if(SetPixelFormat((_window.hDC), pf, &pfd) == FALSE){
		logError << "SetPixelFormat";
		return;
	} 
	DescribePixelFormat((_window.hDC), pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	_glContext = wglCreateContext((_window.hDC));
	if(!_glContext){
		logError << "wglCreateContext";
		return;
	}
	makeCurrentContext();

	logInfo << "Sucsses init device with main window";
	loadExtensions();
}

BaseOSDevice::~BaseOSDevice()
{
	if(_glContext){
		wglMakeCurrent(0, 0);
		wglDeleteContext(_glContext);
		_glContext = 0;
	}

	if(_externalCreatedWindow){
		_window.hDC = 0;
		_window.hWnd = 0;
	}
	else{
		if(_window.hDC){
			ReleaseDC((_window.hWnd), (_window.hDC));
			_window.hDC = 0;
		}

		if(_window.hWnd){
			DestroyWindow((_window.hWnd));
			_window.hWnd = 0;
		}
	}
}

boolean BaseOSDevice::makeCurrentContext()
{
	if(!wglMakeCurrent((_window.hDC), _glContext)){
		logWarning << "wglMakeCurrent";
		return false;
	}
	return true;
}

boolean BaseOSDevice::setSyncInterval(uint synk)
{
	if(wglSwapIntervalEXT){
		wglSwapIntervalEXT(synk);
		return true;
	}
	return false;
}

BaseOSFrameWindow::BaseOSFrameWindow(BaseOSDevice& dev, boolean isMainWindowHack) : IFrameWindow(dev), _osdevice(dev)
{
	if(isMainWindowHack)
	{
		_window = _osdevice.window();
		_glContext = _osdevice.glContext();
		_noDeleteContextOnDeestroy = true;

		RECT r;
		GetClientRect((_window.hWnd), &r);
		_size = Vector2i(r.right - r.left, r.bottom - r.top);
	}
	else
	{
		_glContext = 0;
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
	if(!_noDeleteContextOnDeestroy && _glContext){
		_osdevice.makeCurrentContext();
		wglDeleteContext(_glContext);
		_glContext = 0;
	}
	_glContext = 0;
	_window.hDC = 0;
	_window.hWnd = 0;
	_size.set(0,0);
}

boolean BaseOSFrameWindow::init(const WindowHandle& wnd, boolean fullscrean, boolean stereo)
{
	deinit();

	_window = wnd;

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	int pf = ChoosePixelFormat((_window.hDC), &pfd);
	if(pf == 0){
		logError << "ChoosePixelFormat";
		return false;
	}
	if(SetPixelFormat((_window.hDC), pf, &pfd) == FALSE){
		logError << "SetPixelFormat";
		deinit();
		return false;
	} 
	DescribePixelFormat((_window.hDC), pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	_glContext = wglCreateContext((_window.hDC));
	if(!_glContext){
		logError << "wglCreateContext";
		deinit();
		return false;
	}

	if(!wglShareLists(_osdevice.glContext(), _glContext)){
		logError << "wglShareLists";
		deinit();
		return false;
	}

	logInfo << "Initialization OpenGL window - OK";

	RECT r;
	GetClientRect((_window.hWnd), &r);
	_size = Vector2i(r.right - r.left, r.bottom - r.top);
	makeCurrentContext();
	glViewport(0, 0, _size.x, _size.y);
	_osdevice.makeCurrentContext();
	return true;
}

boolean BaseOSFrameWindow::makeCurrentContext()
{
	if(!wglMakeCurrent((_window.hDC), _glContext)){
		logWarning << "wglMakeCurrent";
		return false;
	}
	return true;
}

void BaseOSFrameWindow::resize()
{
	if(!isValid())
		return;
	makeCurrentContext();
	RECT r;
	GetClientRect((_window.hWnd), &r);
	_size = Vector2i(r.right - r.left, r.bottom - r.top);
	glViewport(0, 0, _size.x, _size.y);
	_osdevice.makeCurrentContext();
}

void BaseOSFrameWindow::resize(uint width, uint height)
{
	if(!isValid())
		return;
	makeCurrentContext();
	_size = Vector2i(width, height);
	glViewport(0, 0, _size.x, _size.y);
	_osdevice.makeCurrentContext();
}

void BaseOSFrameWindow::present()
{
	//_osdevice.setSyncInterval(4);
	SwapBuffers((_window.hDC));
}

}

#endif

