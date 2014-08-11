#pragma once

#include "widget.h"

namespace GUI
{

class Screen : public Widget, public ScreenBase
{
public:
	rl_class(GUI::Screen)
	rl_base_class0(GUI::Widget)
	rl_reflective

	Library* library;

	Screen();

	void focus(uint channel, Widget* w);
	void unfocus(uint channel, Widget* w);
	void unfocus(Widget* w);
	Widget* lastFocusedWidget(uint device);

	boolean recive(const Input::EventBase& ev);
	void update(Scalar dt);
	void draw(IDrawer& render);

	virtual Library* getLibrary()const
	{
		return library;
	}

	Delegate<void(ConstString, Scalar)> soundBeepCallback;

	virtual void soundBeep(ConstString name, Scalar volume)
	{
		if(soundBeepCallback)
			soundBeepCallback(name, volume);
	}

	void setLenghts(Scalar pixelSize);

	boolean updateLayoutChanges;

protected:
	boolean guisysTraverseFocusEvent(const Input::EventBase& ev);
	boolean guisysTraverseCursor(const Input::EventBase& ev);

	Array< WeakPtr<Widget> > focusList[Input::EventBase::gMaxChannels];
	Array< WeakPtr<Widget> > prevCursorHitList[Input::EventBase::gMaxChannels];

	Array<TrawersData> traverseList;

	void clampCursor(uint ch)
	{
		CursorState& cur = cursorState[ch];
		Vector newPos = clamp(cur.position, Vector(0.0f, 0.0f), size);
		cursorOvermove[ch] += cur.position - newPos;
		cur.position = newPos;
	}
};

}
