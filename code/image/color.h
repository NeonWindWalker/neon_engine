#pragma once

#include "../base/include.h"

namespace ImageLib
{

USING_BASE_LIB

enum EColor
{
	EColor_None = 0,
	EColor_RGBA,
	EColor_RGB,
	EColor_LA,
	EColor_L,
	EColor_RGBAF,
	EColor_RGBF,
	EColor_RGF,
	EColor_RF,

	EColor_DXT1,
	EColor_DXT1A,
	EColor_DXT3,
	EColor_DXT5,

	EColor_PVR2,
	EColor_PVR4,
	EColor_PVR2A,
	EColor_PVR4A,

	EColor_ETC1,

	EColor_ATC,
	EColor_ATCI,

	EColor_EnumCount
};

static INLINE_NO_DEBUG boolean isPackedFormat(EColor type)
{
	return EColor_DXT1 <= type && type < EColor_EnumCount;
}


struct ColorRGB
{
	union
	{
		struct { byte b,g,r; };
		byte arr[3];
	};
	INLINE_NO_DEBUG ColorRGB(){}
	INLINE_NO_DEBUG ColorRGB(byte R, byte G, byte B, byte A) : r(R), g(G), b(B) {}
	INLINE_NO_DEBUG ColorRGB(byte R, byte G, byte B) : r(R), g(G), b(B) {}
	INLINE_NO_DEBUG void set(byte R, byte G, byte B, byte A) { r = R; g = G; b = B; }
	INLINE_NO_DEBUG void set(byte R, byte G, byte B) { r = R; g = G; b = B; }
	INLINE_NO_DEBUG ColorRGB& operator = (const ColorRGB& other) { r = other.r; g = other.g; b = other.b; return *this; }
	INLINE_NO_DEBUG boolean operator == (const ColorRGB& other) { return (uint(r) | (uint(g) << 8) | (uint(b) << 16)) == (uint(other.r) | (uint(other.g) << 8) | (uint(other.b) << 16)); }
	INLINE_NO_DEBUG boolean operator != (const ColorRGB& other) { return (uint(r) | (uint(g) << 8) | (uint(b) << 16)) != (uint(other.r) | (uint(other.g) << 8) | (uint(other.b) << 16)); }
};

struct ColorRGBA
{
	union
	{
		struct { byte b,g,r,a; };
		byte arr[4];
		uint32 u32;
	};
	INLINE_NO_DEBUG ColorRGBA(){}
	INLINE_NO_DEBUG ColorRGBA(byte R, byte G, byte B, byte A) : r(R), g(G), b(B), a(A) {}
	INLINE_NO_DEBUG ColorRGBA(uint32 U32) : u32(U32) {}
	INLINE_NO_DEBUG ColorRGBA(ColorRGB c3, byte A) : r(c3.r), g(c3.g), b(c3.b), a(A) {}
	INLINE_NO_DEBUG void set(byte R, byte G, byte B, byte A) { r = R; g = G; b = B; a = A; }
	INLINE_NO_DEBUG void set(uint32 U32) { u32 = U32; }
	INLINE_NO_DEBUG void set(ColorRGB c3, byte A) { r = c3.r; g = c3.g; b = c3.b; a = A; }
	INLINE_NO_DEBUG ColorRGBA& operator = (const ColorRGBA& other) { u32 = other.u32; return *this; }
	INLINE_NO_DEBUG boolean operator == (const ColorRGBA& other)const { return u32 == other.u32; }
	INLINE_NO_DEBUG boolean operator != (const ColorRGBA& other)const { return u32 != other.u32; }

	INLINE_NO_DEBUG ColorRGB& c3() { return *reinterpret_cast<ColorRGB*>(this); }
	INLINE_NO_DEBUG const ColorRGB& c3()const { return *reinterpret_cast<const ColorRGB*>(this); }
	
	static INLINE_NO_DEBUG ColorRGBA getRed(){ return ColorRGBA(255,0,0,255); }
	static INLINE_NO_DEBUG ColorRGBA getGreen(){ return ColorRGBA(0,255,0,255); }
	static INLINE_NO_DEBUG ColorRGBA getBlue(){ return ColorRGBA(0,0,255,255); }
};



struct ColorLA
{
	union
	{
		struct { byte l,a; };
		byte arr[2];
		uint16 u16;
	};

	INLINE_NO_DEBUG void set(byte L, byte A)
	{
		l = L;
		a = A;
	}
};


typedef uint8 ColorL;

struct Block4x4RGBA
{
	Vector4f color[2];
	float weight[4][4]; 
};

struct Block4x4RGB
{
	Vector3f color[2];
	float weight[4][4]; 
};

struct Block4x4L
{
	float color[2];
	float weight[4][4]; 
};

struct Block4x4Transparency
{
	byte transparent[4][4];
};

struct ColorDXT1
{
	uint16 color[2];
	byte weight[4];

	enum MyEnum
	{
		gMinimalBlocksX = 1,
		gMinimalBlocksY = 1,
		gWidth = 4,
		gHeight = 4
	}; 

	ColorDXT1()
	{
		static_assert(sizeof(ColorDXT1) == 8, "invalid size");
	}

	static INLINE_NO_DEBUG Vector3f unpackColor(uint16 c)
	{
		return Vector3f(
			float((c>>11) & 31) / 31.0f,
			float((c>>5) & 63)  / 63.0f,
			float(c & 31) / 31.0f);
	}

	static INLINE_NO_DEBUG uint16 packColor(const Vector3f& c)
	{
		return (uint16)(uint(c.z * 31.0f + 0.5f) | (uint(c.y * 63.0f + 0.5f)<<5) | (uint(c.x * 31.0f + 0.5f)<<11));
	}
};
    
struct ColorDXT1A
{
	uint16 c0,c1;
	byte block[4];

	ColorDXT1A()
	{
		static_assert(sizeof(ColorDXT1A) == 8, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 1,
		gMinimalBlocksY = 1,
		gWidth = 4,
		gHeight = 4
	}; 
};

struct ColorDXT3
{
	uint16 c0,c1;
	byte block[4];
	byte a0,a1;
	byte ablock[6];

	ColorDXT3()
	{
		static_assert(sizeof(ColorDXT3) == 16, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 1,
		gMinimalBlocksY = 1,
		gWidth = 4,
		gHeight = 4
	}; 
};

struct ColorDXT5
{
	uint16 c0,c1;
	byte block[4];
	byte a0,a1;
	byte ablock[6];

	ColorDXT5()
	{
		static_assert(sizeof(ColorDXT5) == 16, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 1,
		gMinimalBlocksY = 1,
		gWidth = 4,
		gHeight = 4
	}; 
};

struct ColorPVR2
{
	byte bits[8];

	ColorPVR2()
	{
		static_assert(sizeof(ColorPVR2) == 8, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 2,
		gMinimalBlocksY = 2,
		gWidth = 8,
		gHeight = 4
	}; 
};

struct ColorPVR4
{
	uint16 c0,c1;
	byte block[4];

	ColorPVR4()
	{
		static_assert(sizeof(ColorPVR4) == 8, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 2,
		gMinimalBlocksY = 2,
		gWidth = 4,
		gHeight = 4
	}; 
};

    
struct ColorPVR2A
{
	byte bits[8];

	ColorPVR2A()
	{
		static_assert(sizeof(ColorPVR2A) == 8, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 2,
		gMinimalBlocksY = 2,
		gWidth = 8,
		gHeight = 4
	}; 
};
    
struct ColorPVR4A
{
	uint16 c0,c1;
	byte block[4];

	ColorPVR4A()
	{
		static_assert(sizeof(ColorPVR4A) == 8, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 2,
		gMinimalBlocksY = 2,
		gWidth = 4,
		gHeight = 4
	}; 
};
    
struct ColorETC1
{
	uint16 c0,c1;
	byte block[4];

	ColorETC1()
	{
		static_assert(sizeof(ColorETC1) == 8, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 1,
		gMinimalBlocksY = 1,
		gWidth = 4,
		gHeight = 4
	}; 
};

struct ColorATC
{
	uint16 c0,c1;
	byte block[4];

	ColorATC()
	{
		static_assert(sizeof(ColorATC) == 8, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 1,
		gMinimalBlocksY = 1,
		gWidth = 4,
		gHeight = 4
	}; 
};
    
struct ColorATCA
{
	uint16 c0,c1;
	byte block[4];
	byte a0,a1;
	byte ablock[6];

	ColorATCA()
	{
		static_assert(sizeof(ColorATCA) == 16, "invalid size");
	}

	enum MyEnum
	{
		gMinimalBlocksX = 1,
		gMinimalBlocksY = 1,
		gWidth = 4,
		gHeight = 4
	}; 
};


typedef Vector4f ColorRGBAF;
typedef Vector3f ColorRGBF;
typedef Vector2f ColorRGF;
typedef float ColorRF;
typedef Vector4f Color;

static INLINE_NO_DEBUG uint eColorSize(EColor fmt)
{
	switch(fmt)
	{
	case EColor_RGBA: return sizeof(ColorRGBA);
	case EColor_RGB: return sizeof(ColorRGB);
	case EColor_LA: return sizeof(ColorLA);
	case EColor_L: return sizeof(ColorL);
	case EColor_DXT1: return sizeof(ColorDXT1);
	case EColor_DXT1A: return sizeof(ColorDXT1A);
	case EColor_DXT3: return sizeof(ColorDXT3);
	case EColor_DXT5: return sizeof(ColorDXT5);

	case EColor_PVR2: return sizeof(ColorPVR2);
	case EColor_PVR4: return sizeof(ColorPVR4);
	case EColor_PVR2A: return sizeof(ColorPVR2A);
	case EColor_PVR4A: return sizeof(ColorPVR4A);
	case EColor_ETC1: return sizeof(ColorETC1);
	case EColor_ATC: return sizeof(ColorATC);
	case EColor_ATCI: return sizeof(ColorATCA);

	case EColor_RGBAF: return sizeof(ColorRGBAF);
	case EColor_RGBF: return sizeof(ColorRGBF);
	case EColor_RGF: return sizeof(ColorRGF);
	case EColor_RF: return sizeof(ColorRF);
	default: return 0;
	}
}


template<typename t_Type> class EColorOfType { public: static const EColor value = EColor_None; };
template<> class EColorOfType<ColorRGBA> { public: static const EColor value = EColor_RGBA; };
template<> class EColorOfType<ColorRGB> { public: static const EColor value = EColor_RGB; };
template<> class EColorOfType<ColorL> { public: static const EColor value = EColor_L; };
template<> class EColorOfType<ColorLA> { public: static const EColor value = EColor_LA; };
template<> class EColorOfType<ColorDXT1> { public: static const EColor value = EColor_DXT1; };
template<> class EColorOfType<ColorDXT1A> { public: static const EColor value = EColor_DXT1A; };
template<> class EColorOfType<ColorDXT3> { public: static const EColor value = EColor_DXT3; };
template<> class EColorOfType<ColorDXT5> { public: static const EColor value = EColor_DXT5; };
template<> class EColorOfType<ColorPVR2> { public: static const EColor value = EColor_PVR2; };
template<> class EColorOfType<ColorPVR4> { public: static const EColor value = EColor_PVR4; };
template<> class EColorOfType<ColorPVR2A> { public: static const EColor value = EColor_PVR2A; };
template<> class EColorOfType<ColorPVR4A> { public: static const EColor value = EColor_PVR4A; };
template<> class EColorOfType<ColorETC1> { public: static const EColor value = EColor_ETC1; };
template<> class EColorOfType<ColorATC> { public: static const EColor value = EColor_ATC; };
template<> class EColorOfType<ColorATCA> { public: static const EColor value = EColor_ATCI; };
template<> class EColorOfType<ColorRGBAF> { public: static const EColor value = EColor_RGBAF; };
template<> class EColorOfType<ColorRGBF> { public: static const EColor value = EColor_RGBF; };
template<> class EColorOfType<ColorRGF> { public: static const EColor value = EColor_RGF; };
template<> class EColorOfType<ColorRF> { public: static const EColor value = EColor_RF; };

static INLINE_NO_DEBUG uint formatBitSize(EColor fmt)
{
	switch(fmt)
	{
	case EColor_RGBA: return 32;
	case EColor_RGB: return 32;
	case EColor_LA: return 16;
	case EColor_L: return 8;
	case EColor_DXT1: return 4;
	case EColor_DXT1A: return 4;
	case EColor_DXT3: return 8;
	case EColor_DXT5: return 8;

	case EColor_PVR2: return 2;
	case EColor_PVR4: return 4;
	case EColor_PVR2A: return 4;
	case EColor_PVR4A: return 4;

	case EColor_ETC1: return 4;

	case EColor_ATC: return 4;
	case EColor_ATCI: return 8;

	case EColor_RGBAF: return 16*8;
	case EColor_RGBF: return 12*8;
	default: return 0;
	}
}

struct BlockInfo
{
	uint byteSize;
	uint minXBlocks;
	uint minYBlocks;
	uint width;
	uint height;

	BlockInfo()
	{
		byteSize = 0;
		minXBlocks = 0;
		minYBlocks = 0;
		width = 0;
		height = 0;
	}

	INLINE uint calcImageSize(uint w, uint h)const
	{
		return byteSize * maximum<uint>(w / width, minXBlocks) * maximum<uint>(h / height, minYBlocks);
	}
};

static INLINE_NO_DEBUG BlockInfo blockInfo(EColor fmt)
{
	BlockInfo bi;
	switch(fmt)
	{
	case EColor_PVR2: 
		bi.byteSize = sizeof(ColorPVR2);
		bi.minXBlocks = ColorPVR2::gMinimalBlocksX;
		bi.minYBlocks = ColorPVR2::gMinimalBlocksY;
		bi.width = ColorPVR2::gWidth;
		bi.height = ColorPVR2::gHeight;
		return bi;

	case EColor_PVR4:
		bi.byteSize = sizeof(ColorPVR4);
		bi.minXBlocks = ColorPVR4::gMinimalBlocksX;
		bi.minYBlocks = ColorPVR4::gMinimalBlocksY;
		bi.width = ColorPVR4::gWidth;
		bi.height = ColorPVR4::gHeight;
		return bi;

	case EColor_PVR2A: 
		bi.byteSize = sizeof(ColorPVR2A);
		bi.minXBlocks = ColorPVR2A::gMinimalBlocksX;
		bi.minYBlocks = ColorPVR2A::gMinimalBlocksY;
		bi.width = ColorPVR2A::gWidth;
		bi.height = ColorPVR2A::gHeight;
		return bi;

	case EColor_PVR4A:
		bi.byteSize = sizeof(ColorPVR4A);
		bi.minXBlocks = ColorPVR4A::gMinimalBlocksX;
		bi.minYBlocks = ColorPVR4A::gMinimalBlocksY;
		bi.width = ColorPVR4A::gWidth;
		bi.height = ColorPVR4A::gHeight;
		return bi;

	case EColor_ETC1:
		bi.byteSize = sizeof(ColorETC1);
		bi.minXBlocks = ColorETC1::gMinimalBlocksX;
		bi.minYBlocks = ColorETC1::gMinimalBlocksY;
		bi.width = ColorETC1::gWidth;
		bi.height = ColorETC1::gHeight;
		return bi;

	case EColor_DXT1:
		bi.byteSize = sizeof(ColorDXT1);
		bi.minXBlocks = ColorDXT1::gMinimalBlocksX;
		bi.minYBlocks = ColorDXT1::gMinimalBlocksY;
		bi.width = ColorDXT1::gWidth;
		bi.height = ColorDXT1::gHeight;
		return bi;

	case EColor_DXT1A:
		bi.byteSize = sizeof(ColorDXT1A);
		bi.minXBlocks = ColorDXT1A::gMinimalBlocksX;
		bi.minYBlocks = ColorDXT1A::gMinimalBlocksY;
		bi.width = ColorDXT1A::gWidth;
		bi.height = ColorDXT1A::gHeight;
		return bi;

	case EColor_DXT3:
		bi.byteSize = sizeof(ColorDXT3);
		bi.minXBlocks = ColorDXT3::gMinimalBlocksX;
		bi.minYBlocks = ColorDXT3::gMinimalBlocksY;
		bi.width = ColorDXT3::gWidth;
		bi.height = ColorDXT3::gHeight;
		return bi;

	case EColor_DXT5:
		bi.byteSize = sizeof(ColorDXT5);
		bi.minXBlocks = ColorDXT5::gMinimalBlocksX;
		bi.minYBlocks = ColorDXT5::gMinimalBlocksY;
		bi.width = ColorDXT5::gWidth;
		bi.height = ColorDXT5::gHeight;
		return bi;

	case EColor_ATC:
		bi.byteSize = sizeof(ColorATC);
		bi.minXBlocks = ColorATC::gMinimalBlocksX;
		bi.minYBlocks = ColorATC::gMinimalBlocksY;
		bi.width = ColorATC::gWidth;
		bi.height = ColorATC::gHeight;
		return bi;

	case EColor_ATCI:
		bi.byteSize = sizeof(ColorATCA);
		bi.minXBlocks = ColorATCA::gMinimalBlocksX;
		bi.minYBlocks = ColorATCA::gMinimalBlocksY;
		bi.width = ColorATCA::gWidth;
		bi.height = ColorATCA::gHeight;
		return bi;
            
    default:
        Assert(false);
	}

	return bi;
}


template<typename t_Type> class ColorConvertor{};

template<> class ColorConvertor<ColorRGBA>
{ 
public:
	typedef Vector4f OperationType;

	static INLINE_NO_DEBUG OperationType epsilen()
	{
		return OperationType(1.0f / 256.0f, 1.0f / 256.0f, 1.0f / 256.0f, 1.0f / 256.0f);
	}

	static INLINE_NO_DEBUG OperationType zero()
	{
		return OperationType(0.0f,0.0f,0.0f,0.0f);
	}

	static INLINE_NO_DEBUG OperationType unpuck(const ColorRGBA& src)
	{
		return OperationType(float(src.r), float(src.g), float(src.b), float(src.a)) * (1.0f / 255.0f);
	}

	static INLINE_NO_DEBUG ColorRGBA puck(const OperationType& src)
	{
		OperationType c = clamp01(src) * 255.0f + OperationType(0.5f,0.5f,0.5f,0.5f);
		ColorRGBA ret;
		ret.r = (uint8)c.x;
		ret.g = (uint8)c.y;
		ret.b = (uint8)c.z;
		ret.a = (uint8)c.w;
		return ret;
	}

	static INLINE_NO_DEBUG ColorRGBA puck(const Vector3f& src)
	{
		Vector3f c = clamp01(src) * 255.0f + Vector3f(0.5f,0.5f,0.5f);
		ColorRGBA ret;
		ret.r = (uint8)c.x;
		ret.g = (uint8)c.y;
		ret.b = (uint8)c.z;
		ret.a = 255;
		return ret;
	}

	static INLINE_NO_DEBUG ColorRGBA puck(const Vector2f& src)
	{
		Vector2f c = clamp01(src) * 255.0f + Vector2f(0.5f,0.5f);
		ColorRGBA ret;
		ret.r = (uint8)c.x;
		ret.a = (uint8)c.y;
		ret.b = ret.g = ret.r;
		return ret;
	}

	static INLINE_NO_DEBUG ColorRGBA puck(const float src)
	{
		float c = clamp01(src) * 255.0f + 0.5f;
		ColorRGBA ret;
		ret.r = (uint8)c;
		ret.b = ret.g = ret.r;
		ret.a = 255;
		return ret;
	}
};

template<> class ColorConvertor<ColorRGB>
{ 
public:
	typedef Vector3f OperationType;

	static INLINE_NO_DEBUG OperationType epsilen()
	{
		return OperationType(1.0f / 256.0f, 1.0f / 256.0f, 1.0f / 256.0f);
	}

	static INLINE_NO_DEBUG OperationType zero()
	{
		return OperationType(0.0f,0.0f,0.0f);
	}

	static INLINE_NO_DEBUG OperationType unpuck(const ColorRGB& src)
	{
		return OperationType(float(src.r), float(src.g), float(src.b)) * (1.0f / 255.0f);
	}

	static INLINE_NO_DEBUG ColorRGB puck(const Vector4f& src)
	{
		OperationType c = clamp01(src.xyz()) * 255.0f + OperationType(0.5f,0.5f,0.5f);
		ColorRGB ret;
		ret.r = (uint8)c.x;
		ret.g = (uint8)c.y;
		ret.b = (uint8)c.z;
		return ret;
	}

	static INLINE_NO_DEBUG ColorRGB puck(const OperationType& src)
	{
		OperationType c = clamp01(src) * 255.0f + OperationType(0.5f,0.5f,0.5f);
		ColorRGB ret;
		ret.r = (uint8)c.x;
		ret.g = (uint8)c.y;
		ret.b = (uint8)c.z;
		return ret;
	}

	static INLINE_NO_DEBUG ColorRGB puck(const Vector2f& src)
	{
		float c = clamp01(src.x) * 255.0f + 0.5f;
		ColorRGB ret;
		ret.r = (uint8)c;
		ret.b = ret.g = ret.r;
		return ret;
	}

	static INLINE_NO_DEBUG ColorRGB puck(const float src)
	{
		float c = clamp01(src) * 255.0f + 0.5f;
		ColorRGB ret;
		ret.r = (uint8)c;
		ret.b = ret.g = ret.r;
		return ret;
	}
};

template<> class ColorConvertor<ColorL>
{
public:
	typedef float OperationType;

	static INLINE_NO_DEBUG OperationType epsilen()
	{
		return OperationType(1.0f / 256.0f);
	}

	static INLINE_NO_DEBUG OperationType zero()
	{
		return OperationType(0.0f);
	}

	static INLINE_NO_DEBUG OperationType unpuck(const ColorL& src)
	{
		return float(src) * (1.0f / 255.0f);
	}

	static INLINE_NO_DEBUG ColorL puck(const OperationType& src)
	{
		float c = clamp01(src) * 255.0f + 0.5f;
		byte ret;
		ret = (uint8)c;
		return ret;
	}

	static INLINE_NO_DEBUG ColorL puck(const Vector2f& src)
	{
		float c = clamp01(src.x) * 255.0f + 0.5f;
		byte ret;
		ret = (uint8)c;
		return ret;
	}

	static INLINE_NO_DEBUG ColorL puck(const Vector3f& src)
	{
		float c = clamp01((src.x + src.y + src.z) * 0.33333f) * 255.0f + 0.5f;
		byte ret;
		ret = (uint8)c;
		return ret;
	}

	static INLINE_NO_DEBUG ColorL puck(const Vector4f& src)
	{
		float c = clamp01((src.x + src.y + src.z) * 0.33333f) * 255.0f + 0.5f;
		byte ret;
		ret = (uint8)c;
		return ret;
	}
};

template<> class ColorConvertor<ColorLA>
{
public:
	typedef Vector2f OperationType;

	static INLINE_NO_DEBUG OperationType zero()
	{
		return OperationType(0.0f, 0.0f);
	}

	static INLINE_NO_DEBUG OperationType epsilen()
	{
		return OperationType(1.0f / 256.0f, 1.0f / 256.0f);
	}

	static INLINE_NO_DEBUG OperationType unpuck(const ColorLA& src)
	{
		return OperationType(float(src.l), float(src.a)) * (1.0f / 255.0f);
	}

	static INLINE_NO_DEBUG ColorLA puck(const OperationType& src)
	{
		OperationType c = clamp01(src) * 255.0f + OperationType(0.5f,0.5f);
		ColorLA ret;
		ret.l = (uint8)c.x;
		ret.a = (uint8)c.y;
		return ret;
	}
};

template<> class ColorConvertor<ColorRGBAF>
{
public:
	typedef Vector4f OperationType;

	static INLINE_NO_DEBUG OperationType epsilen()
	{
		return OperationType(1e-7f,1e-7f,1e-7f,1e-7f);
	}

	static INLINE_NO_DEBUG OperationType zero()
	{
		return OperationType(0.0f, 0.0f, 0.0f, 0.0f);
	}

	static INLINE_NO_DEBUG OperationType unpuck(const ColorRGBAF& src)
	{
		return src;
	}

	static INLINE_NO_DEBUG ColorRGBAF puck(const OperationType& src)
	{
		return src;
	}

	static INLINE_NO_DEBUG void set(ColorRGBAF& dst, const OperationType& src)
	{
		dst = puck(src);
	}
};

template<> class ColorConvertor<ColorRGBF>
{
public:
	typedef Vector3f OperationType;

	static INLINE_NO_DEBUG OperationType epsilen()
	{
		return OperationType(1e-7f,1e-7f,1e-7f);
	}

	static INLINE_NO_DEBUG OperationType zero()
	{
		return OperationType(0.0f, 0.0f, 0.0f);
	}

	static INLINE_NO_DEBUG OperationType unpuck(const ColorRGBF& src)
	{
		return src;
	}

	static INLINE_NO_DEBUG ColorRGBF puck(const OperationType& src)
	{
		return src;
	}
};

template<> class ColorConvertor<ColorRGF>
{
public:
	typedef Vector2f OperationType;

	static INLINE_NO_DEBUG OperationType epsilen()
	{
		return OperationType(1e-7f,1e-7f);
	}

	static INLINE_NO_DEBUG OperationType zero()
	{
		return OperationType(0.0f, 0.0f);
	}

	static INLINE_NO_DEBUG OperationType unpuck(const ColorRGF& src)
	{
		return src;
	}

	static INLINE_NO_DEBUG ColorRGF puck(const OperationType& src)
	{
		return src;
	}
};

template<> class ColorConvertor<ColorRF>
{
public:
	typedef float OperationType;

	static INLINE_NO_DEBUG OperationType epsilen()
	{
		return OperationType(1e-7f);
	}

	static INLINE_NO_DEBUG OperationType zero()
	{
		return OperationType(0.0f);
	}

	static INLINE_NO_DEBUG OperationType unpuck(const ColorRF& src)
	{
		return src;
	}

	static INLINE_NO_DEBUG ColorRF puck(const OperationType& src)
	{
		return src;
	}
};




template<> class ColorConvertor<ColorDXT1>
{ 
public:
	typedef Block4x4RGB OperationType;

	static INLINE_NO_DEBUG OperationType unpuck(const ColorDXT1& src)
	{
		OperationType bl;
		for(uint n=0; n < 2; ++n)
			 bl.color[n] = ColorDXT1::unpackColor(src.color[n]);
		for(uint n=0; n < 16; ++n)
			bl.weight[0][n] = float((src.weight[n>>2]>>((n & 3)<<1)) & 3) / 3.0f;
		return bl;
	}

	static INLINE_NO_DEBUG ColorDXT1 puck(const OperationType& src)
	{
		ColorDXT1 dst;
		for(uint n=0; n < 2; ++n)
			dst.color[n] = ColorDXT1::packColor(src.color[n]);
		for(uint n=0; n < 4; ++n)
			dst.weight[0] = 0;
		for(uint n=0; n < 16; ++n)
			dst.weight[n>>2] |= byte(src.weight[0][n] * 3.0f + 0.5f) << ((n & 3)<<1);
		return dst;
	}
};


static INLINE_NO_DEBUG Vector3f colorFromHueSaturationValue(const Vector3f& h03sv)
{
	Vector3f c;
	if(h03sv.x < 1.0f)
	{
		float f = h03sv.x;
		c.set(1.0f - f, f, 0.0f);
	}
	else if(h03sv.x < 2.0f)
	{
		float f = h03sv.x - 1.0f;
		c.set(0.0f, 1.0f - f, f);
	}
	else
	{
		float f = h03sv.x - 2.0f;
		c.set(f, 0.0f, 1.0f - f);
	}

	c /= max(c.x, max(c.y, c.z));

	c *= h03sv.y;
	float gray = 1.0f - h03sv.y;
	c += Vector3f(gray, gray, gray);
	return c * h03sv.z;
}

static INLINE_NO_DEBUG Vector3f hueSaturationValueFromColor(const Vector3f& color)
{
	float m = min(color.x, min(color.y, color.z));
	float M = max(color.x, max(color.y, color.z));
	float saturation = 1.0f - m / (M + 1e-32f);
	Vector3f c = color - Vector3f(m,m,m);
	c /= c.x + c.y + c.z;
	float hue03 = 0;
	if(Base::abs(c.z) < 1e-8f)
	{
		hue03 = c.y;
	}
	else if(Base::abs(c.x) < 1e-8f)
	{
		hue03 = c.z + 1.0f;
	}
	else if(Base::abs(c.y) < 1e-8f)
	{
		hue03 = c.x + 2.0f;
	}

	return Vector3f(hue03, saturation, M);
}

//DEL THIS. hue operation is unlinear operation, this is principilay unpossible 
static INLINE_NO_DEBUG void hueMatrix(Matrix33f& mat, float hue03)
{
	if(hue03 < 1.0f)
	{
		float f = hue03;
		mat.x.set(1.0f - f, f, 0.0f);
		mat.y.set(0.0f, 1.0f - f, f);
		mat.z.set(f, 0.0f, 1.0f - f);
	}
	else if(hue03 < 2.0f)
	{
		float f = hue03 - 1.0f;
		mat.x.set(0.0f, 1.0f - f, f);
		mat.y.set(f, 0.0f, 1.0f - f);
		mat.z.set(1.0f - f, f, 0.0f);
	}
	else
	{
		float f = hue03 - 2.0f;
		mat.x.set(f, 0.0f, 1.0f - f);
		mat.y.set(1.0f - f, f, 0.0f);
		mat.z.set(0.0f, 1.0f - f, f);
	}

	mat.x /= max(mat.x.x, max(mat.x.y, mat.x.z));
	mat.y /= max(mat.y.x, max(mat.y.y, mat.y.z));
	mat.z /= max(mat.z.x, max(mat.z.y, mat.z.z));
}

}


