#include "screen.h"

namespace GUI
{

Screen::Screen() : Widget(*(Widget*)0)
{
	screen = this;
}

void Screen::focus(uint channel, Widget* w)
{
	Assert(channel < Input::EventBase::gMaxChannels);
	if(w == lastFocusedWidget(channel))
		return;
	focusList[channel].clear();

	while(w){
		focusList[channel].insert(0, WeakPtr<Widget>(w));
		w = w->parent;
	}
}

void Screen::unfocus(uint channel, Widget* w)
{
	for(uint i=0; i < focusList[channel].size(); i++)
		if(focusList[channel][i].get() == w)
		{
			focusList[channel].resize(i);
			break;
		}
}

void Screen::unfocus(Widget* w)
{
	for(uint ch = 0; ch < Input::EventBase::gMaxChannels; ch++)
		unfocus(ch, w);
}

Widget* Screen::lastFocusedWidget(uint channel)
{
	Assert(channel < Input::EventBase::gMaxChannels);
	return !focusList[channel].empty() ? focusList[channel].last()->get() : 0;
}

boolean Screen::guisysTraverseFocusEvent(const Input::EventBase& ev)
{
	Assert(ev.channel < Input::EventBase::gMaxChannels);

	for(uint i = 0; i < focusList[ev.channel].size(); ++i)
	{
		StrongPtr<Widget> p = focusList[ev.channel][i].lock();
		if(!p)
			return false;
		if(p->onFocusInputTunneling(ev))
			return true;
	}

	for(int i = focusList[ev.channel].size() - 1; i >= 0; --i)
	{
		StrongPtr<Widget> p = focusList[ev.channel][i].lock();
		if(!p)
			return false;
		if(p->onFocusInputBubbling(ev))
			return true;
	}

	return false;
}

boolean Screen::guisysTraverseCursor(const Input::EventBase& ev)
{
	traverseList.clearSaveReserve();
	Widget::guisysTraverseCursor(traverseList, cursorState[ev.channel].position);

	boolean ret = false;
	uint lastIndex = 0;
	foreach(it, traverseList)
	{
		StrongPtr<Widget> p = it->ptr.lock();
		if(!p)
			break;
		if(it->bubbling)
		{
			if(p->onCursorBubbling(ev, it->locpos, cursorState[ev.channel])){
				ret = true;
				break;
			}
		}
		else
		{
			if(p->onCursorTunneling(ev, it->locpos, cursorState[ev.channel])){
				ret = true;
				break;
			}
		}
		lastIndex = traverseList.indexOf(it);
	}

	for(uint i = 0; i < prevCursorHitList[ev.channel].size(); ++i)
	{
		StrongPtr<Widget> p = prevCursorHitList[ev.channel][i].lock();
		if(!p)
			continue;

		boolean isFinded = false;
		for(uint n=0; n <= lastIndex; n++)
			if(traverseList[n].ptr.get() == p.get()){
				isFinded = true;
				break;
			}
		if(isFinded)
			continue;

		p->onCursorLeave(cursorState[ev.channel], ev.channel);
	}

	prevCursorHitList[ev.channel].clearSaveReserve();
	for(uint n=0; n <= lastIndex; n++)
		if(!traverseList[n].bubbling)
			*prevCursorHitList[ev.channel].push() = traverseList[n].ptr;
		
	traverseList.clearSaveReserve();
	return ret;
}

boolean Screen::recive(const Input::EventBase& ev)
{
	switch(ev.type())
	{
	case Input::EEventType_Motion:
		{
			const Input::EventMotion* e = (const Input::EventMotion*)&ev;
			cursorState[e->channel].position.x += e->dx;
			cursorState[e->channel].position.y += e->dy;
			cursorState[e->channel].enabled = true;
			clampCursor(e->channel);
		}
		break;
	case Input::EEventType_Touch:
		{
			const Input::EventTouch* e = (const Input::EventTouch*)&ev;
			cursorState[e->channel].position.x = e->x;
			cursorState[e->channel].position.y = e->y;
			cursorState[e->channel].enabled = false;
			cursorState[e->channel].pushed = !e->up;
			clampCursor(e->channel);
		}
		break;
	}

	boolean bFoc = guisysTraverseFocusEvent(ev);
	boolean bCur = guisysTraverseCursor(ev);
	return bFoc | bCur;
}

void Screen::update(Scalar dt)
{
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
		cursorState[i].position.selfClamp(Vector::getZero(), size);

	Array< WeakPtr<Widget> > list;
	guisysTraverseUpdate(list);
	for(uint i = 0; i < list.size(); ++i)
	{
		StrongPtr<Widget> p = list[i].lock();
		if(p)
			p->onUpdate(dt);
	}

	for(uint substeps = 0; substeps < 12; substeps ++)
	{
		updateLayoutChanges = false;
		guisysTraverseUpdateLayout();
		if(!updateLayoutChanges)
			break;
	}
	
}

void Screen::draw(IDrawer& render)
{
	ParentDrawInfo di;
	di.bound.set(position, position + size);
	guisysTraverseDraw(di, render);

	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
	{
		if(!cursorState[i].enabled || !cursorState[i].visible)
			continue;

		Scalar k = cursorState[i].pushed ? 2.0f : 1.0f;

		render.drawWidget(bound(), cursorState[i].position - Vector(Scalar(32),Scalar(32)) * k, Vector(Scalar(64),Scalar(64)) * k, _CS("cursor"), Vector::getOne(), Vector::getZero());
	}
}

void Screen::setLenghts(Scalar pixelSize)
{
	Scalar pilexlInMetter = 1.0f / pixelSize;
	lenghts.lenghts[GUI::ELenghtType_RealLenght].set(pilexlInMetter, pilexlInMetter);

	Scalar minimalOptimalButtonSize = 0.012f;
	Scalar minScreenLen = min(size.x, size.y) * pixelSize;
	Scalar subdivisionSize = minScreenLen / 8.0f;
	Scalar ergonomicSize = minimalOptimalButtonSize;

	if(minimalOptimalButtonSize > subdivisionSize && subdivisionSize > 0.0f)
	{
		ergonomicSize = sqrt(subdivisionSize * minimalOptimalButtonSize);
		Scalar k = floor(minScreenLen / ergonomicSize + 0.5f);
		ergonomicSize = minScreenLen / k;
	}

	lenghts.lenghts[GUI::ELenghtType_ErgonomicLenght].set(pilexlInMetter * ergonomicSize, pilexlInMetter * ergonomicSize);
}

}
