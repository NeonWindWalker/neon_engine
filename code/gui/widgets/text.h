#pragma once

#include "../widget.h"
#include "../screen.h"

namespace GUI
{

class Text : public Widget
{
public:
	rl_class1(GUI::Text, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(String, text);
	rl_field(Color, color);
	rl_field(String, font);
	rl_field(Scalar, fontScale);
	rl_field(boolean, wrapping);
	rl_field(boolean, autoSizeX);
	rl_field(boolean, autoSizeY);
	rl_field(boolean, autoFontScaleByX);
	rl_field(boolean, autoFontScaleByY);
	rl_field(boolean, autoFontScaleByYConsiderateNewLines);
	rl_field(Scalar, maxSizeX);
	rl_field(ELenghtType, maxSizeXType);
	rl_field(Scalar, maxSizeY);
	rl_field(ELenghtType, maxSizeYType);
	rl_field(String, localizationEntry);
	rl_field(Vector, shadowShift);
	rl_field(Vector4f, shadowColor);
	
	Text(Widget& inFather) : Widget(inFather)
	{
		color.setOne();
		fontScale = 1;
		wrapping = false;
		autoSizeX = false;
		autoSizeY = false;
		autoFontScaleByX = false;
		autoFontScaleByY = false;
		autoFontScaleByYConsiderateNewLines = false;
		maxSizeX = 1;
		maxSizeXType = ELenghtType_None;
		maxSizeY = 1;
		maxSizeYType = ELenghtType_None;
		shadowShift.set(0.0416f, -0.0416f);
		shadowColor.set(0,0,0,0.625f);
	}

	virtual void onUpdate(Scalar dt);
	virtual void onDraw(const ParentDrawInfo& pdi, IDrawer& render);
	virtual void updateLayout_FromParent(const Bound& parentBound);

	rl_method_0(setBestFitCentred)
	{
		dockX.setCenter();
		dockY.setCenter();
		wrapping = false;
		autoSizeX = true;
		autoSizeY = true;
		autoFontScaleByX = autoFontScaleByY = true;
		autoFontScaleByYConsiderateNewLines = true;
		maxSizeX = 1;
		maxSizeXType = ELenghtType_Relative;
		maxSizeY = 1;
		maxSizeYType = ELenghtType_Relative;
	}

	rl_method_0(setCentred)
	{
		dockX.setCenter();
		dockY.setCenter();
		wrapping = false;
		autoSizeX = true;
		autoSizeY = true;
		autoFontScaleByX = autoFontScaleByY = false;
		autoFontScaleByYConsiderateNewLines = false;
		maxSizeX = 1;
		maxSizeXType = ELenghtType_None;
		maxSizeY = 1;
		maxSizeYType = ELenghtType_None;
	}

	
};

}
