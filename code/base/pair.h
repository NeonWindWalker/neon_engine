#pragma once

#include "comparator.h"
#include "hash.h"

namespace Base
{

template<class t_Key, class t_Value>
struct Pair
{
	t_Key key;
	t_Value val;
	INLINE Pair(){}
	INLINE Pair(const t_Key& k, const t_Value& v) : key(k), val(v) {}
	INLINE Pair(const Pair& other) : key(other.key), val(other.val) {}
	INLINE Pair& operator = (const Pair& other)
	{
		key = other.key;
		val = other.val;
		return *this;
	}

	//INLINE Pair(const t_Key& k) : key(k) {}
	template<class t_OtherKey>
	INLINE Pair& operator = (const t_OtherKey& k)
	{
		key = k;
		return *this;
	}

	template<class t_OtherVal>
	INLINE boolean operator == (const t_OtherVal& b)const{ return this->key == b; }
	template<class t_OtherVal>
	INLINE boolean operator != (const t_OtherVal& b)const{ return this->key != b; }

	INLINE boolean operator < (const Pair& b)const { return this->key < b.key; }
	INLINE boolean operator > (const Pair& b)const { return this->key > b.key; }
	INLINE boolean operator <= (const Pair& b)const { return this->key <= b.key; }
	INLINE boolean operator >= (const Pair& b)const { return this->key >= b.key; }
	INLINE boolean operator == (const Pair& b)const { return this->key == b.key; }
	INLINE boolean operator != (const Pair& b)const { return this->key != b.key; }

	friend INLINE boolean operator < (const Pair& a, const t_Key& b) { return a.key < b; }
	friend INLINE boolean operator > (const Pair& a, const t_Key& b) { return a.key > b; }
	friend INLINE boolean operator <= (const Pair& a, const t_Key& b) { return a.key <= b; }
	friend INLINE boolean operator >= (const Pair& a, const t_Key& b) { return a.key >= b; }
	friend INLINE boolean operator == (const Pair& a, const t_Key& b) { return a.key == b; }
	friend INLINE boolean operator != (const Pair& a, const t_Key& b) { return a.key != b; }

	friend INLINE boolean operator < (const t_Key& a, const Pair& b) { return a < b.key; }
	friend INLINE boolean operator > (const t_Key& a, const Pair& b) { return a > b.key; }
	friend INLINE boolean operator <= (const t_Key& a, const Pair& b) { return a <= b.key; }
	friend INLINE boolean operator >= (const t_Key& a, const Pair& b) { return a >= b.key; }
	friend INLINE boolean operator == (const t_Key& a, const Pair& b) { return a == b.key; }
	friend INLINE boolean operator != (const t_Key& a, const Pair& b) { return a != b.key; }
};

template<class t_Key, class t_Value> Pair<t_Key,t_Value> makePair(t_Key& k, t_Value& v)
{
	return Pair<t_Key,t_Value>(k, v);
}


template<class t_Key, class t_Value>
class DefaultComparator< const Pair<t_Key,t_Value>, const Pair<t_Key,t_Value> >
{
public:
	INLINE int operator() (const Pair<t_Key,t_Value>& a, const Pair<t_Key,t_Value>& b)const
	{
		return DefaultComparator<const t_Key, const t_Key>()(a.key, b.key);
	}

	template<class t_OtherValue>
	INLINE int operator() (const Pair<t_Key,t_Value>& a, const t_OtherValue& b)const
	{
		return DefaultComparator<const t_Key, const t_OtherValue>()(a.key, b);
	}

	template<class t_OtherValue>
	INLINE int operator() (const t_OtherValue& a, const Pair<t_Key,t_Value>& b)const
	{
		return DefaultComparator<const t_OtherValue, const t_Key>()(a, b.key);
	}
};

template<class t_Key, class t_Value, class t_Other>
class DefaultComparator< const Pair<t_Key,t_Value>, const t_Other >
{
public:
	INLINE int operator() (const Pair<t_Key,t_Value>& a, const t_Other& b)const
	{
		return DefaultComparator<const t_Key, const t_Other>()(a.key, b);
	}

	INLINE int operator() (const t_Other& a, const Pair<t_Key,t_Value>& b)const
	{
		return DefaultComparator<const t_Other, const t_Key>()(a, b.key);
	}
};

template<class t_Key, class t_Value, class t_Other>
class DefaultComparator< const t_Other, const Pair<t_Key,t_Value> >
{
public:
	INLINE int operator() (const Pair<t_Key,t_Value>& a, const t_Other& b)const
	{
		return DefaultComparator<const t_Key, const t_Other>()(a.key, b);
	}

	INLINE int operator() (const t_Other& a, const Pair<t_Key,t_Value>& b)const
	{
		return DefaultComparator<const t_Other, const t_Key>()(a, b.key);
	}
};


template<class t_Key, class t_Value>
class DefaultHashGen< const Pair<t_Key,t_Value> >
{
public:
	INLINE Hash operator () (const Pair<t_Key,t_Value>& pair) const { return DefaultHashGen<const t_Key>()(pair.key); }
	INLINE Hash operator () (const t_Key& key) const { return DefaultHashGen<const t_Key>()(key); }
};

template<typename t_Key, typename t_Val>
static INLINE void move(Pair<t_Key, t_Val>& newObj, Pair<t_Key, t_Val>& oldObj) 
{
	move(newObj.key, oldObj.key);
	move(newObj.val, oldObj.val);
}

template<typename t_Key, typename t_Val>
static INLINE void swap(Pair<t_Key, t_Val>& a, Pair<t_Key, t_Val>& b) 
{
	swap(a.key, b.key);
	swap(a.val, b.val);
}

template<typename t_Key, typename t_Val>
static INLINE void construct_destroy(Pair<t_Key, t_Val>* newObj, Pair<t_Key, t_Val>& oldObj) 
{
	new(newObj) Pair<t_Key, t_Val>();
	move(newObj->key, oldObj.key);
	move(newObj->val, oldObj.val);
	oldObj.~Pair<t_Key, t_Val>();
}

}
