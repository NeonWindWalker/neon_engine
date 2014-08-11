#include "modificators.h"
#include "widgets/text.h"

namespace GUI
{

void TimeLine::update(Widget& target, Scalar dt)
{
	TimeLineTime_t idt = timeLineTimeFromScalar(dt * timeScale);
	while(idt)
	{
		TimeLineTime_t newPos = position + idt;
		if(newPos > lenght)
		{
			if(position >= lenght)
			{
				if(!looped)
					break;

				position = 0;
				continue;
			}
			
			idt = newPos - lenght;
			newPos = lenght;
		}
		else idt = 0;

		updateIntervals(target, newPos, position);
		position = newPos;
	}
}

void TimeLine::updateIntervals(Widget& target, TimeLineTime_t from, TimeLineTime_t to)
{
	foreach(it, intervals)
	{
		TimeInterval* p = *it;
		if(p)
		{
			uint end = p->position + p->lenght;
			if(from < p->position && to >= p->position)
				p->start(target);
			if(p->position <= from && end > to)
				p->update(target, to - p->position);
			if(from < end && to >= end)
				p->end(target);
		}			
	}
}

void PositionModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	if(pEndTarget && table.size())
		pEndTarget->position = getLinearTableValue<VectorAndTime>(cashId, table, elapsedFromStartTime);
}

void SizeModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	if(pEndTarget && table.size())
		pEndTarget->size = getLinearTableValue(cashId, table, elapsedFromStartTime);
}

void ImageNameModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	if(pEndTarget)
		pEndTarget->image = imageName;
}

void VisibleModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	if(pEndTarget)
		pEndTarget->visible(visible);
}

void TextModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	GUI::Text* p = ReflectionLib::dynamicCast<GUI::Text>(pEndTarget);
	if(p)
		p->text = text;
}

void TextLocalizationModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	GUI::Text* p = ReflectionLib::dynamicCast<GUI::Text>(pEndTarget);
	if(p)
		p->localizationEntry = localization;
}

void ChildRectScaleModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	if(pEndTarget && table.size())
		pEndTarget->childRectScale = getLinearTableValue<VectorAndTime>(cashId, table, elapsedFromStartTime);
}

void ChildRectOffsetModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	if(pEndTarget && table.size())
		pEndTarget->childRectOffset = getLinearTableValue(cashId, table, elapsedFromStartTime);
}

void ChildRectScaleWeightPoweredModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	if(pEndTarget && table.size())
		pEndTarget->childRectScale = getLinearTableValue(cashId, table, elapsedFromStartTime);
}

void ChildRectOffsetWeightPoweredModificatorTimeInterval::update(class Widget& target, TimeLineTime_t elapsedFromStartTime)
{
	Widget* pEndTarget = findTarget(target);
	if(pEndTarget && table.size())
		pEndTarget->childRectOffset = getLinearTableValue(cashId, table, elapsedFromStartTime);
}

}