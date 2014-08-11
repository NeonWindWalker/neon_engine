#pragma once

#include "base.h"

namespace Geometry {


static INLINE boolean frustumContainPoint(const Vector4f* rfPlane, const Vector3f& point)
{
	if(rfPlane[0].dot(point.xyz1()) > 0)return false;
	if(rfPlane[1].dot(point.xyz1()) > 0)return false;
	if(rfPlane[2].dot(point.xyz1()) > 0)return false;
	if(rfPlane[3].dot(point.xyz1()) > 0)return false;
	if(rfPlane[4].dot(point.xyz1()) > 0)return false;
	return true;
}


static INLINE boolean contain(const SFrustum& fr, const Vector3f& point)
{
	return frustumContainPoint(fr.plane, point);
}


static INLINE boolean frustumContainLineSegment(const Vector4f* rfPlane, const Vector3f& a, const Vector3f& b)
{
	if(rfPlane[0].dot(a.xyz1()) > 0)return false;
	if(rfPlane[0].dot(b.xyz1()) > 0)return false;
	if(rfPlane[1].dot(a.xyz1()) > 0)return false;
	if(rfPlane[1].dot(b.xyz1()) > 0)return false;
	if(rfPlane[2].dot(a.xyz1()) > 0)return false;
	if(rfPlane[2].dot(b.xyz1()) > 0)return false;
	if(rfPlane[3].dot(a.xyz1()) > 0)return false;
	if(rfPlane[3].dot(b.xyz1()) > 0)return false;
	if(rfPlane[4].dot(a.xyz1()) > 0)return false;
	if(rfPlane[4].dot(b.xyz1()) > 0)return false;
	return true;
}


static INLINE boolean contain(const SFrustum& fr, const SLineSegment3f& seg)
{
	return frustumContainLineSegment(fr.plane, seg.a, seg.b);
}



static INLINE boolean frustumContainTriangle(const Vector4f* rfPlane, const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	if(rfPlane[0].dot(a.xyz1()) > 0)return false;
	if(rfPlane[0].dot(b.xyz1()) > 0)return false;
	if(rfPlane[0].dot(c.xyz1()) > 0)return false;
	if(rfPlane[1].dot(a.xyz1()) > 0)return false;
	if(rfPlane[1].dot(b.xyz1()) > 0)return false;
	if(rfPlane[1].dot(c.xyz1()) > 0)return false;
	if(rfPlane[2].dot(a.xyz1()) > 0)return false;
	if(rfPlane[2].dot(b.xyz1()) > 0)return false;
	if(rfPlane[2].dot(c.xyz1()) > 0)return false;
	if(rfPlane[3].dot(a.xyz1()) > 0)return false;
	if(rfPlane[3].dot(b.xyz1()) > 0)return false;
	if(rfPlane[3].dot(c.xyz1()) > 0)return false;
	if(rfPlane[4].dot(a.xyz1()) > 0)return false;
	if(rfPlane[4].dot(b.xyz1()) > 0)return false;
	if(rfPlane[4].dot(c.xyz1()) > 0)return false;
	return true;
}


static INLINE boolean contain(const SFrustum& fr, const STriangle3f& tr)
{
	return frustumContainTriangle(fr.plane, tr.a, tr.b, tr.c);
}



static INLINE boolean frustumContainSphere(const Vector4f* rfPlane, const Vector3f& center, float radiys)
{
	if(rfPlane[0].dot(center.xyz1()) > -radiys)return false;
	if(rfPlane[1].dot(center.xyz1()) > -radiys)return false;
	if(rfPlane[2].dot(center.xyz1()) > -radiys)return false;
	if(rfPlane[3].dot(center.xyz1()) > -radiys)return false;
	if(rfPlane[4].dot(center.xyz1()) > -radiys)return false;
	return true;
}


static INLINE boolean contain(const SFrustum& fr, const SRadial3f& sp)
{
	return frustumContainSphere(fr.plane, sp.c, sp.r);
}


static INLINE boolean frustumContainAABB(const Vector4f* rfPlane, const Vector3f& center, const Vector3f& hsize)
{
	if(rfPlane[0].dot(center.xyz1()) > -(rfPlane[0].xyz() * hsize).componentAbsSumm())return false;
	if(rfPlane[1].dot(center.xyz1()) > -(rfPlane[1].xyz() * hsize).componentAbsSumm())return false;
	if(rfPlane[2].dot(center.xyz1()) > -(rfPlane[2].xyz() * hsize).componentAbsSumm())return false;
	if(rfPlane[3].dot(center.xyz1()) > -(rfPlane[3].xyz() * hsize).componentAbsSumm())return false;
	if(rfPlane[4].dot(center.xyz1()) > -(rfPlane[4].xyz() * hsize).componentAbsSumm())return false;
	return true;
}


static INLINE boolean contain(const SFrustum& fr, const SAABB3f& aabb)
{
	return frustumContainAABB(fr.plane, aabb.c, aabb.h);
}


static INLINE boolean frustumContainOBB(const Vector4f* rfPlane, const SOBB3f& box)
{
	if(rfPlane[0].dot(box.p.xyz1()) > -box.m33().tranMult(rfPlane[0].xyz()).componentAbsSumm())return false;
	if(rfPlane[1].dot(box.p.xyz1()) > -box.m33().tranMult(rfPlane[1].xyz()).componentAbsSumm())return false;
	if(rfPlane[2].dot(box.p.xyz1()) > -box.m33().tranMult(rfPlane[2].xyz()).componentAbsSumm())return false;
	if(rfPlane[3].dot(box.p.xyz1()) > -box.m33().tranMult(rfPlane[3].xyz()).componentAbsSumm())return false;
	if(rfPlane[4].dot(box.p.xyz1()) > -box.m33().tranMult(rfPlane[4].xyz()).componentAbsSumm())return false;
	return true;
}


static INLINE boolean contain(const SFrustum& fr, const SOBB3f& obb)
{
	return frustumContainOBB(fr.plane, obb);
}


static INLINE boolean frustumContainConvex(const Vector4f* rfPlane, const Vector3f* v, const Vector3f* v_end)
{
	for(; v != v_end; ++v){
		if(rfPlane[0].dot(v->xyz1()) > 0)return false;
		if(rfPlane[1].dot(v->xyz1()) > 0)return false;
		if(rfPlane[2].dot(v->xyz1()) > 0)return false;
		if(rfPlane[3].dot(v->xyz1()) > 0)return false;
		if(rfPlane[4].dot(v->xyz1()) > 0)return false;
	}
	return true;
}


static INLINE boolean frustumContainFrustum(const Vector4f* rfPlane, const Vector3f* v)
{
	if(rfPlane[0].dot(v[0].xyz1()) > 0)return false;
	if(rfPlane[0].dot(v[1].xyz1()) > 0)return false;
	if(rfPlane[0].dot(v[2].xyz1()) > 0)return false;
	if(rfPlane[0].dot(v[3].xyz1()) > 0)return false;
	if(rfPlane[0].dot(v[4].xyz1()) > 0)return false;
	if(rfPlane[1].dot(v[0].xyz1()) > 0)return false;
	if(rfPlane[1].dot(v[1].xyz1()) > 0)return false;
	if(rfPlane[1].dot(v[2].xyz1()) > 0)return false;
	if(rfPlane[1].dot(v[3].xyz1()) > 0)return false;
	if(rfPlane[1].dot(v[4].xyz1()) > 0)return false;
	if(rfPlane[2].dot(v[0].xyz1()) > 0)return false;
	if(rfPlane[2].dot(v[1].xyz1()) > 0)return false;
	if(rfPlane[2].dot(v[2].xyz1()) > 0)return false;
	if(rfPlane[2].dot(v[3].xyz1()) > 0)return false;
	if(rfPlane[2].dot(v[4].xyz1()) > 0)return false;
	if(rfPlane[3].dot(v[0].xyz1()) > 0)return false;
	if(rfPlane[3].dot(v[1].xyz1()) > 0)return false;
	if(rfPlane[3].dot(v[2].xyz1()) > 0)return false;
	if(rfPlane[3].dot(v[3].xyz1()) > 0)return false;
	if(rfPlane[3].dot(v[4].xyz1()) > 0)return false;
	if(rfPlane[4].dot(v[0].xyz1()) > 0)return false;
	if(rfPlane[4].dot(v[1].xyz1()) > 0)return false;
	if(rfPlane[4].dot(v[2].xyz1()) > 0)return false;
	if(rfPlane[4].dot(v[3].xyz1()) > 0)return false;
	if(rfPlane[4].dot(v[4].xyz1()) > 0)return false;
	return true;
}


static INLINE boolean contain(const SFrustum& fr, const SFrustum& fr2)
{
	return frustumContainFrustum(fr.plane, fr2.vertex);
}





static INLINE boolean betveenChange(Vector3f& e, const Vector3f& a, const Vector3f& b, const Vector3f& axb)
{
	//Assert(e.dot(axb) < 0.01f * axb.lenght() * e.lenght());

	//Vector3f axb2 = a.cross(b);
	//Assert(equals((axb2 & axb) * (axb2 & axb) , axb2.lenghtSquare() * axb.lenghtSquare(), axb2.lenghtSquare() * axb.lenghtSquare() * 0.01f));
	
	float crdt = a.cross(e).dot(axb);

	//if(xabs(crdt) < 1e-6f)
	//	return false;

	if(crdt < 0)
		e = -e;
	return b.cross(e).dot(axb) < 0;

	/*if(ret)
	{
		Vector3 cr = a.cross(e);
		Assert(cr.dot(axb) > 0);
		cr = b.cross(e);
		Assert(cr.dot(axb) < 0);

		float eaxb = e.dot(axb);
		float lell = axb.lenght() * e.lenght();
		Assert(eaxb < 0.1f * lell );
	}

	return ret;*/
}


INLINE boolean intersectFrustumAndSphereFast(const Vector4f* frPlane, const Vector3f& frRootVertex, const float frRadiys, const Vector3f& center, const float radiys)
{
	if((center - frRootVertex).lenghtSquare() > radiys*radiys + frRadiys*frRadiys + float(2.0)*radiys*frRadiys)return false;

	if(frPlane[0].dot(center.xyz1()) > radiys)return false;
	if(frPlane[1].dot(center.xyz1()) > radiys)return false;
	if(frPlane[2].dot(center.xyz1()) > radiys)return false;
	if(frPlane[3].dot(center.xyz1()) > radiys)return false;
	if(frPlane[4].dot(center.xyz1()) > radiys)return false;

	return true;
}



INLINE boolean intersectFrustumAndSphere(const Vector4f* frPlane, const Vector3f* frEdge, const Vector3f* frVertex, const float frRadiys, const Vector3f& center, const float radiys)
{
	if((center - frVertex[4]).lenghtSquare() > radiys*radiys + frRadiys*frRadiys + float(2.0)*radiys*frRadiys)return false;

	if(frPlane[0].dot(center.xyz1()) > radiys)return false;
	if(frPlane[1].dot(center.xyz1()) > radiys)return false;
	if(frPlane[2].dot(center.xyz1()) > radiys)return false;
	if(frPlane[3].dot(center.xyz1()) > radiys)return false;
	if(frPlane[4].dot(center.xyz1()) > radiys)return false;

	Vector3f d = center - frVertex[4];
	Vector3f e;
	e = d.subdot(frEdge[0]);
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(d) * rsqFast(e.lenghtSquare()) > radiys)return false;
	e = d.subdot(frEdge[1]);
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(d) * rsqFast(e.lenghtSquare()) > radiys)return false;
	e = d.subdot(frEdge[2]);
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(d) * rsqFast(e.lenghtSquare()) > radiys)return false;
	e = d.subdot(frEdge[3]);
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(d) * rsqFast(e.lenghtSquare()) > radiys)return false;

	float m,M;
	e = (center - frVertex[4]).normalFast();
	m = M = e.dot(frVertex[0]);
	Base::minMaxIncludePoint<float>(m, M, e.dot(frVertex[1]));
	Base::minMaxIncludePoint<float>(m, M, e.dot(frVertex[2]));
	Base::minMaxIncludePoint<float>(m, M, e.dot(frVertex[3]));
	Base::minMaxIncludePoint<float>(m, M, e.dot(frVertex[4]));
	if(!Base::intersectDiapasonAndDiapason<float>((M + m)*0.5f, (M - m)*0.5f, e.dot(center), radiys))return false;

	return true;
}


INLINE boolean intersectFast(const SFrustum& fr, const SRadial3f& sp)
{
	return intersectFrustumAndSphereFast(fr.plane, fr.vertex[4], fr.radiys, sp.c, sp.r);
}


INLINE boolean intersect(const SFrustum& fr, const SRadial3f& sp)
{
	return intersectFrustumAndSphere(fr.plane, fr.edge, fr.vertex, fr.radiys, sp.c, sp.r);
}


INLINE boolean intersectFrustumAndAABBFast(const Vector4f* frPlane, const Vector3f* frEdge, const Vector3f& frRootVert, const Vector3f& frAabbCenter, const Vector3f& frAabbHSize, const Vector3f& aabbCenter, const Vector3f& aabbHSize)
{
	if((aabbHSize + frAabbHSize - abs(frAabbCenter - aabbCenter)).isAnyNegative())return false;

	Vector4f aabbCenter1 = aabbCenter.xyz1();
	if(frPlane[0].dot(aabbCenter1) > (aabbHSize * frPlane[0].xyz()).componentAbsSumm())return false;
	if(frPlane[1].dot(aabbCenter1) > (aabbHSize * frPlane[1].xyz()).componentAbsSumm())return false;
	if(frPlane[2].dot(aabbCenter1) > (aabbHSize * frPlane[2].xyz()).componentAbsSumm())return false;
	if(frPlane[3].dot(aabbCenter1) > (aabbHSize * frPlane[3].xyz()).componentAbsSumm())return false;
	if(frPlane[4].dot(aabbCenter1) > (aabbHSize * frPlane[4].xyz()).componentAbsSumm())return false;

	return true;
}

INLINE boolean intersectFrustumAndAABB(const Vector4f* frPlane, const Vector3f* frEdge, const Vector3f& frRootVert, const Vector3f& frAabbCenter, const Vector3f& frAabbHSize, const Vector3f& aabbCenter, const Vector3f& aabbHSize)
{
	if((aabbHSize + frAabbHSize - abs(frAabbCenter - aabbCenter)).isAnyNegative())return false;

	Vector4f aabbCenter1 = aabbCenter.xyz1();
	if(frPlane[0].dot(aabbCenter1) > (aabbHSize * frPlane[0].xyz()).componentAbsSumm())return false;
	if(frPlane[1].dot(aabbCenter1) > (aabbHSize * frPlane[1].xyz()).componentAbsSumm())return false;
	if(frPlane[2].dot(aabbCenter1) > (aabbHSize * frPlane[2].xyz()).componentAbsSumm())return false;
	if(frPlane[3].dot(aabbCenter1) > (aabbHSize * frPlane[3].xyz()).componentAbsSumm())return false;
	if(frPlane[4].dot(aabbCenter1) > (aabbHSize * frPlane[4].xyz()).componentAbsSumm())return false;

	Vector3f d = aabbCenter - frRootVert;
	Vector3f e;
	e = frEdge[0].crossX();
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;
	e = frEdge[0].crossY();
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;
	e = frEdge[0].crossZ();
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;

	e = frEdge[1].crossX();
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;
	e = frEdge[1].crossY();
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;
	e = frEdge[1].crossZ();
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;

	e = frEdge[2].crossX();
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;
	e = frEdge[2].crossY();
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;
	e = frEdge[2].crossZ();
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;

	e = frEdge[3].crossX();
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;
	e = frEdge[3].crossY();
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;
	e = frEdge[3].crossZ();
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(d) > (e * aabbHSize).componentAbsSumm())return false;

	return true;
}

INLINE boolean intersectFast(const SFrustum& fr, const SAABB3f& aabb)
{
	return intersectFrustumAndAABBFast(fr.plane, fr.edge, fr.vertex[4], fr.aabb.c, fr.aabb.h,  aabb.c, aabb.h);
}

INLINE boolean intersect(const SFrustum& fr, const SAABB3f& aabb)
{
	return intersectFrustumAndAABB(fr.plane, fr.edge, fr.vertex[4], fr.aabb.c, fr.aabb.h,  aabb.c, aabb.h);
}

INLINE boolean intersectFrustumAndOBBFast(const Vector4f* frPlane, const Vector3f* frEdge, const Vector3f* frVertex, const SOBB3f& box)
{
	Vector4f boxP1 = box.p.xyz1();
	if(frPlane[0].dot(boxP1) > box.m33().tranMult(frPlane[0].xyz()).componentAbsSumm())return false;
	if(frPlane[1].dot(boxP1) > box.m33().tranMult(frPlane[1].xyz()).componentAbsSumm())return false;
	if(frPlane[2].dot(boxP1) > box.m33().tranMult(frPlane[2].xyz()).componentAbsSumm())return false;
	if(frPlane[3].dot(boxP1) > box.m33().tranMult(frPlane[3].xyz()).componentAbsSumm())return false;
	if(frPlane[4].dot(boxP1) > box.m33().tranMult(frPlane[4].xyz()).componentAbsSumm())return false;
	
	float m,M;
	m = M = box.x.dot(frVertex[0]);
	Base::minMaxIncludePoint<float>(m, M, box.x.dot(frVertex[1]));
	Base::minMaxIncludePoint<float>(m, M, box.x.dot(frVertex[2]));
	Base::minMaxIncludePoint<float>(m, M, box.x.dot(frVertex[3]));
	Base::minMaxIncludePoint<float>(m, M, box.x.dot(frVertex[4]));
	if(!Base::intersectDiapasonAndDiapason<float>((M + m)*float(0.5), (M - m)*float(0.5), box.x.dot(box.p),  box.x.lenghtSquare()))return false;
	m = M = box.y.dot(frVertex[0]);
	Base::minMaxIncludePoint<float>(m, M, box.y.dot(frVertex[1]));
	Base::minMaxIncludePoint<float>(m, M, box.y.dot(frVertex[2]));
	Base::minMaxIncludePoint<float>(m, M, box.y.dot(frVertex[3]));
	Base::minMaxIncludePoint<float>(m, M, box.y.dot(frVertex[4]));
	if(!Base::intersectDiapasonAndDiapason<float>((M + m)*float(0.5), (M - m)*float(0.5), box.y.dot(box.p),  box.y.lenghtSquare()))return false;
	m = M = box.z.dot(frVertex[0]);
	Base::minMaxIncludePoint<float>(m, M, box.z.dot(frVertex[1]));
	Base::minMaxIncludePoint<float>(m, M, box.z.dot(frVertex[2]));
	Base::minMaxIncludePoint<float>(m, M, box.z.dot(frVertex[3]));
	Base::minMaxIncludePoint<float>(m, M, box.z.dot(frVertex[4]));
	if(!Base::intersectDiapasonAndDiapason<float>((M + m)*float(0.5), (M - m)*float(0.5), box.z.dot(box.p),  box.z.lenghtSquare()))return false;

	return true;
}


INLINE boolean intersectFrustumAndOBB(const Vector4f* frPlane, const Vector3f* frEdge, const Vector3f* frVertex, const SOBB3f& box)
{
	Vector4f boxP1 = box.p.xyz1();
	if(frPlane[0].dot(boxP1) > box.m33().tranMult(frPlane[0].xyz()).componentAbsSumm())return false;
	if(frPlane[1].dot(boxP1) > box.m33().tranMult(frPlane[1].xyz()).componentAbsSumm())return false;
	if(frPlane[2].dot(boxP1) > box.m33().tranMult(frPlane[2].xyz()).componentAbsSumm())return false;
	if(frPlane[3].dot(boxP1) > box.m33().tranMult(frPlane[3].xyz()).componentAbsSumm())return false;
	if(frPlane[4].dot(boxP1) > box.m33().tranMult(frPlane[4].xyz()).componentAbsSumm())return false;

	float m,M;
	m = M = box.x.dot(frVertex[0]);
	Base::minMaxIncludePoint<float>(m, M, box.x.dot(frVertex[1]));
	Base::minMaxIncludePoint<float>(m, M, box.x.dot(frVertex[2]));
	Base::minMaxIncludePoint<float>(m, M, box.x.dot(frVertex[3]));
	Base::minMaxIncludePoint<float>(m, M, box.x.dot(frVertex[4]));
	if(!Base::intersectDiapasonAndDiapason<float>((M + m)*float(0.5), (M - m)*float(0.5), box.x.dot(box.p),  box.x.lenghtSquare()))return false;
	m = M = box.y.dot(frVertex[0]);
	Base::minMaxIncludePoint<float>(m, M, box.y.dot(frVertex[1]));
	Base::minMaxIncludePoint<float>(m, M, box.y.dot(frVertex[2]));
	Base::minMaxIncludePoint<float>(m, M, box.y.dot(frVertex[3]));
	Base::minMaxIncludePoint<float>(m, M, box.y.dot(frVertex[4]));
	if(!Base::intersectDiapasonAndDiapason<float>((M + m)*float(0.5), (M - m)*float(0.5), box.y.dot(box.p),  box.y.lenghtSquare()))return false;
	m = M = box.z.dot(frVertex[0]);
	Base::minMaxIncludePoint<float>(m, M, box.z.dot(frVertex[1]));
	Base::minMaxIncludePoint<float>(m, M, box.z.dot(frVertex[2]));
	Base::minMaxIncludePoint<float>(m, M, box.z.dot(frVertex[3]));
	Base::minMaxIncludePoint<float>(m, M, box.z.dot(frVertex[4]));
	if(!Base::intersectDiapasonAndDiapason<float>((M + m)*float(0.5), (M - m)*float(0.5), box.z.dot(box.p),  box.z.lenghtSquare()))return false;

	Vector3f d = box.p - frVertex[4];
	Vector3f e;
	e = frEdge[0].cross(box.x);
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;
	e = frEdge[0].cross(box.y);
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;
	e = frEdge[0].cross(box.z);
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;

	e = frEdge[1].cross(box.x);
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;
	e = frEdge[1].cross(box.y);
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;
	e = frEdge[1].cross(box.z);
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;

	e = frEdge[2].cross(box.x);
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;
	e = frEdge[2].cross(box.y);
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;
	e = frEdge[2].cross(box.z);
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;

	e = frEdge[3].cross(box.x);
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;
	e = frEdge[3].cross(box.y);
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;
	e = frEdge[3].cross(box.z);
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(d) > box.m33().tranMult(e).componentAbsSumm())return false;

	return true;
}


INLINE boolean intersectFast(const SFrustum& fr, const SOBB3f& obb)
{
	return intersectFrustumAndOBBFast(fr.plane, fr.edge, fr.vertex, obb);
}


INLINE boolean intersect(const SFrustum& fr, const SOBB3f& obb)
{
	return intersectFrustumAndOBB(fr.plane, fr.edge, fr.vertex, obb);
}




INLINE boolean intersectFrustumAndFrustumFast(const Vector4f* frPlane, const Vector3f* frVertex, const Vector4f* fr2Plane, const Vector3f* fr2Vertex)
{
	if(frPlane[0].dot(fr2Vertex[0].xyz1()) > 0 && frPlane[0].dot(fr2Vertex[1].xyz1()) > 0 && frPlane[0].dot(fr2Vertex[2].xyz1()) > 0 && frPlane[0].dot(fr2Vertex[3].xyz1()) > 0 && frPlane[0].dot(fr2Vertex[4].xyz1()) > 0)return false;
	if(frPlane[1].dot(fr2Vertex[0].xyz1()) > 0 && frPlane[1].dot(fr2Vertex[1].xyz1()) > 0 && frPlane[1].dot(fr2Vertex[2].xyz1()) > 0 && frPlane[1].dot(fr2Vertex[3].xyz1()) > 0 && frPlane[1].dot(fr2Vertex[4].xyz1()) > 0)return false;
	if(frPlane[2].dot(fr2Vertex[0].xyz1()) > 0 && frPlane[2].dot(fr2Vertex[1].xyz1()) > 0 && frPlane[2].dot(fr2Vertex[2].xyz1()) > 0 && frPlane[2].dot(fr2Vertex[3].xyz1()) > 0 && frPlane[2].dot(fr2Vertex[4].xyz1()) > 0)return false;
	if(frPlane[3].dot(fr2Vertex[0].xyz1()) > 0 && frPlane[3].dot(fr2Vertex[1].xyz1()) > 0 && frPlane[3].dot(fr2Vertex[2].xyz1()) > 0 && frPlane[3].dot(fr2Vertex[3].xyz1()) > 0 && frPlane[3].dot(fr2Vertex[4].xyz1()) > 0)return false;

	if(fr2Plane[0].dot(frVertex[0].xyz1()) > 0 && fr2Plane[0].dot(frVertex[1].xyz1()) > 0 && fr2Plane[0].dot(frVertex[2].xyz1()) > 0 && fr2Plane[0].dot(frVertex[3].xyz1()) > 0 && fr2Plane[0].dot(frVertex[4].xyz1()) > 0)return false;
	if(fr2Plane[1].dot(frVertex[0].xyz1()) > 0 && fr2Plane[1].dot(frVertex[1].xyz1()) > 0 && fr2Plane[1].dot(frVertex[2].xyz1()) > 0 && fr2Plane[1].dot(frVertex[3].xyz1()) > 0 && fr2Plane[1].dot(frVertex[4].xyz1()) > 0)return false;
	if(fr2Plane[2].dot(frVertex[0].xyz1()) > 0 && fr2Plane[2].dot(frVertex[1].xyz1()) > 0 && fr2Plane[2].dot(frVertex[2].xyz1()) > 0 && fr2Plane[2].dot(frVertex[3].xyz1()) > 0 && fr2Plane[2].dot(frVertex[4].xyz1()) > 0)return false;
	if(fr2Plane[3].dot(frVertex[0].xyz1()) > 0 && fr2Plane[3].dot(frVertex[1].xyz1()) > 0 && fr2Plane[3].dot(frVertex[2].xyz1()) > 0 && fr2Plane[3].dot(frVertex[3].xyz1()) > 0 && fr2Plane[3].dot(frVertex[4].xyz1()) > 0)return false;

	return true;
}


INLINE boolean intersectFrustumAndFrustum(const Vector4f* frPlane, const Vector3f* frEdge, const Vector3f* frVertex, const Vector4f* fr2Plane, const Vector3f* fr2Edge, const Vector3f* fr2Vertex)
{
	if(frPlane[0].dot(fr2Vertex[0].xyz1()) > 0 && frPlane[0].dot(fr2Vertex[1].xyz1()) > 0 && frPlane[0].dot(fr2Vertex[2].xyz1()) > 0 && frPlane[0].dot(fr2Vertex[3].xyz1()) > 0 && frPlane[0].dot(fr2Vertex[4].xyz1()) > 0)return false;
	if(frPlane[1].dot(fr2Vertex[0].xyz1()) > 0 && frPlane[1].dot(fr2Vertex[1].xyz1()) > 0 && frPlane[1].dot(fr2Vertex[2].xyz1()) > 0 && frPlane[1].dot(fr2Vertex[3].xyz1()) > 0 && frPlane[1].dot(fr2Vertex[4].xyz1()) > 0)return false;
	if(frPlane[2].dot(fr2Vertex[0].xyz1()) > 0 && frPlane[2].dot(fr2Vertex[1].xyz1()) > 0 && frPlane[2].dot(fr2Vertex[2].xyz1()) > 0 && frPlane[2].dot(fr2Vertex[3].xyz1()) > 0 && frPlane[2].dot(fr2Vertex[4].xyz1()) > 0)return false;
	if(frPlane[3].dot(fr2Vertex[0].xyz1()) > 0 && frPlane[3].dot(fr2Vertex[1].xyz1()) > 0 && frPlane[3].dot(fr2Vertex[2].xyz1()) > 0 && frPlane[3].dot(fr2Vertex[3].xyz1()) > 0 && frPlane[3].dot(fr2Vertex[4].xyz1()) > 0)return false;

	if(fr2Plane[0].dot(frVertex[0].xyz1()) > 0 && fr2Plane[0].dot(frVertex[1].xyz1()) > 0 && fr2Plane[0].dot(frVertex[2].xyz1()) > 0 && fr2Plane[0].dot(frVertex[3].xyz1()) > 0 && fr2Plane[0].dot(frVertex[4].xyz1()) > 0)return false;
	if(fr2Plane[1].dot(frVertex[0].xyz1()) > 0 && fr2Plane[1].dot(frVertex[1].xyz1()) > 0 && fr2Plane[1].dot(frVertex[2].xyz1()) > 0 && fr2Plane[1].dot(frVertex[3].xyz1()) > 0 && fr2Plane[1].dot(frVertex[4].xyz1()) > 0)return false;
	if(fr2Plane[2].dot(frVertex[0].xyz1()) > 0 && fr2Plane[2].dot(frVertex[1].xyz1()) > 0 && fr2Plane[2].dot(frVertex[2].xyz1()) > 0 && fr2Plane[2].dot(frVertex[3].xyz1()) > 0 && fr2Plane[2].dot(frVertex[4].xyz1()) > 0)return false;
	if(fr2Plane[3].dot(frVertex[0].xyz1()) > 0 && fr2Plane[3].dot(frVertex[1].xyz1()) > 0 && fr2Plane[3].dot(frVertex[2].xyz1()) > 0 && fr2Plane[3].dot(frVertex[3].xyz1()) > 0 && fr2Plane[3].dot(frVertex[4].xyz1()) > 0)return false;

	Vector3f e;
	e, frEdge[0].cross(fr2Edge[0]);
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[0].cross(fr2Edge[1]);
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[0].cross(fr2Edge[2]);
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[0].cross(fr2Edge[3]);
	if(betveenChange(e, frPlane[3].xyz(), frPlane[0].xyz(), frEdge[0]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;

	e = frEdge[1].cross(fr2Edge[0]);
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[1].cross(fr2Edge[1]);
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[1].cross(fr2Edge[2]);
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[1].cross(fr2Edge[3]);
	if(betveenChange(e, frPlane[0].xyz(), frPlane[1].xyz(), frEdge[1]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;

	e = frEdge[2].cross(fr2Edge[0]);
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[2].cross(fr2Edge[1]);
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[2].cross(fr2Edge[2]);
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[2].cross(fr2Edge[3]);
	if(betveenChange(e, frPlane[1].xyz(), frPlane[2].xyz(), frEdge[2]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;

	e = frEdge[3].cross(fr2Edge[0]);
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[3].cross(fr2Edge[1]);
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[3].cross(fr2Edge[2]);
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;
	e = frEdge[3].cross(fr2Edge[3]);
	if(betveenChange(e, frPlane[2].xyz(), frPlane[3].xyz(), frEdge[3]))
		if(e.dot(fr2Vertex[0] - frVertex[4]) > 0 && e.dot(fr2Vertex[1] - frVertex[4]) > 0 && e.dot(fr2Vertex[2] - frVertex[4]) > 0 && e.dot(fr2Vertex[3] - frVertex[4]) > 0 && e.dot(fr2Vertex[4] - frVertex[4]) > 0)return false;

	return true;
}


INLINE boolean intersectFast(const SFrustum& fr, const SFrustum& fr2)
{
	return intersectFrustumAndFrustumFast(fr.plane, fr.vertex, fr2.plane, fr2.vertex);
}


INLINE boolean intersect(const SFrustum& fr, const SFrustum& fr2)
{
	return intersectFrustumAndFrustum(fr.plane, fr.edge, fr.vertex, fr2.plane, fr2.edge, fr2.vertex);
}





}
