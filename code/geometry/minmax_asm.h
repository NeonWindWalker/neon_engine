#pragma once

#include "base.h"

namespace Geometry {

INLINE boolean minMaxContainPoint(const Vector2f& min, const Vector2f& max, const Vector2f& point)
{
	return Base::minMaxContainPoint(min.x, max.x, point.x) && Base::minMaxContainPoint(min.y, max.y, point.y);
}

INLINE boolean contain(const SMinMax2f& mm, const Vector2f& point)
{
	return minMaxContainPoint(mm.min, mm.max, point);
}

INLINE boolean minMaxContainPoint(const Vector2i& min, const Vector2i& max, const Vector2i& point)
{
	return Base::minMaxContainPoint(min.x, max.x, point.x) && Base::minMaxContainPoint(min.y, max.y, point.y);
}

INLINE boolean contain(const SMinMax2i& mm, const Vector2i& point)
{
	return minMaxContainPoint(mm.min, mm.max, point);
}

INLINE boolean minMaxContainPoint(const Vector3f& min, const Vector3f& max, const Vector3f& point)
{
	return Base::minMaxContainPoint(min.x, max.x, point.x) && Base::minMaxContainPoint(min.y, max.y, point.y) && Base::minMaxContainPoint(min.z, max.z, point.z);
}

INLINE boolean contain(const SMinMax3f& mm, const Vector3f& point)
{
	return minMaxContainPoint(mm.min, mm.max, point);
}

INLINE boolean minMaxContainPoint(const Vector4f& min, const Vector4f& max, const Vector4f& point)
{
	return Base::minMaxContainPoint(min.x, max.x, point.x) && Base::minMaxContainPoint(min.y, max.y, point.y) && Base::minMaxContainPoint(min.z, max.z, point.z) && Base::minMaxContainPoint(min.w, max.w, point.w);
}

INLINE boolean contain(const SMinMax4f& mm, const Vector4f& point)
{
	return minMaxContainPoint(mm.min, mm.max, point);
}

INLINE boolean minMaxContainMinMax(const Vector2f& aMin, const Vector2f& aMax, const Vector2f& bMin, const Vector2f& bMax)
{
	return Base::minMaxContainMinMax(aMin.x, aMax.x, bMin.x, bMax.x) && Base::minMaxContainMinMax(aMin.y, aMax.y, bMin.y, bMax.y);
}

INLINE boolean contain(const SMinMax2f& a, const SMinMax2f& b)
{
	return minMaxContainMinMax(a.min, a.max, b.min, b.max);
}

INLINE boolean minMaxContainMinMax(const Vector2i& aMin, const Vector2i& aMax, const Vector2i& bMin, const Vector2i& bMax)
{
	return Base::minMaxContainMinMax(aMin.x, aMax.x, bMin.x, bMax.x) && Base::minMaxContainMinMax(aMin.y, aMax.y, bMin.y, bMax.y);
}

INLINE boolean contain(const SMinMax2i& a, const SMinMax2i& b)
{
	return minMaxContainMinMax(a.min, a.max, b.min, b.max);
}

INLINE boolean minMaxContainMinMax(const Vector3f& aMin, const Vector3f& aMax, const Vector3f& bMin, const Vector3f& bMax)
{
	return Base::minMaxContainMinMax(aMin.x, aMax.x, bMin.x, bMax.x) && Base::minMaxContainMinMax(aMin.y, aMax.y, bMin.y, bMax.y)  && Base::minMaxContainMinMax(aMin.z, aMax.z, bMin.z, bMax.z);
}

INLINE boolean contain(const SMinMax3f& a, const SMinMax3f& b)
{
	return minMaxContainMinMax(a.min, a.max, b.min, b.max);
}

INLINE boolean minMaxContainMinMax(const Vector4f& aMin, const Vector4f& aMax, const Vector4f& bMin, const Vector4f& bMax)
{
	return Base::minMaxContainMinMax(aMin.x, aMax.x, bMin.x, bMax.x) && Base::minMaxContainMinMax(aMin.y, aMax.y, bMin.y, bMax.y)  && Base::minMaxContainMinMax(aMin.z, aMax.z, bMin.z, bMax.z) && Base::minMaxContainMinMax(aMin.w, aMax.w, bMin.w, bMax.w);
}

INLINE boolean contain(const SMinMax4f& a, const SMinMax4f& b)
{
	return minMaxContainMinMax(a.min, a.max, b.min, b.max);
}

INLINE boolean intersectMinMaxAndMinMax(const Vector2f& aMin, const Vector2f& aMax, const Vector2f& bMin, const Vector2f& bMax)
{
	return Base::intersectMinMaxAndMinMax(aMin.x, aMax.x, bMin.x, bMax.x) && Base::intersectMinMaxAndMinMax(aMin.y, aMax.y, bMin.y, bMax.y);
}

INLINE boolean intersect(const SMinMax2f& a, const SMinMax2f& b)
{
	return intersectMinMaxAndMinMax(a.min, a.max, b.min, b.max);
}

INLINE boolean intersectMinMaxAndMinMax(const Vector2i& aMin, const Vector2i& aMax, const Vector2i& bMin, const Vector2i& bMax)
{
	return Base::intersectMinMaxAndMinMax(aMin.x, aMax.x, bMin.x, bMax.x) && Base::intersectMinMaxAndMinMax(aMin.y, aMax.y, bMin.y, bMax.y);
}

INLINE boolean intersect(const SMinMax2i& a, const SMinMax2i& b)
{
	return intersectMinMaxAndMinMax(a.min, a.max, b.min, b.max);
}

INLINE boolean intersectMinMaxAndMinMax(const Vector3f& aMin, const Vector3f& aMax, const Vector3f& bMin, const Vector3f& bMax)
{
	return Base::intersectMinMaxAndMinMax(aMin.x, aMax.x, bMin.x, bMax.x) && Base::intersectMinMaxAndMinMax(aMin.y, aMax.y, bMin.y, bMax.y) && Base::intersectMinMaxAndMinMax(aMin.z, aMax.z, bMin.z, bMax.z);
}

INLINE boolean intersect(const SMinMax3f& a, const SMinMax3f& b)
{
	return intersectMinMaxAndMinMax(a.min, a.max, b.min, b.max);
}

INLINE boolean intersectMinMaxAndMinMax(const Vector4f& aMin, const Vector4f& aMax, const Vector4f& bMin, const Vector4f& bMax)
{
	return Base::intersectMinMaxAndMinMax(aMin.x, aMax.x, bMin.x, bMax.x) && Base::intersectMinMaxAndMinMax(aMin.y, aMax.y, bMin.y, bMax.y) && Base::intersectMinMaxAndMinMax(aMin.z, aMax.z, bMin.z, bMax.z)  && Base::intersectMinMaxAndMinMax(aMin.w, aMax.w, bMin.w, bMax.w);
}

INLINE boolean intersect(const SMinMax4f& a, const SMinMax4f& b)
{
	return intersectMinMaxAndMinMax(a.min, a.max, b.min, b.max);
}

INLINE boolean intersectionMinMaxAndMinMax(Vector2f& min, Vector2f& max, const Vector2f& aMin, const Vector2f& aMax, const Vector2f& bMin, const Vector2f& bMax)
{
	return Base::intersectionMinMaxAndMinMax<float>(min.x, max.x, aMin.x, aMax.x, bMin.x, bMax.x) && Base::intersectionMinMaxAndMinMax<float>(min.y, max.y, aMin.y, aMax.y, bMin.y, bMax.y);
}

INLINE boolean intersection(SMinMax2f& res, const SMinMax2f& a, const SMinMax2f& b)
{
	return intersectionMinMaxAndMinMax(res.min, res.max, a.min, a.max, b.min, b.max);
}

INLINE boolean intersectionMinMaxAndMinMax(Vector2i& min, Vector2i& max, const Vector2i& aMin, const Vector2i& aMax, const Vector2i& bMin, const Vector2i& bMax)
{
	return Base::intersectionMinMaxAndMinMax<int>(min.x, max.x, aMin.x, aMax.x, bMin.x, bMax.x) && Base::intersectionMinMaxAndMinMax<int>(min.y, max.y, aMin.y, aMax.y, bMin.y, bMax.y);
}

INLINE boolean intersection(SMinMax2i& res, const SMinMax2i& a, const SMinMax2i& b)
{
	return intersectionMinMaxAndMinMax(res.min, res.max, a.min, a.max, b.min, b.max);
}

INLINE boolean intersectionMinMaxAndMinMax(Vector3f& min, Vector3f& max, const Vector3f& aMin, const Vector3f& aMax, const Vector3f& bMin, const Vector3f& bMax)
{
	return Base::intersectionMinMaxAndMinMax<float>(min.x, max.x, aMin.x, aMax.x, bMin.x, bMax.x) && Base::intersectionMinMaxAndMinMax<float>(min.y, max.y, aMin.y, aMax.y, bMin.y, bMax.y) && Base::intersectionMinMaxAndMinMax<float>(min.z, max.z, aMin.z, aMax.z, bMin.z, bMax.z);
}

INLINE boolean intersection(SMinMax3f& res, const SMinMax3f& a, const SMinMax3f& b)
{
	return intersectionMinMaxAndMinMax(res.min, res.max, a.min, a.max, b.min, b.max);
}

INLINE boolean intersectionMinMaxAndMinMax(Vector4f& min, Vector4f& max, const Vector4f& aMin, const Vector4f& aMax, const Vector4f& bMin, const Vector4f& bMax)
{
	return Base::intersectionMinMaxAndMinMax<float>(min.x, max.x, aMin.x, aMax.x, bMin.x, bMax.x) && Base::intersectionMinMaxAndMinMax<float>(min.y, max.y, aMin.y, aMax.y, bMin.y, bMax.y) && Base::intersectionMinMaxAndMinMax<float>(min.z, max.z, aMin.z, aMax.z, bMin.z, bMax.z) && Base::intersectionMinMaxAndMinMax<float>(min.w, max.w, aMin.w, aMax.w, bMin.w, bMax.w);
}

INLINE boolean intersection(SMinMax4f& res, const SMinMax4f& a, const SMinMax4f& b)
{
	return intersectionMinMaxAndMinMax(res.min, res.max, a.min, a.max, b.min, b.max);
}

INLINE void minMaxIncludePoint(Vector2f& min, Vector2f& max, const Vector2f& point)
{
	minimize(min, point);
	maximize(max, point);
}

INLINE void include(SMinMax2f& reciver, const Vector2f& point)
{
	minMaxIncludePoint(reciver.min, reciver.max, point);
}

INLINE void minMaxIncludePoint(Vector2i& min, Vector2i& max, const Vector2i& point)
{
	minimize(min, point);
	maximize(max, point);
}

INLINE void include(SMinMax2i& reciver, const Vector2i& point)
{
	minMaxIncludePoint(reciver.min, reciver.max, point);
}

INLINE void minMaxIncludePoint(Vector3f& min, Vector3f& max, const Vector3f& point)
{
	minimize(min, point);
	maximize(max, point);
}

INLINE void include(SMinMax3f& reciver, const Vector3f& point)
{
	minMaxIncludePoint(reciver.min, reciver.max, point);
}

INLINE void minMaxIncludePoint(Vector4f& min, Vector4f& max, const Vector4f& point)
{
	minimize(min, point);
	maximize(max, point);
}

INLINE void include(SMinMax4f& reciver, const Vector4f& point)
{
	minMaxIncludePoint(reciver.min, reciver.max, point);
}


INLINE void minMaxIncludeMinMax(Vector2f& min, Vector2f& max, const Vector2f& bMin, const Vector2f& bMax)
{
	minimize(min, bMin);
	maximize(max, bMax);
}

INLINE void include(SMinMax2f& reciver, const SMinMax2f& mm)
{
	minMaxIncludeMinMax(reciver.min, reciver.max, mm.min, mm.max);
}

INLINE void minMaxIncludeMinMax(Vector2i& min, Vector2i& max, const Vector2i& bMin, const Vector2i& bMax)
{
	minimize(min, bMin);
	maximize(max, bMax);
}

INLINE void include(SMinMax2i& reciver, const SMinMax2i& mm)
{
	minMaxIncludeMinMax(reciver.min, reciver.max, mm.min, mm.max);
}

INLINE void minMaxIncludeMinMax(Vector3f& min, Vector3f& max, const Vector3f& bMin, const Vector3f& bMax)
{
	minimize(min, bMin);
	maximize(max, bMax);
}

INLINE void include(SMinMax3f& reciver, const SMinMax3f& mm)
{
	minMaxIncludeMinMax(reciver.min, reciver.max, mm.min, mm.max);
}

INLINE void minMaxIncludeMinMax(Vector4f& min, Vector4f& max, const Vector4f& bMin, const Vector4f& bMax)
{
	minimize(min, bMin);
	maximize(max, bMax);
}

INLINE void include(SMinMax4f& reciver, const SMinMax4f& mm)
{
	minMaxIncludeMinMax(reciver.min, reciver.max, mm.min, mm.max);
}


INLINE void minMaxBuild(Vector2f& min, Vector2f& max, const Vector2f* v, const Vector2f* v_end)
{
	min = max = *v++;
	for(; v != v_end; ++v)
		minMaxIncludePoint(min, max, *v);
}

INLINE void build(SMinMax2f& reciver, const Vector2f* v, const Vector2f* v_end)
{
	minMaxBuild(reciver.min, reciver.max, v, v_end);
}

INLINE void minMaxBuild(Vector2i& min, Vector2i& max, const Vector2i* v, const Vector2i* v_end)
{
	min = max = *v++;
	for(; v != v_end; ++v)
		minMaxIncludePoint(min, max, *v);
}

INLINE void build(SMinMax2i& reciver, const Vector2i* v, const Vector2i* v_end)
{
	minMaxBuild(reciver.min, reciver.max, v, v_end);
}


INLINE void minMaxBuild(Vector3f& min, Vector3f& max, const Vector3f* v, const Vector3f* v_end)
{
	min = max = *v++;
	for(; v != v_end; ++v)
		minMaxIncludePoint(min, max, *v);
}


INLINE void build(SMinMax3f& reciver, const Vector3f* v, const Vector3f* v_end)
{
	minMaxBuild(reciver.min, reciver.max, v, v_end);
}


INLINE void minMaxBuild(Vector4f& min, Vector4f& max, const Vector4f* v, const Vector4f* v_end)
{
	min = max = *v++;
	for(; v != v_end; ++v)
		minMaxIncludePoint(min, max, *v);
}


INLINE void build(SMinMax4f& reciver, const Vector4f* v, const Vector4f* v_end)
{
	minMaxBuild(reciver.min, reciver.max, v, v_end);
}


INLINE void convert(SOBB3f& dst, const SMinMax3f& src)
{
	dst.p = (src.min + src.max) * float(0.5);
	Vector3f d = (src.max - src.min) * float(0.5);
	dst.x.set(d.x, 0.0f, 0.0f);
	dst.y.set(0.0f, d.x, 0.0f);
	dst.z.set(0.0f, 0.0f, d.z);
}

}
