#pragma once

#include "vector.h"
#include "quaternion.h"

namespace Base
{

template<class Scalar> INLINE_NO_DEBUG Scalar det2x2(
	const Scalar x0, const Scalar y0,
	const Scalar x1, const Scalar y1)
{
	return x0*y1 - y0*x1;
}
template<class Scalar> INLINE_NO_DEBUG Scalar det3x3(
	const Scalar x0, const Scalar y0, const Scalar z0,
	const Scalar x1, const Scalar y1, const Scalar z1,
	const Scalar x2, const Scalar y2, const Scalar z2)
{
	return x0*(y1*z2-z1*y2) - y0*(x1*z2-z1*x2) + z0*(x1*y2-y1*x2);
}
template<class Scalar> INLINE_NO_DEBUG Scalar det4x4(
	const Scalar x0, const Scalar y0, const Scalar z0, const Scalar w0,
	const Scalar x1, const Scalar y1, const Scalar z1, const Scalar w1,
	const Scalar x2, const Scalar y2, const Scalar z2, const Scalar w2,
	const Scalar x3, const Scalar y3, const Scalar z3, const Scalar w3)
{
	return x0*det3x3(y1,z1,w1,
		y2,z2,w2,
		y3,z3,w3)
		-  y0*det3x3(x1,z1,w1,
		x2,z2,w2,
		x3,z3,w3)
		+  z0*det3x3(x1,y1,w1,
		x2,y2,w2,
		x3,y3,w3)
		-  w0*det3x3(x1,y1,z1,
		x2,y2,z2,
		x3,y3,z3);
}

template<class t_Scalar>
static INLINE_NO_DEBUG void transpose_impl(TVector4<t_Scalar>& dstX, TVector4<t_Scalar>& dstY, TVector4<t_Scalar>& dstZ, TVector4<t_Scalar>& dstW,
										   const TVector4<t_Scalar>& x, const TVector4<t_Scalar>& y, const TVector4<t_Scalar>& z, const TVector4<t_Scalar>& w)
{
	dstX.set(x.a[0], y.a[0], z.a[0], w.a[0]);
	dstY.set(x.a[1], y.a[1], z.a[1], w.a[1]);
	dstZ.set(x.a[2], y.a[2], z.a[2], w.a[2]);
	dstW.set(x.a[2], y.a[2], z.a[2], w.a[3]);
}

template<class t_Scalar>
static INLINE_NO_DEBUG void transpose_impl(TVector3<t_Scalar>& dstX, TVector3<t_Scalar>& dstY, TVector3<t_Scalar>& dstZ,
										   const TVector3<t_Scalar>& x, const TVector3<t_Scalar>& y, const TVector3<t_Scalar>& z)
{
	dstX.set(x.a[0], y.a[0], z.a[0]);
	dstY.set(x.a[1], y.a[1], z.a[1]);
	dstZ.set(x.a[2], y.a[2], z.a[2]);
}

template<class t_Scalar>
static INLINE_NO_DEBUG void transpose_impl(TVector2<t_Scalar>& dstX, TVector2<t_Scalar>& dstY,
										   const TVector2<t_Scalar>& x, const TVector2<t_Scalar>& y)
{
	dstX.set(x.a[0], y.a[0]);
	dstY.set(x.a[1], y.a[1]);
}

#ifdef INSTRUCTIONS_SIMD

static INLINE_NO_DEBUG void transpose_impl(TVector4<float>& dstX, TVector4<float>& dstY, TVector4<float>& dstZ, TVector4<float>& dstW,
										   const TVector4<float>& x, const TVector4<float>& y, const TVector4<float>& z, const TVector4<float>& w)
{
	simd_m44_transpose(dstX, dstY, dstZ, dstW, x, y, z, w);
}

static INLINE_NO_DEBUG void transpose_impl(TVector3<float>& dstX, TVector3<float>& dstY, TVector3<float>& dstZ,
										   const TVector3<float>& x, const TVector3<float>& y, const TVector3<float>& z)
{
	simd_m33_transpose(dstX, dstY, dstZ, x, y, z);
}

static INLINE_NO_DEBUG void transpose_impl(TVector2<float>& dstX, TVector2<float>& dstY,
										   const TVector2<float>& x, const TVector2<float>& y)
{
	simd_m22_transpose(dstX, dstY, x, y);
}

#endif


template<typename t_Scalar>
struct TMatrix22
{
	typedef t_Scalar Scalar_t;
	typedef TVector2<t_Scalar> Vector_t;
	typedef typename Vector_t::ScalarOptimal_t ScalarOptimal_t;

	Vector_t x, y;

	INLINE_NO_DEBUG TMatrix22() : x(Scalar_t(1), Scalar_t(0)), y(Scalar_t(0), Scalar_t(1))	{}
	INLINE_NO_DEBUG TMatrix22(const TMatrix22& m) : x(m.x), y(m.y)							{}
	INLINE_NO_DEBUG TMatrix22(const Vector_t& X, const Vector_t& Y) : x(X), y(Y)			{}
	INLINE_NO_DEBUG void set(const TMatrix22& m) 											{ x = m.x; y = m.y; };
	INLINE_NO_DEBUG void set(const Vector_t& X, const Vector_t& Y)							{ x = X; y = Y; }
	INLINE_NO_DEBUG void setIdentity()														{ x.set(Scalar_t(1), Scalar_t(0)); y.set(Scalar_t(0), Scalar_t(1)); }
	INLINE_NO_DEBUG static TMatrix22 getIdentity() 											{ return TMatrix22(Vector_t(Scalar_t(1), Scalar_t(0)),Vector_t(Scalar_t(0), Scalar_t(1))); }
	INLINE_NO_DEBUG TMatrix22& operator = (const TMatrix22& m)								{ x = m.x; y = m.y; return *this; }
	INLINE_NO_DEBUG boolean operator == (const TMatrix22& m) const							{ return this->x == m.x && this->y == m.y; }
	INLINE_NO_DEBUG boolean operator != (const TMatrix22& m) const							{ return this->x != m.x || this->y != m.y; }

	INLINE_NO_DEBUG Scalar_t& operator () (int nRow, int nColumn)							{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Scalar_t  operator () (int nRow, int nColumn) const						{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Vector_t& operator [] (int index)										{ return (&x)[index]; }
	INLINE_NO_DEBUG const Vector_t& operator [] (int index) const							{ return (&x)[index]; }	

	friend INLINE_NO_DEBUG Vector_t operator * (const TMatrix22& m, const Vector_t& v)
	{
		return m.x * v.x + m.y * v.y;
	}

	INLINE_NO_DEBUG Vector_t tranMult(const Vector_t& v)const
	{
		return Vector_t(x.dot(v), y.dot(v));
	}

	friend INLINE_NO_DEBUG TMatrix22 operator * (const TMatrix22& a, const TMatrix22& b)
	{
		return TMatrix22(a * b.x, a * b.y);
	}

	friend INLINE_NO_DEBUG TMatrix22 operator + (const TMatrix22& a, const TMatrix22& b)	{ return TMatrix22(a.x + b.x, a.y + b.y); }
	friend INLINE_NO_DEBUG TMatrix22 operator - (const TMatrix22& a, const TMatrix22& b)	{ return TMatrix22(a.x - b.x, a.y - b.y); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix22 operator * (const t_OtherScalar& f, const TMatrix22& m){ ScalarOptimal_t F(f); return TMatrix22(m.x * F, m.y * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix22 operator * (const TMatrix22& m, const t_OtherScalar& f){ ScalarOptimal_t F(f); return TMatrix22(m.x * F, m.y * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix22 operator / (const TMatrix22& m, const t_OtherScalar& f){ ScalarOptimal_t F(rcp(f)); return TMatrix22(m.x * F, m.y * F); }

	INLINE_NO_DEBUG TMatrix22 operator + () const											{ return *this; }
	INLINE_NO_DEBUG TMatrix22 operator - () const											{ return TMatrix22(-x, -y); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)								{ ScalarOptimal_t F(f); x *= F; y *= F; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)								{ ScalarOptimal_t F(rcp(f)); x *= F; y *= F; }
	INLINE_NO_DEBUG void operator += (const TMatrix22& m)									{ x += m.x; y += m.y; }
	INLINE_NO_DEBUG void operator -= (const TMatrix22& m)									{ x -= m.x; y -= m.y; }
	INLINE_NO_DEBUG void operator *= (const TMatrix22& m)									{ *this = *this * m; }

	INLINE_NO_DEBUG TMatrix22 transpose()const
	{
		TMatrix22 ret;
		transpose_impl(ret.x, ret.y, x, y);
		return ret;
	}

	INLINE_NO_DEBUG TMatrix22 invertForOrthogonal()const
	{
		TMatrix22 ret = transpose();
		Vector_t k = rcp(Vector_t(x.lenghtSquare(), y.lenghtSquare()));
		ret.x *= k;
		ret.y *= k;
		return ret;
	}

	INLINE_NO_DEBUG TMatrix22 invert()const
	{
		TMatrix22 M;
		M.x.a[0] = y.a[1];
		M.y.a[0] = -y.a[0];
		M.x.a[1] = -x.a[1];
		M.y.a[1] = x.a[0];
		Scalar_t det = x.a[0]*M.x.a[0] + x.a[1]*M.y.a[0];
		return M / det;
	}

	INLINE_NO_DEBUG Scalar_t determinant()const
	{
		return det2x2(	x.a[0], y.a[0],
						x.a[1], y.a[1]);
	}

	INLINE_NO_DEBUG TMatrix22 postScale(const Vector_t& s)const
	{
		return TMatrix22(x * s, y * s);
	}

	INLINE_NO_DEBUG TMatrix22 preScale(const Vector_t& s)const
	{
		return TMatrix22(x * s.x, y * s.y);
	}

	INLINE_NO_DEBUG static TMatrix22 makeScaleMatrix(const Vector_t& s) 
	{
		return TMatrix22(Vector_t(s.x,Scalar_t(0)),Vector_t(Scalar_t(0),s.y));
	}

	INLINE_NO_DEBUG static TMatrix22 makeRotationMatrix(Scalar_t alpha) 
	{
		TMatrix22 m;
		cosSin(m.x.a[0], m.x.a[1], alpha);
		m.y.a[0] = -m.x.a[1];
		m.y.a[1] = m.x.a[0];
		return m;
	}

	INLINE_NO_DEBUG void setRotationMatrix(float angle)
	{
		*this = makeRotationMatrix(angle);
	}

	INLINE_NO_DEBUG void setRow(uint n, const Vector_t& v)
	{
		(*this)(n, 0) = v.x;
		(*this)(n, 1) = v.y;
	}
};

template<class t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TMatrix22<t_Scalar>& m, const TMatrix22<t_Scalar>& n, typename TMatrix22<t_Scalar>::Scalar_t eps)
{ 
	return equals(m.x, n.x, eps) && equals(m.y, n.y, eps);
}

template<class t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TMatrix22<t_Scalar>& m)
{
	return isFinite(m.x) && isFinite(m.y);
}



template<typename t_Scalar>
struct TMatrix23
{
	typedef t_Scalar Scalar_t;
	typedef TVector2<t_Scalar> Vector_t;
	typedef typename Vector_t::ScalarOptimal_t ScalarOptimal_t;

	Vector_t x, y, p;

	INLINE_NO_DEBUG TMatrix22<Scalar_t>& m22() { return *reinterpret_cast<TMatrix22<Scalar_t>*>(this); }
	INLINE_NO_DEBUG const TMatrix22<Scalar_t>& m22()const { return *reinterpret_cast<const TMatrix22<Scalar_t>*>(this); }

	INLINE_NO_DEBUG TMatrix23() : x(Scalar_t(1), Scalar_t(0)), y(Scalar_t(0), Scalar_t(1))		{}
	INLINE_NO_DEBUG TMatrix23(const TMatrix23& m) : x(m.x), y(m.y), p(m.p) {}
	INLINE_NO_DEBUG TMatrix23(const Vector_t& X, const Vector_t& Y, const Vector_t& P) : x(X), y(Y), p(P) {}
	INLINE_NO_DEBUG TMatrix23(const TMatrix22<Scalar_t>& m, const Vector_t& P) : x(m.x), y(m.y), p(P) {}
	INLINE_NO_DEBUG void set(const TMatrix23& m) 												{ x = m.x; y = m.y; p = m.p; };
	INLINE_NO_DEBUG void set(const Vector_t& X, const Vector_t& Y, const Vector_t& Z, const Vector_t& P) { x = X; y = Y; p = P; }
	INLINE_NO_DEBUG void set(const TMatrix22<Scalar_t>& m, const Vector_t& P)					{ x = m.x; y = m.y; p = P; };
	INLINE_NO_DEBUG void setIdentity()															{ x.set(Scalar_t(1), Scalar_t(0)); y.set(Scalar_t(0), Scalar_t(1)); p.setZero(); }
	INLINE_NO_DEBUG static TMatrix23 getIdentity() 												{ return TMatrix23(Vector_t(Scalar_t(1), Scalar_t(0)), Vector_t(Scalar_t(0), Scalar_t(1)), Vector_t(Scalar_t(0), Scalar_t(0))); }
	INLINE_NO_DEBUG TMatrix23& operator = (const TMatrix23& m)									{ x = m.x; y = m.y; p = m.p; return *this; }
	INLINE_NO_DEBUG boolean operator == (const TMatrix23& m) const								{ return this->x == m.x && this->y == m.y  && p == m.p; }
	INLINE_NO_DEBUG boolean operator != (const TMatrix23& m) const								{ return this->x != m.x || this->y != m.y  || p != m.p; }

	INLINE_NO_DEBUG Scalar_t& operator () (int nRow, int nColumn)								{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Scalar_t  operator () (int nRow, int nColumn) const							{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Vector_t& operator [] (int index)											{ return (&x)[index]; }
	INLINE_NO_DEBUG const Vector_t& operator [] (int index) const								{ return (&x)[index]; }	

	friend INLINE_NO_DEBUG Vector_t operator * (const TMatrix23& m, const Vector_t& v)
	{
		return m.x * v.x + m.y * v.y + m.p;
	}

	INLINE_NO_DEBUG Vector_t tranMult(const Vector_t& v)const
	{
		return m22().tranMult(v - p);
	}

	friend INLINE_NO_DEBUG TMatrix23 operator * (const TMatrix23& a, const TMatrix23& b)
	{
		TMatrix23 ret;
		ret.x = a.m22() * b.x;
		ret.y = a.m22() * b.y;
		ret.p = a.m22() * b.p + a.p;
		return ret;
	}

	friend INLINE_NO_DEBUG TMatrix23 operator + (const TMatrix23& a, const TMatrix23& b)	{ return TMatrix23(a.x + b.x, a.y + b.y, a.p + b.p); }
	friend INLINE_NO_DEBUG TMatrix23 operator - (const TMatrix23& a, const TMatrix23& b)	{ return TMatrix23(a.x - b.x, a.y - b.y, a.p - b.p); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix23 operator * (const t_OtherScalar& f, const TMatrix23& m){ ScalarOptimal_t F(f); return TMatrix23(m.x * F, m.y * F, m.p * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix23 operator * (const TMatrix23& m, const t_OtherScalar& f){ ScalarOptimal_t F(f); return TMatrix23(m.x * F, m.y * F, m.p * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix23 operator / (const TMatrix23& m, const t_OtherScalar& f){ ScalarOptimal_t F(rcp(f)); return TMatrix23(m.x * F, m.y * F, m.p * F); }

	INLINE_NO_DEBUG TMatrix23 operator + () const											{ return *this; }
	INLINE_NO_DEBUG TMatrix23 operator - () const											{ return TMatrix23(-x, -y, -p); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)								{ ScalarOptimal_t F(f); x *= F; y *= F; p *= F; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)								{ ScalarOptimal_t F(rcp(f)); x *= F; y *= F; p *= F; }
	INLINE_NO_DEBUG void operator += (const TMatrix23& m)									{ x += m.x; y += m.y; p += m.p; }
	INLINE_NO_DEBUG void operator -= (const TMatrix23& m)									{ x -= m.x; y -= m.y; p -= m.p; }
	INLINE_NO_DEBUG void operator *= (const TMatrix23& m)									{ *this = *this * m; }

	INLINE_NO_DEBUG TMatrix23 transpose()const
	{
		TMatrix23 ret;
		ret.m22() = m22().transpose();
		ret.p = - (ret.m22() * p);
		return ret;
	}

	INLINE_NO_DEBUG TMatrix23 invertForOrthogonal()const
	{
		TMatrix23 ret;
		ret.m22() = m22().transpose();
		Vector_t k = rcp(Vector_t(x.lenghtSquare(), y.lenghtSquare()));
		ret.x *= k;
		ret.y *= k;
		ret.p = - (ret.m22() * p);
		return ret;
	}

	INLINE_NO_DEBUG TMatrix23 invert()const
	{
		TMatrix23 M;

		M.x.a[0] = det2x2(
			y.a[1],p.a[1],
			y.a[2],p.a[2]);
		M.y.a[0] = -det2x2(
			y.a[0],p.a[0],
			y.a[2],p.a[2]);
		M.p.a[0] = det2x2(
			y.a[0],p.a[0],
			y.a[1],p.a[1]);

		M.x.a[1] = -det2x2(
			x.a[1],p.a[1],
			x.a[2],p.a[2]);
		M.y.a[1] = det2x2(
			x.a[0],p.a[0],
			x.a[2],p.a[2]);
		M.p.a[1] = -det2x2(
			x.a[0],p.a[0],
			x.a[1],p.a[1]);

		Scalar_t det = x.a[0]*M.x.a[0] + x.a[1]*M.y.a[0];
		return M / det;
	}

	INLINE_NO_DEBUG TMatrix23 postScale(const Vector_t& s)const
	{
		return TMatrix23(x * s, y * s, p * s);
	}

	INLINE_NO_DEBUG TMatrix23 preScale(const Vector_t& s)const
	{
		return TMatrix23(x * s.x, y * s.y, p);
	}

	INLINE_NO_DEBUG static TMatrix23 makeScaleMatrix(const Vector_t& s)
	{
		return TMatrix23(Vector_t(s.x,Scalar_t(0)),Vector_t(Scalar_t(0),s.y),Vector_t(Scalar_t(0),Scalar_t(0)));
	}

	INLINE_NO_DEBUG static TMatrix23 makeTranslateMatrix(const Vector_t& t)
	{ 
		return TMatrix23(Vector_t(Scalar_t(1),Scalar_t(0)), Vector_t(Scalar_t(0),Scalar_t(1)),t);
	}

	INLINE_NO_DEBUG static TMatrix23 makeScaleTranslateMatrix(const Vector_t& s, const Vector_t& t)
	{
		return TMatrix23(Vector_t(s.x,Scalar_t(0)),Vector_t(Scalar_t(0),s.y), t);
	}
};

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TMatrix23<t_Scalar>& m, const TMatrix23<t_Scalar>& n, typename TMatrix23<t_Scalar>::Scalar_t eps)
{
	return equals(m.x, n.x, eps) && equals(m.y, n.y, eps) && equals(m.p, n.p, eps);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TMatrix23<t_Scalar>& m)
{
	return isFinite(m.x) && isFinite(m.y) && isFinite(m.p);
}

template<typename t_Scalar>
struct TMatrix33
{
	typedef t_Scalar Scalar_t;
	typedef TVector3<t_Scalar> Vector_t;
	typedef typename Vector_t::ScalarOptimal_t ScalarOptimal_t;

	Vector_t x, y, z;

	INLINE_NO_DEBUG TMatrix33() : x(Scalar_t(1), Scalar_t(0), Scalar_t(0)), y(Scalar_t(0), Scalar_t(1), Scalar_t(0)), z(Scalar_t(0), Scalar_t(0), Scalar_t(1)) {}
	INLINE_NO_DEBUG TMatrix33(const TMatrix33& m) : x(m.x), y(m.y), z(m.z)						{}
	INLINE_NO_DEBUG TMatrix33(const Vector_t& X, const Vector_t& Y, const Vector_t& Z) : x(X), y(Y), z(Z) {}
	INLINE_NO_DEBUG void set(const TMatrix33& m) 												{ x = m.x; y = m.y; z = m.z; }
	INLINE_NO_DEBUG void set(const Vector_t& X, const Vector_t& Y, const Vector_t& Z, const Vector_t& P) { x = X; y = Y; z = Z; }
	INLINE_NO_DEBUG void setIdentity()															{ x.set(Scalar_t(1), Scalar_t(0), Scalar_t(0)); y.set(Scalar_t(0), Scalar_t(1), Scalar_t(0)); z.set(Scalar_t(0), Scalar_t(0), Scalar_t(1)); }
	INLINE_NO_DEBUG static TMatrix33 getIdentity() 												{ return TMatrix33(Vector_t(Scalar_t(1), Scalar_t(0), Scalar_t(0)),Vector_t(Scalar_t(0), Scalar_t(1), Scalar_t(0)),Vector_t(Scalar_t(0), Scalar_t(0), Scalar_t(1))); }
	INLINE_NO_DEBUG TMatrix33& operator = (const TMatrix33& m)									{ x = m.x; y = m.y; z = m.z; return *this; }
	INLINE_NO_DEBUG boolean operator == (const TMatrix33& m) const								{ return this->x == m.x && this->y == m.y && this->z == m.z; }
	INLINE_NO_DEBUG boolean operator != (const TMatrix33& m) const								{ return this->x != m.x || this->y != m.y || this->z != m.z; }

	INLINE_NO_DEBUG Scalar_t& operator () (int nRow, int nColumn)								{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Scalar_t  operator () (int nRow, int nColumn) const							{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Vector_t& operator [] (int index)											{ return (&x)[index]; }
	INLINE_NO_DEBUG const Vector_t& operator [] (int index) const								{ return (&x)[index]; }	

	friend INLINE_NO_DEBUG Vector_t operator * (const TMatrix33& m, const Vector_t& v)
	{
		return m.x * v.x + m.y * v.y + m.z * v.z;
	}

	INLINE_NO_DEBUG Vector_t tranMult(const Vector_t& v)const
	{
		return Vector_t(x.dot(v), y.dot(v), z.dot(v));
	}

	friend INLINE_NO_DEBUG TMatrix33 operator * (const TMatrix33& a, const TMatrix33& b)
	{
		return TMatrix33(a * b.x, a * b.y, a * b.z);
	}

	friend INLINE_NO_DEBUG TMatrix33 operator + (const TMatrix33& a, const TMatrix33& b)	{ return TMatrix33(a.x + b.x, a.y + b.y, a.z + b.z); }
	friend INLINE_NO_DEBUG TMatrix33 operator - (const TMatrix33& a, const TMatrix33& b)	{ return TMatrix33(a.x - b.x, a.y - b.y, a.z - b.z); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix33 operator * (const t_OtherScalar& f, const TMatrix33& m){ ScalarOptimal_t F(f); return TMatrix33(m.x * F, m.y * F, m.z * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix33 operator * (const TMatrix33& m, const t_OtherScalar& f){ ScalarOptimal_t F(f); return TMatrix33(m.x * F, m.y * F, m.z * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix33 operator / (const TMatrix33& m, const t_OtherScalar& f){ ScalarOptimal_t F(rcp(f)); return TMatrix33(m.x * F, m.y * F, m.z * F); }

	INLINE_NO_DEBUG TMatrix33 operator + () const											{ return *this; }
	INLINE_NO_DEBUG TMatrix33 operator - () const											{ return TMatrix33(-x, -y, -z); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)								{ ScalarOptimal_t F(f); x *= F; y *= F; z *= F; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)								{ ScalarOptimal_t F(rcp(f)); x *= F; y *= F; z *= F; }
	INLINE_NO_DEBUG void operator += (const TMatrix33& m)									{ x += m.x; y += m.y; z += m.z; }
	INLINE_NO_DEBUG void operator -= (const TMatrix33& m)									{ x -= m.x; y -= m.y; z -= m.z; }
	INLINE_NO_DEBUG void operator *= (const TMatrix33& m)									{ *this = *this * m; }

	INLINE_NO_DEBUG TMatrix33 transpose()const
	{
		TMatrix33 ret;
		transpose_impl(ret.x, ret.y, ret.z, x, y, z);
		return ret;
	}

	INLINE_NO_DEBUG TMatrix33 invertForOrthogonal()const
	{
		TMatrix33 ret = transpose();
		Vector_t k(rcp(x.lenghtSquare()), rcp(y.lenghtSquare()), rcp(z.lenghtSquare()));
		ret.x *= k;
		ret.y *= k;
		ret.z *= k;
		return ret;
	}

	INLINE_NO_DEBUG TMatrix33 invert()const
	{
		TMatrix33 M;

		M.x.a[0] = det2x2(
			y.a[1],z.a[1],
			y.a[2],z.a[2]);
		M.y.a[0] = -det2x2(
			y.a[0],z.a[0],
			y.a[2],z.a[2]);
		M.z.a[0] = det2x2(
			y.a[0],z.a[0],
			y.a[1],z.a[1]);

		M.x.a[1] = -det2x2(
			x.a[1],z.a[1],
			x.a[2],z.a[2]);
		M.y.a[1] = det2x2(
			x.a[0],z.a[0],
			x.a[2],z.a[2]);
		M.z.a[1] = -det2x2(
			x.a[0],z.a[0],
			x.a[1],z.a[1]);

		M.x.a[2] = det2x2(
			x.a[1],y.a[1],
			x.a[2],y.a[2]);
		M.y.a[2] = -det2x2(
			x.a[0],y.a[0],
			x.a[2],y.a[2]);
		M.z.a[2] = det2x2(
			x.a[0],y.a[0],
			x.a[1],y.a[1]);

		Scalar_t det = x.a[0]*M.x.a[0] + x.a[1]*M.y.a[0] + x.a[2]*M.z.a[0];
		return M / det;
	}

	INLINE_NO_DEBUG Scalar_t determinant()const
	{
		return det3x3(	x.a[0], y.a[0], z.a[0],
						x.a[1], y.a[1], z.a[1],
						x.a[2], y.a[2], z.a[2]);
	}
	
	INLINE_NO_DEBUG void setRotationMatrixFromAnglesXZY(const Vector_t& angles) 
	{
		Scalar_t c1,s1;
		Scalar_t c2,s2;
		Scalar_t c3,s3;
		cosSin(c1,s1,angles.a[0]);
		cosSin(c2,s2,angles.a[1]);
		cosSin(c3,s3,angles.a[2]);

		x.a[0] = c2*c3;
		y.a[0] = -s2;
		z.a[0] = c2*s3;

		x.a[1] = s1*s3 + c1*c3*s2;
		y.a[1] = c1*c2;
		z.a[1] = c1*s2*s3 - c3*s1;

		x.a[2] = c3*s1*s2 - c1*s3;
		y.a[2] = c2*s1;
		z.a[2] = c1*c3 + s1*s2*s3;
	}

	INLINE_NO_DEBUG void setRotationMatrixFromAnglesXYZ(const Vector_t& angles) 
	{
		Scalar_t c1,s1;
		Scalar_t c2,s2;
		Scalar_t c3,s3;
		cosSin(c1,s1,angles.a[0]);
		cosSin(c2,s2,angles.a[1]);
		cosSin(c3,s3,angles.a[2]);

		x.a[0] = c2*c3;
		x.a[1] = c1*s3 + c3*s1*s2;
		x.a[2] = s1*s3 - c1*c3*s2;

		y.a[0] = -c2*s3;
		y.a[1] = c1*c3 - s1*s2*s3;
		y.a[2] = c3*s1 + c1*s2*s3;

		z.a[0] = s2;
		z.a[1] = -c2*s1;
		z.a[2] = c1*c2;
	}

	INLINE_NO_DEBUG void setRotationMatrixFromAnglesYXZ(const Vector_t& angles) 
	{
		Scalar_t c1,s1;
		Scalar_t c2,s2;
		Scalar_t c3,s3;
		cosSin(c1,s1,angles.a[0]);
		cosSin(c2,s2,angles.a[1]);
		cosSin(c3,s3,angles.a[2]);

		x.a[0] = c1*c3 + s1*s2*s3;
		x.a[1] = c2*s3;
		x.a[2] = c1*s2*s3 - c3*s1;

		y.a[0] = c3*s1*s2 - c1*s3;
		y.a[1] = c2*c3;
		y.a[2] = s1*s3 + c1*c3*s2;

		z.a[0] = c2*s1;
		z.a[1] = -s2;
		z.a[2] = c1*c2;
	}

	INLINE_NO_DEBUG void setRotationMatrixFromAnglesYZX(const Vector_t& angles) 
	{
		Scalar_t c1,s1;
		Scalar_t c2,s2;
		Scalar_t c3,s3;
		cosSin(c1,s1,angles.a[0]);
		cosSin(c2,s2,angles.a[1]);
		cosSin(c3,s3,angles.a[2]);

		x.a[0] = c1*c2;
		x.a[1] = s2;
		x.a[2] = -c2*s1;

		y.a[0] = s1*s3 - c1*c3*s2;
		y.a[1] = c2*c3;
		y.a[2] = c1*s3 + c3*s1*s2;

		z.a[0] = c3*s1 + c1*s2*s3;	
		z.a[1] = -c2*s3;
		z.a[2] = c1*c3 - s1*s2*s3;
	}

	INLINE_NO_DEBUG void setRotationMatrixFromAnglesZYX(const Vector_t& angles) 
	{
		Scalar_t c1,s1;
		Scalar_t c2,s2;
		Scalar_t c3,s3;
		cosSin(c1,s1,angles.a[0]);
		cosSin(c2,s2,angles.a[1]);
		cosSin(c3,s3,angles.a[2]);

		x.a[0] = c1*c2;
		x.a[1] = c2*s1;
		x.a[2] = -s2;

		y.a[0] = c1*s2*s3 - c3*s1;
		y.a[1] = c1*c3 + s1*s2*s3;
		y.a[2] = c2*s3;

		z.a[0] = s1*s3 + c1*c3*s2;
		z.a[1] = c3*s1*s2 - c1*s3;
		z.a[2] = c2*c3;
	}

	INLINE_NO_DEBUG void setRotationMatrixFromAnglesZXY(const Vector_t& angles) 
	{
		Scalar_t c1,s1;
		Scalar_t c2,s2;
		Scalar_t c3,s3;
		cosSin(c1,s1,angles.a[0]);
		cosSin(c2,s2,angles.a[1]);
		cosSin(c3,s3,angles.a[2]);

		x.a[0] = c1*c3 - s1*s2*s3;
		x.a[1] = c3*s1 + c1*s2*s3;
		x.a[2] = -c2*s3;

		y.a[0] = -c2*s1;
		y.a[1] = c1*c2;
		y.a[2] = s2;

		z.a[0] = c1*s3 + c3*s1*s2;
		z.a[1] = s1*s3 - c1*c3*s2;
		z.a[2] = c2*c3;
	}

	INLINE_NO_DEBUG void setRotationAroundXMatrix(Scalar_t angle) 
	{
		Scalar_t c,s;
		cosSin(c, s, angle);

		x.a[0] = Scalar_t(1);
		x.a[1] = Scalar_t(0);
		x.a[2] = Scalar_t(0);
		y.a[0] = Scalar_t(0);
		y.a[1] = c;
		y.a[2] = s;
		z.a[0] = Scalar_t(0);
		z.a[1] = -s;
		z.a[2] = c;
	}

	INLINE_NO_DEBUG void setRotationAroundYMatrix(Scalar_t angle) 
	{
		Scalar_t c,s;
		cosSin(c, s, angle);

		x.a[0] = c;
		x.a[1] = Scalar_t(0);
		x.a[2] = -s;
		y.a[0] = Scalar_t(0);
		y.a[1] = Scalar_t(1);
		y.a[2] = Scalar_t(0);
		z.a[0] = s;
		z.a[1] = Scalar_t(0);
		z.a[2] = c;
	}

	INLINE_NO_DEBUG void setRotationAroundZMatrix(Scalar_t angle) 
	{
		Scalar_t c,s;
		cosSin(c, s, angle);

		x.a[0] = c;
		x.a[1] = s;
		x.a[2] = Scalar_t(0);
		y.a[0] = -s;
		y.a[1] = c;
		y.a[2] = Scalar_t(0);
		z.a[0] = Scalar_t(0);
		z.a[1] = Scalar_t(0);
		z.a[2] = Scalar_t(1);
	}

	INLINE_NO_DEBUG void setRotationMatrixFromQuaternion(const TQuaternion<Scalar_t>& quat) 
	{
		Scalar_t	wx, wy, wz;
		Scalar_t	xx, yy, yz;
		Scalar_t	xy, xz, zz;
		Scalar_t	x2, y2, z2;

		x2 = quat.a[0] + quat.a[0];
		y2 = quat.a[1] + quat.a[1];
		z2 = quat.a[2] + quat.a[2];

		xx = quat.a[0] * x2;
		xy = quat.a[0] * y2;
		xz = quat.a[0] * z2;

		yy = quat.a[1] * y2;
		yz = quat.a[1] * z2;
		zz = quat.a[2] * z2;

		wx = quat.a[3] * x2;
		wy = quat.a[3] * y2;
		wz = quat.a[3] * z2;

		x.a[0] = Scalar_t(1.0) - ( yy + zz );
		x.a[1] = xy + wz;
		x.a[2] = xz - wy;

		y.a[0] = xy - wz;
		y.a[1] = Scalar_t(1.0) - ( xx + zz );
		y.a[2] = yz + wx;

		z.a[0] = xz + wy;
		z.a[1] = yz - wx;
		z.a[2] = Scalar_t(1.0) - ( xx + yy );
	}

	INLINE_NO_DEBUG void setRotationMatrixFromAxisAngle(const Vector_t& axisAngle) 
	{
		Scalar_t fSin,fCos;
		cosSin(fCos, fSin, axisAngle.w);
		x.a[0] = axisAngle.a[0] * axisAngle.a[0] + (Scalar_t(1) - axisAngle.a[0] * axisAngle.a[0]) * fCos;
		x.a[1] = axisAngle.a[0] * axisAngle.a[1] * (Scalar_t(1) - fCos) + axisAngle.a[2] * fSin;
		x.a[2] = axisAngle.a[0] * axisAngle.a[2] * (Scalar_t(1) - fCos) - axisAngle.a[1] * fSin;

		y.a[0] = axisAngle.a[1] * axisAngle.a[0] * (Scalar_t(1) - fCos) - axisAngle.a[2] * fSin;
		y.a[1] = axisAngle.a[1] * axisAngle.a[1] + (Scalar_t(1) - axisAngle.a[1] * axisAngle.a[1]) * fCos;
		y.a[2] = axisAngle.a[1] * axisAngle.a[2] * (Scalar_t(1) - fCos) + axisAngle.a[0] * fSin;

		z.a[0] = axisAngle.a[2] * axisAngle.a[0] * (Scalar_t(1) - fCos) + axisAngle.a[1] * fSin;
		z.a[1] = axisAngle.a[2] * axisAngle.a[1] * (Scalar_t(1) - fCos) - axisAngle.a[0] * fSin;
		z.a[2] = axisAngle.a[2] * axisAngle.a[2] + (Scalar_t(1) - axisAngle.a[2] * axisAngle.a[2]) * fCos;
	}

	INLINE_NO_DEBUG Vector_t toAnglesXZY()const
	{
		Vector_t angles;
		angles.a[0] = atan2(y.a[2], y.a[1]);
		angles.a[1] = asin(-y.a[0]);
		angles.a[2] = atan2(z.a[0], x.a[0]);
		return angles;
	}

	INLINE_NO_DEBUG Vector_t toAnglesXYZ()const
	{
		Vector_t angles;
		angles.a[0] = atan2(-z.a[1], z.a[2]);
		angles.a[1] = asin(z.a[0]);
		angles.a[2] = atan2(-y.a[0], x.a[0]);
		return angles;
	}

	INLINE_NO_DEBUG Vector_t toAnglesYXZ()const
	{
		Vector_t angles;
		angles.a[0] = atan2(z.a[0], z.a[2]);
		angles.a[1] = asin(-z.a[1]);
		angles.a[2] = atan2(x.a[1], y.a[1]);
		return angles;
	}

	INLINE_NO_DEBUG Vector_t toAnglesYZX()const
	{
		Vector_t angles;
		angles.a[0] = atan2(-x.a[2], x.a[0]);
		angles.a[1] = asin(x.a[1]);
		angles.a[2] = atan2(-z.a[1], y.a[1]);
		return angles;
	}

	INLINE_NO_DEBUG Vector_t toAnglesZYX()const
	{
		Vector_t angles;
		angles.a[0] = atan2(x.a[1], x.a[0]);
		angles.a[1] = asin(-x.a[2]);
		angles.a[2] = atan2(y.a[2], z.a[2]);
		return angles;
	}

	INLINE_NO_DEBUG Vector_t toAnglesZXY()const
	{
		Vector_t angles;
		angles.a[0] = atan2(-y.a[0], y.a[1]);
		angles.a[1] = asin(y.a[2]);
		angles.a[2] = atan2(-x.a[2], z.a[2]);
		return angles;
	}

	INLINE_NO_DEBUG TQuaternion<Scalar_t> toQuaternion()const
	{
		TQuaternion<Scalar_t> q;
		float trace = x.a[0] + y.a[1] + z.a[2];
		if (trace > Scalar_t(0)){
			Scalar_t s = Scalar_t(0.5) / sqrt(trace + Scalar_t(1));
			q.a[3] = Scalar_t(0.25) / s;
			q.a[0] = (y.a[2] - z.a[1]) * s;
			q.a[1] = (z.a[0] - x.a[2]) * s;
			q.a[2] = (x.a[1] - y.a[0]) * s;
		}
		else 
		{
			if (x.a[0] > y.a[1] && x.a[0] > z.a[2]) {
				float s = Scalar_t(2) * sqrt(Scalar_t(1) + x.a[0] - y.a[1] - z.a[2]);
				q.a[3] = (y.a[2] - z.a[1]) / s;
				q.a[0] = Scalar_t(0.25) * s;
				q.a[1] = (y.a[0] + x.a[1]) / s;
				q.a[2] = (z.a[0] + x.a[2]) / s;
			} else if (y.a[1] > z.a[2]) {
				float s = Scalar_t(2) * sqrt(Scalar_t(1) + y.a[1] - x.a[0] - z.a[2]);
				q.a[3] = (z.a[0] - x.a[2]) / s;
				q.a[0] = (y.a[0] + x.a[1]) / s;
				q.a[1] = Scalar_t(0.25) * s;
				q.a[2] = (z.a[1] + y.a[2]) / s;
			} else {
				float s = Scalar_t(2) * sqrt(Scalar_t(1) + z.a[2] - x.a[0] - y.a[1]);
				q.a[3] = (x.a[1] - y.a[0]) / s;
				q.a[0] = (z.a[0] + x.a[2]) / s;
				q.a[1] = (z.a[1] + y.a[2]) / s;
				q.a[2] = Scalar_t(0.25) * s;
			}
		}
		return q;
	}

	INLINE_NO_DEBUG Vector_t toAxisAngle()const
	{
		return axisAngleFromQuaternion(toQuaternion());
	}


	INLINE_NO_DEBUG static TMatrix33 makeRotationMatrixFromAnglesXZY(const Vector_t& angles) 
	{
		TMatrix33 ret;
		ret.setRotationMatrixFromAnglesXZY(angles);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationMatrixFromAnglesXYZ(const Vector_t& angles) 
	{
		TMatrix33 ret;
		ret.setRotationMatrixFromAnglesXYZ(angles);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationMatrixFromAnglesYXZ(const Vector_t& angles) 
	{
		TMatrix33 ret;
		ret.setRotationMatrixFromAnglesYXZ(angles);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationMatrixFromAnglesYZX(const Vector_t& angles) 
	{
		TMatrix33 ret;
		ret.setRotationMatrixFromAnglesYZX(angles);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationMatrixFromAnglesZYX(const Vector_t& angles) 
	{
		TMatrix33 ret;
		ret.setRotationMatrixFromAnglesZYX(angles);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationMatrixFromAnglesZXY(const Vector_t& angles) 
	{
		TMatrix33 ret;
		ret.setRotationMatrixFromAnglesZXY(angles);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationAroundXMatrix(Scalar_t angle) 
	{
		TMatrix33 ret;
		ret.setRotationAroundXMatrix(angle);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationAroundYMatrix(Scalar_t angle) 
	{
		TMatrix33 ret;
		ret.setRotationAroundYMatrix(angle);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationAroundZMatrix(Scalar_t angle) 
	{
		TMatrix33 ret;
		ret.setRotationAroundZMatrix(angle);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationMatrix(const TQuaternion<Scalar_t>& quat) 
	{
		TMatrix33 ret;
		ret.setRotationMatrix(quat);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeRotationMatrixFromAxisAngle(const Vector_t& axisAngle) 
	{
		TMatrix33 ret;
		ret.setRotationMatrixFromAxisAngle(axisAngle);
		return ret;
	}

	INLINE_NO_DEBUG static TMatrix33 makeScaleMatrix(const Vector_t& s)
	{
		return TMatrix33(Vector_t(s.x,Scalar_t(0),Scalar_t(0)),Vector_t(Scalar_t(0),s.y,Scalar_t(0)),Vector_t(Scalar_t(0),Scalar_t(0),s.z));
	}


	INLINE_NO_DEBUG TMatrix33 postScale(const Vector_t& s)const
	{
		return TMatrix33(x * s, y * s, z * s);
	}

	INLINE_NO_DEBUG TMatrix33 preScale(const Vector_t& s)const
	{
		return TMatrix33(x * s.x, y * s.y, z * s.z);
	}

	INLINE_NO_DEBUG TMatrix33 normal()const
	{
		return TMatrix33(x.normal(), y.normal(), z.normal());
	}

	INLINE_NO_DEBUG TMatrix33 normalFast()const
	{
		return TMatrix33(x.normalFast(), y.normalFast(), z.normalFast());
	}

	INLINE_NO_DEBUG TMatrix33 othogonalNormal_RightestX()const
	{
		TMatrix33 m;
		m.x = x.normal();
		m.y = y.subdot(m.x).normal();
		m.z = z.subdot(m.x).subdot(m.y).normal();
		return m;
	}

	INLINE_NO_DEBUG TMatrix33 othogonalNormal_RightestY()const
	{
		TMatrix33 m;
		m.y = y.normal();
		m.x = x.subdot(m.y).normal();
		m.z = z.subdot(m.x).subdot(m.y).normal();
		return m;
	}

	INLINE_NO_DEBUG TMatrix33 othogonalNormal_RightestZ()const
	{
		TMatrix33 m;
		m.z = z.normal();
		m.x = x.subdot(m.z).normal();
		m.y = y.subdot(m.x).subdot(m.z).normal();
		return m;
	}

	INLINE_NO_DEBUG void selfOrthogonalizeAndNormalize_RightestX()
	{
		*this = othogonalNormal_RightestX(); 
	}

	INLINE_NO_DEBUG void selfOrthogonalizeAndNormalize_RightestY()
	{
		*this = othogonalNormal_RightestY(); 
	}

	INLINE_NO_DEBUG void selfOrthogonalizeAndNormalize_RightestZ()
	{
		*this = othogonalNormal_RightestZ(); 
	}

	INLINE_NO_DEBUG void setRow(uint n, const Vector_t& v)
	{
		(*this)(n, 0) = v.x;
		(*this)(n, 1) = v.y;
		(*this)(n, 2) = v.z;
	}
};

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TMatrix33<t_Scalar>& m, const TMatrix33<t_Scalar>& n, typename TMatrix33<t_Scalar>::Scalar_t eps)
{
	return equals(m.x, n.x, eps) && equals(m.y, n.y, eps) && equals(m.z, n.z, eps);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TMatrix33<t_Scalar>& m)
{
	return isFinite(m.x) && isFinite(m.y) && isFinite(m.z);
}


template<typename t_Scalar>
struct TMatrix34
{
	typedef t_Scalar Scalar_t;
	typedef TVector3<t_Scalar> Vector_t;
	typedef typename Vector_t::ScalarOptimal_t ScalarOptimal_t;

	Vector_t x, y, z, p;

	INLINE_NO_DEBUG TMatrix33<Scalar_t>& m33() { return *reinterpret_cast<TMatrix33<Scalar_t>*>(this); }
	INLINE_NO_DEBUG const TMatrix33<Scalar_t>& m33()const { return *reinterpret_cast<const TMatrix33<Scalar_t>*>(this); }

	INLINE_NO_DEBUG TMatrix34() : x(Scalar_t(1), Scalar_t(0), Scalar_t(0)), y(Scalar_t(0), Scalar_t(1), Scalar_t(0)), z(Scalar_t(0), Scalar_t(0), Scalar_t(1)) {}
	INLINE_NO_DEBUG TMatrix34(const TMatrix34& m) : x(m.x), y(m.y), z(m.z), p(m.p) {}
	INLINE_NO_DEBUG TMatrix34(const Vector_t& X, const Vector_t& Y, const Vector_t& Z, const Vector_t& P) : x(X), y(Y), z(Z), p(P) {}
	INLINE_NO_DEBUG TMatrix34(const TMatrix33<Scalar_t>& m, const Vector_t& P) : x(m.x), y(m.y), z(m.z), p(P) {}
	INLINE_NO_DEBUG void set(const TMatrix34& m) 												{ x = m.x; y = m.y; z = m.z; p = m.p; };
	INLINE_NO_DEBUG void set(const Vector_t& X, const Vector_t& Y, const Vector_t& Z, const Vector_t& P) { x = X; y = Y; z = Z; p = P; }
	INLINE_NO_DEBUG void set(const TMatrix33<Scalar_t>& m, const Vector_t& P)					{ x = m.x; y = m.y; z = m.z; p = P; };
	INLINE_NO_DEBUG void setIdentity()															{ x.set(Scalar_t(1), Scalar_t(0), Scalar_t(0)); y.set(Scalar_t(0), Scalar_t(1), Scalar_t(0)); z.set(Scalar_t(0), Scalar_t(0), Scalar_t(1)); p.setZero(); }
	INLINE_NO_DEBUG static TMatrix34 getIdentity() 												{ return TMatrix34(Vector_t(Scalar_t(1), Scalar_t(0), Scalar_t(0)),Vector_t(Scalar_t(0), Scalar_t(1), Scalar_t(0)),Vector_t(Scalar_t(0), Scalar_t(0), Scalar_t(1)),Vector_t(Scalar_t(0), Scalar_t(0), Scalar_t(0))); }
	INLINE_NO_DEBUG TMatrix34& operator = (const TMatrix34& m)									{ x = m.x; y = m.y; z = m.z; p = m.p; return *this; }
	INLINE_NO_DEBUG boolean operator == (const TMatrix34& m) const								{ return this->x == m.x && this->y == m.y && this->z == m.z && p == m.p; }
	INLINE_NO_DEBUG boolean operator != (const TMatrix34& m) const								{ return this->x != m.x || this->y != m.y || this->z != m.z || p != m.p; }

	INLINE_NO_DEBUG Scalar_t& operator () (int nRow, int nColumn)								{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Scalar_t  operator () (int nRow, int nColumn) const							{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Vector_t& operator [] (int index)											{ return (&x)[index]; }
	INLINE_NO_DEBUG const Vector_t& operator [] (int index) const								{ return (&x)[index]; }	
		
	friend INLINE_NO_DEBUG Vector_t operator * (const TMatrix34& m, const Vector_t& v)
	{
		return m.m33() * v + m.p;
	}

	INLINE_NO_DEBUG Vector_t tranMult(const Vector_t& v)const
	{
		return m33().tranMult(v - p);
	}

	friend INLINE_NO_DEBUG TMatrix34 operator * (const TMatrix34& a, const TMatrix34& b)
	{
		TMatrix34 ret;
		ret.x = a.m33() * b.x;
		ret.y = a.m33() * b.y;
		ret.z = a.m33() * b.z;
		ret.p = a.m33() * b.p + a.p;
		return ret;
	}

	friend INLINE_NO_DEBUG TMatrix34 operator + (const TMatrix34& a, const TMatrix34& b)	{ return TMatrix34(a.x + b.x, a.y + b.y, a.z + b.z, a.p + b.p); }
	friend INLINE_NO_DEBUG TMatrix34 operator - (const TMatrix34& a, const TMatrix34& b)	{ return TMatrix34(a.x - b.x, a.y - b.y, a.z - b.z, a.p - b.p); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix34 operator * (const t_OtherScalar& f, const TMatrix34& m){ ScalarOptimal_t F(f); return TMatrix34(m.x * F, m.y * F, m.z * F, m.p * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix34 operator * (const TMatrix34& m, const t_OtherScalar& f){ ScalarOptimal_t F(f); return TMatrix34(m.x * F, m.y * F, m.z * F, m.p * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix34 operator / (const TMatrix34& m, const t_OtherScalar& f){ ScalarOptimal_t F(rcp(f)); return TMatrix34(m.x * F, m.y * F, m.z * F, m.p * F); }

	INLINE_NO_DEBUG TMatrix34 operator + () const											{ return *this; }
	INLINE_NO_DEBUG TMatrix34 operator - () const											{ return TMatrix34(-x, -y, -z, -p); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)								{ ScalarOptimal_t F(f); x *= F; y *= F; z *= F; p *= F; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)								{ ScalarOptimal_t F(rcp(f)); x *= F; y *= F; z *= F; p *= F; }
	INLINE_NO_DEBUG void operator += (const TMatrix34& m)									{ x += m.x; y += m.y; z += m.z; p += m.p; }
	INLINE_NO_DEBUG void operator -= (const TMatrix34& m)									{ x -= m.x; y -= m.y; z -= m.z; p -= m.p; }
	INLINE_NO_DEBUG void operator *= (const TMatrix34& m)									{ *this = *this * m; }

	INLINE_NO_DEBUG TMatrix34 transpose()const
	{
		TMatrix34 ret;
		ret.m33() = m33().transpose();
		ret.p = - (ret.m33() * p);
		return ret;
	}

	INLINE_NO_DEBUG TMatrix34 invertForOrthogonal()const
	{
		TMatrix34 ret;
		ret.m33() = m33().transpose();
		Vector_t k(rcp(x.lenghtSquare()), rcp(y.lenghtSquare()), rcp(z.lenghtSquare()));
		ret.x *= k;
		ret.y *= k;
		ret.z *= k;
		ret.p = - (ret.m33() * p);
		return ret;
	}

	INLINE_NO_DEBUG TMatrix34 invert()const
	{
		TMatrix34 M;
		
		M.x.a[0] = det2x2(
			y.a[1],z.a[1],
			y.a[2],z.a[2]);
		M.y.a[0] = -det2x2(
			y.a[0],z.a[0],
			y.a[2],z.a[2]);
		M.z.a[0] = det2x2(
			y.a[0],z.a[0],
			y.a[1],z.a[1]);
		M.p.a[0] = -det3x3(
			y.a[0],z.a[0],p.a[0],
			y.a[1],z.a[1],p.a[1],
			y.a[2],z.a[2],p.a[2]);

		M.x.a[1] = -det2x2(
			x.a[1],z.a[1],
			x.a[2],z.a[2]);
		M.y.a[1] = det2x2(
			x.a[0],z.a[0],
			x.a[2],z.a[2]);
		M.z.a[1] = -det2x2(
			x.a[0],z.a[0],
			x.a[1],z.a[1]);
		M.p.a[1] = det3x3(
			x.a[0],z.a[0],p.a[0],
			x.a[1],z.a[1],p.a[1],
			x.a[2],z.a[2],p.a[2]);

		M.x.a[2] = det2x2(
			x.a[1],y.a[1],
			x.a[2],y.a[2]);
		M.y.a[2] = -det2x2(
			x.a[0],y.a[0],
			x.a[2],y.a[2]);
		M.z.a[2] = det2x2(
			x.a[0],y.a[0],
			x.a[1],y.a[1]);
		M.p.a[2] = -det3x3(
			x.a[0],y.a[0],p.a[0],
			x.a[1],y.a[1],p.a[1],
			x.a[2],y.a[2],p.a[2]);

		Scalar_t det = x.a[0]*M.x.a[0] + x.a[1]*M.y.a[0] + x.a[2]*M.z.a[0];
		return M / det;
	}

	INLINE_NO_DEBUG TMatrix34 postScale(const Vector_t& s)const
	{
		return TMatrix34(x * s, y * s, z * s, p * s);
	}

	INLINE_NO_DEBUG TMatrix34 preScale(const Vector_t& s)const
	{
		return TMatrix34(x * s.x, y * s.y, z * s.z, p);
	}

	INLINE_NO_DEBUG static TMatrix34 makeScaleMatrix(const Vector_t& s)
	{
		return TMatrix34(Vector_t(s.x, Scalar_t(0), Scalar_t(0)), Vector_t(Scalar_t(0), s.y, Scalar_t(0)), Vector_t(Scalar_t(0), Scalar_t(0), s.z), Vector_t(Scalar_t(0), Scalar_t(0), Scalar_t(0)));
	}

	INLINE_NO_DEBUG static TMatrix34 makTranslateMatrix(const Vector_t& t)
	{
		return TMatrix34(Vector_t(Scalar_t(1), Scalar_t(0), Scalar_t(0)),Vector_t(Scalar_t(0), Scalar_t(1), Scalar_t(0)), Vector_t(Scalar_t(0), Scalar_t(0), Scalar_t(1)), t);
	}

	INLINE_NO_DEBUG static TMatrix34 makScaleTranslateMatrix(const Vector_t& s, const Vector_t& t)
	{
		return TMatrix34(Vector_t(s.x, Scalar_t(0), Scalar_t(0)), Vector_t(Scalar_t(0), s.y, Scalar_t(0)), Vector_t(Scalar_t(0), Scalar_t(0), s.z), t);
	}
};

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TMatrix34<t_Scalar>& m, const TMatrix34<t_Scalar>& n, typename TMatrix34<t_Scalar>::Scalar_t eps)
{
	return equals(m.x, n.x, eps) && equals(m.y, n.y, eps) && equals(m.z, n.z, eps) && equals(m.p, n.p, eps);
}

template<typename t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TMatrix34<t_Scalar>& m)
{
	return isFinite(m.x) && isFinite(m.y) && isFinite(m.z) && isFinite(m.p);
}


template<typename t_Scalar>
struct TMatrix44
{
	typedef t_Scalar Scalar_t;
	typedef TVector4<t_Scalar> Vector_t;
	typedef typename Vector_t::ScalarOptimal_t ScalarOptimal_t;

	Vector_t x, y, z, w;

	INLINE_NO_DEBUG TMatrix44() :	x(Scalar_t(1), Scalar_t(0), Scalar_t(0), Scalar_t(0)), 
									y(Scalar_t(0), Scalar_t(1), Scalar_t(0), Scalar_t(0)),
									z(Scalar_t(0), Scalar_t(0), Scalar_t(1), Scalar_t(0)),
									w(Scalar_t(0), Scalar_t(0), Scalar_t(0), Scalar_t(1)) {}
	INLINE_NO_DEBUG TMatrix44(const TMatrix44& m) : x(m.x), y(m.y), z(m.z), w(m.w) {}
	INLINE_NO_DEBUG TMatrix44(const Vector_t& X, const Vector_t& Y, const Vector_t& Z, const Vector_t& W) : x(X), y(Y), z(Z), w(W) {}
	INLINE_NO_DEBUG void set(const TMatrix44& m) 											{ x = m.x; y = m.y; z = m.z; w = m.w; };
	INLINE_NO_DEBUG void set(const Vector_t& X, const Vector_t& Y, const Vector_t& Z, const Vector_t& W) { x = X; y = Y; z = Z; w = W; }
	INLINE_NO_DEBUG void setIdentity()														{ x.set(Scalar_t(1), Scalar_t(0), Scalar_t(0), Scalar_t(0)); y.set(Scalar_t(0), Scalar_t(1), Scalar_t(0), Scalar_t(0)); z.set(Scalar_t(0), Scalar_t(0), Scalar_t(1), Scalar_t(0)); w.set(Scalar_t(0), Scalar_t(0), Scalar_t(0), Scalar_t(1)); }
	INLINE_NO_DEBUG static TMatrix44 getIdentity() 											{ return TMatrix44(Vector_t(Scalar_t(1), Scalar_t(0), Scalar_t(0), Scalar_t(0)), Vector_t(Scalar_t(0), Scalar_t(1), Scalar_t(0), Scalar_t(0)), Vector_t(Scalar_t(0), Scalar_t(0), Scalar_t(1), Scalar_t(0)), Vector_t(Scalar_t(0), Scalar_t(0), Scalar_t(0), Scalar_t(1))); }
	INLINE_NO_DEBUG TMatrix44& operator = (const TMatrix44& m)								{ x = m.x; y = m.y; z = m.z; w = m.w; return *this; }
	INLINE_NO_DEBUG boolean operator == (const TMatrix44& m)const							{ return this->x == m.x && this->y == m.y && this->z == m.z && w == m.w; }
	INLINE_NO_DEBUG boolean operator != (const TMatrix44& m)const							{ return this->x != m.x || this->y != m.y || this->z != m.z || w != m.w; }

	INLINE_NO_DEBUG Scalar_t& operator () (int nRow, int nColumn)							{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Scalar_t  operator () (int nRow, int nColumn) const						{ return (&x)[nColumn].a[nRow]; }
	INLINE_NO_DEBUG Vector_t& operator [] (int index)										{ return (&x)[index]; }
	INLINE_NO_DEBUG const Vector_t& operator [] (int index) const							{ return (&x)[index]; }	

	friend INLINE_NO_DEBUG Vector_t operator * (const TMatrix44& m, const Vector_t& v)
	{
		return m.x * v.x + m.y * v.y + m.z * v.z + m.w * v.w;
	}

	INLINE_NO_DEBUG Vector_t tranMult(const Vector_t& v)const
	{
		return Vector_t(x.dot(v), y.dot(v), z.dot(v), w.dot(v));
	}

	friend INLINE_NO_DEBUG TMatrix44 operator * (const TMatrix44& a, const TMatrix44& b)
	{
		TMatrix44 ret;
		ret.x = a * b.x;
		ret.y = a * b.y;
		ret.z = a * b.z;
		ret.w = a * b.w;
		return ret;
	}

	friend INLINE_NO_DEBUG TMatrix44 operator + (const TMatrix44& a, const TMatrix44& b)	{ return TMatrix44(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
	friend INLINE_NO_DEBUG TMatrix44 operator - (const TMatrix44& a, const TMatrix44& b)	{ return TMatrix44(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix44 operator * (const t_OtherScalar& f, const TMatrix44& m){ ScalarOptimal_t F(f); return TMatrix44(m.x * F, m.y * F, m.z * F, m.w * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix44 operator * (const TMatrix44& m, const t_OtherScalar& f){ ScalarOptimal_t F(f); return TMatrix44(m.x * F, m.y * F, m.z * F, m.w * F); }
	template<class t_OtherScalar>
	friend INLINE_NO_DEBUG TMatrix44 operator / (const TMatrix44& m, const t_OtherScalar& f){ ScalarOptimal_t F(rcp(f)); return TMatrix44(m.x * F, m.y * F, m.z * F, m.w * F); }

	INLINE_NO_DEBUG TMatrix44 operator + () const											{ return *this; }
	INLINE_NO_DEBUG TMatrix44 operator - () const											{ return TMatrix44(-x, -y, -z, -w); }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator *= (const t_OtherScalar& f)							{ ScalarOptimal_t F(f); x *= F; y *= F; z *= F; w *= F; }
	template<class t_OtherScalar>
	INLINE_NO_DEBUG void operator /= (const t_OtherScalar& f)							{ ScalarOptimal_t F(rcp(f)); x *= F; y *= F; z *= F; w *= F; }
	INLINE_NO_DEBUG void operator += (const TMatrix44& m)									{ x += m.x; y += m.y; z += m.z; w += m.w; }
	INLINE_NO_DEBUG void operator -= (const TMatrix44& m)									{ x -= m.x; y -= m.y; z -= m.z; w -= m.w; }
	INLINE_NO_DEBUG void operator *= (const TMatrix44& m)									{ *this = *this * m; }

	INLINE_NO_DEBUG TMatrix44 transpose()const
	{
		TMatrix44 ret;
		transpose_impl(ret.x, ret.y, ret.z, ret.w, x, y, z, w);
		return ret;
	}

	INLINE_NO_DEBUG TMatrix44 invertForOrthogonal()const
	{
		TMatrix44 ret = transpose();
		Vector_t k = rcp(Vector_t(x.lenghtSquare(), y.lenghtSquare(), z.lenghtSquare(), w.lenghtSquare()));
		ret.x *= k;
		ret.y *= k;
		ret.z *= k;
		ret.w *= k;
		return ret;
	}

	INLINE_NO_DEBUG TMatrix44 invert()const
	{
		TMatrix44 M;

		M.x[0] = det3x3(
			y.a[1],z.a[1],w.a[1],
			y.a[2],z.a[2],w.a[2],
			y.a[3],z.a[3],w.a[3]);
		M.y[0] = -det3x3(
			y.a[0],z.a[0],w.a[0],
			y.a[2],z.a[2],w.a[2],
			y.a[3],z.a[3],w.a[3]);
		M.z[0] = det3x3(
			y.a[0],z.a[0],w.a[0],
			y.a[1],z.a[1],w.a[1],
			y.a[3],z.a[3],w.a[3]);
		M.w[0] = -det3x3(
			y.a[0],z.a[0],w.a[0],
			y.a[1],z.a[1],w.a[1],
			y.a[2],z.a[2],w.a[2]);

		M.x[1] = -det3x3(
			x.a[1],z.a[1],w.a[1],
			x.a[2],z.a[2],w.a[2],
			x.a[3],z.a[3],w.a[3]);
		M.y[1] = det3x3(
			x.a[0],z.a[0],w.a[0],
			x.a[2],z.a[2],w.a[2],
			x.a[3],z.a[3],w.a[3]);
		M.z[1] = -det3x3(
			x.a[0],z.a[0],w.a[0],
			x.a[1],z.a[1],w.a[1],
			x.a[3],z.a[3],w.a[3]);
		M.w[1] = det3x3(
			x.a[0],z.a[0],w.a[0],
			x.a[1],z.a[1],w.a[1],
			x.a[2],z.a[2],w.a[2]);

		M.x[2] = det3x3(
			x.a[1],y.a[1],w.a[1],
			x.a[2],y.a[2],w.a[2],
			x.a[3],y.a[3],w.a[3]);
		M.y[2] = -det3x3(
			x.a[0],y.a[0],w.a[0],
			x.a[2],y.a[2],w.a[2],
			x.a[3],y.a[3],w.a[3]);
		M.z[2] = det3x3(
			x.a[0],y.a[0],w.a[0],
			x.a[1],y.a[1],w.a[1],
			x.a[3],y.a[3],w.a[3]);
		M.w[2] = -det3x3(
			x.a[0],y.a[0],w.a[0],
			x.a[1],y.a[1],w.a[1],
			x.a[2],y.a[2],w.a[2]);

		M.x[3] = -det3x3(
			x.a[1],y.a[1],z.a[1],
			x.a[2],y.a[2],z.a[2],
			x.a[3],y.a[3],z.a[3]);
		M.y[3] = det3x3(
			x.a[0],y.a[0],z.a[0],
			x.a[2],y.a[2],z.a[2],
			x.a[3],y.a[3],z.a[3]);
		M.z[3] = -det3x3(
			x.a[0],y.a[0],z.a[0],
			x.a[1],y.a[1],z.a[1],
			x.a[3],y.a[3],z.a[3]);
		M.w[3] = det3x3(
			x.a[0],y.a[0],z.a[0],
			x.a[1],y.a[1],z.a[1],
			x.a[2],y.a[2],z.a[2]);

		Scalar_t det = x.a[0]*M.x[0] + x.a[1]*M.y[0] + x.a[2]*M.z[0] + x.a[3]*M.w[0];
		return M / det;
	}

	INLINE_NO_DEBUG Scalar_t determinant()const
	{
		return det4x4(	x.a[0], y.a[0], z.a[0], w.a[0],
						x.a[1], y.a[1], z.a[1], w.a[1],
						x.a[2], y.a[2], z.a[2], w.a[2],
						x.a[3], y.a[3], z.a[3], w.a[3]);
	}

	static INLINE_NO_DEBUG TMatrix44 makeProjectiveMatrix(const TMatrix34<Scalar_t>& other)
	{
		return TMatrix44(other.x.xyz0(), other.y.xyz0(), other.z.xyz0(), other.p.xyz1());
	}

	INLINE_NO_DEBUG TMatrix44 postScale(const Vector_t& s)const
	{
		return TMatrix44(x * s, y * s, z * s, w * s);
	}

	INLINE_NO_DEBUG TMatrix44 preScale(const Vector_t& s)const
	{
		return TMatrix44(x * s.x, y * s.y, z * s.z, w * s.w);
	}

	INLINE_NO_DEBUG void setRow(uint n, const Vector_t& v)
	{
		(*this)(n, 0) = v.x;
		(*this)(n, 1) = v.y;
		(*this)(n, 2) = v.z;
		(*this)(n, 3) = v.w;
	}
};

template<class t_Scalar>
static INLINE_NO_DEBUG boolean equals(const TMatrix44<t_Scalar>& m, const TMatrix44<t_Scalar>& n, typename TMatrix44<t_Scalar>::Scalar_t eps)
{
	return equals(m.x, n.x, eps) && equals(m.y, n.y, eps) && equals(m.z, n.z, eps) && equals(m.w, n.w, eps);
}

template<class t_Scalar>
static INLINE_NO_DEBUG boolean isFinite(const TMatrix44<t_Scalar>& m) 
{
	return isFinite(m.x) && isFinite(m.y) && isFinite(m.z) && isFinite(m.w);
}


template<class t_Scalar>
struct Matrix22T
{
	typedef TMatrix22< typename RealScalarT<t_Scalar>::Type > Type;
};

template<class t_Scalar>
struct Matrix23T
{
	typedef TMatrix23< typename RealScalarT<t_Scalar>::Type > Type;
};

template<class t_Scalar>
struct Matrix33T
{
	typedef TMatrix33< typename RealScalarT<t_Scalar>::Type > Type;
};

template<class t_Scalar>
struct Matrix34T
{
	typedef TMatrix34< typename RealScalarT<t_Scalar>::Type > Type;
};

template<class t_Scalar>
struct Matrix44T
{
	typedef TMatrix44< typename RealScalarT<t_Scalar>::Type > Type;
};



typedef TMatrix22<float> Matrix22f;
typedef TMatrix23<float> Matrix23f;
typedef TMatrix33<float> Matrix33f;
typedef TMatrix34<float> Matrix34f;
typedef TMatrix44<float> Matrix44f;

typedef TMatrix22<double> Matrix22d;
typedef TMatrix23<double> Matrix23d;
typedef TMatrix33<double> Matrix33d;
typedef TMatrix34<double> Matrix34d;
typedef TMatrix44<double> Matrix44d;

}
