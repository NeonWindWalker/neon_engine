#pragma once

#include "memory.h"

namespace Base 
{

struct TempAllocatorPageHeader
{
public:
	TempAllocatorPageHeader* next;
	byte* seek;
	byte* end;

	INLINE void init(uinteger pageSize)
	{
		next = 0;
		setSeekToStart();
		end = seek + pageSize - sizeof(TempAllocatorPageHeader);
	}

	INLINE void setSeekToStart()
	{
		seek = reinterpret_cast<byte*>(this + 1);
	}

	INLINE uinteger usedBytes()const
	{
		return seek - reinterpret_cast<const byte*>(this + 1);
	}

	INLINE uinteger totalBytes()const
	{
		return end - reinterpret_cast<const byte*>(this + 1);
	}

	static TempAllocatorPageHeader* reorderBySizeAndGetUsedSize(uinteger& usedSize, TempAllocatorPageHeader* first);
};

template<uint t_nStaticReservedCount=0>
class TTempAllocator : public IAllocator
{
public:
	INLINE TTempAllocator() 
	{
		this->firstPage.init(sizeof(TempAllocatorPageHeader) + t_nStaticReservedCount);
		this->currentPage = &this->firstPage;
	}

	INLINE ~TTempAllocator()
	{
		free();
	}

private:
	TTempAllocator(const TTempAllocator& other) {}
	void operator = (const TTempAllocator& other) {}

public:

	byte* alloc(uinteger size, uint align, uint offset)
	{
		Assert(align >= 1);

		while(true) 
		{
			uinteger ptr = ((reinterpret_cast<uinteger>(this->currentPage->seek) + offset + align - 1) & (~uinteger(align-1))) - offset;
			if(ptr + size <= reinterpret_cast<uinteger>(this->currentPage->end)){
				this->currentPage->seek = reinterpret_cast<byte*>(ptr + size);
				return reinterpret_cast<byte*>(ptr);
			}
			if(this->currentPage->next)
				this->currentPage = this->currentPage->next;
			else
			{
				uinteger pageSize = sizeof(TempAllocatorPageHeader) + size + align;
				void* pageMem = allocateMemoryPages(pageSize, pageSize);
				TempAllocatorPageHeader* page = new(pageMem) TempAllocatorPageHeader();
				page->init(pageSize);
				this->currentPage->next = page;
				this->currentPage = page;
			}
		}
	}

	void free(void* p){}

	INLINE void free()
	{
		for(TempAllocatorPageHeader * p = this->firstPage.next; p; ){
			TempAllocatorPageHeader* pdel = p;
			p = p->next;
			Base::freeMemoryPages(pdel);
		}
		this->currentPage = &this->firstPage;
		this->firstPage.next = 0;
		this->firstPage.setSeekToStart();
	}

	INLINE void freeTimeCoherenceOptimizedMode()
	{
		uinteger used;
		this->firstPage.next = TempAllocatorPageHeader::reorderBySizeAndGetUsedSize(used, this->firstPage.next);
		used += this->firstPage.usedBytes();

		uinteger s = 0;
		TempAllocatorPageHeader* p = &this->firstPage;
		TempAllocatorPageHeader* plast;
		do{
			s += p->totalBytes();
			p->setSeekToStart();
			plast = p;
			p = p->next;
			if(s >= used)
				break;
		}while(p);

		plast->next = 0;
		this->currentPage = &this->firstPage;

		while(p){
			TempAllocatorPageHeader* pdel = p;
			p = p->next;
			Base::freeMemoryPages(pdel);
		}
	}
protected:

	TempAllocatorPageHeader *currentPage;
	union
	{
		TempAllocatorPageHeader firstPage;
		byte reservedBuffer[sizeof(TempAllocatorPageHeader) + t_nStaticReservedCount];
	};


};

typedef TTempAllocator<0> TempAllocator;

}
