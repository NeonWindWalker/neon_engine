#pragma once

#include "seed.h"

namespace Base
{
	template<typename t_UIntType> 
	static INLINE_NO_DEBUG t_UIntType bitReverse(t_UIntType s)
	{
		t_UIntType d = 0;
		for(uint i=0; i < sizeof(t_UIntType)*8; ++i){
			d <<= 1;
			d |= s & 1;
			s >>= 1;
		}
		return d;
	}

	static INLINE_NO_DEBUG void uint32Set(uint32& dest, uint32 b0, uint32 b1, uint32 b2, uint32 b3)
	{
		dest = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
	}

	static INLINE_NO_DEBUG int8 bitReverse(int8 s)
	{
		return (int8)bitReverse((uint8)s);
	}

	static INLINE_NO_DEBUG int16 bitReverse(int16 s)
	{
		return (int16)bitReverse((uint16)s);
	}

	static INLINE_NO_DEBUG int32 bitReverse(int32 s)
	{
		return (int32)bitReverse((uint32)s);
	}

	static INLINE_NO_DEBUG int64 bitReverse(int64 s)
	{
		return (int64)bitReverse((uint64)s);
	}

	static INLINE_NO_DEBUG uint16 byteReverse(uint16 s)
	{
		return (s>>8) | (s<<8);
	}

	static INLINE_NO_DEBUG uint32 byteReverse(uint32 s)
	{
		return ((s>>24) & 255) | ((s>>8) & (255<<8)) | ((s<<8) & (255<<16)) | ((s<<24) & (255<<24));
	}

	static INLINE_NO_DEBUG uint64 byteReverse(uint64 s)
	{
		uint32* s32 = reinterpret_cast<uint32*>(&s);
		uint32 res[2];
		res[0] = byteReverse(s32[1]);
		res[1] = byteReverse(s32[0]);
		return *reinterpret_cast<uint64*>(res);
	}

	static INLINE_NO_DEBUG int16 byteReverse(int16 s)
	{
		return (int16)byteReverse((uint16)s);
	}

	static INLINE_NO_DEBUG int32 byteReverse(int32 s)
	{
		return (int32)byteReverse((uint32)s);
	}

	static INLINE_NO_DEBUG int64 byteReverse(int64 s)
	{
		return (int64)byteReverse((uint64)s);
	}

	static INLINE_NO_DEBUG int8 standardValueFromNativeValue(int8 s)
	{
		return s;
	}

	static INLINE_NO_DEBUG uint8 standardValueFromNativeValue(uint8 s)
	{
		return s;
	}

	static INLINE_NO_DEBUG int16 standardValueFromNativeValue(int16 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG uint16 standardValueFromNativeValue(uint16 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG int32 standardValueFromNativeValue(int32 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG uint32 standardValueFromNativeValue(uint32 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG int64 standardValueFromNativeValue(int64 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG uint64 standardValueFromNativeValue(uint64 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG float standardValueFromNativeValue(float s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		uint32 u = *reinterpret_cast<uint32*>(&s);
		uint32 res = byteReverse(u);
		return *reinterpret_cast<float*>(&res);
#endif
	}

	static INLINE_NO_DEBUG double standardValueFromNativeValue(double s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		uint64 u = *reinterpret_cast<uint64*>(&s);
		uint64 res = byteReverse(u);
		return *reinterpret_cast<double*>(&res);
#endif
	}


	static INLINE_NO_DEBUG int8 nativeValueFromStandardValue(int8 s)
	{
		return s;
	}

	static INLINE_NO_DEBUG uint8 nativeValueFromStandardValue(uint8 s)
	{
		return s;
	}

	static INLINE_NO_DEBUG int16 nativeValueFromStandardValue(int16 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG uint16 nativeValueFromStandardValue(uint16 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG int32 nativeValueFromStandardValue(int32 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG uint32 nativeValueFromStandardValue(uint32 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG int64 nativeValueFromStandardValue(int64 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG uint64 nativeValueFromStandardValue(uint64 s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		return byteReverse(s);
#endif
	}

	static INLINE_NO_DEBUG float nativeValueFromStandardValue(float s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		uint32 u = *reinterpret_cast<uint32*>(&s);
		uint32 res = byteReverse(u);
		return *reinterpret_cast<float*>(&res);
#endif
	}

	static INLINE_NO_DEBUG double nativeValueFromStandardValue(double s)
	{
#ifdef LITTLE_ENDIAN_ARCH
		return s;
#else
		uint64 u = *reinterpret_cast<uint64*>(&s);
		uint64 res = byteReverse(u);
		return *reinterpret_cast<double*>(&res);
#endif
	}

	static INLINE boolean01 negativeBit(int64 i)
	{
		return (boolean01)(uint64(i)>>63);
	}

	static INLINE boolean01 negativeBit(int32 i)
	{
		return uint32(i)>>31;
	}

	static INLINE boolean01 negativeBit(int8 i)
	{
		return negativeBit((int32)i);
	}

	static INLINE boolean01 negativeBit(int16 i)
	{
		return negativeBit((int32)i);
	}

	static INLINE_NO_DEBUG int hiOneBit(uint32 x)
	{
		x = x & (~(x >>1));
		float f = (float)x; 
		return ((uint32&)f >> 23) - 127;
	}

	static INLINE_NO_DEBUG uint lowOneBit(uint32 x)
	{
		x = x & uint32(-(int)x);
		float f = (float)x;
		return ((uint32&)f >> 23) - 127;
	}

	static INLINE_NO_DEBUG int hiZeroBit(uint32 n)
	{
		return hiOneBit(~n);
	}

	static INLINE_NO_DEBUG uint lowZeroBit(uint32 n)
	{
		return lowOneBit(~n);
	}

	static INLINE_NO_DEBUG uint8 saturate(uint8 x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		return ++x;
	}

	static INLINE_NO_DEBUG uint16 saturate(uint16 x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		return ++x;
	}

	static INLINE_NO_DEBUG uint32 saturate(uint32 x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return ++x;
	}

	static INLINE_NO_DEBUG uint64 saturate(uint64 x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		x |= x >> 32;
		return ++x;
	}

	static INLINE_NO_DEBUG int8 saturate(int8 x)
	{
		return (int8)saturate((uint8)x);
	}

	static INLINE_NO_DEBUG int16 saturate(int16 x)
	{
		return (int16)saturate((uint16)x);
	}

	static INLINE_NO_DEBUG int32 saturate(int32 x)
	{
		return (int32)saturate((uint32)x);
	}

	static INLINE_NO_DEBUG int64 saturate(int64 x)
	{
		return (int64)saturate((uint64)x);
	}

	template<typename t_Uint>
	static INLINE_NO_DEBUG boolean isPowOf2(t_Uint x)
	{
		return (x & (x - 1)) == 0;
	}

	template<typename t_UInt, typename t_UInt2>
	static INLINE_NO_DEBUG boolean isBit(t_UInt number, t_UInt2 bit)
	{
		return (number >> bit) & 1;
	}

	template<typename t_IntType, typename t_IntType2>
	static INLINE_NO_DEBUG t_IntType alignToGreater(t_IntType n, t_IntType2 a)
	{
		return ((n + a-1) / a) * a;
	}

	template<typename t_IntType, typename t_IntType2>
	static INLINE_NO_DEBUG t_IntType alignToLess(t_IntType n, t_IntType2 a)
	{
		return n - n % a;
	}

	template<typename t_IntType, typename t_IntType2>
	static INLINE_NO_DEBUG t_IntType alignToGreaterFast(t_IntType n, t_IntType2 a)
	{
		Assert( isPowOf2(a) );
		--a;
		return (n + a) & (~t_IntType(a));
	}

	template<typename t_IntType, typename t_IntType2>
	static INLINE_NO_DEBUG t_IntType alignToLessFast(t_IntType n, t_IntType2 a)
	{
		Assert( isPowOf2(a) );
		return n & (~t_IntType(a - 1));
	}


	//TODO Optimize
	static INLINE_NO_DEBUG int8 abs(int8 v)		{ return v >= 0 ? v : -v; }
	static INLINE_NO_DEBUG int16 abs(int16 v)	{ return v >= 0 ? v : -v; }
	static INLINE_NO_DEBUG int32 abs(int32 v)	{ return v >= 0 ? v : -v; }
	static INLINE_NO_DEBUG int64 abs(int64 v)	{ return v >= 0 ? v : -v; }
	static INLINE_NO_DEBUG int8 sign(int8 v)	{ return v >= 0 ? 1 : -1; }
	static INLINE_NO_DEBUG int16 sign(int16 v)	{ return v >= 0 ? 1 : -1; }
	static INLINE_NO_DEBUG int32 sign(int32 v)	{ return v >= 0 ? 1 : -1; }
	static INLINE_NO_DEBUG int64 sign(int64 v)	{ return v >= 0 ? 1 : -1; }

	static INLINE_NO_DEBUG uint8 min(uint8 a, uint8 b)		{ return a < b ? a : b; }
	static INLINE_NO_DEBUG int8 min(int8 a, int8 b)			{ return a < b ? a : b; }
	static INLINE_NO_DEBUG uint16 min(uint16 a, uint16 b)	{ return a < b ? a : b; }
	static INLINE_NO_DEBUG int16 min(int16 a, int16 b)		{ return a < b ? a : b; }
	static INLINE_NO_DEBUG uint32 min(uint32 a, uint32 b)	{ return a < b ? a : b; }
	static INLINE_NO_DEBUG int32 min(int32 a, int32 b)		{ return a < b ? a : b; }
	static INLINE_NO_DEBUG uint64 min(uint64 a, uint64 b)	{ return a < b ? a : b; }
	static INLINE_NO_DEBUG int64 min(int64 a, int64 b)		{ return a < b ? a : b; }

	static INLINE_NO_DEBUG uint8 max(uint8 a, uint8 b)		{ return a > b ? a : b; }
	static INLINE_NO_DEBUG int8 max(int8 a, int8 b)			{ return a > b ? a : b; }
	static INLINE_NO_DEBUG uint16 max(uint16 a, uint16 b)	{ return a > b ? a : b; }
	static INLINE_NO_DEBUG int16 max(int16 a, int16 b)		{ return a > b ? a : b; }
	static INLINE_NO_DEBUG uint32 max(uint32 a, uint32 b)	{ return a > b ? a : b; }
	static INLINE_NO_DEBUG int32 max(int32 a, int32 b)		{ return a > b ? a : b; }
	static INLINE_NO_DEBUG uint64 max(uint64 a, uint64 b)	{ return a > b ? a : b; }
	static INLINE_NO_DEBUG int64 max(int64 a, int64 b)		{ return a > b ? a : b; }

	static INLINE_NO_DEBUG void minimize(uint8& a, uint8 b)		{ if(a > b) a = b; }
	static INLINE_NO_DEBUG void minimize(int8& a, int8 b)		{ if(a > b) a = b; }
	static INLINE_NO_DEBUG void minimize(uint16& a, uint16 b)	{ if(a > b) a = b; }
	static INLINE_NO_DEBUG void minimize(int16& a, int16 b)		{ if(a > b) a = b; }
	static INLINE_NO_DEBUG void minimize(uint32& a, uint32 b)	{ if(a > b) a = b; }
	static INLINE_NO_DEBUG void minimize(int32& a, int32 b)		{ if(a > b) a = b; }
	static INLINE_NO_DEBUG void minimize(uint64& a, uint64 b)	{ if(a > b) a = b; }
	static INLINE_NO_DEBUG void minimize(int64& a, int64 b)		{ if(a > b) a = b; }

	static INLINE_NO_DEBUG void maximize(uint8& a, uint8 b)		{ if(a < b) a = b; }
	static INLINE_NO_DEBUG void maximize(int8& a, int8 b)		{ if(a < b) a = b; }
	static INLINE_NO_DEBUG void maximize(uint16& a, uint16 b)	{ if(a < b) a = b; }
	static INLINE_NO_DEBUG void maximize(int16& a, int16 b)		{ if(a < b) a = b; }
	static INLINE_NO_DEBUG void maximize(uint32& a, uint32 b)	{ if(a < b) a = b; }
	static INLINE_NO_DEBUG void maximize(int32& a, int32 b)		{ if(a < b) a = b; }
	static INLINE_NO_DEBUG void maximize(uint64& a, uint64 b)	{ if(a < b) a = b; }
	static INLINE_NO_DEBUG void maximize(int64& a, int64 b)		{ if(a < b) a = b; }

	static INLINE_NO_DEBUG uint8 clamp(uint8 val, uint8 low, uint8 hi)		{ return val >= low ? (val <= hi ? val : hi) : low; }
	static INLINE_NO_DEBUG int8 clamp(int8 val, int8 low, int8 hi)			{ return val >= low ? (val <= hi ? val : hi) : low; }
	static INLINE_NO_DEBUG uint16 clamp(uint16 val, uint16 low, uint16 hi)	{ return val >= low ? (val <= hi ? val : hi) : low; }
	static INLINE_NO_DEBUG int16 clamp(int16 val, int16 low, int16 hi)		{ return val >= low ? (val <= hi ? val : hi) : low; }
	static INLINE_NO_DEBUG uint32 clamp(uint32 val, uint32 low, uint32 hi)	{ return val >= low ? (val <= hi ? val : hi) : low; }
	static INLINE_NO_DEBUG int32 clamp(int32 val, int32 low, int32 hi)		{ return val >= low ? (val <= hi ? val : hi) : low; }
	static INLINE_NO_DEBUG uint64 clamp(uint64 val, uint64 low, uint64 hi)	{ return val >= low ? (val <= hi ? val : hi) : low; }
	static INLINE_NO_DEBUG int64 clamp(int64 val, int64 low, int64 hi)		{ return val >= low ? (val <= hi ? val : hi) : low; }

	static INLINE_NO_DEBUG boolean isFinite(int8 x){ return true; }
	static INLINE_NO_DEBUG boolean isFinite(uint8 x){ return true; }
	static INLINE_NO_DEBUG boolean isFinite(int16 x){ return true; }
	static INLINE_NO_DEBUG boolean isFinite(uint16 x){ return true; }
	static INLINE_NO_DEBUG boolean isFinite(int32 x){ return true; }
	static INLINE_NO_DEBUG boolean isFinite(uint32 x){ return true; }
	static INLINE_NO_DEBUG boolean isFinite(int64 x){ return true; }
	static INLINE_NO_DEBUG boolean isFinite(uint64 x){ return true; }
}
