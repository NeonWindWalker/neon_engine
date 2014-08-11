#pragma once

#include "array.h"

namespace Base
{

template<typename t_Value, typename t_AllocatorWrapper = SystemAllocatorWrapper, uint t_ReservedCount = DefaultArrayReservedElementsCount<t_Value>::gcReservedElementsCount, boolean t_IsRawValueType = IsRawType<t_Value>::gcIsRawType, typename t_Comparator = DefaultComparator<const t_Value, const t_Value> >
class SortedArray : public Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>
{
public:
	typedef Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType> Array_t;
	typedef typename Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>::Iterator Iterator;
	typedef typename Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>::ConstIterator ConstIterator;

	INLINE SortedArray() {}
	INLINE SortedArray(typename t_AllocatorWrapper::Data_t& ad) : Array_t(ad) {}
	INLINE SortedArray(const SortedArray& other) : Array_t(other.array()) {}
	INLINE SortedArray(SortedArray& other, MoveInMemoryConstructorExtension flag) : Array_t(other.array(), flag) {}
	INLINE void move(SortedArray& other) { Array_t::move(other.array()); }

	INLINE SortedArray& operator = (const SortedArray& other)
	{
		Array_t::set(other.array());
		return *this;
	}

	INLINE const Array_t& array()const { return *this; }
	INLINE Array_t& array() { return *this; }

	template<class t_CompareValue>
	INLINE t_Value* find(const t_CompareValue& val)
	{
		return Array_t::first() + findInSortedArray(Array_t::first(), Array_t::size(), val, t_Comparator());
	}

	template<class t_CompareValue, class tt_Comparator>
	INLINE t_Value* find(const t_CompareValue& val, const tt_Comparator& cmp)
	{
		return Array_t::first() + findInSortedArray(Array_t::first(), Array_t::size(), val, cmp);
	}

	template<class t_CompareValue>
	INLINE const t_Value* find(const t_CompareValue& val)const
	{
		return Array_t::first() + findInSortedArray(Array_t::first(), Array_t::size(), val, t_Comparator());
	}

	template<class t_CompareValue, class tt_Comparator>
	INLINE const t_Value* find(const t_CompareValue& val, const tt_Comparator& cmp)const
	{
		return Array_t::first() + findInSortedArray(Array_t::first(), Array_t::size(), val, cmp);
	}

	template<class t_CompareValue>
	INLINE void erase(const t_CompareValue& val)
	{
		t_Value* v = Array_t::first() + findInSortedArray(Array_t::first(), Array_t::size(), val, t_Comparator());
		if(v == Array_t::end())
			return;
		Array_t::erase(v);
	}

	template<class t_CompareValue, class tt_Comparator>
	INLINE void erase(const t_CompareValue& val, const tt_Comparator& cmp)
	{
		t_Value* v = Array_t::first() + findInSortedArray(Array_t::first(), Array_t::size(), val, cmp);
		if(v == Array_t::end())
			return;
		Array_t::erase(v);
	}

	INLINE void erase(Iterator it)
	{
		Array_t::erase(it);
	}

	INLINE void eraseByIndex(uint position)
	{
		Array_t::erase(position);
	}

	template<class t_OtherKey>
	INLINE Iterator insert(boolean& bIns, const t_OtherKey& key)
	{
		uint n = findInsertPositionSortedArray(bIns, Array_t::first(), Array_t::size(), key, t_Comparator());
		if(bIns)
			*Array_t::insertDefault(n) = key;
		return &Array_t::first()[n];
	}

	template<class t_OtherKey, class tt_Comparator>
	INLINE Iterator insert(boolean& bIns, const t_OtherKey& key, const tt_Comparator& cmp)
	{
		uint n = findInsertPositionSortedArray(bIns, Array_t::first(), Array_t::size(), key, cmp);
		if(bIns)
			*Array_t::insertDefault(n) = key;
		return &Array_t::first()[n];
	}

	template<class t_OtherKey>
	INLINE Iterator insert(const t_OtherKey& key)
	{
		uint low = findGreaterOrEqualInSortedArray(Array_t::first(), Array_t::size(), key, t_Comparator());
		*Array_t::insertDefault(low) = key;
		return Array_t::first() + low;
	}

	template<class t_OtherKey, class tt_Comparator>
	INLINE Iterator insert(const t_OtherKey& key, const tt_Comparator& cmp)
	{
		uint low = findGreaterOrEqualInSortedArray(Array_t::first(), Array_t::size(), key, cmp);
		*Array_t::insertDefault(low) = key;
		return Array_t::first() + low;
	}

	template<class t_CompareValue>
	INLINE ConstIterator lessequal(const t_CompareValue& val)const
	{
		return Array_t::first() + findLesserOrEqualInSortedArray(Array_t::first(), Array_t::size(), val, t_Comparator());
	}

	template<class t_CompareValue, class tt_Comparator>
	INLINE ConstIterator lessequal(const t_CompareValue& val, const tt_Comparator& cmp)const
	{
		return Array_t::first() + findLesserOrEqualInSortedArray(Array_t::first(), Array_t::size(), val, cmp);
	}

	template<class t_CompareValue>
	INLINE Iterator lessequal(const t_CompareValue& val)
	{
		return Array_t::first() + findLesserOrEqualInSortedArray(Array_t::first(), Array_t::size(), val, t_Comparator());
	}

	template<class t_CompareValue, class tt_Comparator>
	INLINE Iterator lessequal(const t_CompareValue& val, const tt_Comparator& cmp)
	{
		return Array_t::first() + findLesserOrEqualInSortedArray(Array_t::first(), Array_t::size(), val, cmp);
	}

	template<class t_CompareValue>
	INLINE ConstIterator greaterequal(const t_CompareValue& val)const
	{
		return Array_t::first() + findGreaterOrEqualInSortedArray(Array_t::first(), Array_t::size(), val, t_Comparator());
	}

	template<class t_CompareValue, class tt_Comparator>
	INLINE ConstIterator greaterequal(const t_CompareValue& val, const tt_Comparator& cmp)const
	{
		return Array_t::first() + findGreaterOrEqualInSortedArray(Array_t::first(), Array_t::size(), val, cmp);
	}

	template<class t_CompareValue>
	INLINE Iterator greaterequal(const t_CompareValue& val)
	{
		return Array_t::first() + findGreaterOrEqualInSortedArray(Array_t::first(), Array_t::size(), val, t_Comparator());
	}

	template<class t_CompareValue, class tt_Comparator>
	INLINE Iterator greaterequal(const t_CompareValue& val, const tt_Comparator& cmp)
	{
		return Array_t::first() + findGreaterOrEqualInSortedArray(Array_t::first(), Array_t::size(), val, cmp);
	}

	template<class t_OtherKey>
	INLINE Iterator operator () (const t_OtherKey& key) { boolean outIsInserted; return insert(outIsInserted, key); }
};


template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount, boolean t_IsRawValueType>
static INLINE void move(SortedArray<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& newObj, SortedArray<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& oldObj) 
{
	newObj.move(oldObj);
}

template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount, boolean t_IsRawValueType>
static INLINE void swap(SortedArray<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& a, SortedArray<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& b) 
{
	a.swap(b);
}

template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount, boolean t_IsRawValueType>
static INLINE void construct_destroy(SortedArray<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>* newObj, SortedArray<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& oldObj) 
{
	new(newObj) SortedArray<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>();
	newObj->move(oldObj);
	oldObj.~SortedArray<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>();
}

}
