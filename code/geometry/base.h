#pragma once

#include "../base/vector.h"
#include "../base/matrix.h"

namespace Geometry {

USING_BASE_LIB

struct SMinMax2f
{
	Vector2f min, max;
	SMinMax2f(){}
	SMinMax2f(const Vector2f& inMin, const Vector2f& inMax) : min(inMin), max(inMax) {}
};

struct SMinMax2i
{
	Vector2i min, max;
	SMinMax2i(){}
	SMinMax2i(const Vector2i& inMin, const Vector2i& inMax) : min(inMin), max(inMax) {}
};

template<class t_Scalar>
struct SMinMax2T
{
	typedef void Type;
};

template<>
struct SMinMax2T<float>
{
	typedef SMinMax2f Type;
};

template<>
struct SMinMax2T<int>
{
	typedef SMinMax2i Type;
};

struct SMinMax3f
{
	Vector3f min, max;
	SMinMax3f(){}
	SMinMax3f(const Vector3f& inMin, const Vector3f& inMax) : min(inMin), max(inMax) {}
};

template<class t_Scalar>
struct SMinMax3T
{
	typedef void Type;
};

template<>
struct SMinMax3T<float>
{
	typedef SMinMax3f Type;
};

struct SMinMax4f
{
	Vector4f min, max;
	SMinMax4f(){}
	SMinMax4f(const Vector4f& inMin, const Vector4f& inMax) : min(inMin), max(inMax) {}
};

template<class t_Scalar>
struct SMinMax4T
{
	typedef void Type;
};

template<>
struct SMinMax4T<float>
{
	typedef SMinMax4f Type;
};

struct SLineSegment3f
{
	Vector3f a, b;
	SLineSegment3f(){}
	SLineSegment3f(const Vector3f& inA, const Vector3f& inB) : a(inA), b(inB) {}
};

template<class t_Scalar>
struct SLineSegmen3T
{
	typedef void Type;
};

template<>
struct SLineSegmen3T<float>
{
	typedef SLineSegment3f Type;
};

struct SLineSegment2f
{
	Vector2f a, b;
	SLineSegment2f(){}
	SLineSegment2f(const Vector2f& inA, const Vector2f& inB) : a(inA), b(inB) {}
};

template<class t_Scalar>
struct SLineSegment2T
{
	typedef void Type;
};

template<>
struct SLineSegment2T<float>
{
	typedef SLineSegment2f Type;
};

struct SRay3f
{
	Vector3f p, d;
	SRay3f(){}
	SRay3f(const Vector3f& inP, const Vector3f& inD) : p(inP), d(inD) {}
};

template<class t_Scalar>
struct SRay3T
{
	typedef void Type;
};

template<>
struct SRay3T<float>
{
	typedef SRay3f Type;
};

struct SRay2f
{
	Vector2f p, d;
	SRay2f(){}
	SRay2f(const Vector2f& inP, const Vector2f& inD) : p(inP), d(inD) {}
};

template<class t_Scalar>
struct SRay2T
{
	typedef void Type;
};

template<>
struct SRay2T<float>
{
	typedef SRay2f Type;
};

struct SLine3f
{
	Vector3f p, d;
	SLine3f(){}
	SLine3f(const Vector3f& inP, const Vector3f& inD) : p(inP), d(inD) {}
};

template<class t_Scalar>
struct SLine3T
{
	typedef void Type;
};

template<>
struct SLine3T<float>
{
	typedef SLine3f Type;
};


struct SLine2f
{
	Vector2f p, d;
	SLine2f(){}
	SLine2f(const Vector2f& inP, const Vector2f& inD) : p(inP), d(inD) {}
};

template<class t_Scalar>
struct SLine2T
{
	typedef void Type;
};

template<>
struct SLine2T<float>
{
	typedef SLine2f Type;
};


struct SRadial3f
{
	Vector3f c;
	float r;
	SRadial3f(){}
	SRadial3f(const Vector3f& inC, const float inR) : c(inC), r(inR) {}
};

template<class t_Scalar>
struct SRadial3T
{
	typedef void Type;
};

template<>
struct SRadial3T<float>
{
	typedef SRadial3f Type;
};


struct SRadial2f
{
	Vector2f c;
	float r;
	SRadial2f(){}
	SRadial2f(const Vector2f& inC, const float inR) : c(inC), r(inR) {}
};

template<class t_Scalar>
struct SRadial2T
{
	typedef void Type;
};

template<>
struct SRadial2T<float>
{
	typedef SRadial2f Type;
};

struct SAABB3f
{
	Vector3f c, h;
	SAABB3f(){}
	SAABB3f(const Vector3f& inC, const Vector3f& inH) : c(inC), h(inH) {}
};

template<class t_Scalar>
struct SAABB3T
{
	typedef void Type;
};

template<>
struct SAABB3T<float>
{
	typedef SAABB3f Type;
};

struct SAABB2f
{
	Vector2f c, h;
	SAABB2f(){}
	SAABB2f(const Vector2f& inC, const Vector2f& inH) : c(inC), h(inH) {}
};

template<class t_Scalar>
struct SAABB2T
{
	typedef void Type;
};

template<>
struct SAABB2T<float>
{
	typedef SAABB2f Type;
};


typedef Matrix34f SOBB3f;

template<class t_Scalar>
struct SOBB3T
{
	typedef void Type;
};

template<>
struct SOBB3T<float>
{
	typedef SOBB3f Type;
};


typedef Matrix23f SOBB2f;

template<class t_Scalar>
struct SOBB2T
{
	typedef void Type;
};

template<>
struct SOBB2T<float>
{
	typedef SOBB2f Type;
};


struct STriangle3f
{
	Vector3f a, b, c;
	STriangle3f(){}
	STriangle3f(const Vector3f& inA, const Vector3f& inB, const Vector3f& inC) : a(inA), b(inB), c(inC) {}
};

template<class t_Scalar>
struct STriangle3T
{
	typedef void Type;
};

template<>
struct STriangle3T<float>
{
	typedef STriangle3f Type;
};


struct STriangle2f
{
	Vector2f a, b, c;
	STriangle2f(){}
	STriangle2f(const Vector2f& inA, const Vector2f& inB, const Vector2f& inC) : a(inA), b(inB), c(inC) {}
};

template<class t_Scalar>
struct STriangle2T
{
	typedef void Type;
};

template<>
struct STriangle2T<float>
{
	typedef STriangle2f Type;
};

struct SFrustum
{
	Vector4f plane[5];
	Vector3f edge[6];
	Vector3f vertex[5];
	SAABB3f aabb;
	float radiys;
};

static INLINE void convert(SAABB3f& dst, const SMinMax3f& src)
{
	dst.c = src.max + src.min;
	dst.c *= float(0.5);
	dst.h = src.max - src.min;
	dst.h *= float(0.5);
}

static INLINE void convert(SAABB2f& dst, const SMinMax2f& src)
{
	dst.c = src.max + src.min;
	dst.c *= float(0.5);
	dst.h = src.max - src.min;
	dst.h *= float(0.5);
}

static INLINE void convert(SMinMax3f& dst, const SAABB3f& src)
{
	dst.min = src.c - src.h;
	dst.max = src.c + src.h;
}

static INLINE void convert(SMinMax2f& dst, const SAABB2f& src)
{
	dst.min = src.c - src.h;
	dst.max = src.c + src.h;
}

}
