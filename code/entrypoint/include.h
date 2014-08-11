#pragma once

#include "../base/include.h"
#include "../input/events.h"
#include "../filestorage/include.h"
#include "../drawer/include.h"

namespace EntryPoint
{

USING_BASE_LIB

extern void registerDelegates();
extern Base::Delegate<Base::boolean (FileStorageLib::IStorage* packedge, FileStorageLib::IStorage* homedir)> gInitDelegate;
extern Base::Delegate<Base::boolean (Base::ConstRange< Input::EventBase* const> events)> gStepDelegate;
extern Base::Delegate<void ()> gStepFinishDelegate;
extern Base::Delegate<void ()> gDeInitDelegate;

extern float getDensity();
extern Base::boolean setDensity(float f);
extern float getPixelSize();
extern Base::Vector2i getScreenSize();
extern void leaveInput(Base::uint channel);
extern Base::String getDeviceName();
extern void openURL(ConstString url);
extern void openRateUs();
extern Base::String getLanguage();

extern Base::boolean renderInit(DrawerLib::IDevice*& rDevice, DrawerLib::IFrameWindow*& rWindow);
extern Base::boolean renderWindowPrepare(DrawerLib::IFrameWindow* rWindow);
extern Base::boolean renderWindowPresent(DrawerLib::IFrameWindow* rWindow);
extern Base::boolean renderDeInit(DrawerLib::IDevice* rDevice, DrawerLib::IFrameWindow* rWindow);

extern void doPurchase(Base::ConstString item);
extern Base::boolean isPurchased(Base::ConstString item);
extern Base::String getPrice(Base::ConstString item);
extern Base::boolean connectToBillingServerRequest();
extern void restorePurchases();

extern Input::State gInputState;
extern const char* gcGameName;
extern Base::boolean gYUp;

static INLINE void addCharacterToText(Input::State::Text& text, Base::uint ch)
{
	{
		if(ch == '\b')
			return;

		if(text.caret > text.string.size())
			text.caret = text.string.size();
		Base::utf8 utrch = ch;
		text.string.insert(text.caret++, utrch);
	}
}

}
