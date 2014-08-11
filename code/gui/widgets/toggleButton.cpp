#include "toggleButton.h"

namespace GUI
{

ToggleButton::ToggleButton(Widget& inFather) : Toggle(inFather)
{
}

boolean ToggleButton::onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state)
{
	const Input::EventKey* keyev = ev.castToKeyEvent();
	if(keyev)
	{
		if(keyev->key == Input::EKeyType_MouseLeft && keyev->down){
			toggled(ev.channel, !toggled());
			return true;
		}
	}

	const Input::EventTouch* touch = ev.castToTouchEvent();
	if(touch)
	{
		if(touch->down){
			toggled(ev.channel, !toggled());
			return true;
		}
	}
	return false;
}

};