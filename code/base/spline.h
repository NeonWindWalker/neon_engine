#include "matrix.h"
#include "range.h"

namespace Base
{
    
struct SplineMatrixesCollection
{
	Matrix44f gcfCubicSplineCoefMatrix;
	Matrix44f gcfCubicBSplineCoefMatrix;
	Matrix44f gcfCubicBezierSplineCoefMatrix;
	Matrix44f gcfCubicSplineDerivataCoefMatrix;
	Matrix44f gcfCubicBSplineDerivataCoefMatrix;
	Matrix44f gcfCubicBezierSplineDerivataCoefMatrix;
	Matrix44f gcfCubicSplineDerivata2CoefMatrix;
	Matrix44f gcfCubicBSplineDerivata2CoefMatrix;
	Matrix44f gcfCubicBezierSplineDerivata2CoefMatrix;

	SplineMatrixesCollection()
	{
		gcfCubicSplineCoefMatrix.setRow(0, Vector4f(-0.5f,  1.0f, -0.5f,  0.0f));
		gcfCubicSplineCoefMatrix.setRow(1, Vector4f( 1.5f, -2.5f,  0.0f,  1.0f));
		gcfCubicSplineCoefMatrix.setRow(2, Vector4f(-1.5f,  2.0f,  0.5f,  0.0f));
		gcfCubicSplineCoefMatrix.setRow(3, Vector4f( 0.5f, -0.5f,  0.0f,  0.0f));

		gcfCubicBSplineCoefMatrix.setRow(0, Vector4f(-1.0f,  3.0f, -3.0f,  1.0f) / 6.0f);
		gcfCubicBSplineCoefMatrix.setRow(1, Vector4f( 3.0f, -6.0f,  0.0f,  4.0f) / 6.0f);
		gcfCubicBSplineCoefMatrix.setRow(2, Vector4f(-3.0f,  3.0f,  3.0f,  1.0f) / 6.0f);
		gcfCubicBSplineCoefMatrix.setRow(3, Vector4f( 1.0f,  0.0f,  0.0f,  0.0f) / 6.0f);

		gcfCubicBezierSplineCoefMatrix.setRow(0, Vector4f(-1.0f,  3.0f, -3.0f,  1.0f));
		gcfCubicBezierSplineCoefMatrix.setRow(1, Vector4f( 3.0f, -6.0f,  3.0f,  0.0f));
		gcfCubicBezierSplineCoefMatrix.setRow(2, Vector4f(-3.0f,  3.0f,  0.0f,  0.0f));
		gcfCubicBezierSplineCoefMatrix.setRow(3, Vector4f( 1.0f,  0.0f,  0.0f,  0.0f));

		//Vector4f xxx = gcfCubicBezierSplineCoefMatrix * Vector4f(0.5f*0.5f*0.5f, 0.5f * 0.5f, 0.5f, 1.0f);

		/*gcfCubicSplineDerivataCoefMatrix.setRow(0, Vector4f(0.0f, -1.5f,  2.0f, -0.5f));
		 gcfCubicSplineDerivataCoefMatrix.setRow(1, Vector4f(0.0f,  4.5f, -5.0f,  0.0f));
		 gcfCubicSplineDerivataCoefMatrix.setRow(2, Vector4f(0.0f, -4.5f,  4.0f,  0.5f));
		 gcfCubicSplineDerivataCoefMatrix.setRow(3, Vector4f(0.0f,  1.5f, -1.0f,  0.0f));*/

		gcfCubicSplineDerivataCoefMatrix.x.setZero();
		gcfCubicSplineDerivataCoefMatrix.y = gcfCubicSplineCoefMatrix.x * 3.0f;
		gcfCubicSplineDerivataCoefMatrix.z = gcfCubicSplineCoefMatrix.y * 2.0f;
		gcfCubicSplineDerivataCoefMatrix.w = gcfCubicSplineCoefMatrix.z;

		gcfCubicBSplineDerivataCoefMatrix.x.setZero();
		gcfCubicBSplineDerivataCoefMatrix.y = gcfCubicBSplineCoefMatrix.x * 3.0f;
		gcfCubicBSplineDerivataCoefMatrix.z = gcfCubicBSplineCoefMatrix.y * 2.0f;
		gcfCubicBSplineDerivataCoefMatrix.w = gcfCubicBSplineCoefMatrix.z;

		gcfCubicBezierSplineDerivataCoefMatrix.x.setZero();
		gcfCubicBezierSplineDerivataCoefMatrix.y = gcfCubicBezierSplineCoefMatrix.x * 3.0f;
		gcfCubicBezierSplineDerivataCoefMatrix.z = gcfCubicBezierSplineCoefMatrix.y * 2.0f;
		gcfCubicBezierSplineDerivataCoefMatrix.w = gcfCubicBezierSplineCoefMatrix.z;

		gcfCubicSplineDerivata2CoefMatrix.x.setZero();
		gcfCubicSplineDerivata2CoefMatrix.y.setZero();
		gcfCubicSplineDerivata2CoefMatrix.z = gcfCubicSplineDerivataCoefMatrix.y * 2.0f;
		gcfCubicSplineDerivata2CoefMatrix.w = gcfCubicSplineDerivataCoefMatrix.z;

		gcfCubicBSplineDerivata2CoefMatrix.x.setZero();
		gcfCubicBSplineDerivata2CoefMatrix.y.setZero();
		gcfCubicBSplineDerivata2CoefMatrix.z = gcfCubicBSplineDerivataCoefMatrix.y * 2.0f;
		gcfCubicBSplineDerivata2CoefMatrix.w = gcfCubicBSplineDerivataCoefMatrix.z;

		gcfCubicBezierSplineDerivata2CoefMatrix.x.setZero();
		gcfCubicBezierSplineDerivata2CoefMatrix.y.setZero();
		gcfCubicBezierSplineDerivata2CoefMatrix.z = gcfCubicBezierSplineDerivataCoefMatrix.y * 2.0f;
		gcfCubicBezierSplineDerivata2CoefMatrix.w = gcfCubicBezierSplineDerivataCoefMatrix.z;
	}
};

extern byte gSplineMatrixesCollectionMemForFixAligningInCLANG[sizeof(SplineMatrixesCollection) + 32];

static INLINE SplineMatrixesCollection* gSplineMatrixesCollection()
{
	return reinterpret_cast<SplineMatrixesCollection*>((uinteger(gSplineMatrixesCollectionMemForFixAligningInCLANG) + 32) & (~uinteger(31)));
}

template<class T>
struct TSplineMatrixesCollection;

template<>
struct TSplineMatrixesCollection<float>
{
	static INLINE_NO_DEBUG const Matrix44f& getCubicSplineCoefMatrix()					{ return gSplineMatrixesCollection()->gcfCubicSplineCoefMatrix; }
	static INLINE_NO_DEBUG const Matrix44f& getCubicBSplineCoefMatrix()					{ return gSplineMatrixesCollection()->gcfCubicBSplineCoefMatrix; }
	static INLINE_NO_DEBUG const Matrix44f& getCubicBezierSplineCoefMatrix()			{ return gSplineMatrixesCollection()->gcfCubicBezierSplineCoefMatrix; }
	static INLINE_NO_DEBUG const Matrix44f& getCubicSplineDerivataCoefMatrix()			{ return gSplineMatrixesCollection()->gcfCubicSplineDerivataCoefMatrix; }
	static INLINE_NO_DEBUG const Matrix44f& getCubicBSplineDerivataCoefMatrix()			{ return gSplineMatrixesCollection()->gcfCubicBSplineDerivataCoefMatrix; }
	static INLINE_NO_DEBUG const Matrix44f& getCubicBezierSplineDerivataCoefMatrix()	{ return gSplineMatrixesCollection()->gcfCubicBezierSplineDerivataCoefMatrix; }
	static INLINE_NO_DEBUG const Matrix44f& getCubicSplineDerivata2CoefMatrix()			{ return gSplineMatrixesCollection()->gcfCubicSplineDerivata2CoefMatrix; }
	static INLINE_NO_DEBUG const Matrix44f& getCubicBSplineDerivata2CoefMatrix()		{ return gSplineMatrixesCollection()->gcfCubicBSplineDerivata2CoefMatrix; }
	static INLINE_NO_DEBUG const Matrix44f& getCubicBezierSplineDerivata2CoefMatrix()	{ return gSplineMatrixesCollection()->gcfCubicBezierSplineDerivata2CoefMatrix; }
};



template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type splineTayVector(const t_ScalarQ& tay)
{
	typename Vector4T<t_ScalarQ>::Type::ScalarOptimal_t taysq = tay * tay;
	return typename Vector4T<t_ScalarQ>::Type(taysq*tay, taysq, tay, typename Vector4T<t_ScalarQ>::Type::ScalarOptimal_t(1));
}


template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicSplineCoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicSplineCoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicSplineCoefficients(const t_ScalarQ& tay)
{
	return cubicSplineCoefficientsV(splineTayVector(tay));
}

template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicSplineDerivataCoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicSplineDerivataCoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicSplineDerivataCoefficients(const t_ScalarQ& tay)
{
	return cubicSplineDerivataCoefficientsV(splineTayVector(tay));
}

template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicSplineDerivata2CoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicSplineDerivata2CoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicSplineDerivata2Coefficients(const t_ScalarQ& tay)
{
	return cubicSplineDerivata2CoefficientsV(splineTayVector(tay));
}




//BSpline
template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicBSplineCoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicBSplineCoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicBSplineCoefficients(const t_ScalarQ& tay)
{
	return cubicBSplineCoefficientsV(splineTayVector(tay));
}

template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicBSplineDerivataCoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicBSplineDerivataCoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicBSplineDerivataCoefficients(const t_ScalarQ& tay)
{
	return cubicBSplineDerivataCoefficientsV(splineTayVector(tay));
}

template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicBSplineDerivata2CoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicBSplineDerivata2CoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicBSplineDerivata2Coefficients(const t_ScalarQ& tay)
{
	return cubicBSplineDerivata2CoefficientsV(splineTayVector(tay));
}



//BezierSpline
template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicBezierSplineCoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicBezierSplineCoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicBezierSplineCoefficients(const t_ScalarQ& tay)
{
	return cubicBezierSplineCoefficientsV(splineTayVector(tay));
}

template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicBezierSplineDerivataCoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicBezierSplineDerivataCoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicBezierSplineDerivataCoefficients(const t_ScalarQ& tay)
{
	return cubicBezierSplineDerivataCoefficientsV(splineTayVector(tay));
}

template<class t_Scalar>
static INLINE TVector4<t_Scalar> cubicBezierSplineDerivata2CoefficientsV(const TVector4<t_Scalar>& tays)
{
	return TSplineMatrixesCollection<t_Scalar>::getCubicBezierSplineDerivata2CoefMatrix() * tays;
}

template<class t_ScalarQ>
static INLINE typename Vector4T<t_ScalarQ>::Type cubicBezierSplineDerivata2Coefficients(const t_ScalarQ& tay)
{
	return cubicBezierSplineDerivata2CoefficientsV(splineTayVector(tay));
}



template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicSplineVertexV(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicSplineCoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicSplineVertex(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicSplineVertexV(V_1, V0, V1, V2, splineTayVector(tay));
}

template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicSplineDerivataV(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicSplineDerivataCoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicSplineDerivata(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicSplineDerivataV(V_1, V0, V1, V2, splineTayVector(tay));
}

template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicSplineDerivata2V(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicSplineDerivata2CoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicSplineDerivata2(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicSplineDerivata2V(V_1, V0, V1, V2, splineTayVector(tay));
}




template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicBSplineVertexV(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicBSplineCoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicBSplineVertex(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicBSplineVertexV(V_1, V0, V1, V2, splineTayVector(tay));
}

template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicBSplineDerivataV(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicBSplineDerivataCoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicBSplineDerivata(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicBSplineDerivataV(V_1, V0, V1, V2, splineTayVector(tay));
}

template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicBSplineDerivata2V(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicBSplineDerivata2CoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicBSplineDerivata2(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicBSplineDerivata2V(V_1, V0, V1, V2, splineTayVector(tay));
}


template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicBezierSplineVertexV(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicBezierSplineCoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicBezierSplineVertex(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicBezierSplineVertexV(V_1, V0, V1, V2, splineTayVector(tay));
}

template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicBezierSplineDerivataV(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicBezierSplineDerivataCoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicBezierSplineDerivata(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicBezierSplineDerivataV(V_1, V0, V1, V2, splineTayVector(tay));
}

template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicBezierSplineDerivata2V(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicBezierSplineDerivata2CoefMatrix() * tays;
	return V_1 * kv.x + V0 * kv.y + V1 * kv.z + V2 * kv.w;
}

template<class t_Vertex, class t_ScalarQ>
static INLINE t_Vertex cubicBezierSplineDerivata2(const t_Vertex& V_1, const t_Vertex& V0, const t_Vertex& V1, const t_Vertex& V2, const t_ScalarQ& tay)
{
	return cubicBezierSplineDerivata2V(V_1, V0, V1, V2, splineTayVector(tay));
}




template<class t_Vertex, class t_Scalar>
static INLINE t_Vertex cubicRationalBezierSplineVertexV(const t_Vertex& V0, const t_Vertex& VT0, const t_Vertex& VT1, const t_Vertex& V1, const TVector4<t_Scalar>& weights, const TVector4<t_Scalar>& tays)
{
	TVector4<t_Scalar> kv = TSplineMatrixesCollection<t_Scalar>::getCubicBezierSplineCoefMatrix() * tays;
	kv /= weights.dot(kv);
	kv *= weights;
	return V0 * kv[0] + VT0 * kv[1] + VT1 * kv[2] + V1 * kv[3];
}



template<class t_Vertex, class t_ScalarQ>
static INLINE void cubicMySplineVertex(t_Vertex& outV, const t_Vertex& V0, const t_Vertex& T0, const t_ScalarQ& W0, const t_Vertex& V1, const t_Vertex& T1,  const t_ScalarQ& W1, const t_ScalarQ& tay)
{
	typename Vector4T<t_ScalarQ>::Type kv = TSplineMatrixesCollection<t_ScalarQ>::getCubicBezierSplineCoefMatrix() * splineTayVector(tay);
	typedef typename Vector4T<t_ScalarQ>::Type::Scalar_t Scalar;

	t_Vertex TN = Scalar(0.3333) * (V1 - V0);
	t_Vertex D0 = T0 - TN;
	t_Vertex D1 = T1 - TN;
	Scalar w0 = W0 * W0 / (W0 * W0 + tay + Scalar(1e-12));
	Scalar w1 = W1 * W1 / (W1 * W1 + (Scalar(1.0) - tay) + Scalar(1e-12));
	t_Vertex VT0 = V0 + TN + w0 * D0;
	t_Vertex VT1 = V1 - TN - w1 * D1;

	outV = V0 * kv[0] + VT0 * kv[1] + VT1 * kv[2] + V1 * kv[3];

}

template<class t_Vertex>
static INLINE void kolyaSplineCoefs(t_Vertex& A, t_Vertex& B, t_Vertex& C, t_Vertex& D, const t_Vertex& x, const t_Vertex& k)
{
	D = k;
	C = - k * x * 0.5f;
	A = 2.0f * k - k * x * 0.5f;
	B = - A - C - D;
}

template<class t_Vertex>
static INLINE void kolyaSplineValue(t_Vertex& outV, const t_Vertex& V0, const t_Vertex& T0x, const t_Vertex& T0y, const t_Vertex& V1, const t_Vertex& T1x, const t_Vertex& T1y, typename t_Vertex::Scalar_t tay)
{
	t_Vertex nk = V1 - V0;

	t_Vertex A0,B0,C0,D0;
	kolyaSplineCoefs(A0,B0,C0,D0, T0x, T0y / T0x - nk);

	float v1 = A0 + B0 + C0 + D0;
	float dv1 = 4.0f * A0 + 3.0f * B0 + 2.0f * C0 + D0;
	float dvm = 4.0f * A0 * T0x * T0x * T0x + 3.0f * B0 * T0x * T0x + 2.0f * C0 * T0x + D0;
	

	t_Vertex A1,B1,C1,D1;
	kolyaSplineCoefs(A1,B1,C1,D1, -T1x, nk - T1y / T1x);

	float invTay = 1.0f - tay;

	float tay2 = tay * tay;
	float tay3 = tay2 * tay;
	float tay4 = tay2 * tay2;

	float invTay2 = invTay * invTay;
	float invTay3 = invTay2 * invTay;
	float invTay4 = invTay2 * invTay2;

	outV = V0 * invTay + V1 * tay   +   A0 * tay4 + B0 * tay3 + C0 * tay2 + D0 * tay  +   A1 * invTay4 + B1 * invTay3 + C1 * invTay2 + D1 * invTay;

}


template<class t_ValueAndTime>
typename t_ValueAndTime::Value_t getLinearTableValue(uint& cashIndex, ConstRange<t_ValueAndTime> table, typename t_ValueAndTime::Time_t time)
{
	Assert(table.size());
	if(table.size() == 1)
		return table[0].getValue();
	
	for(uint i = cashIndex; i + 1 < table.size(); ++i)
	{
		typename t_ValueAndTime::Time_t t0 = table[i].getTime();
		typename t_ValueAndTime::Time_t t1 = table[i+1].getTime();
		if(t0 <= time && time < t1)
		{
			typename t_ValueAndTime::Value_t v0 = table[i].getValue();
			return v0 + (table[i+1].getValue() - v0) * table[i].timeDivision(time - t0, t1 - t0);
		}
		if(t0 > time)
			break;
	}

	cashIndex = 0;
	for(uint i = cashIndex; i + 1 < table.size(); ++i)
	{
		typename t_ValueAndTime::Time_t t0 = table[i].getTime();
		typename t_ValueAndTime::Time_t t1 = table[i+1].getTime();
		if(t0 <= time && time < t1)
		{
			typename t_ValueAndTime::Value_t v0 = table[i].getValue();
			return v0 + (table[i+1].getValue() - v0) * table[i].timeDivision(time - t0, t1 - t0);
		}
	}

	if(table[0].getTime() >= time)
		return table[0].getValue();
	else
		return table.last()->getValue();
}





}
