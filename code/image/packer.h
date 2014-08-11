#include "color.h"

namespace ImageLib
{

struct PackerSourceBlockRGB
{
	typedef Vector3f Color_t;
	Vector3f color[4][4];
	float discreteWeightsTable[8];
	uint discreteWeightsTableCount;
	uint colorBits[3];
};

template<class t_Dst, class t_Src>
float packBlockFromSourceBlock(t_Dst& dst, const t_Src& src) //return error
{
	typename t_Src::Color_t midddleColor = zero<typename t_Src::Color_t>();
	float middleWeight = 0;
	for(uint n=0; n < 16; ++n)
	{
		middleColor += src.color[0][n] * w;
		middleWeight += 1.0f;
	}
	middleColor /= w;

	typename t_Src::Color_t bestAxis;
	float bestAxisCoef = -1;
	for(uint n=0; n < 16; ++n)
	{
		typename t_Src::Color_t axis = color[0][n].normal();
		float l = 0;
		for(uint i=0; i < 16; ++i)
			l += abs(axis.dot(color[0][i]));
		if(bestAxisCoef < l){
			bestAxisCoef = l;
			bestAxis = axis;
		}
	}




}

}


