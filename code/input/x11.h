#include "events.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace Input
{

static INLINE EventBase* convertKeyEvent(const XEvent& we)
{
	switch(we.type)
	{
	case ButtonPress:
		if(we.xbutton.button == 1){
			EventKey* ev = new EventKey();
			ev->down = true;
			ev->key = EKeyType_MouseLeft;
			return ev;
		}else if(we.xbutton.button == 2){
			EventKey* ev = new EventKey();
			ev->down = true;
			ev->key = EKeyType_MouseMiddle;
			return ev;
		}else if(we.xbutton.button == 3){
			EventKey* ev = new EventKey();
			ev->down = true;
			ev->key = EKeyType_MouseRight;
			return ev;
		}
		else if(we.xbutton.button == 4){
			EventKey* ev = new EventKey();
			ev->down = true;
			ev->key = EKeyType_ArrowOut;
			return ev;
		}
		else if(we.xbutton.button == 5){
			EventKey* ev = new EventKey();
			ev->down = true;
			ev->key = EKeyType_ArrowIn;
			return ev;
		}
		return 0;

	case ButtonRelease:
		if(we.xbutton.button == 1){
			EventKey* ev = new EventKey();
			ev->up = true;
			ev->key = EKeyType_MouseLeft;
			return ev;
		}else if(we.xbutton.button == 2){
			EventKey* ev = new EventKey();
			ev->up = true;
			ev->key = EKeyType_MouseMiddle;
			return ev;
		}else if(we.xbutton.button == 3){
			EventKey* ev = new EventKey();
			ev->up = true;
			ev->key = EKeyType_MouseRight;
			return ev;
		}
		else if(we.xbutton.button == 4){
			EventKey* ev = new EventKey();
			ev->up = true;
			ev->key = EKeyType_ArrowOut;
			return ev;
		}
		else if(we.xbutton.button == 5){
			EventKey* ev = new EventKey();
			ev->up = true;
			ev->key = EKeyType_ArrowIn;
			return ev;
		}
		return 0;

	case KeyPress:
		{
			EventKey* ev = new EventKey();
			ev->down = true;
			switch(we.xkey.keycode)
			{
			case 9:
				ev->key = EKeyType_Escape;
				break;
			case 36:
				ev->key = EKeyType_Enter;
				break;
			case 65:
				ev->key = EKeyType_Space;
				break;
			case 25:
				ev->key = EKeyType_W;
				break;
			case 39:
				ev->key = EKeyType_S;
				break;
			case 38:
				ev->key = EKeyType_A;
				break;
			case 40:
				ev->key = EKeyType_D;
				break;

			case 113:
				ev->key = EKeyType_ArrowLeft;
				break;
			case 114:
				ev->key = EKeyType_ArrowRight;
				break;
			case 116:
				ev->key = EKeyType_ArrowDown;
				break;
			case 111:
				ev->key = EKeyType_ArrowUp;
				break;
			default:
				delete ev;
				return 0;
			}
			return ev;
		}

	case KeyRelease:
		{
			EventKey* ev = new EventKey();
			ev->up = true;
			switch(we.xkey.keycode)
			{
			case 9:
				ev->key = EKeyType_Escape;
				break;
			case 36:
				ev->key = EKeyType_Enter;
				break;
			case 65:
				ev->key = EKeyType_Space;
				break;
			case 25:
				ev->key = EKeyType_W;
				break;
			case 39:
				ev->key = EKeyType_S;
				break;
			case 38:
				ev->key = EKeyType_A;
				break;
			case 40:
				ev->key = EKeyType_D;
				break;

			case 113:
				ev->key = EKeyType_ArrowLeft;
				break;
			case 114:
				ev->key = EKeyType_ArrowRight;
				break;
			case 116:
				ev->key = EKeyType_ArrowDown;
				break;
			case 111:
				ev->key = EKeyType_ArrowUp;
				break;

			default:
				delete ev;
				return 0;
			}
			return ev;
		}
	}

	return 0;
}

}
