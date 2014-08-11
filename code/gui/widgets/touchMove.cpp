#include "touchMove.h"

namespace GUI
{

TouchMove::TouchMove(Widget& inFather) : Toggle(inFather)
{
	touchDeltha.setZero();
}

boolean TouchMove::onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state)
{
	const Input::EventTouch* touch = ev.castToTouchEvent();
	if(touch)
	{
		if(touch->down && _toggledChannel < 0)
		{
			touchDeltha = pos - size * Scalar(0.5);
			toggled(ev.channel, true);
		}

		if(touch->channel == _toggledChannel)
		{
			if(touch->up)
			{
				touchDeltha.setZero();
				toggled(ev.channel, false);
			}
			else
			{
				touchDeltha = pos - size * Scalar(0.5);
			}
		}
	}

	return breakCursorBubbling;
}

void TouchMove::onCursorLeave(const CursorState& state, uint channel)
{
	toggled(channel, false);
}

}