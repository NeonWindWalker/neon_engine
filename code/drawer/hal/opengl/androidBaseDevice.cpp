#include "androidBaseDevice.h"

#if defined(DRAWER_API_OPENGL_ES2) && defined(__ANDROID__)

namespace DrawerLib
{

void BaseOSDevice::initFields(const DisplayHandle& display)
{
	_window.jENV = display.jENV;
	_window.jRenderIniter = display.jRenderIniter;

	jclass cls = javaENV()->GetObjectClass(javaRenderIniter());
	if(!cls)
		logError << "cant find render initer java class";

	javaInitMethodId = javaENV()->GetMethodID(cls, "init", "()Ljava/lang/Object;");
	if(!javaInitMethodId)
		logError << "cant find init method in render initer java class";

	javaInitWithMainWindowMethodId = javaENV()->GetMethodID(cls, "initWithMainWindow", "(Ljava/lang/Object;IIZZ)Ljava/lang/Object;");
	if(!javaInitWithMainWindowMethodId)
		logError << "cant find initWithMainWindow method in render initer java class";

	javaDeInitMethodId = javaENV()->GetMethodID(cls, "deinit", "()Z");
	if(!javaDeInitMethodId)
		logError << "cant find bool deinit() method in render initer java class";

	javaMakeCurrentContextMethodId = javaENV()->GetMethodID(cls, "makeCurrentContext", "()Z");
	if(!javaMakeCurrentContextMethodId)
		logError << "cant find bool makeCurrentContext() method in render initer java class";

	javaSetSyncIntervalMethodId = javaENV()->GetMethodID(cls, "setSyncIntrval", "(I)Z");
	if(!javaSetSyncIntervalMethodId)
		logError << "cant find bool setSyncIntrval() method in render initer java class";

	javaNewWindowIniterMethodId = javaENV()->GetMethodID(cls, "newWindowIniter", "(Z)Ljava/lang/Object;");
	if(!javaNewWindowIniterMethodId)
		logError << "cant find Object newWindowIniter(boolean) method in render initer java class";

	javaDeleteWindowIniterMethodId = javaENV()->GetMethodID(cls, "deleteWindowIniter", "(Ljava/lang/Object;)Z");
	if(!javaDeleteWindowIniterMethodId)
		logError << "cant find void deleteWindowIniter(Object) method in render initer java class";

	logDebug << "deleting loc ref";

	javaENV()->DeleteLocalRef(cls);
	logDebug << "success";
}

BaseOSDevice::BaseOSDevice(const DisplayHandle& display)
{
	initFields(display);
	jobject ret = javaENV()->CallObjectMethod(javaRenderIniter(), javaInitMethodId);
	_window.jSurfaceHolder = javaENV()->NewGlobalRef(ret);
	javaENV()->DeleteLocalRef(ret);
}

BaseOSDevice::BaseOSDevice(const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo)
{
	initFields(srcWainWindowHandle);
	jobject ret = javaENV()->CallObjectMethod(javaRenderIniter(), javaInitWithMainWindowMethodId, srcWainWindowHandle.jSurfaceHolder, srcWainWindowHandle.width, srcWainWindowHandle.height,  (jboolean)fullscrean, (jboolean)stereo);
	_window.jSurfaceHolder = javaENV()->NewGlobalRef(ret);
	javaENV()->DeleteLocalRef(ret);
}

BaseOSDevice::~BaseOSDevice()
{
	javaENV()->CallBooleanMethod(javaRenderIniter(), javaDeInitMethodId);
	javaENV()->DeleteGlobalRef(_window.jSurfaceHolder);
	_window.jSurfaceHolder = 0;
}

boolean BaseOSDevice::setSyncInterval(uint synk)
{
	return javaENV()->CallBooleanMethod(javaRenderIniter(), javaMakeCurrentContextMethodId, (jint)synk);
}

boolean BaseOSDevice::makeCurrentContext()
{
	return javaENV()->CallBooleanMethod(javaRenderIniter(), javaSetSyncIntervalMethodId);
}


BaseOSFrameWindow::BaseOSFrameWindow(BaseOSDevice& dev, boolean isMainWindowHack) : IFrameWindow(dev), _osdevice(dev)
{
	_window = _osdevice.window();
	mainWindow = isMainWindowHack;
	if(!isMainWindowHack)
		_window.jSurfaceHolder = 0;

	jobject locref = _osdevice.javaENV()->CallObjectMethod(_osdevice.javaRenderIniter(), _osdevice.getJavaNewWindowIniterMethodId(), (jboolean)isMainWindowHack );
	javaRenderWidnowIniter = _osdevice.javaENV()->NewGlobalRef(locref);
	_osdevice.javaENV()->DeleteLocalRef(locref);

	jclass cls = _osdevice.javaENV()->GetObjectClass(javaRenderWidnowIniter);
	if (!cls)
		logError << "cant find render initer java class";

	javaInitMethodId = _osdevice.javaENV()->GetMethodID(cls, "init", "(Ljava/lang/Object;IIZZ)I");
	if (!javaInitMethodId)
		logError << "cant find int init() method in render window initer java class";

	javaDeInitMethodId = _osdevice.javaENV()->GetMethodID(cls, "deinit", "()Z");
	if (!javaDeInitMethodId)
		logError << "cant find bool deinit() method in render window initer java class";

	javaPresentMethodId = _osdevice.javaENV()->GetMethodID(cls, "present", "()I");
	if (!javaPresentMethodId)
		logError << "cant find int present() method in render window initer java class";

	javaMakeCurrentContextMethodId = _osdevice.javaENV()->GetMethodID(cls, "makeCurrentContext", "()Z");
	if (!javaMakeCurrentContextMethodId)
		logError << "cant find bool makeCurrentContext() method in render window initer java class";

	_osdevice.javaENV()->DeleteLocalRef(cls);
}

BaseOSFrameWindow::~BaseOSFrameWindow()
{
	_osdevice.javaENV()->CallBooleanMethod(_osdevice.javaRenderIniter(), _osdevice.getJavaDeleteWindowIniterMethodId(), javaRenderWidnowIniter);
	_osdevice.javaENV()->DeleteGlobalRef(javaRenderWidnowIniter);
	javaRenderWidnowIniter = 0;
}


boolean BaseOSFrameWindow::init(const WindowHandle& wnd, boolean fullscrean, boolean stereo)
{
	_window = wnd;
	uint ret = (uint)_osdevice.javaENV()->CallIntMethod(javaRenderWidnowIniter, javaInitMethodId, wnd.jSurfaceHolder, wnd.width, wnd.height, (jboolean)fullscrean, (jboolean)stereo );
	_size.x = ret & 65535;
	_size.y = (ret>>16) & 65535;
	if(ret == 0)
		_window.jSurfaceHolder = 0; //FIXME GLOBAL REF

	logInfo << "size = " << _size.x << "x" << _size.y;
}

void BaseOSFrameWindow::deinit()
{
	_osdevice.javaENV()->CallBooleanMethod(javaRenderWidnowIniter, javaDeInitMethodId);
	_window.jSurfaceHolder = 0; //FIXME GLOBAL REF
}
	

boolean BaseOSFrameWindow::makeCurrentContext()
{
	return _osdevice.javaENV()->CallBooleanMethod(javaRenderWidnowIniter, javaMakeCurrentContextMethodId);
}

void BaseOSFrameWindow::present()
{
	uint ret = _osdevice.javaENV()->CallIntMethod(javaRenderWidnowIniter, javaPresentMethodId);
	_size.x = ret & 65535;
	_size.y = (ret>>16) & 65535;
}

}

#endif
