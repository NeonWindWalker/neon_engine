#pragma once

#include "allocator.h"

#ifdef __ANDROID__

INLINE void *operator new(size_t size)
{
	return Base::newOverrideImpl(Base::gSystemAllocator, size);
}

INLINE void operator delete(void *p)
{
	Base::deleteOverrideImpl(p);
}

INLINE void *operator new[](size_t size)
{
	return Base::newArrayOverrideImpl(Base::gSystemAllocator, size);
}

INLINE void operator delete[](void *p)
{
	Base::deleteArrayOverrideImpl(p);
}

INLINE void *operator new(size_t size, const std::nothrow_t &)
{
	return Base::newOverrideImpl(Base::gSystemAllocator, size);
}

INLINE void operator delete(void *p, const std::nothrow_t &)
{
	Base::deleteOverrideImpl(p);
}

INLINE void *operator new[](size_t size, const std::nothrow_t &)
{
	return Base::newArrayOverrideImpl(Base::gSystemAllocator, size);
}

INLINE void operator delete[](void *p, const std::nothrow_t &)
{
	Base::deleteArrayOverrideImpl(p);
}

#else

INLINE void *operator new(size_t size) throw(std::bad_alloc)
{
	void *p = Base::newOverrideImpl(Base::gSystemAllocator, size);
	if (!p)
		throw std::bad_alloc();
	return p;
}

INLINE void operator delete(void *p) throw()
{
	Base::deleteOverrideImpl(p);
}

INLINE void *operator new[](size_t size) throw(std::bad_alloc)
{
	void *p = Base::newArrayOverrideImpl(Base::gSystemAllocator, size);
	if (!p)
		throw std::bad_alloc();
	return p;
}

INLINE void operator delete[](void *p) throw()
{
	Base::deleteArrayOverrideImpl(p);
}

INLINE void *operator new(size_t size, const std::nothrow_t &) throw()
{
	return Base::newOverrideImpl(Base::gSystemAllocator, size);
}

INLINE void operator delete(void *p, const std::nothrow_t &) throw()
{
	Base::deleteOverrideImpl(p);
}

INLINE void *operator new[](size_t size, const std::nothrow_t &) throw()
{
	return Base::newArrayOverrideImpl(Base::gSystemAllocator, size);
}

INLINE void operator delete[](void *p, const std::nothrow_t &) throw()
{
	Base::deleteArrayOverrideImpl(p);
}

#endif


INLINE void *operator new(size_t size, Base::IAllocator& alc) throw()
{
	return Base::newOverrideImpl(alc, size);
}

INLINE void operator delete(void* p, Base::IAllocator& alc) throw()
{
	Base::deleteOverrideImpl(p);
}

INLINE void *operator new[](size_t size, Base::IAllocator& alc) throw()
{
	return Base::newArrayOverrideImpl(alc, size);
}

INLINE void operator delete[](void* p, Base::IAllocator& alc) throw()
{
	Base::deleteArrayOverrideImpl(p);
}