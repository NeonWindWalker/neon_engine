#pragma once

#include "aabb_asm.h"

namespace Geometry 
{

class AABB3f : public SAABB3f
{
public:
	typedef float Scalar_t;

	INLINE AABB3f() {}
	INLINE AABB3f(const Vector3f& srcCenter, const Vector3f& srcHalfSize) : SAABB3f(srcCenter, srcHalfSize) {}
	INLINE void set(const Vector3f& srcCenter, const Vector3f& srcHalfSize){ c = srcCenter; h = srcHalfSize; }

	INLINE boolean operator == (const AABB3f& other)const						{ return this->c == other.c && this->h == other.h; }
	INLINE boolean operator != (const AABB3f& other)const						{ return this->c != other.c || this->h != other.h; }

	INLINE Vector3f size()const { return 2.0f * h; }
	INLINE float width()const { return 2.0f * h.x; }
	INLINE float height()const { return 2.0f * h.y; }
	INLINE float depth()const { return 2.0f * h.z; }

	INLINE boolean contain(const Vector3f& point)const { return Geometry::contain(*this, point); }
	INLINE boolean contain(const SLineSegment3f& other)const { return Geometry::contain(*this, other); }
	INLINE boolean contain(const STriangle3f& other)const { return Geometry::contain(*this, other); }
	INLINE boolean contain(const SAABB3f& other)const { return Geometry::contain(*this, other); }
	INLINE boolean contain(const SOBB3f& other)const { return Geometry::contain(*this, other); }
	INLINE boolean contain(const SRadial3f& other)const { return Geometry::contain(*this, other); }
	INLINE boolean contain(const SFrustum& other)const { return Geometry::contain(*this, other); }

	INLINE boolean intersect(const AABB3f& other)const { return Geometry::intersect(*this, other); }
};

}