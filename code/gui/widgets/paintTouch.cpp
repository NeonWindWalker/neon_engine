#include "paintTouch.h"

namespace GUI
{

PaintTouch::PaintTouch(Widget& inFather) : Widget(inFather)
{
	beforePaintingTouchDelay = 0;
	beforePaintingTouchTime = 0;
	flashing = false;
	sliding = false;
	bindex = 0;
	scaleAccum = 1;
	point.setZero();
	moveDeltha.setZero();
	scale = 1;
	exScale = 1;
	exScalePoint.setZero();
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
		actived[i] = false;

	scaleStep = 0;
	scaleNivilation = 0;
	//scaleStep = Scalar(1.125);
	//scaleNivilation = Scalar(0.2);
	transparentInput = false;
}

boolean PaintTouch::onCursorBubbling(const Input::EventBase& ev, const Vector& inPos, const CursorState& state)
{
	Vector pos = inPos - position;
	cursorPosition[ev.channel][bindex] = pos;

	const Input::EventMotion* motev = ev.castToMotionEvent();
	if(motev && motev->dz != 0)
	{
		exScale *= clamp(Scalar(1) + Scalar(motev->dz) * Scalar(0.025), Scalar(1.0 / 1.125),Scalar(1.125));
		exScalePoint = pos;
		return !transparentInput;
	}

	const Input::EventKey* keyev = ev.castToKeyEvent();
	if(keyev)
	{
		if(keyev->key == Input::EKeyType_ArrowIn && keyev->down){
			exScale *= 1.0f / 1.125f;
			exScalePoint = pos;
			return !transparentInput;
		}
		
		if(keyev->key == Input::EKeyType_ArrowOut && keyev->down){
			exScale *= 1.125f;
			exScalePoint = pos;
			return !transparentInput;
		}
		
		if(keyev->key == Input::EKeyType_MouseLeft && keyev->down){
			actived[ev.channel] = true;
			cursorPosition[ev.channel][!bindex] = pos;
			return !transparentInput;
		}

		if(keyev->key == Input::EKeyType_MouseRight && keyev->down){
			actived[ev.channel] = 2;
			cursorPosition[ev.channel][!bindex] = pos;
			return !transparentInput;
		}

		if((keyev->key == Input::EKeyType_MouseLeft || keyev->key == Input::EKeyType_MouseRight) && keyev->up){
			actived[ev.channel] = false;
			return !transparentInput;
		}
	}

	const Input::EventTouch* touch = ev.castToTouchEvent();
	if(touch)
	{
		if(touch->up){
			actived[ev.channel] = false;
			return !transparentInput;
		}
		if(touch->down)
		{
			actived[ev.channel] = 3;
			cursorPosition[ev.channel][!bindex] = pos;
			return !transparentInput;
		}
	}
	return false;
}

void PaintTouch::onCursorLeave(const CursorState& state, uint channel)
{
	actived[channel] = false;
}

void PaintTouch::onUpdate(Scalar dt)
{
	if(scaleAccum > Scalar(1))
		scaleAccum = max(Scalar(1), scaleAccum - scaleNivilation * dt);
	else
		scaleAccum = min(Scalar(1), scaleAccum + scaleNivilation * dt);

	Vector centerD(Scalar(0), Scalar(0));
	Vector curCenter(Scalar(0), Scalar(0));
	Array<uint> activedChannels;
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
		if(actived[i]){
			curCenter += cursorPosition[i][bindex];
			centerD += cursorPosition[i][bindex] - cursorPosition[i][!bindex];
			activedChannels.push(i);
		}
	
	if(activedChannels.empty())
	{
		flashing = false;
		sliding = false;
		moveDeltha.setZero();
		scale = 1;
		beforePaintingTouchTime = 0;
	}
	else
	{
		curCenter /= (Scalar)activedChannels.size();
		centerD /= (Scalar)activedChannels.size();

		point = curCenter;
		moveDeltha = centerD;

		if(activedChannels.size() == 1)
		{
			if(actived[activedChannels[0]] == 3)
			{
				beforePaintingTouchTime += dt;
				if(beforePaintingTouchTime >= beforePaintingTouchDelay)
				{
					flashing = true;
					sliding = false;
				}
				else
				{
					flashing = false;
					sliding = false;
				}
			}
			else if(actived[activedChannels[0]] == 2)
			{
				flashing = false;
				sliding = true;
				beforePaintingTouchTime = 0;
			}
			else
			{
				flashing = true;
				sliding = false;
				beforePaintingTouchTime = 0;
			}
			scale = 1;
		}
		else
		{
			beforePaintingTouchTime = 0;

			flashing = false;
			sliding = true;

			Scalar summ = 0;
			for(uint* it = activedChannels.first(); it != activedChannels.end(); ++it)
			{
				Vector d = cursorPosition[*it][bindex] - cursorPosition[*it][!bindex] - moveDeltha;
				Vector r = cursorPosition[*it][bindex] - point;
				if(r.lenghtSquare() < FloatConstants<Scalar>::small())
					continue;
				summ += d.dot(r) / r.lenghtSquare();
			}
			if(scaleStep > gcfEpsilon)
			{
				Scalar k = clamp(Scalar(1) + summ, Scalar(1.0 / scaleStep), scaleStep);
				scaleAccum *= k;
				if(scaleAccum < Scalar(1.0 / scaleStep) || scaleAccum > scaleStep){
					scale = scaleAccum;
					scaleAccum = 1;
				}
				else scale = 1;
			}
			else
			{
				scale = Scalar(1) + summ;
				scaleAccum = 1;
			}
		}
	}

	bindex = !bindex;
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
		cursorPosition[i][bindex] = cursorPosition[i][!bindex];

	if(exScale != 1)
	{
		if(!sliding && !flashing)
			point = exScalePoint;
		sliding = true;
		scale *= exScale;
	}
	exScale = 1;
	exScalePoint.setZero();
}

void PaintTouch::onHide()
{
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
		actived[i] = false;
}

}



