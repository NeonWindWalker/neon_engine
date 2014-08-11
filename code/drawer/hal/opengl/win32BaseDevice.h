#pragma once

#include "../base.h"

#if defined(_WIN32) && defined(DRAWER_API_OPENGL)

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef far
#undef near

#include <GL/gl.h>
#include "../../../../../external/wingl/glext.h"
#include "../../../../../external/wingl/wglext.h"

#ifdef BASE_CONFIG_COMMENT_LOG_INFO
#define LOAD_EXTENSION(type, name) name = reinterpret_cast<type>(wglGetProcAddress((LPSTR)#name));
#else
#define LOAD_EXTENSION(type, name) name = reinterpret_cast<type>(wglGetProcAddress((LPSTR)#name));\
	if(name)\
		logInfo << #name;\
	else\
		logInfo << "!" << #name;
#endif

namespace DrawerLib 
{ 

	extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

	class BaseOSDevice;

	class BaseOSFrameWindow : public IFrameWindow
	{
	public:
		BaseOSFrameWindow(BaseOSDevice& dev, boolean isMainWindowHack = false);
		~BaseOSFrameWindow();
		boolean init(const WindowHandle& wnd, boolean fullscrean = false, boolean stereo = false);
		void deinit();
		void resize();
		void resize(uint width, uint height);
		Vector2i size()const { return _size; }
		boolean isValid()const{ return _glContext != 0; }
		WindowHandle handle()const { return _window; }

		boolean makeCurrentContext();
		void present();
	private:
		WindowHandle _window;
		HGLRC _glContext;
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
		boolean isValid()const { return _glContext != 0; }
		boolean setSyncInterval(uint synk=1);

		boolean makeCurrentContext();

	protected:
		INLINE HGLRC glContext()const { return _glContext; }
		INLINE WindowHandle window()const { return _window; }
		friend class BaseOSFrameWindow;

	private:
		WindowHandle _window;
		HGLRC _glContext;
		bool _fullscreanWindow;
		bool _externalCreatedWindow;
	};

}

#endif
