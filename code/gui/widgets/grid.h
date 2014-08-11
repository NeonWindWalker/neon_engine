#pragma once

#include "../widget.h"

namespace GUI
{

enum EGridSizeType
{
	EGridSizeType_Pixel = 1,
	EGridSizeType_Percent,
	EGridSizeType_Auto
};

}

#define ReflectionEnum_EnumStrings {"Pixel", "Percent", "Auto"}
#define ReflectionEnum_EnumValues  {GUI::EGridSizeType_Pixel, GUI::EGridSizeType_Percent, GUI::EGridSizeType_Auto}
ReflectionEnum(GUI::EGridSizeType, 3)
#undef ReflectionEnum_EnumStrings
#undef ReflectionEnum_EnumValues


namespace GUI
{

class GridSize
{
public:
	rl_class(GUI::GridSize)

	rl_field(Scalar, size);
	rl_field(EGridSizeType, type);
	Scalar realSize;
	Scalar realPosition;

	GridSize()
	{
		size = 0;
		type = EGridSizeType_Pixel;
		realSize = 0;
		realPosition = 0;
	}
};


class Grid : public Widget
{
public:
	rl_class1(GUI::Grid, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(Array<GridSize>, rows);
	rl_field(Array<GridSize>, columns);

	Grid(Widget& inFather) : Widget(inFather) {}

	virtual void onUpdateLayout();
};

}
