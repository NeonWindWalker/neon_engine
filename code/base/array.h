#pragma once

#include "range.h"
#include "scalar.h"
#include "memory.h"
#include "alghoritms.h"

namespace Base
{

template<class t_Value>
class DefaultArrayReservedElementsCount
{
public:
	static const uint gcReservedElementsCount = 48 / sizeof(t_Value);
};

template<typename t_Value, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_ReservedCount = DefaultArrayReservedElementsCount<t_Value>::gcReservedElementsCount, boolean t_IsRawValueType = IsRawType<t_Value>::gcIsRawType>
class Array : public ConstRange<t_Value>, public t_AllocatorWrapper
{
public:
	typedef t_Value* Iterator;
	typedef const t_Value* ConstIterator;
	typedef t_Value Value_t;
	typedef t_AllocatorWrapper AllocatorWrapper_t;

	INLINE Array() : _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
	}

	INLINE Array(typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
	}

	INLINE Array(const Array& other) : t_AllocatorWrapper(*(const t_AllocatorWrapper*)&other), _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
		set(other);
	}

protected:
	INLINE void construct_destroy_optimized(t_Value *d, t_Value *s, uint size)
	{
		if(t_IsRawValueType)
		{
			memoryCopy(d, s, size * sizeof(t_Value));
		}
		else
		{
			for(t_Value *d_e = d + size; d != d_e; ++d, ++s)
				construct_destroy(d, *s);
		}
	}
public:

	INLINE void move(Array& other)
	{
		if(t_AllocatorWrapper::allocatorMayMove(other))
		{
			for(t_Value *p = this->_first, *p_e = end(); p != p_e; ++p)
				p->~t_Value();

			if(this->_first != getReservedDataPtr())
				this->allocator().free(this->_first);

			t_AllocatorWrapper::allocatorMove(other);

			this->_size = other._size;
			if(other._first == other.getReservedDataPtr())
			{
				this->_first = getReservedDataPtr();
				construct_destroy_optimized(this->_first, other._first, other._size);
				this->_capasity = t_ReservedCount;
			}
			else
			{
				this->_first = other._first;
				this->_capasity = other._capasity;
			}
			other._first = other.getReservedDataPtr();
			other._size = 0;
			other._capasity = t_ReservedCount;
		}
		else
		{
			set(other);
		}
	}

	INLINE void swap(Array& other)
	{
		if(t_AllocatorWrapper::allocatorSwap(other))
		{
			if(this->_first != this->getReservedDataPtr())
			{
				if(other._first != other.getReservedDataPtr())
				{
					Base::swap(other._first, this->_first);
					Base::swap(other._size, this->_size);
					Base::swap(other._capasity, this->_capasity);
				}
				else
				{
					uint save_size = other._size;

					other._first = this->_first;
					other._size = this->_size;
					other._capasity = this->_capasity;

					this->_first = this->getReservedDataPtr();
					this->_size = save_size;
					this->_capasity = t_ReservedCount;

					construct_destroy_optimized(this->_first, other.getReservedDataPtr(), this->_size);
				}
			}
			else
			{
				if(other._first != other.getReservedDataPtr())
				{
					uint save_size = this->_size;

					this->_first = other._first;
					this->_size = other._size;
					this->_capasity = other._capasity;

					other._first = other.getReservedDataPtr();
					other._size = save_size;
					other._capasity = t_ReservedCount;

					construct_destroy_optimized(other._first, this->getReservedDataPtr(), other._size);
				}
				else
				{
					uint minSize = min(this->_size, other._size);
					for(t_Value *a = this->_first, *a_e = this->_first + minSize, *b = other._first; a != a_e; ++a, ++b)
						Base::swap(*a, *b);

					if(this->_size > minSize)
					{
						construct_destroy_optimized(other._first + minSize, this->_first + minSize, this->_size - minSize); 
					}
					else if(other._size > minSize)
					{
						construct_destroy_optimized(this->_first + minSize, other._first + minSize, other._size - minSize);
					}

					Base::swap(other._size, this->_size);
				}
			}
		}
		else
		{
			if(this->size() <= other.size())
			{
				Array cp(*this);
				*this = other;
				other = cp;
			}
			else
			{
				Array cp(other);
				other = *this;
				*this = cp;
			}
		}
	}

	INLINE Array(const ConstRange<t_Value>& other) 
		: _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
		set(other);
	}

	INLINE Array(const ConstRange<t_Value>& other, typename t_AllocatorWrapper::Data_t& ad) 
		: t_AllocatorWrapper(ad), _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
		set(other);
	}

	INLINE Array(const t_Value* srcFirst, const t_Value* srcEnd) 
		: _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
		set(srcFirst, srcEnd);
	}

	INLINE Array(const t_Value* srcFirst, const t_Value* srcEnd, typename t_AllocatorWrapper::Data_t& ad) 
		: t_AllocatorWrapper(ad), _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
		set(srcFirst, srcEnd);
	}

	INLINE Array(const t_Value* srcFirst, uint srcCount) 
		: _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
		set(srcFirst, srcCount);
	}

	INLINE Array(const t_Value* srcFirst, uint srcCount, typename t_AllocatorWrapper::Data_t& ad) 
		: t_AllocatorWrapper(ad), _capasity(t_ReservedCount)
	{
		this->_first = getReservedDataPtr();
		set(srcFirst, srcCount);
	}

	INLINE Array& operator = (const ConstRange<t_Value>& other)
	{
		set(other);
		return *this;
	}

	INLINE ~Array()
	{
		for(t_Value *p = this->_first, *p_e = end(); p != p_e; ++p)
			p->~t_Value();

		if(this->_first != getReservedDataPtr())
			this->allocator().free(this->_first);
	}

	INLINE Range<t_Value> range() { return Range<t_Value>(this->_first, this->_size); }
	INLINE const ConstRange<t_Value>& range()const { return *this; }

	INLINE t_Value* begin() { return this->_first - 1; }
	INLINE t_Value* first() { return this->_first; }
	INLINE t_Value* last() { return this->_first + this->_size - 1; }
	INLINE t_Value* end() { return this->_first + this->_size; }

	INLINE const t_Value* begin()const { return this->_first - 1; }
	INLINE const t_Value* first()const { return this->_first; }
	INLINE const t_Value* last()const { return this->_first + this->_size - 1; }
	INLINE const t_Value* end()const { return this->_first + this->_size; }

	INLINE t_Value& operator [] (uint n) { return this->_first[n]; }
	INLINE t_Value& at(uint n) { return this->_first[n]; }

	INLINE const t_Value& operator [] (uint n)const { return this->_first[n]; }
	INLINE const t_Value& at(uint n)const { return this->_first[n]; }

	INLINE uint indexOf(Iterator it) { return it - first(); }
	INLINE uint indexOf(ConstIterator it) { return it - first(); }

	INLINE Range<t_Value> afterLeftTrim(uint n) { return reinterpret_cast<Range<t_Value>*>((ConstRange<t_Value>*)this)->afterLeftTrim(n); }
	INLINE Range<t_Value> afterRightTrim(uint n) { return reinterpret_cast<Range<t_Value>*>((ConstRange<t_Value>*)this)->afterRightTrim(n); }
	INLINE Range<t_Value> leftPart(uint n) { return reinterpret_cast<Range<t_Value>*>((ConstRange<t_Value>*)this)->leftPart(n); }
	INLINE Range<t_Value> rightPart(uint n) { return reinterpret_cast<Range<t_Value>*>((ConstRange<t_Value>*)this)->rightPart(n); }

	INLINE ConstRange<t_Value> afterLeftTrim(uint n)const { return ConstRange<t_Value>::afterLeftTrim(n); }
	INLINE ConstRange<t_Value> afterRightTrim(uint n)const { return ConstRange<t_Value>::afterRightTrim(n); }
	INLINE ConstRange<t_Value> leftPart(uint n)const { return ConstRange<t_Value>::leftPart(n); }
	INLINE ConstRange<t_Value> rightPart(uint n)const { return ConstRange<t_Value>::rightPart(n); }

	template<class t_OtherValue, class t_Comparator>
	INLINE t_Value* find(const t_OtherValue& v, const t_Comparator& cmp) { return Base::find(first(), end(), v, cmp); }
	template<class t_OtherValue>
	INLINE t_Value* find(const t_OtherValue& v) { return Base::find(first(), end(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE t_Value* rfind(const t_OtherValue& v, const t_Comparator& cmp) { return Base::rfind(last(), begin(), v, cmp); }
	template<class t_OtherValue>
	INLINE t_Value* rfind(const t_OtherValue& v) { return Base::rfind(last(), begin(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE t_Value* findRange(const Range<t_OtherValue>& v, const t_Comparator& cmp) { return Base::find(first(), end(), v.first(), v.end(), cmp); }
	template<class t_OtherValue>
	INLINE t_Value* findRange(const Range<t_OtherValue>& v) { return Base::find(first(), end(), v.first(), v.end()); }

	template<class t_OtherValue, class t_Comparator>
	INLINE t_Value* rfindRange(const Range<t_OtherValue>& v, const t_Comparator& cmp) { return Base::rfind(last(), begin(), v.last(), v.begin(), cmp); }
	template<class t_OtherValue>
	INLINE t_Value* rfindRange(const Range<t_OtherValue>& v) { return Base::rfind(last(), begin(), v.last(), v.begin()); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const t_Value* find(const t_OtherValue& v, const t_Comparator& cmp)const { return Base::find(first(), end(), v, cmp); }
	template<class t_OtherValue>
	INLINE const t_Value* find(const t_OtherValue& v)const { return Base::find(first(), end(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const t_Value* rfind(const t_OtherValue& v, const t_Comparator& cmp)const { return Base::rfind(last(), begin(), v, cmp); }
	template<class t_OtherValue>
	INLINE const t_Value* rfind(const t_OtherValue& v)const { return Base::rfind(last(), begin(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const t_Value* findRange(const Range<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::find(first(), end(), v.first(), v.end(), cmp); }
	template<class t_OtherValue>
	INLINE const t_Value* findRange(const Range<t_OtherValue>& v)const { return Base::find(first(), end(), v.first(), v.end()); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const t_Value* rfindRange(const Range<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::rfind(last(), begin(), v.last(), v.begin(), cmp); }
	template<class t_OtherValue>
	INLINE const t_Value* rfindRange(const Range<t_OtherValue>& v)const { return Base::rfind(last(), begin(), v.last(), v.begin()); }

	INLINE void sort() { Base::sort(first(), end()); }

	template<class t_Comparator>
	INLINE void sort(const t_Comparator& cmp) { Base::sort(first(), end(), cmp); }

	INLINE void reverse() { Base::reverse(first(), last()); }
	INLINE uint capasity()const { return _capasity; }
	INLINE void clear(uint minimalReserveCapasity = t_ReservedCount) { resize(0, minimalReserveCapasity); }
	INLINE void clearSaveReserve() { resize(0, _capasity); }
	INLINE void forceSetSize(uint s) { this->_size = s; }

	void resize(uint newSize, uint newCapasity)
	{
		Assert(newCapasity >= newSize);

		newCapasity = newCapasity <= t_ReservedCount ? t_ReservedCount : saturate(max(newCapasity, (t_ReservedCount<<1)));

		if(newCapasity == this->_capasity)
		{
			if(newSize > this->_size)
			{
				for(t_Value *d = end(), *d_e = this->_first + newSize; d != d_e; ++d)
					new(d) t_Value();
			}
			else
			{
				for(t_Value *d = this->_first + newSize, *d_e = end(); d != d_e; ++d)
					d->~t_Value();
			}
			this->_size = newSize;
			Assert((getReservedDataPtr() == this->_first) == _capasity <= t_ReservedCount);
			return;
		}

		t_Value* newdata;
		if(newCapasity <= t_ReservedCount)
		{
			newdata = getReservedDataPtr();
			Assert(this->_first != newdata);
		}
		else
		{
			newdata = reinterpret_cast<t_Value*>(this->allocator().alloc(sizeof(t_Value) * newCapasity, ALLOCATION_ALIGNMENT(sizeof(t_Value)), 0));
		}

		construct_destroy_optimized(newdata, this->_first, min(this->_size, newSize));
		
		if(newSize > this->_size)
		{
			for(t_Value *d = newdata + this->_size, *d_e = newdata + newSize; d != d_e; ++d)
				new(d) t_Value();
		}
		else
		{
			for(t_Value *d = this->_first + newSize, *d_e = this->_first + this->_size; d != d_e; ++d)
				d->~t_Value();
		}

		if(this->_first != getReservedDataPtr())
			this->allocator().free(this->_first);

		this->_first = newdata;
		this->_size = newSize;
		this->_capasity = newCapasity;
		Assert((getReservedDataPtr() == this->_first) == _capasity <= t_ReservedCount);
	}

	void resize(uint newSize)
	{
		resize(newSize, max(newSize, this->_capasity));
	}

	void resizeOptimal(uint newSize)
	{
		resize(newSize, newSize);
	}

	void resizeOptimal()
	{
		resizeOptimal(this->_size);
	}

protected:
	void realloc_impl(t_Value *newdata, uint newcapasity)
	{
		Assert(newdata != this->_first);
		Assert(newcapasity != this->_capasity);

		construct_destroy_optimized(newdata, this->_first, this->_size);

		if(this->_first != getReservedDataPtr())
			this->allocator().free(this->_first);

		this->_first = newdata;
		this->_capasity = newcapasity;
		Assert((getReservedDataPtr() == this->_first) == _capasity <= t_ReservedCount);
	}

public:
	void reserve(uint s)
	{
		Assert(_capasity >= t_ReservedCount);
		if(s <= _capasity)
			return;

		uint newcapasity = saturate(max(s, (t_ReservedCount<<1)));
		t_Value * newdata = reinterpret_cast<t_Value*>(this->allocator().alloc(sizeof(t_Value) * newcapasity, ALLOCATION_ALIGNMENT(sizeof(t_Value)), 0));

		realloc_impl(newdata, newcapasity);
	}

	void mayrealloc()
	{
		Assert(this->_size <= this->_capasity);

		if((this->_size<<2) > this->_capasity)
			return;

		uint newcapasity;
		t_Value * newdata;
		if(this->_size <= t_ReservedCount)
		{
			newdata = getReservedDataPtr();
			if(this->_first == newdata)
				return;
			newcapasity = t_ReservedCount;
		}
		else
		{
			newcapasity = saturate(max(this->_size, (t_ReservedCount<<1)));
			newdata = reinterpret_cast<t_Value*>(this->allocator().alloc(sizeof(t_Value) * newcapasity, ALLOCATION_ALIGNMENT(sizeof(t_Value)), 0));
		}

		realloc_impl(newdata, newcapasity);
	}

	INLINE void set(const ConstRange<t_Value>& other)
	{
		uint s = other.size();
		clear(s);
		fpush(other.first(), s);
	}

	INLINE void set(const t_Value* arrFirst, uint arrCount)
	{
		clear(arrCount);
		fpush(arrFirst, arrCount);
	}

	INLINE void set(const t_Value* arrFirst, const t_Value* arrEnd)
	{
		uint s = arrEnd - arrFirst;
		clear(s);
		fpush(arrFirst, s);
	}

	INLINE void finsertUnconstructed(Iterator first, uint count)
	{
		if(!count)
			return;

		if(this->last() != first)
		{
			Iterator s = end();
			Iterator d = s + count;
			Iterator e = first;
			do{
				-- s; -- d;
				construct_destroy(d, *s);
			}while(s != e);
		}
		this->_size += count;
	}

	INLINE void insertUnconstructed(uint position, uint count)
	{
		if(!count)
			return;

		reserve(this->_size + count);

		if(this->_size != position)
		{
			Iterator s = end();
			Iterator d = s + count;
			Iterator e = this->_first + position;
			do{
				-- s; -- d;
				construct_destroy(d, *s);
			}while(s != e);
		}
		this->_size += count;
	}

	INLINE Iterator finsert(Iterator it, const t_Value& v)
	{
		finsertUnconstructed(it, 1);
		return new (it) t_Value(v);;
	}

	INLINE Iterator finsert(uint position, const t_Value& v)
	{
		finsertUnconstructed(position, 1);
		return new (this->_first + position) t_Value(v);
	}

	INLINE Iterator finsert(Iterator position, const t_Value* first, uint count)
	{
		finsertUnconstructed(position, count);
		t_Value * dest = position;
		for(const t_Value* i = first, *i_e = first + count; i != i_e; ++dest, ++i)
			new (dest) t_Value(*i);
		return this->_first + position;
	}

	INLINE Iterator finsert(Iterator position, const t_Value* first, const t_Value* end)
	{
		finsertUnconstructed(position, end - first);
		t_Value * dest = position;
		for(const t_Value* i = first; i != end; ++dest, ++i)
			new (dest) t_Value(*i);
		return this->_first + position;
	}

	INLINE Iterator finsert(Iterator position, const ConstRange<t_Value>& r)
	{
		return finsert(position, r.first(), r.size());
	}

	INLINE Iterator insert(uint position, const t_Value& v)
	{
		insertUnconstructed(position, 1);
		return new (this->_first + position) t_Value(v);
	}

	INLINE Iterator insert(uint position, const t_Value* first, uint count)
	{
		insertUnconstructed(position, count);
		t_Value * dest = this->_first + position;
		for(const t_Value* i = first, *i_e = first + count; i != i_e; ++dest, ++i)
			new (dest) t_Value(*i);
		return this->_first + position;
	}

	INLINE Iterator insert(uint position, const t_Value* first, const t_Value* end)
	{
		insertUnconstructed(position, end - first);
		t_Value * dest = this->_first + position;
		for(const t_Value* i = first; i != end; ++dest, ++i)
			new (dest) t_Value(*i);
		return this->_first + position;
	}

	INLINE Iterator insert(uint position, const ConstRange<t_Value>& r)
	{
		return insert(position, r.first(), r.size());
	}

	INLINE Iterator finsertDefault(Iterator it)
	{
		finsertUnconstructed(it, 1);
		return new (it) t_Value();
	}

	INLINE Iterator finsertDefault(uint position)
	{
		finsertUnconstructed(position, 1);
		return new (this->_first + position) t_Value();
	}

	INLINE Iterator finsertDefault(Iterator it, uint count)
	{
		finsertUnconstructed(it, count);
		for(const t_Value* i = it,*i_end = it + count; i != i_end; ++i_end)
			new (i) t_Value();
		return it;
	}

	INLINE Iterator finsertDefault(uint position, uint count)
	{
		finsertUnconstructed(position, count);
		for(const t_Value* i = this->_first + position, *i_end = i + count; i != i_end; ++i_end)
			new (i) t_Value();
		return this->_first + position;
	}

	INLINE Iterator insertDefault(uint position)
	{
		insertUnconstructed(position, 1);
		return new (this->_first + position) t_Value();
	}

	INLINE Iterator insertDefault(uint position, uint count)
	{
		insertUnconstructed(position, count);
		for(const t_Value* i = this->_first + position, *i_end = i + count; i != i_end; ++i_end)
			new (i) t_Value();
		return this->_first + position;
	}

	INLINE void fpush(const t_Value* first, uint count)
	{
		t_Value* dest = end();

		if(t_IsRawValueType)
		{
			memoryCopy(dest, first, count * sizeof(t_Value));
		}
		else
		{
			for(const t_Value* i = first, *i_e = first + count; i != i_e; ++dest, ++i)
				new (dest) t_Value(*i);
		}
		
		this->_size += count;
	}

	INLINE void fpush(const t_Value* first, const t_Value* end)
	{
		t_Value* dest = end();
		for(const t_Value* i = first; i != end; ++dest, ++i)
			new (dest) t_Value(*i);
		this->_size += end - first;
	}

	INLINE void fpush(const ConstRange<t_Value>& r)
	{
		fpush(r.first(), r.size());
	}

	INLINE Iterator fpush(const t_Value& v)
	{
		t_Value* ret = new (end()) t_Value(v);
		++ this->_size;
		return ret;
	}

	INLINE void push(const t_Value* first, uint count)
	{
		reserve(this->_size + count);
		t_Value * dest = end();
		for(const t_Value* i = first, *i_e = first + count; i != i_e; ++dest, ++i)
			new (dest) t_Value(*i);
		this->_size += count;
	}

	INLINE void push(const t_Value* first, const t_Value* end)
	{
		uint count = end - first;
		reserve(this->_size + count);
		t_Value * dest = this->end();
		for(const t_Value* i = first; i != end; ++dest, ++i)
			new (dest) t_Value(*i);
		this->_size += count;
	}

	INLINE void push(const ConstRange<t_Value>& r)
	{
		push(r.first(), r.size());
	}

	INLINE Iterator push(const t_Value& v)
	{
		reserve(this->_size + 1);
		t_Value* ret = new (end())  t_Value(v);
		++ this->_size;
		return ret;
	}

	INLINE Iterator push()
	{
		reserve(this->_size + 1);
		t_Value* ret = new (end())  t_Value();
		++ this->_size;
		return ret;
	}

	INLINE Iterator fpushDefault()
	{
		t_Value* ret = new (end()) t_Value();
		++ this->_size;
		return ret;
	}

	INLINE Iterator fpushDefault(uint count)
	{
		t_Value* ret = end();
		for(t_Value* p = ret, *p_e = p + count; p != p_e; ++p)
			new (p) t_Value();

		this->_size += count;
		return ret;
	}

	INLINE Iterator pushDefault()
	{
		reserve(this->_size + 1);
		return fpushDefault();
	}

	INLINE Iterator pushDefault(uint count)
	{
		reserve(this->_size + count);
		return fpushDefault(count);
	}

	INLINE void ferase(Iterator infirst, Iterator inend)
	{
		for(Iterator dst = infirst, src = inend; src != end(); ++src, ++dst)
			*dst = *src;
		
		Iterator oldend = end();
		this->_size -= inend - infirst;

		for(Iterator dst = end(); dst != oldend; ++dst)
			dst->~t_Value();
	}

	INLINE void ferase(Iterator first, uint count)
	{
		for(Iterator dst = first, src = first + count; src != end(); ++src, ++dst)
			*dst = *src;

		Iterator oldend = end();
		this->_size -= count;

		for(Iterator dst = end(); dst != oldend; ++dst)
			dst->~t_Value();
	}

	INLINE void ferase(const Range<t_Value>& r)
	{
		ferase(r.first(), r.size());
	}

	INLINE void ferase(uint position)
	{
		ferase(this->_first + position, (uint)1);
	}

	INLINE void ferase(int position)
	{
		ferase(this->_first + position, (uint)1);
	}

	INLINE void ferase(Iterator it)
	{
		ferase(it, 1);
	}

	INLINE void erase(uint position, uint count)
	{
		ferase(this->_first + position, this->_first + position + count);
		mayrealloc();
	}

	INLINE void erase(Iterator first, Iterator end)
	{
		ferase(first, end);
		mayrealloc();
	}

	INLINE void erase(Iterator first, uint count)
	{
		ferase(first, count);
		mayrealloc();
	}

	INLINE void erase(uint position)
	{
		erase(position, 1);
	}

	INLINE void erase(Iterator first)
	{
		erase(first, 1);
	}

	INLINE void fxerase(Iterator position)
	{
		position->~t_Value();
		-- this->_size;
		if(end() != position){
			new (position) t_Value(*end());
			end()->~t_Value();
		}
	}

	INLINE void xerase(uint position)
	{
		fxerase(first() + position);
		mayrealloc();
	}

	INLINE Iterator ferase_retNext(Iterator it)
	{
		ferase(it);
		return it;
	}

	INLINE Iterator ferase_retPrev(Iterator it)
	{
		ferase(it);
		return it - 1;
	}

	INLINE Iterator erase_retNext(Iterator it)
	{
		uint ind = indexOf(it);
		erase(ind);
		return first() + ind;
	}

	INLINE Iterator erase_retPrev(Iterator it)
	{
		uint ind = indexOf(it);
		erase(ind);
		return first() + ind - 1;
	}


	INLINE void fpop(t_Value& v)
	{
		--this->_size;
		v = *end();
		end()->~t_Value();
	}

	INLINE void pop(t_Value& v)
	{
		fpop(v);
		mayrealloc();
	}

	INLINE void fpop()
	{
		--this->_size;
		end()->~t_Value();
	}

	INLINE t_Value frpop()
	{
		t_Value v;
		fpop(v);
		return v;
	}
	
	INLINE void pop()
	{
		fpop();
		mayrealloc();
	}

	INLINE t_Value popr()
	{
		t_Value v;
		pop(v);
		return v;
	}

	INLINE t_Value fpopr()
	{
		t_Value v;
		fpop(v);
		return v;
	}

	INLINE void fpopn(uint count)
	{
		for(t_Value *e = end(), *p = e - count; p != e; ++p)
			p->~t_Value();
		this->_size -= count;
	}

	INLINE void popn(uint count)
	{
		fpopn(count);
		mayrealloc();
	}

	INLINE Array& operator << (const t_Value& x)
	{
		push(x);
		return *this;
	}

	INLINE Array& operator << (const ConstRange<t_Value>& r)
	{
		push(r);
		return *this;
	}

protected:
	union
	{
		uint _capasity;
		struct
		{
			byte _reservedBuffer[t_ReservedCount ? sizeof(uint) + t_ReservedCount * sizeof(t_Value) + ALLOCATION_ALIGNMENT(sizeof(t_Value)) - DEFAULT_IMPLY_ALIGMENT : sizeof(uint)];
		};
	};
	
	INLINE byte* getReservedPtr()
	{
		if(t_ReservedCount)
		{
			uinteger a = ALLOCATION_ALIGNMENT(sizeof(t_Value));
			return reinterpret_cast<byte*>(alignToGreaterFast(reinterpret_cast<uinteger>(this->_reservedBuffer + sizeof(uint)), a));
		}
		else
		{
			return this->_reservedBuffer + sizeof(uint);
		}
	}

	INLINE t_Value* getReservedDataPtr()
	{
		return reinterpret_cast<t_Value*>(getReservedPtr());
	}

	INLINE const t_Value* getReservedDataPtr()const
	{
		return reinterpret_cast<const t_Value*>(getReservedPtr());
	}
};

template<class t_Value, class t_AllocatorWrapper, uint t_ReservedCount, class t_ElementB>
class DefaultComparator< const Array<t_Value, t_AllocatorWrapper, t_ReservedCount>, const ConstRange<t_ElementB> >
{
public:
	INLINE int operator() (const Array<t_Value, t_AllocatorWrapper, t_ReservedCount>& a, const ConstRange<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
class DefaultComparator<const ConstRange<t_ElementA>, const Array<t_Value, t_AllocatorWrapper, t_ReservedCount> >
{
public:
	INLINE int operator() (const ConstRange<t_ElementA>& a, const Array<t_Value, t_AllocatorWrapper, t_ReservedCount>& b)const{ return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_Value, class t_AllocatorWrapper, uint t_ReservedCount, class t_ElementB>
class DefaultComparator< const Array<t_Value, t_AllocatorWrapper, t_ReservedCount>, const Range<t_ElementB> >
{
public:
	INLINE int operator() (const Array<t_Value, t_AllocatorWrapper, t_ReservedCount>& a, const Range<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
class DefaultComparator<const Range<t_ElementA>, const Array<t_Value, t_AllocatorWrapper, t_ReservedCount> >
{
public:
	INLINE int operator() (const Range<t_ElementA>& a, const Array<t_Value, t_AllocatorWrapper, t_ReservedCount>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ValueA, class t_AllocatorWrapperA, uint t_ReservedCountA, class t_ValueB, class t_AllocatorWrapperB, uint t_ReservedCountB >
class DefaultComparator< const Array<t_ValueA, t_AllocatorWrapperA, t_ReservedCountA >, const Array<t_ValueB, t_AllocatorWrapperB, t_ReservedCountB > >
{
public:
	INLINE int operator() (const Array<t_ValueA, t_AllocatorWrapperA, t_ReservedCountA>& a, const Array<t_ValueB, t_AllocatorWrapperB, t_ReservedCountB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};


template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount, boolean t_IsRawValueType>
static INLINE void move(Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& newObj, Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& oldObj) 
{
	newObj.move(oldObj);
}

template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount, boolean t_IsRawValueType>
static INLINE void swap(Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& a, Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& b) 
{
	a.swap(b);
}

template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount, boolean t_IsRawValueType>
static INLINE void construct_destroy(Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>* newObj, Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>& oldObj) 
{
	new(newObj) Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>();
	newObj->move(oldObj);
	oldObj.~Array<t_Value, t_AllocatorWrapper, t_ReservedCount, t_IsRawValueType>();
}

}
