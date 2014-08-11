#pragma once

#include "../base/string.h"
#include "../base/vector.h"

namespace Input
{

enum EKeyType
{
	EKeyType_None = 0,
	EKeyType_Escape,
	EKeyType_1,
	EKeyType_2,
	EKeyType_3,
	EKeyType_4,
	EKeyType_5,
	EKeyType_6,
	EKeyType_7,
	EKeyType_8,
	EKeyType_9,
	EKeyType_0,
	EKeyType_Minus,
	EKeyType_Equals,
	EKeyType_Back,
	EKeyType_Tab,
	EKeyType_Q,
	EKeyType_W,
	EKeyType_E,
	EKeyType_R,
	EKeyType_T,
	EKeyType_Y,
	EKeyType_U,
	EKeyType_I,
	EKeyType_O,
	EKeyType_P,
	EKeyType_LBracket,
	EKeyType_RBracket,
	EKeyType_Enter,
	EKeyType_LControl,
	EKeyType_A,
	EKeyType_S,
	EKeyType_D,
	EKeyType_F,
	EKeyType_G,
	EKeyType_H,
	EKeyType_J,
	EKeyType_K,
	EKeyType_L,
	EKeyType_Semicolon,
	EKeyType_Apostrophe,
	EKeyType_Grave,
	EKeyType_LShift,
	EKeyType_BackSlash,
	EKeyType_Z,
	EKeyType_X,
	EKeyType_C,
	EKeyType_V,
	EKeyType_B,
	EKeyType_N,
	EKeyType_M,
	EKeyType_Comma,
	EKeyType_Period,
	EKeyType_Slash,
	EKeyType_RShift,
	EKeyType_Multiply,
	EKeyType_LMenu,
	EKeyType_Space,
	EKeyType_Capital,
	EKeyType_F1,
	EKeyType_F2,
	EKeyType_F3,
	EKeyType_F4,
	EKeyType_F5,
	EKeyType_F6,
	EKeyType_F7,
	EKeyType_F8,
	EKeyType_F9,
	EKeyType_F10,
	EKeyType_NumLock,
	EKeyType_Scroll,
	EKeyType_Numpad7,
	EKeyType_Numpad8,
	EKeyType_Numpad9,
	EKeyType_Substract,
	EKeyType_Numpad4,
	EKeyType_Numpad5,
	EKeyType_Numpad6,
	EKeyType_Add,
	EKeyType_Numpad1,
	EKeyType_Numpad2,
	EKeyType_Numpad3,
	EKeyType_Numpad0,
	EKeyType_Decimal,
	EKeyType_OEM_102,
	EKeyType_F11,
	EKeyType_F12,
	EKeyType_F13,
	EKeyType_F14,
	EKeyType_F15,
	EKeyType_KANA,
	EKeyType_ABNT_C1,
	EKeyType_Convert,
	EKeyType_NoConvert,
	EKeyType_YEN,
	EKeyType_ABNT_C2,
	EKeyType_NumpadEquals,
	EKeyType_PrevTrack,
	EKeyType_AT,
	EKeyType_Colon,
	EKeyType_Underline,
	EKeyType_KANJI,
	EKeyType_Stop,
	EKeyType_AX,
	EKeyType_Unlabeled,
	EKeyType_NextTrack,
	EKeyType_NumpadEnter,
	EKeyType_RControl,
	EKeyType_Mute,
	EKeyType_Calculator,
	EKeyType_PlayPause,
	EKeyType_MediaStop,
	EKeyType_VolumeDown,
	EKeyType_VolumeUp,
	EKeyType_WebHome,
	EKeyType_NumpadComma,
	EKeyType_Divide,
	EKeyType_SysRQ,
	EKeyType_RMenu,
	EKeyType_Pause,
	EKeyType_Home,
	EKeyType_Up,
	EKeyType_Prior,
	EKeyType_Left,
	EKeyType_Right,
	EKeyType_End,
	EKeyType_Down,
	EKeyType_Next,
	EKeyType_Insert,
	EKeyType_Delete,
	EKeyType_LWin,
	EKeyType_RWin,
	EKeyType_Apps,
	EKeyType_Power,
	EKeyType_Sleep,
	EKeyType_Wake,
	EKeyType_WebSearch,
	EKeyType_WebFavorites,
	EKeyType_WebRefresh,
	EKeyType_WebStop,
	EKeyType_WebForward,
	EKeyType_WebBack,
	EKeyType_MyComputer,
	EKeyType_Mail,
	EKeyType_MediaSelect,

	EKeyType_MouseLeft,
	EKeyType_MouseRight,
	EKeyType_MouseMiddle,

	EKeyType_ArrowIn,
	EKeyType_ArrowOut,

	EKeyType_Menu,

	EKeyType_CloseApplication,

EKeyType_EnumCount
};

enum EEventType
{
	EEventType_None = 0,
	EEventType_Key,
	EEventType_Motion,
	EEventType_Touch,
	EEventType_CursorAccure,
	EEventType_EnumCount
};

class EventBase
{
public:
	static const Base::uint gMaxChannels = 16;
	Base::uint channel;

	EventBase() { channel = 0; }
	virtual ~EventBase(){}
	virtual EEventType type()const=0;
	
	INLINE const class EventKey* castToKeyEvent()const;
	INLINE const class EventMotion* castToMotionEvent()const;
	INLINE const class EventTouch* castToTouchEvent()const;
};

class EventKey : public EventBase
{
public:
	EKeyType key;
	float pressure;
	Base::boolean up, down;

	EventKey()
	{
		key = (EKeyType)0;
		pressure = 1.0f;
		up = down = false;
	}

	EEventType type()const { return EEventType_Key; }
};

class EventMotion : public EventBase
{
public:
	float dx, dy, dz;
	EventMotion() { dx = dy = dz = 0; }
	EEventType type()const { return EEventType_Motion; }
};

class EventTouch : public EventBase
{
public:
	float x, y, z, dx, dy, dz;
	float pressure, size;
	Base::boolean up, down;
	Base::uint seqCount;

	EventTouch()
	{
		x = y = z = dx = dy = dz = 0;
		pressure = 1.0f;
		size = 1.0f;
		up = down = false;
	}

	EEventType type()const { return EEventType_Touch; }
};

class EventCursorAccure : public EventBase
{
public:
	float x, y;

	EventCursorAccure()
	{
		x = y = 0;
	}

	EEventType type()const { return EEventType_CursorAccure; }
};

class State
{
public:
	Base::Vector3f acceleration;

	class Text
	{
	public:
		Base::String string;
		Base::uint caret;
		Base::boolean printing;
		Text()
		{
			caret = 0;
			printing = false;
		}

		void caretLeft()
		{
			if(caret > 0)
				caret --;
		}
		void caretRight()
		{
			if(caret < string.size())
				caret ++;
		}
		void deleteAction()
		{
			if(caret >= string.size())
				return;

			string.erase(caret);
		}
		void backspaceAction()
		{
			if(caret == 0)
				return;
			if(caret >= string.size()){
				caret = string.size() - 1;
				string.pop();
				return;
			}
			caret --;
			string.erase(caret);
		}

	};

	Text text[EventBase::gMaxChannels];
};

INLINE const EventKey* EventBase::castToKeyEvent()const
{
	return type() == EEventType_Key ? static_cast<const EventKey*>(this) : 0;
}

INLINE const EventMotion* EventBase::castToMotionEvent()const
{
	return type() == EEventType_Motion ? static_cast<const EventMotion*>(this) : 0;
}

INLINE const EventTouch* EventBase::castToTouchEvent()const
{
	return type() == EEventType_Touch ? static_cast<const EventTouch*>(this) : 0;
}

static INLINE Base::boolean isCloseApplicationEvent(const EventBase& ev)
{
	const EventKey* kev = ev.castToKeyEvent();
	return kev && kev->key == EKeyType_CloseApplication && kev->up;
}

static INLINE Base::boolean isBackKeyUpEvent(const EventBase& ev)
{
	const EventKey* kev = ev.castToKeyEvent();
	return kev && kev->key == EKeyType_Back && kev->up;
}

static INLINE Base::boolean isEscapeKeyUpEvent(const EventBase& ev)
{
	const EventKey* kev = ev.castToKeyEvent();
	return kev && kev->key == EKeyType_Escape && kev->up;
}

}
