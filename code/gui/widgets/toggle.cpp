#include "toggle.h"

namespace GUI
{

Toggle::Toggle(Widget& inFather) : Widget(inFather)
{
	image = _CS("toggle");
	_toggledChannel = -1;
}

void Toggle::onDraw(const ParentDrawInfo& pdi, IDrawer& render)
{
	String img = image;
	if(toggled())
		img <<  _CS("_toggled");
	render.drawWidget(pdi.bound, position, size, img, pdi.scale, pdi.offset);
}

void Toggle::onPostDraw(const ParentDrawInfo& pdi, IDrawer& render)
{

}

void Toggle::toggled(uint channel, boolean val)
{
	if(toggled())
	{
		if(val)
			return;
		if(channel != _toggledChannel)
			return;
		_toggledChannel = -1;
	}
	else
	{
		if(!val)
			return;
		_toggledChannel = channel;
	}

	toggledChangedEvent(*this, toggled());
	if(!toggledChangedMethodName.empty()){
		IReflective* p = getModelInterface();
		if(p){
			Reflection sender = reflectionOf(*this);
			boolean b = toggled();
			Reflection args = reflectionOf(b);
			p->reflection().reflectionByPath(toggledChangedMethodName).call(sender, args);
		}
	}
}

void Toggle::untoggle()
{
	for(uint ch = 0; ch < Input::EventBase::gMaxChannels; ch++)
		toggled(ch, false);
}

}