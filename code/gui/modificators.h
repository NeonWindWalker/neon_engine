#pragma once

#include "widget.h"

namespace GUI
{
typedef uint TimeLineTime_t;

static INLINE TimeLineTime_t timeLineTimeFromScalar(Scalar f){ return TimeLineTime_t(f * Scalar(1000000) + Scalar(0.5)); }
static INLINE Scalar scalarFromTimeLineTime(TimeLineTime_t t){ return Scalar(t) * 0.000001f; }

class TimeInterval : public ReflectionLib::Object
{
public:
	rl_class(GUI::TimeInterval)
	rl_base_class0(ReflectionLib::Object)
	rl_reflective

	rl_field(TimeLineTime_t, position)
	rl_field(TimeLineTime_t, lenght)
	rl_field(String, targetName);

	virtual void start(Widget& target){}
	virtual void update(Widget& target, TimeLineTime_t elapsedFromStartTime){}
	virtual void end(Widget& target){}

	TimeInterval()
	{
		position = 0;
		lenght = 0;
	}

	Widget* findTarget(Widget& target)const
	{
		if(targetName.empty())
			return &target;
		return target.findWidget(targetName);
	}
};

class TimeLine : public Modificator
{
public:
	rl_class(GUI::TimeLine)
	rl_base_class0(GUI::Modificator)
	rl_reflective

	rl_field(TimeLineTime_t, lenght)
	rl_field(TimeLineTime_t, position)
	rl_field(Scalar, timeScale);
	rl_field(boolean, looped)

	typedef ArrayOfConstructables< TimeInterval, StrongPtr<TimeInterval> > Intervals_t;
	rl_field(Intervals_t, intervals)
	
	virtual void update(class Widget& owner, Scalar dt);
	void updateIntervals(Widget& target, TimeLineTime_t from, TimeLineTime_t to);

	TimeLine()
	{
		position = 0;
		lenght = 0;
		looped = false;
		timeScale = 1;
	}
	
};

class VectorAndTime
{
public:
	rl_class(GUI::VectorAndTime)

	rl_field(TimeLineTime_t, time);
	rl_field(Vector, val);
	
	VectorAndTime()
	{
		time = 0;
		val.setZero();
	}

	typedef Vector Value_t;
	typedef TimeLineTime_t Time_t;

	Time_t getTime()const { return time; }
	Value_t getValue()const { return val; }
	Scalar timeDivision(uint a, uint b)const { return Scalar(a) / Scalar(b); } 
};

class VectorAndTimeWeightPowered
{
public:
	rl_class(GUI::VectorAndTimeWeightPowered)

	rl_field(TimeLineTime_t, time);
	rl_field(Vector, val);
	rl_field(Scalar, pow);

	VectorAndTimeWeightPowered()
	{
		time = 0;
		pow = 1;
		val.setZero();
	}

	typedef Vector Value_t;
	typedef TimeLineTime_t Time_t;

	Time_t getTime()const { return time; }
	Value_t getValue()const { return val; }
	Scalar timeDivision(uint a, uint b)const { return Base::pow(Scalar(a) / Scalar(b), pow); } 
};


class PositionModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::PositionModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(Array<VectorAndTime>, table);
	uint cashId;

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);

	PositionModificatorTimeInterval()
	{
		cashId = 0;
	}
};

class SizeModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::SizeModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(Array<VectorAndTime>, table);
	uint cashId;

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);

	SizeModificatorTimeInterval()
	{
		cashId = 0;
	}
};


class ImageNameModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::ImageNameModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(String, imageName);

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);
};

class VisibleModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::VisibleModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(boolean, visible);

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);
};

class TextModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::TextModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(String, text);

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);
};

class TextLocalizationModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::TextLocalizationModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(String, localization);

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);
};

class ChildRectScaleModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::ChildRectScaleModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(Array<VectorAndTime>, table);
	uint cashId;

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);

	ChildRectScaleModificatorTimeInterval()
	{
		cashId = 0;
	}
};

class ChildRectOffsetModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::ChildRectOffsetModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(Array<VectorAndTime>, table);
	uint cashId;

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);

	ChildRectOffsetModificatorTimeInterval()
	{
		cashId = 0;
	}
};

class ChildRectScaleWeightPoweredModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::ChildRectScaleWeightPoweredModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(Array<VectorAndTimeWeightPowered>, table);
	uint cashId;

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);

	ChildRectScaleWeightPoweredModificatorTimeInterval()
	{
		cashId = 0;
	}
};

class ChildRectOffsetWeightPoweredModificatorTimeInterval : public TimeInterval
{
public:
	rl_class(GUI::ChildRectOffsetWeightPoweredModificatorTimeInterval)
	rl_base_class0(GUI::TimeInterval)
	rl_reflective

	rl_field(Array<VectorAndTimeWeightPowered>, table);
	uint cashId;

	virtual void update(class Widget& target, TimeLineTime_t elapsedFromStartTime);

	ChildRectOffsetWeightPoweredModificatorTimeInterval()
	{
		cashId = 0;
	}
};


class CircleAlternativeGeometry : public AlternativeGeometry
{
public:
	rl_class(GUI::CircleAlternativeGeometry)
	rl_base_class0(GUI::AlternativeGeometry)
	rl_reflective

	rl_field(boolean, xy)
	rl_field(boolean, Xy)
	rl_field(boolean, xY)
	rl_field(boolean, XY)

	CircleAlternativeGeometry()
	{
		xy = Xy = xY = XY = true;
	}

	virtual boolean contain(const Widget& owner, const Vector& locPoint)
	{
		Vector l = locPoint - Vector(Scalar(0.5), Scalar(0.5));
		if(l.y >= 0)
		{
			if(l.x >= 0)
			{
				if(!XY)
					return true;
			}
			else
			{
				if(!xY)
					return true;
			}
		}
		else
		{
			if(l.x >= 0)
			{
				if(!Xy)
					return true;
			}
			else
			{
				if(!xy)
					return true;
			}
		}
		return l.lenghtSquare() <= Scalar(0.25);
	}
};

class SegmentPointerAlternativeGeometry : public AlternativeGeometry
{
public:
	rl_class(GUI::SegmentPointerAlternativeGeometry)
	rl_base_class0(GUI::AlternativeGeometry)
	rl_reflective

	rl_field(boolean, yUp)
	rl_field(Scalar, radius)
	rl_field(Scalar, centerHeight)

	SegmentPointerAlternativeGeometry()
	{
		yUp = true;
		radius = Scalar(0.5);
		centerHeight = Scalar(0.5);
	}

	virtual boolean contain(const Widget& owner, const Vector& locPoint)
	{
		Vector l = locPoint;
		if(!yUp)
			l.y = Scalar(1.0) - l.y;

		if(l.distanceSquareTo(Vector(Scalar(0.5), centerHeight)) <= radius)
			return true;

		if(l.y <= centerHeight && l.y >= Scalar(0) && abs((l.x - Scalar(0.5)) * centerHeight) < abs(l.y * radius))
			return true;

		return false;
	}
};

}