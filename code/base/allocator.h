#pragma once

#include "seed.h"

namespace Base
{

class IAllocator
{
public:
	virtual ~IAllocator(){}
	virtual byte* alloc(uinteger size, uint align, uint alignOffset)=0;
	virtual void free(void* p)=0;
	 
	template<class T>
	INLINE T* alloc()
	{
		return alloc(sizeof(T), ALLOCATION_ALIGNMENT(sizeof(T)), 0);
	}
	
	template<class T>
	INLINE T* alloc(uint n)
	{
		return alloc(sizeof(T) * n, ALLOCATION_ALIGNMENT(sizeof(T)), 0);
	}
};

template<class t_Allocator>
INLINE byte* newOverrideImpl(t_Allocator& alc, size_t size)
{
	byte* p = alc.alloc(size + sizeof(IAllocator*), ALLOCATION_ALIGNMENT(size), sizeof(IAllocator*));
	*reinterpret_cast<IAllocator**>(p) = &alc;
	return p + sizeof(IAllocator*);
}

static INLINE void deleteOverrideImpl(void *p)
{
	if(p){
		IAllocator** pa = reinterpret_cast<IAllocator**>(p) - 1;
		IAllocator* a = *pa;
		a->free(pa);
	}
}

template<class t_Allocator>
INLINE byte* newArrayOverrideImpl(t_Allocator& alc, size_t size)
{
	uinteger align = ALLOCATION_ALIGNMENT(size);
	uinteger alignWithoutCountInt = size > sizeof(uint) ? ALLOCATION_ALIGNMENT(size - sizeof(uint)) : 0;

	uinteger offset = sizeof(IAllocator*);
	size += sizeof(IAllocator*);

	if(alignWithoutCountInt > align){
		align = alignWithoutCountInt;
		offset += sizeof(uint);
	}

	byte* p = alc.alloc(size, align, offset);
	*reinterpret_cast<IAllocator**>(p) = &alc;
	return p + sizeof(IAllocator*);
}

static INLINE void deleteArrayOverrideImpl(void *p)
{
	deleteOverrideImpl(p);
}

#ifdef BASE_MEMORY_LEAKS_DEBUG
static void memoryLeaksDebugOnAlloc(void* p);
static void memoryLeaksDebugOnFree(void* p);
#else
static INLINE void memoryLeaksDebugOnAlloc(void* p){}
static INLINE void memoryLeaksDebugOnFree(void* p){}
#endif

static INLINE byte* alignMemoryAddres(void* mem, uinteger align = DEFAULT_IMPLY_ALIGMENT)
{
	uinteger ptr = ((reinterpret_cast<uinteger>(mem) + align - 1) & (~uinteger(align-1)));
	return reinterpret_cast<byte*>(ptr);
}

template<class t_Type>
static INLINE t_Type* alignMemoryAddresAndCast(void* mem)
{
	return (t_Type*)alignMemoryAddres(mem, ALLOCATION_ALIGNMENT(sizeof(t_Type)));
}

class SystemAllocator : public IAllocator
{
public:
	static INLINE byte* _alloc(uinteger size, uint align, uint offset)
	{
		Assert(align >= 1);

		size += sizeof(void*);
		offset += sizeof(void*);

		byte *mem = (byte*)malloc(size + align - 1);
		byte* shiftedMem = alignMemoryAddres(mem + offset, align) - offset;
		*reinterpret_cast<void**>(shiftedMem) = mem;
		shiftedMem += sizeof(void*);

		memoryLeaksDebugOnAlloc(shiftedMem);
		return shiftedMem;
	}

	static INLINE void _free(void* p)
	{
		memoryLeaksDebugOnFree(p);
		::free(reinterpret_cast<void**>(p)[-1]);
	}

	byte* alloc(uinteger size, uint align, uint offset)
	{
		return _alloc(size, align, offset);
	}

	void free(void* p)
	{
		_free(p);
	}
};

extern SystemAllocator gSystemAllocator;

class SystemAllocatorWrapper
{
public:
	struct Data_t {};
	typedef SystemAllocator Allocator_t;

	INLINE SystemAllocatorWrapper(){}
	INLINE SystemAllocatorWrapper(const SystemAllocatorWrapper& other){}
	INLINE SystemAllocatorWrapper& operator = (const SystemAllocatorWrapper& other){ return *this; }
	INLINE SystemAllocatorWrapper(Data_t& ad){}
	INLINE SystemAllocator& allocator() { return gSystemAllocator; }
	INLINE boolean allocatorMaySwap(SystemAllocatorWrapper& other){ return true; }
	INLINE boolean allocatorMayMove(SystemAllocatorWrapper& other){ return true; }
	INLINE boolean allocatorSwap(SystemAllocatorWrapper& other){ return true; }
	INLINE boolean allocatorMove(SystemAllocatorWrapper& other){ return true; }
};

template<class t_Allocator>
class AllocatorReferenceWrapper
{
private:
	t_Allocator* _allocator;
public:
	typedef t_Allocator Data_t;
	typedef t_Allocator Allocator_t;

	INLINE AllocatorReferenceWrapper(const AllocatorReferenceWrapper& other) : _allocator(other._allocator) {}
	INLINE AllocatorReferenceWrapper& operator = (const AllocatorReferenceWrapper& other){ _allocator = other._allocator; return *this; }
	INLINE AllocatorReferenceWrapper(Data_t& alc){ _allocator = &alc; }
	INLINE t_Allocator& allocator() { return *_allocator; }
	INLINE boolean allocatorMaySwap(AllocatorReferenceWrapper& other){ return true; }
	INLINE boolean allocatorMayMove(AllocatorReferenceWrapper& other){ return true; }
	INLINE boolean allocatorSwap(AllocatorReferenceWrapper& other){ swap(_allocator, other._allocator); return true; }
	INLINE boolean allocatorMove(AllocatorReferenceWrapper& other){ move(_allocator, other._allocator); return true; }
};


template<class t_Allocator>
class AllocatorWrapper
{
private:
	t_Allocator _allocator;
public:
	struct Data_t {};
	typedef t_Allocator Allocator_t;

	INLINE AllocatorWrapper() {}
	INLINE AllocatorWrapper(Data_t&) {}
	INLINE t_Allocator& allocator() { return _allocator; }
	INLINE boolean allocatorMaySwap(AllocatorWrapper& other){ return true; }
	INLINE boolean allocatorMayMove(AllocatorWrapper& other){ return true; }
	INLINE boolean allocatorSwap(AllocatorWrapper& other){ return false; }
	INLINE boolean allocatorMove(AllocatorWrapper& other){ return false; }
private:
	INLINE AllocatorWrapper(const AllocatorWrapper& other) {}
	INLINE AllocatorWrapper& operator = (const AllocatorWrapper& other){ return *this; }
};

}
