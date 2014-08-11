#pragma once
#include "../base/include.h"
#include "../reflection/include.h"
#include "../input/events.h"
#include "../geometry/include.h"

namespace GUI
{

USING_BASE_LIB
USING_BASE_MATH
using namespace ReflectionLib;
using namespace Geometry;

#ifdef GUILIB_USE_DOUBLE

typedef double Scalar;
typedef Base::Vector2d Vector2;
typedef Base::Vector3d Vector3;
typedef Base::Vector4d Vector4;
typedef Base::Quaterniond Quaternion;
typedef Base::Matrix22d Matrix22;
typedef Base::Matrix23d Matrix23;
typedef Base::Matrix33d Matrix33;
typedef Base::Matrix34d Matrix34;
typedef Base::Matrix44d Matrix44;

typedef Base::Vector4f Color;
typedef Base::Matrix44f ColorMatrix;

typedef Vector2d Vector;
typedef MinMax2d Bound;

#else

typedef float Scalar;
typedef Base::Vector2f Vector2;
typedef Base::Vector3f Vector3;
typedef Base::Vector4f Vector4;
typedef Base::Quaternionf Quaternion;
typedef Base::Matrix22f Matrix22;
typedef Base::Matrix23f Matrix23;
typedef Base::Matrix33f Matrix33;
typedef Base::Matrix34f Matrix34;
typedef Base::Matrix44f Matrix44;

typedef Base::Vector4f Color;
typedef Base::Matrix44f ColorMatrix;

typedef Vector2f Vector;
typedef MinMax2f Bound;

#endif

enum ELenghtType
{
	ELenghtType_None = 0,
	ELenghtType_Absolute,
	ELenghtType_Relative,
	ELenghtType_RelativeX,
	ELenghtType_RelativeY,
	ELenghtType_RelativeMinXY,
	ELenghtType_RelativeMaxXY,
	ELenghtType_RealLenght,
	ELenghtType_ErgonomicLenght,
	ELenghtType_EnumCount
};

}

#define ReflectionEnum_EnumStrings { "None", "Absolute", "Relative", "RelativeX", "RelativeY", "RelativeMinXY", "RelativeMaxXY", "RealLenght", "ErgonomicLenght" }
#define ReflectionEnum_EnumValues { GUI::ELenghtType_None, GUI::ELenghtType_Absolute, GUI::ELenghtType_Relative, GUI::ELenghtType_RelativeX, GUI::ELenghtType_RelativeY, GUI::ELenghtType_RelativeMinXY, GUI::ELenghtType_RelativeMaxXY, GUI::ELenghtType_RealLenght, GUI::ELenghtType_ErgonomicLenght }
ReflectionEnum(GUI::ELenghtType, GUI::ELenghtType_EnumCount)
#undef ReflectionEnum_EnumStrings
#undef ReflectionEnum_EnumValues

namespace GUI
{

struct LenghtTypesLenght
{
	Vector lenghts[ELenghtType_EnumCount];

	LenghtTypesLenght()
	{
		for(uint i=0; i < ELenghtType_EnumCount; ++i)
			lenghts[i].setOne();
	}

	INLINE void tmpSet(const Vector& parentSize)
	{
		lenghts[ELenghtType_Relative] = parentSize;
		lenghts[ELenghtType_RelativeX].set(parentSize.x, parentSize.x);
		lenghts[ELenghtType_RelativeY].set(parentSize.y, parentSize.y);
		Scalar m = min(parentSize.x, parentSize.y);
		Scalar M = max(parentSize.x, parentSize.y);
		lenghts[ELenghtType_RelativeMinXY].set(m, m);
		lenghts[ELenghtType_RelativeMaxXY].set(M, M);
	}
};

struct Margin
{
	rl_class(GUI::Margin)

	rl_field(Scalar, x);
	rl_field(Scalar, X);
	rl_field(Scalar, y);
	rl_field(Scalar, Y);

	rl_field(ELenghtType, xType);
	rl_field(ELenghtType, XType);
	rl_field(ELenghtType, yType);
	rl_field(ELenghtType, YType);

	INLINE Margin()
	{
		x = 0;
		X = 0;
		y = 0;
		Y = 0;
		xType = ELenghtType_Absolute;
		XType = ELenghtType_Absolute;
		yType = ELenghtType_Absolute;
		YType = ELenghtType_Absolute;
	}

	INLINE Vector low(const Vector& parentSize, LenghtTypesLenght& lenghts)const
	{
		lenghts.tmpSet(parentSize);
		Assert(xType < ELenghtType_EnumCount);
		Assert(yType < ELenghtType_EnumCount);
		return Vector(x * lenghts.lenghts[xType].x, y * lenghts.lenghts[yType].y);
	}

	INLINE Vector hi(const Vector& parentSize, LenghtTypesLenght& lenghts)const
	{
		lenghts.tmpSet(parentSize);
		Assert(XType < ELenghtType_EnumCount);
		Assert(YType < ELenghtType_EnumCount);
		return Vector(X * lenghts.lenghts[xType].x, Y * lenghts.lenghts[yType].y);
	}

	INLINE void getLowAndHi(Vector& low, Vector& hi, const Vector& parentSize, LenghtTypesLenght& lenghts)const
	{
		lenghts.tmpSet(parentSize);
		Assert(xType < ELenghtType_EnumCount);
		Assert(XType < ELenghtType_EnumCount);
		Assert(yType < ELenghtType_EnumCount);
		Assert(YType < ELenghtType_EnumCount);
		low.set(x * lenghts.lenghts[xType].x, y * lenghts.lenghts[yType].y);
		hi.set(X * lenghts.lenghts[xType].x, Y * lenghts.lenghts[yType].y);
	}

	rl_method_1(setBorderRelativeMinXY, Scalar, f)
	{
		x = X = y = Y = f;
		xType = XType = yType = YType = ELenghtType_RelativeMinXY;
	}

	rl_method_1(setBorderRelative, Scalar, f)
	{
		x = X = y = Y = f;
		xType = XType = yType = YType = ELenghtType_Relative;
	}
};

struct Dock
{
	rl_class(GUI::Dock)

	rl_field(Scalar, l0)
	rl_field(Scalar, l0Tay)
	rl_field(ELenghtType, l0Type)
	rl_field(Scalar, l1)
	rl_field(Scalar, l1Tay)
	rl_field(ELenghtType, l1Type)
	
	rl_field(Scalar, h1)
	rl_field(Scalar, h1Tay)
	rl_field(ELenghtType, h1Type)
	rl_field(Scalar, h0)
	rl_field(Scalar, h0Tay)
	rl_field(ELenghtType, h0Type)

	rl_field(Scalar, s)
	rl_field(ELenghtType, sType)

	rl_field(boolean, ruleSize)

	INLINE Dock()
	{
		l0 = 0;
		l0Tay = 0;
		l0Type = ELenghtType_None;
		l1 = 1;
		l1Tay = 1;
		l1Type = ELenghtType_None;

		h1 = 0;
		h1Tay = 1;
		h1Type = ELenghtType_None;
		h0 = 1;
		h0Tay = 0;
		h0Type = ELenghtType_None;

		s = 1;
		sType = ELenghtType_None;

		ruleSize = false;
	}

	INLINE boolean active()const { return l0Type | l1Type | h1Type | h0Type | sType; }

	rl_method_0(setFull)
	{
		l0 = 0;
		l0Tay = 0;
		l0Type = ELenghtType_Absolute;
		l1 = 1;
		l1Tay = 1;
		l1Type = ELenghtType_None;

		h1 = 0;
		h1Tay = 1;
		h1Type = ELenghtType_None;
		h0 = 1;
		h0Tay = 0;
		h0Type = ELenghtType_None;

		s = 1;
		sType = ELenghtType_Relative;
	}

	rl_method_0(setCenter)
	{
		l0 = 0.5;
		l0Tay = 0.5;
		l0Type = ELenghtType_Relative;
		l1 = 1;
		l1Tay = 1;
		l1Type = ELenghtType_None;

		h1 = 0;
		h1Tay = 1;
		h1Type = ELenghtType_None;
		h0 = 1;
		h0Tay = 0;
		h0Type = ELenghtType_None;

		s = 1;
		sType = ELenghtType_None;
	}

	rl_method_2(setRelPosSize, GUI::Scalar, pos, GUI::Scalar, size)
	{
		l0 = pos;
		l0Tay = 0;
		l0Type = ELenghtType_Relative;
		l1 = 1;
		l1Tay = 1;
		l1Type = ELenghtType_None;

		h1 = 0;
		h1Tay = 1;
		h1Type = ELenghtType_None;
		h0 = 1;
		h0Tay = 0;
		h0Type = ELenghtType_None;

		s = size;
		sType = ELenghtType_Relative;
	}

	rl_method_1(setCenterPoint, GUI::Scalar, pos)
	{
		l0 = pos;
		l0Tay = 0.5;
		l0Type = ELenghtType_Absolute;
		l1 = 1;
		l1Tay = 1;
		l1Type = ELenghtType_None;

		h1 = 0;
		h1Tay = 1;
		h1Type = ELenghtType_None;
		h0 = 1;
		h0Tay = 0;
		h0Type = ELenghtType_None;

		s = 1;
		sType = ELenghtType_None;
	}

	INLINE void cacl(Scalar& refPos, Scalar& refSize, const Vector& low, const Vector& hi, uint component, LenghtTypesLenght& lenghts)
	{
		if(!active())
			return;

		lenghts.tmpSet(hi - low);

		uint pointsCount = 0;
		Scalar point[2];
		Scalar pointTay[2];

		if(l0Type)
		{
			point[pointsCount] = low.a[component] + l0 * lenghts.lenghts[l0Type].a[component];
			pointTay[pointsCount++] = l0Tay;
		}

		if(l1Type)
		{
			point[pointsCount] = low.a[component] + l1 * lenghts.lenghts[l1Type].a[component];
			pointTay[pointsCount++] = l1Tay;
		}

		if(h1Type && pointsCount < 2)
		{
			point[pointsCount] = hi.a[component] - h1 * lenghts.lenghts[h1Type].a[component];
			pointTay[pointsCount++] = h1Tay;
		}

		if(h0Type && pointsCount < 2)
		{
			point[pointsCount] = hi.a[component] - h0 * lenghts.lenghts[h0Type].a[component];
			pointTay[pointsCount++] = h0Tay;
		}

		if(sType)
		{
			refSize = s * lenghts.lenghts[sType].a[component];
			if(pointsCount)
				refPos = point[0] - refSize * pointTay[0];
		}
		else if(pointsCount == 2)
		{
			Scalar d = pointTay[1] - pointTay[0];
			if(abs(d) >= Base::FloatConstants<Scalar>::small())
			{
				Scalar A = (point[1] - point[0]) / d;
				Scalar B = point[0] - A * pointTay[0];
				refPos = B;
				refSize = A;
			}
		}
		else if(pointsCount == 1)
		{
			if(!ruleSize)
			{
				refPos = point[0] - refSize * pointTay[0];
			}
			else
			{
				if(abs(pointTay[0]) >= Base::FloatConstants<Scalar>::small())
					refSize = (point[0] - refPos) / pointTay[0];
			}
		}
	}
};


struct CursorState
{
	Vector position;
	boolean enabled;
	boolean visible;
	boolean pushed;//debug
	WeakPtr<class Widget> captured;

	CursorState()
	{
		position.setZero();
		enabled = false;
		visible = true;
		pushed = false;
	}
};


class ScreenBase
{
public:
	Vector cursorOvermove[Input::EventBase::gMaxChannels];
	CursorState cursorState[Input::EventBase::gMaxChannels];
	LenghtTypesLenght lenghts;

	ScreenBase()
	{
		for(uint i=0; i < Input::EventBase::gMaxChannels; ++i)
			cursorOvermove[i].setZero();
	}
};


}
