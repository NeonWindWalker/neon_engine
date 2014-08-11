#include "screen.h"

namespace GUI
{

Widget::Widget(Widget& inFather) : parent(&inFather), childs( makeDelegate(this, &Widget::constructChild) )
{
	if(parent)
		screen = parent->screen;
	else
		screen = 0;
	modelInterface = 0;
	position.setZero();
	size.setZero();
	minsize.setZero();
	_visible = true;
	_visibleFromMask = true;
	order = 0;
	row = column = 0;
	childClip = true;
	color.setOne();
	variationsMask = 0xffffffff;

	aspect = 0;
	aspectDockXDirection = 0;
	aspectDockYDirection = 0;

	autoSizeX = autoSizeY = false;
	autoSizeXDirection = 0;
	autoSizeYDirection = 0;
	autoChildRectScaleX = false;
	autoChildRectScaleY = false;
	autoChildRectOffsetX = false;
	autoChildRectOffsetY = false;
	childRectScale.setOne();
	childRectOffset.setZero();
	breakCursorBubbling = false;
	updateInvisible = true;
	exclusiveChildsCursorCollision = false;
}

Widget::~Widget()
{
	foreach(ch, childs)
		(*ch)->parent = 0;
	childs.clear();
}

void* Widget::constructChild(Widget*& baseptr, const Type& type, IAllocator& alc)
{
	Reflection r = type.construct(alc, *this);
	Reflection wr = r.cast(typeOf<Widget>());
	baseptr = (Widget*)wr.objectPtr();
	if(baseptr){
		Library* lib = getLibrary();
		if(lib)
			lib->buildWidget(r);
		return r.objectPtr();
	}
	logError << "cant create widget of type:" << type.name();
	r.destruct();
	baseptr = 0;
	return 0;
}

void Widget::focus()
{
	screen->focus(0, this);
}

boolean Widget::onFocusInputBubbling(const Input::EventBase& ev)
{
	Assert(ev.channel < Input::EventBase::gMaxChannels);
	const Input::EventKey* keyev = ev.castToKeyEvent();
	if(keyev)
	{
		if(!keyev->down)
			return false;
		if(keyev->key == Input::EKeyType_Left && parent){
			screen->focus(ev.channel, parent);
			return true;
		}
		if(keyev->key == Input::EKeyType_Right && !childs.empty()){
			screen->focus(ev.channel, childs[0]);
			return true;
		}
		if(keyev->key == Input::EKeyType_Down || keyev->key == Input::EKeyType_Up){
			uint n = findChildIndex(screen->lastFocusedWidget(ev.channel));
			if(n >= childs.size())
				return false;
			if(keyev->key == Input::EKeyType_Down)
				n = max(n + 1, childs.size() - 1);
			else
				if(n)n--;
			screen->focus(ev.channel, childs[n]);
			return true;
		}
	}
	return false;
}

boolean Widget::guisysTraverseCursor(Array<TrawersData>& list, const Vector& pos)
{
	if(!visible() || !containEx(pos))
		return false;
	
	TrawersData& d = *list.push();
	d.ptr = this;
	d.bubbling = false;
	d.locpos = pos - position;

	Vector cp = toChildCoords(pos);
	for(Childs_t::Iterator ch = childs.last(); ch != childs.begin(); --ch)
		if((*ch) && (*ch)->guisysTraverseCursor(list, cp) && exclusiveChildsCursorCollision)
			break;
	
	TrawersData& d2 = *list.push();
	d2.ptr = this;
	d2.bubbling = true;
	d2.locpos = pos - position;
	return true;
}

void Widget::guisysTraverseUpdate(Array< WeakPtr<Widget> >& list)
{
	boolean vis = visible();
	if(!updateInvisible && !vis)
		return;

	list.push(WeakPtr<Widget>(this));

	if(vis)
		for(Childs_t::Iterator ch = childs.first(); ch != childs.end(); ++ch)
			if(*ch)
				(*ch)->guisysTraverseUpdate(list);
}

void Widget::guisysTraverseUpdateLayout()
{
	if(!visible())
		return;
	Vector oldPos = position;
	Vector oldSize = size;
	onUpdateLayout();
	for(uint i=0; i < childs.size(); ++i)
		if(childs[i])
			childs[i]->guisysTraverseUpdateLayout();
	if(screen && ((oldPos - position).lenghtSquare() > 1.0f || (oldSize - size).lenghtSquare() > 1.0f))
		screen->updateLayoutChanges = true;
}

void Widget::onDraw(const ParentDrawInfo& pdi, IDrawer& render)
{
	render.drawWidget(pdi.bound, position, size, image, pdi.scale, pdi.offset, color);
}

void Widget::guisysTraverseDraw(const ParentDrawInfo& pdi, IDrawer& render)
{
	if(!visible())
		return;

	onDraw(pdi, render);

	Array< Pair<int, Widget*> > list;
	list.reserve(childs.size());
	for(uint i=0; i < childs.size(); ++i)
		if(childs[i])
			list.fpush(Pair<int, Widget*>(childs[i]->order * 1024 + i, childs[i]));
	list.sort();

	ParentDrawInfo di;
	di.offset = pdi.offset + (position + childRectOffset) * pdi.scale;
	di.scale = childRectScale * pdi.scale;
	Bound chb = childBound();
	chb.min = chb.min * pdi.scale + pdi.offset;
	chb.max = chb.max * pdi.scale + pdi.offset;

	if(childClip)
	{
		if( !chb.intersection(di.bound, pdi.bound)){
			onPostDraw(pdi, render);
			return;
		}
	}else
		di.bound = pdi.bound;
	
	//di.bound.set(di.offset, pdi.offset + (position + size - childRectMargin.hi()) * pdi.scale);

	for(uint i=0; i < list.size(); ++i)
		list[i].val->guisysTraverseDraw(di, render);

	onPostDraw(pdi, render);
}

void Widget::visible(boolean val)
{ 
	val = boolean01FromBoolean(val);
	boolean oldVis = visible();
	if(val == _visible)
		return;
	_visible = val;
	//onVisibleChanged();
	//visibleChangedEvent(*this);
	if(!_visible && oldVis != visible())
	{
		/*
		Widget* par = parent;
		while(par)
		{
			if(!par->visible())
				return;
			par = par->parent;
		}*/
		WidgetTrawerseArray hideList;
		guisysHideTraverse(hideList);
		foreach(it, hideList)
		{
			StrongPtr<Widget> p = (*it).lock();
			if(p)
			{
				p->onHide();
				p->hideEvent(*p);
			}
		}
	}
}

void Widget::guisysHideTraverse(WidgetTrawerseArray& list)
{
	if(!visible())
		return;
	foreach(it, childs)
		(*it)->guisysHideTraverse(list);
}

void Widget::onHide()
{
	screen->unfocus(this);
}

void Widget::updateLayout_FromParent(const Bound& bound)
{
	if(dockX.active() || dockY.active())
	{
		Vector s = bound.size();
		Vector low, hi;
		margin.getLowAndHi(low, hi, s, screen->lenghts);

		low = bound.min + low;
		hi = bound.max - hi;
		dockX.cacl(position.x, size.x, low, hi, 0, screen->lenghts);
		dockY.cacl(position.y, size.y, low, hi, 1, screen->lenghts);
	}

	if(aspect)
	{
		Scalar targetX = size.y * aspect;
		Scalar targetY = size.x / aspect;
		if(targetX > size.x)
		{
			if(aspectDockYDirection == 0)
				position.y += (size.y - targetY) / Scalar(2);
			else if(aspectDockYDirection > 0)
				position.y += size.y - targetY;

			size.y = targetY;
		}
		else if(targetY > size.y)
		{
			if(aspectDockXDirection == 0)
				position.x += (size.x - targetX) / Scalar(2);
			else if(aspectDockXDirection > 0)
				position.x += size.x - targetX;

			size.x = targetX;
		}
	}

	maximize(size, minsize);
}

void Widget::updateLayout_FromChilds()
{
	if(!childs.empty() && (autoSizeX | autoSizeY | autoChildRectScaleX | autoChildRectScaleY | autoChildRectOffsetX | autoChildRectOffsetY))
	{
		Bound bound = childs[0]->marginBound();
		for(uint i=1; i < childs.size(); ++i)
			if(childs[i])
				bound.include(childs[i]->marginBound());

		if(autoSizeX)
		{
			Scalar old = size.x;
			size.x = bound.max.x * childRectScale.x + childRectOffset.x;
			if(autoSizeXDirection < 0)
				position.x -= size.x - old;
			else if(autoSizeXDirection == 0)
				position.x -= (size.x - old) / Scalar(2);
		}

		if(autoSizeY)
		{
			Scalar old = size.y;
			size.y = bound.max.y * childRectScale.y + childRectOffset.y;
			if(autoSizeYDirection < 0)
				position.y -= size.y - old;
			else if(autoSizeYDirection == 0)
				position.y -= (size.y - old) / Scalar(2);
		}

		if(autoChildRectScaleX && bound.width() > 0)
			childRectScale.x = size.x / bound.width();

		if(autoChildRectScaleY && bound.height() > 0)
			childRectScale.y = size.y / bound.height();

		if(autoChildRectOffsetX)
			childRectOffset.x = - bound.min.x * childRectScale.x;

		if(autoChildRectOffsetY)
			childRectOffset.y = - bound.min.y * childRectScale.y;
	}

	maximize(size, minsize);
}

void Widget::onUpdate(Scalar dt)
{
	boolean visMask = (screen->variationsMask & variationsMask) != 0;
	if(_visibleFromMask != visMask)
	{
		_visibleFromMask = visMask;
		if(!_visibleFromMask)
		{
			WidgetTrawerseArray hideList;
			guisysHideTraverse(hideList);
			foreach(it, hideList)
			{
				StrongPtr<Widget> p = (*it).lock();
				if(p)
				{
					p->onHide();
					p->hideEvent(*p);
				}
			}
		}
		return;
	}
	
	if(visible())
		foreach(it, modificators)
			if(*it)
				(*it)->update(*this, dt);
}

void Widget::onUpdateLayout()
{
	Bound bound = childLocalBound();

	for(StrongPtr<Widget>* ch = childs.first(); ch != childs.end(); ++ch)
		if(*ch)
			(*ch)->updateLayout_FromParent(bound);

	updateLayout_FromChilds();
}

Bound Widget::marginBound()const
{
	Assert(parent);
	Vector l, h;
	margin.getLowAndHi(l, h, parent->size, screen->lenghts);
	return Bound(position - l, position + size + h);
}

Bound Widget::childBound()const
{
	return Bound(position, position + size);
}

Bound Widget::childLocalBound()const
{
	return Bound(- childRectOffset / childRectScale, (size - childRectOffset) / childRectScale);
}

Widget* Widget::findInScreenWidget()
{
	Widget* w = this;
	do
	{
		if(w->parent == screen)
			return w;
		w = w->parent;
	}
	while(w);
	return 0;
}

Widget* Widget::findParentByName(ConstString name)
{
	Widget* w = parent;
	while(w)
	{
		if(w->name == name)
			return w;
		w = w->parent;
	}
	return 0;
}


Vector Widget::fromScreenPosToLocalPos(const Vector& scrpos)const
{
	Array<const Widget*, SystemAllocatorWrapper, 32> list;
	if(parent)
	{
		const Widget* p = parent;
		while(p->parent)
		{
			list.push(p);
			p = p->parent;
		}
	}

	Vector pos = scrpos;
	rforeach(it, list)
	{
		const Widget* w = *it;
		pos = w->toChildCoords(pos);
	}

	return pos - position;
}

}