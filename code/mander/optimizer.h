#pragma once

#include "indexes.h"



namespace Mander
{

	int32* tipsify(const int32* indices,
		int nTriangles,
		int nVertices,
		int k);

USING_BASE_LIB

static inline boolean findNextStripTriangle(uint32& ret, uint32 last, const uint32* aInd, const int32* aNear, const uint32* aShadedNumber, const uint32* aShadedVertexNumber, uint32 currentLineNumber)
{
	int32 enge[3] = {aNear[last * 3 + 0], aNear[last * 3 + 1], aNear[last * 3 + 2]};
	uint prior[3] = {0,0,0};
	for(uint i=0; i<3; ++i)
		if(enge[i] >= 0 && !aShadedNumber[enge[i]])
		{
			if(aShadedVertexNumber[aInd[enge[i]*3 + 0]] == currentLineNumber - 1)
				prior[i] ++;
			if(aShadedVertexNumber[aInd[enge[i]*3 + 1]] == currentLineNumber - 1)
				prior[i] ++;
			if(aShadedVertexNumber[aInd[enge[i]*3 + 2]] == currentLineNumber - 1)
				prior[i] ++;

			if(aShadedVertexNumber[aInd[enge[i]*3 + 0]])
				prior[i] ++;
			if(aShadedVertexNumber[aInd[enge[i]*3 + 1]])
				prior[i] ++;
			if(aShadedVertexNumber[aInd[enge[i]*3 + 2]])
				prior[i] ++;
		}
	uint im = Base::indexOfMax(prior[0], prior[1], prior[2]);
	if(!prior[im])
		return false;
	ret = enge[im];
	Assert(!aShadedNumber[ret]);
	return true;
}


static inline uint32 findRootTriangle(const uint32* aInd, const int32* aNear, const uint32* aShadedNumber, const uint32* aShadedVertexNumber, const uint32* aLastLineTris, uint32 lastLineTrisCount, uint32 currentLineNumber, uint32 trisCount)
{
	for(uint32 i=0; i < lastLineTrisCount; ++i)
	{
		uint32 ret;
		if(findNextStripTriangle(ret, aLastLineTris[i], aInd, aNear, aShadedNumber, aShadedVertexNumber, currentLineNumber))
			return ret;
	}

	//bad root
	for(uint32 i=0; i < trisCount; ++i)
	{
		if(aShadedNumber[i])
			continue;
		int32 abnt = aNear[i * 3 + 0];
		int32 bcnt = aNear[i * 3 + 1];
		int32 cant = aNear[i * 3 + 2];
		
		if(abnt >= 0 && aShadedNumber[abnt])
			return i;
		if(bcnt >= 0 && aShadedNumber[bcnt])
			return i;
		if(cant >= 0 && aShadedNumber[cant])
			return i;
	}

	//very bad or first root
	for(uint32 i=0; i < trisCount; ++i)
	{
		if(aShadedNumber[i])
			continue;
		return i;
	}
	return 0;
}


static inline uint32* optimizeIndexes(const uint32* aInd, uint trisCount, uint stripLineTrisCount = 6)
{
	uint maxIndex = 0;
	for(uint i=0; i < trisCount * 3; ++i)
		maximize(maxIndex, aInd[i]);

	return (uint32*)tipsify((int32*)aInd, trisCount, maxIndex + 1, 10);

	/*
	int32* aNear = findNearTris(aInd, trisCount);
	uint32* aShadedNumber = new uint32[trisCount];
	uint32* aShadedVertexNumber = new uint32[trisCount * 3];
	for(uint i=0; i<trisCount; ++i){
		aShadedNumber[i] = 0;
		aShadedVertexNumber[i*3 + 0] = 0;
		aShadedVertexNumber[i*3 + 1] = 0;
		aShadedVertexNumber[i*3 + 2] = 0;
	}
	uint32* aDestTris = new uint32[trisCount];

	uint32* aLastLineTris = aDestTris;
	uint32 lastLineTrisCount = 0;
	uint32 line = 1;
	for(uint32 nDestTr=0; nDestTr != trisCount; line ++)
	{
		uint32 root = findRootTriangle(aInd, aNear, aShadedNumber, aShadedVertexNumber, aLastLineTris, lastLineTrisCount, line, trisCount);
		Assert(!aShadedNumber[root]);
		aDestTris[nDestTr] = root;
		aLastLineTris = &aDestTris[nDestTr];
		lastLineTrisCount = 1;
		aShadedNumber[root] = line;
		aShadedVertexNumber[aInd[root*3 + 0]] = line;
		aShadedVertexNumber[aInd[root*3 + 1]] = line;
		aShadedVertexNumber[aInd[root*3 + 2]] = line;
		nDestTr++;
		if(nDestTr == trisCount)
			break;

		uint32 last = root;
		uint32 tr;
		while(findNextStripTriangle(tr, last, aInd, aNear, aShadedNumber, aShadedVertexNumber, line) && lastLineTrisCount < stripLineTrisCount)
		{
			Assert(!aShadedNumber[tr]);
			aDestTris[nDestTr] = tr;
			lastLineTrisCount ++;
			aShadedNumber[tr] = line;
			aShadedVertexNumber[aInd[tr*3 + 0]] = line;
			aShadedVertexNumber[aInd[tr*3 + 1]] = line;
			aShadedVertexNumber[aInd[tr*3 + 2]] = line;
			nDestTr++;
			if(nDestTr == trisCount)
				break;

			last = tr;
		}
	}

	delete[] aNear;
	delete[] aShadedNumber;
	delete[] aShadedVertexNumber;

	uint32* aI = new uint32[trisCount * 3];
	for(uint i=0; i<trisCount; ++i){
		aI[i*3 + 0] = aInd[aDestTris[i]*3 + 0];
		aI[i*3 + 1] = aInd[aDestTris[i]*3 + 1];
		aI[i*3 + 2] = aInd[aDestTris[i]*3 + 2];
	}
	delete[] aDestTris;
	return aI;
	*/

	

}

static inline uint optimizeVertexes(uint32*& pNewIndex, uint32*& pVertexInvertRemap, const uint32* pI, const uint IndexCount, const uint VertexCount)
{
	if(pI == 0 || IndexCount == 0 || VertexCount == 0)
		return 0;
	pNewIndex = new uint32[IndexCount];
	pVertexInvertRemap = new uint32[VertexCount];

	for(uint n=0; n<VertexCount; n++)
		pVertexInvertRemap[n] = InvalidIndex;
	uint vert=0;
	for(uint n=0; n<IndexCount; n++)
	{
		if(pVertexInvertRemap[pI[n]] == InvalidIndex)
		{
			pNewIndex[n] = vert;
			pVertexInvertRemap[pI[n]] = vert;
			vert ++;
		}
		else
			pNewIndex[n] = pVertexInvertRemap[pI[n]];
	}
	return vert;
}


template<class t_Vertex>
static inline boolean optimizeVertexes(uint32*& pNewIndex, t_Vertex*& aOptVert, uint& nOptVertCount, const uint32 * pI, const uint nIndexCount, const t_Vertex* aVert, uint nVertexCount)
{
	if(0 == aVert || 0 == nVertexCount || 0 == nIndexCount)
		return false;
	uint32 * aInvRemap;
	nOptVertCount = optimizeVertexes(pNewIndex,aInvRemap,pI,nIndexCount,nVertexCount);
	if(0 == nOptVertCount)
		return 0;
	aOptVert = new t_Vertex[nOptVertCount];
	if(nOptVertCount == nVertexCount)
		invremapData(aOptVert, aInvRemap, aVert, nVertexCount);
	else
		invremapDataSafe(aOptVert, aInvRemap, aVert, nVertexCount);
	delete[] aInvRemap;
	return true;
}

template<class t_Index>
static inline void quadPatchOptimizedIndexesCW(t_Index * aI, uint quad_width, uint quad_height, uint optimize_quad_height = 4)
{
	if(0 == aI || 0 == quad_width || 0 == quad_height || 0 == optimize_quad_height)return;
	if(optimize_quad_height > quad_height)optimize_quad_height = quad_height;
	for(uint j=0; j < quad_height; j += optimize_quad_height)
		for(uint i=0; i < quad_width; i++)
			for(uint k=j; k < j+optimize_quad_height; k++)
			{
				int i0 = k*(quad_width+1) + i;
				*(aI++) = t_Index(i0);
				*(aI++) = t_Index(i0 + quad_width + 1);
				*(aI++) = t_Index(i0 + quad_width + 2);
				*(aI++) = t_Index(i0 + quad_width + 2);
				*(aI++) = t_Index(i0 + 1);
				*(aI++) = t_Index(i0);
			}
}

template<class t_Index>
static inline void quadPatchOptimizedIndexesCCW(t_Index * aI, uint quad_width, uint quad_height, uint optimize_quad_height = 4)
{
	if(0 == aI || 0 == quad_width || 0 == quad_height || 0 == optimize_quad_height)return;
	if(optimize_quad_height > quad_height)optimize_quad_height = quad_height;
	for(uint j=0; j < quad_height; j += optimize_quad_height)
		for(uint i=0; i < quad_width; i++)
			for(uint k=j; k < j+optimize_quad_height; k++)
			{
				int i0 = k*(quad_width+1) + i;
				*(aI++) = t_Index(i0);
				*(aI++) = t_Index(i0 + 1);
				*(aI++) = t_Index(i0 + quad_width + 2);
				*(aI++) = t_Index(i0 + quad_width + 2);
				*(aI++) = t_Index(i0 + quad_width + 1);
				*(aI++) = t_Index(i0);
			}
}

template<class t_Vertex>
static inline boolean optimize(t_Vertex*& outVert, uint& outVertexCount, uint32*& outInd, const t_Vertex* aVert, uint trisCount, uint stripLineSize = 6)
{
	if(!aVert || !trisCount)
		return false;

	t_Vertex* aV;
	uint32* aI;
	uint vertCount = 0;
	if(!convertToIndexed(aV, vertCount, aI, aVert, trisCount * 3, BinDataCompare32bitAlligned<t_Vertex, t_Vertex>() ))
		return false;

	uint32* aOptI = optimizeIndexes(aI, trisCount, stripLineSize);
	delete[] aI;

	optimizeVertexes(outInd, outVert, outVertexCount, aOptI, trisCount * 3, aV, vertCount);
	delete[] aV;
	delete[] aOptI;
	return true;

}




}