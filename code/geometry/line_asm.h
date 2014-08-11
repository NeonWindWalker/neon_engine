#pragma once

#include "base.h"

namespace Geometry
{

static INLINE float distanceToLineSegment(const Vector3f& p, const Vector3f& a, const Vector3f& b)
{
	Vector3f d = b - a;
	Vector3f ap = p - a;
	float t = ap.dot(d) / d.lenghtSquare();
	t = clamp01(t);
	return p.distanceTo(a + d * t);
}

static INLINE boolean intersectionLineAndLine(Vector3f& outP, float& outT1, float& outT2, const Vector3f& p1, const Vector3f& d1, const Vector3f& p2, const Vector3f& d2)
{
	float d1d2 = d1.dot(d2);
	float d1d1 = d1.lenghtSquare();
	float d2d2 = d2.lenghtSquare();
	float X = d1d1 * d2d2 - d1d2 * d1d2;
	if(X < Base::FloatConstants<float>::epsilon() * d1d1 * d2d2)
		return false;
	X = 1.0f / X;

	Vector3f p1p2 = p2 - p1;
	float d1_p1p2 = d1.dot(p1p2);
	float d2_p1p2 = d2.dot(p1p2);
	outT1 = (-d1d2 * d2_p1p2 + d2d2 * d2_p1p2) * X;
	outT2 = ( d1d2 * d1_p1p2 - d1d1 * d2_p1p2) * X;
	outP = outT1 * d1 + p1;
	return true;
}

static INLINE boolean intersectionLineAndLine(Vector3f& outP, const Vector3f& p1, const Vector3f& d1, const Vector3f& p2, const Vector3f& d2)
{
	float t1,t2;
	return intersectionLineAndLine(outP,t1,t2,p1,d1,p2,d2);
}

static INLINE boolean intersectionRayAndRay(Vector3f& outP, float& outT1, float& outT2, const Vector3f& p1, const Vector3f& d1, const Vector3f& p2, const Vector3f& d2)
{
	return intersectionLineAndLine(outP,outT1,outT2,p1,d1,p2,d2) && outT1 >= 0.0f && outT2 >= 0.0f;
}

static INLINE boolean intersectionRayAndRay(Vector3f& outP, const Vector3f& p1, const Vector3f& d1, const Vector3f& p2, const Vector3f& d2)
{
	float t1,t2;
	return intersectionRayAndRay(outP,t1,t2,p1,d1,p2,d2);
}

}