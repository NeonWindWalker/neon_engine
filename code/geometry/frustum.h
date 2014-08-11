#pragma once

#include "minmax_asm.h"
#include "frustum_asm.h"

namespace Geometry {

class Frustum : public SFrustum
{
public:
	INLINE Frustum() {}
	INLINE Frustum(const Matrix34f& place, float tanHalfHor, float tanHalfVer, float farr){ set(place, tanHalfHor, tanHalfVer, farr); }
	INLINE void set(const Matrix34f& place, float tanHalfHor, float tanHalfVer, float farr)
	{
		this->edge[0] = place.m33() * Vector3f(-tanHalfHor, -tanHalfVer, -1.0f);
		this->edge[1] = place.m33() * Vector3f(-tanHalfHor,  tanHalfVer, -1.0f);
		this->edge[2] = place.m33() * Vector3f( tanHalfHor,  tanHalfVer, -1.0f);
		this->edge[3] = place.m33() * Vector3f( tanHalfHor, -tanHalfVer, -1.0f);
		this->edge[4] = place.x;
		this->edge[5] = place.y;
		
		this->plane[0].pxyz() = this->edge[1].cross(this->edge[0]).normalFast(); this->plane[0].w = -this->plane[0].xyz().dot(place.p);
		this->plane[1].pxyz() = this->edge[2].cross(this->edge[1]).normalFast(); this->plane[1].w = -this->plane[1].xyz().dot(place.p);
		this->plane[2].pxyz() = this->edge[3].cross(this->edge[2]).normalFast(); this->plane[2].w = -this->plane[2].xyz().dot(place.p);
		this->plane[3].pxyz() = this->edge[0].cross(this->edge[3]).normalFast(); this->plane[3].w = -this->plane[3].xyz().dot(place.p);
		this->plane[4].pxyz() = -place.z.normalFast();
		this->plane[4].w = - this->plane[4].xyz().dot(place.p) - farr;

		this->vertex[0] = this->edge[0]*farr + place.p;
		this->vertex[1] = this->edge[1]*farr + place.p;
		this->vertex[2] = this->edge[2]*farr + place.p;
		this->vertex[3] = this->edge[3]*farr + place.p;
		this->vertex[4] = place.p;

		SMinMax3f bnd;
		minMaxBuild(bnd.min, bnd.max, this->vertex, this->vertex + 5);
		convert(this->aabb, bnd);

		this->radiys = this->vertex[4].distanceTo(this->vertex[0]);
	}

	INLINE boolean contain(const Vector3f& point)const { return Geometry::contain(*this, point); }
	INLINE boolean contain(const SRadial3f& sp)const { return Geometry::contain(*this, sp); }
	INLINE boolean contain(const SAABB3f& aabb)const { return Geometry::contain(*this, aabb); }
	INLINE boolean contain(const Matrix34f& obb)const { return Geometry::contain(*this, obb); }
	INLINE boolean contain(const SFrustum& fr)const { return Geometry::contain(*this, fr); }

	INLINE boolean intersect(const SRadial3f& sp)const { return Geometry::intersect(*this, sp); }
	INLINE boolean intersect(const SAABB3f& aabb)const { return Geometry::intersect(*this, aabb); }
	INLINE boolean intersect(const Matrix34f& obb)const { return Geometry::intersect(*this, obb); }
	INLINE boolean intersect(const SFrustum& fr)const { return Geometry::intersect(*this, fr); }

	INLINE boolean intersectFast(const SRadial3f& sp)const { return Geometry::intersectFast(*this, sp); }
	INLINE boolean intersectFast(const SAABB3f& aabb)const { return Geometry::intersectFast(*this, aabb); }
	INLINE boolean intersectFast(const Matrix34f& obb)const { return Geometry::intersectFast(*this, obb); }
	INLINE boolean intersectFast(const SFrustum& fr)const { return Geometry::intersectFast(*this, fr); }
};


}
