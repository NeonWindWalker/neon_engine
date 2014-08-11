#include "doubleClickAndTapHandler.h"
#include "../screen.h"

namespace GUI
{

DoubleClickAndTapHandler::DoubleClickAndTapHandler(Widget& inFather) : Widget(inFather)
{
	clearState();
}

void DoubleClickAndTapHandler::clearState()
{
	for(uint i = 0; i < Input::EventBase::gMaxChannels; ++i)
		afterUpTime[i] = afterDownTime[i] = FloatConstants<Scalar>::big();
}

boolean DoubleClickAndTapHandler::onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state)
{
	const Input::EventKey* keyev = ev.castToKeyEvent();
	if(keyev)
	{
		if(keyev->key == Input::EKeyType_MouseLeft && keyev->down){
			if(afterDownTime[ev.channel] && ((downPosition[ev.channel] - pos) / screen->lenghts.lenghts[ELenghtType_RealLenght]).lenghtSquare() >  0.01f * 0.01f)
				afterUpTime[ev.channel] = 1e32f;

			afterDownTime[ev.channel] = 0;
			downPosition[ev.channel] = pos;
		}

		if(keyev->key == Input::EKeyType_MouseLeft && keyev->up)
		{
			if(afterDownTime[ev.channel] && ((downPosition[ev.channel] - pos) / screen->lenghts.lenghts[ELenghtType_RealLenght]).lenghtSquare() >  0.005f * 0.005f)
				afterDownTime[ev.channel] = afterUpTime[ev.channel] = 1e32f;

			Scalar upTime = afterUpTime[ev.channel];
			afterUpTime[ev.channel] = 0;

			if(upTime < 0.40f)
				if(onEvent(pos))
					return breakCursorBubbling;

			if(afterDownTime[ev.channel] < 0.25f)
				if(onSingleEvent(pos))
					return breakCursorBubbling;
			
		}
		return breakCursorBubbling;
	}

	const Input::EventTouch* touch = ev.castToTouchEvent();
	if(touch)
	{
		if(touch->down){
			if(afterDownTime[ev.channel] && ((downPosition[ev.channel] - pos) / screen->lenghts.lenghts[ELenghtType_RealLenght]).lenghtSquare() >  0.01f * 0.01f)
				afterUpTime[ev.channel] = 1e32f;

			afterDownTime[ev.channel] = 0;
			downPosition[ev.channel] = pos;
		}

		if(touch->up)
		{
			if(afterDownTime[ev.channel] && ((downPosition[ev.channel] - pos) / screen->lenghts.lenghts[ELenghtType_RealLenght]).lenghtSquare() >  0.005f * 0.005f)
				afterDownTime[ev.channel] = afterUpTime[ev.channel] = 1e32f;

			Scalar upTime = afterUpTime[ev.channel];
			afterUpTime[ev.channel] = 0;

			if(afterDownTime[ev.channel] < 0.35f)
			{
				for(uint i = 0; i < Input::EventBase::gMaxChannels; ++i)
				{
					if(i == ev.channel)
						continue;
					if(afterDownTime[i] < 0.35f && afterUpTime[i] <= afterDownTime[i]){
						if(onDoubleFingersSingleClick(pos, downPosition[i]))
							return breakCursorBubbling;
						break;
					}
				}
			}

			if(upTime < 0.50f)
				if(onEvent(pos))
					return breakCursorBubbling;

			if(afterDownTime[ev.channel] < 0.35f)
				if(onSingleEvent(pos))
					return breakCursorBubbling;
		}
		return breakCursorBubbling;
	}

	return breakCursorBubbling;
}

void DoubleClickAndTapHandler::onUpdate(Scalar dt)
{
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
	{
		afterUpTime[i] += dt;
		afterDownTime[i] += dt;
	}
}


boolean DoubleClickAndTapHandler::onEvent(const Vector& pos)
{
	boolean ret = doubleClickEvent.callOrAll(*this, pos);
	if(!doubleClickMethodName.empty()){
		IReflective* p = getModelInterface();
		if(p)
		{
			boolean r = false;
			p->reflection().reflectionByPath(doubleClickMethodName).rcall(r, *this, pos);
			ret |= r;
		}
	}
	return ret;
}

boolean DoubleClickAndTapHandler::onSingleEvent(const Vector& pos)
{
	boolean ret = singleClickEvent.callOrAll(*this, pos);
	if(!singleClickMethodName.empty()){
		IReflective* p = getModelInterface();
		if(p)
		{
			boolean r = false;
			p->reflection().reflectionByPath(singleClickMethodName).rcall(r, *this, pos);
			ret |= r;
		}
	}
	return ret;
}

boolean DoubleClickAndTapHandler::onDoubleFingersSingleClick(const Vector& a, const Vector& b)
{
	boolean ret = doubleFingersSingleClickEvent.callOrAll(*this, a, b);
	if(!doubleFingersSingleClickMethodName.empty()){
		IReflective* p = getModelInterface();
		if(p)
		{
			boolean r = false;
			p->reflection().reflectionByPath(doubleFingersSingleClickMethodName).rcall(r, *this, a, b);
			ret |= r;
		}
	}
	return ret;
}

}
