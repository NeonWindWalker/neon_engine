#pragma once
#include "base.h"

namespace ParticlesLib
{

class View
{
public:
	Matrix34f place;
	float cone;

	ListElement<View> lel;

	View() : lel(this)
	{
		place.setIdentity();
		cone = 1;
	}

	struct Entry
	{
		union
		{
			float radius;
			uint32 nradius;
		};
		uint32 number;
	};

	void begin()
	{
		radiusAndRemapArray.clearSaveReserve();
		unsortedQuadArray.clearSaveReserve();
	}

	Quad* pushQuad(float r)
	{
		uint n = radiusAndRemapArray.size();
		Entry& e = *radiusAndRemapArray.push();
		e.radius = r;
		e.number = n;
		unsortedQuadArray.resize(n + 1);
		return &unsortedQuadArray[n];
	}

	template<class t_UserEndClass>
	void end(t_UserEndClass& userobj)
	{
		radiusAndRemapArray.sort(EntryComparator());
		userobj.destanationQuadArraySetSize(radiusAndRemapArray.size());
		for(uint i=0; i < radiusAndRemapArray.size(); ++i)
			userobj.assignDestanationQuad(i, unsortedQuadArray[radiusAndRemapArray[i].number]);
	}

	virtual void callBeginImpl(){ begin(); }
	virtual void callEndImpl()=0;

	ConstRange< Entry> radiuses()const { return radiusAndRemapArray.range(); }

private:
	Array<Quad, Base::SystemAllocatorWrapper, 0, true> unsortedQuadArray;
	Array<Entry, Base::SystemAllocatorWrapper, 0, true> radiusAndRemapArray;

	class EntryComparator
	{
	public:
		INLINE int operator () (const Entry& a, const Entry& b)const { return b.nradius - a.nradius; }
	};
};

}
