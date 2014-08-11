#include "events.h"

namespace Input
{

static INLINE EventKey* convertKeyEvent(const DIDEVICEOBJECTDATA& data)
{
	EventKey ev;
	Base::boolean on = Base::isBit(data.dwData, 7);
	ev.down = on;
	ev.up = !on;

	switch(data.dwOfs)
	{
case  DIK_ESCAPE: ev.key = EKeyType_Escape; break;
case  DIK_1: ev.key = EKeyType_1; break;
case  DIK_2: ev.key = EKeyType_2; break;
case  DIK_3: ev.key = EKeyType_3; break;
case  DIK_4: ev.key = EKeyType_4; break;
case  DIK_5: ev.key = EKeyType_5; break;
case  DIK_6: ev.key = EKeyType_6; break;
case  DIK_7: ev.key = EKeyType_7; break;
case  DIK_8: ev.key = EKeyType_8; break;
case  DIK_9: ev.key = EKeyType_9; break;
case  DIK_0: ev.key = EKeyType_0; break;
case  DIK_MINUS: ev.key = EKeyType_Minus; break;
case  DIK_EQUALS: ev.key = EKeyType_Equals; break;
case  DIK_BACK: ev.key = EKeyType_Back; break;
case  DIK_TAB: ev.key = EKeyType_Tab; break;
case  DIK_Q: ev.key = EKeyType_Q; break;
case  DIK_W: ev.key = EKeyType_W; break;
case  DIK_E: ev.key = EKeyType_E; break;
case  DIK_R: ev.key = EKeyType_R; break;
case  DIK_T: ev.key = EKeyType_T; break;
case  DIK_Y: ev.key = EKeyType_Y; break;
case  DIK_U: ev.key = EKeyType_U; break;
case  DIK_I: ev.key = EKeyType_I; break;
case  DIK_O: ev.key = EKeyType_O; break;
case  DIK_P: ev.key = EKeyType_P; break;
case  DIK_LBRACKET: ev.key = EKeyType_LBracket; break;
case  DIK_RBRACKET: ev.key = EKeyType_RBracket; break;
case  DIK_RETURN: ev.key = EKeyType_Enter; break;
case  DIK_LCONTROL: ev.key = EKeyType_LControl; break;
case  DIK_A: ev.key = EKeyType_A; break;
case  DIK_S: ev.key = EKeyType_S; break;
case  DIK_D: ev.key = EKeyType_D; break;
case  DIK_F: ev.key = EKeyType_F; break;
case  DIK_G: ev.key = EKeyType_G; break;
case  DIK_H: ev.key = EKeyType_H; break;
case  DIK_J: ev.key = EKeyType_J; break;
case  DIK_K: ev.key = EKeyType_K; break;
case  DIK_L: ev.key = EKeyType_L; break;
case  DIK_SEMICOLON: ev.key = EKeyType_Semicolon; break;
case  DIK_APOSTROPHE: ev.key = EKeyType_Apostrophe; break;
case  DIK_GRAVE: ev.key = EKeyType_Grave; break;
case  DIK_LSHIFT: ev.key = EKeyType_LShift; break;
case  DIK_BACKSLASH: ev.key = EKeyType_BackSlash; break;
case  DIK_Z: ev.key = EKeyType_Z; break;
case  DIK_X: ev.key = EKeyType_X; break;
case  DIK_C: ev.key = EKeyType_C; break;
case  DIK_V: ev.key = EKeyType_V; break;
case  DIK_B: ev.key = EKeyType_B; break;
case  DIK_N: ev.key = EKeyType_N; break;
case  DIK_M: ev.key = EKeyType_M; break;
case  DIK_COMMA: ev.key = EKeyType_Comma; break;
case  DIK_PERIOD: ev.key = EKeyType_Period; break;
case  DIK_SLASH: ev.key = EKeyType_Slash; break;
case  DIK_RSHIFT: ev.key = EKeyType_RShift; break;
case  DIK_MULTIPLY: ev.key = EKeyType_Multiply; break;
case  DIK_LMENU: ev.key = EKeyType_LMenu; break;
case  DIK_SPACE: ev.key = EKeyType_Space; break;
case  DIK_CAPITAL: ev.key = EKeyType_Capital; break;
case  DIK_F1: ev.key = EKeyType_F1; break;
case  DIK_F2: ev.key = EKeyType_F2; break;
case  DIK_F3: ev.key = EKeyType_F3; break;
case  DIK_F4: ev.key = EKeyType_F4; break;
case  DIK_F5: ev.key = EKeyType_F5; break;
case  DIK_F6: ev.key = EKeyType_F6; break;
case  DIK_F7: ev.key = EKeyType_F7; break;
case  DIK_F8: ev.key = EKeyType_F8; break;
case  DIK_F9: ev.key = EKeyType_F9; break;
case  DIK_F10: ev.key = EKeyType_F10; break;
case  DIK_NUMLOCK: ev.key = EKeyType_NumLock; break;
case  DIK_SCROLL: ev.key = EKeyType_Scroll; break;
case  DIK_NUMPAD7: ev.key = EKeyType_Numpad7; break;
case  DIK_NUMPAD8: ev.key = EKeyType_Numpad8; break;
case  DIK_NUMPAD9: ev.key = EKeyType_Numpad9; break;
case  DIK_SUBTRACT: ev.key = EKeyType_Substract; break;
case  DIK_NUMPAD4: ev.key = EKeyType_Numpad4; break;
case  DIK_NUMPAD5: ev.key = EKeyType_Numpad5; break;
case  DIK_NUMPAD6: ev.key = EKeyType_Numpad6; break;
case  DIK_ADD: ev.key = EKeyType_Add; break;
case  DIK_NUMPAD1: ev.key = EKeyType_Numpad1; break;
case  DIK_NUMPAD2: ev.key = EKeyType_Numpad2; break;
case  DIK_NUMPAD3: ev.key = EKeyType_Numpad3; break;
case  DIK_NUMPAD0: ev.key = EKeyType_Numpad0; break;
case  DIK_DECIMAL: ev.key = EKeyType_Decimal; break;
case  DIK_OEM_102: ev.key = EKeyType_OEM_102; break;
case  DIK_F11: ev.key = EKeyType_F11; break;
case  DIK_F12: ev.key = EKeyType_F12; break;
case  DIK_F13: ev.key = EKeyType_F13; break;
case  DIK_F14: ev.key = EKeyType_F14; break;
case  DIK_F15: ev.key = EKeyType_F15; break;
case  DIK_KANA: ev.key = EKeyType_KANA; break;
case  DIK_ABNT_C1: ev.key = EKeyType_ABNT_C1; break;
case  DIK_CONVERT: ev.key = EKeyType_Convert; break;
case  DIK_NOCONVERT: ev.key = EKeyType_NoConvert; break;
case  DIK_YEN: ev.key = EKeyType_YEN; break;
case  DIK_ABNT_C2: ev.key = EKeyType_ABNT_C2; break;
case  DIK_NUMPADEQUALS: ev.key = EKeyType_NumpadEquals; break;
case  DIK_PREVTRACK: ev.key = EKeyType_PrevTrack; break;
case  DIK_AT: ev.key = EKeyType_AT; break;
case  DIK_COLON: ev.key = EKeyType_Colon; break;
case  DIK_UNDERLINE: ev.key = EKeyType_Underline; break;
case  DIK_KANJI: ev.key = EKeyType_KANJI; break;
case  DIK_STOP: ev.key = EKeyType_Stop; break;
case  DIK_AX: ev.key = EKeyType_AX; break;
case  DIK_UNLABELED: ev.key = EKeyType_Unlabeled; break;
case  DIK_NEXTTRACK: ev.key = EKeyType_NextTrack; break;
case  DIK_NUMPADENTER: ev.key = EKeyType_NumpadEnter; break;
case  DIK_RCONTROL: ev.key = EKeyType_RControl; break;
case  DIK_MUTE: ev.key = EKeyType_Mute; break;
case  DIK_CALCULATOR: ev.key = EKeyType_Calculator; break;
case  DIK_PLAYPAUSE: ev.key = EKeyType_PlayPause; break;
case  DIK_MEDIASTOP: ev.key = EKeyType_MediaStop; break;
case  DIK_VOLUMEDOWN: ev.key = EKeyType_VolumeDown; break;
case  DIK_VOLUMEUP: ev.key = EKeyType_VolumeUp; break;
case  DIK_WEBHOME: ev.key = EKeyType_WebHome; break;
case  DIK_NUMPADCOMMA: ev.key = EKeyType_NumpadComma; break;
case  DIK_DIVIDE: ev.key = EKeyType_Divide; break;
case  DIK_SYSRQ: ev.key = EKeyType_SysRQ; break;
case  DIK_RMENU: ev.key = EKeyType_RMenu; break;
case  DIK_PAUSE: ev.key = EKeyType_Pause; break;
case  DIK_HOME: ev.key = EKeyType_Home; break;
case  DIK_UP: ev.key = EKeyType_Up; break;
case  DIK_PRIOR: ev.key = EKeyType_Prior; break;
case  DIK_LEFT: ev.key = EKeyType_Left; break;
case  DIK_RIGHT: ev.key = EKeyType_Right; break;
case  DIK_END: ev.key = EKeyType_End; break;
case  DIK_DOWN: ev.key = EKeyType_Down; break;
case  DIK_NEXT: ev.key = EKeyType_Next; break;
case  DIK_INSERT: ev.key = EKeyType_Insert; break;
case  DIK_DELETE: ev.key = EKeyType_Delete; break;
case  DIK_LWIN: ev.key = EKeyType_LWin; break;
case  DIK_RWIN: ev.key = EKeyType_RWin; break;
case  DIK_APPS: ev.key = EKeyType_Apps; break;
case  DIK_POWER: ev.key = EKeyType_Power; break;
case  DIK_SLEEP: ev.key = EKeyType_Sleep; break;
case  DIK_WAKE: ev.key = EKeyType_Wake; break;
case  DIK_WEBSEARCH: ev.key = EKeyType_WebSearch; break;
case  DIK_WEBFAVORITES: ev.key = EKeyType_WebFavorites; break;
case  DIK_WEBREFRESH: ev.key = EKeyType_WebRefresh; break;
case  DIK_WEBSTOP: ev.key = EKeyType_WebStop; break;
case  DIK_WEBFORWARD: ev.key = EKeyType_WebForward; break;
case  DIK_WEBBACK: ev.key = EKeyType_WebBack; break;
case  DIK_MYCOMPUTER: ev.key = EKeyType_MyComputer; break;
case  DIK_MAIL: ev.key = EKeyType_Mail; break;
case  DIK_MEDIASELECT: ev.key = EKeyType_MediaSelect; break;
default: return 0;
	}

	if(ev.key)
		return new EventKey(ev);
	else
		return 0;
}

static INLINE EventKey* convertMouseEvent(const DIDEVICEOBJECTDATA& data)
{
	EventKey ev;
	Base::boolean on = Base::isBit(data.dwData, 7);
	ev.down = on;
	ev.up = !on;

	switch(data.dwOfs)
	{
	case DIMOFS_BUTTON0: ev.key = EKeyType_MouseLeft; break;
	case DIMOFS_BUTTON1: ev.key = EKeyType_MouseRight; break;
	case DIMOFS_BUTTON2: ev.key = EKeyType_MouseMiddle; break;
	}

	if(ev.key)
		return new EventKey(ev);
	else
		return 0;
}

}