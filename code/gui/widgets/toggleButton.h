#pragma once

#include "toggle.h"

namespace GUI
{

class ToggleButton : public Toggle
{
public:
	rl_class1(GUI::ToggleButton, GUI::Widget)
	rl_base_class0(GUI::Toggle)
	rl_reflective

	ToggleButton(Widget& inFather);

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state);
};

}
