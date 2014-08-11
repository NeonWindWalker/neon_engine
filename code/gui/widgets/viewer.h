#pragma once

#include "../widget.h"

namespace GUI
{

class Viewer : public Widget
{
public:
	rl_class1(GUI::Viewer, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(boolean, zoomable);
	rl_field(Scalar, scaleStep);
	rl_field(Scalar, scaleNivilation);

	rl_field(Scalar, minScale);
	rl_field(Scalar, maxScale);
	rl_field(Vector, viewClampMin);
	rl_field(Vector, viewClampMax);
	rl_field(boolean, viewClampEnable);
	rl_field(boolean, viewClampFarMode);
	rl_field(Scalar, inertion);

	Viewer(Widget& inFather);

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state);
	virtual void onCursorLeave(const CursorState& state, uint device);
	virtual void onUpdate(Scalar dt);

	INLINE void unactive()
	{
		for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
			actived[i] = false;
	}

	INLINE uint activeCount()
	{
		uint count = 0;
		for(uint i = 0; i < Input::EventBase::gMaxChannels; ++i)
			if(actived[i])
				count++;
		return count;
	}

	INLINE void centerToPoint(const Vector& p)
	{
		childRectOffset = Scalar(0.5) * size - p * childRectScale;
	}

	INLINE void scale(Scalar k, const Vector& P)
	{
		Scalar newScale = childRectScale.x * k;
		if(newScale < reallyMinScale)
			k = reallyMinScale / childRectScale.x;
		if(newScale > reallyMaxScale)
			k = reallyMaxScale / childRectScale.x;

		childRectOffset -= (P - position - childRectOffset) * (k - Scalar(1));
		childRectScale *= k;
	}

protected:
	boolean actived[Input::EventBase::gMaxChannels];
	Vector cursorPosition[Input::EventBase::gMaxChannels][2];
	boolean bindex;
	Scalar scaleAccum;
	Vector moveVelosity;

	Scalar reallyMinScale;
	Scalar reallyMaxScale;
};

}
