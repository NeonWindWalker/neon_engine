#pragma once

#include "seed.h"
#include "vector.h"
#include "list.h"

namespace Base
{

template<class t_Owner, class t_Flat>
class TSpaceTreeFlatListElement;

template<class t_Owner, class t_Flat>
class TOctreeFlat;

template<class t_AllocatorWrapper, class t_ElementOwner, typename t_Scalar>
class TOctree;

template<class t_AllocatorWrapper, class t_ElementOwner, typename t_Scalar>
class Quadtree;

template<class t_Owner, class t_Flat>
class TSpaceTreeFlatListElement
{
protected:
	PrevNext prevnext;
	t_Flat* flat;

public:
	typedef t_Owner Owner_t;
	typedef t_Flat Flat_t;

	t_Owner* owner;

	INLINE TSpaceTreeFlatListElement(t_Owner* p)
	{
		this->owner = p;
		flat = 0;
	}

	INLINE TSpaceTreeFlatListElement(TSpaceTreeFlatListElement& src, MoveInMemoryConstructorExtension ex) : prevnext(src, ex)
	{
		this->owner = src.owner;
		this->flat = src.flat;
	}

	INLINE ~TSpaceTreeFlatListElement()
	{
		this->prevnext.erase();
	}

	INLINE void beforeInsert(TSpaceTreeFlatListElement& e){ this->prevnext.beforeInsert(e); }
	INLINE void afterInsert(TSpaceTreeFlatListElement& e){ this->prevnext.afterInsert(e); }
	INLINE void erase(){ this->prevnext.erase(); }
	INLINE TSpaceTreeFlatListElement* next()const { return reinterpret_cast<TSpaceTreeFlatListElement*>(this->prevnext.next); }
	INLINE TSpaceTreeFlatListElement* prev()const { return reinterpret_cast<TSpaceTreeFlatListElement*>(this->prevnext.prev); }
	INLINE boolean included()const { return this->prevnext.included(); }

	template<class, class>
	friend class TSpaceTreeFlatListRoot;
	template<class, class, class>
	friend class TOctree;
	template<class, class, class>
	friend class TQuadtree;

private:
	TSpaceTreeFlatListElement(const TSpaceTreeFlatListElement& other){}
	void operator = (const TSpaceTreeFlatListElement& other){}
};

template<class t_Owner, class t_Flat>
class TSpaceTreeFlatListRoot
{
protected:
	PrevNext prevnext;
	t_Flat* flat;

public:
	typedef t_Owner Owner_t;
	typedef t_Flat Flat_t;

	INLINE TSpaceTreeFlatListRoot(t_Flat* srcFlat) { this->flat = srcFlat; }
	INLINE TSpaceTreeFlatListRoot(t_Flat* srcFlat, TSpaceTreeFlatListRoot& src, MoveInMemoryConstructorExtension ex) : PrevNext(src, ex)
	{
		this->flat = srcFlat;
		for(PrevNext* i = this->prevnext.next; i != &this->prevnext; i = i->next)
			reinterpret_cast< TSpaceTreeFlatListElement<t_Owner, Flat_t> >(i)->flat = this->flat;
	}

	INLINE ~TSpaceTreeFlatListRoot()
	{
		clear();
	}

	INLINE void insertLast(TSpaceTreeFlatListElement<t_Owner, Flat_t>& e){ this->prevnext.beforeInsert(e.prevnext); e.flat = this->flat; }
	INLINE void insertFirst(TSpaceTreeFlatListElement<t_Owner, Flat_t>& e){ this->prevnext.afterInsert(e.prevnext); e.flat = this->flat; }
	INLINE void insert(TSpaceTreeFlatListElement<t_Owner, Flat_t>& e){ insertLast(e); }
	INLINE boolean empty() { return this->prevnext.prev == &prevnext; }
	INLINE void clear()
	{ 
		for(PrevNext* n = prevnext.next; n != &prevnext; ){
			PrevNext* del = n;
			n = n->next;
			del->prev = del->next = del;
		}
		prevnext.prev = prevnext.next = &prevnext;
	}

	class Iterator
	{
	public:
		Iterator(){ this->_ptr = 0; }
		Iterator(TSpaceTreeFlatListElement<t_Owner, t_Flat> * p) { this->_ptr = p; }
		INLINE Iterator operator ++ () { this->_ptr = this->_ptr->next(); return *this; }
		INLINE Iterator operator -- () { this->_ptr = this->_ptr->prev(); return *this; }
		friend INLINE boolean operator != (const Iterator& a, const Iterator& b) { return a._ptr != b._ptr; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator& b) { return a._ptr == b._ptr; }
		INLINE t_Owner* operator -> () const { return this->_ptr->owner; }
		INLINE t_Owner& operator * () const { return *this->_ptr->owner; }
		INLINE void beforeInsert(TSpaceTreeFlatListElement<t_Owner, t_Flat>& e) const { this->_ptr->beforeInsert(e); e.flat = this->_ptr->flat; }
		INLINE void afterInsert(TSpaceTreeFlatListElement<t_Owner, t_Flat>& e) const { this->_ptr->afterInsert(e); e.flat = this->_ptr->flat; }
	private:
		TSpaceTreeFlatListElement<t_Owner, t_Flat>* _ptr;
	};

	class ConstIterator
	{
	public:
		ConstIterator(){ this->_ptr = 0; }
		ConstIterator(const TSpaceTreeFlatListElement<t_Owner, t_Flat>* p) { this->_ptr = p; }
		INLINE ConstIterator operator ++ () { this->_ptr = this->_ptr->next(); return *this; }
		INLINE ConstIterator operator -- () { this->_ptr = this->_ptr->prev(); return *this; }
		friend INLINE boolean operator != (const ConstIterator& a, const ConstIterator& b) { return a._ptr != b._ptr; }
		friend INLINE boolean operator == (const ConstIterator& a, const ConstIterator& b) { return a._ptr == b._ptr; }
		friend INLINE boolean operator != (const ConstIterator& a, const Iterator& b) { return a._ptr != b._ptr; }
		friend INLINE boolean operator == (const ConstIterator& a, const Iterator& b) { return a._ptr == b._ptr; }
		friend INLINE boolean operator != (const Iterator& a, const ConstIterator& b) { return a._ptr != b._ptr; }
		friend INLINE boolean operator == (const Iterator& a, const ConstIterator& b) { return a._ptr == b._ptr; }
		INLINE const t_Owner* operator -> () const { return this->_ptr->owner; }
		INLINE const t_Owner& operator * () const { return *this->_ptr->owner; }
	private:
		const TSpaceTreeFlatListElement<t_Owner, t_Flat>* _ptr;
	};

	INLINE Iterator first() { return Iterator(reinterpret_cast<TSpaceTreeFlatListElement<t_Owner, t_Flat>*>(this->prevnext.next)); }
	INLINE Iterator last() { return Iterator(reinterpret_cast<TSpaceTreeFlatListElement<t_Owner, t_Flat>*>(this->prevnext.prev)); }
	INLINE Iterator begin() { return Iterator(reinterpret_cast<TSpaceTreeFlatListElement<t_Owner, t_Flat>*>(&this->prevnext)); }
	INLINE Iterator end() { return Iterator(reinterpret_cast<TSpaceTreeFlatListElement<t_Owner, t_Flat>*>(&this->prevnext)); }

	INLINE ConstIterator first()const { return ConstIterator(reinterpret_cast<const TSpaceTreeFlatListElement<t_Owner, t_Flat>*>(this->prevnext.next)); }
	INLINE ConstIterator last()const { return ConstIterator(reinterpret_cast<const TSpaceTreeFlatListElement<t_Owner, t_Flat>*>(this->prevnext.prev)); }
	INLINE ConstIterator begin()const { return ConstIterator(reinterpret_cast<const TSpaceTreeFlatListElement<t_Owner, t_Flat>*>(&this->prevnext)); }
	INLINE ConstIterator end()const { return ConstIterator(reinterpret_cast<const TSpaceTreeFlatListElement<t_Owner, t_Flat>*>(&this->prevnext)); }

private:
	TSpaceTreeFlatListRoot(const TSpaceTreeFlatListRoot& other){}
	void operator = (const TSpaceTreeFlatListRoot& other){}
};


template<class t_ElementOwner, typename t_Scalar = float>
class TOctreeFlat
{
public:
	typedef typename Vector3T<t_Scalar>::Type Vector_t;
	typedef t_ElementOwner ElementOwner_t;
	typedef t_Scalar Scalar_t;

	Vector_t center;
	Scalar_t halfsize;
	TOctreeFlat* father;
	uint8 inFatherNumber[3];
	uint8 level;
	TOctreeFlat* child[2][2][2];
	TSpaceTreeFlatListRoot<t_ElementOwner, TOctreeFlat> list;

	INLINE TOctreeFlat() : list(this)
	{
		eraseFields();
	}

	INLINE TOctreeFlat(TOctreeFlat& other, MoveInMemoryConstructorExtension ex) : list(other, ex)
	{
		if(other.father)
			other.father->child[other.inFatherNumber[2]][other.inFatherNumber[1]][other.inFatherNumber[0]] = this;
		this->father = other.father;
		this->inFatherNumber[0] = other.inFatherNumber[0];
		this->inFatherNumber[1] = other.inFatherNumber[1];
		this->inFatherNumber[2] = other.inFatherNumber[2];
		this->level = other.level;
		this->child[0][0][0] = other.child[0][0][0];
		this->child[0][0][1] = other.child[0][0][1];
		this->child[0][1][0] = other.child[0][1][0];
		this->child[0][1][1] = other.child[0][1][1];
		this->child[1][0][0] = other.child[1][0][0];
		this->child[1][0][1] = other.child[1][0][1];
		this->child[1][1][0] = other.child[1][1][0];
		this->child[1][1][1] = other.child[1][1][1];
		this->center = other.center;
		this->halfsize = other.halfsize;
		other.eraseFields();
	}

protected:
	INLINE void eraseFields()
	{
		this->father = 0;
		this->inFatherNumber[0] = 0;
		this->inFatherNumber[1] = 0;
		this->inFatherNumber[2] = 0;
		this->level = 0;
		this->child[0][0][0] = 0;
		this->child[0][0][1] = 0;
		this->child[0][1][0] = 0;
		this->child[0][1][1] = 0;
		this->child[1][0][0] = 0;
		this->child[1][0][1] = 0;
		this->child[1][1][0] = 0;
		this->child[1][1][1] = 0;
		this->center.setZero();
		this->halfsize = t_Scalar(0);
	}

private:
	TOctreeFlat(const TOctreeFlat& other){}
	void operator = (const TOctreeFlat& other){}
};

template<class t_AllocatorWrapper, class t_ElementOwner, typename t_Scalar = float>
class TOctree : public t_AllocatorWrapper
{
public:
	typedef typename Vector3T<t_Scalar>::Type Vector_t;
	typedef t_Scalar Scalar_t;
	typedef t_AllocatorWrapper AllocatorWrapper_t;
	typedef TOctreeFlat<t_ElementOwner, t_Scalar> Flat_t;
	typedef TSpaceTreeFlatListElement<t_ElementOwner, Flat_t> Element_t;

	INLINE TOctree(t_Scalar halfsize, uint8 maxLevel) : _maxLevel(maxLevel)
	{
		_root.halfsize = halfsize;
	}

	INLINE TOctree(t_Scalar halfsize, uint8 maxLevel, typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), _maxLevel(maxLevel)
	{
		_root.halfsize = halfsize;
	}

	INLINE void include(Element_t& e, const Vector_t& position, const Vector_t& halfsize)
	{
		Assert(!e.included());
		include(&_root, e, position, halfsize);
		//verify(&_root);
	}

	INLINE void exclude(Element_t& e)
	{
		Assert(e.included());
		e.erase();
		mayUpFree(e.flat);
		//verify(&_root);
	}

	INLINE void update(Element_t& e, const Vector_t& position,  const Vector_t& halfsize)
	{
		Assert(e.included());
		Flat_t* f = e.flat;
		Vector_t da = (position - f->center).abs();
		boolean bContatin = false;
		if((da + halfsize - Vector_t(f->halfsize, f->halfsize, f->halfsize)).isAllNegative())//contain
		{
			bContatin = true;

			Scalar_t characteristicSize = max(halfsize.x, max(halfsize.y, halfsize.z)) * Scalar_t(4.0);
			if(characteristicSize >= f->halfsize || f->level >= _maxLevel)//no need go down
				return;

			if((da - halfsize).isAnyNegative())// cross witch flat separation planes
				return;
		}

		e.erase();
		include(bContatin ? f : &_root, e, position, halfsize);
		mayUpFree(f);
		//verify(&_root);
	}

	Flat_t& root() { return _root; }

protected:
	Flat_t _root;
	uint8 _maxLevel;

	void include(Flat_t* f, Element_t& e, const Vector_t& position,  const Vector_t& halfsize)
	{
		t_Scalar characteristicSize = max(halfsize.x, max(halfsize.y, halfsize.z)) * t_Scalar(4.0);
		while(true)
		{
			if(characteristicSize >= f->halfsize || f->level >= _maxLevel || //no need go down
					(abs(position - f->center) - halfsize).isAnyNegative())// cross witch flat separation planes
			{
				f->list.insert(e);
				e.flat = f;
				return;
			}

			Vector_t pmc = position - f->center;
			boolean01 Ix = negativeBit(pmc.x);
			boolean01 Iy = negativeBit(pmc.y);
			boolean01 Iz = negativeBit(pmc.z);

			if(f->child[Iz][Iy][Ix]){
				f = f->child[Iz][Iy][Ix];
				continue;
			}
			
			Vector_t fI((Scalar_t)Ix, (Scalar_t)Iy, (Scalar_t)Iz);
			Flat_t* c = new(this->allocator()) Flat_t();
			c->father = f;
			c->inFatherNumber[0] = Ix;
			c->inFatherNumber[1] = Iy;
			c->inFatherNumber[2] = Iz;
			f->child[Iz][Iy][Ix] = c;
			c->level = f->level + 1;
			c->center = f->center + f->halfsize * (Scalar_t(0.5) - fI);
			c->halfsize = f->halfsize * Scalar_t(0.5);
			f = c;

			Assert(f->father->child[f->inFatherNumber[2]][f->inFatherNumber[1]][f->inFatherNumber[0]] == f);
		}
	}

	void mayUpFree(Flat_t* f)
	{
		do
		{
			if(!f->list.empty())return;
			if(!f->father)return;
			if(f->child[0][0][0])return;
			if(f->child[0][0][1])return;
			if(f->child[0][1][0])return;
			if(f->child[0][1][1])return;
			if(f->child[1][0][0])return;
			if(f->child[1][0][1])return;
			if(f->child[1][1][0])return;
			if(f->child[1][1][1])return;
			
			Flat_t * del = f;
			Assert(f->father->child[f->inFatherNumber[2]][f->inFatherNumber[1]][f->inFatherNumber[0]] == f);
			f->father->child[f->inFatherNumber[2]][f->inFatherNumber[1]][f->inFatherNumber[0]] = 0;
			f = f->father;
			delete del;
		}
		while(true);
	}

	void verify(Flat_t* f)
	{
		if(f == 0)
			return;
		verify(f->child[0][0][0]);
		verify(f->child[0][0][1]);
		verify(f->child[0][1][0]);
		verify(f->child[0][1][1]);
		verify(f->child[1][0][0]);
		verify(f->child[1][0][1]);
		verify(f->child[1][1][0]);
		verify(f->child[1][1][1]);
		if(f->father == 0)
			return;
		Assert(f->father->child[f->inFatherNumber[2]][f->inFatherNumber[1]][f->inFatherNumber[0]] == f);
	}
};

template<class t_ElementOwner, class t_Scalar = float>
struct OctreeWithCellAllocatorT
{
	typedef TOctree<
		Base::AllocatorWrapper< Base::TCellDefragmentAllocator< Base::TOctreeFlat<t_ElementOwner, t_Scalar > > >,
		t_ElementOwner, 
		t_Scalar> Type;
};




template<class t_ElementOwner, typename t_Scalar = float>
class TQuadtreeFlat
{
public:
	typedef typename Vector2T<t_Scalar>::Type Vector_t;
	typedef t_ElementOwner ElementOwner_t;
	typedef t_Scalar Scalar_t;

	Vector_t center;
	Scalar_t halfsize;
	TQuadtreeFlat* father;
	uint8 inFatherNumber[2];
	uint8 level;
	TQuadtreeFlat* child[2][2];
	TSpaceTreeFlatListRoot<t_ElementOwner, TQuadtreeFlat> list;

	INLINE TQuadtreeFlat() : list(this)
	{
		eraseFields();
	}

	INLINE TQuadtreeFlat(TQuadtreeFlat& other, MoveInMemoryConstructorExtension ex) : list(other, ex)
	{
		if(other.father)
			other.father->child[other.inFatherNumber[1]][other.inFatherNumber[0]] = this;
		this->father = other.father;
		this->inFatherNumber[0] = other.inFatherNumber[0];
		this->inFatherNumber[1] = other.inFatherNumber[1];
		this->level = other.level;
		this->child[0][0] = other.child[0][0];
		this->child[0][1] = other.child[0][1];
		this->child[1][0] = other.child[1][0];
		this->child[1][1] = other.child[1][1];
		this->center = other.center;
		this->halfsize = other.halfsize;
		other.eraseFields();
	}

protected:
	INLINE void eraseFields()
	{
		this->father = 0;
		this->inFatherNumber[0] = 0;
		this->inFatherNumber[1] = 0;
		this->level = 0;
		this->child[0][0] = 0;
		this->child[0][1] = 0;
		this->child[1][0] = 0;
		this->child[1][1] = 0;
		this->center.setZero();
		this->halfsize = t_Scalar(0);
	}

private:
	TQuadtreeFlat(const TQuadtreeFlat& other){}
	void operator = (const TQuadtreeFlat& other){}
};


template<class t_AllocatorWrapper, class t_ElementOwner, typename t_Scalar = float>
class TQuadtree : public t_AllocatorWrapper
{
public:
	typedef typename Vector2T<t_Scalar>::Type Vector_t;
	typedef t_Scalar Scalar_t;
	typedef t_AllocatorWrapper AllocatorWrapper_t;
	typedef TQuadtreeFlat<t_ElementOwner, t_Scalar> Flat_t;
	typedef TSpaceTreeFlatListElement<t_ElementOwner, Flat_t> Element_t;

	INLINE TQuadtree(t_Scalar halfsize, uint8 maxLevel) : _maxLevel(maxLevel)
	{
		_root.halfsize = halfsize;
	}

	INLINE TQuadtree(t_Scalar halfsize, uint8 maxLevel, typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), _maxLevel(maxLevel)
	{
		_root.halfsize = halfsize;
	}

	INLINE void include(Element_t& e, const Vector_t& position, const Vector_t& halfsize)
	{
		Assert(!e.included());
		include(&_root, e, position, halfsize);
		//verify(&_root);
	}

	INLINE void exclude(Element_t& e)
	{
		Assert(e.included());
		e.erase();
		mayUpFree(e.flat);
		//verify(&_root);
	}

	INLINE void update(Element_t& e, const Vector_t& position,  const Vector_t& halfsize)
	{
		Assert(e.included());
		Flat_t* f = e.flat;
		Vector_t da = (position - f->center).abs();
		boolean bContatin = false;
		if((da + halfsize - Vector_t(f->halfsize, f->halfsize)).isAllNegative())//contain
		{
			bContatin = true;

			Scalar_t characteristicSize = max(halfsize.x, halfsize.y) * Scalar_t(4.0);
			if(characteristicSize >= f->halfsize || f->level >= _maxLevel)//no need go down
				return;

			if((da - halfsize).isAnyNegative())// cross witch flat separation planes
				return;
		}

		e.erase();
		include(bContatin ? f : &_root, e, position, halfsize);
		mayUpFree(f);
		//verify(&_root);
	}

	Flat_t& root() { return _root; }

protected:
	Flat_t _root;
	uint8 _maxLevel;

	void include(Flat_t* f, Element_t& e, const Vector_t& position,  const Vector_t& halfsize)
	{
		t_Scalar characteristicSize = max(halfsize.x, max(halfsize.y, halfsize.z)) * t_Scalar(4.0);
		while(true)
		{
			if(characteristicSize >= f->halfsize || f->level >= _maxLevel || //no need go down
				(abs(position - f->center) - halfsize).isAnyNegative())// cross witch flat separation planes
			{
				f->list.insert(e);
				e.flat = f;
				return;
			}

			Vector_t pmc = position - f->center;
			boolean01 Ix = negativeBit(pmc.x);
			boolean01 Iy = negativeBit(pmc.y);

			if(f->child[Iy][Ix]){
				f = f->child[Iy][Ix];
				continue;
			}

			Vector_t fI((Scalar_t)Ix, (Scalar_t)Iy);
			Flat_t* c = new(this->allocator()) Flat_t();
			c->father = f;
			c->inFatherNumber[0] = Ix;
			c->inFatherNumber[1] = Iy;
			f->child[Iy][Ix] = c;
			c->level = f->level + 1;
			c->center = f->center + f->halfsize * (Scalar_t(0.5) - fI);
			c->halfsize = f->halfsize * Scalar_t(0.5);
			f = c;

			Assert(f->father->child[f->inFatherNumber[1]][f->inFatherNumber[0]] == f);
		}
	}

	void mayUpFree(Flat_t* f)
	{
		do
		{
			if(!f->list.empty())return;
			if(!f->father)return;
			if(f->child[0][0])return;
			if(f->child[0][1])return;
			if(f->child[1][0])return;
			if(f->child[1][1])return;

			Flat_t * del = f;
			Assert(f->father->child[f->inFatherNumber[1]][f->inFatherNumber[0]] == f);
			f->father->child[f->inFatherNumber[1]][f->inFatherNumber[0]] = 0;
			f = f->father;
			delete del;
		}
		while(true);
	}

	void verify(Flat_t* f)
	{
		if(f == 0)
			return;
		verify(f->child[0][0]);
		verify(f->child[0][1]);
		verify(f->child[1][0]);
		verify(f->child[1][1]);
		if(f->father == 0)
			return;
		Assert(f->father->child[f->inFatherNumber[1]][f->inFatherNumber[0]] == f);
	}
};

template<class t_ElementOwner, class t_Scalar = float>
struct QuadtreeWithCellAllocatorT
{
	typedef TQuadtree<
		Base::AllocatorWrapper< Base::TCellDefragmentAllocator< Base::TQuadtreeFlat<t_ElementOwner, t_Scalar > > >,
		t_ElementOwner, 
		t_Scalar> Type;
};

}
