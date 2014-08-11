#include "memory.h"
#include "scalar.h"
#include "threading.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace Base
{

namespace Private
{
	uinteger gMemoryPageSize = 0;

#ifdef __linux__
	static const uinteger gMemoryPageMyOffset = 32;
#endif

#ifdef DEBUG
	volatile int gTotalAllocatedMemoryPages = 0;
#endif
}

byte* allocateMemoryPages(uinteger& realsize, uinteger requestedSize)
{
	Assert(requestedSize);

#if defined(__APPLE__) || defined(__ANDROID__)

	realsize = alignToGreater(requestedSize, 4096);
	return SystemAllocator::_alloc(realsize, 32, 0);

#else
	if(Private::gMemoryPageSize == 0)
	{
#ifdef _WIN32
		SYSTEM_INFO sysInf;
		GetSystemInfo(&sysInf);
		Private::gMemoryPageSize = sysInf.dwAllocationGranularity;
#endif

#ifdef __linux__
		Private::gMemoryPageSize = ::getpagesize();
#endif
	}

#ifdef _WIN32
	realsize = alignToGreater(requestedSize, Private::gMemoryPageSize);
	byte* mem = (byte*)::VirtualAlloc(0, realsize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	Assert(mem);
#endif

#ifdef __linux__
	realsize = alignToGreater(requestedSize + Private::gMemoryPageMyOffset, Base::Private::gMemoryPageSize);
	byte* mem = (byte*)::mmap(0, realsize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	Assert(mem);
	*(uinteger*)mem = realsize;
	realsize -= Private::gMemoryPageMyOffset;
	mem += Private::gMemoryPageMyOffset;
#endif

#ifdef DEBUG
	atomicIncrement(Private::gTotalAllocatedMemoryPages);
#endif

	return mem;
#endif
}

void freeMemoryPages(void* p)
{
#if defined(__APPLE__) || defined(__ANDROID__)

	SystemAllocator::_free(p);

#else

#ifdef _WIN32
	BOOL res = ::VirtualFree(p, 0, MEM_RELEASE);
	Assert(res);
#endif

#ifdef __linux__
	if(!p)
		return;
	byte* mem = (byte*)p - Base::Private::gMemoryPageMyOffset;
	uinteger realsize = *(uinteger*)mem;
	int res = ::munmap(mem, realsize)
	Assert(res == 0);
#endif

#ifdef DEBUG
	atomicDecrement(Private::gTotalAllocatedMemoryPages);
#endif

#endif
}

}
