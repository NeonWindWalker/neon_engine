#pragma once

#include "alghoritms.h"


namespace Base
{

template<class t_Value>
class ConstRange
{
public:
	typedef const t_Value Value_t;

	INLINE ConstRange() : _first(0), _size(0) {}
	INLINE ConstRange(const t_Value* srcFirst, const t_Value* srcEnd) : _first(const_cast<t_Value*>(srcFirst)), _size(srcEnd - srcFirst) {}
	INLINE ConstRange(const t_Value* srcFirst, uinteger srcSize) : _first(const_cast<t_Value*>(srcFirst)), _size(srcSize) {}
	
	INLINE const t_Value* begin()const { return this->_first - 1; }
	INLINE const t_Value* first()const { return this->_first; }
	INLINE const t_Value* last()const { return this->_first + this->_size - 1; }
	INLINE const t_Value* end()const { return this->_first + this->_size; }

	INLINE uint size()const { return this->_size; }
	INLINE uinteger byteSize()const { return this->_size * sizeof(t_Value); }
	INLINE boolean empty()const { return 0 == this->_size; }
	INLINE uint index(const t_Value* i)const { return i - this->_first; }
	INLINE const t_Value& operator [] (uint n)const { return this->_first[n]; }
	INLINE const t_Value& at(uint n)const { return this->_first[n]; }

	INLINE ConstRange afterLeftTrim(uint n)const { return ConstRange(this->_first + n, this->_size - n); }
	INLINE ConstRange afterRightTrim(uint n)const { return ConstRange(this->_first, this->_size - n); }
	INLINE ConstRange leftPart(uint n)const { return ConstRange(this->_first, n); }
	INLINE ConstRange rightPart(uint n)const { return ConstRange(this->end() - n, n); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const Value_t* find(const t_OtherValue& v, const t_Comparator& cmp)const { return Base::find(first(), end(), v, cmp); }
	template<class t_OtherValue>
	INLINE const Value_t* find(const t_OtherValue& v)const { return Base::find(first(), end(), v); }

	template<class t_OtherValue,class t_Comparator>
	INLINE const Value_t* rfind(const t_OtherValue& v, const t_Comparator& cmp)const { return Base::rfind(last(), begin(), v, cmp); }
	template<class t_OtherValue>
	INLINE const Value_t* rfind(const t_OtherValue& v)const { return Base::rfind(last(), begin(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const Value_t* findRange(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::find(first(), end(), v.first(), v.end(), cmp); }
	template<class t_OtherValue>
	INLINE const Value_t* findRange(const ConstRange<t_OtherValue>& v)const { return Base::find(first(), end(), v.first(), v.end()); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const Value_t* rfindRange(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::rfind(last(), begin(), v.last(), v.begin(), cmp); }
	template<class t_OtherValue>
	INLINE const Value_t* rfindRange(const ConstRange<t_OtherValue>& v)const { return Base::rfind(last(), begin(), v.last(), v.begin()); }

	template<class t_OtherValue>
	INLINE boolean contains(const t_OtherValue& v)const { return Base::find(first(), end(), v) != end(); }

	template<class t_OtherValue>
	INLINE boolean containsRange(const ConstRange<t_OtherValue>& v)const { return Base::find(first(), end(), v.first(), v.end()) != end(); }

	template<class t_OtherValue>
	INLINE boolean endsWith(const t_OtherValue& v)const
	{
		return size() && *last() == v;
	}

	template<class t_OtherValue>
	INLINE boolean startsWith(const t_OtherValue& v)const
	{
		return size() && *first() == v;
	}

	template<class t_OtherValue>
	INLINE boolean endsWithRange(const ConstRange<t_OtherValue>& other)const
	{
		return this->size() >= other.size() && !compare(other.first(), other.size(), this->end() - other.size(), other.size());
	}

	template<class t_OtherValue>
	INLINE boolean startsWithRange(const ConstRange<t_OtherValue>& other)const
	{
		return this->size() >= other.size() && !compare(other.first(), other.size(), this->first(), other.size());
	}

	template<class t_RangeContainer, class t_SplitValue, class t_EqualComparator>
	INLINE void split(t_RangeContainer& dest, const t_SplitValue& val, const t_EqualComparator& ecmp){ split(dest, *this, val, ecmp); }
	template<class t_RangeContainer, class t_SplitValue>
	INLINE void split(t_RangeContainer& dest, const t_SplitValue& val) { split(dest, *this, val); }

protected:
	t_Value* _first;
	uint _size;
};

template<class t_Value>
class Range : public ConstRange<t_Value>
{
public:
	typedef t_Value Value_t;

	INLINE Range() {}
	INLINE Range(t_Value* srcFirst, t_Value* srcEnd) : ConstRange<t_Value>(srcFirst, srcEnd) {}
	INLINE Range(t_Value* srcFirst, uinteger srcSize) : ConstRange<t_Value>(srcFirst, srcSize) {}
	
	INLINE t_Value* begin()const { return this->_first - 1; }
	INLINE t_Value* first()const { return this->_first; }
	INLINE t_Value* last()const { return this->_first + this->_size - 1; }
	INLINE t_Value* end()const { return this->_first + this->_size; }

	INLINE t_Value& operator [] (uint n)const { return this->_first[n]; }
	INLINE t_Value& at(uint n)const { return this->_first[n]; }
	
	INLINE Range afterLeftTrim(uint n)const { return Range(this->_first + n, this->_size - n); }
	INLINE Range afterRightTrim(uint n)const { return Range(this->_first, this->_size - n); }
	INLINE Range leftPart(uint n)const { return Range(this->_first, n); }
	INLINE Range rightPart(uint n)const { return Range(this->end() - n, n); }
	
	template<class t_OtherValue, class t_Comparator>
	INLINE Value_t* find(const t_OtherValue& v, const t_Comparator& cmp)const { return Base::find(first(), end(), v, cmp); }
	template<class t_OtherValue>
	INLINE Value_t* find(const t_OtherValue& v)const { return Base::find(first(), end(), v); }

	template<class t_OtherValue,class t_Comparator>
	INLINE Value_t* rfind(const t_OtherValue& v, const t_Comparator& cmp)const { return Base::rfind(last(), begin(), v, cmp); }
	template<class t_OtherValue>
	INLINE Value_t* rfind(const t_OtherValue& v)const { return Base::rfind(last(), begin(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE Value_t* findRange(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::find(first(), end(), v.first(), v.end(), cmp); }
	template<class t_OtherValue>
	INLINE Value_t* findRange(const ConstRange<t_OtherValue>& v)const { return Base::find(first(), end(), v.first(), v.end()); }

	template<class t_OtherValue, class t_Comparator>
	INLINE Value_t* rfindRange(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::rfind(last(), begin(), v.last(), v.begin(), cmp); }
	template<class t_OtherValue>
	INLINE Value_t* rfindRange(const ConstRange<t_OtherValue>& v)const { return Base::rfind(last(), begin(), v.last(), v.begin()); }

	INLINE void sort()const { sort(first(), end()); }
	template<class t_Comparator>
	INLINE void sort(const t_Comparator& cmp)const { sort(first(), end(), cmp); }

	template<class t_RangeContainer, class t_SplitValue, class t_EqualComparator>
	INLINE void split(t_RangeContainer& dest, const t_SplitValue& val, const t_EqualComparator& ecmp){ split(dest, *this, val, ecmp); }
	template<class t_RangeContainer, class t_SplitValue>
	INLINE void split(t_RangeContainer& dest, const t_SplitValue& val) { split(dest, *this, val); }
};


template<typename t_Value>
Range<t_Value> makeRange(t_Value* arr, uint size)
{
	return Range<t_Value>(arr, size);
}

template<typename t_Value>
Range<t_Value> makeRange(t_Value* first, t_Value* end)
{
	return Range<t_Value>(first, end);
}

template<typename t_Value>
ConstRange<t_Value> makeConstRange(const t_Value* arr, uint size)
{
	return ConstRange<t_Value>(arr, size);
}

template<typename t_Value>
ConstRange<t_Value> makeConstRange(const t_Value* first, t_Value* end)
{
	return ConstRange<t_Value>(first, end);
}


template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE uint findGreaterOrEqualInSortedRange(const ConstRange<t_Element>& arr, const t_ValElement& val, const t_Comparator& cmp)
{
	return findGreaterOrEqualInSortedArray(arr.first(), arr.size(), val, cmp);
}

template<typename t_Element, typename t_ValElement>
static INLINE uint findGreaterOrEqualInSortedRange(const ConstRange<t_Element>& arr, const t_ValElement& val)
{
	return findGreaterOrEqualInSortedArray(arr.first(), arr.size(), val);
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE int findLesserOrEqualInSortedRange(const ConstRange<t_Element>& arr, const t_ValElement& val, const t_Comparator& cmp)
{
	return findLesserOrEqualInSortedArray(arr.first(), arr.size(), val, cmp);
}

template<typename t_Element, typename t_ValElement>
static INLINE int findLesserOrEqualInSortedRange(const ConstRange<t_Element>& arr, const t_ValElement& val)
{
	return findLesserOrEqualInSortedArray(arr.first(), arr.size(), val);
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE uint findInSortedRange(const ConstRange<t_Element>& arr, const t_ValElement& val, const t_Comparator& cmp)
{
	return findInSortedArray(arr.first(), arr.size(), val, cmp);
}

template<typename t_Element, typename t_ValElement>
static INLINE uint findInSortedRange(const ConstRange<t_Element>& arr, const t_ValElement& val)
{
	return findInSortedArray(arr.first(), arr.size(), val);
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE Range<t_Element> findEqualRangeInSortedRange(const Range<t_Element>& arr, t_ValElement& val, const t_Comparator& cmp)
{
	t_Element* e;
	t_Element* f = findEqualRangeInSortedArray(e, arr.first(), arr.size(), val, cmp);
	return Range<t_Element>(f, e);
}

template<typename t_Element, typename t_ValElement>
static INLINE Range<t_Element> findEqualRangeInSortedRange(const Range<t_Element>& arr, t_ValElement& val)
{
	t_Element* e;
	t_Element* f = findEqualRangeInSortedArray(e, arr.first(), arr.size(), val);
	return Range<t_Element>(f, e);
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE ConstRange<t_Element> findEqualRangeInSortedRange(const ConstRange<t_Element>& arr, t_ValElement& val, const t_Comparator& cmp)
{
	t_Element* e;
	t_Element* f = findEqualRangeInSortedArray(e, arr.first(), arr.size(), val, cmp);
	return ConstRange<t_Element>(f, e);
}

template<typename t_Element, typename t_ValElement>
static INLINE ConstRange<t_Element> findEqualRangeInSortedRange(const ConstRange<t_Element>& arr, t_ValElement& val)
{
	t_Element* e;
	t_Element* f = findEqualRangeInSortedArray(e, arr.first(), arr.size(), val);
	return ConstRange<t_Element>(f, e);
}

template<typename t_ElementA, typename t_ElementB>
INLINE boolean operator < (const ConstRange<t_ElementA>& a, const ConstRange<t_ElementB>& b){ return compare(a.first(), a.size(), b.first(), b.size()) < 0; }
template<typename t_ElementA, typename t_ElementB>
INLINE boolean operator > (const ConstRange<t_ElementA>& a, const ConstRange<t_ElementB>& b){ return compare(a.first(), a.size(), b.first(), b.size()) > 0; }
template<typename t_ElementA, typename t_ElementB>
INLINE boolean operator <= (const ConstRange<t_ElementA>& a, const ConstRange<t_ElementB>& b){ return compare(a.first(), a.size(), b.first(), b.size()) <= 0; }
template<typename t_ElementA, typename t_ElementB>
INLINE boolean operator >= (const ConstRange<t_ElementA>& a, const ConstRange<t_ElementB>& b){ return compare(a.first(), a.size(), b.first(), b.size()) >= 0; }
template<typename t_ElementA, typename t_ElementB>
INLINE boolean operator == (const ConstRange<t_ElementA>& a, const ConstRange<t_ElementB>& b){ return compare(a.first(), a.size(), b.first(), b.size()) == 0; }
template<typename t_ElementA, typename t_ElementB>
INLINE boolean operator != (const ConstRange<t_ElementA>& a, const ConstRange<t_ElementB>& b){ return compare(a.first(), a.size(), b.first(), b.size()) != 0; }

template<class t_ElementA, class t_ElementB>
class DefaultComparator< const ConstRange<t_ElementA>, const ConstRange<t_ElementB> >
{
public:
	INLINE int operator() (const ConstRange<t_ElementA>& a, const ConstRange<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_ElementB>
class DefaultComparator< const ConstRange<t_ElementA>, const Range<t_ElementB> >
{
public:
	INLINE int operator() (const ConstRange<t_ElementA>& a, const Range<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_ElementB>
class DefaultComparator< const Range<t_ElementA>, const ConstRange<t_ElementB> >
{
public:
	INLINE int operator() (const Range<t_ElementA>& a, const ConstRange<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_ElementB>
class DefaultComparator< const Range<t_ElementA>, const Range<t_ElementB> >
{
public:
	INLINE int operator() (const Range<t_ElementA>& a, const Range<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

}
