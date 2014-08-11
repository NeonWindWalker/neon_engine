#pragma once

#include "seed.h"
#include "drawer.h"
#include "library.h"

namespace GUI
{

class Screen;
struct CursorState;

struct TrawersData
{
	WeakPtr<class Widget> ptr;
	boolean bubbling;
	Vector locpos;
};

class Widget;

class Modificator : public ReflectionLib::Object
{
public:
	rl_abstract_class(GUI::Modificator)
	rl_base_class0(ReflectionLib::Object)
	rl_reflective

	virtual void update(Widget& owner, Scalar dt)=0;
};

class AlternativeGeometry : public ReflectionLib::Object
{
public:
	rl_abstract_class(GUI::AlternativeGeometry)
	rl_base_class0(ReflectionLib::Object)
	rl_reflective

	virtual boolean contain(const Widget& owner, const Vector& locPoint)=0;
};

class Widget : public Object
{
public:
	rl_class1(GUI::Widget, GUI::Widget)
	rl_base_class0(ReflectionLib::Object)
	rl_reflective

	Widget* parent;
	Screen* screen;
	rl_field(Vector, position);
	rl_field(Vector, size);
	rl_field(Vector, minsize);
	rl_field(Margin, margin);
	rl_field(Dock, dockX);
	rl_field(Dock, dockY);
	rl_field(Scalar, aspect)
	rl_field(int, aspectDockXDirection)
	rl_field(int, aspectDockYDirection)
	rl_field(int, order);
	rl_field(String, image);
	rl_field(uint, row);
	rl_field(uint, column);
	rl_field(Vector4f, color);
	rl_field(uint32, variationsMask);

	IReflective* modelInterface;

	rl_field(boolean, autoChildRectScaleX);
	rl_field(boolean, autoChildRectScaleY);
	rl_field(boolean, autoChildRectOffsetX);
	rl_field(boolean, autoChildRectOffsetY);
	rl_field(boolean, autoSizeX);
	rl_field(boolean, autoSizeY);
	rl_field(int, autoSizeXDirection);
	rl_field(int, autoSizeYDirection);

	rl_field(boolean, childClip);
	rl_field(Vector, childRectScale);
	rl_field(Vector, childRectOffset);

	rl_field(String, tag);
	rl_field(String, name);
	rl_field(boolean, breakCursorBubbling);
	rl_field(boolean, updateInvisible);
	rl_field(boolean, exclusiveChildsCursorCollision);

	typedef ArrayOfConstructables< Widget, StrongPtr<Widget> > Childs_t;
	rl_field(Childs_t, childs)

	typedef ArrayOfConstructables< Modificator, StrongPtr<Modificator> > Modificators_t;
	rl_field(Modificators_t, modificators)

	typedef Constructable< AlternativeGeometry, StrongPtr<AlternativeGeometry> > AlternativeGeometry_t;
	rl_field(AlternativeGeometry_t, alternativeGeometry)

	rl_method_0(onLoadBegin)
	{
		childs.clear();
		modificators.clear();
	}

	rl_method_0(setFullDock)
	{
		dockX.setFull();
		dockY.setFull();
	}

	rl_method_0(setCenterDock)
	{
		dockX.setCenter();
		dockY.setCenter();
	}

	rl_method_1(setCenterPointDock, GUI::Vector, pos)
	{
		dockX.setCenterPoint(pos.x);
		dockY.setCenterPoint(pos.y);
	}

	rl_method_2(setRelPosSizeDock, GUI::Vector, pos, GUI::Vector, size)
	{
		dockX.setRelPosSize(pos.x, size.x);
		dockY.setRelPosSize(pos.y, size.y);
	}

	rl_method_0(focus);

	rl_method_1(setChildRectOffsetCentredTo, GUI::Vector, pos)
	{
		childRectOffset = size * Scalar(0.5) - pos * childRectScale;
	}

	Widget(Widget& inFather);
	virtual ~Widget();
	static Widget* rl_construct(IAllocator& alc, void** args);
	void* constructChild(Widget*& baseptr, const Type& type, IAllocator& alc);

	virtual void onFocusChanged(boolean focus, uint channel){}
	virtual boolean onFocusInputTunneling(const Input::EventBase& ev){ return false; }
	virtual boolean onFocusInputBubbling(const Input::EventBase& ev);

	virtual boolean onCursorTunneling(const Input::EventBase& ev, const Vector& locpos, const CursorState& state) { return false; }
	virtual boolean onCursorBubbling(const Input::EventBase& ev, const Vector& locpos, const CursorState& state) { return breakCursorBubbling; }
	virtual void onCursorLeave(const CursorState& state, uint channel){}
	boolean guisysTraverseCursor(Array< TrawersData >& list, const Vector& pos);

	virtual void onUpdate(Scalar dt);
	void guisysTraverseUpdate(Array< WeakPtr<Widget> >& list);

	virtual void onUpdateLayout();
	void guisysTraverseUpdateLayout();
	virtual void updateLayout_FromParent(const Bound& parentBound);
	virtual void updateLayout_FromChilds();

	virtual void onDraw(const ParentDrawInfo& pdi, IDrawer& render);
	virtual void onPostDraw(const ParentDrawInfo& pdi, IDrawer& render){}
	void guisysTraverseDraw(const ParentDrawInfo& pdi, IDrawer& render);

	virtual void soundBeep(ConstString name, Scalar volume = Scalar(1))
	{
		if(parent)
			parent->soundBeep(name, volume);
	}

	INLINE boolean contain(const Vector& p)const
	{
		Vector d = p - position;
		Vector e = d - size;
		return d.x >= 0 && d.y >= 0 && e.x <= 0 && e.y <= 0;
	}

	INLINE boolean containEx(const Vector& p)const
	{
		if(!contain(p))
			return false;

		if(!alternativeGeometry)
			return true;

		return alternativeGeometry->contain(*this, (p - position) / size);
	}

	/*
	INLINE Scalar containDistance(const Vector& p)const
	{
		Vector d = abs(p - (position + size * Scalar(0.5))) / size;
		return d.x + d.y;
	}

	INLINE Scalar containExDistance(const Vector& p)const
	{
		if(!alternativeGeometry)
			return containDistance(p);

		return alternativeGeometry->containDistance(*this, (p - position) / size);
	}*/

	Vector toChildCoords(const Vector& P)const
	{
		return (P - position - childRectOffset) / childRectScale;
	}

	INLINE uint findChildIndex(const Widget* w)const
	{
		for(uint i=0; i < childs.size(); ++i)
			if(childs[i] == w)
				return i;
		return childs.size();
	}

	Widget* findWidget(ConstString inName)
	{
		if(name == inName)
			return this;
		for(uint i=0; i < childs.size(); ++i)
			if(childs[i]){
				Widget* w = childs[i]->findWidget(inName);
				if(w)
					return w;
			}
		return 0;
	}

	template<class t_Type>
	t_Type* find(ConstString inName)
	{
		Reflection rl = reflectionOf(*this);
		if(name == inName && rl.type() == typeOf<t_Type>())
			return reinterpret_cast<t_Type*>(rl.cast<t_Type>().objectPtr());
		for(uint i=0; i < childs.size(); ++i)
			if(childs[i]){
				t_Type* w = childs[i]->find<t_Type>(inName);
				if(w)
					return w;
			}
		return 0;
	}

	template<class t_Type>
	t_Type* find()
	{
		Reflection rl = reflectionOf(*this);
		if(rl.type() == typeOf<t_Type>())
			return reinterpret_cast<t_Type*>(rl.cast<t_Type>().objectPtr());
		for(uint i=0; i < childs.size(); ++i)
			if(childs[i]){
				t_Type* w = childs[i]->find<t_Type>();
				if(w)
					return w;
			}
		return 0;
	}

	Widget* findInScreenWidget();
	Widget* findParentByName(ConstString name);

	Vector toScreenCoordFromChildCoord(const Vector& p)
	{
		Vector p2 = p * childRectScale + childRectOffset + position;
		if(parent)
			return parent->toScreenCoordFromChildCoord(p2);
		else
			return p2;
	}

	INLINE Vector toScreenCoord(const Vector& p)
	{
		Vector p2 = p + position;
		if(parent)
			return parent->toScreenCoordFromChildCoord(p2);
		else
			return p2;
	}

	Vector transformDelthaToScreenCoordsFromChildCoord(const Vector& d)
	{
		Vector d2 = d * childRectScale;
		if(parent)
			return parent->transformDelthaToScreenCoordsFromChildCoord(d2);
		else
			return d2;
	}

	INLINE Vector transformDelthaToScreenCoords(const Vector& d)
	{
		if(parent)
			return parent->transformDelthaToScreenCoordsFromChildCoord(d);
		else
			return d;
	}

	Vector fromScreenPosToLocalPos(const Vector& scrpos)const;

	Bound marginBound()const;

	INLINE Bound bound()const
	{
		return Bound(position, position + size);
	}

	Bound childBound()const;

	Bound childLocalBound()const;

	IReflective* getModelInterface()const
	{
		if(modelInterface)
			return modelInterface;
		if(parent)
			return parent->getModelInterface();
		return 0;
	}

	virtual Library* getLibrary()const
	{
		if(parent)
			return parent->getLibrary();
		return 0;
	}

	

	typedef Array< WeakPtr<Widget>, Base::SystemAllocatorWrapper, 64> WidgetTrawerseArray;
	virtual void onHide();
	Event<void(Widget&)> hideEvent;
	//virtual void onVisibleChanged(){}
	//Event<void(Widget&)> visibleChangedEvent;
	boolean visible()const { return _visible & _visibleFromMask; }
	void visible(boolean val);
	void guisysHideTraverse(WidgetTrawerseArray& list);

	rl_field(boolean, _visible);
	rl_field(boolean, _visibleFromMask);
};

}
