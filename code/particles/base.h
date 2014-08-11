#pragma once
#include "../base/include.h"
#include "../image/include.h"
#include "../reflection/include.h"

namespace ParticlesLib
{
USING_BASE_LIB
USING_BASE_MATH
using namespace ImageLib;

class Atlas;
class View;
class Scene;

class TexCoords
{
public:
	rl_class(ParticlesLib::TexCoords)

	rl_field(uint16, v00x)
	rl_field(uint16, v00y)
	rl_field(uint16, v01x)
	rl_field(uint16, v01y)
	rl_field(uint16, v10x)
	rl_field(uint16, v10y)
	rl_field(uint16, v11x)
	rl_field(uint16, v11y)
	rl_field(uint8, layer)

	TexCoords()
	{
		v00x = v00y = v00x = v01y = v10x = v10y = v10x = v11y = 0;
		layer = 0;
	}
};

class AnimatedImageInfo
{
public:
	rl_class(ParticlesLib::AnimatedImageInfo)

	rl_field(Array<TexCoords>, frames)

	void caclTexCoordsByProgress0N(const TexCoords*& t0, const TexCoords*& t1, float& blend, float progress)const
	{
		uint n = uintAndPFrac(blend, progress);
		t0 = &frames[n % frames.size()];
		t1 = &frames[(n + 1) % frames.size()];
	}

	void caclTexCoordsByProgress01(const TexCoords*& t0, const TexCoords*& t1, float& blend, float progress)const
	{
		caclTexCoordsByProgress0N(t0, t1, blend, progress * frames.size());
	}
};

class AtlasInfo
{
public:
	rl_class(ParticlesLib::AtlasInfo)

	typedef HashArray<Pair<String, AnimatedImageInfo> >  AnimatedImages_t;
	rl_field(AnimatedImages_t, animatedImages)

	rl_field(uint, layersCount)
	rl_field(uint, widthDeg)
	rl_field(uint, heightDeg)

	AtlasInfo()
	{
		layersCount = 1;
		widthDeg = 0;
		heightDeg = 0;
	};
};


struct QuadVertex
{
	Vector3f position;
	float depth;
	Vector4f color;
	uint16 texCoord[2][2];
	float hue;
	uint8 texCoordLayer[2];
	uint8_01 texBlend;
	uint8_01 saturation;
	
};

enum EAxis
{
	EAxis_None = 0,
	EAxis_GlobalX,
	EAxis_GlobalY,
	EAxis_GlobalZ,
	EAxis_LocalX,
	EAxis_LocalY,
	EAxis_LocalZ,
	EAxis_CameraX,
	EAxis_CameraY,
	EAxis_CameraZ,
	EAxis_EnumCount
};

struct Quad
{
	QuadVertex v[2][2];

	INLINE void setTexCoords(const TexCoords& t0, const TexCoords& t1, uint8_01 blendcoef)
	{
		v[0][0].texCoord[0][0] = t0.v00x;
		v[0][0].texCoord[0][1] = t0.v00y;
		v[0][0].texCoord[1][0] = t1.v00x;
		v[0][0].texCoord[1][1] = t1.v00y;
		v[0][0].texCoordLayer[0] = t0.layer;
		v[0][0].texCoordLayer[1] = t1.layer;
		v[0][0].texBlend = blendcoef;

		v[0][1].texCoord[0][0] = t0.v01x;
		v[0][1].texCoord[0][1] = t0.v01y;
		v[0][1].texCoord[1][0] = t1.v01x;
		v[0][1].texCoord[1][1] = t1.v01y;
		v[0][1].texCoordLayer[0] = t0.layer;
		v[0][1].texCoordLayer[1] = t1.layer;
		v[0][1].texBlend = blendcoef;

		v[1][0].texCoord[0][0] = t0.v10x;
		v[1][0].texCoord[0][1] = t0.v10y;
		v[1][0].texCoord[1][0] = t1.v10x;
		v[1][0].texCoord[1][1] = t1.v10y;
		v[1][0].texCoordLayer[0] = t0.layer;
		v[1][0].texCoordLayer[1] = t1.layer;
		v[1][0].texBlend = blendcoef;

		v[1][1].texCoord[0][0] = t0.v11x;
		v[1][1].texCoord[0][1] = t0.v11y;
		v[1][1].texCoord[1][0] = t1.v11x;
		v[1][1].texCoord[1][1] = t1.v11y;
		v[1][1].texCoordLayer[0] = t0.layer;
		v[1][1].texCoordLayer[1] = t1.layer;
		v[1][1].texBlend = blendcoef;
	}

	INLINE void setTexCoordsStartShift(const TexCoords& t0, const TexCoords& t1, uint8_01 blendcoef, float shift)
	{
		uint k = uint(shift * 65536.0f);
		v[0][0].texCoord[0][0] = t0.v00x + (((t0.v01x - t0.v00x) * k) >> 16);
		v[0][0].texCoord[0][1] = t0.v00y + (((t0.v01y - t0.v00y) * k) >> 16);
		v[0][0].texCoord[1][0] = t1.v00x + (((t1.v01x - t1.v00x) * k) >> 16);
		v[0][0].texCoord[1][1] = t1.v00y + (((t1.v01y - t1.v00y) * k) >> 16);
		v[0][0].texCoordLayer[0] = t0.layer;
		v[0][0].texCoordLayer[1] = t1.layer;
		v[0][0].texBlend = blendcoef;

		v[0][1].texCoord[0][0] = t0.v01x;
		v[0][1].texCoord[0][1] = t0.v01y;
		v[0][1].texCoord[1][0] = t1.v01x;
		v[0][1].texCoord[1][1] = t1.v01y;
		v[0][1].texCoordLayer[0] = t0.layer;
		v[0][1].texCoordLayer[1] = t1.layer;
		v[0][1].texBlend = blendcoef;

		v[1][0].texCoord[0][0] = t0.v10x + (((t0.v11x - t0.v10x) * k) >> 16);
		v[1][0].texCoord[0][1] = t0.v10y + (((t0.v11y - t0.v10y) * k) >> 16);
		v[1][0].texCoord[1][0] = t1.v10x + (((t1.v11x - t1.v10x) * k) >> 16);
		v[1][0].texCoord[1][1] = t1.v10y + (((t1.v11y - t1.v10y) * k) >> 16);
		v[1][0].texCoordLayer[0] = t0.layer;
		v[1][0].texCoordLayer[1] = t1.layer;
		v[1][0].texBlend = blendcoef;

		v[1][1].texCoord[0][0] = t0.v11x;
		v[1][1].texCoord[0][1] = t0.v11y;
		v[1][1].texCoord[1][0] = t1.v11x;
		v[1][1].texCoord[1][1] = t1.v11y;
		v[1][1].texCoordLayer[0] = t0.layer;
		v[1][1].texCoordLayer[1] = t1.layer;
		v[1][1].texBlend = blendcoef;
	}

	INLINE void setTexCoordsByProgress01(const AnimatedImageInfo& aii, float progress)
	{
		const TexCoords *t0, *t1;
		float blend;
		aii.caclTexCoordsByProgress01(t0, t1, blend, progress);
		setTexCoords(*t0, *t1, flooru8_01(blend));
	}

	INLINE void setTexCoordsByProgress0N(const AnimatedImageInfo& aii, float progress)
	{
		const TexCoords *t0, *t1;
		float blend;
		aii.caclTexCoordsByProgress0N(t0, t1, blend, progress);
		setTexCoords(*t0, *t1, flooru8_01(blend));
	}
};


class Source : RefCountable
{
public:
	ListElement<Source> lel;

	Source() : lel(this)
	{
	}

	virtual void update(Scene& scene, float dt)=0;
	virtual void visualize(Scene& scene, View& view, Atlas& atlas)=0;
};

}