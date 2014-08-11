#pragma once

#include "scalar.h"
#include "simd.h"

namespace Base
{

template<typename t_Scalar> struct TVector2;
template<typename t_Scalar> struct TVector3;
template<typename t_Scalar> struct TVector4;

struct MakeFromScalarFlag {};

template<class t_Scalar>
struct TPackedVector4
{
	union
	{
		struct { t_Scalar x,y,z,w; };
		t_Scalar a[4];
	};

	INLINE_NO_DEBUG TPackedVector4() : x(t_Scalar(0)), y(t_Scalar(0)), z(t_Scalar(0)), w(t_Scalar(0))																	{}
	INLINE_NO_DEBUG TPackedVector4(const t_Scalar srcX, const t_Scalar srcY, const t_Scalar srcZ, const t_Scalar srcW):  x(srcX), y(srcY), z(srcZ), w(srcW) {}
	INLINE_NO_DEBUG TPackedVector4(const t_Scalar* arr): x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])	{}
	template<class t_OtherVector>
	INLINE_NO_DEBUG void set(const t_OtherVector& v) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w; }
	INLINE_NO_DEBUG void set(const t_Scalar srcX, const t_Scalar srcY, const t_Scalar srcZ, const t_Scalar srcW) { x = srcX; y = srcY; z = srcZ; w = srcW; }
	INLINE_NO_DEBUG TPackedVector4(const TVector4<t_Scalar>& v);
	INLINE_NO_DEBUG TPackedVector4& operator = (const TVector4<t_Scalar>& v);
};

template<class t_Scalar>
struct TPackedVector3
{
	union
	{
		struct { t_Scalar x,y,z; };
		t_Scalar a[3];
	};
	INLINE_NO_DEBUG TPackedVector3() : x(t_Scalar(0)), y(t_Scalar(0)), z(t_Scalar(0))																	{}
	INLINE_NO_DEBUG TPackedVector3(const t_Scalar srcX, const t_Scalar srcY, const t_Scalar srcZ):  x(srcX), y(srcY), z(srcZ) {}
	INLINE_NO_DEBUG TPackedVector3(const t_Scalar* arr): x(arr[0]), y(arr[1]), z(arr[2])	{}
	template<class t_OtherVector>
	INLINE_NO_DEBUG void set(const t_OtherVector& v) { this->x = v.x; this->y = v.y; this->z = v.z; }
	INLINE_NO_DEBUG void set(const t_Scalar srcX, const t_Scalar srcY, const t_Scalar srcZ) { x = srcX; y = srcY; z = srcZ; }
	INLINE_NO_DEBUG TPackedVector3(const TVector3<t_Scalar>& v);
	INLINE_NO_DEBUG TPackedVector3& operator = (const TVector3<t_Scalar>& v);
};

template<class t_Scalar>
struct TPackedVector2
{
	union
	{
		struct { t_Scalar x,y; };
		t_Scalar a[2];
	};
	INLINE_NO_DEBUG TPackedVector2() : x(t_Scalar(0)), y(t_Scalar(0))							{}
	INLINE_NO_DEBUG TPackedVector2(const t_Scalar srcX, const t_Scalar srcY):  x(srcX), y(srcY) {}
	INLINE_NO_DEBUG TPackedVector2(const t_Scalar* arr): x(arr[0]), y(arr[1])	{}
	template<class t_OtherVector>
	INLINE_NO_DEBUG void set(const t_OtherVector& v) { this->x = v.x; this->y = v.y; }
	INLINE_NO_DEBUG void set(const t_Scalar srcX, const t_Scalar srcY) { this->x = srcX; this->y = srcY; }
	INLINE_NO_DEBUG TPackedVector2(const TVector2<t_Scalar>& v);
	INLINE_NO_DEBUG TPackedVector2& operator = (const TVector2<t_Scalar>& v);
};


template<typename t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TVector2<t_Scalar>& v);


template<typename t_Scalar>
struct TVector2
{
	union
	{
		struct { t_Scalar x, y; };
		t_Scalar a[2];
	};

	typedef t_Scalar Scalar_t;
	typedef t_Scalar ScalarOptimal_t;

#ifdef DEBUG
	INLINE_NO_DEBUG void verify()const { Assert(isFinite(*this)); }
#else
	INLINE_NO_DEBUG void verify()const {}
#endif

	INLINE_NO_DEBUG TVector2(Scalar_t f, MakeFromScalarFlag flag) : x(f), y(f)		{verify();}
	INLINE_NO_DEBUG TVector2(Scalar_t srcX, Scalar_t srcY) :  x(srcX), y(srcY)		{verify();}
	INLINE_NO_DEBUG TVector2(const Scalar_t* arr) : x(arr[0]), y(arr[1])			{verify();}
	INLINE_NO_DEBUG TVector2(const TVector2& u) : x(u.x), y(u.y)					{verify();}
	INLINE_NO_DEBUG TVector2(const TPackedVector2<Scalar_t>& u): x(u.x), y(u.y)		{verify();}

	INLINE_NO_DEBUG void set(Scalar_t f) 											{ x = f; y = f; verify(); }
	INLINE_NO_DEBUG void set(Scalar_t srcX, Scalar_t srcY) 							{ x = srcX; y = srcY; verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr) 									{ x = arr[0]; y = arr[1]; verify(); };
	INLINE_NO_DEBUG void set(const TPackedVector2<Scalar_t>& u) 					{ x = u.x; y = u.y; verify(); };
	INLINE_NO_DEBUG void set(const TVector2& u) 									{ x = u.x; y = u.y; verify(); };
	INLINE_NO_DEBUG void setOne()													{ x = y = Scalar_t(1); }
	INLINE_NO_DEBUG void setZero() 													{ x = y = Scalar_t(0); }
	INLINE_NO_DEBUG static TVector2 getZero() 										{ return TVector2(Scalar_t(0), Scalar_t(0)); }
	INLINE_NO_DEBUG static TVector2 getOne() 										{ return TVector2(Scalar_t(1), Scalar_t(1)); }
	INLINE_NO_DEBUG TVector2& operator = (const TVector2& u)						{ x = u.x; y = u.y; verify(); return *this; }
	INLINE_NO_DEBUG TVector2& operator = (const TPackedVector2<Scalar_t>& u) 		{ x = u.x; y = u.y; verify(); return *this; }
	INLINE_NO_DEBUG boolean operator == (const TVector2& u)const					{ return x == u.x && y == u.y; }
	INLINE_NO_DEBUG boolean operator != (const TVector2& u)const					{ return x != u.x || y != u.y; }
	
	INLINE_NO_DEBUG TVector2 operator - () const									{ return TVector2(-x, -y); }
	INLINE_NO_DEBUG void operator += (const TVector2& u)							{ x += u.x; y += u.y; }
	INLINE_NO_DEBUG void operator -= (const TVector2& u)							{ x -= u.x; y -= u.y; }
	INLINE_NO_DEBUG void operator += (const Scalar_t f)								{ x += f; y += f; }
	INLINE_NO_DEBUG void operator -= (const Scalar_t f)								{ x -= f; y -= f; }
	INLINE_NO_DEBUG void operator *= (const TVector2& u)							{ x *= u.x; y *= u.y; }
	INLINE_NO_DEBUG void operator /= (const TVector2& u)							{ x /= u.x; y /= u.y; }
	INLINE_NO_DEBUG void operator *= (const Scalar_t f)								{ x *= f; y *= f; }
	INLINE_NO_DEBUG void operator /= (const Scalar_t f)								{ x /= f; y /= f; }

	friend INLINE_NO_DEBUG TVector2 operator + (const TVector2& a, const TVector2& b){ return TVector2(a.x + b.x, a.y + b.y); }
	friend INLINE_NO_DEBUG TVector2 operator - (const TVector2& a, const TVector2& b){ return TVector2(a.x - b.x, a.y - b.y); }
	friend INLINE_NO_DEBUG TVector2 operator * (const TVector2& a, const TVector2& b){ return TVector2(a.x * b.x, a.y * b.y); }
	friend INLINE_NO_DEBUG TVector2 operator / (const TVector2& a, const TVector2& b){ return TVector2(a.x / b.x, a.y / b.y); }
	friend INLINE_NO_DEBUG TVector2 operator + (const Scalar_t f, const TVector2& v) { return TVector2(f + v.x, f + v.y); }
	friend INLINE_NO_DEBUG TVector2 operator - (const Scalar_t f, const TVector2& v) { return TVector2(f - v.x, f - v.y); }
	friend INLINE_NO_DEBUG TVector2 operator * (const Scalar_t f, const TVector2& v) { return TVector2(f * v.x, f * v.y); }
	friend INLINE_NO_DEBUG TVector2 operator / (const Scalar_t f, const TVector2& v) { return TVector2(f / v.x, f / v.y); }
	friend INLINE_NO_DEBUG TVector2 operator + (const TVector2& v, const Scalar_t f) { return TVector2(v.x + f, v.y + f); }
	friend INLINE_NO_DEBUG TVector2 operator - (const TVector2& v, const Scalar_t f) { return TVector2(v.x - f, v.y - f); }
	friend INLINE_NO_DEBUG TVector2 operator * (const TVector2& v, const Scalar_t f) { return TVector2(v.x * f, v.y * f); }
	friend INLINE_NO_DEBUG TVector2 operator / (const TVector2& v, const Scalar_t f) { return TVector2(v.x / f, v.y / f); }
	
	INLINE_NO_DEBUG Scalar_t dot(const TVector2 &u)const 							{ return x * u.x + y * u.y; }
	INLINE_NO_DEBUG Scalar_t lenghtSquare()const									{ return x * x + y * y; }
	INLINE_NO_DEBUG Scalar_t lenght()const											{ return Base::sqrt(lenghtSquare()); }
	INLINE_NO_DEBUG Scalar_t distanceSquareTo(const TVector2& u)const				{ return (u - *this).lenghtSquare(); }
	INLINE_NO_DEBUG Scalar_t distanceTo(const TVector2& u)const						{ return (u - *this).lenght(); }
	INLINE_NO_DEBUG Scalar_t componentSumm()const									{ return x + y; }
	INLINE_NO_DEBUG Scalar_t componentAbsSumm()const								{ return Base::abs(x) + Base::abs(y); }
	INLINE_NO_DEBUG TVector2 cross()const											{ return TVector2(y, -x); }
	INLINE_NO_DEBUG TVector2 normal()const											{ return *this * rsq(lenghtSquare()); }
	INLINE_NO_DEBUG TVector2 normalFast()const										{ return *this * rsqFast(lenghtSquare()); }

	INLINE_NO_DEBUG boolean01 isAllPositive()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (~(nx | ny))>>31;
	}

	INLINE_NO_DEBUG boolean01 isAllNegative()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (nx & ny)>>31;
	}

	INLINE_NO_DEBUG boolean01 isAnyPositive()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (~(nx & ny))>>31;
	}

	INLINE_NO_DEBUG boolean01 isAnyNegative()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (nx | ny)>>31;
	}

	INLINE_NO_DEBUG TVector2()														{ setZero(); }
	INLINE_NO_DEBUG TVector2 operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG void selfClamp(const TVector2& low, const TVector2& hi);
	INLINE_NO_DEBUG void selfClamp01();
	INLINE_NO_DEBUG void selfClamp11();
	
	INLINE_NO_DEBUG boolean trySelfNormalize(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsq(lsq);
			return true;
		}
		return false;
	}

	INLINE_NO_DEBUG boolean trySelfNormalizeFast(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsqFast(lsq);
			return true;
		}
		return false;
	}
};


template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> abs(const TVector2<t_Scalar>& vec)
{
	return TVector2<t_Scalar>(abs(vec.x), abs(vec.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> sign(const TVector2<t_Scalar>& vec)
{
	return TVector2<t_Scalar>(sign(vec.x), sign(vec.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> min(const TVector2<t_Scalar>& a, const TVector2<t_Scalar>& b)
{
	return TVector2<t_Scalar>(min(a.x, b.x), min(a.y, b.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> max(const TVector2<t_Scalar>& a, const TVector2<t_Scalar>& b)
{
	return TVector2<t_Scalar>(max(a.x, b.x), max(a.y, b.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG void minimize(TVector2<t_Scalar>& a, const TVector2<t_Scalar>& b)
{
	minimize(a.x, b.x);
	minimize(a.y, b.y);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG void maximize(TVector2<t_Scalar>& a, const TVector2<t_Scalar>& b)
{
	maximize(a.x, b.x);
	maximize(a.y, b.y);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> clamp(const TVector2<t_Scalar>& vec, const TVector2<t_Scalar>& low, const TVector2<t_Scalar>& hi)
{
	return TVector2<t_Scalar>(clamp(vec.x, low.x, hi.x), clamp(vec.y, low.y, hi.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> clamp01(const TVector2<t_Scalar>& vec)
{
	return TVector2<t_Scalar>(clamp01(vec.x), clamp01(vec.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> clamp11(const TVector2<t_Scalar>& vec)
{
	return TVector2<t_Scalar>(clamp11(vec.x), clamp11(vec.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> rcp(const TVector2<t_Scalar>& vec)
{
	return TVector2<t_Scalar>(rcp(vec.x), rcp(vec.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> rsq(const TVector2<t_Scalar>& vec)
{
	return TVector2<t_Scalar>(rsq(vec.x), rsq(vec.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> sqrt(const TVector2<t_Scalar>& vec)
{
	return TVector2<t_Scalar>(sqrt(vec.x), sqrt(vec.y));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TVector2<t_Scalar>& a, const TVector2<t_Scalar>& b, t_Scalar eps)
{
	return Base::equals(a.a[0], b.a[0], eps) && Base::equals(a.a[1], b.a[1], eps);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TVector2<t_Scalar>& vec)
{
	return isFinite(vec.a[0]) && isFinite(vec.a[1]);
}

template<typename t_Scalar>
INLINE_NO_DEBUG void TVector2<t_Scalar>::selfClamp(const TVector2<t_Scalar>& low, const TVector2<t_Scalar>& hi)	{ *this = clamp(*this, low, hi); }
template<typename t_Scalar>
INLINE_NO_DEBUG void TVector2<t_Scalar>::selfClamp01()											{ *this = clamp01(*this); }
template<typename t_Scalar>
INLINE_NO_DEBUG void TVector2<t_Scalar>::selfClamp11()											{ *this = clamp11(*this); }

template<class t_Scalar>
class Vector2fRef : public TVector2<t_Scalar>
{
public:
	INLINE_NO_DEBUG void operator = (const TVector2<t_Scalar>& other)
	{
		this->x = other.x; this->y = other.y;
	}
private:
	Vector2fRef(){}
	Vector2fRef(const Vector2fRef&){}
};



#ifdef INSTRUCTIONS_SSE
template<>
struct TVector2<float> : public FloatVector
{
	typedef FloatVector Base_t;
	typedef float Scalar_t;
	typedef Float ScalarOptimal_t;

#ifdef DEBUG
	INLINE_NO_DEBUG void verify()const { Assert(isFinite(*this)); Assert(this->a[2] == 0.0f && this->a[3] == 0.0f); }
#else
	INLINE_NO_DEBUG void verify()const {}
#endif

	template<class t_OtherScalar>
	INLINE_NO_DEBUG TVector2(const t_OtherScalar& f, MakeFromScalarFlag flag)			{ Base_t::setXY00(f); verify(); }
	template<typename t_ValX, typename t_ValY>
	INLINE_NO_DEBUG TVector2(const t_ValX& srcX, const t_ValY& srcY)					{ Base_t::setXY00(srcX, srcY); verify(); }
	INLINE_NO_DEBUG TVector2(const Scalar_t* arr)										{ Base_t::setXY00(arr); verify(); }
	INLINE_NO_DEBUG TVector2(const TVector2& v) : Base_t(*(const Base_t*)&v)			{ verify(); }
	INLINE_NO_DEBUG TVector2(const TPackedVector2<Scalar_t>& u)							{ Base_t::setXY00(u.a); verify(); }

	template<class t_OtherScalar>
	INLINE_NO_DEBUG void set(const t_OtherScalar& f) 									{ Base_t::setXY00(f); verify(); }
	template<typename t_ValX, typename t_ValY>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, const t_ValY& srcY) 					{ Base_t::setXY00(srcX, srcY); verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr) 										{ Base_t::setXY00(arr); verify(); };
	INLINE_NO_DEBUG void set(const TPackedVector2<Scalar_t>& u) 						{ Base_t::setXY00(u.a); verify(); };
	INLINE_NO_DEBUG void set(const TVector2& u) 										{ this->v = u.v; verify(); };

	INLINE_NO_DEBUG void setZero() 														{ SIMD4::setZero(); verify(); }
	INLINE_NO_DEBUG void setOne()														{ SIMD4::setXYZW1100(); verify(); }
	INLINE_NO_DEBUG static TVector2 getZero() 											{ return *(TVector2*)&SIMD4::getZero(); }
	INLINE_NO_DEBUG static TVector2 getOne() 											{ return *(TVector2*)&SIMD4::getXYZW1100(); }
	INLINE_NO_DEBUG TVector2& operator = (const TVector2& u)							{ this->v = u.v; verify(); return *this; };
	INLINE_NO_DEBUG TVector2& operator = (const TPackedVector2<Scalar_t>& u) 			{ Base_t::setXY00(u.a); verify(); return *this; };
	INLINE_NO_DEBUG boolean operator == (const TVector2& u)const						{ return !simd_noeqcmp2(*this, u); }
	INLINE_NO_DEBUG boolean operator != (const TVector2& u)const						{ return simd_noeqcmp2(*this, u); }

	INLINE_NO_DEBUG TVector2 operator - () const										{ return *(TVector2*)&simd_neg2(*this); }
	INLINE_NO_DEBUG void operator += (const TVector2& u)								{ this->v = simd_add(*this, u).v; }
	INLINE_NO_DEBUG void operator -= (const TVector2& u)								{ this->v = simd_sub(*this, u).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator += (const t_OtherScalar& f)							{ this->v = simd_add(*this, TVector2(f, MakeFromScalarFlag())).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator -= (const t_OtherScalar& f)							{ this->v = simd_sub(*this, TVector2(f, MakeFromScalarFlag())).v; }
	INLINE_NO_DEBUG void operator *= (const TVector2& u)								{ this->v = simd_mul(*this, u).v; }
	INLINE_NO_DEBUG void operator /= (const TVector2& u)								{ this->v = simd_div_by_xy11(*this, u).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)							{ this->v = simd_mul(*this, Float(f)).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)							{ this->v = simd_div(*this, Float(f)).v; }


	friend INLINE_NO_DEBUG TVector2 operator + (const TVector2& a, const TVector2& b)	{ return *(TVector2*)&simd_add(a, b); }
	friend INLINE_NO_DEBUG TVector2 operator - (const TVector2& a, const TVector2& b)	{ return *(TVector2*)&simd_sub(a, b); }
	friend INLINE_NO_DEBUG TVector2 operator * (const TVector2& a, const TVector2& b)	{ return *(TVector2*)&simd_mul(a, b); }
	friend INLINE_NO_DEBUG TVector2 operator / (const TVector2& a, const TVector2& b)	{ return *(TVector2*)&simd_div_by_xy11(a, b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator + (const t_OtherScalar& a, const TVector2& b) { return *(TVector2*)&simd_add(TVector2(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator - (const t_OtherScalar& a, const TVector2& b) { return *(TVector2*)&simd_sub(TVector2(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator * (const t_OtherScalar& a, const TVector2& b) { return *(TVector2*)&simd_mul(Float(a), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator / (const t_OtherScalar& a, const TVector2& b) { return *(TVector2*)&simd_div_by_xy11(TVector2(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator + (const TVector2& a, const t_OtherScalar& b) { return *(TVector2*)&simd_add(a, TVector2(b, MakeFromScalarFlag())); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator - (const TVector2& a, const t_OtherScalar& b) { return *(TVector2*)&simd_sub(a, TVector2(b, MakeFromScalarFlag())); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator * (const TVector2& a, const t_OtherScalar& b) { return *(TVector2*)&simd_mul(a, Float(b)); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator / (const TVector2& a, const t_OtherScalar& b) { return *(TVector2*)&simd_div(a, Float(b)); }

	INLINE_NO_DEBUG ScalarOptimal_t dot(const TVector2& u)const 						{ return *(ScalarOptimal_t*)&simd_dot2(*this, u); }
	INLINE_NO_DEBUG ScalarOptimal_t lenghtSquare()const									{ return *(ScalarOptimal_t*)&simd_lenghtSquare2(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t lenght()const										{ return *(ScalarOptimal_t*)&simd_lenght2(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t distanceSquareTo(const TVector2& u)const			{ return (u - *this).lenghtSquare(); }
	INLINE_NO_DEBUG ScalarOptimal_t distanceTo(const TVector2& u)const					{ return (u - *this).lenght(); }
	INLINE_NO_DEBUG ScalarOptimal_t componentSumm()const								{ return *(ScalarOptimal_t*)&simd_summ2(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t componentAbsSumm()const								{ return *(ScalarOptimal_t*)&simd_summ2(simd_abs(*this)); }
	INLINE_NO_DEBUG TVector2 cross()const												{ return TVector2(a[1],-a[0]); }
	INLINE_NO_DEBUG TVector2 normal()const												{ return *this * (*(ScalarOptimal_t*)&simd_invLenghtSquareRoot2(*this)); }
	INLINE_NO_DEBUG TVector2 normalFast()const											{ return *this * (*(ScalarOptimal_t*)&simd_invLenghtSquareRootFast2(*this)); }

	INLINE_NO_DEBUG boolean01 isAllPositive()const
	{ 
		uint32 flags = (~_mm_movemask_ps(v)) & 3;
		return (flags & (flags>>1)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAllNegative()const
	{ 
		uint32 flags = _mm_movemask_ps(v) & 3;
		return (flags & (flags>>1)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAnyPositive()const
	{
		uint32 flags = (~_mm_movemask_ps(v)) & 3;
		return (flags | (flags>>1)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAnyNegative()const
	{ 
		uint32 flags = _mm_movemask_ps(v) & 3;
		return (flags | (flags>>1)) & 1;
	}

	INLINE_NO_DEBUG TVector2()														{ setZero(); }
	INLINE_NO_DEBUG TVector2 operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG void selfClamp(const TVector2& low, const TVector2& hi);
	INLINE_NO_DEBUG void selfClamp01();
	INLINE_NO_DEBUG void selfClamp11();

	INLINE_NO_DEBUG boolean trySelfNormalize(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsq(lsq);
			return true;
		}
		return false;
	}

	INLINE_NO_DEBUG boolean trySelfNormalizeFast(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsqFast(lsq);
			return true;
		}
		return false;
	}
};

static INLINE_NO_DEBUG TVector2<float> rcp(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_rcpXY_zeroZW(vec);
}

static INLINE_NO_DEBUG TVector2<float> rsq(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_rsqXY_zeroZW(vec);
}
#endif


#ifdef INSTRUCTIONS_NEON
template<>
struct TVector2<float> : public FloatShortVector
{
	typedef FloatVector Base_t;
	typedef float Scalar_t;
	typedef FloatShort ScalarOptimal_t;


#ifdef DEBUG
	INLINE_NO_DEBUG void verify()const { Assert(isFinite(*this)); Assert(this->a[2] == 0.0f && this->a[3] == 0.0f); }
#else
	INLINE_NO_DEBUG void verify()const {}
#endif

	template<class t_OtherScalar>
	INLINE_NO_DEBUG TVector2(const t_OtherScalar& f, MakeFromScalarFlag flag)			{ Base_t::set(f); verify(); }
	template<typename t_ValX, typename t_ValY>
	INLINE_NO_DEBUG TVector2(const t_ValX& srcX, const t_ValY& srcY)					{ Base_t::set(srcX, srcY); verify(); }
	INLINE_NO_DEBUG TVector2(const Scalar_t* arr)										{ Base_t::set(arr); verify(); }
	INLINE_NO_DEBUG TVector2(const TVector2& v) : Base_t(*(const Base_t*)&v)			{ verify(); }
	INLINE_NO_DEBUG TVector2(const TPackedVector2<Scalar_t>& u)							{ Base_t::set(u.a); verify(); }

	template<class t_OtherScalar>
	INLINE_NO_DEBUG void set(const t_OtherScalar& f) 									{ Base_t::set(f); verify(); }
	template<typename t_ValX, typename t_ValY>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, const t_ValY& srcY) 					{ Base_t::set(srcX, srcY); verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr) 										{ Base_t::set(arr); verify(); };
	INLINE_NO_DEBUG void set(const TPackedVector2<Scalar_t>& u) 						{ Base_t::set(u.a); verify(); };
	INLINE_NO_DEBUG void set(const TVector2& u) 										{ this->v = u.v; verify(); };

	INLINE_NO_DEBUG void setZero() 														{ SIMD4::setZero(); verify(); }
	INLINE_NO_DEBUG void setOne()														{ SIMD4::setOne(); verify(); }
	INLINE_NO_DEBUG static TVector2 getZero() 											{ return *(TVector2*)&SIMD4::getZero(); }
	INLINE_NO_DEBUG static TVector2 getOne() 											{ return *(TVector2*)&SIMD4::getOne(); }
	INLINE_NO_DEBUG TVector2& operator = (const TVector2& u)							{ this->v = u.v; verify(); return *this; };
	INLINE_NO_DEBUG TVector2& operator = (const TPackedVector2<Scalar_t>& u) 			{ Base_t::setXY(u.a); verify(); return *this; };
	INLINE_NO_DEBUG boolean operator == (const TVector2& u)const						{ return !simd_noeqcmp(*this, u); }
	INLINE_NO_DEBUG boolean operator != (const TVector2& u)const						{ return simd_noeqcmp(*this, u); }

	INLINE_NO_DEBUG TVector2 operator - () const										{ return *(TVector2*)&simd_neg2(*this); }
	INLINE_NO_DEBUG void operator += (const TVector2& u)								{ this->v = simd_add(*this, u).v; }
	INLINE_NO_DEBUG void operator -= (const TVector2& u)								{ this->v = simd_sub(*this, u).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator += (const t_OtherScalar& f)							{ this->v = simd_add(*this, TVector2(f, MakeFromScalarFlag())).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator -= (const t_OtherScalar& f)							{ this->v = simd_sub(*this, TVector2(f, MakeFromScalarFlag())).v; }
	INLINE_NO_DEBUG void operator *= (const TVector2& u)								{ this->v = simd_mul(*this, u).v; }
	INLINE_NO_DEBUG void operator /= (const TVector2& u)								{ this->v = simd_div(*this, u).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)							{ this->v = simd_mul(*this, FloatShort(f)).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)							{ this->v = simd_div(*this, FloatShort(f)).v; }

	friend INLINE_NO_DEBUG TVector2 operator + (const TVector2& a, const TVector2& b)	{ return *(TVector2*)&simd_add(a, b); }
	friend INLINE_NO_DEBUG TVector2 operator - (const TVector2& a, const TVector2& b)	{ return *(TVector2*)&simd_sub(a, b); }
	friend INLINE_NO_DEBUG TVector2 operator * (const TVector2& a, const TVector2& b)	{ return *(TVector2*)&simd_mul(a, b); }
	friend INLINE_NO_DEBUG TVector2 operator / (const TVector2& a, const TVector2& b)	{ return *(TVector2*)&simd_div(a, b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator + (const t_OtherScalar& a, const TVector2& b) { return *(TVector2*)&simd_add(TVector2(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator - (const t_OtherScalar& a, const TVector2& b) { return *(TVector2*)&simd_sub(TVector2(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator * (const t_OtherScalar& a, const TVector2& b) { return *(TVector2*)&simd_mul(FloatShort(a), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator / (const t_OtherScalar& a, const TVector2& b) { return *(TVector2*)&simd_div(TVector2(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator + (const TVector2& a, const t_OtherScalar& b) { return *(TVector2*)&simd_add(a, TVector2(b, MakeFromScalarFlag())); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator - (const TVector2& a, const t_OtherScalar& b) { return *(TVector2*)&simd_sub(a, TVector2(b, MakeFromScalarFlag())); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator * (const TVector2& a, const t_OtherScalar& b) { return *(TVector2*)&simd_mul(a, FloatShort(b)); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector2 operator / (const TVector2& a, const t_OtherScalar& b) { return *(TVector2*)&simd_div(a, FloatShort(b)); }

	INLINE_NO_DEBUG ScalarOptimal_t dot(const TVector2& u)const 						{ return *(ScalarOptimal_t*)&simd_dot(*this, u); }
	INLINE_NO_DEBUG ScalarOptimal_t lenghtSquare()const									{ return *(ScalarOptimal_t*)&simd_lenghtSquare(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t lenght()const										{ return *(ScalarOptimal_t*)&simd_lenght(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t distanceSquareTo(const TVector2& u)const			{ return (u - *this).lenghtSquare(); }
	INLINE_NO_DEBUG ScalarOptimal_t distanceTo(const TVector2& u)const					{ return (u - *this).lenght(); }
	INLINE_NO_DEBUG ScalarOptimal_t componentSumm()const								{ return *(ScalarOptimal_t*)&simd_summ(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t componentAbsSumm()const								{ return *(ScalarOptimal_t*)&simd_summ(simd_abs(*this)); }
	INLINE_NO_DEBUG TVector2 cross()const												{ return TVector2(a[1],-a[0]); }
	INLINE_NO_DEBUG TVector2 normal()const												{ return *this * (*(ScalarOptimal_t*)&simd_invLenghtSquareRoot(*this)); }
	INLINE_NO_DEBUG TVector2 normalFast()const											{ return *this * (*(ScalarOptimal_t*)&simd_invLenghtSquareRootFast(*this)); }

	INLINE_NO_DEBUG boolean01 isAllPositive()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (~(nx | ny))>>31;
	}

	INLINE_NO_DEBUG boolean01 isAllNegative()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (nx & ny)>>31;
	}

	INLINE_NO_DEBUG boolean01 isAnyPositive()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (~(nx & ny))>>31;
	}

	INLINE_NO_DEBUG boolean01 isAnyNegative()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (nx | ny)>>31;
	}

	INLINE_NO_DEBUG TVector2()														{ setZero(); }
	INLINE_NO_DEBUG TVector2 operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG void selfClamp(const TVector2& low, const TVector2& hi);
	INLINE_NO_DEBUG void selfClamp01();
	INLINE_NO_DEBUG void selfClamp11();

	INLINE_NO_DEBUG boolean trySelfNormalize(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsq(lsq);
			return true;
		}
		return false;
	}

	INLINE_NO_DEBUG boolean trySelfNormalizeFast(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsqFast(lsq);
			return true;
		}
		return false;
	}
};

static INLINE_NO_DEBUG TVector2<float> rcp(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_rcp(vec);
}

static INLINE_NO_DEBUG TVector2<float> rsq(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_rsq(vec);
}
#endif


#ifdef INSTRUCTIONS_SIMD
static INLINE_NO_DEBUG TVector2<float> abs(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_abs(vec);
}

static INLINE_NO_DEBUG TVector2<float> sign(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_sign2_zw00(vec);
}

static INLINE_NO_DEBUG TVector2<float> min(const TVector2<float>& a, const TVector2<float>& b)
{
	return *(const TVector2<float>*)&simd_min(a, b);
}

static INLINE_NO_DEBUG TVector2<float> max(const TVector2<float>& a, const TVector2<float>& b)
{
	return *(const TVector2<float>*)&simd_max(a, b);
}

static INLINE_NO_DEBUG void minimize(TVector2<float>& a, const TVector2<float>& b)
{
	a = *(const TVector2<float>*)&simd_min(a, b);
}

static INLINE_NO_DEBUG void maximize(TVector2<float>& a, const TVector2<float>& b)
{
	a = *(const TVector2<float>*)&simd_max(a, b);
}

static INLINE_NO_DEBUG TVector2<float> clamp(const TVector2<float>& vec, const TVector2<float>& low, const TVector2<float>& hi)
{
	return *(const TVector2<float>*)&simd_clamp(vec, low, hi);
}

static INLINE_NO_DEBUG TVector2<float> clamp01(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_clamp01(vec);
}

static INLINE_NO_DEBUG TVector2<float> clamp11(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_clamp11(vec);
}

static INLINE_NO_DEBUG TVector2<float> sqrt(const TVector2<float>& vec)
{
	return *(const TVector2<float>*)&simd_sqrt(vec);
}

INLINE_NO_DEBUG void TVector2<float>::selfClamp(const TVector2<float>& low, const TVector2<float>& hi)	{ *this = clamp(*this, low, hi); }
INLINE_NO_DEBUG void TVector2<float>::selfClamp01()											{ *this = clamp01(*this); }
INLINE_NO_DEBUG void TVector2<float>::selfClamp11()											{ *this = clamp11(*this); }

template<>
class Vector2fRef<float> : public FloatVector
{
public:
	INLINE_NO_DEBUG void operator = (const TVector2<float>& other)
	{
		FloatVector::setXY(other);
	}
private:
	Vector2fRef(){}
	Vector2fRef(const Vector2fRef&){}
};

#endif


template<class t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TVector3<t_Scalar>& vec);

template<typename t_Scalar>
struct TVector3
{
	union
	{
		struct { t_Scalar x, y, z; };
		t_Scalar a[3];
	};

	typedef t_Scalar Scalar_t;
	typedef t_Scalar ScalarOptimal_t;

#ifdef DEBUG
	INLINE_NO_DEBUG void verify()const { Assert(isFinite(*this)); }
#else
	INLINE_NO_DEBUG void verify()const {}
#endif

	INLINE_NO_DEBUG TVector3(Scalar_t f, MakeFromScalarFlag flag) : x(f), y(f), z(f)					{verify();}
	INLINE_NO_DEBUG TVector3(Scalar_t srcX, Scalar_t srcY, Scalar_t srcZ) :  x(srcX), y(srcY), z(srcZ)	{verify();}
	INLINE_NO_DEBUG TVector3(const Scalar_t* arr) : x(arr[0]), y(arr[1]), z(arr[2])						{verify();}
	INLINE_NO_DEBUG TVector3(const TVector3& u) : x(u.x), y(u.y), z(u.z)								{verify();}
	INLINE_NO_DEBUG TVector3(const TPackedVector3<Scalar_t>& u): x(u.x), y(u.y), z(u.z)					{verify();}
	INLINE_NO_DEBUG TVector3(const TVector2<Scalar_t>& u, Scalar_t Z) : x(u.x), y(u.y), z(Z)			{verify();}

	INLINE_NO_DEBUG void set(Scalar_t f) 											{ x = y = z = f; verify(); }
	INLINE_NO_DEBUG void set(Scalar_t srcX, Scalar_t srcY, Scalar_t srcZ) 			{ x = srcX; y = srcY; z = srcZ; verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr) 									{ x = arr[0]; y = arr[1]; z = arr[2]; verify(); };
	INLINE_NO_DEBUG void set(const TPackedVector3<Scalar_t>& u) 					{ x = u.x; y = u.y; z = u.z; verify(); };
	INLINE_NO_DEBUG void set(const TVector3& u) 									{ x = u.x; y = u.y; z = u.z; verify(); };
	INLINE_NO_DEBUG void set(const TVector2<Scalar_t>& u, Scalar_t srcZ) 			{ x = u.x; y = u.y; z = srcZ; verify(); };
	INLINE_NO_DEBUG void setOne()													{ x = y = z = Scalar_t(1); }
	INLINE_NO_DEBUG void setZero() 													{ x = y = z = Scalar_t(0); }
	INLINE_NO_DEBUG static TVector3 getZero() 										{ return TVector3(Scalar_t(0), Scalar_t(0), Scalar_t(0)); }
	INLINE_NO_DEBUG static TVector3 getOne() 										{ return TVector3(Scalar_t(1), Scalar_t(1), Scalar_t(1)); }
	INLINE_NO_DEBUG TVector3& operator = (const TVector3& u)						{ x = u.x; y = u.y; z = u.z; verify(); return *this; }
	INLINE_NO_DEBUG TVector3& operator = (const TPackedVector3<Scalar_t>& u) 		{ x = u.x; y = u.y; z = u.z; verify(); return *this; }
	INLINE_NO_DEBUG boolean operator == (const TVector3& u)const					{ return x == u.x && y == u.y && z == u.z; }
	INLINE_NO_DEBUG boolean operator != (const TVector3& u)const					{ return x != u.x || y != u.y || z != u.z; }

	INLINE_NO_DEBUG TVector3 operator - () const									{ return TVector3(-x, -y, -z); }
	INLINE_NO_DEBUG void operator += (const TVector3& u)							{ x += u.x; y += u.y; z += u.z; }
	INLINE_NO_DEBUG void operator -= (const TVector3& u)							{ x -= u.x; y -= u.y; z -= u.z; }
	INLINE_NO_DEBUG void operator += (const Scalar_t f)								{ x += f; y += f; z += f; }
	INLINE_NO_DEBUG void operator -= (const Scalar_t f)								{ x -= f; y -= f; z -= f; }
	INLINE_NO_DEBUG void operator *= (const TVector3& u)							{ x *= u.x; y *= u.y; z *= u.z; }
	INLINE_NO_DEBUG void operator /= (const TVector3& u)							{ x /= u.x; y /= u.y; z /= u.z; }
	INLINE_NO_DEBUG void operator *= (const Scalar_t f)								{ x *= f; y *= f; z *= f; }
	INLINE_NO_DEBUG void operator /= (const Scalar_t f)								{ x /= f; y /= f; z /= f; }

	friend INLINE_NO_DEBUG TVector3 operator + (const TVector3& a, const TVector3& b){ return TVector3(a.x + b.x, a.y + b.y, a.z + b.z); }
	friend INLINE_NO_DEBUG TVector3 operator - (const TVector3& a, const TVector3& b){ return TVector3(a.x - b.x, a.y - b.y, a.z - b.z); }
	friend INLINE_NO_DEBUG TVector3 operator * (const TVector3& a, const TVector3& b){ return TVector3(a.x * b.x, a.y * b.y, a.z * b.z); }
	friend INLINE_NO_DEBUG TVector3 operator / (const TVector3& a, const TVector3& b){ return TVector3(a.x / b.x, a.y / b.y, a.z / b.z); }
	friend INLINE_NO_DEBUG TVector3 operator + (const Scalar_t f, const TVector3& u) { return TVector3(f + u.x, f + u.y, f + u.z); }
	friend INLINE_NO_DEBUG TVector3 operator - (const Scalar_t f, const TVector3& u) { return TVector3(f - u.x, f - u.y, f - u.z); }
	friend INLINE_NO_DEBUG TVector3 operator * (const Scalar_t f, const TVector3& u) { return TVector3(f * u.x, f * u.y, f * u.z); }
	friend INLINE_NO_DEBUG TVector3 operator / (const Scalar_t f, const TVector3& u) { return TVector3(f / u.x, f / u.y, f / u.z); }
	friend INLINE_NO_DEBUG TVector3 operator + (const TVector3& u, const Scalar_t f) { return TVector3(u.x + f, u.y + f, u.z + f); }
	friend INLINE_NO_DEBUG TVector3 operator - (const TVector3& u, const Scalar_t f) { return TVector3(u.x - f, u.y - f, u.z - f); }
	friend INLINE_NO_DEBUG TVector3 operator * (const TVector3& u, const Scalar_t f) { return TVector3(u.x * f, u.y * f, u.z * f); }
	friend INLINE_NO_DEBUG TVector3 operator / (const TVector3& u, const Scalar_t f) { return TVector3(u.x / f, u.y / f, u.z / f); }
	
	INLINE_NO_DEBUG Scalar_t dot(const TVector3& u)const 							{ return x * u.x + y * u.y + z * u.z; }
	INLINE_NO_DEBUG TVector3 cross(const TVector3& u)const 							{ return TVector3(a[1]*u[2] - a[2]*u[1], a[2]*u[0] - a[0]*u[2], a[0]*u[1] - a[1]*u[0]); }
	INLINE_NO_DEBUG Scalar_t lenghtSquare()const									{ return x * x + y * y + z * z; }
	INLINE_NO_DEBUG Scalar_t lenght()const											{ return Base::sqrt(lenghtSquare()); }
	INLINE_NO_DEBUG Scalar_t distanceSquareTo(const TVector3& u)const				{ return (u - *this).lenghtSquare(); }
	INLINE_NO_DEBUG Scalar_t distanceTo(const TVector3& u)const						{ return (u - *this).lenght(); }
	INLINE_NO_DEBUG Scalar_t componentSumm()const									{ return x + y + z; }
	INLINE_NO_DEBUG Scalar_t componentAbsSumm()const								{ return Base::abs(x) + Base::abs(y) + Base::abs(z); }
	INLINE_NO_DEBUG TVector3 normal()const											{ return *this * rsq(lenghtSquare()); }
	INLINE_NO_DEBUG TVector3 normalFast()const										{ return *this * rsqFast(lenghtSquare()); }

	INLINE_NO_DEBUG const TVector2<Scalar_t>& xy()const 							{ return *reinterpret_cast<const TVector2<Scalar_t>*>(this); }

	INLINE_NO_DEBUG boolean01 isAllPositive()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
		uint32 nz = *(uint32*)&a[2];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nz = *((uint32*)&a[2] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (~(nx | ny | nz))>>31;
	}

	INLINE_NO_DEBUG boolean01 isAllNegative()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
		uint32 nz = *(uint32*)&a[2];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nz = *((uint32*)&a[2] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (nx & ny & nz)>>31;
	}

	INLINE_NO_DEBUG boolean01 isAnyPositive()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
		uint32 nz = *(uint32*)&a[2];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nz = *((uint32*)&a[2] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (~(nx & ny & nz))>>31;
	}

	INLINE_NO_DEBUG boolean01 isAnyNegative()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
		uint32 nz = *(uint32*)&a[2];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nz = *((uint32*)&a[2] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (nx | ny | nz)>>31;
	}

	INLINE_NO_DEBUG TVector3()														{ setZero(); }
	INLINE_NO_DEBUG Vector2fRef<Scalar_t>& pxy() 									{ return *(Vector2fRef<Scalar_t>*)this; }
	INLINE_NO_DEBUG const TVector3& xyz()const 										{ return *this; }
	INLINE_NO_DEBUG const TVector4<Scalar_t> xyz0()const;
	INLINE_NO_DEBUG const TVector4<Scalar_t> xyz1()const;
	INLINE_NO_DEBUG TVector3 operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG TVector3 subdot(const TVector3& n)const 						{ return *this - n * n.dot(*this); }
	INLINE_NO_DEBUG TVector3 reflect(const TVector3& n)const 						{ ScalarOptimal_t f =  n.dot(*this); return *this - n * (f + f); }
	INLINE_NO_DEBUG TVector3 crossX()const											{ return TVector3(0.0f, a[2], -a[1]); }
	INLINE_NO_DEBUG TVector3 crossY()const											{ return TVector3(-a[2], 0.0f, a[0]); }
	INLINE_NO_DEBUG TVector3 crossZ()const											{ return TVector3(a[1], -a[0], 0.0f); }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG void selfClamp(const TVector3& low, const TVector3& hi);
	INLINE_NO_DEBUG void selfClamp01();
	INLINE_NO_DEBUG void selfClamp11();

	INLINE_NO_DEBUG boolean trySelfNormalize(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsq(lsq);
			return true;
		}
		return false;
	}

	INLINE_NO_DEBUG boolean trySelfNormalizeFast(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsqFast(lsq);
			return true;
		}
		return false;
	}
};


template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> abs(const TVector3<t_Scalar>& vec)
{
	return TVector3<t_Scalar>(abs(vec.x), abs(vec.y), abs(vec.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> sign(const TVector3<t_Scalar>& vec)
{
	return TVector3<t_Scalar>(sign(vec.x), sign(vec.y), sign(vec.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> min(const TVector3<t_Scalar>& a, const TVector3<t_Scalar>& b)
{
	return TVector3<t_Scalar>(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> max(const TVector3<t_Scalar>& a, const TVector3<t_Scalar>& b)
{
	return TVector3<t_Scalar>(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG void minimize(TVector3<t_Scalar>& a, const TVector3<t_Scalar>& b)
{
	minimize(a.x, b.x);
	minimize(a.y, b.y);
	minimize(a.z, b.z);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG void maximize(TVector3<t_Scalar>& a, const TVector3<t_Scalar>& b)
{
	maximize(a.x, b.x);
	maximize(a.y, b.y);
	maximize(a.z, b.z);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> clamp(const TVector3<t_Scalar>& vec, const TVector3<t_Scalar>& low, const TVector3<t_Scalar>& hi)
{
	return TVector3<t_Scalar>(clamp(vec.x, low.x, hi.x), clamp(vec.y, low.y, hi.y), clamp(vec.z, low.z, hi.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> clamp01(const TVector3<t_Scalar>& vec)
{
	return TVector3<t_Scalar>(clamp01(vec.x), clamp01(vec.y), clamp01(vec.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> clamp11(const TVector3<t_Scalar>& vec)
{
	return TVector3<t_Scalar>(clamp11(vec.x), clamp11(vec.y), clamp11(vec.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> rcp(const TVector3<t_Scalar>& vec)
{
	return TVector3<t_Scalar>(rcp(vec.x), rcp(vec.y), rcp(vec.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> rsq(const TVector3<t_Scalar>& vec)
{
	return TVector3<t_Scalar>(rsq(vec.x), rsq(vec.y), rsq(vec.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> sqrt(const TVector3<t_Scalar>& vec)
{
	return TVector3<t_Scalar>(sqrt(vec.x), sqrt(vec.y), sqrt(vec.z));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TVector3<t_Scalar>& a, const TVector3<t_Scalar>& b, t_Scalar eps)
{
	return Base::equals(a.a[0], b.a[0], eps) && Base::equals(a.a[1], b.a[1], eps) && Base::equals(a.a[2], b.a[2], eps);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TVector3<t_Scalar>& vec)
{
	return isFinite(vec.a[0]) && isFinite(vec.a[1]) && isFinite(vec.a[2]);
}

template<typename t_Scalar>
INLINE_NO_DEBUG void TVector3<t_Scalar>::selfClamp(const TVector3<t_Scalar>& low, const TVector3<t_Scalar>& hi) { *this = clamp(*this, low, hi); }
template<typename t_Scalar>
INLINE_NO_DEBUG void TVector3<t_Scalar>::selfClamp01() { *this = clamp01(*this); }
template<typename t_Scalar>
INLINE_NO_DEBUG void TVector3<t_Scalar>::selfClamp11() { *this = clamp11(*this); }

template<typename t_Scalar>
class Vector3fRef : public TVector3<t_Scalar>
{
public:
	INLINE_NO_DEBUG void operator = (const TVector3<t_Scalar>& other)
	{
		this->x = other.x; this->y = other.y; this->z = other.z;
	}
private:
	Vector3fRef(){}
	Vector3fRef(const Vector3fRef&){}
};



#ifdef INSTRUCTIONS_SIMD
template<>
struct TVector3<float> : public FloatVector
{
	typedef FloatVector Base_t;
	typedef float Scalar_t;
	typedef Float ScalarOptimal_t;

#ifdef DEBUG
	INLINE_NO_DEBUG void verify()const { Assert(isFinite(*this)); Assert(a[3] == 0.0f); }
#else
	INLINE_NO_DEBUG void verify()const {}
#endif

	template<class t_OtherScalar>
	INLINE_NO_DEBUG TVector3(const t_OtherScalar& f, MakeFromScalarFlag flag)			{ Base_t::setXYZ0(f); verify(); }
	template<typename t_ValX, typename t_ValY, typename t_ValZ>
	INLINE_NO_DEBUG TVector3(const t_ValX& srcX, const t_ValY& srcY, const t_ValZ& srcZ){ Base_t::setXYZ0(srcX, srcY, srcZ); verify(); }
	INLINE_NO_DEBUG TVector3(const Scalar_t* arr)										{ Base_t::setXYZ0(arr); verify(); }
	INLINE_NO_DEBUG TVector3(const TVector3& u) : Base_t(*(const Base_t*)&u)			{ verify(); }
	INLINE_NO_DEBUG TVector3(const TPackedVector3<Scalar_t>& u)							{ Base_t::setXYZ0(u.a); verify(); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG TVector3(const TVector2<Scalar_t>& xy, const t_OtherScalar& Z)		{ Base_t::setXY_Z0(*(const Base_t*)&xy, Z); verify(); }
	
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void set(const t_OtherScalar& f) 										{ Base_t::setXYZ0(f); verify(); }
	template<typename t_ValX, typename t_ValY, typename t_ValZ>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, const t_ValY& srcY, const t_ValZ& srcZ){ Base_t::setXYZ0(srcX, srcY, srcZ); verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr) 										{ Base_t::setXYZ0(arr); verify(); };
	INLINE_NO_DEBUG void set(const TPackedVector3<Scalar_t>& u) 						{ Base_t::setXYZ0(u.a); verify(); };
	INLINE_NO_DEBUG void set(const TVector3& u) 										{ this->v = u.v; verify(); };
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void set(const TVector2<Scalar_t>& xy, const t_OtherScalar& Z)		{ Base_t::setXY_Z0(*(const Base_t*)&xy, Z); verify(); }
	
	INLINE_NO_DEBUG void setZero() 														{ SIMD4::setZero(); verify(); }
	INLINE_NO_DEBUG void setOne()														{ SIMD4::setXYZW1110(); verify(); }
	INLINE_NO_DEBUG static TVector3 getZero() 											{ return *(TVector3*)&SIMD4::getZero(); }
	INLINE_NO_DEBUG static TVector3 getOne() 											{ return *(TVector3*)&SIMD4::getXYZW1110(); }
	INLINE_NO_DEBUG TVector3& operator = (const TVector3& u)							{ this->v = u.v; verify(); return *this; };
	INLINE_NO_DEBUG TVector3& operator = (const TPackedVector3<Scalar_t>& u) 			{ Base_t::setXYZ0(u.a); verify(); return *this; };
	INLINE_NO_DEBUG boolean operator == (const TVector3& u)const						{ return !simd_noeqcmp3(*this, u); }
	INLINE_NO_DEBUG boolean operator != (const TVector3& u)const						{ return simd_noeqcmp3(*this, u); }

	INLINE_NO_DEBUG TVector3 operator - () const										{ return *(TVector3*)&simd_neg3(*this); }
	INLINE_NO_DEBUG void operator += (const TVector3& u)								{ this->v = simd_add(*this, u).v; }
	INLINE_NO_DEBUG void operator -= (const TVector3& u)								{ this->v = simd_sub(*this, u).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator += (const t_OtherScalar& f)							{ this->v = simd_add(*this, TVector3(f, MakeFromScalarFlag())).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator -= (const t_OtherScalar& f)							{ this->v = simd_sub(*this, TVector3(f, MakeFromScalarFlag())).v; }
	INLINE_NO_DEBUG void operator *= (const TVector3& u)								{ this->v = simd_mul(*this, u).v; }
	INLINE_NO_DEBUG void operator /= (const TVector3& u)								{ this->v = simd_div_by_xyz1(*this, u).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)							{ this->v = simd_mul(*this, Float(f)).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)							{ this->v = simd_div(*this, Float(f)).v; }

	friend INLINE_NO_DEBUG TVector3 operator + (const TVector3& a, const TVector3& b)	{ return *(TVector3*)&simd_add(a, b); }
	friend INLINE_NO_DEBUG TVector3 operator - (const TVector3& a, const TVector3& b)	{ return *(TVector3*)&simd_sub(a, b); }
	friend INLINE_NO_DEBUG TVector3 operator * (const TVector3& a, const TVector3& b)	{ return *(TVector3*)&simd_mul(a, b); }
	friend INLINE_NO_DEBUG TVector3 operator / (const TVector3& a, const TVector3& b)	{ return *(TVector3*)&simd_div_by_xyz1(a, b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector3 operator + (const t_OtherScalar& a, const TVector3& b){ return *(TVector3*)&simd_add(TVector3(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector3 operator - (const t_OtherScalar& a, const TVector3& b){ return *(TVector3*)&simd_sub(TVector3(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector3 operator * (const t_OtherScalar& a, const TVector3& b){ return *(TVector3*)&simd_mul(Float(a), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector3 operator / (const t_OtherScalar& a, const TVector3& b){ return *(TVector3*)&simd_div_by_xyz1(Float(a), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector3 operator + (const TVector3& a, const t_OtherScalar& b){ return *(TVector3*)&simd_add(a, TVector3(b, MakeFromScalarFlag())); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector3 operator - (const TVector3& a, const t_OtherScalar& b){ return *(TVector3*)&simd_sub(a, TVector3(b, MakeFromScalarFlag())); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector3 operator * (const TVector3& a, const t_OtherScalar& b){ return *(TVector3*)&simd_mul(a, Float(b)); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector3 operator / (const TVector3& a, const t_OtherScalar& b) { return *(TVector3*)&simd_div(a, Float(b)); }

	INLINE_NO_DEBUG ScalarOptimal_t dot(const TVector3& u)const 					{ return *(ScalarOptimal_t*)&simd_dot(*this, u); }
	INLINE_NO_DEBUG TVector3 cross(const TVector3& u)const 							{ return *(TVector3*)&simd_cross3(*this, u); }
	INLINE_NO_DEBUG ScalarOptimal_t lenghtSquare()const								{ return *(ScalarOptimal_t*)&simd_lenghtSquare(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t lenght()const									{ return *(ScalarOptimal_t*)&simd_lenght(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t distanceSquareTo(const TVector3& u)const		{ return (u - *this).lenghtSquare(); }
	INLINE_NO_DEBUG ScalarOptimal_t distanceTo(const TVector3& u)const				{ return (u - *this).lenght(); }
	INLINE_NO_DEBUG ScalarOptimal_t componentSumm()const							{ return *(ScalarOptimal_t*)&simd_summ(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t componentAbsSumm()const							{ return *(ScalarOptimal_t*)&simd_summ(simd_abs(*this)); }
	INLINE_NO_DEBUG TVector3 normal()const											{ return *this * (*(ScalarOptimal_t*)&simd_invLenghtSquareRoot(*this)); }
	INLINE_NO_DEBUG TVector3 normalFast()const										{ return *this * (*(ScalarOptimal_t*)&simd_invLenghtSquareRootFast(*this)); }

	INLINE_NO_DEBUG const TVector2<Scalar_t> xy()const 								{ Base_t ret; ret.setXY00(*(const Base_t*)this); return *(TVector2<Scalar_t>*)&ret; }

	INLINE_NO_DEBUG boolean01 isAllPositive()const
	{ 
		uint32 flags = (~_mm_movemask_ps(v)) & 7;
		return (flags & (flags>>1) & (flags>>2)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAllNegative()const
	{ 
		uint32 flags = _mm_movemask_ps(v) & 7;
		return (flags & (flags>>1) & (flags>>2)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAnyPositive()const
	{
		uint32 flags = (~_mm_movemask_ps(v)) & 7;
		return (flags | (flags>>1) | (flags>>2)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAnyNegative()const
	{ 
		uint32 flags = _mm_movemask_ps(v) & 7;
		return (flags | (flags>>1) | (flags>>2)) & 1;
	}

	INLINE_NO_DEBUG TVector3()														{ setZero(); }
	INLINE_NO_DEBUG Vector2fRef<Scalar_t>& pxy() 									{ return *(Vector2fRef<Scalar_t>*)this; }
	INLINE_NO_DEBUG const TVector3& xyz()const 										{ return *this; }
	INLINE_NO_DEBUG const TVector4<Scalar_t> xyz0()const;
	INLINE_NO_DEBUG const TVector4<Scalar_t> xyz1()const;
	INLINE_NO_DEBUG TVector3 operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG TVector3 subdot(const TVector3& n)const 						{ return *this - n * n.dot(*this); }
	INLINE_NO_DEBUG TVector3 reflect(const TVector3& n)const 						{ ScalarOptimal_t f =  n.dot(*this); return *this - n * (f + f); }
	INLINE_NO_DEBUG TVector3 crossX()const											{ return TVector3(0.0f, a[2], -a[1]); }
	INLINE_NO_DEBUG TVector3 crossY()const											{ return TVector3(-a[2], 0.0f, a[0]); }
	INLINE_NO_DEBUG TVector3 crossZ()const											{ return TVector3(a[1], -a[0], 0.0f); }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG void selfClamp(const TVector3& low, const TVector3& hi);
	INLINE_NO_DEBUG void selfClamp01();
	INLINE_NO_DEBUG void selfClamp11();

	INLINE_NO_DEBUG boolean trySelfNormalize(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsq(lsq);
			return true;
		}
		return false;
	}

	INLINE_NO_DEBUG boolean trySelfNormalizeFast(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsqFast(lsq);
			return true;
		}
		return false;
	}
};

static INLINE_NO_DEBUG TVector3<float> abs(const TVector3<float>& vec)
{
	return *(const TVector3<float>*)&simd_abs(vec);
}

static INLINE_NO_DEBUG TVector3<float> sign(const TVector3<float>& vec)
{
	return *(const TVector3<float>*)&simd_sign3_w0(vec);
}

static INLINE_NO_DEBUG TVector3<float> min(const TVector3<float>& a, const TVector3<float>& b)
{
	return *(const TVector3<float>*)&simd_min(a, b);
}

static INLINE_NO_DEBUG TVector3<float> max(const TVector3<float>& a, const TVector3<float>& b)
{
	return *(const TVector3<float>*)&simd_max(a, b);
}

static INLINE_NO_DEBUG void minimize(TVector3<float>& a, const TVector3<float>& b)
{
	a = *(const TVector3<float>*)&simd_min(a, b);
}

static INLINE_NO_DEBUG void maximize(TVector3<float>& a, const TVector3<float>& b)
{
	a = *(const TVector3<float>*)&simd_max(a, b);
}

static INLINE_NO_DEBUG TVector3<float> clamp(const TVector3<float>& vec, const TVector3<float>& low, const TVector3<float>& hi)
{
	return *(const TVector3<float>*)&simd_clamp(vec, low, hi);
}

static INLINE_NO_DEBUG TVector3<float> clamp01(const TVector3<float>& vec)
{
	return *(const TVector3<float>*)&simd_clamp01(vec);
}

static INLINE_NO_DEBUG TVector3<float> clamp11(const TVector3<float>& vec)
{
	return *(const TVector3<float>*)&simd_clamp11(vec);
}

static INLINE_NO_DEBUG TVector3<float> rcp(const TVector3<float>& vec)
{
	return *(const TVector3<float>*)&simd_rcpXYZ_zeroW(vec);
}

static INLINE_NO_DEBUG TVector3<float> rsq(const TVector3<float>& vec)
{
	return *(const TVector3<float>*)&simd_rsqXYZ_zeroW(vec);
}

static INLINE_NO_DEBUG TVector3<float> sqrt(const TVector3<float>& vec)
{
	return *(const TVector3<float>*)&simd_sqrt(vec);
}

INLINE_NO_DEBUG void TVector3<float>::selfClamp(const TVector3<float>& low, const TVector3<float>& hi) { *this = clamp(*this, low, hi); }
INLINE_NO_DEBUG void TVector3<float>::selfClamp01() { *this = clamp01(*this); }
INLINE_NO_DEBUG void TVector3<float>::selfClamp11() { *this = clamp11(*this); }

template<>
class Vector3fRef<float> : public FloatVector
{
public:
	INLINE_NO_DEBUG void operator = (const TVector3<float>& other)
	{
		FloatVector::setXYZ(other);
	}
private:
	Vector3fRef(){}
	Vector3fRef(const Vector3fRef&){}
};

#endif



template<class t_Scalar>
class Vector2fZWRef;

template<class t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TVector4<t_Scalar>& vec);

template<typename t_Scalar>
struct TVector4
{
	union
	{
		struct { t_Scalar x, y, z, w; };
		t_Scalar a[4];
	};

	typedef t_Scalar Scalar_t;
	typedef t_Scalar ScalarOptimal_t;

#ifdef DEBUG
	INLINE_NO_DEBUG void verify()const { Assert(isFinite(*this)); }
#else
	INLINE_NO_DEBUG void verify()const {}
#endif

	INLINE_NO_DEBUG TVector4(Scalar_t f, MakeFromScalarFlag flag) : x(f), y(f), z(f), w(f)										{verify();}
	INLINE_NO_DEBUG TVector4(Scalar_t srcX, Scalar_t srcY, Scalar_t srcZ, Scalar_t srcW) :  x(srcX), y(srcY), z(srcZ), w(srcW)  {verify();}
	INLINE_NO_DEBUG TVector4(const Scalar_t* arr) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])									{verify();}
	INLINE_NO_DEBUG TVector4(const TVector4& u) : x(u.x), y(u.y), z(u.z), w(u.w)												{verify();}
	INLINE_NO_DEBUG TVector4(const TPackedVector4<Scalar_t>& u): x(u.x), y(u.y), z(u.z), w(u.w)									{verify();}
	INLINE_NO_DEBUG TVector4(const TVector3<Scalar_t>& u, Scalar_t srcW) : x(u.x), y(u.y), z(u.z), w(srcW)						{verify();}
	INLINE_NO_DEBUG TVector4(const TVector2<Scalar_t>& xy, const TVector2<Scalar_t>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y)	{verify();}

	INLINE_NO_DEBUG void set(Scalar_t f) 											{ x = y = z = w = f; verify(); }
	INLINE_NO_DEBUG void set(Scalar_t srcX, Scalar_t srcY, Scalar_t srcZ, Scalar_t srcW) { x = srcX; y = srcY; z = srcZ; w = srcW; verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr) 									{ x = arr[0]; y = arr[1]; z = arr[2]; w = arr[3]; verify(); };
	INLINE_NO_DEBUG void set(const TPackedVector4<Scalar_t>& u) 					{ x = u.x; y = u.y; z = u.z; w = u.w; verify(); };
	INLINE_NO_DEBUG void set(const TVector4& u) 									{ x = u.x; y = u.y; z = u.z; w = u.w; verify(); };
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& u, Scalar_t W) 				{ x = u.x; y = u.y; z = u.z; w = W; verify(); };
	INLINE_NO_DEBUG void set(const TVector2<Scalar_t>& xy, const TVector2<Scalar_t>& zw) { x = xy.x; y = xy.y; z = zw.x; w = zw.y; verify(); };
	INLINE_NO_DEBUG void setOne()													{ x = y = z = w = Scalar_t(1); }
	INLINE_NO_DEBUG void setZero() 													{ x = y = z = w = Scalar_t(0); }
	INLINE_NO_DEBUG static TVector4 getZero() 										{ return TVector4(Scalar_t(0), Scalar_t(0), Scalar_t(0), Scalar_t(0)); }
	INLINE_NO_DEBUG static TVector4 getOne() 										{ return TVector4(Scalar_t(1), Scalar_t(1), Scalar_t(1), Scalar_t(1)); }
	INLINE_NO_DEBUG TVector4& operator = (const TVector4& u)						{ x = u.x; y = u.y; z = u.z; w = u.w; verify(); return *this; }
	INLINE_NO_DEBUG TVector4& operator = (const TPackedVector4<Scalar_t>& u) 		{ x = u.x; y = u.y; z = u.z; w = u.w; verify(); return *this; }
	INLINE_NO_DEBUG boolean operator == (const TVector4& u)const					{ return x == u.x && y == u.y && z == u.z && w == u.w; }
	INLINE_NO_DEBUG boolean operator != (const TVector4& u)const					{ return x != u.x || y != u.y || z != u.z || w != u.w; }

	INLINE_NO_DEBUG TVector4 operator - () const									{ return TVector4(-x, -y, -z, -w); }
	INLINE_NO_DEBUG void operator += (const TVector4& u)							{ x += u.x; y += u.y; z += u.z; w += u.w; }
	INLINE_NO_DEBUG void operator -= (const TVector4& u)							{ x -= u.x; y -= u.y; z -= u.z; w -= u.w; }
	INLINE_NO_DEBUG void operator += (const Scalar_t f)								{ x += f; y += f; z += f; w += f; }
	INLINE_NO_DEBUG void operator -= (const Scalar_t f)								{ x -= f; y -= f; z -= f; w -= f; }
	INLINE_NO_DEBUG void operator *= (const TVector4& u)							{ x *= u.x; y *= u.y; z *= u.z; w *= u.w; }
	INLINE_NO_DEBUG void operator /= (const TVector4& u)							{ x /= u.x; y /= u.y; z /= u.z; w /= u.w; }
	INLINE_NO_DEBUG void operator *= (const Scalar_t f)								{ x *= f; y *= f; z *= f; w *= f; }
	INLINE_NO_DEBUG void operator /= (const Scalar_t f)								{ x /= f; y /= f; z /= f; w /= f; }

	friend INLINE_NO_DEBUG TVector4 operator + (const TVector4& a, const TVector4& b){ return TVector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
	friend INLINE_NO_DEBUG TVector4 operator - (const TVector4& a, const TVector4& b){ return TVector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
	friend INLINE_NO_DEBUG TVector4 operator * (const TVector4& a, const TVector4& b){ return TVector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
	friend INLINE_NO_DEBUG TVector4 operator / (const TVector4& a, const TVector4& b){ return TVector4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
	friend INLINE_NO_DEBUG TVector4 operator + (const Scalar_t a, const TVector4& b) { return TVector4(a + b.x, a + b.y, a + b.z, a + b.w); }
	friend INLINE_NO_DEBUG TVector4 operator - (const Scalar_t a, const TVector4& b) { return TVector4(a - b.x, a - b.y, a - b.z, a - b.w); }
	friend INLINE_NO_DEBUG TVector4 operator * (const Scalar_t a, const TVector4& b) { return TVector4(a * b.x, a * b.y, a * b.z, a * b.w); }
	friend INLINE_NO_DEBUG TVector4 operator / (const Scalar_t a, const TVector4& b) { return TVector4(a / b.x, a / b.y, a / b.z, a / b.w); }
	friend INLINE_NO_DEBUG TVector4 operator + (const TVector4& a, const Scalar_t b) { return TVector4(a.x + b, a.y + b, a.z + b, a.w + b); }
	friend INLINE_NO_DEBUG TVector4 operator - (const TVector4& a, const Scalar_t b) { return TVector4(a.x - b, a.y - b, a.z - b, a.w - b); }
	friend INLINE_NO_DEBUG TVector4 operator * (const TVector4& a, const Scalar_t b) { return TVector4(a.x * b, a.y * b, a.z * b, a.w * b); }
	friend INLINE_NO_DEBUG TVector4 operator / (const TVector4& a, const Scalar_t b) { return TVector4(a.x / b, a.y / b, a.z / b, a.w / b); }

	INLINE_NO_DEBUG Scalar_t dot(const TVector4& u)const 							{ return x * u.x + y * u.y + z * u.z + w * u.w; }
	INLINE_NO_DEBUG Scalar_t lenghtSquare()const									{ return x * x + y * y + z * z + w * w; }
	INLINE_NO_DEBUG Scalar_t lenght()const											{ return Base::sqrt(lenghtSquare()); }
	INLINE_NO_DEBUG Scalar_t distanceSquareTo(const TVector4& u)const				{ return (u - *this).lenghtSquare(); }
	INLINE_NO_DEBUG Scalar_t distanceTo(const TVector4& u)const						{ return (u - *this).lenght(); }
	INLINE_NO_DEBUG Scalar_t componentSumm()const									{ return x + y + z; }
	INLINE_NO_DEBUG Scalar_t componentAbsSumm()const								{ return Base::abs(x) + Base::abs(y) + Base::abs(z) + Base::abs(w); }
	INLINE_NO_DEBUG TVector4 normal()const											{ return *this * rsq(lenghtSquare()); }
	INLINE_NO_DEBUG TVector4 normalFast()const										{ return *this * rsqFast(lenghtSquare()); }

	INLINE_NO_DEBUG const TVector2<Scalar_t> xy()const 								{ return *reinterpret_cast<const TVector2<Scalar_t>*>(this); }
	INLINE_NO_DEBUG const TVector2<Scalar_t> zw()const 								{ return *reinterpret_cast<const TVector2<Scalar_t>*>(&this->z); }
	INLINE_NO_DEBUG const TVector3<Scalar_t> xyz()const 							{ return *reinterpret_cast<const TVector3<Scalar_t>*>(this); }

	INLINE_NO_DEBUG boolean01 isAllPositive()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
		uint32 nz = *(uint32*)&a[2];
		uint32 nw = *(uint32*)&a[3];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nz = *((uint32*)&a[2] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nw = *((uint32*)&a[3] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (~(nx | ny | nz | nw))>>31;
	}

	INLINE_NO_DEBUG boolean01 isAllNegative()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
		uint32 nz = *(uint32*)&a[2];
		uint32 nw = *(uint32*)&a[3];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nz = *((uint32*)&a[2] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nw = *((uint32*)&a[3] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (nx & ny & nz & nw)>>31;
	}

	INLINE_NO_DEBUG boolean01 isAnyPositive()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
		uint32 nz = *(uint32*)&a[2];
		uint32 nw = *(uint32*)&a[3];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nz = *((uint32*)&a[2] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nw = *((uint32*)&a[3] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (~(nx & ny & nz & nw))>>31;
	}

	INLINE_NO_DEBUG boolean01 isAnyNegative()const
	{ 
		static_assert(sizeof(t_Scalar) >= 4, "size so small");
#ifdef BIG_ENDIAN_ARCH
		uint32 nx = *(uint32*)&a[0];
		uint32 ny = *(uint32*)&a[1];
		uint32 nz = *(uint32*)&a[2];
		uint32 nw = *(uint32*)&a[3];
#else
		uint32 nx = *((uint32*)&a[0] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 ny = *((uint32*)&a[1] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nz = *((uint32*)&a[2] + (sizeof(t_Scalar) - sizeof(uint32)));
		uint32 nw = *((uint32*)&a[3] + (sizeof(t_Scalar) - sizeof(uint32)));
#endif
		return (nx | ny | nz | nw)>>31;
	}

	INLINE_NO_DEBUG TVector4()														{ setZero(); }
	INLINE_NO_DEBUG Vector2fRef<Scalar_t>& pxy() 									{ return *(Vector2fRef<Scalar_t>*)this; }
	INLINE_NO_DEBUG Vector2fZWRef<Scalar_t>& pzw() 									{ return *(Vector2fZWRef<Scalar_t>*)this; }	
	INLINE_NO_DEBUG Vector3fRef<Scalar_t>& pxyz() 									{ return *(Vector3fRef<Scalar_t>*)this; }	
	INLINE_NO_DEBUG TVector4 operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG TVector4 subdot(const TVector4& n)const 						{ return *this - n * n.dot(*this); }
	INLINE_NO_DEBUG TVector4 reflect(const TVector4& n)const 						{ Scalar_t f = n.dot(*this); return *this - n * (f + f); }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG void selfClamp(const TVector4& low, const TVector4& hi);
	INLINE_NO_DEBUG void selfClamp01();
	INLINE_NO_DEBUG void selfClamp11();

	INLINE_NO_DEBUG boolean trySelfNormalize(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsq(lsq);
			return true;
		}
		return false;
	}

	INLINE_NO_DEBUG boolean trySelfNormalizeFast(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsqFast(lsq);
			return true;
		}
		return false;
	}
};

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> abs(const TVector4<t_Scalar>& vec)
{
	return TVector4<t_Scalar>(abs(vec.x), abs(vec.y), abs(vec.z), abs(vec.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> sign(const TVector4<t_Scalar>& vec)
{
	return TVector4<t_Scalar>(sign(vec.x), sign(vec.y), sign(vec.z), sign(vec.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> min(const TVector4<t_Scalar>& a, const TVector4<t_Scalar>& b)
{
	return TVector4<t_Scalar>(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> max(const TVector4<t_Scalar>& a, const TVector4<t_Scalar>& b)
{
	return TVector4<t_Scalar>(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG void minimize(TVector4<t_Scalar>& a, const TVector4<t_Scalar>& b)
{
	minimize(a.x, b.x);
	minimize(a.y, b.y);
	minimize(a.z, b.z);
	minimize(a.w, b.w);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG void maximize(TVector4<t_Scalar>& a, const TVector4<t_Scalar>& b)
{
	maximize(a.x, b.x);
	maximize(a.y, b.y);
	maximize(a.z, b.z);
	maximize(a.w, b.w);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> clamp(const TVector4<t_Scalar>& vec, const TVector4<t_Scalar>& low, const TVector4<t_Scalar>& hi)
{
	return TVector4<t_Scalar>(clamp(vec.x, low.x, hi.x), clamp(vec.y, low.y, hi.y), clamp(vec.z, low.z, hi.z), clamp(vec.w, low.w, hi.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> clamp01(const TVector4<t_Scalar>& vec)
{
	return TVector4<t_Scalar>(clamp01(vec.x), clamp01(vec.y), clamp01(vec.z), clamp01(vec.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> clamp11(const TVector4<t_Scalar>& vec)
{
	return TVector4<t_Scalar>(clamp11(vec.x), clamp11(vec.y), clamp11(vec.z), clamp11(vec.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> rcp(const TVector4<t_Scalar>& vec)
{
	return TVector4<t_Scalar>(rcp(vec.x), rcp(vec.y), rcp(vec.z), rcp(vec.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> rsq(const TVector4<t_Scalar>& vec)
{
	return TVector4<t_Scalar>(rsq(vec.x), rsq(vec.y), rsq(vec.z), rsq(vec.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG TVector4<t_Scalar> sqrt(const TVector4<t_Scalar>& vec)
{
	return TVector4<t_Scalar>(sqrt(vec.x), sqrt(vec.y), sqrt(vec.z), sqrt(vec.w));
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TVector4<t_Scalar>& a, const TVector4<t_Scalar>& b, t_Scalar eps)
{
	return Base::equals(a.a[0], b.a[0], eps) && Base::equals(a.a[1], b.a[1], eps) && Base::equals(a.a[2], b.a[2], eps) && Base::equals(a.a[3], b.a[3], eps);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TVector4<t_Scalar>& vec)
{
	return isFinite(vec.a[0]) && isFinite(vec.a[1]) && isFinite(vec.a[2]) && isFinite(vec.a[3]);
}

template<typename t_Scalar>
INLINE_NO_DEBUG void TVector4<t_Scalar>::selfClamp(const TVector4<t_Scalar>& low, const TVector4<t_Scalar>& hi)		{ *this = clamp(*this, low, hi); }
template<typename t_Scalar>
INLINE_NO_DEBUG void TVector4<t_Scalar>::selfClamp01()												{ *this = clamp01(*this); }
template<typename t_Scalar>
INLINE_NO_DEBUG void TVector4<t_Scalar>::selfClamp11()												{ *this = clamp11(*this); }


template<class t_Scalar>
class Vector2fZWRef : public TVector4<t_Scalar>
{
public:
	INLINE_NO_DEBUG void operator = (const TVector2<t_Scalar>& other)
	{
		this->z = other.x; this->w = other.y;
	}
private:
	Vector2fZWRef(){}
	Vector2fZWRef(const Vector2fZWRef&){}
};

template<class t_Scalar>
INLINE_NO_DEBUG const TVector4<t_Scalar> TVector3<t_Scalar>::xyz0()const
{
	return TVector4<t_Scalar>(x, y, z, t_Scalar(0));
}

template<class t_Scalar>
INLINE_NO_DEBUG const TVector4<t_Scalar> TVector3<t_Scalar>::xyz1()const
{
	return TVector4<Scalar_t>(x, y, z, t_Scalar(1));
}


#ifdef INSTRUCTIONS_SIMD

template<>
class Vector2fZWRef<float> : public FloatVector
{
public:
	INLINE_NO_DEBUG void operator = (const TVector2<float>& other)
	{
		FloatVector::setZW(other);
	}
private:
	Vector2fZWRef(){}
	Vector2fZWRef(const Vector2fZWRef&){}
};

template<>
struct TVector4<float> : public FloatVector
{
	typedef FloatVector Base_t;
	typedef float Scalar_t;
	typedef Float ScalarOptimal_t;

#ifdef DEBUG
	INLINE_NO_DEBUG void verify()const { Assert(isFinite(*this)); }
#else
	INLINE_NO_DEBUG void verify()const {}
#endif

	template<class t_OtherScalar>
	INLINE_NO_DEBUG TVector4(const t_OtherScalar& f, MakeFromScalarFlag flag)			{ Base_t::set(f); verify(); }
	template<typename t_ValX, typename t_ValY, typename t_ValZ, typename t_ValW>
	INLINE_NO_DEBUG TVector4(const t_ValX& srcX, const t_ValY& srcY, const t_ValZ& srcZ, const t_ValW& srcW) { Base_t::set(srcX, srcY, srcZ, srcW); verify(); }
	INLINE_NO_DEBUG TVector4(const Scalar_t* arr)										{ Base_t::set(arr); verify(); }
	INLINE_NO_DEBUG TVector4(const TVector4& u) : Base_t(*(const Base_t*)&u)			{ verify(); }
	INLINE_NO_DEBUG TVector4(const TPackedVector4<Scalar_t>& u)							{ Base_t::set(u.a); verify(); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG TVector4(const TVector3<Scalar_t>& xyz, const t_OtherScalar& W)		{ Base_t::setXYZ_W(*(const Base_t*)&xyz, W); verify(); }
	INLINE_NO_DEBUG TVector4(const TVector2<Scalar_t>& xy, const TVector2<Scalar_t>& zw){ Base_t::setXY_ZW(*(const Base_t*)&xy, *(const Base_t*)&zw); verify(); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void set(const t_OtherScalar& f)									{ Base_t::set(f); verify(); }
	template<typename t_ValX, typename t_ValY, typename t_ValZ, typename t_ValW>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, const t_ValY& srcY, const t_ValZ& srcZ, const t_ValW& srcW){ Base_t::set(srcX, srcY, srcZ, srcW); verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr)										{ Base_t::set(arr); verify(); }
	INLINE_NO_DEBUG void set(const TVector4& u)											{ this->v = u.v; verify(); }
	INLINE_NO_DEBUG void set(const TPackedVector4<Scalar_t>& u)							{ Base_t::set(u.a); verify(); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& xyz, const t_OtherScalar& W)		{ Base_t::setXYZ_W(*(const Base_t*)&xyz, W); verify(); }
	INLINE_NO_DEBUG void set(const TVector2<Scalar_t>& xy, const TVector2<Scalar_t>& zw){ Base_t::setXY_ZW(*(const Base_t*)&xy, *(const Base_t*)&zw); verify(); }
	
	INLINE_NO_DEBUG void setZero() 														{ SIMD4::setZero(); verify(); }
	INLINE_NO_DEBUG void setOne()														{ SIMD4::setOne(); verify(); }
	INLINE_NO_DEBUG static TVector4 getZero() 											{ return *(TVector4*)&SIMD4::getZero(); }
	INLINE_NO_DEBUG static TVector4 getOne() 											{ return *(TVector4*)&SIMD4::getOne(); }
	INLINE_NO_DEBUG TVector4& operator = (const TVector4& u)							{ this->v = u.v; verify(); return *this; };
	INLINE_NO_DEBUG TVector4& operator = (const TPackedVector4<Scalar_t>& u) 			{ Base_t::set(u.a); verify(); return *this; };
	INLINE_NO_DEBUG boolean operator == (const TVector4& u)const						{ return !simd_noeqcmp(*this, u); }
	INLINE_NO_DEBUG boolean operator != (const TVector4& u)const						{ return simd_noeqcmp(*this, u); }

	INLINE_NO_DEBUG TVector4 operator - () const										{ return *(TVector4*)&simd_neg(*this); }
	INLINE_NO_DEBUG void operator += (const TVector4& u)								{ this->v = simd_add(*this, u).v; }
	INLINE_NO_DEBUG void operator -= (const TVector4& u)								{ this->v = simd_sub(*this, u).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator += (const t_OtherScalar& f)							{ this->v = simd_add(*this, TVector4(f, MakeFromScalarFlag())).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator -= (const t_OtherScalar& f)							{ this->v = simd_sub(*this, TVector4(f, MakeFromScalarFlag())).v; }
	INLINE_NO_DEBUG void operator *= (const TVector4& u)								{ this->v = simd_mul(*this, u).v; }
	INLINE_NO_DEBUG void operator /= (const TVector4& u)								{ this->v = simd_div(*this, u).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)							{ this->v = simd_mul(*this, Float(f)).v; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)							{ this->v = simd_div(*this, Float(f)).v; }

	friend INLINE_NO_DEBUG TVector4 operator + (const TVector4& a, const TVector4& b)	{ return *(TVector4*)&simd_add(a, b); }
	friend INLINE_NO_DEBUG TVector4 operator - (const TVector4& a, const TVector4& b)	{ return *(TVector4*)&simd_sub(a, b); }
	friend INLINE_NO_DEBUG TVector4 operator * (const TVector4& a, const TVector4& b)	{ return *(TVector4*)&simd_mul(a, b); }
	friend INLINE_NO_DEBUG TVector4 operator / (const TVector4& a, const TVector4& b)	{ return *(TVector4*)&simd_div(a, b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector4 operator + (const t_OtherScalar& a, const TVector4& b) { return *(TVector4*)&simd_add(TVector4(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector4 operator - (const t_OtherScalar& a, const TVector4& b) { return *(TVector4*)&simd_sub(TVector4(a, MakeFromScalarFlag()), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector4 operator * (const t_OtherScalar& a, const TVector4& b) { return *(TVector4*)&simd_mul(Float(a), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector4 operator / (const t_OtherScalar& a, const TVector4& b) { return *(TVector4*)&simd_div(Float(a), b); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector4 operator + (const TVector4& a, const t_OtherScalar& b) { return *(TVector4*)&simd_add(a, TVector4(b, MakeFromScalarFlag())); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector4 operator - (const TVector4& a, const t_OtherScalar& b) { return *(TVector4*)&simd_sub(a, TVector4(b, MakeFromScalarFlag())); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector4 operator * (const TVector4& a, const t_OtherScalar& b) { return *(TVector4*)&simd_mul(a, Float(b)); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TVector4 operator / (const TVector4& a, const t_OtherScalar& b) { return *(TVector4*)&simd_div(a, Float(b)); }

	INLINE_NO_DEBUG ScalarOptimal_t dot(const TVector4& u)const 						{ return *(ScalarOptimal_t*)&simd_dot(*this, u); }
	INLINE_NO_DEBUG ScalarOptimal_t lenghtSquare()const									{ return *(ScalarOptimal_t*)&simd_lenghtSquare(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t lenght()const										{ return *(ScalarOptimal_t*)&simd_lenght(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t distanceSquareTo(const TVector4& u)const			{ return (u - *this).lenghtSquare(); }
	INLINE_NO_DEBUG ScalarOptimal_t distanceTo(const TVector4& u)const					{ return (u - *this).lenght(); }
	INLINE_NO_DEBUG ScalarOptimal_t componentSumm()const								{ return *(ScalarOptimal_t*)&simd_summ(*this); }
	INLINE_NO_DEBUG ScalarOptimal_t componentAbsSumm()const								{ return *(ScalarOptimal_t*)&simd_summ(simd_abs(*this)); }
	INLINE_NO_DEBUG TVector4 normal()const												{ return *this * (*(ScalarOptimal_t*)&simd_invLenghtSquareRoot(*this)); }
	INLINE_NO_DEBUG TVector4 normalFast()const											{ return *this * (*(ScalarOptimal_t*)&simd_invLenghtSquareRootFast(*this)); }

	INLINE_NO_DEBUG const TVector2<Scalar_t> xy()const 									{ Base_t ret; ret.setXY00(*(const Base_t*)this); return *(TVector2<Scalar_t>*)&ret; }
	INLINE_NO_DEBUG const TVector2<Scalar_t> zw()const 									{ Base_t ret; ret.setXY00_FromZW(*(const Base_t*)this); return *(TVector2<Scalar_t>*)&ret; }
	INLINE_NO_DEBUG const TVector3<Scalar_t> xyz()const 								{ Base_t ret; ret.setXYZ0(*(const Base_t*)this); return *(TVector3<Scalar_t>*)&ret; }

	INLINE_NO_DEBUG boolean01 isAllPositive()const
	{ 
		uint32 flags = (~_mm_movemask_ps(v)) & 15;
		return (flags & (flags>>1) & (flags>>2) & (flags>>3)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAllNegative()const
	{ 
		uint32 flags = _mm_movemask_ps(v) & 15;
		return (flags & (flags>>1) & (flags>>2) & (flags>>3)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAnyPositive()const
	{
		uint32 flags = (~_mm_movemask_ps(v)) & 15;
		return (flags | (flags>>1) | (flags>>2) | (flags>>3)) & 1;
	}

	INLINE_NO_DEBUG boolean01 isAnyNegative()const
	{ 
		uint32 flags = _mm_movemask_ps(v) & 15;
		return (flags | (flags>>1) | (flags>>2) | (flags>>3)) & 1;
	}

	INLINE_NO_DEBUG TVector4()														{ setZero(); }
	INLINE_NO_DEBUG Vector2fRef<Scalar_t>& pxy() 									{ return *(Vector2fRef<Scalar_t>*)this; }
	INLINE_NO_DEBUG Vector2fZWRef<Scalar_t>& pzw() 									{ return *(Vector2fZWRef<Scalar_t>*)this; }	
	INLINE_NO_DEBUG Vector3fRef<Scalar_t>& pxyz() 									{ return *(Vector3fRef<Scalar_t>*)this; }	
	INLINE_NO_DEBUG TVector4 operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG TVector4 subdot(const TVector4& n)const 						{ return *this - n * n.dot(*this); }
	INLINE_NO_DEBUG TVector4 reflect(const TVector4& n)const 						{ ScalarOptimal_t f = n.dot(*this); return *this - n * (f + f); }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG void selfClamp(const TVector4& low, const TVector4& hi);
	INLINE_NO_DEBUG void selfClamp01();
	INLINE_NO_DEBUG void selfClamp11();

	INLINE_NO_DEBUG boolean trySelfNormalize(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsq(lsq);
			return true;
		}
		return false;
	}

	INLINE_NO_DEBUG boolean trySelfNormalizeFast(Scalar_t epsilon = 1e-32f)				
	{
		Scalar_t lsq = lenghtSquare();
		if(lsq > epsilon){
			*this *= rsqFast(lsq);
			return true;
		}
		return false;
	}
};

static INLINE_NO_DEBUG TVector4<float> abs(const TVector4<float>& vec)
{
	return *(const TVector4<float>*)&simd_abs(vec);
}

static INLINE_NO_DEBUG TVector4<float> sign(const TVector4<float>& vec)
{
	return *(const TVector4<float>*)&simd_sign(vec);
}

static INLINE_NO_DEBUG TVector4<float> min(const TVector4<float>& a, const TVector4<float>& b)
{
	return *(const TVector4<float>*)&simd_min(a, b);
}

static INLINE_NO_DEBUG TVector4<float> max(const TVector4<float>& a, const TVector4<float>& b)
{
	return *(const TVector4<float>*)&simd_max(a, b);
}

static INLINE_NO_DEBUG void minimize(TVector4<float>& a, const TVector4<float>& b)
{
	a = *(const TVector4<float>*)&simd_min(a, b);
}

static INLINE_NO_DEBUG void maximize(TVector4<float>& a, const TVector4<float>& b)
{
	a = *(const TVector4<float>*)&simd_max(a, b);
}

static INLINE_NO_DEBUG TVector4<float> clamp(const TVector4<float>& vec, const TVector4<float>& low, const TVector4<float>& hi)
{
	return *(const TVector4<float>*)&simd_clamp(vec, low, hi);
}

static INLINE_NO_DEBUG TVector4<float> clamp01(const TVector4<float>& vec)
{
	return *(const TVector4<float>*)&simd_clamp01(vec);
}

static INLINE_NO_DEBUG TVector4<float> clamp11(const TVector4<float>& vec)
{
	return *(const TVector4<float>*)&simd_clamp11(vec);
}

static INLINE_NO_DEBUG TVector4<float> rcp(const TVector4<float>& vec)
{
	return *(const TVector4<float>*)&simd_rcp(vec);
}

static INLINE_NO_DEBUG TVector4<float> rsq(const TVector4<float>& vec)
{
	return *(const TVector4<float>*)&simd_rsq(vec);
}

static INLINE_NO_DEBUG TVector4<float> sqrt(const TVector4<float>& vec)
{
	return *(const TVector4<float>*)&simd_sqrt(vec);
}

INLINE_NO_DEBUG void TVector4<float>::selfClamp(const TVector4<float>& low, const TVector4<float>& hi)		{ *this = clamp(*this, low, hi); }
INLINE_NO_DEBUG void TVector4<float>::selfClamp01()												{ *this = clamp01(*this); }
INLINE_NO_DEBUG void TVector4<float>::selfClamp11()												{ *this = clamp11(*this); }

INLINE_NO_DEBUG const TVector4<float> TVector3<float>::xyz0()const
{
	return *(TVector4<float>*)this;
}

INLINE_NO_DEBUG const TVector4<float> TVector3<float>::xyz1()const
{
	Base_t ret; ret.setXYZ1(*this); return *(TVector4<float>*)&ret;
}

#endif


template<class t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> decartFromCircle(const TVector2<t_Scalar>& rf)
{
	TVector2<t_Scalar> xy; 
	cosSin(xy.a[0], xy.a[1], rf.a[1]);
	xy.a[0] *= rf.a[0];
	xy.a[1] *= rf.a[0];
	return xy;
}

template<class t_Scalar>
static INLINE_NO_DEBUG TVector2<t_Scalar> circleFromDecart(const TVector2<t_Scalar>& xy)
{
	TVector2<t_Scalar> rf;
	rf.a[0] = xy.lenght();
	rf.a[1] = atan2(xy.a[1], xy.a[0]);
	return rf;
}

template<class t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> decartFromCylindric(const TVector3<t_Scalar>& rfz)
{
	TVector3<t_Scalar> dec = rfz; 
	cosSin(dec.a[0], dec.a[1], rfz.a[1]);
	dec.a[0] *= rfz.a[0];
	dec.a[1] *= rfz.a[0];
	return dec;
}

template<class t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> decartFromSpheric(const TVector3<t_Scalar>& rfq)
{
	TVector3<t_Scalar> xyz;
	t_Scalar cosFi,sinFi;
	t_Scalar cosQ,sinQ;
	cosSin(cosFi, sinFi, rfq.a[1]);
	cosSin(cosQ, sinQ, rfq.a[2]);
	xyz.a[0] = rfq.a[0] * sinQ * cosFi;
	xyz.a[1] = rfq.a[0] * sinQ * sinFi;
	xyz.a[2] = rfq.a[0] * cosQ;
	return xyz;
}

template<class t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> cylindricFromDecart(const TVector3<t_Scalar>& xyz)
{
	TVector3<t_Scalar> rfz = xyz;
	rfz.a[0] = xyz.xy().lenght();
	rfz.a[1] = atan2(xyz.a[1],xyz.a[0]);
	return rfz;
}

template<class t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> cylindricFromSpheric(const TVector3<t_Scalar>& rfq)
{
	TVector3<t_Scalar> rfz;
	t_Scalar cosQ,sinQ;
	cosSin(cosQ, sinQ, rfq[2]);
	rfz[0] = rfq[0] * sinQ;
	rfz[1] = rfq[1];
	rfz[2] = rfq[0] * cosQ;
	return rfz;
}

template<class t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> sphericFromDecart(const TVector3<t_Scalar>& xyz)
{
	TVector3<t_Scalar> rfq;
	rfq[0] = xyz.lenght();
	rfq[1] = atan2(xyz[1], xyz[0]);
	rfq[2] = rfq[0] > t_Scalar(0) ? acos(Base::clamp11(xyz[2] / rfq[0])) : t_Scalar(0);
	return rfq;
}

template<class t_Scalar>
static INLINE_NO_DEBUG TVector3<t_Scalar> sphericFromCylindric(const TVector3<t_Scalar>& rfz)
{
	TVector3<t_Scalar> rfq;
	rfq[0] = sqrt(rfz[0]*rfz[0] + rfz[2]*rfz[2]);
	rfq[1] = rfz[1];
	rfq[2] = rfq[0] > t_Scalar(0) ? acos(Base::clamp11(rfz[2] / rfq[0])) : t_Scalar(0);
	return rfq;
}


template<class t_Scalar>
INLINE_NO_DEBUG TPackedVector4<t_Scalar>::TPackedVector4(const TVector4<t_Scalar>& v): x(v.x), y(v.y), z(v.z), w(v.w)		{}
template<class t_Scalar>
INLINE_NO_DEBUG TPackedVector4<t_Scalar>& TPackedVector4<t_Scalar>::operator = (const TVector4<t_Scalar>& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

template<class t_Scalar>
INLINE_NO_DEBUG TPackedVector3<t_Scalar>::TPackedVector3(const TVector3<t_Scalar>& v): x(v.x), y(v.y), z(v.z)				{}
template<class t_Scalar>
INLINE_NO_DEBUG TPackedVector3<t_Scalar>& TPackedVector3<t_Scalar>::operator = (const TVector3<t_Scalar>& v) { x = v.x; y = v.y; z = v.z; return *this; }

template<class t_Scalar>
INLINE_NO_DEBUG TPackedVector2<t_Scalar>::TPackedVector2(const TVector2<t_Scalar>& v): x(v.x), y(v.y)						{}
template<class t_Scalar>
INLINE_NO_DEBUG TPackedVector2<t_Scalar>& TPackedVector2<t_Scalar>::operator = (const TVector2<t_Scalar>& v) { x = v.x; y = v.y; return *this; }


template<class t_Scalar>
struct Vector2T
{
	typedef TVector2< typename RealScalarT<t_Scalar>::Type > Type;
};

template<class t_Scalar>
struct Vector3T
{
	typedef TVector3< typename RealScalarT<t_Scalar>::Type > Type;
};

template<class t_Scalar>
struct Vector4T
{
	typedef TVector4< typename RealScalarT<t_Scalar>::Type > Type;
};

typedef TPackedVector4<float> PackedVector4f;
typedef TPackedVector3<float> PackedVector3f;
typedef TPackedVector2<float> PackedVector2f;

typedef TVector2<float> Vector2f;
typedef TVector3<float> Vector3f;
typedef TVector4<float> Vector4f;

typedef TVector2<int> Vector2i;
typedef TVector3<int> Vector3i;
typedef TVector4<int> Vector4i;

typedef TVector2<double> Vector2d;
typedef TVector3<double> Vector3d;
typedef TVector4<double> Vector4d;

static INLINE_NO_DEBUG Vector2i toVector2i(const Vector2f& src){ return Vector2i((int)src.x, (int)src.y); }
static INLINE_NO_DEBUG Vector2f toVector2f(const Vector2i& src){ return Vector2f((float)src.x, (float)src.y); }
static INLINE_NO_DEBUG Vector2f toVector2f(const Vector2d& src){ return Vector2f((float)src.x, (float)src.y); }
static INLINE_NO_DEBUG Vector2f toVector2f(const Vector2f& src){ return src; }
static INLINE_NO_DEBUG Vector2d toVector2d(const Vector2f& src){ return Vector2d((double)src.x, (double)src.y); }
static INLINE_NO_DEBUG Vector2d toVector2d(const Vector2i& src){ return Vector2d((double)src.x, (double)src.y); }
}
