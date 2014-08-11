#pragma once

#include "seed.h"

#include <math.h>
#include <float.h>

namespace Base
{
	static const double gcE = 2.71828182845904523536;
	static const double gcPi = 3.1415926535897932384626433832795;
	static const double gcHalfPi  = gcPi * 0.5;
	static const double gcTwoPi   = gcPi * 2.0;
	static const double gcInvPi   = 1.0 / gcPi;
	static const double gcPiOver180 = gcPi / 180.0;
	static const double gcPiUnder180 = 180.0 / gcPi;
	static const double gcPi2 = gcPi*gcPi;
	static const double gcIntSqrtTwo = 0.70710678118654752440084436210485;
	static const double gcLn2 = 0.693147180559945309417;

	static const float gcfE = float(gcE);
	static const float gcfPi = float(gcPi);
	static const float gcfHalfPi  = float(gcHalfPi);
	static const float gcfTwoPi   = float(gcTwoPi);
	static const float gcfInvPi   = float(gcInvPi);
	static const float gcfPiOver180 = float(gcPiOver180);
	static const float gcfPiUnder180 = float(gcPiUnder180);
	static const float gcfPi2 = float(gcPi2);
	static const float gcfIntSqrtTwo = float(gcIntSqrtTwo);
	static const float gcfLn2 = 0.693147180559945309417f;

	static const double gcEpsilon = 0.000000059604644775390625;// 2^-24; 24 = half double mantises size
	static const double gcOnePlusEpsilon = 1.0 + gcEpsilon;
	static const double gcOneMinusEpsilon = 1.0 - gcEpsilon;
	static const double gcBig = 1.3407807929942597099574024998206e154; // 2^512
	static const double gcSmall = 7.4583407312002067432909653154629e-155; // 2^-512

	static const float gcfEpsilon = 0.000244140625; // 2^-12; 12 = half float mantises size
	static const float gcfOnePlusEpsilon = 1.0f + gcfEpsilon;
	static const float gcfOneMinusEpsilon = 1.0f - gcfEpsilon;
	static const float gcfBig = 4294967296.0f; // 2^32;
	static const float gcfSmall = 0.00000000023283064365386962890625f; // 2^-32


	template<class T> struct FloatConstants;

	template<> 
	struct FloatConstants<double>
	{
		static INLINE_NO_DEBUG double epsilon() { return gcEpsilon; }
		static INLINE_NO_DEBUG double onePlusEpsilon() { return gcOnePlusEpsilon; }
		static INLINE_NO_DEBUG double oneMinusEpsilon() { return gcOneMinusEpsilon; }
		static INLINE_NO_DEBUG double big() { return gcBig; }
		static INLINE_NO_DEBUG double small() { return gcSmall; }
	};

	template<> 
	struct FloatConstants<float>
	{
		static INLINE_NO_DEBUG float epsilon() { return gcfEpsilon; }
		static INLINE_NO_DEBUG float onePlusEpsilon() { return gcfOnePlusEpsilon; }
		static INLINE_NO_DEBUG float oneMinusEpsilon() { return gcfOneMinusEpsilon; }
		static INLINE_NO_DEBUG float big() { return gcfBig; }
		static INLINE_NO_DEBUG float small() { return gcfSmall; }
	};
	
	static INLINE_NO_DEBUG boolean01 negativeBit(float f)
	{
		uint32 x = *reinterpret_cast<const uint32*>(&f);
		return x>>31;
	}

	static INLINE_NO_DEBUG boolean01 negativeBit(double f)
	{
#ifdef LITTLE_ENDIAN_ARCH
		uint32 x = *reinterpret_cast<const uint32*>(&f);
#else
		uint32 x = *(reinterpret_cast<const uint32*>(&f) + 1);
#endif
		return x>>31;
	}

	static INLINE_NO_DEBUG float sign(float f)
	{
		static const float one = 1.0f;
		static const uint32 n_one = *reinterpret_cast<const uint32*>(&one);
		uint32 x = *reinterpret_cast<const uint32*>(&f);
		uint32 nret = n_one | (x & (1<<31));
		return *reinterpret_cast<float*>(&nret);
	}

	static INLINE_NO_DEBUG double sign(double f)
	{
		static const double one = 1.0f;
		static const uint64 n_one = *reinterpret_cast<const uint64*>(&one);
		uint64 x = *reinterpret_cast<const uint64*>(&f);
		uint64 nret = n_one | (x & (uint64(1)<<63));
		return *reinterpret_cast<double*>(&nret);
	}

	static INLINE_NO_DEBUG float min(float a, float b)
	{
		return a < b ? a : b;
	}

	static INLINE_NO_DEBUG double min(double a, double b)
	{
		return a < b ? a : b;
	}

	static INLINE_NO_DEBUG float max(float a, float b)
	{
		return a > b ? a : b;
	}

	static INLINE_NO_DEBUG double max(double a, double b)
	{
		return a > b ? a : b;
	}

	static INLINE_NO_DEBUG void minimize(float& a, float b)
	{
		if(a > b)
			a = b;
	}

	static INLINE_NO_DEBUG void minimize(double& a, double b)
	{
		if(a > b)
			a = b;
	}

	static INLINE_NO_DEBUG void maximize(float& a, float b)
	{
		if(a < b) a = b;
	}

	static INLINE_NO_DEBUG void maximize(double& a, double b)
	{
		if(a < b)
			a = b;
	}

	static INLINE_NO_DEBUG float clamp(float val, float low, float hi)
	{
		return val >= low ? (val <= hi ? val : hi) : low;
	}

	static INLINE_NO_DEBUG double clamp(double val, double low, double hi)
	{
		return val >= low ? (val <= hi ? val : hi) : low;
	}

	static INLINE_NO_DEBUG float clamp01(float val)
	{
		return clamp(val, 0.0f, 1.0f);
	}

	static INLINE_NO_DEBUG double clamp01(double val)
	{
		return clamp(val, 0.0, 1.0);
	}

	static INLINE_NO_DEBUG float clamp11(float val)
	{
		return clamp(val, 0.0f, 1.0f);
	}

	static INLINE_NO_DEBUG double clamp11(double val)
	{
		return clamp(val, 0.0, 1.0);
	}

	static INLINE_NO_DEBUG float abs(const float a)
	{ 
		return ::fabs(a);
	}

	static INLINE_NO_DEBUG double abs(const double a)
	{ 
		return ::fabs(a);
	}

	INLINE_NO_DEBUG boolean equals(float a, float b, float eps)
	{
		return Base::abs(a - b) < eps;
	}

	INLINE_NO_DEBUG boolean equals(double a, double b, double eps)
	{
		return Base::abs(a - b) < eps;
	}

	static INLINE_NO_DEBUG float floor(float a)
	{ 
		return ::floor(a);
	}

	static INLINE_NO_DEBUG double floor(double a)
	{ 
		return ::floor(a);
	}

	static INLINE_NO_DEBUG float round(float a)
	{ 
		return ::floor(a + 0.5f);
	}

	static INLINE_NO_DEBUG double round(double a)
	{ 
		return ::floor(a + 0.5);
	}

	static INLINE_NO_DEBUG float ceil(float a)
	{ 
		return ::ceil(a);
	}

	static INLINE_NO_DEBUG double ceil(double a)
	{ 
		return ::ceil(a);
	}

	static INLINE_NO_DEBUG float pfrac(float a)
	{ 
		return a - floor(a);
	}

	static INLINE_NO_DEBUG double pfrac(double a)
	{ 
		return a - floor(a);
	}

	static INLINE_NO_DEBUG float pfrac(float a, float divider)
	{ 
		float f = a / divider;
		return (f - floor(f)) * divider;
	}

	static INLINE_NO_DEBUG double pfrac(double a, double divider)
	{ 
		double f = a / divider;
		return (f - floor(f)) * divider;
	}

	static INLINE_NO_DEBUG float frac(float a)
	{ 
		return a >= 0.0f ? a - floor(a) : -(abs(a) - floor(abs(a)));
	}

	static INLINE_NO_DEBUG double frac(double a)
	{ 
		return a >= 0.0 ? a - floor(a) : -(abs(a) - floor(abs(a)));
	}

	static INLINE_NO_DEBUG float angleToPiRange(float a)
	{ 
		float f;
		if(a >= 0.0f)
			f = pfrac(a / gcfTwoPi) * gcfTwoPi;
		else
			f = gcfTwoPi - pfrac(-a / gcfTwoPi) * gcfTwoPi;

		if(f > gcfHalfPi)
			return f - gcfTwoPi;
		else
			return f;
	}

	static INLINE_NO_DEBUG double angleToPiRange(double a)
	{ 
		double f;
		if(a >= 0.0)
			f = pfrac(a / gcTwoPi) * gcTwoPi;
		else
			f = gcTwoPi - pfrac(-a / gcTwoPi) * gcTwoPi;

		if(f > gcHalfPi)
			return f - gcTwoPi;
		else
			return f;
	}

	static INLINE_NO_DEBUG float mod(float a, float b)
	{ 
		return ::fmod(a,b);
	}

	static INLINE_NO_DEBUG double mod(double a, double b)
	{ 
		return ::fmod(a,b);
	}

	static INLINE_NO_DEBUG float sqrt(float a)
	{ 
		return ::sqrt(a);
	}

	static INLINE_NO_DEBUG double sqrt(double a)
	{ 
		return ::sqrt(a);
	}

	static INLINE_NO_DEBUG float rcp(float a)
	{ 
		return 1.0f / a;
	}
	static INLINE_NO_DEBUG double rcp(double a)
	{ 
		return 1.0 / a;
	}

	static INLINE_NO_DEBUG float rsq(float a)
	{ 
		return 1.0f/::sqrt(a);
	}
	static INLINE_NO_DEBUG double rsq(double a)
	{ 
		return 1.0/::sqrt(a);
	}

	static INLINE_NO_DEBUG float rsqFast(float a)
	{
		uint32 ua = *reinterpret_cast<uint32*>(&a);
		ua = (0xBE800000 - ua)>>1;
		float y = *reinterpret_cast<float*>(&ua);
//		*(unsigned long*)&y = (0xBE800000/*(0x3F800000 << 1) + 0x3F800000*/ - *(unsigned long*)&a) >> 1;
		//*reinterpret_cast<uint32*>(&y) = (0xBE800000/*(0x3F800000 << 1) + 0x3F800000*/ - *reinterpret_cast<uint32*>(&a)) >> 1;
		return y * (1.47f - 0.47f * a * y * y);
	}

	static INLINE_NO_DEBUG double rsqFast(double a)
	{
		return rsq(a);
	}

	static INLINE_NO_DEBUG float sqrtFast(float a)
	{ 
		return 1.0f / rsqFast(a);
	}

	static INLINE_NO_DEBUG double sqrtFast(double a)
	{ 
		return sqrt(a);
	}

	static INLINE_NO_DEBUG float pow(float x, float y)
	{ 
		return ::pow(x,y);
	}
	
	static INLINE_NO_DEBUG double pow(double x, double y)
	{ 
		return ::pow(x,y);
	}
	
	static INLINE_NO_DEBUG float exp(float a)
	{ 
		return ::exp(a);
	}
	
	static INLINE_NO_DEBUG double exp(double a)
	{ 
		return ::exp(a);
	}
	
	static INLINE_NO_DEBUG float exp2(float a)
	{ 
		return ::pow(2.0f,a);
	}
	
	static INLINE_NO_DEBUG double exp2(double a)
	{ 
		return ::pow(2.0,a);
	}
	
	static INLINE_NO_DEBUG float log(float a, float e)
	{ 
		return ::log(a) / ::log(e);
	}
	
	static INLINE_NO_DEBUG double log(double a, double e)
	{ 
		return ::log(a) / ::log(e);
	}
	
	static INLINE_NO_DEBUG float logE(float a)
	{ 
		return ::log(a);
	}
	
	static INLINE_NO_DEBUG double logE(double a)
	{ 
		return ::log(a);
	}
	
	static INLINE_NO_DEBUG float log10(float a)
	{ 
		return ::log10(a);
	}
	
	static INLINE_NO_DEBUG double log10(double a)
	{ 
		return ::log10(a);
	}
	
	static INLINE_NO_DEBUG float log2(float a)
	{ 
		return (::log(a) / gcfLn2);
	}
	
	static INLINE_NO_DEBUG double log2(double a)
	{ 
		return (::log(a) / gcLn2);
	}

	static INLINE_NO_DEBUG float radiansFromDegrees(const float a)
	{ 
		return gcfPiOver180*a;
	}

	static INLINE_NO_DEBUG double radiansFromDegrees(const double a)
	{ 
		return gcPiOver180*a;
	}

	static INLINE_NO_DEBUG float degreesFromRadians(const float a)
	{ 
		return gcfPiUnder180*a;
	}

	static INLINE_NO_DEBUG double degreesFromRadians(const double a)
	{ 
		return gcPiUnder180*a;
	}

	static INLINE_NO_DEBUG float sin(float a)
	{ 
		return ::sin(a);
	}

	static INLINE_NO_DEBUG double sin(double a)
	{ 
		return ::sin(a);
	}

	static INLINE_NO_DEBUG float cos(float a)
	{ 
		return ::cos(a);
	}
	
	static INLINE_NO_DEBUG double cos(double a)
	{
		return ::cos(a);
	}
	
	static INLINE_NO_DEBUG void cosSin(float& c, float& s, const float angle)
	{
#if defined(_MSC_VER) && (defined(ARCH_X86))
		float localCos, localSin;
		float local = angle;
		_asm    fld     local
		_asm    fsincos
		_asm    fstp    localCos
		_asm    fstp    localSin
		c = localCos;
		s = localSin;
#else
		c = cos(angle);
		s = sin(angle);
#endif
	}

	static INLINE_NO_DEBUG float tan(float a)
	{ 
		return ::tan(a);
	}

	static INLINE_NO_DEBUG double tan(double a)
	{ 
		return ::tan(a);
	}

	static INLINE_NO_DEBUG float asin(float f)
	{ 
		return ::asin(f);
	}

	static INLINE_NO_DEBUG double asin(double f)
	{
		return ::asin(f);
	}

	static  INLINE_NO_DEBUG float acos(float f)
	{ 
		return ::acos(f);
	}

	static INLINE_NO_DEBUG double acos(double f)
	{ 
		return ::acos(f);
	}

	static INLINE_NO_DEBUG float atan(float a)
	{ 
		return ::atan(a);
	}

	static INLINE_NO_DEBUG double atan(double a)
	{ 
		return ::atan(a);
	}
	/// Arctangent of (y/x) with correct sign.
	static INLINE_NO_DEBUG float atan2(float y, float x)
	{ 
		return ::atan2(y,x);
	}
	/// Arctangent of (y/x) with correct sign.
    static INLINE_NO_DEBUG double atan2(double y, double x)
	{ 
		return ::atan2(y,x);
	}

	/// Returns true if the passed number is a finite floating point number as opposed to INF, NAN, etc.
	static INLINE_NO_DEBUG boolean isFinite(float value)
	{
#ifdef _WIN32
		return (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(value) ));
#else
        return isfinite(value);
#endif
	}

	/// Returns true if the passed number is a finite floating point number as opposed to INF, NAN, etc.
	static INLINE_NO_DEBUG boolean isFinite(double value)
	{
#ifdef _WIN32
		return (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(value) ));
#else
        return isfinite(value);
#endif
	}
	
}

