#pragma once

#include "toggle.h"

namespace GUI
{

class TouchMove : public Toggle
{
public:
	rl_class1(GUI::TouchMove, GUI::Widget)
	rl_base_class0(GUI::Toggle)
	rl_reflective

	TouchMove(Widget& inFather);

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state);
	virtual void onCursorLeave(const CursorState& state, uint channel);

	INLINE Vector deltha()const { return touchDeltha; }

protected:
	Vector touchDeltha;
};

}
