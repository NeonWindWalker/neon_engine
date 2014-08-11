#pragma once

#include "../base.h"

#if defined(DRAWER_API_OPENGL_ES2) && defined(__APPLE__)

//#include <EGL/egl.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#ifdef __ANDROID__
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03
#endif

#define LOAD_EXTENSION(type, name) name = 0;

namespace DrawerLib
{

class BaseOSDevice;

class BaseOSFrameWindow : public IFrameWindow
{
public:
	BaseOSFrameWindow(BaseOSDevice& dev, boolean isMainWindowHack = false);
	~BaseOSFrameWindow();
	boolean init(const WindowHandle& wnd, boolean fullscrean = false, boolean stereo = false);
	void deinit();
	void resize();
	Vector2i size()const;
	boolean isValid()const { return _glFBO != 0; }
	WindowHandle handle()const { return _window; }
    boolean makeCurrentContext() { return true; }
    GLuint getFBO() { return _glFBO; }

	void present();
private:
	WindowHandle _window;
	BaseOSDevice& _osdevice;
	void* renderWindowIniter;
    GLuint _glFBO;
};


class BaseOSDevice : public IDevice
{
public:
	BaseOSDevice(const DisplayHandle& display);
	BaseOSDevice(const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo);
	~BaseOSDevice();
	boolean isValid()const { return _bIsValid; }
	boolean setSyncInterval(uint synk);
	boolean makeCurrentContext() { return true; }

protected:
	INLINE WindowHandle window()const { return _window; }
	friend class BaseOSFrameWindow;
	
private:
	WindowHandle _window;
    boolean _bIsValid;
};

}

#endif
