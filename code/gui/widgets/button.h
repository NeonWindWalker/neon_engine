#pragma once

#include "../widget.h"

namespace GUI
{

class Button : public Widget
{
public:
	rl_class1(GUI::Button, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective
	

	rl_field(String, pushDownMethodName);
	rl_field(String, pushUpMethodName);
	rl_field(String, clickMethodName);
	Event<void(Button&)> clickEvent;
	Event<void(Button&)> pushDownEvent;
	Event<void(Button&)> pushUpEvent;

	rl_field(String, sound);
	rl_field(boolean, noHandleInput);
	rl_field(boolean, enabled);
	rl_field(Scalar, maxClickTime);

	Button(Widget& inFather);

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state);
	virtual void onCursorLeave(const CursorState& state, uint channel);
	virtual void onHide();
	virtual void onUpdate(Scalar dt);

	void pushed(uint channel, boolean val);
	boolean pushed()const
	{
		return _pushedChannel >= 0;
	}

protected:
	int _pushedChannel;
	Scalar afterPushTime;

	virtual void onPushDown();
	virtual void onPushUp();
	virtual void onClick();
	virtual void onDraw(const ParentDrawInfo& pdi, IDrawer& render);
};

}
