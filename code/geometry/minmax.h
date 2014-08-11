#pragma once

#include "minmax_asm.h"

namespace Geometry 
{

template<class t_Scalar>
class MinMax2 : public SMinMax2f
{
public:
	typedef t_Scalar Scalar_t;
	typedef TVector2<t_Scalar> Vector_t;
	typedef typename TVector2<t_Scalar>::ScalarOptimal_t ScalarOptimal_t;

	TVector2<t_Scalar> min, max;
	

	INLINE MinMax2() {}
	INLINE MinMax2(const Vector_t& srcMin, const Vector_t& srcMax) : min(srcMin), max(srcMax) {}
	INLINE MinMax2(t_Scalar srcMinX, t_Scalar srcMinY, t_Scalar srcMaxX, t_Scalar srcMaxY) : min(srcMinX, srcMinY), max(srcMaxX, srcMaxY) {}
	INLINE void set(const Vector_t& srcMin, const Vector_t& srcMax){ min = srcMin; max = srcMax; }
	INLINE void set(t_Scalar srcMinX, t_Scalar srcMinY, t_Scalar srcMaxX, t_Scalar srcMaxY){ this->min.set(srcMinX, srcMinY); this->max.set(srcMaxX, srcMaxY); }

	INLINE boolean operator == (const MinMax2& other)const						{ return this->min == other.min && this->max == other.max; }
	INLINE boolean operator != (const MinMax2& other)const						{ return this->min != other.min || this->max != other.max; }

	INLINE Vector_t size()const { return this->max - this->min; }
	INLINE ScalarOptimal_t width()const { return this->max.x - this->min.x; }
	INLINE ScalarOptimal_t height()const { return this->max.y - this->min.y; }

	//INLINE boolean contain(const Vector_t& point)const { return Geometry::contain(*this, point); }
	//INLINE boolean contain(const MinMax2f& other)const { return Geometry::contain(*this, other); }
	INLINE void include(const Vector_t& point)
	{
		minimize(min, point);
		maximize(max, point);
	}
	INLINE void include(const MinMax2& other)
	{
		minimize(min, other.min);
		maximize(max, other.max);
	}
	//INLINE boolean intersect(const SMinMax2f& b)const { return Geometry::intersect(*this, b); }
	INLINE boolean intersection(MinMax2& ret, const MinMax2& b)const
	{
		return Base::intersectionMinMaxAndMinMax<Scalar_t>(ret.min.x, ret.max.x, min.x, max.x, b.min.x, b.max.x) && Base::intersectionMinMaxAndMinMax<Scalar_t>(ret.min.y, ret.max.y, min.y, max.y, b.min.y, b.max.y);
	}
	//INLINE void build(const Vector_t* v, const Vector_t* v_end) { Geometry::build(*this, v, v_end); }
};

typedef MinMax2<float> MinMax2f;
typedef MinMax2<double> MinMax2d;

class MinMax2i : public SMinMax2i
{
public:
	typedef int Scalar_t;

	INLINE MinMax2i() {}
	INLINE MinMax2i(const Vector2i& srcMin, const Vector2i& srcMax) : SMinMax2i(srcMin, srcMax) {}
	INLINE MinMax2i(const int srcMinX, const int srcMinY, const int srcMaxX, const int srcMaxY) : SMinMax2i(Vector2i(srcMinX,srcMinY), Vector2i(srcMaxX,srcMaxY)) {}
	INLINE void set(const Vector2i& srcMin, const Vector2i& srcMax){ min = srcMin; max = srcMax; }
	INLINE void set(const int srcMinX, const int srcMinY, const int srcMaxX, const int srcMaxY){ this->min.set(srcMinX,srcMinY); this->max.set(srcMaxX,srcMaxY); }

	INLINE boolean operator == (const MinMax2i& other)const						{ return this->min == other.min && this->max == other.max; }
	INLINE boolean operator != (const MinMax2i& other)const						{ return this->min != other.min || this->max != other.max; }

	INLINE Vector2i size()const { return this->max - this->min; }
	INLINE int width()const { return this->max.x - this->min.x; }
	INLINE int height()const { return this->max.y - this->min.y; }

	INLINE boolean contain(const Vector2i& point)const { return Geometry::contain(*this, point); }
	INLINE boolean contain(const MinMax2i& other)const { return Geometry::contain(*this, other); }
	INLINE void include(const Vector2i& point) { Geometry::include(*this, point); }
	INLINE void include(const MinMax2i& other) { Geometry::include(*this, other); }
	INLINE boolean intersect(const SMinMax2i& b)const { return Geometry::intersect(*this, b); }
	INLINE boolean intersection(SMinMax2i& ret, const SMinMax2i& b)const { return Geometry::intersection(ret, *this, b); }
	INLINE void build(const Vector2i* v, const Vector2i* v_end) { Geometry::build(*this, v, v_end); }
};


class MinMax3f : public SMinMax3f
{
public:
	typedef float Scalar_t;

	INLINE MinMax3f() {}
	INLINE MinMax3f(const Vector3f& srcMin, const Vector3f& srcMax) : SMinMax3f(srcMin, srcMax) {}
	INLINE void set(const Vector3f& srcMin, const Vector3f& srcMax){ min = srcMin; max = srcMax; }

	INLINE boolean operator == (const MinMax3f& other)const						{ return this->min == other.min && this->max == other.max; }
	INLINE boolean operator != (const MinMax3f& other)const						{ return this->min != other.min || this->max != other.max; }

	INLINE Vector3f center()const { return (this->max + this->min)*0.5f; }
	INLINE Vector3f size()const { return this->max - this->min; }
	INLINE float width()const { return this->max.x - this->min.x; }
	INLINE float height()const { return this->max.y - this->min.y; }
	INLINE float depth()const { return this->max.z - this->min.z; }

	INLINE boolean contain(const Vector3f& point)const { return Geometry::contain(*this, point); }
	INLINE boolean contain(const MinMax3f& other)const { return Geometry::contain(*this, other); }
	INLINE void include(const Vector3f& point) { Geometry::include(*this, point); }
	INLINE void include(const MinMax3f& other) { Geometry::include(*this, other); }
	INLINE boolean intersect(const SMinMax3f& b)const { return Geometry::intersect(*this, b); }
	INLINE boolean intersection(SMinMax3f& ret, const SMinMax3f& b)const { return Geometry::intersection(ret, *this, b); }
	INLINE void build(const Vector3f* v, const Vector3f* v_end) { Geometry::build(*this, v, v_end); }
};


class MinMax4f : public SMinMax4f
{
public:
	typedef float Scalar_t;

	INLINE MinMax4f() {}
	INLINE MinMax4f(const Vector4f& srcMin, const Vector4f& srcMax) : SMinMax4f(srcMin, srcMax) {}
	INLINE void set(const Vector4f& srcMin, const Vector4f& srcMax){ min = srcMin; max = srcMax; }

	INLINE boolean operator == (const MinMax4f& other)const						{ return this->min == other.min && this->max == other.max; }
	INLINE boolean operator != (const MinMax4f& other)const						{ return this->min != other.min || this->max != other.max; }

	INLINE Vector4f size()const { return this->max - this->min; }

	INLINE boolean contain(const Vector4f& point)const { return Geometry::contain(*this, point); }
	INLINE boolean contain(const MinMax4f& other)const { return Geometry::contain(*this, other); }
	INLINE void include(const Vector4f& point) { Geometry::include(*this, point); }
	INLINE void include(const MinMax4f& other) { Geometry::include(*this, other); }
	INLINE boolean intersect(const SMinMax4f& b)const { return Geometry::intersect(*this, b); }
	INLINE boolean intersection(SMinMax4f& ret, const SMinMax4f& b)const { return Geometry::intersection(ret, *this, b); }
	INLINE void build(const Vector4f* v, const Vector4f* v_end) { Geometry::build(*this, v, v_end); }
};


}
