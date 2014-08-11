#pragma once

#include "../base.h"

#if defined(DRAWER_API_OPENGL_ES2) && defined(__ANDROID__)

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#ifdef __ANDROID__
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03
#endif

#ifdef BASE_CONFIG_COMMENT_LOG_INFO
#define LOAD_EXTENSION(type, name) name = reinterpret_cast<type>(eglGetProcAddress(#name));
#else
#define LOAD_EXTENSION(type, name) name = reinterpret_cast<type>(eglGetProcAddress(#name));\
	if(name)\
	logInfo << #name;\
	else\
	logInfo << "!" << #name;
#endif

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
	void resize(){}
	Vector2i size()const { return _size; }
	boolean isValid()const{ return mainWindow || javaRenderWidnowIniter != 0; }
	WindowHandle handle()const { return _window; }

	boolean makeCurrentContext();
	void present();
private:
	WindowHandle _window;
	BaseOSDevice& _osdevice;
	Vector2i _size;

	boolean mainWindow;
	jobject javaRenderWidnowIniter;
	jmethodID javaInitMethodId;
	jmethodID javaDeInitMethodId;
	jmethodID javaPresentMethodId;
	jmethodID javaMakeCurrentContextMethodId;
};


class BaseOSDevice : public IDevice
{
public:
	BaseOSDevice(const DisplayHandle& display);
	BaseOSDevice(const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo);
	~BaseOSDevice();
	boolean isValid()const { return _window.jSurfaceHolder != 0; }
	boolean setSyncInterval(uint synk=1);
	boolean makeCurrentContext();

protected:
	INLINE WindowHandle window()const { return _window; }
	friend class BaseOSFrameWindow;

	JNIEnv* javaENV()const { return _window.jENV; }
	jobject javaRenderIniter()const { return _window.jRenderIniter; }
	jmethodID getJavaNewWindowIniterMethodId()const { return javaNewWindowIniterMethodId; }
	jmethodID getJavaDeleteWindowIniterMethodId()const { return javaDeleteWindowIniterMethodId; }
	
private:
	WindowHandle _window;

	jmethodID javaInitMethodId;
	jmethodID javaInitWithMainWindowMethodId;
	jmethodID javaDeInitMethodId;
	jmethodID javaMakeCurrentContextMethodId;
	jmethodID javaSetSyncIntervalMethodId;
	jmethodID javaNewWindowIniterMethodId;
	jmethodID javaDeleteWindowIniterMethodId;
	void initFields(const DisplayHandle& display);
};

}

#endif
