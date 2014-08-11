#pragma once

#include "config.h"

#ifdef INSTRUCTIONS_SIMD

namespace Base
{
class FloatX;
class FloatY;
class FloatZ;
class FloatW;
struct SIMD4;
struct Float;
struct FloatVector;
#ifdef INSTRUCTIONS_NEON
struct SIMD2;
struct FloatShort;
struct FloatShortVector;
#endif

class FloatX
{
public:
	static const uint gIndex = 0;

	INLINE_NO_DEBUG FloatX()
	{
		Assert((reinterpret_cast<Base::uinteger>(this) & 15) == 0);
	}

	INLINE_NO_DEBUG operator float& ()
	{
		return _value; 
	}

	INLINE_NO_DEBUG operator const float& ()const
	{
		return _value; 
	}

	INLINE_NO_DEBUG void operator = (float flt)
	{
		_value = flt;
	}

	INLINE_NO_DEBUG void operator = (const Float& f);
	INLINE_NO_DEBUG void operator = (const FloatX& f);
	INLINE_NO_DEBUG void operator = (const FloatY& f);
	INLINE_NO_DEBUG void operator = (const FloatZ& f);
	INLINE_NO_DEBUG void operator = (const FloatW& f);
#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG void operator = (const FloatShort& f);
	INLINE_NO_DEBUG void operator = (const FloatShortX& f);
	INLINE_NO_DEBUG void operator = (const FloatShortY& f);
#endif

	INLINE_NO_DEBUG SIMD4& simd();
	INLINE_NO_DEBUG const SIMD4& simd()const;

	INLINE_NO_DEBUG FloatX(const Float& other) { *this = other; }
	INLINE_NO_DEBUG FloatX(const FloatX& other) { *this = other; }
	INLINE_NO_DEBUG FloatX(const FloatY& other) { *this = other; }
	INLINE_NO_DEBUG FloatX(const FloatZ& other) { *this = other; }
	INLINE_NO_DEBUG FloatX(const FloatW& other) { *this = other; }
#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG FloatX(const FloatShort& other) { *this = other; }
	INLINE_NO_DEBUG FloatX(const FloatShortX& other) { *this = other; }
	INLINE_NO_DEBUG FloatX(const FloatShortY& other) { *this = other; }
#endif

	template<class t_Val>
	INLINE_NO_DEBUG void operator += (const t_Val& other) { *this = *this + other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator -= (const t_Val& other) { *this = *this - other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator *= (const t_Val& other) { *this = *this * other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator /= (const t_Val& other) { *this = *this / other; }

private:
	float _value;
	friend struct SIMD4;
};

class FloatY
{
public:
	static const uint gIndex = 1;

	INLINE_NO_DEBUG FloatY()
	{
		Assert(((reinterpret_cast<Base::uinteger>(this) - 4) & 15) == 0);
	}

	INLINE_NO_DEBUG operator float& ()
	{
		return _value; 
	}

	INLINE_NO_DEBUG operator const float& ()const
	{
		return _value; 
	}

	INLINE_NO_DEBUG void operator = (float flt)
	{
		_value = flt;
	}

	INLINE_NO_DEBUG void operator = (const Float& f);
	INLINE_NO_DEBUG void operator = (const FloatX& f);
	INLINE_NO_DEBUG void operator = (const FloatY& f);
	INLINE_NO_DEBUG void operator = (const FloatZ& f);
	INLINE_NO_DEBUG void operator = (const FloatW& f);
#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG void operator = (const FloatShort& f);
	INLINE_NO_DEBUG void operator = (const FloatShortX& f);
	INLINE_NO_DEBUG void operator = (const FloatShortY& f);
#endif

	INLINE_NO_DEBUG SIMD4& simd();
	INLINE_NO_DEBUG const SIMD4& simd()const;

	INLINE_NO_DEBUG FloatY(const Float& other) { *this = other; }
	INLINE_NO_DEBUG FloatY(const FloatX& other) { *this = other; }
	INLINE_NO_DEBUG FloatY(const FloatY& other) { *this = other; }
	INLINE_NO_DEBUG FloatY(const FloatZ& other) { *this = other; }
	INLINE_NO_DEBUG FloatY(const FloatW& other) { *this = other; }
#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG FloatY(const FloatShort& other) { *this = other; }
	INLINE_NO_DEBUG FloatY(const FloatShortX& other) { *this = other; }
	INLINE_NO_DEBUG FloatY(const FloatShortY& other) { *this = other; }
#endif

	template<class t_Val>
	INLINE_NO_DEBUG void operator += (const t_Val& other) { *this = *this + other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator -= (const t_Val& other) { *this = *this - other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator *= (const t_Val& other) { *this = *this * other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator /= (const t_Val& other) { *this = *this / other; }

private:
	float _value;
	friend struct SIMD4;
};

class FloatZ
{
public:
	static const uint gIndex = 2;

	INLINE_NO_DEBUG FloatZ()
	{
		Assert(((reinterpret_cast<Base::uinteger>(this) - 8) & 15) == 0);
	}

	INLINE_NO_DEBUG operator float& ()
	{
		return _value; 
	}

	INLINE_NO_DEBUG operator const float& ()const
	{
		return _value; 
	}

	INLINE_NO_DEBUG void operator = (float flt)
	{
		_value = flt;
	}

	INLINE_NO_DEBUG void operator = (const Float& f);
	INLINE_NO_DEBUG void operator = (const FloatX& f);
	INLINE_NO_DEBUG void operator = (const FloatY& f);
	INLINE_NO_DEBUG void operator = (const FloatZ& f);
	INLINE_NO_DEBUG void operator = (const FloatW& f);
#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG void operator = (const FloatShort& f);
	INLINE_NO_DEBUG void operator = (const FloatShortX& f);
	INLINE_NO_DEBUG void operator = (const FloatShortY& f);
#endif

	INLINE_NO_DEBUG SIMD4& simd();
	INLINE_NO_DEBUG const SIMD4& simd()const;

	INLINE_NO_DEBUG FloatZ(const Float& other) { *this = other; }
	INLINE_NO_DEBUG FloatZ(const FloatX& other) { *this = other; }
	INLINE_NO_DEBUG FloatZ(const FloatY& other) { *this = other; }
	INLINE_NO_DEBUG FloatZ(const FloatZ& other) { *this = other; }
	INLINE_NO_DEBUG FloatZ(const FloatW& other) { *this = other; }
#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG FloatZ(const FloatShort& other) { *this = other; }
	INLINE_NO_DEBUG FloatZ(const FloatShortX& other) { *this = other; }
	INLINE_NO_DEBUG FloatZ(const FloatShortY& other) { *this = other; }
#endif

	template<class t_Val>
	INLINE_NO_DEBUG void operator += (const t_Val& other) { *this = *this + other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator -= (const t_Val& other) { *this = *this - other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator *= (const t_Val& other) { *this = *this * other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator /= (const t_Val& other) { *this = *this / other; }

private:
	float _value;
	friend struct SIMD4;
};

class FloatW
{
public:
	static const uint gIndex = 3;
	INLINE_NO_DEBUG FloatW()
	{
		Assert(((reinterpret_cast<Base::uinteger>(this) - 12) & 15) == 0);
	}

	INLINE_NO_DEBUG operator float& ()
	{
		return _value; 
	}

	INLINE_NO_DEBUG operator const float& ()const
	{
		return _value; 
	}

	INLINE_NO_DEBUG void operator = (float flt)
	{
		_value = flt;
	}

	INLINE_NO_DEBUG void operator = (const Float& f);
	INLINE_NO_DEBUG void operator = (const FloatX& f);
	INLINE_NO_DEBUG void operator = (const FloatY& f);
	INLINE_NO_DEBUG void operator = (const FloatZ& f);
	INLINE_NO_DEBUG void operator = (const FloatW& f);
#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG void operator = (const FloatShort& f);
	INLINE_NO_DEBUG void operator = (const FloatShortX& f);
	INLINE_NO_DEBUG void operator = (const FloatShortY& f);
#endif
	
	INLINE_NO_DEBUG SIMD4& simd();
	INLINE_NO_DEBUG const SIMD4& simd()const;

	INLINE_NO_DEBUG FloatW(const Float& other) { *this = other; }
	INLINE_NO_DEBUG FloatW(const FloatX& other) { *this = other; }
	INLINE_NO_DEBUG FloatW(const FloatY& other) { *this = other; }
	INLINE_NO_DEBUG FloatW(const FloatZ& other) { *this = other; }
	INLINE_NO_DEBUG FloatW(const FloatW& other) { *this = other; }
#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG FloatW(const FloatShort& other) { *this = other; }
	INLINE_NO_DEBUG FloatW(const FloatShortX& other) { *this = other; }
	INLINE_NO_DEBUG FloatW(const FloatShortY& other) { *this = other; }
#endif

	template<class t_Val>
	INLINE_NO_DEBUG void operator += (const t_Val& other) { *this = *this + other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator -= (const t_Val& other) { *this = *this - other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator *= (const t_Val& other) { *this = *this * other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator /= (const t_Val& other) { *this = *this / other; }

private:
	float _value;
	friend struct SIMD4;
};

#ifdef INSTRUCTIONS_NEON
class FloatShortX
{
public:
	static const uint gIndex = 0;

	INLINE_NO_DEBUG FloatShortX()
	{
		Assert((reinterpret_cast<Base::uinteger>(this) & 7) == 0);
	}

	INLINE_NO_DEBUG operator float& ()
	{
		return _value; 
	}

	INLINE_NO_DEBUG operator const float& ()const
	{
		return _value; 
	}

	INLINE_NO_DEBUG void operator = (float flt)
	{
		_value = flt;
	}

	INLINE_NO_DEBUG void operator = (const Float& f);
	INLINE_NO_DEBUG void operator = (const FloatX& f);
	INLINE_NO_DEBUG void operator = (const FloatY& f);
	INLINE_NO_DEBUG void operator = (const FloatZ& f);
	INLINE_NO_DEBUG void operator = (const FloatW& f);
	INLINE_NO_DEBUG void operator = (const FloatShort& f);
	INLINE_NO_DEBUG void operator = (const FloatShortX& f);
	INLINE_NO_DEBUG void operator = (const FloatShortY& f);

	INLINE_NO_DEBUG SIMD2& simd();
	INLINE_NO_DEBUG const SIMD2& simd()const;

	INLINE_NO_DEBUG FloatShortX(const Float& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortX(const FloatX& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortX(const FloatY& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortX(const FloatZ& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortX(const FloatW& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortX(const FloatShort& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortX(const FloatShortX& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortX(const FloatShortY& other) { *this = other; }

	template<class t_Val>
	INLINE_NO_DEBUG void operator += (const t_Val& other) { *this = *this + other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator -= (const t_Val& other) { *this = *this - other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator *= (const t_Val& other) { *this = *this * other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator /= (const t_Val& other) { *this = *this / other; }

private:
	float _value;
	friend struct SIMD2;
};

class FloatShortY
{
public:
	static const uint gIndex = 1;

	INLINE_NO_DEBUG FloatY()
	{
		Assert(((reinterpret_cast<Base::uinteger>(this) - 4) & 7) == 0);
	}

	INLINE_NO_DEBUG operator float& ()
	{
		return _value; 
	}

	INLINE_NO_DEBUG operator const float& ()const
	{
		return _value; 
	}

	INLINE_NO_DEBUG void operator = (float flt)
	{
		_value = flt;
	}

	INLINE_NO_DEBUG void operator = (const Float& f);
	INLINE_NO_DEBUG void operator = (const FloatX& f);
	INLINE_NO_DEBUG void operator = (const FloatY& f);
	INLINE_NO_DEBUG void operator = (const FloatZ& f);
	INLINE_NO_DEBUG void operator = (const FloatW& f);
	INLINE_NO_DEBUG void operator = (const FloatShort& f);
	INLINE_NO_DEBUG void operator = (const FloatShortX& f);
	INLINE_NO_DEBUG void operator = (const FloatShortY& f);

	INLINE_NO_DEBUG SIMD2& simd();
	INLINE_NO_DEBUG const SIMD2& simd()const;

	INLINE_NO_DEBUG FloatShortY(const Float& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortY(const FloatX& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortY(const FloatY& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortY(const FloatZ& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortY(const FloatW& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortY(const FloatShort& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortY(const FloatShortX& other) { *this = other; }
	INLINE_NO_DEBUG FloatShortY(const FloatShortY& other) { *this = other; }

	template<class t_Val>
	INLINE_NO_DEBUG void operator += (const t_Val& other) { *this = *this + other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator -= (const t_Val& other) { *this = *this - other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator *= (const t_Val& other) { *this = *this * other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator /= (const t_Val& other) { *this = *this / other; }

private:
	float _value;
	friend struct SIMD2;
};
#endif


struct DECLARE_ALIGN(16) SIMD4
{
#ifdef INSTRUCTIONS_SSE
	union 
	{
		__m128 v;
		struct { float a[4]; };
		struct
		{
			FloatX x;
			FloatY y;
			FloatZ z;
			FloatW w;
		};
	};
#endif

	INLINE_NO_DEBUG SIMD4()
	{
		Assert((reinterpret_cast<Base::uinteger>(this) & 15) == 0); 
	}

	INLINE_NO_DEBUG SIMD4(const SIMD4& other)
	{
#ifdef INSTRUCTIONS_SSE
		v = other.v;
#endif
	}

	INLINE_NO_DEBUG SIMD4& operator = (const SIMD4& other)
	{
#ifdef INSTRUCTIONS_SSE
		v = other.v;
#endif
		return *this;
	}


	INLINE_NO_DEBUG SIMD4& simd() { return *this; }
	INLINE_NO_DEBUG const SIMD4& simd()const { return *this; }

	INLINE_NO_DEBUG void setZero()
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_setzero_ps();
#endif
	}

	INLINE_NO_DEBUG void setOne()
	{
#ifdef INSTRUCTIONS_SSE
		static const __m128 x = _mm_set1_ps(1.0f);
		v = x;
#endif
	}

	INLINE_NO_DEBUG void setXYZW1100()
	{
#ifdef INSTRUCTIONS_SSE
		static const __m128 x = _mm_set_ps(0.0f, 0.0f, 1.0f, 1.0f);
		v = x;
#endif
	}

	INLINE_NO_DEBUG void setXYZW1110()
	{
#ifdef INSTRUCTIONS_SSE
		static const __m128 x = _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f);
		v = x;
#endif
	}

	static INLINE_NO_DEBUG SIMD4 getZero()
	{
#ifdef INSTRUCTIONS_SSE
		return *(SIMD4*)&_mm_setzero_ps();
#endif
	}

	static INLINE_NO_DEBUG SIMD4 getOne()
	{
#ifdef INSTRUCTIONS_SSE
		static const __m128 one = _mm_set1_ps(1.0f);
		return *(SIMD4*)&one;
#endif
	}

	static INLINE_NO_DEBUG SIMD4 getXYZW1100()
	{
#ifdef INSTRUCTIONS_SSE
		static const __m128 x = _mm_set_ps(0.0f, 0.0f, 1.0f, 1.0f);
		return *(SIMD4*)&x;
#endif
	}

	static INLINE_NO_DEBUG SIMD4 getXYZW1110()
	{
#ifdef INSTRUCTIONS_SSE
		static const __m128 x = _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f);
		return *(SIMD4*)&x;
#endif
	}
};

#ifdef INSTRUCTIONS_NEON
struct DECLARE_ALIGN(8) SIMD2
{
	union 
	{
		float32x2_t v;
		struct { float a[2]; };
		struct
		{
			FloatShortX x;
			FloatShortY y;
		};
	};

	INLINE_NO_DEBUG SIMD2()
	{
		Assert((reinterpret_cast<Base::uinteger>(this) & 7) == 0); 
	}

	INLINE_NO_DEBUG SIMD2(const SIMD2& other)
	{
		v = other.v;
	}

	INLINE_NO_DEBUG SIMD2& operator = (const SIMD2& other)
	{
		v = other.v;
		return *this;
	}

	INLINE_NO_DEBUG SIMD2& simd() { return *this; }
	INLINE_NO_DEBUG const SIMD2& simd()const { return *this; }

	INLINE_NO_DEBUG void setZero()
	{
		
	}

	INLINE_NO_DEBUG void setOne()
	{

	}

	static INLINE_NO_DEBUG SIMD4 getZero()
	{

	}

	static INLINE_NO_DEBUG SIMD4 getOne()
	{
	}
};
#endif


INLINE_NO_DEBUG const SIMD4& FloatX::simd()const
{
	return *reinterpret_cast<const SIMD4*> (this - gIndex);
}

INLINE_NO_DEBUG const SIMD4& FloatY::simd()const
{
	return *reinterpret_cast<const SIMD4*> (this - gIndex);
}

INLINE_NO_DEBUG const SIMD4& FloatZ::simd()const
{
	return *reinterpret_cast<const SIMD4*> (this - gIndex);
}

INLINE_NO_DEBUG const SIMD4& FloatW::simd()const
{
	return *reinterpret_cast<const SIMD4*> (this - gIndex);
}


INLINE_NO_DEBUG SIMD4& FloatX::simd()
{
	return *reinterpret_cast<SIMD4*> (this - gIndex);
}

INLINE_NO_DEBUG SIMD4& FloatY::simd()
{
	return *reinterpret_cast<SIMD4*> (this - gIndex);
}

INLINE_NO_DEBUG SIMD4& FloatZ::simd()
{
	return *reinterpret_cast<SIMD4*> (this - gIndex);
}

INLINE_NO_DEBUG SIMD4& FloatW::simd()
{
	return *reinterpret_cast<SIMD4*> (this - gIndex);
}

#ifdef INSTRUCTIONS_NEON
INLINE_NO_DEBUG const SIMD2& FloatShortX::simd()const
{
	return *reinterpret_cast<const SIMD2*> (this - gIndex);
}

INLINE_NO_DEBUG const SIMD2& FloatShortY::simd()const
{
	return *reinterpret_cast<const SIMD2*> (this - gIndex);
}

INLINE_NO_DEBUG SIMD2& FloatShortX::simd()
{
	return *reinterpret_cast<SIMD2*> (this - gIndex);
}

INLINE_NO_DEBUG SIMD2& FloatShortY::simd()
{
	return *reinterpret_cast<SIMD2*> (this - gIndex);
}
#endif

struct Float : public SIMD4
{
	static const uint gIndex = 0;

	INLINE_NO_DEBUG Float(){}

	INLINE_NO_DEBUG Float(const Float& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = f.v;
#endif
	}

	INLINE_NO_DEBUG Float& operator = (const Float& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = f.v;
#endif
		return *this;
	}

	INLINE_NO_DEBUG Float(float f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps(f);
#endif
	}

	INLINE_NO_DEBUG Float(int8 x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(uint8 x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(int16 x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(uint16 x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(int32 x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(uint32 x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(int64 x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(uint64 x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(double x)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps((float)x);
#endif
	}

	INLINE_NO_DEBUG Float(const FloatX& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(f.simd().v, f.simd().v, _MM_SHUFFLE(0,0,0,0));
#endif
	}

	INLINE_NO_DEBUG Float(const FloatY& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(f.simd().v, f.simd().v, _MM_SHUFFLE(1,1,1,1));
#endif
	}

	INLINE_NO_DEBUG Float(const FloatZ& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(f.simd().v, f.simd().v, _MM_SHUFFLE(2,2,2,2));
#endif
	}

	INLINE_NO_DEBUG Float(const FloatW& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(f.simd().v, f.simd().v, _MM_SHUFFLE(3,3,3,3));
#endif
	}

#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG Float(const FloatShort& f)
	{
	}

	INLINE_NO_DEBUG Float(const FloatShortX& f)
	{
	}

	INLINE_NO_DEBUG Float(const FloatShortY& f)
	{
	}
#endif


	INLINE_NO_DEBUG Float& operator = (float f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps(f);
#endif
		return *this;
	}

	INLINE_NO_DEBUG Float& operator = (int8 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (uint8 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (int16 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (uint16 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (int32 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (uint32 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (int64 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (uint64 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (const FloatX& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(f.simd().v, f.simd().v, _MM_SHUFFLE(0,0,0,0));
#endif
		return *this;
	}

	INLINE_NO_DEBUG Float& operator = (const FloatY& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(f.simd().v, f.simd().v, _MM_SHUFFLE(1,1,1,1));
#endif
		return *this;
	}

	INLINE_NO_DEBUG Float& operator = (const FloatZ& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(f.simd().v, f.simd().v, _MM_SHUFFLE(2,2,2,2));
#endif
		return *this;
	}

	INLINE_NO_DEBUG Float& operator = (const FloatW& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(f.simd().v, f.simd().v, _MM_SHUFFLE(3,3,3,3));
#endif
		return *this;
	}

#ifdef INSTRUCTIONS_NEON
	INLINE_NO_DEBUG Float& operator = (const FloatShort& f)
	{
	}

	INLINE_NO_DEBUG Float& operator = (const FloatShortX& f)
	{
	}

	INLINE_NO_DEBUG Float& operator = (const FloatShortY& f)
	{
	}
#endif

	INLINE_NO_DEBUG operator float ()const
	{
		return a[0]; 
	}

	INLINE_NO_DEBUG Float operator + ()const
	{
#ifdef INSTRUCTIONS_SSE
		return *this;
#endif
	}

	INLINE_NO_DEBUG Float operator - ()const
	{
#ifdef INSTRUCTIONS_SSE
		return *(Float*)&_mm_sub_ps(_mm_setzero_ps(), v);
#endif
	}

	template<class t_Val>
	INLINE_NO_DEBUG void operator += (const t_Val& other) { *this = *this + other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator -= (const t_Val& other) { *this = *this - other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator *= (const t_Val& other) { *this = *this * other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator /= (const t_Val& other) { *this = *this / other; }
};

#ifdef INSTRUCTIONS_NEON
struct FloatShort : public SIMD2
{
	static const uint gIndex = 0;

	INLINE_NO_DEBUG FloatShort(){}

	INLINE_NO_DEBUG FloatShort(const FloatShort& f)
	{
		v = f.v;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const FloatShort& f)
	{
		v = f.v;
		return *this;
	}

	INLINE_NO_DEBUG FloatShort(float f)
	{
	}

	INLINE_NO_DEBUG FloatShort(int8 x)
	{
	}

	INLINE_NO_DEBUG FloatShort(uint8 x)
	{
	}

	INLINE_NO_DEBUG FloatShort(int16 x)
	{
	}

	INLINE_NO_DEBUG FloatShort(uint16 x)
	{
	}

	INLINE_NO_DEBUG FloatShort(int32 x)
	{
	}

	INLINE_NO_DEBUG FloatShort(uint32 x)
	{
	}

	INLINE_NO_DEBUG FloatShort(int64 x)
	{
	}

	INLINE_NO_DEBUG FloatShort(uint64 x)
	{
	}

	INLINE_NO_DEBUG FloatShort(double x)
	{
	}

	INLINE_NO_DEBUG FloatShort(const Float& f)
	{

	}

	INLINE_NO_DEBUG FloatShort(const FloatX& f)
	{

	}

	INLINE_NO_DEBUG FloatShort(const FloatY& f)
	{

	}

	INLINE_NO_DEBUG FloatShort(const FloatZ& f)
	{

	}

	INLINE_NO_DEBUG FloatShort(const FloatW& f)
	{

	}

	INLINE_NO_DEBUG FloatShort(const FloatShort& f)
	{

	}

	INLINE_NO_DEBUG FloatShort(const FloatShortX& f)
	{

	}

	INLINE_NO_DEBUG FloatShort(const FloatShortY& f)
	{

	}

	
	INLINE_NO_DEBUG Float& operator = (float f)
	{
		return *this;
	}

	INLINE_NO_DEBUG Float& operator = (int8 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (uint8 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (int16 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (uint16 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (int32 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (uint32 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (int64 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG Float& operator = (uint64 x)
	{
		return *this = (float)x;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const Float& f)
	{
		return *this;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const FloatX& f)
	{
		return *this;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const FloatY& f)
	{
		return *this;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const FloatZ& f)
	{
		return *this;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const FloatW& f)
	{
		return *this;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const FloatShort& f)
	{
		return *this;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const FloatShortX& f)
	{
		return *this;
	}

	INLINE_NO_DEBUG FloatShort& operator = (const FloatShortY& f)
	{
		return *this;
	}

	INLINE_NO_DEBUG operator float ()const
	{
		return a[0]; 
	}

	INLINE_NO_DEBUG FloatShort operator + ()const
	{
		return *this;
	}

	INLINE_NO_DEBUG FloatShort operator - ()const
	{
	}

	template<class t_Val>
	INLINE_NO_DEBUG void operator += (const t_Val& other) { *this = *this + other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator -= (const t_Val& other) { *this = *this - other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator *= (const t_Val& other) { *this = *this * other; }
	template<class t_Val>
	INLINE_NO_DEBUG void operator /= (const t_Val& other) { *this = *this / other; }
};
#endif

INLINE_NO_DEBUG void FloatX::operator = (const Float& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_unpacklo_ps(f.v, simd().v), simd().v, _MM_SHUFFLE(3,2,3,0));
#endif
}

INLINE_NO_DEBUG void FloatX::operator = (const FloatX& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_unpacklo_ps(f.simd().v, simd().v), simd().v, _MM_SHUFFLE(3,2,3,0));
#endif
}

INLINE_NO_DEBUG void FloatX::operator = (const FloatY& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_unpacklo_ps(f.simd().v, simd().v), simd().v, _MM_SHUFFLE(3,2,3,2));
#endif
}

INLINE_NO_DEBUG void FloatX::operator = (const FloatZ& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_shuffle_ps(f.simd().v, simd().v, _MM_SHUFFLE(1,1,2,2)), simd().v, _MM_SHUFFLE(3,2,2,0));
#endif
}

INLINE_NO_DEBUG void FloatX::operator = (const FloatW& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_shuffle_ps(f.simd().v, simd().v, _MM_SHUFFLE(1,1,3,3)), simd().v, _MM_SHUFFLE(3,2,2,0));
#endif
}

#ifdef INSTRUCTIONS_NEON
INLINE_NO_DEBUG void FloatX::operator = (const FloatShort& f)
{

}

INLINE_NO_DEBUG void FloatX::operator = (const FloatShortX& f)
{

}

INLINE_NO_DEBUG void FloatX::operator = (const FloatShortY& f)
{

}
#endif



INLINE_NO_DEBUG void FloatY::operator = (const Float& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_unpacklo_ps(simd().v, f.v), simd().v, _MM_SHUFFLE(3,2,1,0));
#endif
}

INLINE_NO_DEBUG void FloatY::operator = (const FloatX& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_unpacklo_ps(simd().v, f.simd().v), simd().v, _MM_SHUFFLE(3,2,1,0));
#endif
}

INLINE_NO_DEBUG void FloatY::operator = (const FloatY& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_unpacklo_ps(simd().v, f.simd().v), simd().v, _MM_SHUFFLE(3,2,3,0));
#endif
}

INLINE_NO_DEBUG void FloatY::operator = (const FloatZ& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_shuffle_ps(simd().v, f.simd().v, _MM_SHUFFLE(2,2,0,0)), simd().v, _MM_SHUFFLE(3,2,2,0));
#endif
}

INLINE_NO_DEBUG void FloatY::operator = (const FloatW& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(_mm_shuffle_ps(simd().v, f.simd().v, _MM_SHUFFLE(3,3,0,0)), simd().v, _MM_SHUFFLE(3,2,2,0));
#endif
}

#ifdef INSTRUCTIONS_NEON
INLINE_NO_DEBUG void FloatY::operator = (const FloatShort& f)
{

}

INLINE_NO_DEBUG void FloatY::operator = (const FloatShortX& f)
{

}

INLINE_NO_DEBUG void FloatY::operator = (const FloatShortY& f)
{

}
#endif




INLINE_NO_DEBUG void FloatZ::operator = (const Float& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_unpackhi_ps(f.v, simd().v), _MM_SHUFFLE(3,0,1,0));
#endif
}

INLINE_NO_DEBUG void FloatZ::operator = (const FloatX& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_shuffle_ps(f.simd().v, simd().v, _MM_SHUFFLE(3,3,0,0)), _MM_SHUFFLE(3,0,1,0));
#endif
}

INLINE_NO_DEBUG void FloatZ::operator = (const FloatY& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_shuffle_ps(f.simd().v, simd().v, _MM_SHUFFLE(3,3,1,1)), _MM_SHUFFLE(3,0,1,0));
#endif
}

INLINE_NO_DEBUG void FloatZ::operator = (const FloatZ& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_unpackhi_ps(f.simd().v, simd().v), _MM_SHUFFLE(3,0,1,0));
#endif
}

INLINE_NO_DEBUG void FloatZ::operator = (const FloatW& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_unpackhi_ps(f.simd().v, simd().v), _MM_SHUFFLE(3,2,1,0));
#endif
}

#ifdef INSTRUCTIONS_NEON
INLINE_NO_DEBUG void FloatZ::operator = (const FloatShort& f)
{

}

INLINE_NO_DEBUG void FloatZ::operator = (const FloatShortX& f)
{

}

INLINE_NO_DEBUG void FloatZ::operator = (const FloatShortY& f)
{

}
#endif




INLINE_NO_DEBUG void FloatW::operator = (const Float& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_unpackhi_ps(simd().v, f.v), _MM_SHUFFLE(1,0,1,0));
#endif
}

INLINE_NO_DEBUG void FloatW::operator = (const FloatX& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_shuffle_ps(simd().v, f.simd().v, _MM_SHUFFLE(0,0,2,2)), _MM_SHUFFLE(3,0,1,0));
#endif
}

INLINE_NO_DEBUG void FloatW::operator = (const FloatY& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_shuffle_ps(simd().v, f.simd().v, _MM_SHUFFLE(1,1,2,2)), _MM_SHUFFLE(3,0,1,0));
#endif
}

INLINE_NO_DEBUG void FloatW::operator = (const FloatZ& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_unpackhi_ps(simd().v, f.simd().v), _MM_SHUFFLE(1,0,1,0));
#endif
}

INLINE_NO_DEBUG void FloatW::operator = (const FloatW& f)
{
#ifdef INSTRUCTIONS_SSE
	simd().v = _mm_shuffle_ps(simd().v, _mm_unpackhi_ps(simd().v, f.simd().v), _MM_SHUFFLE(3,0,1,0));
#endif
}

#ifdef INSTRUCTIONS_NEON
INLINE_NO_DEBUG void FloatW::operator = (const FloatShort& f)
{

}

INLINE_NO_DEBUG void FloatW::operator = (const FloatShortX& f)
{

}

INLINE_NO_DEBUG void FloatW::operator = (const FloatShortY& f)
{

}
#endif



#ifdef INSTRUCTIONS_NEON

INLINE_NO_DEBUG void FloatShortX::operator = (const Float& f)
{
}

INLINE_NO_DEBUG void FloatShortX::operator = (const FloatX& f)
{
}

INLINE_NO_DEBUG void FloatShortX::operator = (const FloatY& f)
{
}

INLINE_NO_DEBUG void FloatShortX::operator = (const FloatZ& f)
{
}

INLINE_NO_DEBUG void FloatShortX::operator = (const FloatW& f)
{
}

INLINE_NO_DEBUG void FloatShortX::operator = (const FloatShort& f)
{

}

INLINE_NO_DEBUG void FloatShortX::operator = (const FloatShortX& f)
{

}

INLINE_NO_DEBUG void FloatShortX::operator = (const FloatShortY& f)
{

}



INLINE_NO_DEBUG void FloatShortY::operator = (const Float& f)
{
}

INLINE_NO_DEBUG void FloatShortY::operator = (const FloatX& f)
{
}

INLINE_NO_DEBUG void FloatShortY::operator = (const FloatY& f)
{
}

INLINE_NO_DEBUG void FloatShortY::operator = (const FloatZ& f)
{
}

INLINE_NO_DEBUG void FloatShortY::operator = (const FloatW& f)
{
}

INLINE_NO_DEBUG void FloatShortY::operator = (const FloatShort& f)
{

}

INLINE_NO_DEBUG void FloatShortY::operator = (const FloatShortX& f)
{

}

INLINE_NO_DEBUG void FloatShortY::operator = (const FloatShortY& f)
{

}

#endif


static INLINE_NO_DEBUG void swap(FloatX& a, FloatX& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatX& a, FloatY& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatX& a, FloatZ& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatX& a, FloatW& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatX& a, Float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatX& a, float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatY& a, FloatX& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatY& a, FloatY& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatY& a, FloatZ& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatY& a, FloatW& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatY& a, Float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatY& a, float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatZ& a, FloatX& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatZ& a, FloatY& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatZ& a, FloatZ& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatZ& a, FloatW& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatZ& a, Float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatZ& a, float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatW& a, FloatX& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatW& a, FloatY& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatW& a, FloatZ& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatW& a, FloatW& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatW& a, Float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatW& a, float& b){ Float c(a); a = b; b = c; }

#ifdef INSTRUCTIONS_NEON
static INLINE_NO_DEBUG void swap(FloatShortX& a, Float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& a, FloatX& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& a, FloatY& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& a, FloatZ& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& a, FloatW& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& a, FloatShort& b){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& a, FloatShortX& b){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& a, FloatShortY& b){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& a, float& b){ FloatShort c(a); a = b; b = c; }

static INLINE_NO_DEBUG void swap(Float& b, FloatShortX& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatX& b, FloatShortX& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatY& b, FloatShortX& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatZ& b, FloatShortX& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatW& b, FloatShortX& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShort& b, FloatShortX& a){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& b, FloatShortX& a){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& b, FloatShortX& a){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(float& b, FloatShortX& a){ FloatShort c(a); a = b; b = c; }

static INLINE_NO_DEBUG void swap(FloatShortY& a, Float& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& a, FloatX& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& a, FloatY& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& a, FloatZ& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& a, FloatW& b){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& a, FloatShort& b){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& a, FloatShortX& b){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& a, FloatShortY& b){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& a, float& b){ FloatShort c(a); a = b; b = c; }

static INLINE_NO_DEBUG void swap(Float& b, FloatShortY& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatX& b, FloatShortY& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatY& b, FloatShortY& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatZ& b, FloatShortY& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatW& b, FloatShortY& a){ Float c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShort& b, FloatShortY& a){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortX& b, FloatShortY& a){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(FloatShortY& b, FloatShortY& a){ FloatShort c(a); a = b; b = c; }
static INLINE_NO_DEBUG void swap(float& b, FloatShortY& a){ FloatShort c(a); a = b; b = c; }

#endif


static INLINE_NO_DEBUG SIMD4 simd_add(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_add_ps(a.v, b.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_sub(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_sub_ps(a.v, b.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_mul(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_mul_ps(a.v, b.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_div(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_div_ps(a.v, b.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_rcp(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_rcp_ps(vec.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_rsq(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_rsqrt_ps(vec.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_sqrt(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_sqrt_ps(vec.v);
#endif
}

static INLINE_NO_DEBUG uint32 simd_noeqcmp(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	__m128 x = _mm_xor_ps(a.v, b.v);
	const uint32* p = (const uint32*)&x;
	return p[0] | p[1] | p[2] | p[3];
#endif
}

static INLINE_NO_DEBUG uint32 simd_noeqcmp3(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	__m128 x = _mm_xor_ps(a.v, b.v);
	const uint32* p = (const uint32*)&x;
	return p[0] | p[1] | p[2];
#endif
}

static INLINE_NO_DEBUG uint32 simd_noeqcmp2(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	__m128 x = _mm_xor_ps(a.v, b.v);
	const uint32* p = (const uint32*)&x;
	return p[0] | p[1];
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_div_by_xyz1(const SIMD4& a, const SIMD4& b)
{
	SIMD4 b2 = b;
	b2.a[3] = 1.0f;
	return simd_div(a, b2);
}

static INLINE_NO_DEBUG SIMD4 simd_rcpXYZ_zeroW(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	const __m128i mask = _mm_set_epi32(0, -1, -1, -1);
	return *(SIMD4*)&_mm_and_ps(_mm_rcp_ps(_mm_shuffle_ps(vec.v, vec.v, _MM_SHUFFLE(0,2,1,0))), *(const __m128*)&mask);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_rsqXYZ_zeroW(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	const __m128i mask = _mm_set_epi32(0, -1, -1, -1);
	return *(SIMD4*)&_mm_and_ps(_mm_rsqrt_ps(_mm_shuffle_ps(vec.v, vec.v, _MM_SHUFFLE(0,2,1,0))), *(const __m128*)&mask);
#endif
}


static INLINE_NO_DEBUG SIMD4 simd_div_by_xy11(const SIMD4& a, const SIMD4& b)
{
	SIMD4 b2 = b;
	b2.a[2] = 1.0f;
	b2.a[3] = 1.0f;
	return simd_div(a, b2);
}

static INLINE_NO_DEBUG SIMD4 simd_rcpXY_zeroZW(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	const __m128i mask = _mm_set_epi32(0, 0, -1, -1);
	return *(SIMD4*)&_mm_and_ps(_mm_rcp_ps(_mm_shuffle_ps(vec.v, vec.v, _MM_SHUFFLE(0,0,1,0))), *(const __m128*)&mask);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_rsqXY_zeroZW(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	const __m128i mask = _mm_set_epi32(0, 0, -1, -1);
	return *(SIMD4*)&_mm_and_ps(_mm_rsqrt_ps(_mm_shuffle_ps(vec.v, vec.v, _MM_SHUFFLE(0,0,1,0))), *(const __m128*)&mask);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_div_by_xyz1_clear_w(const SIMD4& a, const SIMD4& b)
{
	SIMD4 a2 = a;
	SIMD4 b2 = b;
	a2.a[3] = 0.0f;
	b2.a[3] = 1.0f;
	return simd_div(a, b2);
}

static INLINE_NO_DEBUG SIMD4 simd_div_by_xy11_clear_zw(const SIMD4& a, const SIMD4& b)
{
	SIMD4 a2 = a;
	SIMD4 b2 = b;
	a2.a[2] = 0.0f;
	a2.a[3] = 0.0f;
	b2.a[2] = 1.0f;
	b2.a[3] = 1.0f;
	return simd_div(a, b2);
}

static INLINE_NO_DEBUG SIMD4 simd_min(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_min_ps(a.v, b.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_max(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_max_ps(a.v, b.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_clamp(const SIMD4& x, const SIMD4& min, const SIMD4& max)
{
#ifdef INSTRUCTIONS_SSE
	return *(SIMD4*)&_mm_min_ps(_mm_max_ps(x.v, min.v), max.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_clamp01(const SIMD4& x)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128 one = _mm_set1_ps(1.0f);
	return *(Float*)&_mm_min_ps(_mm_max_ps(x.v, _mm_setzero_ps()), one);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_clamp11(const SIMD4& x)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128 one = _mm_set1_ps(1.0f);
	static const __m128 negone = _mm_set1_ps(-1.0f);
	return *(Float*)&_mm_min_ps(_mm_max_ps(x.v, negone), one);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_abs(const SIMD4& x)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128 sign_mask = _mm_set1_ps(-0.f);
	return *(SIMD4*)&_mm_andnot_ps(sign_mask, x.v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_sign(const SIMD4& x)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128 sign_mask = _mm_set1_ps(-0.f);
	static const __m128 one = _mm_set1_ps(1.0f);
	return *(SIMD4*)&_mm_or_ps(_mm_and_ps(sign_mask, x.v), one);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_sign3_w0(const SIMD4& x)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128 sign_mask = _mm_set_ps(0.0f, -0.0f, -0.0f, -0.0f);
	static const __m128 one = _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f);
	return *(SIMD4*)&_mm_or_ps(_mm_and_ps(sign_mask, x.v), one);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_sign2_zw00(const SIMD4& x)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128 sign_mask = _mm_set_ps(0.0f, 0.0f, -0.0f, -0.0f);
	static const __m128 one = _mm_set_ps(0.0f, 0.0f, 1.0f, 1.0f);
	return *(SIMD4*)&_mm_or_ps(_mm_and_ps(sign_mask, x.v), one);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_dot2(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	__m128 v = _mm_mul_ps(a.v, b.v);
	v = _mm_add_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,1)));
	return *(SIMD4*)&_mm_unpacklo_ps(v, v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_summ2(const SIMD4& v)
{
#ifdef INSTRUCTIONS_SSE
	__m128 s = _mm_add_ps(v.v, _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(0,0,0,1)));
	return *(SIMD4*)&_mm_unpacklo_ps(s, s);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_lenghtSquare2(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	__m128 v = _mm_mul_ps(vec.v, vec.v);
	v = _mm_add_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,1)));
	return *(SIMD4*)&_mm_unpacklo_ps(v, v);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_lenght2(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	__m128 v = _mm_mul_ps(vec.v, vec.v);
	v = _mm_add_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,1)));
	v = _mm_sqrt_ss(v);
	return *(SIMD4*)&_mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_invLenghtSquareRoot2(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	__m128 v = _mm_mul_ps(vec.v, vec.v);
	v = _mm_add_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,1)));
	v = _mm_sqrt_ss(v);
	v = _mm_rcp_ss(v);
	return *(SIMD4*)&_mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_invLenghtSquareRootFast2(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	__m128 v = _mm_mul_ps(vec.v, vec.v);
	v = _mm_add_ps(v, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,1)));
	v = _mm_rsqrt_ss(v);
	return *(SIMD4*)&_mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_dot(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	__m128 m = _mm_mul_ps(a.v, b.v);
	__m128 pairs = _mm_add_ps(m, _mm_shuffle_ps(m, m, _MM_SHUFFLE(2,3,0,1)));
	return *(SIMD4*)&_mm_add_ps(pairs, _mm_shuffle_ps(pairs, pairs, _MM_SHUFFLE(0,1,2,3)));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_summ(const SIMD4& v)
{
#ifdef INSTRUCTIONS_SSE
	__m128 pairs = _mm_add_ps(v.v, _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(2,3,0,1)));
	return *(SIMD4*)&_mm_add_ps(pairs, _mm_shuffle_ps(pairs, pairs, _MM_SHUFFLE(0,1,2,3)));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_lenghtSquare(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	__m128 m = _mm_mul_ps(vec.v, vec.v);
	__m128 pairs = _mm_add_ps(m, _mm_shuffle_ps(m, m, _MM_SHUFFLE(2,3,0,1)));
	return *(SIMD4*)&_mm_add_ps(pairs, _mm_shuffle_ps(pairs, pairs, _MM_SHUFFLE(0,1,2,3)));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_lenght(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	__m128 m = _mm_mul_ps(vec.v, vec.v);
	__m128 pairs = _mm_add_ps(m, _mm_shuffle_ps(m, m, _MM_SHUFFLE(2,3,0,1)));
	__m128 v = _mm_add_ps(pairs, _mm_shuffle_ps(pairs, pairs, _MM_SHUFFLE(0,1,2,3)));
	v = _mm_sqrt_ss(v);
	return *(SIMD4*)&_mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_invLenghtSquareRoot(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	__m128 m = _mm_mul_ps(vec.v, vec.v);
	__m128 pairs = _mm_add_ps(m, _mm_shuffle_ps(m, m, _MM_SHUFFLE(2,3,0,1)));
	__m128 v = _mm_add_ps(pairs, _mm_shuffle_ps(pairs, pairs, _MM_SHUFFLE(0,1,2,3)));
	v = _mm_sqrt_ss(v);
	v = _mm_rcp_ss(v);
	return *(SIMD4*)&_mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_invLenghtSquareRootFast(const SIMD4& vec)
{
#ifdef INSTRUCTIONS_SSE
	__m128 m = _mm_mul_ps(vec.v, vec.v);
	__m128 pairs = _mm_add_ps(m, _mm_shuffle_ps(m, m, _MM_SHUFFLE(2,3,0,1)));
	__m128 v = _mm_add_ps(pairs, _mm_shuffle_ps(pairs, pairs, _MM_SHUFFLE(0,1,2,3)));
	v = _mm_rsqrt_ss(v);
	return *(SIMD4*)&_mm_shuffle_ps(v, v, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_cross3(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	__m128 a1 = _mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3,0,2,1));
	__m128 a2 = _mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3,1,0,2));
	__m128 b1 = _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3,1,0,2));
	__m128 b2 = _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3,0,2,1));
	return *(SIMD4*)& _mm_sub_ps(_mm_mul_ps(a1, b1), _mm_mul_ps(a2, b2));
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_quat_conj(const SIMD4& q)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128i mask = _mm_set_epi32(0, 0x80000000, 0x80000000, 0x80000000);
	return *(SIMD4*)& _mm_xor_ps(q.v, *(const __m128*)&mask);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_quat_mult(const SIMD4& a, const SIMD4& b)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128i mask = _mm_set_epi32(0x80000000, 0, 0, 0);
	__m128 q = 
		_mm_mul_ps(_mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(3,3,3,3)), _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(3,2,1,0)));
	__m128 p =
		_mm_mul_ps(_mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(0,2,1,0)), _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(0,3,3,3)));
	q = _mm_add_ps(q, _mm_xor_ps(p, *(const __m128*)&mask));
	p = _mm_mul_ps(_mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(1,0,2,1)), _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(1,1,0,2)));
	q = _mm_add_ps(q, _mm_xor_ps(p, *(const __m128*)&mask));
	p = _mm_mul_ps(_mm_shuffle_ps(a.v, a.v, _MM_SHUFFLE(2,1,0,2)), _mm_shuffle_ps(b.v, b.v, _MM_SHUFFLE(2,0,2,1)));
	q = _mm_sub_ps(q, p);
	return *(SIMD4*)&q;

	//q.a[0] = a[3]*b[0] + a[0]*b[3] + a[1]*b[2] - a[2]*b[1];
	//q.a[1] = a[3]*b[1] + a[1]*b[3] + a[2]*b[0] - a[0]*b[2];
	//q.a[2] = a[3]*b[2] + a[2]*b[3] + a[0]*b[1] - a[1]*b[0];
	//q.a[3] = a[3]*b[3] - a[0]*b[0] - a[1]*b[1] - a[2]*b[2];

#endif
}

static INLINE_NO_DEBUG void simd_m22_transpose(SIMD4& retX, SIMD4& retY, const SIMD4& X, const SIMD4& Y)
{
#ifdef INSTRUCTIONS_SSE
	retX.v = _mm_shuffle_ps(_mm_unpacklo_ps(X.v, Y.v), X.v, _MM_SHUFFLE(3,2,1,0));
	retY.v = _mm_shuffle_ps(_mm_unpacklo_ps(X.v, Y.v), X.v, _MM_SHUFFLE(3,2,3,2));
#endif
}

static INLINE_NO_DEBUG void simd_m33_transpose(SIMD4& retX, SIMD4& retY, SIMD4& retZ, const SIMD4& X, const SIMD4& Y, const SIMD4& Z)
{
#ifdef INSTRUCTIONS_SSE
	retX.v = _mm_shuffle_ps(_mm_unpacklo_ps(X.v, Y.v), Z.v, _MM_SHUFFLE(3,0,1,0));
	retY.v = _mm_shuffle_ps(_mm_unpacklo_ps(X.v, Y.v), Z.v, _MM_SHUFFLE(3,1,3,2));
	retZ.v = _mm_shuffle_ps(_mm_unpackhi_ps(X.v, Y.v), Z.v, _MM_SHUFFLE(3,2,1,0));
#endif
}

static INLINE_NO_DEBUG void simd_m44_transpose(SIMD4& retX, SIMD4& retY, SIMD4& retZ, SIMD4& retW, const SIMD4& X, const SIMD4& Y, const SIMD4& Z, const SIMD4& W)
{
#ifdef INSTRUCTIONS_SSE
	retX.v = _mm_shuffle_ps(_mm_unpacklo_ps(X.v, Y.v), _mm_unpacklo_ps(Z.v, W.v), _MM_SHUFFLE(1,0,1,0));
	retY.v = _mm_shuffle_ps(_mm_unpacklo_ps(X.v, Y.v), _mm_unpacklo_ps(Z.v, W.v), _MM_SHUFFLE(3,2,3,2));
	retZ.v = _mm_shuffle_ps(_mm_unpackhi_ps(X.v, Y.v), _mm_unpackhi_ps(Z.v, W.v), _MM_SHUFFLE(1,0,1,0));
	retW.v = _mm_shuffle_ps(_mm_unpackhi_ps(X.v, Y.v), _mm_unpackhi_ps(Z.v, W.v), _MM_SHUFFLE(3,2,3,2));
#endif
}


static INLINE_NO_DEBUG SIMD4 simd_quat_rotate_vec(const SIMD4& q, const SIMD4& v)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128i mask = _mm_set_epi32(0x80000000, 0, 0, 0);
	static const __m128i mask2 = _mm_set_epi32(0, -1, -1, -1);
	__m128 tmp = 
		_mm_mul_ps(_mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(0,3,3,3)), _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(0,2,1,0)));
	__m128 pls =
		_mm_mul_ps(_mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(1,0,2,1)), _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(1,1,0,2)));
	tmp = _mm_xor_ps(_mm_add_ps(tmp, pls), *(const __m128*)&mask);
	pls = _mm_mul_ps(_mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(2,1,0,2)), _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(2,0,2,1)));
	tmp = _mm_sub_ps(tmp, pls);

	__m128 ret = 
		_mm_mul_ps(_mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3,1,0,2)), _mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(3,0,2,1)));
	pls = _mm_mul_ps(_mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3,2,1,0)), _mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(3,3,3,3)));
	ret = _mm_add_ps(ret, pls);
	pls = _mm_mul_ps(_mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3,0,2,1)), _mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(3,1,0,2)));
	ret = _mm_sub_ps(ret, pls);
	pls = _mm_mul_ps(_mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3,3,3,3)), _mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(3,2,1,0)));
	ret = _mm_sub_ps(ret, pls);

	return *(SIMD4*)&_mm_and_ps(ret, *(const __m128*)&mask2);
#endif

	/*
	tmp[0] = a[3]*vec.a[0] + a[1]*vec.a[2] - a[2]*vec.a[1];
	tmp[1] = a[3]*vec.a[1] + a[2]*vec.a[0] - a[0]*vec.a[2];
	tmp[2] = a[3]*vec.a[2] + a[0]*vec.a[1] - a[1]*vec.a[0];
	tmp[3] =-a[0]*vec.a[0] - a[1]*vec.a[1] - a[2]*vec.a[2];

	ret.a[0] = -tmp[3]*a[0] + tmp[0]*a[3] - tmp[1]*a[2] + tmp[2]*a[1];
	ret.a[1] = -tmp[3]*a[1] + tmp[1]*a[3] - tmp[2]*a[0] + tmp[0]*a[2];
	ret.a[2] = -tmp[3]*a[2] + tmp[2]*a[3] - tmp[0]*a[1] + tmp[1]*a[0];
	*/
}

static INLINE_NO_DEBUG SIMD4 simd_quat_inv_rotate_vec(const SIMD4& q, const SIMD4& v)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128i mask = _mm_set_epi32(0, 0x80000000, 0x80000000, 0x80000000);
	static const __m128i mask2 = _mm_set_epi32(0, -1, -1, -1);
	__m128 tmp = 
		_mm_mul_ps(_mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(0,3,3,3)), _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(0,2,1,0)));
	__m128 pls =
		_mm_mul_ps(_mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(2,1,0,2)), _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(2,0,2,1)));
	tmp = _mm_add_ps(tmp, pls);
	pls = _mm_mul_ps(_mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(1,0,2,1)), _mm_shuffle_ps(v.v, v.v, _MM_SHUFFLE(1,1,0,2)));
	tmp = _mm_add_ps(tmp, _mm_xor_ps(pls, *(const __m128*)&mask));

	__m128 ret = 
		_mm_mul_ps(_mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3,3,3,3)), _mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(3,2,1,0)));
	pls = _mm_mul_ps(_mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3,2,1,0)), _mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(3,3,3,3)));
	ret = _mm_add_ps(ret, pls);
	pls = _mm_mul_ps(_mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3,0,2,1)), _mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(3,1,0,2)));
	ret = _mm_add_ps(ret, pls);
	pls = _mm_mul_ps(_mm_shuffle_ps(tmp, tmp, _MM_SHUFFLE(3,1,0,2)), _mm_shuffle_ps(q.v, q.v, _MM_SHUFFLE(3,0,2,1)));
	ret = _mm_sub_ps(ret, pls);

	return *(SIMD4*)&_mm_and_ps(ret, *(const __m128*)&mask2);
#endif

	/*
	tmp[0] = a[3]*vec.a[0] - a[1]*vec.a[2] + a[2]*vec.a[1];
	tmp[1] = a[3]*vec.a[1] - a[2]*vec.a[0] + a[0]*vec.a[2];
	tmp[2] = a[3]*vec.a[2] - a[0]*vec.a[1] + a[1]*vec.a[0];
	tmp[3] = a[0]*vec.a[0] + a[1]*vec.a[1] + a[2]*vec.a[2];

	ret.a[0] = tmp[3]*a[0] + tmp[0]*a[3] + tmp[1]*a[2] - tmp[2]*a[1];
	ret.a[1] = tmp[3]*a[1] + tmp[1]*a[3] + tmp[2]*a[0] - tmp[0]*a[2];
	ret.a[2] = tmp[3]*a[2] + tmp[2]*a[3] + tmp[0]*a[1] - tmp[1]*a[0];
	*/
}

static INLINE_NO_DEBUG SIMD4 simd_neg(const SIMD4& v)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128i mask = _mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000);
	return *(SIMD4*)& _mm_xor_ps(v.v, *(const __m128*)&mask);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_neg3(const SIMD4& v)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128i mask = _mm_set_epi32(0, 0x80000000, 0x80000000, 0x80000000);
	return *(SIMD4*)& _mm_xor_ps(v.v, *(const __m128*)&mask);
#endif
}

static INLINE_NO_DEBUG SIMD4 simd_neg2(const SIMD4& v)
{
#ifdef INSTRUCTIONS_SSE
	static const __m128i mask = _mm_set_epi32(0, 0, 0x80000000, 0x80000000);
	return *(SIMD4*)& _mm_xor_ps(v.v, *(const __m128*)&mask);
#endif
}


static INLINE_NO_DEBUG Float operator + (const Float& a, const Float& b) { return *(Float*)&simd_add(a, b); }
static INLINE_NO_DEBUG Float operator + (const Float& a, const float& b) { return *(Float*)&simd_add(a, Float(b)); }
static INLINE_NO_DEBUG Float operator + (const Float& a, const FloatX& b){ return *(Float*)&simd_add(a, Float(b)); }
static INLINE_NO_DEBUG Float operator + (const Float& a, const FloatY& b){ return *(Float*)&simd_add(a, Float(b)); }
static INLINE_NO_DEBUG Float operator + (const Float& a, const FloatZ& b){ return *(Float*)&simd_add(a, Float(b)); }
static INLINE_NO_DEBUG Float operator + (const Float& a, const FloatW& b){ return *(Float*)&simd_add(a, Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const Float& b) { return *(Float*)&simd_add(Float(a), b); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const float& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const FloatX& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const FloatY& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const FloatZ& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const FloatW& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const Float& b) { return *(Float*)&simd_add(Float(a), b); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const float& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const FloatX& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const FloatY& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const FloatZ& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const FloatW& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const Float& b) { return *(Float*)&simd_add(Float(a), b); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const float& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const FloatX& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const FloatY& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const FloatZ& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const FloatW& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const Float& b) { return *(Float*)&simd_add(Float(a), b); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const float& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const FloatX& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const FloatY& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const FloatZ& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const FloatW& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const float& a, const Float& b) { return *(Float*)&simd_add(Float(a), b); }
static INLINE_NO_DEBUG Float operator + (const float& a, const FloatX& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const float& a, const FloatY& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const float& a, const FloatZ& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const float& a, const FloatW& b) { return *(Float*)&simd_add(Float(a), Float(b)); }

#ifdef INSTRUCTIONS_NEON
static INLINE_NO_DEBUG FloatShort operator + (const FloatShort& a, const FloatShort& b) { return *(FloatShort*)&simd_add(a, b); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShort& a, const FloatShortX& b){ return *(FloatShort*)&simd_add(a, FloatShort(b)); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShort& a, const FloatShortY& b){ return *(FloatShort*)&simd_add(a, FloatShort(b)); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShort& a, const float& b){ return *(FloatShort*)&simd_add(a, FloatShort(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShort& a, const Float& b) { return *(Float*)&simd_add(Float(a), b); }
static INLINE_NO_DEBUG Float operator + (const FloatShort& a, const FloatX& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShort& a, const FloatY& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShort& a, const FloatZ& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShort& a, const FloatW& b){ return *(Float*)&simd_add(Float(a), Float(b)); }

static INLINE_NO_DEBUG FloatShort operator + (const FloatShortX& a, const FloatShort& b) { return *(FloatShort*)&simd_add(FloatShort(a), b); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShortX& a, const FloatShortX& b){ return *(FloatShort*)&simd_add(FloatShort(a), FloatShort(b)); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShortX& a, const FloatShortY& b){ return *(FloatShort*)&simd_add(FloatShort(a), FloatShort(b)); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShortX& a, const float& b){ return *(FloatShort*)&simd_add(FloatShort(a), FloatShort(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShortX& a, const Float& b) { return *(Float*)&simd_add(Float(a), b); }
static INLINE_NO_DEBUG Float operator + (const FloatShortX& a, const FloatX& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShortX& a, const FloatY& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShortX& a, const FloatZ& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShortX& a, const FloatW& b){ return *(Float*)&simd_add(Float(a), Float(b)); }

static INLINE_NO_DEBUG FloatShort operator + (const FloatShortY& a, const FloatShort& b) { return *(FloatShort*)&simd_add(FloatShort(a), b); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShortY& a, const FloatShortX& b){ return *(FloatShort*)&simd_add(FloatShort(a), FloatShort(b)); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShortY& a, const FloatShortY& b){ return *(FloatShort*)&simd_add(FloatShort(a), FloatShort(b)); }
static INLINE_NO_DEBUG FloatShort operator + (const FloatShortY& a, const float& b){ return *(FloatShort*)&simd_add(FloatShort(a), FloatShort(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShortY& a, const Float& b) { return *(Float*)&simd_add(Float(a), b); }
static INLINE_NO_DEBUG Float operator + (const FloatShortY& a, const FloatX& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShortY& a, const FloatY& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShortY& a, const FloatZ& b){ return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatShortY& a, const FloatW& b){ return *(Float*)&simd_add(Float(a), Float(b)); }

static INLINE_NO_DEBUG FloatShort operator + (const float& a, const FloatShort& b) { return *(FloatShort*)&simd_add(FloatShort(a), b); }
static INLINE_NO_DEBUG Float operator + (const Float& a, const FloatShort& b) { return *(Float*)&simd_add(a, Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const FloatShort& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const FloatShort& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const FloatShort& b) { return *(Float*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const FloatShort& b) { return *(Float*)&simd_add(Float(a), Float(b)); }

static INLINE_NO_DEBUG FloatShort operator + (const float& a, const FloatShortX& b) { return *(FloatShort*)&simd_add(FloatShort(a), FloatShort(b)); }
static INLINE_NO_DEBUG Float operator + (const Float& a, const FloatShortX& b) { return *(FloatShort*)&simd_add(a, Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const FloatShortX& b) { return *(FloatShort*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const FloatShortX& b) { return *(FloatShort*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const FloatShortX& b) { return *(FloatShort*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const FloatShortX& b) { return *(FloatShort*)&simd_add(Float(a), Float(b)); }

static INLINE_NO_DEBUG FloatShort operator + (const float& a, const FloatShortY& b) { return *(FloatShort*)&simd_add(FloatShort(a), FloatShort(b)); }
static INLINE_NO_DEBUG Float operator + (const Float& a, const FloatShortY& b) { return *(FloatShort*)&simd_add(a, Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatX& a, const FloatShortY& b) { return *(FloatShort*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatY& a, const FloatShortY& b) { return *(FloatShort*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatZ& a, const FloatShortY& b) { return *(FloatShort*)&simd_add(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator + (const FloatW& a, const FloatShortY& b) { return *(FloatShort*)&simd_add(Float(a), Float(b)); }
#endif

static INLINE_NO_DEBUG Float operator - (const Float& a, const Float& b) { return *(Float*)&simd_sub(a, b); }
static INLINE_NO_DEBUG Float operator - (const Float& a, const float& b) { return *(Float*)&simd_sub(a, Float(b)); }
static INLINE_NO_DEBUG Float operator - (const Float& a, const FloatX& b){ return *(Float*)&simd_sub(a, Float(b)); }
static INLINE_NO_DEBUG Float operator - (const Float& a, const FloatY& b){ return *(Float*)&simd_sub(a, Float(b)); }
static INLINE_NO_DEBUG Float operator - (const Float& a, const FloatZ& b){ return *(Float*)&simd_sub(a, Float(b)); }
static INLINE_NO_DEBUG Float operator - (const Float& a, const FloatW& b){ return *(Float*)&simd_sub(a, Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatX& a, const Float& b) { return *(Float*)&simd_sub(Float(a), b); }
static INLINE_NO_DEBUG Float operator - (const FloatX& a, const float& b) { return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatX& a, const FloatX& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatX& a, const FloatY& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatX& a, const FloatZ& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatX& a, const FloatW& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatY& a, const Float& b) { return *(Float*)&simd_sub(Float(a), b); }
static INLINE_NO_DEBUG Float operator - (const FloatY& a, const float& b) { return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatY& a, const FloatX& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatY& a, const FloatY& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatY& a, const FloatZ& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatY& a, const FloatW& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatZ& a, const Float& b) { return *(Float*)&simd_sub(Float(a), b); }
static INLINE_NO_DEBUG Float operator - (const FloatZ& a, const float& b) { return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatZ& a, const FloatX& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatZ& a, const FloatY& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatZ& a, const FloatZ& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatZ& a, const FloatW& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatW& a, const Float& b) { return *(Float*)&simd_sub(Float(a), b); }
static INLINE_NO_DEBUG Float operator - (const FloatW& a, const float& b) { return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatW& a, const FloatX& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatW& a, const FloatY& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatW& a, const FloatZ& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const FloatW& a, const FloatW& b){ return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const float& a, const Float& b) { return *(Float*)&simd_sub(Float(a), b); }
static INLINE_NO_DEBUG Float operator - (const float& a, const FloatX& b) { return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const float& a, const FloatY& b) { return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const float& a, const FloatZ& b) { return *(Float*)&simd_sub(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator - (const float& a, const FloatW& b) { return *(Float*)&simd_sub(Float(a), Float(b)); }


static INLINE_NO_DEBUG Float operator * (const Float& a, const Float& b) { return *(Float*)&simd_mul(a, b); }
static INLINE_NO_DEBUG Float operator * (const Float& a, const float& b) { return *(Float*)&simd_mul(a, Float(b)); }
static INLINE_NO_DEBUG Float operator * (const Float& a, const FloatX& b){ return *(Float*)&simd_mul(a, Float(b)); }
static INLINE_NO_DEBUG Float operator * (const Float& a, const FloatY& b){ return *(Float*)&simd_mul(a, Float(b)); }
static INLINE_NO_DEBUG Float operator * (const Float& a, const FloatZ& b){ return *(Float*)&simd_mul(a, Float(b)); }
static INLINE_NO_DEBUG Float operator * (const Float& a, const FloatW& b){ return *(Float*)&simd_mul(a, Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatX& a, const Float& b) { return *(Float*)&simd_mul(Float(a), b); }
static INLINE_NO_DEBUG Float operator * (const FloatX& a, const float& b) { return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatX& a, const FloatX& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatX& a, const FloatY& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatX& a, const FloatZ& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatX& a, const FloatW& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatY& a, const Float& b) { return *(Float*)&simd_mul(Float(a), b); }
static INLINE_NO_DEBUG Float operator * (const FloatY& a, const float& b) { return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatY& a, const FloatX& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatY& a, const FloatY& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatY& a, const FloatZ& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatY& a, const FloatW& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatZ& a, const Float& b) { return *(Float*)&simd_mul(Float(a), b); }
static INLINE_NO_DEBUG Float operator * (const FloatZ& a, const float& b) { return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatZ& a, const FloatX& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatZ& a, const FloatY& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatZ& a, const FloatZ& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatZ& a, const FloatW& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatW& a, const Float& b) { return *(Float*)&simd_mul(Float(a), b); }
static INLINE_NO_DEBUG Float operator * (const FloatW& a, const float& b) { return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatW& a, const FloatX& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatW& a, const FloatY& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatW& a, const FloatZ& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const FloatW& a, const FloatW& b){ return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const float& a, const Float& b) { return *(Float*)&simd_mul(Float(a), b); }
static INLINE_NO_DEBUG Float operator * (const float& a, const FloatX& b) { return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const float& a, const FloatY& b) { return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const float& a, const FloatZ& b) { return *(Float*)&simd_mul(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator * (const float& a, const FloatW& b) { return *(Float*)&simd_mul(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float operator / (const Float& a, const Float& b) { return *(Float*)&simd_div(a, b); }
static INLINE_NO_DEBUG Float operator / (const Float& a, const float& b) { return *(Float*)&simd_div(a, Float(b)); }
static INLINE_NO_DEBUG Float operator / (const Float& a, const FloatX& b){ return *(Float*)&simd_div(a, Float(b)); }
static INLINE_NO_DEBUG Float operator / (const Float& a, const FloatY& b){ return *(Float*)&simd_div(a, Float(b)); }
static INLINE_NO_DEBUG Float operator / (const Float& a, const FloatZ& b){ return *(Float*)&simd_div(a, Float(b)); }
static INLINE_NO_DEBUG Float operator / (const Float& a, const FloatW& b){ return *(Float*)&simd_div(a, Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatX& a, const Float& b) { return *(Float*)&simd_div(Float(a), b); }
static INLINE_NO_DEBUG Float operator / (const FloatX& a, const float& b) { return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatX& a, const FloatX& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatX& a, const FloatY& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatX& a, const FloatZ& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatX& a, const FloatW& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatY& a, const Float& b) { return *(Float*)&simd_div(Float(a), b); }
static INLINE_NO_DEBUG Float operator / (const FloatY& a, const float& b) { return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatY& a, const FloatX& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatY& a, const FloatY& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatY& a, const FloatZ& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatY& a, const FloatW& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatZ& a, const Float& b) { return *(Float*)&simd_div(Float(a), b); }
static INLINE_NO_DEBUG Float operator / (const FloatZ& a, const float& b) { return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatZ& a, const FloatX& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatZ& a, const FloatY& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatZ& a, const FloatZ& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatZ& a, const FloatW& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatW& a, const Float& b) { return *(Float*)&simd_div(Float(a), b); }
static INLINE_NO_DEBUG Float operator / (const FloatW& a, const float& b) { return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatW& a, const FloatX& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatW& a, const FloatY& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatW& a, const FloatZ& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const FloatW& a, const FloatW& b){ return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const float& a, const Float& b) { return *(Float*)&simd_div(Float(a), b); }
static INLINE_NO_DEBUG Float operator / (const float& a, const FloatX& b) { return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const float& a, const FloatY& b) { return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const float& a, const FloatZ& b) { return *(Float*)&simd_div(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float operator / (const float& a, const FloatW& b) { return *(Float*)&simd_div(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float min(const Float& a, const Float& b){ return *(Float*)&simd_min(a, b); }
static INLINE_NO_DEBUG Float min(const Float& a, const float& b){ return *(Float*)&simd_min(a, Float(b)); }
static INLINE_NO_DEBUG Float min(const Float& a, const FloatX& b){ return *(Float*)&simd_min(a, Float(b)); }
static INLINE_NO_DEBUG Float min(const Float& a, const FloatY& b){ return *(Float*)&simd_min(a, Float(b)); }
static INLINE_NO_DEBUG Float min(const Float& a, const FloatZ& b){ return *(Float*)&simd_min(a, Float(b)); }
static INLINE_NO_DEBUG Float min(const Float& a, const FloatW& b){ return *(Float*)&simd_min(a, Float(b)); }

static INLINE_NO_DEBUG Float min(const float& a, const Float& b){ return *(Float*)&simd_min(Float(a), b); }
//static INLINE_NO_DEBUG Float min(const float& a, const float& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const float& a, const FloatX& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const float& a, const FloatY& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const float& a, const FloatZ& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const float& a, const FloatW& b){ return *(Float*)&simd_min(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float min(const FloatX& a, const Float& b){ return *(Float*)&simd_min(Float(a), b); }
static INLINE_NO_DEBUG Float min(const FloatX& a, const float& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatX& a, const FloatX& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatX& a, const FloatY& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatX& a, const FloatZ& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatX& a, const FloatW& b){ return *(Float*)&simd_min(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float min(const FloatY& a, const Float& b){ return *(Float*)&simd_min(Float(a), b); }
static INLINE_NO_DEBUG Float min(const FloatY& a, const float& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatY& a, const FloatX& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatY& a, const FloatY& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatY& a, const FloatZ& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatY& a, const FloatW& b){ return *(Float*)&simd_min(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float min(const FloatZ& a, const Float& b){ return *(Float*)&simd_min(Float(a), b); }
static INLINE_NO_DEBUG Float min(const FloatZ& a, const float& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatZ& a, const FloatX& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatZ& a, const FloatY& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatZ& a, const FloatZ& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatZ& a, const FloatW& b){ return *(Float*)&simd_min(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float min(const FloatW& a, const Float& b){ return *(Float*)&simd_min(Float(a), b); }
static INLINE_NO_DEBUG Float min(const FloatW& a, const float& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatW& a, const FloatX& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatW& a, const FloatY& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatW& a, const FloatZ& b){ return *(Float*)&simd_min(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float min(const FloatW& a, const FloatW& b){ return *(Float*)&simd_min(Float(a), Float(b)); }




static INLINE_NO_DEBUG Float max(const Float& a, const Float& b){ return *(Float*)&simd_max(a, b); }
static INLINE_NO_DEBUG Float max(const Float& a, const float& b){ return *(Float*)&simd_max(a, Float(b)); }
static INLINE_NO_DEBUG Float max(const Float& a, const FloatX& b){ return *(Float*)&simd_max(a, Float(b)); }
static INLINE_NO_DEBUG Float max(const Float& a, const FloatY& b){ return *(Float*)&simd_max(a, Float(b)); }
static INLINE_NO_DEBUG Float max(const Float& a, const FloatZ& b){ return *(Float*)&simd_max(a, Float(b)); }
static INLINE_NO_DEBUG Float max(const Float& a, const FloatW& b){ return *(Float*)&simd_max(a, Float(b)); }

static INLINE_NO_DEBUG Float max(const float& a, const Float& b){ return *(Float*)&simd_max(Float(a), b); }
//static INLINE_NO_DEBUG Float max(const float& a, const float& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const float& a, const FloatX& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const float& a, const FloatY& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const float& a, const FloatZ& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const float& a, const FloatW& b){ return *(Float*)&simd_max(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float max(const FloatX& a, const Float& b){ return *(Float*)&simd_max(Float(a), b); }
static INLINE_NO_DEBUG Float max(const FloatX& a, const float& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatX& a, const FloatX& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatX& a, const FloatY& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatX& a, const FloatZ& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatX& a, const FloatW& b){ return *(Float*)&simd_max(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float max(const FloatY& a, const Float& b){ return *(Float*)&simd_max(Float(a), b); }
static INLINE_NO_DEBUG Float max(const FloatY& a, const float& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatY& a, const FloatX& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatY& a, const FloatY& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatY& a, const FloatZ& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatY& a, const FloatW& b){ return *(Float*)&simd_max(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float max(const FloatZ& a, const Float& b){ return *(Float*)&simd_max(Float(a), b); }
static INLINE_NO_DEBUG Float max(const FloatZ& a, const float& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatZ& a, const FloatX& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatZ& a, const FloatY& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatZ& a, const FloatZ& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatZ& a, const FloatW& b){ return *(Float*)&simd_max(Float(a), Float(b)); }

static INLINE_NO_DEBUG Float max(const FloatW& a, const Float& b){ return *(Float*)&simd_max(Float(a), b); }
static INLINE_NO_DEBUG Float max(const FloatW& a, const float& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatW& a, const FloatX& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatW& a, const FloatY& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatW& a, const FloatZ& b){ return *(Float*)&simd_max(Float(a), Float(b)); }
static INLINE_NO_DEBUG Float max(const FloatW& a, const FloatW& b){ return *(Float*)&simd_max(Float(a), Float(b)); }


static INLINE_NO_DEBUG Float abs(const Float& x) { return *(Float*)&simd_abs(x); }
static INLINE_NO_DEBUG Float abs(const FloatX& x) { return *(Float*)&simd_abs(Float(x)); }
static INLINE_NO_DEBUG Float abs(const FloatY& x) { return *(Float*)&simd_abs(Float(x)); }
static INLINE_NO_DEBUG Float abs(const FloatZ& x) { return *(Float*)&simd_abs(Float(x)); }
static INLINE_NO_DEBUG Float abs(const FloatW& x) { return *(Float*)&simd_abs(Float(x)); }

static INLINE_NO_DEBUG Float sign(const Float& x) { return *(Float*)&simd_sign(x); }
static INLINE_NO_DEBUG Float sign(const FloatX& x) { return *(Float*)&simd_sign(Float(x)); }
static INLINE_NO_DEBUG Float sign(const FloatY& x) { return *(Float*)&simd_sign(Float(x)); }
static INLINE_NO_DEBUG Float sign(const FloatZ& x) { return *(Float*)&simd_sign(Float(x)); }
static INLINE_NO_DEBUG Float sign(const FloatW& x) { return *(Float*)&simd_sign(Float(x)); }

template<typename t_Min, typename t_Max>
static INLINE_NO_DEBUG Float clamp(const Float& x, const t_Min& min, const t_Max& max) { return *(Float*)&simd_clamp(x, Float(min), Float(max)); }
template<typename t_Min, typename t_Max>
static INLINE_NO_DEBUG Float clamp(const FloatX& x, const t_Min& min, const t_Max& max) { return *(Float*)&simd_clamp(Float(x), Float(min), Float(max)); }
template<typename t_Min, typename t_Max>
static INLINE_NO_DEBUG Float clamp(const FloatY& x, const t_Min& min, const t_Max& max) { return *(Float*)&simd_clamp(Float(x), Float(min), Float(max)); }
template<typename t_Min, typename t_Max>
static INLINE_NO_DEBUG Float clamp(const FloatZ& x, const t_Min& min, const t_Max& max) { return *(Float*)&simd_clamp(Float(x), Float(min), Float(max)); }
template<typename t_Min, typename t_Max>
static INLINE_NO_DEBUG Float clamp(const FloatW& x, const t_Min& min, const t_Max& max) { return *(Float*)&simd_clamp(Float(x), Float(min), Float(max)); }

static INLINE_NO_DEBUG Float clamp01(const Float& x) { return *(Float*)&simd_clamp01(x); }
static INLINE_NO_DEBUG Float clamp01(const FloatX& x) { return *(Float*)&simd_clamp01(Float(x)); }
static INLINE_NO_DEBUG Float clamp01(const FloatY& x) { return *(Float*)&simd_clamp01(Float(x)); }
static INLINE_NO_DEBUG Float clamp01(const FloatZ& x) { return *(Float*)&simd_clamp01(Float(x)); }
static INLINE_NO_DEBUG Float clamp01(const FloatW& x) { return *(Float*)&simd_clamp01(Float(x)); }

static INLINE_NO_DEBUG Float clamp11(const Float& x) { return *(Float*)&simd_clamp11(x); }
static INLINE_NO_DEBUG Float clamp11(const FloatX& x) { return *(Float*)&simd_clamp11(Float(x)); }
static INLINE_NO_DEBUG Float clamp11(const FloatY& x) { return *(Float*)&simd_clamp11(Float(x)); }
static INLINE_NO_DEBUG Float clamp11(const FloatZ& x) { return *(Float*)&simd_clamp11(Float(x)); }
static INLINE_NO_DEBUG Float clamp11(const FloatW& x) { return *(Float*)&simd_clamp11(Float(x)); }

//static INLINE_NO_DEBUG boolean equals(const Float& a, const Float& b, float eps){ return equals((float)a, (float)b, eps); }

//approximation
static INLINE_NO_DEBUG Float rcp(const Float& f)
{
#ifdef INSTRUCTIONS_SSE
	__m128 res = _mm_rcp_ss(f.v);
	return *(Float*)&_mm_shuffle_ps(res, res, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG Float rcp(const FloatX& f) { return rcp(Float(f)); }
static INLINE_NO_DEBUG Float rcp(const FloatY& f) { return rcp(Float(f)); }
static INLINE_NO_DEBUG Float rcp(const FloatZ& f) { return rcp(Float(f)); }
static INLINE_NO_DEBUG Float rcp(const FloatW& f) { return rcp(Float(f)); }

//approximation
static INLINE_NO_DEBUG Float rsq(const Float& f)
{
#ifdef INSTRUCTIONS_SSE
	__m128 res = _mm_rsqrt_ss(f.v);
	return *(Float*)&_mm_shuffle_ps(res, res, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG Float rsq(const FloatX& f) { return rsq(Float(f)); }
static INLINE_NO_DEBUG Float rsq(const FloatY& f) { return rsq(Float(f)); }
static INLINE_NO_DEBUG Float rsq(const FloatZ& f) { return rsq(Float(f)); }
static INLINE_NO_DEBUG Float rsq(const FloatW& f) { return rsq(Float(f)); }

static INLINE_NO_DEBUG Float sqrt(const Float& f)
{
#ifdef INSTRUCTIONS_SSE
	__m128 res = _mm_sqrt_ss(f.v);
	return *(Float*)&_mm_shuffle_ps(res, res, _MM_SHUFFLE(0,0,0,0));
#endif
}

static INLINE_NO_DEBUG Float sqrt(const FloatX& f) { return sqrt(Float(f)); }
static INLINE_NO_DEBUG Float sqrt(const FloatY& f) { return sqrt(Float(f)); }
static INLINE_NO_DEBUG Float sqrt(const FloatZ& f) { return sqrt(Float(f)); }
static INLINE_NO_DEBUG Float sqrt(const FloatW& f) { return sqrt(Float(f)); }

static INLINE_NO_DEBUG Float rsqFast(const Float& f)
{
#ifdef INSTRUCTIONS_SSE
	return rsq(f);
#endif
}

static INLINE_NO_DEBUG Float rsqFast(const FloatX& f) { return rsqFast(Float(f)); }
static INLINE_NO_DEBUG Float rsqFast(const FloatY& f) { return rsqFast(Float(f)); }
static INLINE_NO_DEBUG Float rsqFast(const FloatZ& f) { return rsqFast(Float(f)); }
static INLINE_NO_DEBUG Float rsqFast(const FloatW& f) { return rsqFast(Float(f)); }

template<class t_Scalar>
struct TFloatStartConvertor
{
	static const uint gIndex = 0;
	static INLINE_NO_DEBUG const Float cnv(const t_Scalar& f) { return Float(f); }
};

template<>
struct TFloatStartConvertor<Float>
{
	static const uint gIndex = 0;
	static INLINE_NO_DEBUG const Float& cnv(const Float& f) { return f; }
};

template<>
struct TFloatStartConvertor<FloatX>
{
	static const uint gIndex = 0;
	static INLINE_NO_DEBUG const FloatX& cnv(const FloatX& f) { return f; }
};

template<>
struct TFloatStartConvertor<FloatY>
{
	static const uint gIndex = 1;
	static INLINE_NO_DEBUG const FloatY& cnv(const FloatY& f) { return f; }
};


template<>
struct TFloatStartConvertor<FloatZ>
{
	static const uint gIndex = 2;
	static INLINE_NO_DEBUG const FloatZ& cnv(const FloatZ& f) { return f; }
};

template<>
struct TFloatStartConvertor<FloatW>
{
	static const uint gIndex = 3;
	static INLINE_NO_DEBUG const FloatW& cnv(const FloatW& f) { return f; }
};


struct FloatVector : public SIMD4 // private type, no user usage.
{
public:
	INLINE_NO_DEBUG FloatVector(){}

	INLINE_NO_DEBUG FloatVector(const float x, const float y, const float z, const float w)
	{
		a[0] = x;
		a[1] = y;
		a[2] = z;
		a[3] = w;
	}

	template<class t_Float1, class t_Float2, class t_Float3, class t_Float4>
	INLINE_NO_DEBUG FloatVector(const t_Float1& X, const t_Float2& Y, const t_Float3& Z, const t_Float4& W)
	{
		set(X, Y, Z, W);
	}

	INLINE_NO_DEBUG FloatVector(float F)
	{
		set(F);
	}

	template<class t_Float1>
	INLINE_NO_DEBUG FloatVector(const t_Float1& F)
	{
		set(F);
	}

	template<class t_Float0, class t_Float1, class t_Float2, class t_Float3>
	INLINE_NO_DEBUG void set(const t_Float0& X, const t_Float1& Y, const t_Float2& Z, const t_Float3& W)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(_mm_shuffle_ps(TFloatStartConvertor<t_Float0>::cnv(X).simd().v, TFloatStartConvertor<t_Float1>::cnv(Y).simd().v, _MM_SHUFFLE(TFloatStartConvertor<t_Float1>::gIndex, TFloatStartConvertor<t_Float1>::gIndex, TFloatStartConvertor<t_Float0>::gIndex, TFloatStartConvertor<t_Float0>::gIndex)),
				_mm_shuffle_ps(TFloatStartConvertor<t_Float2>::cnv(Z).simd().v, TFloatStartConvertor<t_Float3>::cnv(W).simd().v, _MM_SHUFFLE(TFloatStartConvertor<t_Float3>::gIndex, TFloatStartConvertor<t_Float3>::gIndex, TFloatStartConvertor<t_Float2>::gIndex, TFloatStartConvertor<t_Float2>::gIndex)), _MM_SHUFFLE(2,0,2,0));
#endif
	}

	INLINE_NO_DEBUG void set(float X, float Y, float Z, float W)
	{
		a[0] = X;
		a[1] = Y;
		a[2] = Z;
		a[3] = W;
	}

	template<class t_Float>
	INLINE_NO_DEBUG void set(const t_Float& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(TFloatStartConvertor<t_Float>::cnv(f).simd().v, TFloatStartConvertor<t_Float>::cnv(f).simd().v, _MM_SHUFFLE(TFloatStartConvertor<t_Float>::gIndex, TFloatStartConvertor<t_Float>::gIndex, TFloatStartConvertor<t_Float>::gIndex, TFloatStartConvertor<t_Float>::gIndex));
#endif
	}

	INLINE_NO_DEBUG void set(float f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_set1_ps(f);
#endif
	}

	INLINE_NO_DEBUG void set(const float* arr)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_load_ps(arr);
#endif
	}

	template<class t_Float0, class t_Float1, class t_Float2>
	INLINE_NO_DEBUG void setXYZ0(const t_Float0& X, const t_Float1& Y, const t_Float2& Z)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(_mm_shuffle_ps(TFloatStartConvertor<t_Float0>::cnv(X).simd().v, TFloatStartConvertor<t_Float1>::cnv(Y).simd().v, _MM_SHUFFLE(TFloatStartConvertor<t_Float1>::gIndex, TFloatStartConvertor<t_Float1>::gIndex, TFloatStartConvertor<t_Float0>::gIndex, TFloatStartConvertor<t_Float0>::gIndex)),
			_mm_shuffle_ps(TFloatStartConvertor<t_Float2>::cnv(Z).simd().v, _mm_setzero_ps(), _MM_SHUFFLE(0, 0, TFloatStartConvertor<t_Float2>::gIndex, TFloatStartConvertor<t_Float2>::gIndex)), _MM_SHUFFLE(2,0,2,0));
#endif
	}

	INLINE_NO_DEBUG void setXYZ0(float X, float Y, float Z)
	{
		a[0] = X;
		a[1] = Y;
		a[2] = Z;
		a[3] = 0.0f;
	}

	INLINE_NO_DEBUG void setXYZ0(const FloatVector& other)
	{
		v = other.v;
		w = 0.0f;
	}

	INLINE_NO_DEBUG void setXYZ1(const FloatVector& other)
	{
		v = other.v;
		w = 1.0f;
	}

	INLINE_NO_DEBUG void setXYZ0(const float* arr)
	{
		a[0] = arr[0];
		a[1] = arr[1];
		a[2] = arr[2];
		a[3] = 0.0f;
	}

	template<class t_Float>
	INLINE_NO_DEBUG void setXYZ0(const t_Float& f)
	{
		v = _mm_shuffle_ps(TFloatStartConvertor<t_Float>::cnv(f).simd().v, _mm_shuffle_ps(TFloatStartConvertor<t_Float>::cnv(f).simd().v, _mm_setzero_ps(), _MM_SHUFFLE(0, 0, TFloatStartConvertor<t_Float>::gIndex, TFloatStartConvertor<t_Float>::gIndex)), _MM_SHUFFLE(2,0,f.gIndex,f.gIndex));
	}

	INLINE_NO_DEBUG void setXYZ0(float f)
	{
		a[0] = f;
		a[1] = f;
		a[2] = f;
		a[3] = 0.0f;
	}

	template<class t_Float2>
	INLINE_NO_DEBUG void setXY_Z0(const SIMD4& XY, const t_Float2& Z)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(XY.v, _mm_shuffle_ps(TFloatStartConvertor<t_Float2>::cnv(Z).simd().v, _mm_setzero_ps(), _MM_SHUFFLE(0,0,TFloatStartConvertor<t_Float2>::gIndex,TFloatStartConvertor<t_Float2>::gIndex)), _MM_SHUFFLE(2,0,1,0));
#endif
	}

	template<class t_Float3>
	INLINE_NO_DEBUG void setXYZ_W(const SIMD4& XYZ, const t_Float3& W)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(XYZ.v, _mm_shuffle_ps(XYZ.v, TFloatStartConvertor<t_Float3>::cnv(W).simd().v, _MM_SHUFFLE(TFloatStartConvertor<t_Float3>::gIndex, TFloatStartConvertor<t_Float3>::gIndex, 2, 2)), _MM_SHUFFLE(2,0,1,0));
#endif
	}

	INLINE_NO_DEBUG void setXY_ZW(const SIMD4& XY, const SIMD4& ZW)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(XY.v, ZW.v, _MM_SHUFFLE(3,2,1,0));
#endif
	}


	template<class t_Float0, class t_Float1>
	INLINE_NO_DEBUG void setXY00(const t_Float0& X, const t_Float1& Y)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(_mm_shuffle_ps(TFloatStartConvertor<t_Float0>::cnv(X).simd().v, TFloatStartConvertor<t_Float1>::cnv(Y).simd().v, _MM_SHUFFLE(TFloatStartConvertor<t_Float1>::gIndex, TFloatStartConvertor<t_Float1>::gIndex, TFloatStartConvertor<t_Float0>::gIndex, TFloatStartConvertor<t_Float0>::gIndex)), _mm_setzero_ps(), _MM_SHUFFLE(0,0,2,0));
#endif
	}

	INLINE_NO_DEBUG void setXY00(float X, float Y)
	{
		a[0] = X;
		a[1] = Y;
		a[2] = 0.0f;
		a[3] = 0.0f;
	}

	INLINE_NO_DEBUG void setXY00(const FloatVector& other)
	{
		v = _mm_shuffle_ps(other.v, _mm_setzero_ps(), _MM_SHUFFLE(3,3,1,0));
	}

	INLINE_NO_DEBUG void setXY00_FromZW(const FloatVector& other)
	{
		v = _mm_shuffle_ps(other.v, _mm_setzero_ps(), _MM_SHUFFLE(3,3,3,2));
	}

	INLINE_NO_DEBUG void setXY00(const float* arr)
	{
		a[0] = arr[0];
		a[1] = arr[1];
		a[2] = 0.0f;
		a[3] = 0.0f;
	}

	template<class t_Float>
	INLINE_NO_DEBUG void setXY00(const t_Float& f)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(TFloatStartConvertor<t_Float>::cnv(f).simd().v, _mm_setzero_ps(), _MM_SHUFFLE(0,0,TFloatStartConvertor<t_Float>::gIndex, TFloatStartConvertor<t_Float>::gIndex));
#endif
	}

	INLINE_NO_DEBUG void setXY00(float f)
	{
		a[0] = f;
		a[1] = f;
		a[2] = 0.0f;
		a[3] = 0.0f;
	}


	INLINE_NO_DEBUG FloatVector getXYZ1()const
	{
#ifdef INSTRUCTIONS_SSE
		static const __m128 one = _mm_set1_ps(1.0f);
		return *(FloatVector*)&_mm_shuffle_ps(v, _mm_shuffle_ps(v, one, _MM_SHUFFLE(0,0,2,2)), _MM_SHUFFLE(2,0,1,0));
#endif
	}

	INLINE_NO_DEBUG FloatVector getXY11()const
	{
#ifdef INSTRUCTIONS_SSE
		static const __m128 one = _mm_set1_ps(1.0f);
		return *(FloatVector*)&_mm_shuffle_ps(v, one, _MM_SHUFFLE(0,0,1,0));
#endif
	}

	INLINE_NO_DEBUG void setXY(const SIMD4& other)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(other.v, v, _MM_SHUFFLE(3,2,1,0));
#endif
	}

	INLINE_NO_DEBUG void setZW(const SIMD4& other)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(v, other.v, _MM_SHUFFLE(3,2,1,0));
#endif
	}

	INLINE_NO_DEBUG void setXYZ(const SIMD4& other)
	{
#ifdef INSTRUCTIONS_SSE
		v = _mm_shuffle_ps(other.v, _mm_unpackhi_ps(other.v, v), _MM_SHUFFLE(1,0,1,0));
#endif
	}
};

template<>
struct RealScalarT<Float>
{
	typedef float Type;
};

template<>
struct RealScalarT<FloatX>
{
	typedef float Type;
};

template<>
struct RealScalarT<FloatY>
{
	typedef float Type;
};

template<>
struct RealScalarT<FloatZ>
{
	typedef float Type;
};

template<>
struct RealScalarT<FloatW>
{
	typedef float Type;
};

#ifdef INSTRUCTIONS_NEON

template<>
struct RealScalarT<FloatShort>
{
	typedef float Type;
};


template<>
struct RealScalarT<FloatShortX>
{
	typedef float Type;
};


template<>
struct RealScalarT<FloatShortY>
{
	typedef float Type;
};

#define Float1(x) FloatShort x;
#define Float2(x,y) union { FloatShortVector x##y; struct { FloatX x; FloatY y; }; };
#define Float3(x,y,z) union { FloatShortVector x##y##z##_xy, x##y##z##_z; struct { FloatShortX x; FloatShortY y; FloatShortX z; }; };
#define Float4(x,y,z,w) union { FloatShortVector x##y, z##w; struct { FloatShortX x; FloatShortY y; FloatShortX z; FloatShortY w; }; };
#else
#define Float1(x) Float x;
#define Float2(x,y) union { FloatVector x##y; struct { FloatX x; FloatY y; }; };
#define Float3(x,y,z) union { FloatVector x##y##z; struct { FloatX x; FloatY y; FloatZ z; }; };
#define Float4(x,y,z,w) union { FloatVector x##y##z##w; struct { FloatX x; FloatY y; FloatZ z; FloatW w; }; };
#endif

}

#else

namespace Base
{
typedef float FloatX;
typedef float FloatY;
typedef float FloatZ;
typedef float FloatW;
typedef float Float;
}

#define Float1(x) float x;
#define Float2(x,y) float x; float y;
#define Float3(x,y,z) float x; float y; float z;
#define Float4(x,y,z,w) float x; float y; float z; float w;

#endif