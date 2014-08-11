#ifdef __ANDROID__

#include "include.h"
#include "../filestorage/myjni.h"
#include "../drawer/platformspecific.h"
#include "../reflection/include.h"
#include <android/log.h>
#include <stdio.h>

//FIXME
#include "../audio/tests/test.h" 

namespace AudioLib
{
	extern void testBegin();
}

Base::String gLogPath;
Base::Array<Input::EventBase*> gEvents;
Base::Mutex gEventsMutex;

struct PurchasableItemInfo
{
	Base::boolean purchased;
	Base::String price;

	PurchasableItemInfo()
	{
		purchased = false;
	}
};

Base::HashArray<Base::Pair<Base::String, PurchasableItemInfo> > gPurchashableItems;
Base::Mutex gPurchashableItemsMutex;

jobject gJavaMyNativeLib = 0;

JavaVM* gMainJavaVirtualMachine = 0;
JavaVM* gJavaVM = 0;
jobject gAssetReader = 0;
DrawerLib::WindowHandle renderWindowHandle;
jobject renderWindowNewSurfaceHolder = 0;
int renderWindowNewSurfaceHolderRealWidth = 0;
int renderWindowNewSurfaceHolderRealHeight = 0;
bool renderWindowNewSurfaceHolderChanged = false;
Base::Mutex gRenderWindowMutex;

FileStorageLib::IStorage* gFileSystemPackedgeStorage = 0;
FileStorageLib::IStorage* gFileSystemHomeStorage = 0;

Base::HashMap< Base::Pair<int, Base::Vector2f> >* gTouchPositionHistory = 0;

volatile Base::boolean gConnectedToBillingServer = false;

namespace EntryPoint
{
Base::Delegate<Base::boolean (FileStorageLib::IStorage* packedge, FileStorageLib::IStorage* homedir)> gInitDelegate;
Base::Delegate<Base::boolean (Base::ConstRange< Input::EventBase* const> events)> gStepDelegate;
Base::Delegate<void ()> gStepFinishDelegate;
Base::Delegate<void ()> gDeInitDelegate;

float gPixelSize;
Base::boolean gYUp = true;

void leaveInput(Base::uint channel)
{

}

void doPurchase(Base::ConstString item)
{
	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "doPurchase", "(Ljava/lang/String;)V");
	if(!methodId)
		logError << "cant find doPurchase method in gJavaMyNativeLib java class";

	javaENV->CallVoidMethod(gJavaMyNativeLib, methodId, jStringFromString(javaENV, item)); // may small mem leak ?
}

Base::boolean isPurchased(Base::ConstString item)
{
	Base::LockGuard lg(gPurchashableItemsMutex);
	auto it = gPurchashableItems.find(item);
	if(it != gPurchashableItems.end())
		return it->val.purchased;
	return false;
}

Base::String getPrice(Base::ConstString item)
{
	Base::LockGuard lg(gPurchashableItemsMutex);
	auto it = gPurchashableItems.find(item);
	if(it == gPurchashableItems.end())
		return _CS("");

	return it->val.price;
}

Base::boolean connectToBillingServerRequest()
{
	if(::gConnectedToBillingServer)
		return true;

	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "connectToBillingServerRequest", "()V");
	if(!methodId)
		logError << "cant find connectToBillingServerRequest method in gJavaMyNativeLib java class";

	javaENV->CallVoidMethod(gJavaMyNativeLib, methodId);

	return false;
}

void restorePurchases()
{
	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "restorePurchases", "()V");
	if(!methodId)
		logError << "cant find restorePurchases method in gJavaMyNativeLib java class";

	javaENV->CallVoidMethod(gJavaMyNativeLib, methodId);
}

}

class LogListener
{
public:
	void onLog(Base::CodePoint codePoint, Base::ConstString trace, Base::ConstString ch, Base::ConstString msg)
	{
		Base::String str;
		Base::LogPrinter::genStandardFormatMessage(str, codePoint, trace, ch, msg);
		str << _CS("\n");
		
		if(ch == _CS("error"))
			__android_log_print(ANDROID_LOG_ERROR, "NativeLib", "%s", str.c_str());
		else if(ch == _CS("warning"))
			__android_log_print(ANDROID_LOG_WARN, "NativeLib", "%s", str.c_str());
		else
			__android_log_print(ANDROID_LOG_INFO, "NativeLib", "%s", str.c_str());
		
		FILE* file = fopen(gLogPath.c_str(), "at");
		if(file){
			fprintf(file, "%s", str.c_str());
			fclose(file);
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

LogListener* gLogListener = 0;

extern "C" JNIEXPORT void JNICALL JNICALL Java_ru_devgame_NativeLib_setLogPath(JNIEnv * env, jobject obj, jstring path)
{
	env->GetJavaVM(&gJavaVM);
	gMainJavaVirtualMachine = gJavaVM;
	
	gLogPath = stringFromJString(env, path);
	FILE* file = fopen(gLogPath.c_str(), "wt");
	if(!file){
		__android_log_print(ANDROID_LOG_ERROR, "NativeLib", "%s", (Base::String() << _CS("cant create log file ") << gLogPath).c_str());
		return;
	}
	fclose(file);
	
	if(!gLogListener)
		gLogListener = new LogListener();
		
	logInfo << "log file created " << gLogPath;
}

extern "C" JNIEXPORT jboolean JNICALL JNICALL Java_ru_devgame_NativeLib_init(JNIEnv* env, jobject obj, jobject renderIniter, jobject assetReader, jstring packDir, jstring homeDir, jfloat pixelSize)
{
	EntryPoint::gPixelSize = pixelSize;
	env->GetJavaVM(&gJavaVM);
	gMainJavaVirtualMachine = gJavaVM;
	gJavaMyNativeLib = env->NewGlobalRef(obj);

	ReflectionLib::initReflectionLibrary();

	renderIniter = env->NewGlobalRef(renderIniter);
	//mainRenderSurface = env->NewGlobalRef(mainRenderSurface);
	gAssetReader = env->NewGlobalRef(assetReader);

	Base::logCompileInfo();
	
	gTouchPositionHistory = new Base::HashMap< Base::Pair<int, Base::Vector2f> >();

	logDebug << "Starting init game";
	
	Base::String packDirStr = stringFromJString(env, packDir);
	if(packDirStr.empty())
	{
		logInfo << "openAssetStorage";
		gFileSystemPackedgeStorage = FileStorageLib::openAssetStorage(gJavaVM, gAssetReader);
		if(!gFileSystemPackedgeStorage){
			logError << "openAssetStorage";
			return false;
		}
	}
	else
	{
		logInfo << "openStorage " << packDirStr;
		gFileSystemPackedgeStorage = FileStorageLib::openStorage(packDirStr);
		if(!gFileSystemPackedgeStorage){
			logError << "openStorage " << packDirStr;
			return false;
		}
	}
	
	Base::String homeDirStr = stringFromJString(env, homeDir);
	logInfo << "openStorage " << homeDirStr;
	gFileSystemHomeStorage = FileStorageLib::openStorage(homeDirStr);
	if(!gFileSystemHomeStorage){
		logError << "openStorage " << homeDirStr;
		return false;
	}

	//restore purchases
	Base::BinaryData purchasesSaveData;
	gFileSystemHomeStorage->read(purchasesSaveData, _CS("purchases.n"));
	Base::ConstString purchasesSaveStr((const Base::utf8*)purchasesSaveData.first(), purchasesSaveData.byteSize());
	gPurchashableItemsMutex.lock();
	while(true)
	{
		const Base::utf8* nl = purchasesSaveStr.find(Base::utf8('\n'));
		if(nl != purchasesSaveStr.end())
		{
			Base::ConstString name(purchasesSaveStr.first(), nl);
			Base::boolean bNew;
			auto it = gPurchashableItems.insert(bNew, name);
			it->val.purchased = true;

			purchasesSaveStr = Base::ConstString(nl + 1, purchasesSaveStr.end());
		}
		else
		{
			break;
		}
	}
	gPurchashableItemsMutex.unlock();

	EntryPoint::registerDelegates();

	while(true)
	{
		gRenderWindowMutex.lock();

		if(renderWindowNewSurfaceHolder)
		{
			renderWindowHandle.jRenderIniter = renderIniter;
			renderWindowHandle.jSurfaceHolder = env->NewGlobalRef(renderWindowNewSurfaceHolder);
			gRenderWindowMutex.unlock();

			if(!EntryPoint::gInitDelegate(gFileSystemPackedgeStorage, gFileSystemHomeStorage)){
				logError << "gInitDelegate return false, do SHUTDOWN";
				return false;
			}
			logInfo << "Succses init game";
			break;
		}
		else
			gRenderWindowMutex.unlock();
		Base::Thread::yield();
	}

	gEventsMutex.lock();
	gTouchPositionHistory->clear();
	for(uint i=0; i < gEvents.size(); ++i)
		delete gEvents[i];
	gEvents.clear();
	gEventsMutex.unlock();
	
	//FIXME
	//AudioLib::testBegin();

	return true;
}

void savePurchases()
{
	Base::LockGuard lg(gPurchashableItemsMutex);
	if(!gFileSystemHomeStorage)
		return;
	Base::String purchasesSaveStr;
	for(uint i=0; i < gPurchashableItems.size(); ++i)
		if(gPurchashableItems[i].val.purchased)
			purchasesSaveStr << gPurchashableItems[i].key << _CS("\n");
	gFileSystemHomeStorage->write(_CS("purchases.n"), Base::ConstBinaryData(purchasesSaveStr.first(), purchasesSaveStr.size()));
}

extern "C" JNIEXPORT void JNICALL Java_ru_devgame_NativeLib_deinit(JNIEnv * env, jobject obj)
{
	savePurchases();

	logInfo << "Deinit game";
	EntryPoint::gDeInitDelegate();
	gPurchashableItemsMutex.lock();
	delete gFileSystemPackedgeStorage;
	delete gFileSystemHomeStorage;
	gFileSystemPackedgeStorage = 0;
	gFileSystemHomeStorage = 0;
	gPurchashableItemsMutex.unlock();
	ReflectionLib::deinitReflectionLibrary();
	env->DeleteGlobalRef(renderWindowHandle.jRenderIniter);
	env->DeleteGlobalRef(renderWindowHandle.jSurfaceHolder);
	env->DeleteGlobalRef(gAssetReader);
	env->DeleteGlobalRef(gJavaMyNativeLib);
	
	delete gTouchPositionHistory;
	gEvents.clear();

	if(!gLogListener){
		delete gLogListener;
		gLogListener = 0;
	}

	gPurchashableItemsMutex.lock();
	gPurchashableItems.clear();
	gPurchashableItemsMutex.unlock();
}

extern "C" JNIEXPORT jboolean JNICALL Java_ru_devgame_NativeLib_step(JNIEnv * env, jobject obj)
{
	gEventsMutex.lock();
	Base::Array<Input::EventBase*> eventsReplyc = gEvents;
	gEvents.clear();
	gEventsMutex.unlock();
	
	Base::boolean b = EntryPoint::gStepDelegate(Base::ConstRange< Input::EventBase* const>(eventsReplyc.first(), eventsReplyc.size()));
	
	for(uint i=0; i < eventsReplyc.size(); ++i)
		delete eventsReplyc[i];
	eventsReplyc.clear();
	
	return b;
}

extern "C" JNIEXPORT void JNICALL Java_ru_devgame_NativeLib_stepFinish(JNIEnv * env, jobject obj)
{
	EntryPoint::gStepFinishDelegate();
	gEventsMutex.lock();
	gTouchPositionHistory->clear();
	for(uint i=0; i < gEvents.size(); ++i)
		delete gEvents[i];
	gEvents.clear();
	gEventsMutex.unlock();
}



extern "C" JNIEXPORT void JNICALL Java_ru_devgame_NativeLib_onTouch(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jfloat width, jfloat height, jfloat pressure, jfloat size, jboolean down, jboolean up)
{
	Base::LockGuard lg(gEventsMutex);

	Input::EventTouch* e = new Input::EventTouch();

	e->channel = id;
	e->up = up;
	e->down = down;
	e->x = x;
	e->y = EntryPoint::gYUp ? height - 1.0f - y : y;
	e->pressure = pressure;
	e->size = size;
	
	Base::boolean bIns;
	auto it = gTouchPositionHistory->insert(bIns, id);
	if(!bIns)
	{
	  e->dx = e->x - it->val.x;
	  e->dy = e->y - it->val.y;
	}
	
	it->val = Base::Vector2f(x, y);
	
	if(up)
	  gTouchPositionHistory->erase(id);

	gEvents.push(e);
}

extern "C" JNIEXPORT void JNICALL Java_ru_devgame_NativeLib_onAccelerometer(JNIEnv * env, jobject obj, jint id, jfloat x, jfloat y, jfloat z)
{
	EntryPoint::gInputState.acceleration.set(x, y, z);
}

extern "C" JNIEXPORT void JNICALL Java_ru_devgame_NativeLib_renderWindowChanged(JNIEnv * env, jobject obj, jobject mainRenderSurface, jint width, jint height)
{
	Base::LockGuard lg(gRenderWindowMutex);

	if(renderWindowNewSurfaceHolder){
		env->DeleteGlobalRef(renderWindowNewSurfaceHolder);
		renderWindowNewSurfaceHolder = 0;
	}
	if(mainRenderSurface)
		renderWindowNewSurfaceHolder = env->NewGlobalRef(mainRenderSurface);
	renderWindowHandle.width = width;
	renderWindowHandle.height = height;
	renderWindowNewSurfaceHolderChanged = true;
}

Base::boolean EntryPoint::renderInit(DrawerLib::IDevice*& rDevice, DrawerLib::IFrameWindow*& rWindow)
{
	logDebug << "EntryPoint::renderInit ";
	renderWindowHandle.jENV = getThreadJavaEnviroment(gJavaVM);

	rDevice = DrawerLib::createDeviceWithMainWindow(rWindow, renderWindowHandle, true, false);
	if(!rDevice)
		logError << "cant init render";
	return rDevice != 0;
}

Base::boolean EntryPoint::renderWindowPrepare(DrawerLib::IFrameWindow* rWindow)
{
	Base::LockGuard lg(gRenderWindowMutex);

	if(!renderWindowNewSurfaceHolderChanged)
		return renderWindowNewSurfaceHolder != 0;

	if(renderWindowNewSurfaceHolder == 0)
		return false;

	logDebug << "reinit main window";
	getThreadJavaEnviroment(gJavaVM)->DeleteGlobalRef(renderWindowHandle.jSurfaceHolder);
	renderWindowHandle.jSurfaceHolder = getThreadJavaEnviroment(gJavaVM)->NewGlobalRef(renderWindowNewSurfaceHolder);
	Base::boolean ret = rWindow->init(renderWindowHandle, true, false);
	if(ret)
		renderWindowNewSurfaceHolderChanged = false;
	return ret;	
}

Base::boolean EntryPoint::renderWindowPresent(DrawerLib::IFrameWindow* rWindow)
{
    rWindow->present();
	return true;
}

Base::boolean EntryPoint::renderDeInit(DrawerLib::IDevice* rDevice, DrawerLib::IFrameWindow* rWindow)
{
	//rWindow->release();
	delete rDevice;
	return true;
}

float gDensity = 1.0f;
float EntryPoint::getDensity()
{
	return gDensity;
}

Base::boolean EntryPoint::setDensity(float f)
{
	gDensity = f;

	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "setDensity", "(F)V");
	if(!methodId)
		logError << "cant find setDensity method in gJavaMyNativeLib java class";

	javaENV->CallVoidMethod(gJavaMyNativeLib, methodId, gDensity);
	return true;
}

float EntryPoint::getPixelSize()
{
	return gPixelSize / gDensity;
}

Base::Vector2i EntryPoint::getScreenSize()
{
	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "getScreenSize", "()I");
	if(!methodId)
		logError << "cant find getScreenSize method in gJavaMyNativeLib java class";

	Base::uint wh = javaENV->CallIntMethod(gJavaMyNativeLib, methodId);

	return Base::Vector2i(wh & 65535, wh >> 16);
}

Base::String EntryPoint::getDeviceName()
{
	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "getDeviceName", "()Ljava/lang/String;");
	if(!methodId)
		logError << "cant find getDeviceName ()Ljava/lang/String; method in gJavaMyNativeLib java class";

	jstring str = (jstring)javaENV->CallObjectMethod(gJavaMyNativeLib, methodId);
	Base::String mystr = stringFromJString(javaENV, str);
	javaENV->DeleteLocalRef(str);

	return mystr;
}

void EntryPoint::openURL(ConstString url)
{
	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "openURL", "(Ljava/lang/String;)V");
	if(!methodId)
		logError << "cant find openURL (Ljava/lang/String;)V method in gJavaMyNativeLib java class";

	javaENV->CallVoidMethod(gJavaMyNativeLib, methodId, jStringFromString(javaENV, url));
}

void EntryPoint::openRateUs()
{
	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "openRateUs", "()V");
	if(!methodId)
		logError << "cant find openRateUs ()V method in gJavaMyNativeLib java class";

	javaENV->CallVoidMethod(gJavaMyNativeLib, methodId);
}


Base::String EntryPoint::getLanguage()
{
	JNIEnv* javaENV = getThreadJavaEnviroment(gJavaVM);

	jclass cls = javaENV->GetObjectClass(gJavaMyNativeLib);
	if(!cls)
		logError << "cant find gJavaMyNativeLib java class";

	jmethodID methodId = javaENV->GetMethodID(cls, "getLanguage", "()Ljava/lang/String;");
	if(!methodId)
		logError << "cant find getLanguage ()Ljava/lang/String; method in gJavaMyNativeLib java class";

	jstring str = (jstring)javaENV->CallObjectMethod(gJavaMyNativeLib, methodId);
	Base::String mystr = stringFromJString(javaENV, str);
	javaENV->DeleteLocalRef(str);

	return mystr;
}

enum EAndroidKeyEventKey
{
// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_UNKNOWN = 0,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SOFT_LEFT = 1,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SOFT_RIGHT = 2,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_HOME = 3,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BACK = 4,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_CALL = 5,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_ENDCALL = 6,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_0 = 7,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_1 = 8,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_2 = 9,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_3 = 10,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_4 = 11,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_5 = 12,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_6 = 13,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_7 = 14,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_8 = 15,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_9 = 16,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_STAR = 17,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_POUND = 18,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_DPAD_UP = 19,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_DPAD_DOWN = 20,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_DPAD_LEFT = 21,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_DPAD_RIGHT = 22,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_DPAD_CENTER = 23,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_VOLUME_UP = 24,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_VOLUME_DOWN = 25,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_POWER = 26,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_CAMERA = 27,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_CLEAR = 28,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_A = 29,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_B = 30,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_C = 31,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_D = 32,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_E = 33,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_F = 34,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_G = 35,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_H = 36,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_I = 37,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_J = 38,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_K = 39,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_L = 40,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_M = 41,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_N = 42,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_O = 43,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_P = 44,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_Q = 45,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_R = 46,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_S = 47,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_T = 48,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_U = 49,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_V = 50,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_W = 51,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_X = 52,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_Y = 53,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_Z = 54,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_COMMA = 55,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_PERIOD = 56,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_ALT_LEFT = 57,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_ALT_RIGHT = 58,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SHIFT_LEFT = 59,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SHIFT_RIGHT = 60,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_TAB = 61,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SPACE = 62,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SYM = 63,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_EXPLORER = 64,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_ENVELOPE = 65,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_ENTER = 66,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_DEL = 67,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_GRAVE = 68,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MINUS = 69,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_EQUALS = 70,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_LEFT_BRACKET = 71,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_RIGHT_BRACKET = 72,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BACKSLASH = 73,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SEMICOLON = 74,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_APOSTROPHE = 75,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SLASH = 76,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_AT = 77,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_NUM = 78,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_HEADSETHOOK = 79,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_FOCUS = 80,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_PLUS = 81,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MENU = 82,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_NOTIFICATION = 83,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SEARCH = 84,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MEDIA_PLAY_PAUSE = 85,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MEDIA_STOP = 86,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MEDIA_NEXT = 87,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MEDIA_PREVIOUS = 88,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MEDIA_REWIND = 89,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MEDIA_FAST_FORWARD = 90,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_MUTE = 91,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_PAGE_UP = 92,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_PAGE_DOWN = 93,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_PICTSYMBOLS = 94,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_SWITCH_CHARSET = 95,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_A = 96,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_B = 97,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_C = 98,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_X = 99,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_Y = 100,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_Z = 101,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_L1 = 102,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_R1 = 103,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_L2 = 104,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_R2 = 105,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_THUMBL = 106,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_THUMBR = 107,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_START = 108,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_SELECT = 109,

// Field descriptor #15 I
EAndroidKeyEventKey_KEYCODE_BUTTON_MODE = 110,

// Field descriptor #15 I
EAndroidKeyEventKey_MAX_KEYCODE = 84,

// Field descriptor #15 I
EAndroidKeyEventKey_ACTION_DOWN = 0,

// Field descriptor #15 I
EAndroidKeyEventKey_ACTION_UP = 1,

// Field descriptor #15 I
EAndroidKeyEventKey_ACTION_MULTIPLE = 2,

// Field descriptor #15 I
EAndroidKeyEventKey_META_ALT_ON = 2,

// Field descriptor #15 I
EAndroidKeyEventKey_META_ALT_LEFT_ON = 16,

// Field descriptor #15 I
EAndroidKeyEventKey_META_ALT_RIGHT_ON = 32,

// Field descriptor #15 I
EAndroidKeyEventKey_META_SHIFT_ON = 1,

// Field descriptor #15 I
EAndroidKeyEventKey_META_SHIFT_LEFT_ON = 64,

// Field descriptor #15 I
EAndroidKeyEventKey_META_SHIFT_RIGHT_ON = 128,

// Field descriptor #15 I
EAndroidKeyEventKey_META_SYM_ON = 4,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_WOKE_HERE = 1,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_SOFT_KEYBOARD = 2,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_KEEP_TOUCH_MODE = 4,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_FROM_SYSTEM = 8,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_EDITOR_ACTION = 16,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_CANCELED = 32,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_VIRTUAL_HARD_KEY = 64,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_LONG_PRESS = 128,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_CANCELED_LONG_PRESS = 256,

// Field descriptor #15 I
EAndroidKeyEventKey_FLAG_TRACKING = 512

};

extern "C" JNIEXPORT jboolean JNICALL Java_ru_devgame_NativeLib_onKey(JNIEnv * env, jobject obj, jint androidKey, jboolean down, jboolean up)
{
	Input::EKeyType key;
	switch(androidKey)
	{
	case EAndroidKeyEventKey_KEYCODE_BACK:
		key = Input::EKeyType_Back;
		break;
	case EAndroidKeyEventKey_KEYCODE_MENU:
		key = Input::EKeyType_Menu;
		break;
	default:
		return false;
	}

	Input::EventKey* ev = new Input::EventKey();
	ev->up = up;
	ev->down = down;
	ev->key = key;
	gEventsMutex.lock();
	gEvents.push(ev);
	gEventsMutex.unlock();
	return true;
}

extern "C" JNIEXPORT void JNICALL Java_ru_devgame_NativeLib_onItemPurchased(JNIEnv * env, jobject obj, jstring item)
{
	Base::LockGuard lg(gPurchashableItemsMutex);
	Base::boolean bNew;
	auto it = gPurchashableItems.insert(bNew, stringFromJString(env, item));
	Base::boolean oldVal = it->val.purchased;
	it->val.purchased = true;
	if(bNew || (!oldVal))
		savePurchases();
}


extern "C" JNIEXPORT void JNICALL Java_ru_devgame_NativeLib_onItemPurchasableInfo(JNIEnv * env, jobject obj, jstring item, bool purchased, jstring price)
{
	Base::LockGuard lg(gPurchashableItemsMutex);
	Base::boolean bNew;
	auto it = gPurchashableItems.insert(bNew, stringFromJString(env, item));
	Base::boolean oldVal = it->val.purchased;
	it->val.purchased = purchased;
	it->val.price = stringFromJString(env, price);
	if(purchased && !oldVal)
		savePurchases();
}

extern "C" JNIEXPORT void JNICALL Java_ru_devgame_NativeLib_onConnectedToBillingServer(JNIEnv * env, jobject obj)
{
	gConnectedToBillingServer = true;
}

#endif
