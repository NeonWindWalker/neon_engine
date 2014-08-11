#include "slideShower.h"

namespace GUI
{

SlideShower::SlideShower(Widget& inFather) : Widget(inFather)
{
	slideTime = Scalar(0.5);
	progress = 1;
}

void SlideShower::onUpdate(Scalar dt)
{
	if(image != images[1])
	{
		if(image == images[0])
		{
			swap(images[0], images[1]);
			progress = Scalar(1.0) - progress;
		}
		else
		{
			if(progress >= Scalar(0.5))
			{
				images[0] = images[1];
				images[1] = image;
			}
			else
			{
				images[1] = image;
			}
			progress = 0;
		}
	}

	progress += dt / slideTime;
	minimize(progress, Scalar(1));
}

void SlideShower::onDraw(const ParentDrawInfo& pdi, IDrawer& render)
{
	boolean bFirst = images[0].size() && progress < Scalar(1);
	if(bFirst)
		render.drawWidget(pdi.bound, position, size, images[0], pdi.scale, pdi.offset, color);
	
	if(images[1].size())
		render.drawWidget(pdi.bound, position, size, images[1], pdi.scale, pdi.offset, bFirst ? Vector4f(color.xyz(), color.w * (float)progress) : color);
	
}

}