#pragma once

#include "seed.h"
#include <string.h>

namespace Base
{

static INLINE void memoryCopy(void* dest, const void* src, uinteger size)
{
	memcpy(dest, src, size);
}

static INLINE void memoryClear(void* dest, uinteger size)
{
	memset(dest, 0, size);
}

extern byte* allocateMemoryPages(uinteger& realsize, uinteger requestedSize);
extern void freeMemoryPages(void* p);
extern boolean getSystemMemoryStatistic(uinteger* totlaSystemMemBytes, uinteger* freeSystemMemBytes, uinteger* thisProcessMemoryUsedBytes);

}
