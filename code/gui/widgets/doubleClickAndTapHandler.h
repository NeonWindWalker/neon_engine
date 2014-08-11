#pragma once

#include "../widget.h"

namespace GUI
{

class DoubleClickAndTapHandler : public Widget
{
public:
	rl_class1(GUI::DoubleClickAndTapHandler, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(String, doubleFingersSingleClickMethodName);
	Event<boolean(DoubleClickAndTapHandler&, const Vector&, const Vector&)> doubleFingersSingleClickEvent;

	rl_field(String, doubleClickMethodName);
	Event<boolean(DoubleClickAndTapHandler&, const Vector&)> doubleClickEvent;

	rl_field(String, singleClickMethodName);
	Event<boolean(DoubleClickAndTapHandler&, const Vector&)> singleClickEvent;

	DoubleClickAndTapHandler(Widget& inFather);

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state);
	virtual void onUpdate(Scalar dt);

	virtual void onHide() { clearState(); }
	void clearState();

	INLINE uint activedChannels()const
	{
		uint count = 0;
		for(uint i = 0; i < Input::EventBase::gMaxChannels; ++i)
			if(afterUpTime[i] > afterDownTime[i])
				count++;
		return count;
	}

protected:
	Scalar afterUpTime[Input::EventBase::gMaxChannels];
	Scalar afterDownTime[Input::EventBase::gMaxChannels];
	Vector downPosition[Input::EventBase::gMaxChannels];

	virtual boolean onEvent(const Vector& p);
	virtual boolean onDoubleFingersSingleClick(const Vector& a, const Vector& b);
	virtual boolean onSingleEvent(const Vector& p);
};

}
