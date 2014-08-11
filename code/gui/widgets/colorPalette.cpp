#include "colorPalette.h"

namespace GUI
{

void ColorPaletterHueSaturationHexaxedron::rebuildTable()
{
	if(!_radialCount)
		return;

	Vector3f oldSelectedColor = selectedColor();
	palette.clear();
	
	Scalar bigRadius = Scalar(1.0);
	elementSize.y = bigRadius / _radialCount;
	elementSize.x = Scalar(1.1547005383792515290182975610039) * elementSize.y;
	Vector elemStep(elementSize.x * Scalar(0.75), elementSize.y);

	Vector hexCenter(Scalar(0.86602540378443864676372317075294), Scalar(1.0));

	Scalar startX = elemStep.x;
	Scalar startY = elemStep.y * Scalar(0.5);
	for(uint i=0; i < _radialCount * 2 - 1; ++i)
	{
		Vector center;
		center.x = Scalar(i) * elemStep.x + startX;
		uint js = i < _radialCount ? _radialCount + i : _radialCount * 3 - 2 - i;
		for(uint j=0; j < js; ++j)
		{
			uint r;
			if(i < _radialCount)
			{
				center.y = Scalar(_radialCount - i) * elemStep.y * Scalar(0.5);
				r = _radialCount - 1 + maximum<int>(maximum<int>(-(int)i, -(int)j), (int)j - (int)(js - 1));
			}
			else
			{
				center.y = Scalar(2 + i - _radialCount) * elemStep.y * Scalar(0.5);
				r = maximum<int>(maximum<int>(i - _radialCount + 1, _radialCount - 1 - j), _radialCount - 1 + (int)j - (int)(js - 1));
			}
			center.y += Scalar(j) * elemStep.y + startY;

			Vector3f c = Vector3f::getOne();
			if(r)
			{
				Vector p = center - hexCenter;
				Scalar a = atan2(p.x, p.y) - gcfTwoPi / Scalar(6.0);
				if(a < 0)
					a += gcfTwoPi;
				if(a < 0)
					a += gcfTwoPi;
				a *= 3.0f / gcfTwoPi;

				if(a < 1.0f)
					c.set(1.0f - a, a, 0.0f);
				else if(a < 2.0f)
					c.set(0.0f, 2.0f - a, a - 1.0f);
				else
					c.set(a - 2.0f, 0.0f, 3.0f - a);

				c /= max(c.x, max(c.y, c.z));
				//c = (c + c / max(c.x, c.y, c.z)) * 0.5f;

				Scalar sat = Scalar(r) / (_radialCount - 1);
				if(_hiSaturation)
					sat = sqrt(sat);
				c = Vector3f(1.0f - sat, 1.0f - sat, 1.0f - sat) + c * sat;
			}

			//c.selfNormalize();
			//c /= (c.x + c.y + c.z); 

			Element& e = *palette.push();
			e.color = c;
			e.center = center;
		}
	}

	selectedColor(oldSelectedColor);
}

void ColorPaletterHueSaturationHexaxedron::onDraw(const ParentDrawInfo& pdi, IDrawer& render)
{
	ColorPaletter::onDraw(pdi, render);
	if(!_radialCount)
		return;

	Scalar bigRadius = min(size.x * Scalar(1.1547005383792515290182975610039), size.y) * Scalar(0.5);
	Vector elemSize = elementSize * bigRadius;
	Vector elemOffset = - elemSize * Scalar(0.5) + size * Scalar(0.5) - bigRadius * Vector(Scalar(0.86602540378443864676372317075294), Scalar(1.0));
	Element* selectedElement = 0;
	foreach(e, palette)
	{
		if(e->color == selectedColor())
			selectedElement = e;
		render.drawWidget(pdi.bound, position + e->center * bigRadius + elemOffset, elemSize, _CS("hexahedron"), pdi.scale, pdi.offset, Vector4f(e->color, 1.0f));
	}
	if(selectedElement)
		render.drawWidget(pdi.bound, position + selectedElement->center * bigRadius + elemOffset, elemSize, _CS("hexahedron_selected"), pdi.scale, pdi.offset);
}

Vector3f ColorPaletterHueSaturationHexaxedron::fixColor(const Vector3f& c)
{
	Scalar d = Scalar(1e32);
	Element* bestElement = &palette[0];
	foreach(e, palette)
	{
		float f = c.distanceSquareTo(e->color);
		if(f < d){
			bestElement = e;
			d = f;
		}
	}
	return bestElement->color;
}

boolean ColorPaletterHueSaturationHexaxedron::colorFromCoord(Vector3f& outColor, const Vector& p)
{
	Scalar bigRadius = min(size.x * Scalar(1.1547005383792515290182975610039), size.y) * Scalar(0.5);
	Vector bigOffset = size * Scalar(0.5) - bigRadius * Vector(Scalar(0.86602540378443864676372317075294), Scalar(1.0));
	Vector locpos = (p - bigOffset) / bigRadius;

	Scalar d = Scalar(1e32);
	Element* bestElement = 0;
	foreach(e, palette)
	{
		Scalar f = locpos.distanceSquareTo(e->center);
		if(f < d){
			bestElement = e;
			d = f;
		}
	}

	if(bestElement && d < elementSize.x * elementSize.x * Scalar(0.25)){
		outColor = bestElement->color;
		return true;
	}
	
	return false;
}

}
