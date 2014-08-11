#pragma once

#include "../screen.h"

namespace GUI
{

class DualOrientationWidget : public Widget
{
public:
	rl_class1(GUI::DualOrientationWidget, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(Vector, minsizeV);
	rl_field(Margin, marginV);
	rl_field(Dock, dockXV);
	rl_field(Dock, dockYV);
	rl_field(Scalar, aspectV)
	rl_field(int, aspectDockXDirectionV)
	rl_field(int, aspectDockYDirectionV)
	rl_field(String, imageV);
	rl_field(uint, rowV);
	rl_field(uint, columnV);
	rl_field(boolean, autoChildRectScaleXV);
	rl_field(boolean, autoChildRectScaleYV);
	rl_field(boolean, autoChildRectOffsetXV);
	rl_field(boolean, autoChildRectOffsetYV);
	rl_field(boolean, autoSizeXV);
	rl_field(boolean, autoSizeYV);
	rl_field(int, autoSizeXDirectionV);
	rl_field(int, autoSizeYDirectionV);

	rl_field(Vector, minsizeH);
	rl_field(Margin, marginH);
	rl_field(Dock, dockXH);
	rl_field(Dock, dockYH);
	rl_field(Scalar, aspectH)
	rl_field(int, aspectDockXDirectionH)
	rl_field(int, aspectDockYDirectionH)
	rl_field(String, imageH);
	rl_field(uint, rowH);
	rl_field(uint, columnH);
	rl_field(boolean, autoChildRectScaleXH);
	rl_field(boolean, autoChildRectScaleYH);
	rl_field(boolean, autoChildRectOffsetXH);
	rl_field(boolean, autoChildRectOffsetYH);
	rl_field(boolean, autoSizeXH);
	rl_field(boolean, autoSizeYH);
	rl_field(int, autoSizeXDirectionH);
	rl_field(int, autoSizeYDirectionH);

	DualOrientationWidget(Widget& inFather);
	virtual void onUpdate(Scalar dt);
	virtual void onDraw(const ParentDrawInfo& pdi, IDrawer& render)
	{
		Widget::onDraw(pdi, render);
	}
};

}
