#pragma once

#include "toggle.h"

namespace GUI
{

class SliderThumb : public Widget
{
public:
	rl_class1(GUI::SliderThumb, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	SliderThumb(Widget& inFather) : Widget(inFather)
	{
		image = _CS("slider_thumb");
		size.set(Scalar(20), Scalar(20));

		dockX.s = 1;
		dockX.sType = ELenghtType_Relative;
		dockY.s = 1;
		dockY.sType = ELenghtType_Relative;

		aspect = Scalar(0.5);
		aspectDockXDirection = -1;
		aspectDockYDirection = -1;
	}
};

class Slider : public Toggle
{
public:
	rl_class1(GUI::Slider, GUI::Widget)
	rl_base_class0(GUI::Toggle)
	rl_reflective

	rl_field(Scalar, valueStep);
	rl_field(Scalar, valueMin);
	rl_field(Scalar, valueMax);
	rl_field(String, valueChangedMethodName);
	Event<void(Slider&, Scalar)> valueChangedEvent;
	StrongPtr<SliderThumb> thumb;

	Scalar value()const
	{
		return _value;

		//Bound bnd = childLocalBound();
		//return ( thumb->position.x - bnd.min.x ) / ( bnd.max.x - bnd.min.x - thumb->size.x );
	}

	void value(Scalar val)
	{
		val = clamp(val, valueMin, valueMax);
		if(valueStep)
			val = floor(val / valueStep + Scalar(0.5)) * valueStep;
		
		if(val == _value)
			return;
		_value = val;
		
		valueChangedEvent(*this, value());
		if(!valueChangedMethodName.empty()){
			IReflective* p = getModelInterface();
			if(p){
				Reflection sender = reflectionOf(*this);
				Reflection args;
				p->reflection().reflectionByPath(valueChangedMethodName).call(sender, args);
			}
		}
	}

	Slider(Widget& inFather) : Toggle(inFather)
	{
		_value = 0;
		valueStep = 0;
		valueMin = 0;
		valueMax = 1;
		image = _CS("slider");
		thumb = new SliderThumb(*this);
		*childs.push() = thumb;
	}

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state)
	{
		const Input::EventKey* keyev = ev.castToKeyEvent();
		if(keyev)
		{
			if(keyev->key == Input::EKeyType_MouseLeft && keyev->down){
				toggled(ev.channel, true);
				screen->focus(ev.channel, this);
				if(_toggledChannel == ev.channel)
					value(valueFromLocalCoord(pos.x));
				return true;
			}
			if(keyev->key == Input::EKeyType_MouseLeft && keyev->up){
				if(_toggledChannel == ev.channel)
					value(valueFromLocalCoord(pos.x));
				screen->unfocus(ev.channel, this);
				toggled(ev.channel, false);
				return true;
			}
		}

		const Input::EventTouch* touch = ev.castToTouchEvent();
		if(touch)
		{
			if(_toggledChannel == ev.channel)
				value(valueFromLocalCoord(pos.x));

			if(touch->down){
				toggled(ev.channel, true);
				screen->focus(ev.channel, this);
				if(_toggledChannel == ev.channel)
					value(valueFromLocalCoord(pos.x));
				return true;
			}
			if(touch->up){
				toggled(ev.channel, false);
				screen->unfocus(ev.channel, this);
				return true;
			}
		}

		const Input::EventMotion* motion = ev.castToMotionEvent();
		if(motion)
		{
			if(_toggledChannel == ev.channel)
				value(valueFromLocalCoord(pos.x));
		}
		return false;
	}

	virtual boolean onFocusInputBubbling(const Input::EventBase& ev)
	{
		const Input::EventKey* keyev = ev.castToKeyEvent();
		if(keyev)
		{
			if(keyev->key == Input::EKeyType_MouseLeft && keyev->up){
				screen->unfocus(ev.channel, this);
				toggled(ev.channel, false);
				return true;
			}
		}

		const Input::EventTouch* touchev = ev.castToTouchEvent();
		if(touchev)
		{
			if(_toggledChannel == ev.channel)
				value(valueFromLocalCoord(fromScreenPosToLocalPos(screen->cursorState[ev.channel].position).x));

			if(touchev->up){
				screen->unfocus(ev.channel, this);
				toggled(ev.channel, false);
				return true;
			}
		}

		const Input::EventMotion* motion = ev.castToMotionEvent();
		if(motion)
		{
			if(_toggledChannel == ev.channel)
				value(valueFromLocalCoord(fromScreenPosToLocalPos(screen->cursorState[ev.channel].position).x));
		}
	
		return false;
	}

	Scalar valueFromLocalCoord(Scalar x)
	{
		return clamp((x - Scalar(0.5) * thumb->size.x) / (size.x - thumb->size.x) * (valueMax - valueMin) + valueMin, valueMin, valueMax);
	}

	virtual void onUpdate(Scalar dt)
	{
		thumb->position.x = (size.x - thumb->size.x) * Scalar((_value - valueMin) / (valueMax - valueMin));
		Toggle::onUpdate(dt);
	}

private:
	Scalar _value;
};

}
