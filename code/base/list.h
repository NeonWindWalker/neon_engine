#pragma once

#include "seed.h"

namespace Base {

class PrevNext
{
public:
	PrevNext* prev;
	PrevNext* next;

	INLINE PrevNext()
	{
		this->prev = this->next = this;
	}
	INLINE ~PrevNext() {}

	INLINE PrevNext(PrevNext& src, MoveInMemoryConstructorExtension ex)
	{
		this->prev = src.prev;
		src.prev->next = this;
		this->next = src.next;
		src.next->prev = this;
		src.prev = src.next = &src;
		if(prev == next)
			prev = next = this;
	}

	INLINE void beforeInsert(PrevNext& e)
	{
		e.prev = this->prev;
		e.next = this;
		e.prev->next = &e;
		e.next->prev = &e;
	}

	INLINE void afterInsert(PrevNext& e)
	{
		e.prev = this;
		e.next = this->next;
		e.prev->next = &e;
		e.next->prev = &e;
	}

	INLINE void erase()
	{
		this->prev->next = this->next;
		this->next->prev = this->prev;
		this->prev = this->next = this;
	}

	INLINE boolean included()const
	{ 
		return prev != this;
	}
};


template<typename t_Indata> class TListRoot;

template<typename t_Indata>
class TListElement
{
protected:
	PrevNext _prevnext;
public:
	t_Indata indata;

	INLINE TListElement(t_Indata& ind) : indata(ind) {}
	INLINE TListElement(t_Indata& ind, TListElement& src, MoveInMemoryConstructorExtension ex) : _prevnext(src, ex), indata(ind) {}
	INLINE ~TListElement() { erase(); }
	
	INLINE void beforeInsert(TListElement& e){ this->_prevnext.beforeInsert(e); }
	INLINE void afterInsert(TListElement& e){ this->_prevnext.afterInsert(e); }
	INLINE void erase(){ this->_prevnext.erase(); }
	INLINE TListElement* next()const { return reinterpret_cast<TListElement*>(this->_prevnext.next); }
	INLINE TListElement* prev()const { return reinterpret_cast<TListElement*>(this->_prevnext.prev); }
	INLINE boolean included()const { return this->_prevnext.included(); }

	friend class TListRoot<t_Indata>;

private:
	TListElement(const TListElement& other){}
	TListElement& operator = (const TListElement& other){}
};

template<typename t_Indata>
class TListRoot
{
protected:
	PrevNext _prevnext;
public:
	INLINE TListRoot() {}
	INLINE TListRoot(TListRoot& src, MoveInMemoryConstructorExtension ex) : _prevnext(src, ex) {}
	INLINE ~TListRoot() { clear(); }

	INLINE void insertLast(TListElement<t_Indata>& e){ this->_prevnext.beforeInsert(e._prevnext); }
	INLINE void insertFirst(TListElement<t_Indata>& e){ this->_prevnext.afterInsert(e._prevnext); }
	INLINE void insert(TListElement<t_Indata>& e){ insertLast(e); }
	INLINE boolean empty() { return this->_prevnext.prev == &_prevnext; }
	INLINE void clear()
	{ 
		for(PrevNext* n = _prevnext.next; n != &_prevnext; ){
			PrevNext* del = n;
			n = n->next;
			del->prev = del->next = del;
		}
		_prevnext.prev = _prevnext.next = &_prevnext;
	}

	class Iterator
	{
	public:
		Iterator(){ this->_ptr = 0; }
		Iterator(TListElement<t_Indata> * p) { this->_ptr = p; }
		INLINE Iterator operator ++ () { this->_ptr = this->_ptr->next(); return *this; }
		INLINE Iterator operator -- () { this->_ptr = this->_ptr->prev(); return *this; }
		friend INLINE boolean operator != (const Iterator& a, const Iterator& b) { return a._ptr != b._ptr; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator& b) { return a._ptr == b._ptr; }
		INLINE t_Indata* operator -> () const { return this->_ptr->indata; }
		INLINE t_Indata& operator * () const { return *this->_ptr->indata; }
		INLINE void beforeInsert(TListElement<t_Indata>& e) const { this->_ptr->beforeInsert(e); }
		INLINE void afterInsert(TListElement<t_Indata>& e) const { this->_ptr->afterInsert(e); }
	private:
		TListElement<t_Indata>* _ptr;
	};

	INLINE Iterator first() { return Iterator(reinterpret_cast<TListElement<t_Indata>*>(this->_prevnext.next)); }
	INLINE Iterator last() { return Iterator(reinterpret_cast<TListElement<t_Indata>*>(this->_prevnext.prev)); }
	INLINE Iterator begin() { return Iterator(reinterpret_cast<TListElement<t_Indata>*>(&this->_prevnext)); }
	INLINE Iterator end() { return Iterator(reinterpret_cast<TListElement<t_Indata>*>(&this->_prevnext)); }

private:
	TListRoot(const TListRoot& other){}
	TListRoot& operator = (const TListRoot& other){}
};





template<typename t_Indata> class ListRoot;

template<typename t_Indata>
class ListElement
{
protected:
	PrevNext _prevnext;
public:
	t_Indata* indata;

	INLINE ListElement(t_Indata* ind) : indata(ind) {}
	INLINE ListElement(t_Indata* ind, ListElement& src, MoveInMemoryConstructorExtension ex) : _prevnext(src, ex), indata(ind) {}
	INLINE ~ListElement() { erase(); }
	
	INLINE void beforeInsert(ListElement& e){ this->_prevnext.beforeInsert(e); }
	INLINE void afterInsert(ListElement& e){ this->_prevnext.afterInsert(e); }
	INLINE void erase(){ this->_prevnext.erase(); }
	INLINE ListElement* next()const { return reinterpret_cast<ListElement*>(this->_prevnext.next); }
	INLINE ListElement* prev()const { return reinterpret_cast<ListElement*>(this->_prevnext.prev); }
	INLINE boolean included()const { return this->_prevnext.included(); }

	friend class ListRoot<t_Indata>;

private:
	ListElement(const ListElement& other){}
	ListElement& operator = (const ListElement& other){}
};

template<typename t_Indata>
class ListRoot
{
protected:
	PrevNext _prevnext;
public:
	INLINE ListRoot() {}
	INLINE ListRoot(ListRoot& src, MoveInMemoryConstructorExtension ex) : _prevnext(src, ex) {}
	INLINE ~ListRoot() { clear(); }

	INLINE void insertLast(ListElement<t_Indata>& e){ this->_prevnext.beforeInsert(e._prevnext); }
	INLINE void insertFirst(ListElement<t_Indata>& e){ this->_prevnext.afterInsert(e._prevnext); }
	INLINE void insert(ListElement<t_Indata>& e){ insertLast(e); }
	INLINE boolean empty() { return this->_prevnext.prev == &_prevnext; }
	INLINE void clear()
	{ 
		for(PrevNext* n = _prevnext.next; n != &_prevnext; ){
			PrevNext* del = n;
			n = n->next;
			del->prev = del->next = del;
		}
			
		_prevnext.prev = _prevnext.next = &_prevnext;
	}

	class Iterator
	{
	public:
		Iterator(){ this->_ptr = 0; }
		Iterator(ListElement<t_Indata> * p) { this->_ptr = p; }
		INLINE Iterator operator ++ () { this->_ptr = this->_ptr->next(); return *this; }
		INLINE Iterator operator -- () { this->_ptr = this->_ptr->prev(); return *this; }
		friend INLINE boolean operator != (const Iterator& a, const Iterator& b) { return a._ptr != b._ptr; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator& b) { return a._ptr == b._ptr; }
		INLINE t_Indata* operator -> () const { return this->_ptr->indata; }
		INLINE t_Indata& operator * () const { return *this->_ptr->indata; }
		INLINE void beforeInsert(ListElement<t_Indata>& e) const { this->_ptr->beforeInsert(e); }
		INLINE void afterInsert(ListElement<t_Indata>& e) const { this->_ptr->afterInsert(e); }
	private:
		ListElement<t_Indata>* _ptr;
	};

	INLINE Iterator first()const { return Iterator(reinterpret_cast<ListElement<t_Indata>*>(const_cast<PrevNext*>(this->_prevnext.next))); }
	INLINE Iterator last()const { return Iterator(reinterpret_cast<ListElement<t_Indata>*>(const_cast<PrevNext*>(this->_prevnext.prev))); }
	INLINE Iterator begin()const { return Iterator(reinterpret_cast<ListElement<t_Indata>*>(const_cast<PrevNext*>(&this->_prevnext))); }
	INLINE Iterator end()const { return Iterator(reinterpret_cast<ListElement<t_Indata>*>(const_cast<PrevNext*>(&this->_prevnext))); }

private:
	ListRoot(const ListRoot& other){}
	ListRoot& operator = (const ListRoot& other){}
};


}
