#pragma once

#include "seed.h"

namespace DrawerLib
{

enum EVertexFormat
{
	EVertexFormat_None = 0,
	EVertexFormat_Mesh,
	EVertexFormat_SkeletalMesh,
	EVertexFormat_LightProbedMesh,
	EVertexFormat_BuildinLightingMesh,
	EVertexFormat_HeightMap,
	EVertexFormat_Particle,
	EVertexFormat_InterfaceQuad,
	EVertexFormat_InterfaceFont,
	EVertexFormat_SimplePrimitive,
	EVertexFormat_EnumCount
};

struct PackedNormal
{
	uint8_01 x, y, z;
	PackedNormal()
	{
		static_assert(sizeof(PackedNormal) == 3, "PackedNormal invalid size");
		x = y = 127;
		z = 254;
	}
};

static INLINE void convert(PackedNormal& dest, const Vector2_t& inNormal)
{
	Vector2_t srcNormalized = inNormal.normal();
	Vector2_t srcClamped = inNormal / max(abs(inNormal.x), max(abs(inNormal.y), abs(inNormal.z)));
	Vector2_t bestN(0.0f,0.0f,0.0f);
	float bestCoef = 0;
	for(float f = 127.0f; f > 0.0f; f -= 1.0f)
	{
		Vector2_t n = srcClamped * f;
		n.x = floor(n.x + 0.5f);
		n.y = floor(n.y + 0.5f);
		n.z = floor(n.z + 0.5f);
		float coef = n.dot(srcNormalized) / n.lenght();
		if(coef > bestCoef){
			bestCoef = coef;
			bestN = n;
		}
	}

	dest.x = (uint8)clampt<int>(int(bestN.x + 127.5f), 0, 254);
	dest.y = (uint8)clampt<int>(int(bestN.y + 127.5f), 0, 254);
	dest.z = (uint8)clampt<int>(int(bestN.z + 127.5f), 0, 254);
}

static INLINE void convert(Vector2_t& dest, const PackedNormal& src)
{
	dest = Vector2_t((float)src.x, (float)src.y, (float)src.z) * (1.0f / 127.0f) - Vector2_t::getOne();
	dest.selfNormalize();
}

/*
static INLINE void convert(PackedNormal& dest, const Vector3& src)
{
	dest.x = int16((src.x * 0.5f + 0.5f) * 32767.0f * sign(src.z));
	dest.y = int16(src.y * 32767.0f);
}

static INLINE void convert(Vector3& dest, const PackedNormal& src)
{
	float s,a;
	signAbs(s,a, float(src.x));
	dest.x = (a * 2.0f - 1.0f) / 32767.0f;
	dest.y = float(src.y) / 32767.0f;
	dest.z = s * sqrt(1.0f - dest.xy().lenghtSquare());
}
*/


struct PackedTexCoord
{
	int16 x,y;
	INLINE PackedTexCoord(){ x = y = 0; }
};

static INLINE void convert(PackedTexCoord& dest, const Vector2& src)
{
	dest.x = int16(src.x * 32767.0f);
	dest.y = int16(src.y * 32767.0f);
}

static INLINE void convert(Vector2& dest, const PackedTexCoord& src)
{
	dest.x = float(src.x) * (1.0f / 32767.0f);
	dest.y = float(src.y) * (1.0f / 32767.0f);
}

/*
struct PackedColorMatrix45
{
	uint8 r[4],g[4],b[4],a[4],p[4];
	//0 - 127
	//-1 - 63
	//+1 - 191
	//+2 ~ 254
	//-2 ~ 0

	PackedColorMatrix45()
	{
		r[0] = 191;
		r[1] = 127;
		r[2] = 127;
		r[3] = 127;

		g[0] = 127;
		g[1] = 191;
		g[2] = 127;
		g[3] = 127;

		b[0] = 127;
		b[1] = 127;
		b[2] = 191;
		b[3] = 127;

		a[0] = 127;
		a[1] = 127;
		a[2] = 127;
		a[3] = 191;

		p[0] = 127;
		p[1] = 127;
		p[2] = 127;
		p[3] = 127;
	}

	static INLINE uint8 valueFromUint8_01(uint8_01 v)
	{
		return (v >> 2) + 127;
	}

	static INLINE uint8 valueFromFloat(float v)
	{
		return uint8(v * 64.0f + 127.0f);
	}

	void set(const Matrix45f& mat)
	{
		r[0] = valueFromFloat(mat.x.x);
		r[1] = valueFromFloat(mat.x.y);
		r[2] = valueFromFloat(mat.x.z);
		r[3] = valueFromFloat(mat.x.w);

		g[0] = valueFromFloat(mat.y.x);
		g[1] = valueFromFloat(mat.y.y);
		g[2] = valueFromFloat(mat.y.z);
		g[3] = valueFromFloat(mat.y.w);

		b[0] = valueFromFloat(mat.z.x);
		b[1] = valueFromFloat(mat.z.y);
		b[2] = valueFromFloat(mat.z.z);
		b[3] = valueFromFloat(mat.z.w);

		a[0] = valueFromFloat(mat.w.x);
		a[1] = valueFromFloat(mat.w.y);
		a[2] = valueFromFloat(mat.w.z);
		a[3] = valueFromFloat(mat.w.w);

		p[0] = valueFromFloat(mat.p.x);
		p[1] = valueFromFloat(mat.p.y);
		p[2] = valueFromFloat(mat.p.z);
		p[3] = valueFromFloat(mat.p.w);
	}
};
*/

struct MeshVertexFormat
{
public:
	static const uint position_offset = 0;
	PackedVector3f	position;

	static const uint texcoord_offset = position_offset + sizeof(PackedVector3f);
	PackedTexCoord	texcoord;

	static const uint tangentAndUnocclusion_offset = texcoord_offset + sizeof(PackedTexCoord);
	PackedNormal	tangent;
	uint8_01		unocclusion;
	
	static const uint binormalAndOpacity_offset = tangentAndUnocclusion_offset + 4;
	PackedNormal	binormal;
	uint8_01		opacity;

	static const uint normalAndOpacity2_offset = binormalAndOpacity_offset + 4;
	PackedNormal	normal;
	uint8_01		opacity2;

	MeshVertexFormat()
	{
		position.set(0.0f, 0.0f, 0.0f);
		unocclusion = 255;
		opacity = 255;
		opacity2 = 255;
	}
};

struct BuildinLightingMeshVertexFormat : public MeshVertexFormat
{
public:
	static const uint lightVectorXLightIntensivityAndLightIntensivity_offset = sizeof(MeshVertexFormat);
	uint16_01 lightVectorXLightIntensivityAndLightIntensivity[4];

	static const uint lightIntensivity_offset = lightVectorXLightIntensivityAndLightIntensivity_offset + sizeof(uint16_01) * 4;
	uint8_01 lightIntensivity[3];
	uint8_01 lightIntensivityScale;

	static const uint ambientIntensivity_offset = lightIntensivity_offset + 4;
	uint8_01 ambientIntensivity[3];
	uint8_01 ambientIntensivityScale;

	BuildinLightingMeshVertexFormat()
	{
		lightVectorXLightIntensivityAndLightIntensivity[0] = 0;
		lightVectorXLightIntensivityAndLightIntensivity[1] = 0;
		lightVectorXLightIntensivityAndLightIntensivity[2] = 0;
		lightVectorXLightIntensivityAndLightIntensivity[3] = 0;
		lightIntensivity[0] = 0;
		lightIntensivity[1] = 0;
		lightIntensivity[2] = 0;
		lightIntensivityScale = 0;
		ambientIntensivity[0] = 0;
		ambientIntensivity[1] = 0;
		ambientIntensivity[2] = 0;
		ambientIntensivityScale = 0;
	}
};

struct LightProbedMeshVertexFormat : public MeshVertexFormat
{
public:
	static const uint lightProbeIndexes_offset = sizeof(MeshVertexFormat);
	uint8			lightProbeIndexes[4];

	static const uint lightProbeWeights_offset = lightProbeIndexes_offset + sizeof(uint8) * 4;
	uint8_01		lightProbeWeights[4];

	LightProbedMeshVertexFormat()
	{
		lightProbeIndexes[0] = lightProbeIndexes[1] = lightProbeIndexes[2] = lightProbeIndexes[3] = 0;
		lightProbeWeights[0] = lightProbeWeights[1] = lightProbeWeights[2] = lightProbeWeights[3] = 0;
	}
};

struct SkeletalMeshVertexFormat : public MeshVertexFormat
{
public:
	static const uint boneIndexes_offset = sizeof(MeshVertexFormat);
	uint8 boneIndexes[4];

	static const uint boneWeights_offset = boneIndexes_offset + sizeof(uint8) * 4;
	uint8_01 boneWeights[4];

	SkeletalMeshVertexFormat()
	{
		boneIndexes[0] = boneIndexes[1] = boneIndexes[2] = boneIndexes[3] = 0;
		boneWeights[0] = boneWeights[1] = boneWeights[2] = boneWeights[3] = 0;
	}
};

struct NoInitFields
{
	NoInitFields(){}
};

struct HeightMapVertexFormat
{
public:
	static const uint indexes_offset = 0;
	union
	{
		struct { int16 i, j; };
		uint32 ij;
	};

	static const uint height_offset = indexes_offset + sizeof(int16) * 2;
	float height;

	static const uint derivata_offset = height_offset + sizeof(float);
	int16 derivataX, derivataY;
	// int(256) == 1

	static const uint layers_offset = derivata_offset + sizeof(int16) * 2;
	uint8_01 layer[8];

	INLINE HeightMapVertexFormat(const NoInitFields& nif)
	{
	}

	INLINE HeightMapVertexFormat()
	{
		i = 0;
		j = 0;
		height = 0;
		derivataX = 0;
		derivataY = 0;
		layer[0] = 255;
		layer[1] = 255;
		layer[2] = 255;
		layer[3] = 255;
		layer[4] = 255;
		layer[5] = 255;
		layer[6] = 255;
		layer[7] = 255;
	}

	INLINE HeightMapVertexFormat(const HeightMapVertexFormat& other)
	{
		assign(other);
	}

	INLINE const HeightMapVertexFormat& operator = (const HeightMapVertexFormat& other)
	{
		assign(other);
		return *this;
	}

	INLINE void assign(const HeightMapVertexFormat& other)
	{
		static_assert(sizeof(HeightMapVertexFormat) == 20, "HeightMapVertexFormat invalid size");
		uint32* dst = reinterpret_cast<uint32*>(this);
		const uint32* src = reinterpret_cast<const uint32*>(&other);
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
		dst[3] = src[3];
		dst[4] = src[4];
	}
};


struct InterfaceQuadVertexFormat
{
	static const uint position_offset = 0;
	PackedVector2f position;

	static const uint texcoord_offset = position_offset + sizeof(PackedVector2f);
	PackedVector2f texcoord;

	static const uint color_offset = texcoord_offset + sizeof(PackedVector2f);
	PackedVector4f color;

	InterfaceQuadVertexFormat()
	{
		position.set(0,0);
		texcoord.set(0,0);
		color.set(1,1,1,1);
	}
};

struct InterfaceFontVertexFormat : public InterfaceQuadVertexFormat
{
	static const uint params_offset = sizeof(InterfaceQuadVertexFormat);
	uint8 byteNumber;
	uint8 bitNumber;
	uint8 bitLenght;
	uint8 reserved;

	InterfaceFontVertexFormat()
	{
		byteNumber = 0;
		bitNumber = 0;
		bitLenght = 1;
		reserved = 0;
	}
};

struct SimplePrimitiveVertexFormat
{
	static const uint position_offset = 0;
	PackedVector3f position;

	static const uint color_offset = position_offset + sizeof(PackedVector3f);
	ColorRGBA color;
	
	SimplePrimitiveVertexFormat(){}
	SimplePrimitiveVertexFormat(const Vector3f& pos) : position(pos), color(0xffffffff) {}
	SimplePrimitiveVertexFormat(const Vector3f& pos, const ColorRGBA color) : position(pos), color(color) {}
};

struct ParticleVertexFormat
{
	static const uint position_offset = 0;
	PackedVector3f position;

	static const uint depth_offset = position_offset + sizeof(PackedVector3f);
	float depth;

	static const uint color_offset = depth_offset + sizeof(float); 
	PackedVector4f color;

	static const uint texCoords_offset = color_offset + sizeof(PackedVector4f);
	PackedTexCoord texCoord[2];

	static const uint hue_offset = texCoords_offset + sizeof(PackedTexCoord) * 2;
	float hue;

	static const uint params_offset = hue_offset + sizeof(float);
	uint8 texCoordLayer[2];
	uint8_01 texCoordBlend;
	uint8_01 saturation;

	ParticleVertexFormat()
	{
		position.set(0.0f, 0.0f, 0.0f);
		depth = 0.0f;
		color.set(0.0f, 0.0f, 0.0f, 0.0f);
		hue = 0.0f;
		texCoordLayer[0] = texCoordLayer[1] = 0;
		texCoordBlend = 0;
		saturation = 255;
	}
};


static INLINE uint formatSize(EVertexFormat fmt)
{
	switch(fmt)
	{
	case EVertexFormat_Mesh: return sizeof(MeshVertexFormat);
	case EVertexFormat_SkeletalMesh: return sizeof(SkeletalMeshVertexFormat);
	case EVertexFormat_LightProbedMesh: return sizeof(LightProbedMeshVertexFormat);
	case EVertexFormat_BuildinLightingMesh: return sizeof(BuildinLightingMeshVertexFormat);
	case EVertexFormat_HeightMap: return sizeof(HeightMapVertexFormat);
	case EVertexFormat_Particle: return sizeof(ParticleVertexFormat);
	case EVertexFormat_InterfaceQuad: return sizeof(InterfaceQuadVertexFormat);
	case EVertexFormat_InterfaceFont: return sizeof(InterfaceFontVertexFormat);
	case EVertexFormat_SimplePrimitive: return sizeof(SimplePrimitiveVertexFormat);
	default: return 0;
	}
}


}

