#include "dualOrientationWidget.h"

namespace GUI
{

DualOrientationWidget::DualOrientationWidget(Widget& inFather) : Widget(inFather)
{
	minsizeV.setZero();
	rowV = 0;
	columnV = 0;
	autoChildRectScaleXV = false;
	autoChildRectScaleYV = false;
	autoChildRectOffsetXV = false;
	autoChildRectOffsetYV = false;
	autoSizeXV = false;
	autoSizeYV = false;
	autoSizeXDirectionV = 0;
	autoSizeYDirectionV = 0;
		
	aspectV = 0;
	aspectDockXDirectionV = 0;
	aspectDockYDirectionV = 0;

	minsizeH.setZero();
	rowH = 0;
	columnH = 0;
	autoChildRectScaleXH = false;
	autoChildRectScaleYH = false;
	autoChildRectOffsetXH = false;
	autoChildRectOffsetYH = false;
	autoSizeXH = false;
	autoSizeYH = false;
	autoSizeXDirectionH = 0;
	autoSizeYDirectionH = 0;

	aspectH = 0;
	aspectDockXDirectionH = 0;
	aspectDockYDirectionH = 0;
}

void DualOrientationWidget::onUpdate(Scalar dt)
{
	if(screen->size.x > screen->size.y)
	{
		minsize = minsizeH;
		margin = marginH;
		dockX = dockXH;
		dockY = dockYH;
		image = imageH;
		row = rowH;
		column = columnH;
		autoChildRectScaleX = autoChildRectScaleXH;
		autoChildRectScaleY = autoChildRectScaleYH;
		autoChildRectOffsetX = autoChildRectOffsetXH;
		autoChildRectOffsetY = autoChildRectOffsetYH;
		autoSizeX = autoSizeXH;
		autoSizeY = autoSizeYH;
		autoSizeXDirection = autoSizeXDirectionH;
		autoSizeYDirection = autoSizeYDirectionH;
		aspect = aspectH;
		aspectDockXDirection = aspectDockXDirectionH;
		aspectDockYDirection = aspectDockYDirectionH;
	}
	else
	{
		minsize = minsizeV;
		margin = marginV;
		dockX = dockXV;
		dockY = dockYV;
		image = imageV;
		row = rowV;
		column = columnV;
		autoChildRectScaleX = autoChildRectScaleXV;
		autoChildRectScaleY = autoChildRectScaleYV;
		autoChildRectOffsetX = autoChildRectOffsetXV;
		autoChildRectOffsetY = autoChildRectOffsetYV;
		autoSizeX = autoSizeXV;
		autoSizeY = autoSizeYV;
		autoSizeXDirection = autoSizeXDirectionV;
		autoSizeYDirection = autoSizeYDirectionV;
		aspect = aspectV;
		aspectDockXDirection = aspectDockXDirectionV;
		aspectDockYDirection = aspectDockYDirectionV;
	}

	Widget::onUpdate(dt);
}

}