#include "toggleHover.h"

namespace GUI
{

ToggleHover::ToggleHover(Widget& inFather) : Toggle(inFather)
{
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
		enterFlag[i] = 0;
}

boolean ToggleHover::onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state)
{
	const Input::EventTouch* touch = ev.castToTouchEvent();
	if(touch)
		enterFlag[ev.channel] = !touch->up;
	else
		enterFlag[ev.channel] = true;

	toggled(0, isAnyEnterFlag());
	return false;
}

void ToggleHover::onCursorLeave(const CursorState& state, uint channel)
{
	enterFlag[channel] = false;
	toggled(0, isAnyEnterFlag());
}

boolean ToggleHover::isAnyEnterFlag()
{
	boolean b = false;
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
		b |= enterFlag[i];
	return b;
}

}