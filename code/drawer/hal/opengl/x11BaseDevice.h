#pragma once

#include "../base.h"

#if defined(__linux__) && defined(DRAWER_API_OPENGL) && !defined(__ANDROID__)

#include <X11/Xlib.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glxext.h>

#define LOAD_EXTENSION(type, name) name = reinterpret_cast<type>(glXGetProcAddress((const GLubyte*)#name));\
if(name)\
	logInfo << "%s" << #name;\
else\
	logInfo << "!%s" << #name;\

namespace DrawerLib 
{ 

extern PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI;

class BaseOSDevice;

class BaseOSFrameWindow : public IFrameWindow
{
public:
	BaseOSFrameWindow(BaseOSDevice& dev, boolean isMainWindowHack = false);
	~BaseOSFrameWindow();
	boolean init(const WindowHandle& wnd, boolean fullscrean = false, boolean stereo = false);
	void deinit();
	void resize();
	Vector2i size()const { return _size; }
	WindowHandle handle()const { return _window; }

	boolean isValid()const{ return _glxContext != 0; }
	boolean makeCurrentContext();
	void present();
private:
	WindowHandle _window;
	GLXContext _glxContext;
	BaseOSDevice& _osdevice;
	boolean _noDeleteContextOnDeestroy;
	Vector2i _size;
};


class BaseOSDevice : public IDevice
{
public:
	BaseOSDevice(const DisplayHandle& display);
	BaseOSDevice(const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo);
	~BaseOSDevice();
	boolean isValid()const { return _glxContext != 0; }
	boolean setSyncInterval(uint synk=1);

	boolean makeCurrentContext();

protected:
	INLINE GLXContext glxContext()const { return _glxContext; }
	INLINE WindowHandle window()const { return _window; }
	friend class BaseOSFrameWindow;

private:
	WindowHandle _window;
	GLXContext _glxContext;
	bool _fullscreanWindow;
	bool _externalCreatedWindow;
};

}

#endif
