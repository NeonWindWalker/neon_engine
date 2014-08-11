#pragma once

#include "seed.h"

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(__linux__) && !defined(__ANDROID__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#ifdef __ANDROID__
#include <jni.h>
#endif

namespace DrawerLib
{

struct DisplayHandle
{

#if defined(__linux__) && !defined(__ANDROID__)
	::Display* display;
	int	screen;
	DisplayHandle()
	{
		display = 0;
		screen = 0;
	}
	DisplayHandle(::Display* inDisplay, int	inScreen)
	{
		display = inDisplay;
		screen = inScreen;
	}
#endif

#if defined(__ANDROID__)
	JNIEnv* jENV;
	jobject jRenderIniter;
	DisplayHandle()
	{
		jENV = 0;
		jRenderIniter = 0;
	}
	DisplayHandle(JNIEnv* env, jobject initer)
	{
		jENV = env;
		jRenderIniter = initer;
	}
#endif
    
#ifdef __APPLE__
    void* renderIniter;
    
    DisplayHandle()
	{
		renderIniter = 0;
	}
	DisplayHandle(void* initer)
	{
		renderIniter = initer;
	}
#endif

};

struct WindowHandle : public DisplayHandle
{

#ifdef _WIN32
	HWND hWnd;
	HDC hDC;
	WindowHandle()
	{
		hWnd = 0;
		hDC = 0;
	}
#endif

#if defined(__linux__) && !defined(__ANDROID__)
	::Window window;
	WindowHandle()
	{
		window = 0;
	}
	WindowHandle(::Display* inDisplay, int	inScreen, ::Window inWindow)
	{
		display = inDisplay;
		screen = inScreen;
		window = inWindow;
	}
#endif

#if defined(__ANDROID__)
	jobject jSurfaceHolder;
	int width;
	int height;
	WindowHandle()
	{
		jSurfaceHolder = 0;
		width = 0;
		height = 0;
	}
	WindowHandle(JNIEnv* env, jobject initer, jobject surf, int w, int h) : DisplayHandle(env, initer)
	{
		jSurfaceHolder = surf;
		width = w;
		height = h;
	}
#endif
    
#ifdef __APPLE__
    void* myRenderView;
    
    WindowHandle()
	{
		myRenderView = 0;
	}
	WindowHandle(void* initer, void* view)
	{
		renderIniter = initer;
        myRenderView = view;
	}
#endif
};

#if defined(__linux__) && !defined(__ANDROID__)
extern XVisualInfo* getXVisualInfo(Colormap& outColorMap, const DisplayHandle& display);
#endif

class IDevice;

#if !defined(__ANDROID__)
extern IDevice* createDevice(const DisplayHandle& display);
#endif

extern IDevice* createDeviceWithMainWindow(IFrameWindow*& retSingleWindow, const WindowHandle& wnd, boolean fullscrean = false, boolean stereo = false);

}

