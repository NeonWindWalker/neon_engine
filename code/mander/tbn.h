#pragma once

#include "../base/include.h"

namespace Mander
{

USING_BASE_LIB


static INLINE void triangleNormalCcwScaledBySurface(float * n, const float * A, const float * B, const float * C)
{
	float ab[3] = {B[0]-A[0],B[1]-A[1],B[2]-A[2]};
	float ac[3] = {C[0]-A[0],C[1]-A[1],C[2]-A[2]};
	n[0] = ab[1]*ac[2] - ab[2]*ac[1];
	n[1] = ab[2]*ac[0] - ab[0]*ac[2];
	n[2] = ab[0]*ac[1] - ab[1]*ac[0];
}

static INLINE void triangleTangentBinormalScaledBySurface(float * vTangent, float * vBinormal, float& fSur, const float * A, const float * B, const float * C, const float * TA, const float * TB, const float * TC)
{
	float ab[3] = {B[0]-A[0],B[1]-A[1],B[2]-A[2]};
	float ac[3] = {C[0]-A[0],C[1]-A[1],C[2]-A[2]};
	float tab[2] = {TB[0]-TA[0],TB[1]-TA[1]};
	float tac[2] = {TC[0]-TA[0],TC[1]-TA[1]};

	float X[3] = {tac[1]*ab[0] - tab[1]*ac[0], tac[1]*ab[1] - tab[1]*ac[1], tac[1]*ab[2] - tab[1]*ac[2]};
	float Y[3] = {tac[0]*ab[0] - tab[0]*ac[0], tac[0]*ab[1] - tab[0]*ac[1], tac[0]*ab[2] - tab[0]*ac[2]};
	float fX = 1.0f/(tab[0]*tac[1] - tab[1]*tac[0]);
	float fY = 1.0f/(tab[1]*tac[0] - tab[0]*tac[1]);
	X[0] *= fX; X[1] *= fX; X[2] *= fX;
	Y[0] *= fY; Y[1] *= fY; Y[2] *= fY;

	float n[3];
	n[0] = ab[1]*ac[2] - ab[2]*ac[1];
	n[1] = ab[2]*ac[0] - ab[0]*ac[2];
	n[2] = ab[0]*ac[1] - ab[1]*ac[0];
	fSur = sqrtf(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);

	float fXq = X[0]*X[0] + X[1]*X[1] + X[2]*X[2];
	float fYq = Y[0]*Y[0] + Y[1]*Y[1] + Y[2]*Y[2];
	if(fXq > 1e+32 || fXq < 1e-32)X[0]=X[1]=X[2]=0;
	if(fYq > 1e+32 || fYq < 1e-32)Y[0]=Y[1]=Y[2]=0;

	vTangent[0] = X[0]*fSur; vTangent[1] = X[1]*fSur; vTangent[2] = X[2]*fSur;
	vBinormal[0] = Y[0]*fSur; vBinormal[1] = Y[1]*fSur; vBinormal[2] = Y[2]*fSur;
}


static INLINE void triangleNormalCcwScaledBySurface(Vector3f& n, const Vector3f& A, const Vector3f& B, const Vector3f& C)
{
	triangleNormalCcwScaledBySurface(n.a, A.a, B.a, C.a);
}

static INLINE void triangleTangentBinormalScaledBySurface(Vector3f& tangent, Vector3f& binormal, float& surface, const Vector3f& A, const Vector3f& B, const Vector3f& C, const Vector2f& TA, const Vector2f& TB, const Vector2f& TC)
{
	triangleTangentBinormalScaledBySurface(tangent.a, binormal.a, surface, A.a, B.a, C.a, TA.a, TB.a, TC.a);
}

struct TangentAndBinormal
{
	Vector3f tangent;
	Vector3f binormal;
};

template<class t_Operator, class t_Vert>
static inline void calcTangentAndBinormal(t_Operator& op,  t_Vert *aVert, uint vertexCount, const uint32* aIndex, uint trianglesCount)
{
	TangentAndBinormal* aVertexTB = new TangentAndBinormal[vertexCount];
	for(uint tr=0; tr < trianglesCount; tr++)
	{
		uint iA = aIndex[tr*3 + 0];
		uint iB = aIndex[tr*3 + 1];
		uint iC = aIndex[tr*3 + 2];
		Vector3f vA = op.getPosition(aVert[iA]);
		Vector3f vB = op.getPosition(aVert[iB]);
		Vector3f vC = op.getPosition(aVert[iC]);
		Vector2f tA = op.getTexCoord(aVert[iA]);
		Vector2f tB = op.getTexCoord(aVert[iB]);
		Vector2f tC = op.getTexCoord(aVert[iC]);

		Vector3f tangent, binormal;
		float s;
		triangleTangentBinormalScaledBySurface(tangent, binormal, s, vA, vB, vC, tA, tB, tC);
		aVertexTB[iA].tangent += tangent;
		aVertexTB[iB].tangent += tangent;
		aVertexTB[iC].tangent += tangent;
		aVertexTB[iA].binormal += binormal;
		aVertexTB[iB].binormal += binormal;
		aVertexTB[iC].binormal += binormal;
	}

	for(uint i=0; i < vertexCount; ++i)
	{
		if(aVertexTB[i].tangent.lenghtSquare() > 1e-32f)
			op.setTangent(aVert[i], aVertexTB[i].tangent.normal());
		if(aVertexTB[i].binormal.lenghtSquare() > 1e-32f)
			op.setBinormal(aVert[i], aVertexTB[i].binormal.normal());
	}
	delete aVertexTB;
}

}