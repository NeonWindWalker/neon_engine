#pragma once

#include "button.h"

namespace GUI
{

class ClosableWidget : public Widget
{
public:
	rl_class1(GUI::ClosableWidget, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	ClosableWidget(Widget& inFather) : Widget(inFather)
	{
	}

	virtual boolean onFocusInputBubbling(const Input::EventBase& ev)
	{
		const Input::EventKey* keyev = ev.castToKeyEvent();
		if(keyev)
		{
			if(keyev->up && (keyev->key == Input::EKeyType_Escape || keyev->key == Input::EKeyType_Back)){
				parent->childs.erase(this);
				return true;
			}
		}
		return Widget::onFocusInputBubbling(ev);
	}
};

class CloseButton : public Button
{
public:
	rl_class1(GUI::CloseButton, GUI::Widget)
	rl_base_class0(GUI::Button)
	rl_reflective

	CloseButton(Widget& inFather) : Button(inFather)
	{
		clickEvent += makeDelegate(this, &CloseButton::onClick);
	}

	void onClick(GUI::Button& sender)
	{
		if(!eraseClosableParent(this)){
			logWarning << "unfinded ClosableWidget";
		}
	}

	static boolean eraseClosableParent(Widget* w)
	{
		do
		{
			if(dynamicCast<ClosableWidget>(w))
			{
				w->parent->childs.erase(w);
				return true;
			}
			w = w->parent;
		}
		while(w);
		return false;
	}
};

}
