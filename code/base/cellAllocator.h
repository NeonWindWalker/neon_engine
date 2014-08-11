#pragma once

#include "seed.h"
#include "list.h"
#include "alghoritms.h"

namespace Base
{
template<class t_Value>
class DefaultCellDefragmentedAllocatorCellCount32Scale
{
public:
	static const uint gcCellCount32Scale = 1;
};

template<class t_DataType, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_CellCount32Scale = DefaultCellDefragmentedAllocatorCellCount32Scale<t_DataType>::gcCellCount32Scale>
class TCellDefragmentAllocator : public IAllocator, public t_AllocatorWrapper
{
protected:
	struct Page;

	struct DECLARE_ALIGN(32) Page
	{
		static const uint gcHeaderSize = ALIGN_NUMBER(2 * sizeof(void*), ALLOCATION_ALIGNMENT(sizeof(t_DataType)));
		static const uint gcCellSize = sizeof(t_DataType) + gcHeaderSize;

		byte cell[t_CellCount32Scale * 32][gcCellSize];
		uint32 useU32[t_CellCount32Scale];
		ListElement<Page> lel;

		INLINE void setUse(uint n){ useU32[n >> 5] |= 1 << (n & 31); }
		INLINE void clearUse(uint n){ useU32[n >> 5] &= ~(1 << (n & 31)); }
		INLINE boolean isUse(uint n)const{ return (useU32[n >> 5] >> (n & 31)) & 1; }

		INLINE int findUsedCellRight()const
		{
			for(int i32 = t_CellCount32Scale - 1; i32 >= 0; --i32)
			{
				uint32 Bits = useU32[i32];
				if(Bits == 0)
					continue;
				int n = hiOneBit(Bits) + i32 * 32;
				Assert(isUse(n));
				return n;
			}
			return -1;
		}

		INLINE uint findFreeCellLeft()const
		{
			for(uint i32=0; i32 < t_CellCount32Scale; ++i32)
			{
				uint32 Bits = useU32[i32];
				if(Bits == 0xffffffff)
					continue;
				uint n = lowZeroBit(Bits) + i32 * 32;
				Assert(!isUse(n));
				return n;
			}
			return t_CellCount32Scale * 32;
		}

		INLINE uint findUsedCellLeftFrom(uint start)const
		{
			start ++;
			uint base = start>>5;
			uint sh = start & 31;
			uint32 Bits = useU32[start>>5] >> sh;
			uint n = lowOneBit(Bits) + sh;
			if(n < 32)
			{
				n += base * 32;
				Assert(isUse(n));
				return n;
			}

			for(uint i32 = base + 1; i32 < t_CellCount32Scale; ++i32)
			{
				uint32 Bits = useU32[i32];
				if(Bits == 0)
					continue;
				uint n = lowOneBit(Bits) + i32 * 32;
				Assert(isUse(n));
				return n;
			}
			return t_CellCount32Scale * 32;
		}

		INLINE t_DataType* get(uint n)
		{
			return reinterpret_cast<t_DataType*>(cell[n] + gcHeaderSize);
		}

		INLINE Page() : lel(this)
		{
			for(uint i=0; i < t_CellCount32Scale; ++i)
				useU32[i] = 0;
		}

		INLINE ~Page()
		{
			lel.erase();
		}

		INLINE byte* alloc(uinteger size, uinteger align, uinteger backAlignumOffset)
		{
			uint i = findFreeCellLeft();
			Assert(i < t_CellCount32Scale * 32);
			Assert(align <= gcHeaderSize);
			Assert(backAlignumOffset <= gcHeaderSize);
			byte* mem = cell[i];
			byte* p = mem + gcHeaderSize - backAlignumOffset;
			Assert(p + size <= mem + gcCellSize);
			setUse(i);
			return p;
		}

		INLINE void free(const void * pincell)
		{
			Assert(reinterpret_cast<uinteger>(pincell) >= reinterpret_cast<uinteger>(cell[0]));
			uint n = uint(reinterpret_cast<uinteger>(pincell) - reinterpret_cast<uinteger>(cell[0])) / gcCellSize;
			Assert(isUse(n));
			clearUse(n);
		}

		INLINE boolean isFull()const
		{
			for(uint i=0; i < t_CellCount32Scale; ++i)
				if((~this->useU32[i]) != 0)
					return false;	
			return true;
		}

		INLINE boolean isFree()const
		{
			for(uint i=0; i < t_CellCount32Scale; ++i)
				if(this->useU32[i] != 0)
					return false;
			return true;
		}
	};

	ListRoot<Page> freePages;
	ListRoot<Page> fullPages;
	
private:

	void operator = (const TCellDefragmentAllocator& other) {}
	TCellDefragmentAllocator(const TCellDefragmentAllocator& other) : t_AllocatorWrapper(other) {}

public:
	typedef t_AllocatorWrapper AllocatorWrapper_t;

	TCellDefragmentAllocator() {}
	TCellDefragmentAllocator(typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad) {}
	
	~TCellDefragmentAllocator()
	{
		uint anallocated = 0;
		for(typename ListRoot<Page>::Iterator e = this->freePages.first(); e != this->freePages.end(); )
		{
			Page* dl = &*e;
			++e;
			if(!dl->isFree())
			{
				for(uint bt = 0; bt < t_CellCount32Scale * 32; ++bt)
					if(dl->isUse(bt))
						anallocated ++;
			}
				
			delete dl;
		}

		for(typename ListRoot<Page>::Iterator e = this->fullPages.first(); e != this->fullPages.end(); )
		{
			Page* dl = &*e;
			++e;
			delete dl;
			anallocated += t_CellCount32Scale * 32;
		}

		Assert(anallocated == 0);
		//if(anallocated)
		//	logError<<"anallocated elements unallocation "<< anallocated;
	}

	byte* alloc(uinteger size, uint align, uint backAlignumOffset)
	{
		if(this->freePages.empty())
		{
			Page* page = new(this->allocator()) Page();
			freePages.insert(page->lel);
		}
		Page* page = &*this->freePages.first();

		byte* ptr = page->alloc(size + sizeof(Page*), align, backAlignumOffset + sizeof(Page*));
		*reinterpret_cast<Page**>(ptr) = page;
		if(page->isFull())
		{
			page->lel.erase();
			this->fullPages.insert(page->lel);
		}
		return reinterpret_cast<byte*>(ptr + sizeof(Page*));
	}

	virtual void free(void* p)
	{
		Page* page = reinterpret_cast<Page**>(p)[-1];
		boolean old_full = page->isFull();
		page->free(p);
		if(old_full)
		{
			page->lel.erase();
			this->freePages.insert(page->lel);
		}
		if(page->isFree() && this->freePages.first() != this->freePages.last())// 2 free pages
			delete page;
	}

	void defragment()
	{
		for(typename ListRoot<Page>::Iterator pgi = this->freePages.last(); pgi != this->freePages.begin(); --pgi)
		{
			Page* page = &*pgi;
			int bt;
			while ((bt = page->findUsedCellRight()) >= 0)
			{
				Page* freepage = &*this->freePages.first();

				if(freepage == page)
					return;

				byte* p = freepage->alloc(sizeof(t_DataType) + sizeof(Page*) + sizeof(IAllocator*), ALLOCATION_ALIGNMENT(sizeof(t_DataType)), sizeof(Page*) + sizeof(IAllocator*));
				*reinterpret_cast<Page**>(p) = freepage;
				*reinterpret_cast<IAllocator**>(p + sizeof(IAllocator*)) = this;
				t_DataType* obj = page->get(bt);
				construct_destroy((t_DataType*)(p + sizeof(Page*) + sizeof(IAllocator*)), *obj);
				page->clearUse(bt);

				if(freepage->isFull()){
					freepage->lel.erase();
					this->fullPages.insert(freepage->lel);
				}
			}
		}

		for(typename ListRoot<Page>::Iterator e = this->freePages.first(); e != this->freePages.end(); )
		{
			Page* dl = &*e;
			++e;
			if(dl->isFree())
				delete dl;
		}
	}

	template<class t_Functor>
	INLINE void forAll(t_Functor& fun)
	{
		for(typename ListRoot<Page>::Iterator e = this->fullPages.first(); e != this->fullPages.end(); ++e)
			for(uint cellindex = 0; cellindex < t_CellCount32Scale * 32; ++cellindex)
				if(!fun(e->get(cellindex)))return;

		for(typename ListRoot<Page>::Iterator e = this->freePages.first(); e != this->freePages.end(); ++e)
			for(uint cellindex = e->findUsedCellLeftFrom(0); cellindex < t_CellCount32Scale * 32; cellindex = e->findUsedCellLeftFrom(cellindex+1))
			{
				if(!fun(e->get(cellindex)))
					return;
				if(cellindex + 1 >= t_CellCount32Scale * 32)
					break;
			}
	}
};

}
