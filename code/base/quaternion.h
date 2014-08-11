#pragma once

#include "vector.h"

namespace Base
{

template<typename t_Scalar> struct TQuaternion;

template<class t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TQuaternion<t_Scalar>& vec);

template<typename t_Scalar>
struct TQuaternion
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

	INLINE_NO_DEBUG TQuaternion(t_Scalar srcX, t_Scalar srcY, t_Scalar srcZ, t_Scalar srcW) : x(srcX), y(srcY), z(srcZ), w(srcW)	{ verify(); }
	INLINE_NO_DEBUG TQuaternion(const Scalar_t* arr) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3])									{verify();}
	INLINE_NO_DEBUG TQuaternion(const TQuaternion& u) : x(u.x), y(u.y), z(u.z), w(u.w)												{verify();}
	INLINE_NO_DEBUG TQuaternion(const TPackedVector4<Scalar_t>& u): x(u.x), y(u.y), z(u.z), w(u.w)									{verify();}
	INLINE_NO_DEBUG TQuaternion(const TVector3<Scalar_t>& u, Scalar_t srcW) : x(u.x), y(u.y), z(u.z), w(srcW)						{verify();}
	INLINE_NO_DEBUG TQuaternion(const TVector2<Scalar_t>& xy, const TVector2<Scalar_t>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y)	{verify();}

	INLINE_NO_DEBUG void set(Scalar_t srcX, Scalar_t srcY, Scalar_t srcZ, Scalar_t srcW) 		{ x = srcX; y = srcY; z = srcZ; w = srcW; verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr) 												{ x = arr[0]; y = arr[1]; z = arr[2]; w = arr[3]; verify(); };
	INLINE_NO_DEBUG void set(const TPackedVector4<Scalar_t>& u) 								{ x = u.x; y = u.y; z = u.z; w = u.w; verify(); };
	INLINE_NO_DEBUG void set(const TQuaternion& u) 												{ x = u.x; y = u.y; z = u.z; w = u.w; verify(); };
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& u, Scalar_t W) 							{ x = u.x; y = u.y; z = u.z; w = W; verify(); };
	INLINE_NO_DEBUG void set(const TVector2<Scalar_t>& xy, const TVector2<Scalar_t>& zw) 		{ x = xy.x; y = xy.y; z = zw.x; w = zw.y; verify(); };
	INLINE_NO_DEBUG TQuaternion& operator = (const TQuaternion& u)								{ x = u.x; y = u.y; z = u.z; w = u.w; verify(); return *this; }
	INLINE_NO_DEBUG TQuaternion& operator = (const TPackedVector4<Scalar_t>& u) 				{ x = u.x; y = u.y; z = u.z; w = u.w; verify(); return *this; }
	INLINE_NO_DEBUG boolean operator == (const TQuaternion& u)const								{ return x == u.x && y == u.y && z == u.z && w == u.w; }
	INLINE_NO_DEBUG boolean operator != (const TQuaternion& u)const								{ return x != u.x || y != u.y || z != u.z || w != u.w; }

	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const TQuaternion& b)
	{
		TQuaternion q;
		q.a[0] = a[3]*b[0] + a[0]*b[3] + a[1]*b[2] - a[2]*b[1];
		q.a[1] = a[3]*b[1] + a[1]*b[3] + a[2]*b[0] - a[0]*b[2];
		q.a[2] = a[3]*b[2] + a[2]*b[3] + a[0]*b[1] - a[1]*b[0];
		q.a[3] = a[3]*b[3] - a[0]*b[0] - a[1]*b[1] - a[2]*b[2];
		return q;
	}

	INLINE_NO_DEBUG TQuaternion operator - () const											{ return TQuaternion(-x, -y, -z, -w); }
	INLINE_NO_DEBUG void operator += (const TQuaternion& u)									{ x += u.x; y += u.y; z += u.z; w += u.w; }
	INLINE_NO_DEBUG void operator -= (const TQuaternion& u)									{ x -= u.x; y -= u.y; z -= u.z; w -= u.w; }
	INLINE_NO_DEBUG void operator *= (const TQuaternion& u)									{ *this = *this * u; }
	INLINE_NO_DEBUG void operator *= (const Scalar_t f)										{ x *= f; y *= f; z *= f; w *= f; }
	INLINE_NO_DEBUG void operator /= (const Scalar_t f)										{ x /= f; y /= f; z /= f; w /= f; }

	friend INLINE_NO_DEBUG TQuaternion operator + (const TQuaternion& a, const TQuaternion& b){ return TQuaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
	friend INLINE_NO_DEBUG TQuaternion operator - (const TQuaternion& a, const TQuaternion& b){ return TQuaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const Scalar_t a, const TQuaternion& b) 	{ return TQuaternion(a * b.x, a * b.y, a * b.z, a * b.w); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const Scalar_t b) 	{ return TQuaternion(a.x * b, a.y * b, a.z * b, a.w * b); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const Scalar_t b) 	{ return TQuaternion(a.x / b, a.y / b, a.z / b, a.w / b); }
	
	INLINE_NO_DEBUG TQuaternion conjurate()const											{ return TQuaternion(-(Scalar_t)this->x, -(Scalar_t)this->y, -(Scalar_t)this->z, (Scalar_t)this->w); }
	INLINE_NO_DEBUG Scalar_t dot(const TQuaternion& u)const 								{ return x * u.x + y * u.y + z * u.z + w * u.w; }
	INLINE_NO_DEBUG Scalar_t lenghtSquare()const											{ return x * x + y * y + z * z + w * w; }
	INLINE_NO_DEBUG Scalar_t lenght()const													{ return Base::sqrt(lenghtSquare()); }
	INLINE_NO_DEBUG TQuaternion normal()const												{ return *this * rsq(lenghtSquare()); }
	INLINE_NO_DEBUG TQuaternion normalFast()const											{ return *this * rsqFast(lenghtSquare()); }

	INLINE_NO_DEBUG TVector3<Scalar_t> rotateVector(const TVector3<Scalar_t>& vec)const
	{
		Scalar_t tmp[4];
		tmp[0] = a[3]*vec.a[0] + a[1]*vec.a[2] - a[2]*vec.a[1];
		tmp[1] = a[3]*vec.a[1] + a[2]*vec.a[0] - a[0]*vec.a[2];
		tmp[2] = a[3]*vec.a[2] + a[0]*vec.a[1] - a[1]*vec.a[0];
		tmp[3] =-a[0]*vec.a[0] - a[1]*vec.a[1] - a[2]*vec.a[2];

		TVector3<Scalar_t> ret;
		ret.a[0] = -tmp[3]*a[0] + tmp[0]*a[3] - tmp[1]*a[2] + tmp[2]*a[1];
		ret.a[1] = -tmp[3]*a[1] + tmp[1]*a[3] - tmp[2]*a[0] + tmp[0]*a[2];
		ret.a[2] = -tmp[3]*a[2] + tmp[2]*a[3] - tmp[0]*a[1] + tmp[1]*a[0];
		return ret;
	}

	INLINE_NO_DEBUG TVector3<Scalar_t> invRotateVector(const TVector3<Scalar_t>& vec)const
	{
		Scalar_t tmp[4];
		tmp[0] = a[3]*vec.a[0] - a[1]*vec.a[2] + a[2]*vec.a[1];
		tmp[1] = a[3]*vec.a[1] - a[2]*vec.a[0] + a[0]*vec.a[2];
		tmp[2] = a[3]*vec.a[2] - a[0]*vec.a[1] + a[1]*vec.a[0];
		tmp[3] = a[0]*vec.a[0] + a[1]*vec.a[1] + a[2]*vec.a[2];

		TVector3<Scalar_t> ret;
		ret.a[0] = tmp[3]*a[0] + tmp[0]*a[3] + tmp[1]*a[2] - tmp[2]*a[1];
		ret.a[1] = tmp[3]*a[1] + tmp[1]*a[3] + tmp[2]*a[0] - tmp[0]*a[2];
		ret.a[2] = tmp[3]*a[2] + tmp[2]*a[3] + tmp[0]*a[1] - tmp[1]*a[0];
		return ret;
	}

	static INLINE_NO_DEBUG  TQuaternion getIdentity() 								{ return TQuaternion(Scalar_t(0),Scalar_t(0),Scalar_t(0),Scalar_t(1)); }
	INLINE_NO_DEBUG void setIdentity() 												{ set(Scalar_t(0),Scalar_t(0),Scalar_t(0),Scalar_t(1)); }
	INLINE_NO_DEBUG TQuaternion()													{ setIdentity(); }	
	INLINE_NO_DEBUG TQuaternion operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG TVector4<Scalar_t>& v4()										{ return *(TVector4<Scalar_t>*)this; }
	INLINE_NO_DEBUG const TVector4<Scalar_t>& v4()const								{ return *(const TVector4<Scalar_t>*)this; }

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


#ifdef INSTRUCTIONS_SIMD
template<>
struct TQuaternion<float> : public FloatVector
{
	typedef FloatVector Base_t;
	typedef float Scalar_t;
	typedef Float ScalarOptimal_t;

	typedef Float BigScalar_t;
	typedef FloatX BigScalarX_t;
	typedef FloatY BigScalarY_t;
	typedef FloatZ BigScalarZ_t;
	typedef FloatW BigScalarW_t;
#ifdef INSTRUCTIONS_NEON
	typedef FloatShort BigScalarShort_t;
	typedef FloatShortX BigScalarShortX_t;
	typedef FloatShortY BigScalarShortY_t;
#endif

#ifdef DEBUG
	INLINE_NO_DEBUG void verify()const { Assert(isFinite(*this)); }
#else
	INLINE_NO_DEBUG void verify()const {}
#endif

	template<typename t_ValX, typename t_ValY, typename t_ValZ, typename t_ValW>
	INLINE_NO_DEBUG TQuaternion(const t_ValX& srcX, const t_ValY& srcY, const t_ValZ& srcZ, const t_ValZ& srcW) { Base_t::set(srcX, srcY, srcZ, srcW); verify(); }
	INLINE_NO_DEBUG TQuaternion(Scalar_t srcX, Scalar_t srcY, Scalar_t srcZ, Scalar_t srcW)				{ Base_t::set(srcX, srcY, srcZ, srcW); verify(); }
	INLINE_NO_DEBUG TQuaternion(const Scalar_t* arr)													{ Base_t::set(arr); verify(); }
	INLINE_NO_DEBUG TQuaternion(const TQuaternion& v) : Base_t(*(const Base_t*)&v)						{ verify(); }
	INLINE_NO_DEBUG TQuaternion(const TPackedVector4<Scalar_t>& v)										{ Base_t::set(v.a); verify(); }
	INLINE_NO_DEBUG TQuaternion(const TVector3<Scalar_t>& xyz, const BigScalar_t& z)					{ Base_t::setXYZ_W(*(const Base_t*)&xyz, z); verify(); }
	INLINE_NO_DEBUG TQuaternion(const TVector3<Scalar_t>& xyz, const BigScalarX_t& z)					{ Base_t::setXYZ_W(*(const Base_t*)&xyz, z); verify(); }
	INLINE_NO_DEBUG TQuaternion(const TVector3<Scalar_t>& xyz, const BigScalarY_t& z)					{ Base_t::setXYZ_W(*(const Base_t*)&xyz, z); verify(); }
	INLINE_NO_DEBUG TQuaternion(const TVector3<Scalar_t>& xyz, const BigScalarZ_t& z)					{ Base_t::setXYZ_W(*(const Base_t*)&xyz, z); verify(); }
	INLINE_NO_DEBUG TQuaternion(const TVector3<Scalar_t>& xyz, const BigScalarW_t& z)					{ Base_t::setXYZ_W(*(const Base_t*)&xyz, z); verify(); }
	INLINE_NO_DEBUG TQuaternion(const TVector3<Scalar_t>& xyz, Scalar_t z)								{ Base_t::setXYZ_W(*(const Base_t*)&xyz, z); verify(); }
	INLINE_NO_DEBUG TQuaternion(const TVector2<Scalar_t>& xy, const TVector2<Scalar_t>& zw)				{ Base_t::setXY_ZW(*(const Base_t*)&xy, *(const Base_t*)&zw); verify(); }

	template<typename t_ValX, typename t_ValY, typename t_ValZ, typename t_ValW>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, const t_ValY& srcY, const t_ValZ& srcZ, const t_ValW& srcW) { Base_t::set(srcX, srcY, srcZ, srcW); verify(); }
	template<typename t_ValY, typename t_ValZ, typename t_ValW>
	INLINE_NO_DEBUG void set(Scalar_t srcX, const t_ValY& srcY, const t_ValZ& srcZ, const t_ValW& srcW) { Base_t::set(srcX, (Scalar_t)srcY, (Scalar_t)srcZ, (Scalar_t)srcW); verify(); }
	template<typename t_ValX, typename t_ValZ, typename t_ValW>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, Scalar_t srcY, const t_ValZ& srcZ, const t_ValW& srcW) { Base_t::set((Scalar_t)srcX, srcY, (Scalar_t)srcZ, (Scalar_t)srcW); verify(); }
	template<typename t_ValX, typename t_ValY, typename t_ValW>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, const t_ValY& srcY, Scalar_t srcZ, const t_ValW& srcW) { Base_t::set((Scalar_t)srcX, (Scalar_t)srcY, srcZ, (Scalar_t)srcW); verify(); }
	template<typename t_ValX, typename t_ValY, typename t_ValZ>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, const t_ValY& srcY, const t_ValZ& srcZ, Scalar_t srcW) { Base_t::set((Scalar_t)srcX, (Scalar_t)srcY, (Scalar_t)srcZ, srcW); verify(); }
	template<typename t_ValZ, typename t_ValW>
	INLINE_NO_DEBUG void set(Scalar_t srcX, Scalar_t srcY, const t_ValZ& srcZ, const t_ValW& srcW) { Base_t::set(srcX, srcY, (Scalar_t)srcZ, (Scalar_t)srcW); verify(); }
	template<typename t_ValX, typename t_ValW>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, Scalar_t srcY, Scalar_t srcZ, const t_ValW& srcW) { Base_t::set((Scalar_t)srcX, srcY, srcZ, (Scalar_t)srcW); verify(); }
	template<typename t_ValX, typename t_ValY>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, const t_ValY& srcY, Scalar_t srcZ, Scalar_t srcW) { Base_t::set((Scalar_t)srcX, (Scalar_t)srcY, srcZ, srcW); verify(); }
	template<typename t_ValY, typename t_ValZ>
	INLINE_NO_DEBUG void set(Scalar_t srcX, const t_ValY& srcY, const t_ValZ& srcZ, Scalar_t srcW) { Base_t::set(srcX, (Scalar_t)srcY, (Scalar_t)srcZ, srcW); verify(); }
	template<typename t_ValX, typename t_ValZ>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, Scalar_t srcY, const t_ValZ& srcZ, Scalar_t srcW) { Base_t::set((Scalar_t)srcX, srcY, (Scalar_t)srcZ, srcW); verify(); }
	template<typename t_ValY, typename t_ValW>
	INLINE_NO_DEBUG void set(Scalar_t srcX, const t_ValY& srcY, Scalar_t srcZ, const t_ValW& srcW) { Base_t::set(srcX, (Scalar_t)srcY, srcZ, (Scalar_t)srcW); verify(); }
	template<typename t_ValX>
	INLINE_NO_DEBUG void set(const t_ValX& srcX, Scalar_t srcY, Scalar_t srcZ, Scalar_t srcW)	{ Base_t::set((Scalar_t)srcX, srcY, srcZ, srcW); verify(); }
	template<typename t_ValY>
	INLINE_NO_DEBUG void set(Scalar_t srcX, const t_ValY& srcY, Scalar_t srcZ, Scalar_t srcW)	{ Base_t::set(srcX, (Scalar_t)srcY, srcZ, srcW); verify(); }
	template<typename t_ValZ>
	INLINE_NO_DEBUG void set(Scalar_t srcX, Scalar_t srcY, const t_ValZ& srcZ, Scalar_t srcW)	{ Base_t::set(srcX, srcY, (Scalar_t)srcZ, srcW); verify(); }
	template<typename t_ValW>
	INLINE_NO_DEBUG void set(Scalar_t srcX, Scalar_t srcY, Scalar_t srcZ, const t_ValW& srcW)	{ Base_t::set(srcX, srcY, srcZ, (Scalar_t)srcW); verify(); }

	INLINE_NO_DEBUG void set(Scalar_t srcX, Scalar_t srcY, Scalar_t srcZ, Scalar_t srcW)		{ Base_t::set(srcX, srcY, srcZ, srcW); verify(); }
	INLINE_NO_DEBUG void set(const Scalar_t* arr)												{ Base_t::set(arr); verify(); }
	INLINE_NO_DEBUG void set(const TQuaternion& u)												{ this->v = u.v; verify(); }
	INLINE_NO_DEBUG void set(const TPackedVector4<Scalar_t>& u)									{ Base_t::set(u.a); verify(); }
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& xyz, const BigScalar_t& W)				{ Base_t::setXYZ_W(*(const Base_t*)&xyz, W); verify(); }
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& xyz, const BigScalarX_t& W)				{ Base_t::setXYZ_W(*(const Base_t*)&xyz, W); verify(); }
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& xyz, const BigScalarY_t& W)				{ Base_t::setXYZ_W(*(const Base_t*)&xyz, W); verify(); }
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& xyz, const BigScalarZ_t& W)				{ Base_t::setXYZ_W(*(const Base_t*)&xyz, W); verify(); }
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& xyz, const BigScalarW_t& W)				{ Base_t::setXYZ_W(*(const Base_t*)&xyz, W); verify(); }
	INLINE_NO_DEBUG void set(const TVector3<Scalar_t>& xyz, Scalar_t W)							{ Base_t::setXYZ_W(*(const Base_t*)&xyz, W); verify(); }
	INLINE_NO_DEBUG void set(const TVector2<Scalar_t>& xy, const TVector2<Scalar_t>& zw)		{ Base_t::setXY_ZW(*(const Base_t*)&xy, *(const Base_t*)&zw); verify(); }
	INLINE_NO_DEBUG TQuaternion& operator = (const TQuaternion& u)								{ this->v = u.v; verify(); return *this; };
	INLINE_NO_DEBUG TQuaternion& operator = (const TPackedVector4<Scalar_t>& u) 				{ Base_t::set(u.a); verify(); return *this; };
	INLINE_NO_DEBUG boolean operator == (const TQuaternion& u)const								{ return !simd_noeqcmp(*this, u); }
	INLINE_NO_DEBUG boolean operator != (const TQuaternion& u)const								{ return simd_noeqcmp(*this, u); }

	INLINE_NO_DEBUG TQuaternion operator - () const												{ return *(TQuaternion*)&simd_neg(*this); }
	INLINE_NO_DEBUG void operator += (const TQuaternion& u)										{ this->v = simd_add(*this, u).v; }
	INLINE_NO_DEBUG void operator -= (const TQuaternion& u)										{ this->v = simd_sub(*this, u).v; }
	INLINE_NO_DEBUG void operator *= (const TQuaternion& u)										{ this->v = simd_quat_mult(*this, u).v; }
	INLINE_NO_DEBUG void operator *= (const BigScalar_t& f)										{ this->v = simd_mul(*this, f).v; }
	INLINE_NO_DEBUG void operator /= (const BigScalar_t& f)										{ this->v = simd_div(*this, f).v; }

	INLINE_NO_DEBUG void operator *= (const BigScalarX_t& f)									{ this->v = simd_mul(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator /= (const BigScalarX_t& f)									{ this->v = simd_div(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator *= (const BigScalarY_t& f)									{ this->v = simd_mul(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator /= (const BigScalarY_t& f)									{ this->v = simd_div(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator *= (const BigScalarZ_t& f)									{ this->v = simd_mul(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator /= (const BigScalarZ_t& f)									{ this->v = simd_div(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator *= (const BigScalarW_t& f)									{ this->v = simd_mul(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator /= (const BigScalarW_t& f)									{ this->v = simd_div(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator *= (Scalar_t f)												{ this->v = simd_mul(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator /= (Scalar_t f)												{ this->v = simd_div(*this, BigScalar_t(f)).v; }

#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG void operator *= (const BigScalarShort_t& f)								{ this->v = simd_mul(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator /= (const BigScalarShort_t& f)								{ this->v = simd_div(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator *= (const BigScalarShortX_t& f)								{ this->v = simd_mul(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator /= (const BigScalarShortX_t& f)								{ this->v = simd_div(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator *= (const BigScalarShortY_t& f)								{ this->v = simd_mul(*this, BigScalar_t(f)).v; }
	INLINE_NO_DEBUG void operator /= (const BigScalarShortY_t& f)								{ this->v = simd_div(*this, BigScalar_t(f)).v; }
#endif

	friend INLINE_NO_DEBUG TQuaternion operator + (const TQuaternion& a, const TQuaternion& b)	{ return *(TQuaternion*)&simd_add(a, b); }
	friend INLINE_NO_DEBUG TQuaternion operator - (const TQuaternion& a, const TQuaternion& b)	{ return *(TQuaternion*)&simd_sub(a, b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const TQuaternion& b)	{ return *(TQuaternion*)&simd_quat_mult(a, b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const BigScalar_t& a, const TQuaternion& b) 	{ return *(TQuaternion*)&simd_mul(a, b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const BigScalar_t& b) 	{ return *(TQuaternion*)&simd_mul(a, b); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const BigScalar_t& b) 	{ return *(TQuaternion*)&simd_div(a, b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const BigScalarX_t& a, const TQuaternion& b) { return *(TQuaternion*)&simd_mul(BigScalar_t(a), b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const BigScalarX_t& b) { return *(TQuaternion*)&simd_mul(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const BigScalarX_t& b) { return *(TQuaternion*)&simd_div(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const BigScalarY_t& a, const TQuaternion& b) { return *(TQuaternion*)&simd_mul(BigScalar_t(a), b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const BigScalarY_t& b) { return *(TQuaternion*)&simd_mul(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const BigScalarY_t& b) { return *(TQuaternion*)&simd_div(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const BigScalarZ_t& a, const TQuaternion& b) { return *(TQuaternion*)&simd_mul(BigScalar_t(a), b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const BigScalarZ_t& b) { return *(TQuaternion*)&simd_mul(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const BigScalarZ_t& b) { return *(TQuaternion*)&simd_div(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const BigScalarW_t& a, const TQuaternion& b) { return *(TQuaternion*)&simd_mul(BigScalar_t(a), b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const BigScalarW_t& b) { return *(TQuaternion*)&simd_mul(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const BigScalarW_t& b) { return *(TQuaternion*)&simd_div(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator * (Scalar_t a, const TQuaternion& b)			{ return *(TQuaternion*)&simd_mul(BigScalar_t(a), b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, Scalar_t b) 			{ return *(TQuaternion*)&simd_mul(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, Scalar_t b) 			{ return *(TQuaternion*)&simd_div(a, BigScalar_t(b)); }

#ifdef INSTRUCTIONS_NEON
	friend INLINE_NO_DEBUG TQuaternion operator * (const BigScalarShort_t& a, const TQuaternion& b) { return *(TQuaternion*)&simd_mul(BigScalar_t(a), b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const BigScalarShort_t& b) { return *(TQuaternion*)&simd_mul(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const BigScalarShort_t& b) { return *(TQuaternion*)&simd_div(a, BigScalar_t(b)); }

	friend INLINE_NO_DEBUG TQuaternion operator * (const BigScalarShortX_t& a, const TQuaternion& b) { return *(TQuaternion*)&simd_mul(BigScalar_t(a), b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const BigScalarShortX_t& b) { return *(TQuaternion*)&simd_mul(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const BigScalarShortX_t& b) { return *(TQuaternion*)&simd_div(a, BigScalar_t(b)); }

	friend INLINE_NO_DEBUG TQuaternion operator * (const BigScalarShortY_t& a, const TQuaternion& b) { return *(TQuaternion*)&simd_mul(BigScalar_t(a), b); }
	friend INLINE_NO_DEBUG TQuaternion operator * (const TQuaternion& a, const BigScalarShortY_t& b) { return *(TQuaternion*)&simd_mul(a, BigScalar_t(b)); }
	friend INLINE_NO_DEBUG TQuaternion operator / (const TQuaternion& a, const BigScalarShortY_t& b) { return *(TQuaternion*)&simd_div(a, BigScalar_t(b)); }
#endif

	INLINE_NO_DEBUG TQuaternion conjurate()const												{ return *(TQuaternion*)&simd_quat_conj(*this); }
	INLINE_NO_DEBUG BigScalar_t dot(const TQuaternion& u)const 									{ return *(BigScalar_t*)&simd_dot(*this, u); }
	INLINE_NO_DEBUG BigScalar_t lenghtSquare()const												{ return *(BigScalar_t*)&simd_lenghtSquare(*this); }
	INLINE_NO_DEBUG BigScalar_t lenght()const													{ return *(BigScalar_t*)&simd_lenght(*this); }
	INLINE_NO_DEBUG TQuaternion normal()const													{ return *this * (*(BigScalar_t*)&simd_invLenghtSquareRoot(*this)); }
	INLINE_NO_DEBUG TQuaternion normalFast()const												{ return *this * (*(BigScalar_t*)&simd_invLenghtSquareRootFast(*this)); }

	INLINE_NO_DEBUG TVector3<Scalar_t> rotateVector(const TVector3<Scalar_t>& vec)const			{ return *(TVector3<Scalar_t>*)&simd_quat_rotate_vec(*this, vec); }
	INLINE_NO_DEBUG TVector3<Scalar_t> invRotateVector(const TVector3<Scalar_t>& vec)const		{ return *(TVector3<Scalar_t>*)&simd_quat_inv_rotate_vec(*this, vec); }

	static INLINE_NO_DEBUG  TQuaternion getIdentity() 								{ return TQuaternion(Scalar_t(0),Scalar_t(0),Scalar_t(0),Scalar_t(1)); }
	INLINE_NO_DEBUG void setIdentity() 												{ set(Scalar_t(0),Scalar_t(0),Scalar_t(0),Scalar_t(1)); }
	INLINE_NO_DEBUG TQuaternion()													{ setIdentity(); }	
	INLINE_NO_DEBUG TQuaternion operator + () const									{ return *this; }
	INLINE_NO_DEBUG Scalar_t& operator [] (int index)								{ return this->a[index]; }
	INLINE_NO_DEBUG const Scalar_t& operator [] (int index) const					{ return this->a[index]; }
	INLINE_NO_DEBUG void selfNormalize()											{ *this = normal(); }
	INLINE_NO_DEBUG void selfNormalizeFast()										{ *this = normalFast(); }
	INLINE_NO_DEBUG TVector4<Scalar_t>& v4()										{ return *(TVector4<Scalar_t>*)this; }
	INLINE_NO_DEBUG const TVector4<Scalar_t>& v4()const								{ return *(const TVector4<Scalar_t>*)this; }

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

#endif




template<typename t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TQuaternion<t_Scalar>& a, const TQuaternion<t_Scalar>& b, t_Scalar eps)
{
	return Base::equals(a.a[0], b.a[0], eps) && Base::equals(a.a[1], b.a[1], eps) && Base::equals(a.a[2], b.a[2], eps) && Base::equals(a.a[3], b.a[3], eps);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TQuaternion<t_Scalar>& vec)
{
	return isFinite(vec.a[0]) && isFinite(vec.a[1]) && isFinite(vec.a[2]) && isFinite(vec.a[3]);
}

template<typename t_Scalar>
static INLINE TQuaternion<t_Scalar> slerp(const TQuaternion<t_Scalar>& A, const TQuaternion<t_Scalar>& B, const t_Scalar t)
{
	t_Scalar magnitude = sqrt(A.lenghtSquare() * B.lenghtSquare()); 
	Assert(magnitude > t_Scalar(0));

	t_Scalar product = t_Scalar(A.dot(B)) / magnitude;
	if (abs(product) < t_Scalar(1))
	{
		// Take care of long angle case see http://en.wikipedia.org/wiki/Slerp
		t_Scalar sign = sign(product);
		t_Scalar theta = acos(sign * product);
		t_Scalar s1 = sin(sign * t * theta);   
		t_Scalar d = t_Scalar(1.0) / sin(theta);
		t_Scalar s0 = sin((t_Scalar(1.0) - t) * theta);
		s0 *= d;
		s1 *= d;
		return A * s0 + B * s1;
	}
	else
	{
		return A;
	}
}

template<typename t_Scalar>
static INLINE TQuaternion<t_Scalar> quaternionFromAxisAngle(const TVector4<t_Scalar>& axisangle)
{
	TQuaternion<t_Scalar> q;
	t_Scalar c, s;
	cosSin(c, s, axisangle.a[3] * t_Scalar(0.5));
	q.v4() = axisangle * s;
	q.w = c;
	return q;
}

template<typename t_Scalar>
static INLINE TVector4<t_Scalar> axisAngleFromQuaternion(const TQuaternion<t_Scalar>& quat)
{
	t_Scalar f = t_Scalar(1) - quat.w*quat.w;
	if (f > gcEpsilon*gcEpsilon)
	{
		TVector4<t_Scalar> axisAngle;
		axisAngle = quat.v4() * rsq(f);
		axisAngle.w = t_Scalar(2) * acos(quat.a[3]);
		return axisAngle;	
	}
	else
		return TVector4<t_Scalar>(t_Scalar(0), t_Scalar(0), t_Scalar(1), t_Scalar(0));
}

template<class t_Scalar>
struct QuaternionT
{
	typedef TQuaternion< typename RealScalarT<t_Scalar>::Type > Type;
};

typedef TQuaternion<float> Quaternionf;
typedef TQuaternion<double> Quaterniond;

}
