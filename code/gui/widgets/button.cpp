#include "button.h"

namespace GUI
{

Button::Button(Widget& inFather) : Widget(inFather)
{
	image = _CS("button");
	sound = _CS("click");
	_pushedChannel = -1;
	noHandleInput = false;
	enabled = true;
	maxClickTime = Scalar(1e32);
	afterPushTime = Scalar(1e32);
}

boolean Button::onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state)
{
	if(!enabled)
		return false;

	const Input::EventKey* keyev = ev.castToKeyEvent();
	if(keyev)
	{
		if(keyev->key == Input::EKeyType_MouseLeft && keyev->down){
			pushed(ev.channel, true);
			return !noHandleInput;
		}
		if(keyev->key == Input::EKeyType_MouseLeft && keyev->up){
			pushed(ev.channel, false);
			return !noHandleInput;
		}
	}

	const Input::EventTouch* touch = ev.castToTouchEvent();
	if(touch)
	{
		if(touch->down){
			pushed(ev.channel, true);
			return !noHandleInput;
		}
		if(touch->up){
			pushed(ev.channel, false);
			return !noHandleInput;
		}
	}
	return false;
}

void Button::onCursorLeave(const CursorState& state, uint channel)
{
	if(channel == _pushedChannel)
		_pushedChannel = -1;
}

void Button::onHide()
{
	_pushedChannel = -1;
	Widget::onHide();
}

void Button::pushed(uint channel, boolean val)
{
	if(channel == _pushedChannel)
	{
		if(val)
			return;

		onPushUp();
		if(afterPushTime < maxClickTime)
			onClick();
		_pushedChannel = -1;
	}
	else
	{
		if(!val)
			return;

		if(_pushedChannel >= 0)
			return;

		_pushedChannel = channel;
		onPushDown();
	}
}

void Button::onPushDown()
{
	afterPushTime = 0;
	pushDownEvent(*this);
	if(!pushDownMethodName.empty()){
		IReflective* p = getModelInterface();
		if(p)
		{
			Reflection sender = reflectionOf(*this);
			Reflection args;
			p->reflection().reflectionByPath(pushDownMethodName).call(sender, args);
		}
	}
}

void Button::onPushUp()
{
	pushUpEvent(*this);
	if(!pushUpMethodName.empty()){
		IReflective* p = getModelInterface();
		if(p){
			Reflection sender = reflectionOf(*this);
			Reflection args;
			p->reflection().reflectionByPath(pushUpMethodName).call(sender, args);
		}
	}
}

void Button::onClick()
{
	if(!sound.empty())
		soundBeep(sound);

	clickEvent(*this);
	if(!clickMethodName.empty()){
		IReflective* p = getModelInterface();
		if(p){
			Reflection sender = reflectionOf(*this);
			Reflection args;
			p->reflection().reflectionByPath(clickMethodName).call(sender, args);
		}
	}
}

void Button::onDraw(const ParentDrawInfo& pdi, IDrawer& render)
{
	String img = image;
	if(_pushedChannel >= 0)
		img <<  _CS("_pushed");
	render.drawWidget(pdi.bound, position, size, img, pdi.scale, pdi.offset);
}

void Button::onUpdate(Scalar dt)
{
	Widget::onUpdate(dt);
	afterPushTime += dt;
}

}