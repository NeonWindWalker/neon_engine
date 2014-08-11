#pragma once

#include "../screen.h"

namespace GUI
{

class Toggle : public Widget
{
public:
	rl_class1(GUI::Toggle, GUI::Widget)
	rl_base_class0(GUI::Widget)
	rl_reflective

	rl_field(String, toggledChangedMethodName);
	Event<void(Toggle&, boolean)> toggledChangedEvent;

	Toggle(Widget& inFather);

	virtual void onDraw(const ParentDrawInfo& pdi, IDrawer& render);
	virtual void onPostDraw(const ParentDrawInfo& pdi, IDrawer& render);
	virtual void onHide() { untoggle(); Widget::onHide(); }

	INLINE boolean toggled()const { return _toggledChannel >= 0; }
	void toggled(uint channel, boolean val);
	void untoggle();
	INLINE void setToggledWithoutEvents(uint channel) { _toggledChannel = channel; }
	INLINE void setUnToggledWithoutEvents() { _toggledChannel = -1; }

protected:
	int _toggledChannel;
};

}
