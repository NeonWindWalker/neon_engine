#pragma once

#include "seed.h"

namespace GUI
{

class IDrawer
{
public:
	virtual void drawWidget(const Bound& bound, const Vector& pos, const Vector& size, ConstString image, const Vector& scale, const Vector& offset, const Vector4f& color = Vector4f(1,1,1,1))=0;

	virtual boolean setTexture(ConstString image)=0;
	virtual void drawFontQuad(const Vector& vmin, const Vector& vmax, const Vector& tmin, const Vector& tmax, const Color& color, uint byteNumber, uint bitNumber, uint bitLenght)=0;
};

struct ParentDrawInfo
{
	Bound bound;
	Vector offset;
	Vector scale;
	//ColorMatrix colorModification;

	INLINE ParentDrawInfo()
	{
		bound.set(0,0,0,0);
		offset.setZero();
		scale.setOne();
	}
};

static INLINE boolean clip(Bound& outV, Bound& outT, const Bound& bound, const Bound& v, const Bound& t )
{
	if(v.intersection(outV, bound)){
		Vector k = t.size() / v.size();
		outT.min =  (outV.min - v.min) * k + t.min;
		outT.max =  (outV.max - v.max) * k + t.max;
		return true;
	}
	return false;
}

static INLINE boolean clipTwiddled(Bound& outV, Bound& outT, const Bound& bound, const Bound& v, const Bound& t )
{
	if(v.intersection(outV, bound)){
		Vector m = (outV.min - v.min) / v.size();
		Vector M = (outV.max - v.max) / v.size();
		swap(m.x, m.y);
		swap(M.x, M.y);
		outT.min = m * t.size() + t.min;
		outT.max = M * t.size() + t.max;
		return true;
	}
	return false;
}

}