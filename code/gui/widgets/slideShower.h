#pragma once

#include "../widget.h"

namespace GUI
{

class SlideShower : public Widget
{
public:
	rl_class1(GUI::SlideShower, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(Scalar, slideTime);

	SlideShower(Widget& inFather);

	virtual void onUpdate(Scalar dt);
	virtual void onDraw(const ParentDrawInfo& pdi, IDrawer& render);

protected:
	String images[2];
	Scalar progress;
};

}
