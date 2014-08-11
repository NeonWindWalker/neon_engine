#pragma once

#include "seed.h"
#include "int.h"
#include "float.h"
#include <math.h>
#include <float.h>

//Min/Max guard
#ifdef min
#error "'min' macros is defined! Probably by 'windows.h' try to define 'NOMINMAX' before include 'windows.h'"
#endif

#ifdef max
#error "'max' macros is defined! Probably by 'windows.h' try to define 'NOMINMAX' before include 'windows.h'"
#endif

namespace Base
{
	template<class t_Scalar>
	struct RealScalarT
	{
		typedef t_Scalar Type;
	};

	static INLINE uint8 flooru8_01(float f)
	{
		return uint8(f * 255.0f);
	}

	static INLINE uint uintAndPFrac(float& fr, float f)
	{
		float fl = floor(f);
		fr = f - fl;
		return (uint)fl; 
	}

	template <typename t_Val>
	static INLINE t_Val minimum(const t_Val& a, const t_Val& b)
	{
		return min(a, b);
	}

	template <typename t_Val>
	static INLINE t_Val maximum(const t_Val& a, const t_Val& b)
	{
		return max(a, b);
	}

	template <typename t_Val>
	static INLINE t_Val clampt(const t_Val& val, const t_Val& low, const t_Val& hi)
	{
		return clamp(val, low, hi);
	}

	template<typename t_Val>
	static INLINE uint indexOfMax(const t_Val& a, const t_Val& b)
	{ 
		return  a > b ? 0 : 1;
	}

	template<typename t_Val>
	static INLINE uint indexOfMin(const t_Val& a, const t_Val& b)
	{ 
		return a < b ? 0 : 1; 
	}

	template<typename t_Val>
	static INLINE uint indexOfMax(const t_Val& v0, const t_Val& v1, const t_Val& v2)
	{ 
		return  v0 > v1 ? (v0 > v2 ? 0 : 2) : (v1 > v2 ? 1 : 2);
	}

	template<typename t_Val>
	static INLINE uint indexOfMin(const t_Val& v0, const t_Val& v1, const t_Val& v2)
	{ 
		return  v0 < v1 ? (v0 < v2 ? 0 : 2) : (v1 < v2 ? 1 : 2);
	}

	template<typename t_Value>
	static INLINE t_Value clampR(const t_Value& val, const t_Value& radiys)
	{
		return clamp(val, -radiys, radiys);
	}

	template<typename ValueType> inline boolean equals(const ValueType& a, const ValueType& b, const ValueType& eps)
	{
		return Base::abs(a - b) < eps;
	}

	template<class t_Val, class t_Scalar>
	static INLINE t_Val lerp(const t_Val& v0, const t_Val& v1, const t_Scalar& t)
	{
		return v0 + (v1 - v0) * t;
	}
	
	static INLINE int rand(int a, int b)
	{ 
		int r = (::rand() + (::rand()<<11)) & 0x3fffffff;
		return a + (r%(b + 1 - a));
	}

	static INLINE double rand(double a, double b)
	{
		const double r = ::rand() / double(RAND_MAX);
		return r*(b-a) + a;
	}

	static INLINE float rand(float a, float b)
	{
		const float r = ::rand() / float(RAND_MAX);
		return r*(b-a) + a;
	}

	template<class t_Val>
	static INLINE t_Val convert01to11(const t_Val& src)
	{
		return src * (typename t_Val::Scalar_t(2.0)) + (typename t_Val::Scalar_t(-1.0));
	}

	template<class t_Val>
	static INLINE t_Val convert11to01(const t_Val& src)	
	{
		return src * (typename t_Val::Scalar_t(0.5)) + (typename t_Val::Scalar_t(0.5));
	}


	/// Return [aMin,aMax] intersection [bMin,bMax]
	template<typename Scalar> static INLINE boolean intersectionMinMaxAndMinMax(Scalar& resMin, Scalar& resMax, Scalar aMin, Scalar aMax, Scalar bMin, Scalar bMax)
	{
		resMin = Base::max(aMin,bMin);
		resMax = Base::min(aMax,bMax);
		return resMin < resMax;
	}
	/// Return [aMin,aMax] union [bMin,bMax]
	template<typename Scalar> static INLINE boolean unionMinMaxAndMinMax(Scalar& resMin, Scalar& resMax, Scalar aMin, Scalar aMax, Scalar bMin, Scalar bMax)
	{
		if(abs(aMin + aMax - bMin - bMax) <= abs(aMax - aMin + bMax - bMin)){
			resMin = Base::min(aMin,bMin);
			resMax = Base::max(aMax,bMax);
			return true;
		}
		return false;
	}
	/// Return [aMin,aMax] - [bMin,bMax)
	template<typename Scalar> static INLINE uint minMaxSubstractMinMax(Scalar& res0Min, Scalar& res0Max, Scalar& res1Min, Scalar& res1Max, Scalar aMin, Scalar aMax, Scalar bMin, Scalar bMax)
	{
		if(bMax < aMin || bMin > aMax)
			return 0;

		if(bMin <= aMin)
		{
			if(bMax > aMax)
				return 0;
			res0Min = bMax;
			res0Max = aMax;
			return 1;
		}
		if(bMax >= aMax)
		{
			if(bMin < aMin)
				return 0;
			res0Min = aMin;
			res0Max = bMin;
			return 1;
		}

		res0Min = aMin;
		res0Max = bMin;
		res1Min = bMax;
		res1Max = aMax;
		return 2;
	}
	/// Is intersect [aMin,aMax] and [bMin,bMax]
	template<typename Scalar> static INLINE boolean intersectMinMaxAndMinMax(Scalar aMin, Scalar aMax, Scalar bMin, Scalar bMax)
	{
		return Base::max(aMin,bMin) <= Base::min(aMax,bMax);
	}

	/// Return [aCenter - aHalfsize, aCenter + aHalfsize] intersection [bCenter - bHalfsize, bCenter + bHalfsize]
	template<typename Scalar> static INLINE boolean intersectionDiapasonAndDiapason(Scalar& resCenter, Scalar& resHalfsize, Scalar aCenter, Scalar aHalfsize, Scalar bCenter, Scalar bHalfsize)
	{
		Scalar ab = bCenter - aCenter;
		Scalar s = sign(ab);
		Scalar D = abs(ab);
		resHalfsize = (aHalfsize + bHalfsize - D) / Scalar(2);
		resCenter = aCenter + s*(aHalfsize - resHalfsize);
		return resHalfsize >= Scalar(0);
	}
	/// Return [aCenter - aHalfsize, aCenter + aHalfsize] union [bCenter - bHalfsize, bCenter + bHalfsize], if union cant be, return false
	template<typename Scalar> static INLINE boolean unionDiapasonAndDiapason(Scalar& resCenter, Scalar& resHalfsize, Scalar aCenter, Scalar aHalfsize, Scalar bCenter, Scalar bHalfsize)
	{
		Scalar ab = bCenter - aCenter;
		Scalar s = sign(ab);
		Scalar D = abs(ab);
		if(D <= aHalfsize + bHalfsize){
			resHalfsize = (aHalfsize + bHalfsize + D) / Scalar(2);
			resCenter = aCenter - aHalfsize + s*resHalfsize;
			return true;
		}
		return false;
	}
	/// Is intersect diapasons [aCenter - aHalfsize, aCenter + aHalfsize] and [bCenter - bHalfsize, bCenter + bHalfsize]
	template<typename Scalar> static INLINE boolean intersectDiapasonAndDiapason(Scalar aCenter, Scalar aHalfsize, Scalar bCenter, Scalar bHalfsize)
	{
		return aHalfsize + bHalfsize >= abs(bCenter - aCenter);
	}
	/// Return true if point in [aMin,aMax]
	template<typename Scalar> static INLINE boolean minMaxContainPoint(Scalar min, Scalar max, Scalar point)
	{
		return point >= min && point <= max;
	}
	/// Return true if point in [aCenter - aHalfsize, aCenter + aHalfsize]
	template<typename Scalar> static INLINE boolean diapasonContainPoint(Scalar center, Scalar halfsize, Scalar point)
	{
		return abs(point - center) <= halfsize;
	}
	/// Return true if [aMin,aMax] contain [bMin,bMax]
	template<typename Scalar> static INLINE boolean minMaxContainMinMax(Scalar aMin, Scalar aMax, Scalar bMin, Scalar bMax)
	{
		return bMax <= aMax && bMin >= aMin;
	}
	/// Return true if [aCenter - aHalfsize, aCenter + aHalfsize] contain [bCenter - bHalfsize, bCenter + bHalfsize]
	template<typename Scalar> static INLINE boolean diapasonContainDiapason(Scalar aCenter, Scalar aHalfsize, Scalar bCenter, Scalar bHalfsize)
	{
		return abs(bCenter - aCenter) <= aHalfsize - bHalfsize;
	}

	template<typename Scalar> static INLINE void minMaxIncludePoint(Scalar& min, Scalar& max, Scalar val)
	{
		if(min > val){
			min = val;
			return;
		}
		if(max < val){
			max = val;
			return;
		}
	}

	

	template<class t_Value>
	class IsRawType
	{
	public:
		static const boolean gcIsRawType = false;
	};

	template<>
	class IsRawType<uint8>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<int8>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<uint16>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<int16>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<uint32>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<int32>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<uint64>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<int64>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<float>
	{
	public:
		static const boolean gcIsRawType = true;
	};

	template<>
	class IsRawType<double>
	{
	public:
		static const boolean gcIsRawType = true;
	};
}


#define USING_BASE_MATH \
using Base::abs; \
using Base::floor; \
using Base::ceil; \
using Base::mod; \
using Base::sqrt; \
using Base::pow; \
using Base::exp; \
using Base::exp2; \
using Base::log; \
using Base::log2; \
using Base::log10; \
using Base::sin; \
using Base::cos; \
using Base::tan; \
using Base::asin; \
using Base::acos; \
using Base::atan; \
using Base::atan2; \
using Base::rand;
