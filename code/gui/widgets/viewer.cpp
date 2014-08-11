#include "viewer.h"

namespace GUI
{

Viewer::Viewer(Widget& inFather) : Widget(inFather)
{
	zoomable = true;
	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)	
		actived[i] = false;
	bindex = 0;
	scaleAccum = 1.0f;
	scaleStep = Scalar(1.125);
	scaleNivilation = Scalar(0.2);
	inertion = 0;

	minScale = Scalar(1e-12);
	maxScale = Scalar(1e12);
	viewClampMin.setZero();
	viewClampMax.setZero();
	viewClampEnable = false;
	viewClampFarMode = false;
	reallyMinScale = minScale;
	reallyMaxScale = maxScale;
}

boolean Viewer::onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state)
{
	cursorPosition[ev.channel][bindex] = pos;

	const Input::EventMotion* motev = ev.castToMotionEvent();
	if(motev && motev->dz != 0)
	{
		Scalar k = clamp(Scalar(1) + Scalar(motev->dz) * Scalar(0.025), Scalar(1.0 / 1.125),Scalar(1.125));
		scale(k, pos);
		return true;
	}

	const Input::EventKey* keyev = ev.castToKeyEvent();
	if(keyev)
	{
		if(keyev->key == Input::EKeyType_ArrowOut && keyev->down){
			scale(1.0f / 1.125f, pos);
			return true;
		}
		
		if(keyev->key == Input::EKeyType_ArrowIn && keyev->down){
			scale(1.125f, pos);
			return true;
		}
		
		if(keyev->key == Input::EKeyType_MouseLeft)
		{
			if(keyev->up)
			{
				actived[ev.channel] = false;
				return true;
			}

			if(keyev->down)
			{
				cursorPosition[ev.channel][!bindex] = pos;
				actived[ev.channel] = true;
				return true;
			}
		}
	}
	
	const Input::EventTouch* tchev = ev.castToTouchEvent();
	if(tchev)
	{
		if(tchev->up){
			actived[ev.channel] = false;
			return true;
		}

		if(tchev->down){
			actived[ev.channel] = true;
			cursorPosition[ev.channel][!bindex] = pos;
			return true;
		}
	}

	return false;
}

void Viewer::onCursorLeave(const CursorState& state, uint channel)
{
	actived[channel] = false;
}

void Viewer::onUpdate(Scalar dt)
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
			activedChannels.push(i);
			curCenter += cursorPosition[i][bindex];
			centerD += cursorPosition[i][bindex] - cursorPosition[i][!bindex];
		}
	if(!activedChannels.empty())
	{
		curCenter /= (Scalar)activedChannels.size();
		centerD /= (Scalar)activedChannels.size();
		moveVelosity = centerD / dt * Scalar(0.25) + moveVelosity * Scalar(0.75);

		childRectOffset += centerD;
		if(activedChannels.size() > 1 && zoomable)
		{
			Scalar summ = 0;
			for(uint* it = activedChannels.first(); it != activedChannels.end(); ++it)
			{
				Vector d = cursorPosition[*it][bindex] - cursorPosition[*it][!bindex] - centerD;
				Vector r = cursorPosition[*it][bindex] - curCenter;
				if(r.lenghtSquare() < FloatConstants<Scalar>::small())
					continue;
				summ += r.dot(d) / r.lenghtSquare();
			}
			summ /= (Scalar)activedChannels.size();

			if(scaleStep > Scalar(1))
			{
				Scalar k = clamp(Scalar(1) + summ, Scalar(1.0 / scaleStep), scaleStep);
				scaleAccum *= k;
				if(scaleAccum < Scalar(1.0 / scaleStep) || scaleAccum > scaleStep){
					scale(scaleAccum, curCenter);
					scaleAccum = 1;
				}
			}
			else
			{
				scaleAccum *= Scalar(1) + summ;
				scale(scaleAccum, curCenter);
				scaleAccum = 1;
			}
			
		}
	}
	else if(inertion > 0)
	{
		moveVelosity *= exp(-dt / inertion);
		childRectOffset += moveVelosity * dt;
	}
	else moveVelosity.setZero();

	for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
		cursorPosition[i][!bindex] = cursorPosition[i][bindex];
	bindex = !bindex;

	if(viewClampEnable)
	{
		Vector clientSize = childBound().size();
		Vector s = clientSize / (viewClampMax - viewClampMin);
		reallyMinScale = viewClampFarMode ? min(s.x, s.y) : max(s.x, s.y);
		maximize(reallyMinScale, minScale);
		reallyMaxScale = max(maxScale, reallyMinScale);

		maximize(childRectScale, Vector(reallyMinScale, reallyMinScale));
		minimize(childRectScale, Vector(reallyMaxScale, reallyMaxScale));

		Vector offsetLow = - childRectScale * viewClampMin;
		Vector offsetHi = clientSize - childRectScale * viewClampMax;
		if(offsetLow.x >= offsetHi.x)
			childRectOffset.x = clamp(childRectOffset.x, offsetHi.x, offsetLow.x);
		else
			childRectOffset.x = (offsetLow.x + offsetHi.x) * 0.5f;

		if(offsetLow.y >= offsetHi.y)
			childRectOffset.y = clamp(childRectOffset.y, offsetHi.y, offsetLow.y);
		else
			childRectOffset.y = (offsetLow.y + offsetHi.y) * 0.5f;
	}
	else
	{
		reallyMinScale = minScale;
		reallyMaxScale = maxScale;
		maximize(childRectScale, Vector(reallyMinScale, reallyMinScale));
		minimize(childRectScale, Vector(reallyMaxScale, reallyMaxScale));
	}
}

}