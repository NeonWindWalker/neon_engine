#pragma once

#include "toggle.h"

namespace GUI
{

class ToggleHover : public Toggle
{
public:
	rl_class1(GUI::ToggleHover, GUI::Widget)
	rl_base_class0(GUI::Toggle)
	rl_reflective

	ToggleHover(Widget& inFather);

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state);
	virtual void onCursorLeave(const CursorState& state, uint device);

private:
	boolean enterFlag[Input::EventBase::gMaxChannels];
	boolean isAnyEnterFlag();
};

}
