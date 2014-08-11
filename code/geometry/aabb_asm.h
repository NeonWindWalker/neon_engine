#pragma once

#include "base.h"

namespace Geometry {

static INLINE boolean aabbContainPoint(const Vector3f& p, const Vector3f& s, const Vector3f& v)
{
	return diapasonContainPoint(p.x, s.x, v.x) && diapasonContainPoint(p.y, s.y, v.y) && diapasonContainPoint(p.z, s.z, v.z);
}

static INLINE boolean contain(const SAABB3f& aabb, const Vector3f& v)
{
	return aabbContainPoint(aabb.c, aabb.h, v);
}

static INLINE boolean aabbContainSegment(const Vector3f& p, const Vector3f& s, const Vector3f& a, const Vector3f& b)
{
	return aabbContainPoint(p, s, a) && aabbContainPoint(p, s, b);
}

static INLINE boolean contain(const SAABB3f& aabb, const SLineSegment3f& l)
{
	return aabbContainSegment(aabb.c, aabb.h, l.a, l.b);
}

static INLINE boolean aabbContainTriangle(const Vector3f& p, const Vector3f& s, const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	return aabbContainPoint(p, s, a) && aabbContainPoint(p, s, b) && aabbContainPoint(p, s, c);
}

static INLINE boolean contain(const SAABB3f& aabb, const STriangle3f& t)
{
	return aabbContainTriangle(aabb.c, aabb.h, t.a, t.b, t.c);
}

static INLINE boolean aabbContainAabb(const Vector3f& ap, const Vector3f& as, const Vector3f& bp, const Vector3f& bs)
{
	return diapasonContainDiapason(ap.x, as.x, bp.x, bs.x) && diapasonContainDiapason(ap.y, as.y, bp.y, bs.y) && diapasonContainDiapason(ap.z, as.z, bp.z, bs.z);
}

static INLINE boolean contain(const SAABB3f& aabb, const SAABB3f& otherAabb)
{
	return aabbContainAabb(aabb.c, aabb.h, otherAabb.c, otherAabb.h);
}

static INLINE boolean aabbContainRadial(const Vector3f& p, const Vector3f& s, const Vector3f& rp, float r)
{
	return diapasonContainDiapason<float>(p.x, s.x, rp.x, r) && diapasonContainDiapason<float>(p.y, s.y, rp.y, r) && diapasonContainDiapason<float>(p.z, s.z, rp.z, r);
}

static INLINE boolean contain(const SAABB3f& aabb, const SRadial3f& rad)
{
	return aabbContainRadial(aabb.c, aabb.h, rad.c, rad.r);
}

static INLINE boolean aabbContainObb(const Vector3f& p, const Vector3f& s, const SOBB3f& obb)
{
	return aabbContainAabb(p, s, obb.p, abs(obb.x) + abs(obb.y) + abs(obb.z));
}

static INLINE boolean contain(const SAABB3f& aabb, const SOBB3f& obb)
{
	return aabbContainObb(aabb.c, aabb.h, obb);
}

static INLINE boolean aabbContainFrustum(const Vector3f& p, const Vector3f& s, const SFrustum& fr)
{
	for(uint i=0; i < 5; ++i)
		if(!aabbContainPoint(p, s, fr.vertex[i]))
			return false;
	return true;
}

static INLINE boolean contain(const SAABB3f& aabb, const SFrustum& fr)
{
	return aabbContainFrustum(aabb.c, aabb.h, fr);
}


static INLINE boolean intersectAabbAndAabb(const Vector3f& c0, const Vector3f& s0, const Vector3f& c1, const Vector3f& s1)
{
	return ((c0 - c1) - s0 - s1).isAllNegative();
}

static INLINE boolean intersect(const SAABB3f& aabb0, const SAABB3f& aabb1)
{
	return intersectAabbAndAabb(aabb0.c, aabb0.h, aabb1.c, aabb1.h);
}


}
