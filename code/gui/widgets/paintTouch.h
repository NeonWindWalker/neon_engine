#pragma once

#include "../widget.h"

namespace GUI
{

class PaintTouch : public Widget
{
public:
	rl_class1(GUI::PaintTouch, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(Scalar, beforePaintingTouchDelay);

	rl_field(Scalar, scaleStep);
	rl_field(Scalar, scaleNivilation);
	rl_field(boolean, transparentInput);

	boolean flashing;
	boolean sliding;

	Vector point;
	Vector moveDeltha;
	Scalar scale;

	PaintTouch(Widget& inFather);

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state);
	virtual void onCursorLeave(const CursorState& state, uint device);
	virtual void onUpdate(Scalar dt);
	virtual void onHide();

	INLINE boolean isAnyActivedChannel()const 
	{
		boolean b = 0;
		for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
			b |= actived[i];
		return b;
	}

	INLINE void unactive()
	{
		for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
			actived[i] = false;
	}

private:
	boolean actived[Input::EventBase::gMaxChannels];
	Vector cursorPosition[Input::EventBase::gMaxChannels][2];
	uint bindex;
	Scalar scaleAccum;
	Scalar exScale;
	Vector exScalePoint;
	Scalar beforePaintingTouchTime;
};

}
