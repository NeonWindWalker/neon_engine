#pragma once

#include "../widget.h"

namespace GUI
{

class ColorPaletter : public Widget
{
public:
	rl_class1(GUI::ColorPaletter, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(String, selectedColorChangedMethodName);
	Event<void(ColorPaletter&)> selectedColorChangedEvent;
	 
	Vector3f selectedColor()const 
	{
		return _color;
	}

	void selectedColor(const Vector3f& c) 
	{
		if(c == _color)
			return;
		Vector3f col = fixColor(c);
		if(col == _color)
			return;
		_color = col;
		selectedColorChangedEvent(*this);
		if(!selectedColorChangedMethodName.empty()){
			IReflective* p = getModelInterface();
			if(p){
				Reflection sender = reflectionOf(*this);
				Reflection args;
				p->reflection().reflectionByPath(selectedColorChangedMethodName).call(sender, args);
			}
		}
	}

	ColorPaletter(Widget& inFather) : Widget(inFather)
	{
		_color.setOne();
		activeChannel = -1;
	}

	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& pos, const CursorState& state)
	{
		const Input::EventKey* keyev = ev.castToKeyEvent();
		if(keyev)
		{
			if(keyev->key == Input::EKeyType_MouseLeft && keyev->down && activeChannel == -1){
				activeChannel = ev.channel;
				Vector3f c;
				if(colorFromCoord(c, pos))
					selectedColor(c);
				return true;
			}
			if(keyev->key == Input::EKeyType_MouseLeft && keyev->up && activeChannel == ev.channel){
				activeChannel = -1;
				return true;
			}
			if(activeChannel == ev.channel){
				Vector3f c;
				if(colorFromCoord(c, pos))
					selectedColor(c);
				return true;
			}
		}

		const Input::EventTouch* touch = ev.castToTouchEvent();
		if(touch)
		{
			if(touch->down && activeChannel == -1){
				activeChannel = ev.channel;
				Vector3f c;
				if(colorFromCoord(c, pos))
					selectedColor(c);
				return true;
			}
			if(touch->up && activeChannel == ev.channel){
				activeChannel = -1;
				return true;
			}
			if(activeChannel == ev.channel){
				Vector3f c;
				if(colorFromCoord(c, pos))
					selectedColor(c);
				return true;
			}
		}
		return false;
	}

	virtual void onCursorLeave(const CursorState& state, uint channel)
	{
		if(activeChannel == channel)
			activeChannel = -1;
	}

	virtual void onHide()
	{
		activeChannel = -1;
		Widget::onHide();
	}

	virtual Vector3f fixColor(const Vector3f& c)
	{
		return c;
	}

	virtual boolean colorFromCoord(Vector3f& outColor, const Vector& p)
	{
		return false;
	}
	
private:
	Vector3f _color;
	int activeChannel;
};

class ColorPaletterHueSaturationHexaxedron : public ColorPaletter
{
public:
	rl_class1(GUI::ColorPaletterHueSaturationHexaxedron, GUI::Widget)
	rl_base_class0(GUI::ColorPaletter)
	rl_reflective

	uint radialCount()const { return _radialCount; }
	void radialCount(uint n) { if(_radialCount == n)return; _radialCount = n; rebuildTable(); }

	rl_val_property(uint, radialCount)

	boolean hiSaturation()const { return _hiSaturation; }
	void hiSaturation(boolean b) { b = boolean01FromBoolean(b); if(_hiSaturation == b)return; _hiSaturation = b; rebuildTable(); }

	rl_val_property(boolean, hiSaturation)

	ColorPaletterHueSaturationHexaxedron(Widget& father) : ColorPaletter(father)
	{
		_radialCount = 0;
		_hiSaturation = false;
	}

	virtual void onDraw(const ParentDrawInfo& pdi, IDrawer& render);
	virtual Vector3f fixColor(const Vector3f& c);
	virtual boolean colorFromCoord(Vector3f& outColor, const Vector& p);

protected:
	uint _radialCount;
	boolean _hiSaturation;
	void rebuildTable();

	struct Element
	{
		Vector3f color;
		Vector center;
	};
	Array<Element> palette;
	Vector elementSize;
};

}
