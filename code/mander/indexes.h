#pragma once

#include "../base/array.h"
#include "../base/scalar.h"

namespace Mander
{

USING_BASE_LIB

static const uint32 InvalidIndex = 0xffffffff;


template<class t_Data, class uint32>
static inline void remapData(t_Data* pDest, const t_Data* pSrcData, const uint32* pRemap, const uint nCount)
{
	if(0 == pDest || 0 == pSrcData || 0 == pRemap || 0 == nCount)return;
	const uint32 * r = pRemap;
	for(t_Data *d = pDest, *d_e = pDest + nCount; d < d_e; d++, r++)
		*d = pSrcData[*r];
}

template<class t_Data, class uint32>
static inline void invremapData(t_Data * pDest, const uint32* pInvRemap, const t_Data* pSrcData, const uint nCount)
{
	if(0 == pDest || 0 == pInvRemap || 0 == pSrcData || 0 == nCount)return;
	const uint32 * r = pInvRemap;
	for(const t_Data *s = pSrcData, *s_e = pSrcData + nCount; s < s_e; s++, r++)
		pDest[*r] = *s;
}
template<class t_Data, class uint32>
static inline void remapDataSafe(t_Data* pDest, const t_Data* pSrcData, const uint32* pRemap, const uint nCount)
{
	if(0 == pDest || 0 == pSrcData || 0 == pRemap || 0 == nCount)return;
	const uint32 * r = pRemap;
	for(t_Data *d = pDest, *d_e = pDest + nCount; d < d_e; d++, r++)
		if(InvalidIndex != *r)
			*d = pSrcData[*r];
}

template<class t_Data, class uint32>
static inline void invremapDataSafe(t_Data* pDest, const uint32* pInvRemap, const t_Data* pSrcData, const uint nCount)
{
	if(0 == pDest || 0 == pInvRemap || 0 == pSrcData || 0 == nCount)return;
	const uint32 * r = pInvRemap;
	for(const t_Data *s = pSrcData, *s_e = pSrcData + nCount; s < s_e; s++, r++)
		if(InvalidIndex != *r)
			pDest[*r] = *s;
}

template<class t_Vertex, class t_Compare>
boolean convertToIndexed(t_Vertex*& outaVert, uint& outVertCount, uint32*& outaInd, const t_Vertex* aVert, uint vertexCount, t_Compare cmp)
{
	Assert(aVert && vertexCount);

	struct VertexAndNumber : public t_Vertex
	{
		uint _convertToIndexed_n;
		uint _convertToIndexed_uncopyedN;
		VertexAndNumber(){}
		~VertexAndNumber(){}
	};

	VertexAndNumber* av = new VertexAndNumber[vertexCount];
	for(uint i=0; i < vertexCount; i++){
		*(t_Vertex*)&av[i] = aVert[i];
		av[i]._convertToIndexed_n = i;
	}
	sort(av, vertexCount, cmp);

	av[0]._convertToIndexed_uncopyedN = 0;
	for(uint i=1; i < vertexCount; i++){
		if(cmp(av[i-1], av[i]) == 0)
			av[i]._convertToIndexed_uncopyedN = av[i - 1]._convertToIndexed_uncopyedN;
		else
			av[i]._convertToIndexed_uncopyedN = av[i - 1]._convertToIndexed_uncopyedN + 1;
	}

	uint* an = new uint[vertexCount];
	for(uint i=0; i < vertexCount; i++)
		an[av[i]._convertToIndexed_n] = i;

	outVertCount = av[vertexCount - 1]._convertToIndexed_uncopyedN + 1;
	outaVert = new t_Vertex[outVertCount];
	outaInd = new uint32[vertexCount];
	for(uint i=0; i < vertexCount; ++i)
		outaInd[i] = av[an[i]]._convertToIndexed_uncopyedN;
	for(uint i=0; i < vertexCount; ++i)
		outaVert[av[i]._convertToIndexed_uncopyedN] = *(t_Vertex*)&av[i];

	delete[] an;
	delete[] av;
	return true;
}

struct Edge
{
	union {
		struct { uint32 a,b; };
		uint64 e;
	};
	uint32 tr;
};

class EdgeCompare
{
public:
	INLINE int operator () (const Edge& a, const Edge& b)
	{
		if(a.e < b.e)
			return -1;
		if(a.e > b.e)
			return 1;
		return 0;
		/*
		uint64 d = a.e - b.e;
#ifdef LESS_ADDRES_GREATER_NUMBER
		return *reinterpret_cast<int32*>(&d);
#else
		return *(reinterpret_cast<int32*>(&d) + 1);
#endif*/
	}
};


static inline int32* findNearTris(const uint32* aInd, uint trisCount)
{
	int32* aN = new int32[3*trisCount];
	for(uint i=0; i < 3*trisCount; i++)
		aN[i] = -1;



	Edge* aE = new Edge[trisCount * 3];
	for(uint i=0; i < trisCount; i++)
	{
		uint32 a = aInd[i*3 + 0];
		uint32 b = aInd[i*3 + 1];
		uint32 c = aInd[i*3 + 2];
		Edge* e0 = &aE[i*3 + 0];
		Edge* e1 = &aE[i*3 + 1];
		Edge* e2 = &aE[i*3 + 2];

		e0->tr = e1->tr = e2->tr = i;

		if(a < b){ e0->a = a; e0->b = b; }
		else { e0->a = b; e0->b = a; }

		if(b < c){ e1->a = b; e1->b = c; }
		else { e1->a = c; e1->b = b; }

		if(c < a){ e2->a = c; e2->b = a; }
		else { e2->a = a; e2->b = c; }
	}

	EdgeCompare cmp;
	Base::sort<Edge, EdgeCompare>(aE, trisCount * 3, cmp);

	for(uint i=0; i < trisCount; i++)
	{
		uint32 a = aInd[i*3 + 0];
		uint32 b = aInd[i*3 + 1];
		uint32 c = aInd[i*3 + 2];

		Edge e0;
		Edge e1;
		Edge e2;

		e0.tr = e1.tr = e2.tr = i;

		if(a < b){ e0.a = a; e0.b = b; }
		else { e0.a = b; e0.b = a; }

		if(b < c){ e1.a = b; e1.b = c; }
		else { e1.a = c; e1.b = b; }

		if(c < a){ e2.a = c; e2.b = a; }
		else { e2.a = a; e2.b = c; }

		Edge* end;
		for(Edge* p = findEqualRangeInSortedArray(end, aE, trisCount * 3, e0, cmp); p != end; ++p)
			if(p->tr != i)
				aN[i*3 + 0] = p->tr;
		for(Edge* p = findEqualRangeInSortedArray(end, aE, trisCount * 3, e1, cmp); p != end; ++p)
			if(p->tr != i)
				aN[i*3 + 1] = p->tr;
		for(Edge* p = findEqualRangeInSortedArray(end, aE, trisCount * 3, e2, cmp); p != end; ++p)
			if(p->tr != i)
				aN[i*3 + 2] = p->tr;
	}
	delete[] aE;
	return aN;
};


}
