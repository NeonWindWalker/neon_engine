#include "include.h"

#if defined(__APPLE__)

#include "../drawer/platformspecific.h"
#include <stdio.h>
#include "../reflection/include.h"

#include <sys/types.h>
#include <sys/sysctl.h>

extern "C" void MyMainThread_onLogCallback(const char* str);
extern "C" float MyRenderView_getDensity(void* slf);
extern "C" void MyRenderView_setDensity(void* slf, float d);
extern "C" void MyRenderView_getScreenSize(int* w, int* h, void* slf);
extern "C" float MyRenderView_getPixelSize(void* slf);
extern "C" void MyObjCWrap_openURL(const char* str);
extern "C" void MyObjCWrap_openRateUs();
extern "C" void MyObjCWrap_doPurchase(const char* str);
extern "C" void EntryPoint_setPurchased(const char* str);
extern "C" void EntryPoint_setPrice(const char* str, const char* price);
extern "C" bool MyObjCWrap_connectToBillingServerRequest();
extern "C" void MyObjCWrap_restorePurchases();
extern "C" void MyObjCWrap_getLanguage(char* lang);

namespace EntryPoint
{
    Base::Delegate<Base::boolean (FileStorageLib::IStorage* packedge, FileStorageLib::IStorage* homedir)> gInitDelegate;
    Base::Delegate<Base::boolean (Base::ConstRange< Input::EventBase* const> events)> gStepDelegate;
    Base::Delegate<void ()> gStepFinishDelegate;
    Base::Delegate<void ()> gDeInitDelegate;
}

void* gRenderIniter;
void* gMyRenderView;
FileStorageLib::IStorage* gReadFileStorage;
FileStorageLib::IStorage* gHomeFileStorage;

Base::String gLogPath;
Base::Array<Input::EventBase*> gEvents;
Base::Mutex* gEventsMutex;

struct TouchInfo
{
	Base::uint id;
	Base::Vector2f pos;
};
Base::HashArray< Base::Pair<Base::uint, TouchInfo> > gTouchEventsRemaping;


class LogListener
{
public:
	void onLog(Base::CodePoint codePoint, Base::ConstString trace, Base::ConstString ch, Base::ConstString msg)
	{
		Base::String str;
		Base::LogPrinter::genStandardFormatMessage(str, codePoint, trace, ch, msg);
		str << _CS("\n");
        
        MyMainThread_onLogCallback(str.c_str());
		
        if(!gLogPath.empty())
        {
            FILE* file = fopen(gLogPath.c_str(), "at");
            if(file){
                fprintf(file, "%s", str.c_str());
                fclose(file);
            }
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

extern "C" bool EntryPoint_initLog(const char* path)
{
    if(!gLogListener)
		gLogListener = new LogListener();
    
    gLogPath = Base::constString(path);
	FILE* file = fopen(gLogPath.c_str(), "wt");
	if(!file){
		//__android_log_print(ANDROID_LOG_ERROR, "NativeLib", (Base::String() << _CS("cant create log file ") << gLogPath).c_str());
		return false;
	}
	fclose(file);
	logInfo << "log file created " << gLogPath;
    return true;
}

extern "C" bool EntryPoint_init(const char* storagePath, const char* homeDirPath, void* renderIniter, void* myRenderView)
{
    ReflectionLib::initReflectionLibrary();
    
    gReadFileStorage = FileStorageLib::openStorage(Base::String() << Base::constString(storagePath) << _CS("/data/"));
    gHomeFileStorage = FileStorageLib::openStorage(Base::constString(homeDirPath));
    gRenderIniter = renderIniter;
    gMyRenderView = myRenderView;
    gEventsMutex = new Base::Mutex();
    
    EntryPoint::registerDelegates();
    
    if(!EntryPoint::gInitDelegate(gReadFileStorage, gHomeFileStorage)){
		logError << "gInitDelegate return false, do SHUTDOWN";
		return false;
	}
	logInfo << "Succses init game";
    
	gEventsMutex->lock();
	gTouchEventsRemaping.clear();
	for(Base::uint i=0; i < gEvents.size(); ++i)
		delete gEvents[i];
	gEvents.clear();
	gEventsMutex->unlock();

    return false;
}

extern "C" void EntryPoint_deinit()
{
    EntryPoint::gDeInitDelegate();

	gEventsMutex->lock();
	gTouchEventsRemaping.clear();
	for(Base::uint i=0; i < gEvents.size(); ++i)
		delete gEvents[i];
	gEvents.clear();
	gEventsMutex->unlock();

    delete gEventsMutex;
    
    ReflectionLib::deinitReflectionLibrary();
}

extern "C" bool EntryPoint_step()
{
    gEventsMutex->lock();
	Base::Array<Input::EventBase*> eventsReplyc = gEvents;
	gEvents.clear();
	gEventsMutex->unlock();
	
	Base::boolean b = EntryPoint::gStepDelegate(Base::ConstRange< Input::EventBase* const>(eventsReplyc.first(), eventsReplyc.size()));
	
	for(Base::uint i=0; i < eventsReplyc.size(); ++i)
		delete eventsReplyc[i];
	eventsReplyc.clear();
	
	return b;
}

extern "C" void EntryPoint_stepFinish()
{
    EntryPoint::gStepFinishDelegate();

	gEventsMutex->lock();
	gTouchEventsRemaping.clear();
	for(Base::uint i=0; i < gEvents.size(); ++i)
		delete gEvents[i];
	gEvents.clear();
	gEventsMutex->unlock();
}

extern "C" void EntryPoint_onTouch(unsigned int id, float x, float y, bool up, bool down)
{
	Base::LockGuard lg(*gEventsMutex);

    Input::EventTouch* e = new Input::EventTouch();
    
	e->up = up;
	e->down = down;
	e->x = x;
	e->y = y;
    
    Base::boolean bNew;
    auto it = gTouchEventsRemaping.insert(bNew, (Base::uint)id);
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
        e->dx = e->x - it->val.pos.x;
        e->dy = e->y - it->val.pos.y;
    }
    it->val.pos.x = e->x;
    it->val.pos.y = e->y;
    
    e->channel = it->val.id;
    Assert(e->channel < Input::EventBase::gMaxChannels);
    
    if(e->up)
        gTouchEventsRemaping.erase(it);
    
	gEvents.push(e);
}


namespace EntryPoint
{
    Base::boolean renderInit(DrawerLib::IDevice*& rDevice, DrawerLib::IFrameWindow*& rWindow)
    {
        DrawerLib::DisplayHandle displayHandle(gRenderIniter);
        rDevice = DrawerLib::createDevice(displayHandle);
        if(!rDevice)
            return false;
        rWindow = rDevice->createFrameWindow();
        DrawerLib::WindowHandle windowHandle(gRenderIniter, gMyRenderView);
        if(!rWindow->init(windowHandle)){
            delete rDevice;
            return false;
        }
        return true;
    }
    
    Base::boolean renderWindowPrepare(DrawerLib::IFrameWindow* rWindow)
    {
        rWindow->resize();
        return true;
    }
    
    Base::boolean renderWindowPresent(DrawerLib::IFrameWindow* rWindow)
    {
        rWindow->present();
        return true;
    }
    
    Base::boolean renderDeInit(DrawerLib::IDevice* rDevice, DrawerLib::IFrameWindow* rWindow)
    {
        rWindow->deinit();
        delete rDevice;
        return true;
    }
    
    float getDensity()
    {
        return MyRenderView_getDensity(gMyRenderView);
    }
    
    Base::boolean setDensity(float f)
    {
        MyRenderView_setDensity(gMyRenderView, f);
        return true;
    }
    
    float getPixelSize()
    {
        return MyRenderView_getPixelSize(gMyRenderView);
    }
    
    Base::Vector2i getScreenSize()
    {
        Base::Vector2i v;
        MyRenderView_getScreenSize(&v.x, &v.y, gMyRenderView);
        return v;
    }
    
    void leaveInput(Base::uint channel)
    {
    }

	String getDeviceName()
	{
        size_t size = 1024;
        String str;
		str.resize(size);
		str[0] = 0;
        int name[] = {CTL_HW,HW_MACHINE};
        sysctl(name, 2, str.first(), &size, NULL, 0);
        str.resize(size);
		return str;
	}
    
    String getLanguage()
	{
        String str;
		str.resize(32);
        MyObjCWrap_getLanguage((char*)str.first());
        str.resizeToCStr();
		return str;
	}
    
    void MyObjCWrap_getLanguage(char* lang);

	void openURL(Base::ConstString url)
	{
		MyObjCWrap_openURL(Base::String(url).c_str());
	}

	void openRateUs()
	{
		MyObjCWrap_openRateUs();
	}

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
    
    
    void doPurchase(Base::ConstString item)
    {
        MyObjCWrap_doPurchase(String(item).c_str());
    }
    
    Base::boolean isPurchased(Base::ConstString item)
    {
        Base::LockGuard lg(gPurchashableItemsMutex);
        auto it = gPurchashableItems.find(item);
        if(it == gPurchashableItems.end())
            return false;
        return it->val.purchased;
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
        return MyObjCWrap_connectToBillingServerRequest();
    }

	void restorePurchases()
	{
		return MyObjCWrap_restorePurchases();
	}
}


extern "C" void EntryPoint_setPurchased(const char* str)
{
    logInfo << str << _CS(" purchased");
    Base::LockGuard lg(EntryPoint::gPurchashableItemsMutex);
    Base::boolean bNew;
    EntryPoint::gPurchashableItems.insert(bNew, Base::constString(str))->val.purchased = true;
}

extern "C" void EntryPoint_setPrice(const char* str, const char* price)
{
    logInfo << str << _CS(" price ") << price;
    Base::LockGuard lg(EntryPoint::gPurchashableItemsMutex);
    Base::boolean bNew;
    EntryPoint::gPurchashableItems.insert(bNew, Base::constString(str))->val.price = Base::constString(price);
}

#endif

