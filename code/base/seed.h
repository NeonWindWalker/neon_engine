#pragma once

#include "config.h"

#include <stdint.h>
#include <stdlib.h>
//#include <stdarg.h>
//#include <string.h>
#include <assert.h>

#ifdef INSTRUCTIONS_SSE3
#include <pmmintrin.h>
#endif

#ifdef INSTRUCTIONS_SSE2
#include <emmintrin.h>
#endif

#ifdef INSTRUCTIONS_SSE
#include <xmmintrin.h>
#endif

#ifdef INSTRUCTIONS_NEON
#include <arm_neon.h>
#endif

#ifdef _MSC_VER
#include <intrin.h>
#endif

#include <new>

namespace Base
{

typedef int8_t			int8;
typedef int16_t			int16;
typedef int32_t			int32;
typedef int64_t			int64;
typedef uint8_t         uint8;
typedef uint16_t		uint16;
typedef uint32_t		uint32;
typedef uint64_t 		uint64;
typedef unsigned int	boolean;
typedef unsigned int	boolean01;
typedef uint8_t			byte;
typedef unsigned int	uint;
typedef uint8_t			utf8;
typedef uint32_t		character;
typedef uint8_t			uint8_01;
typedef int8_t			int8_11;
typedef uint16_t		uint16_01;
typedef int16_t			int16_11;

#ifdef MEMORY_ADDRES_64BIT
typedef uint64_t		uinteger;
typedef int64_t 		integer;
#else
typedef uint32_t		uinteger;
typedef int32_t 		integer;
#endif

struct MoveInMemoryConstructorExtension
{
	void* userptr;
	MoveInMemoryConstructorExtension() : userptr(0) {}
	MoveInMemoryConstructorExtension(void* p) : userptr(p) {}
	~MoveInMemoryConstructorExtension() {}
};

class Uncopyable
{
public:
	Uncopyable(){}
	~Uncopyable(){}
private:
	Uncopyable(const Uncopyable& other){}
	Uncopyable& operator = (const Uncopyable& other){ return *this; }
};

struct CodePoint
{
	CodePoint() : file(0), line(0), function(0) {}
	CodePoint(const char* inFile, uint inLine, const char* inFunction) : file(inFile), line(inLine), function(inFunction) {}
	const char* file;
	uint line;
	const char* function;
};

#define _CodePoint_ Base::CodePoint( __FILE__ , __LINE__ , __FUNCTION__ )


static INLINE boolean01 boolean01FromBoolean(boolean b)
{
	static_assert(sizeof(boolean) == 4, "invalid boolean size");
	b |= b >> 16;
	b |= b >> 8;
	b |= b >> 4;
	b |= b >> 2;
	b |= b >> 1;
	return b & 1;
}

}

#define USING_BASE_LIB using namespace Base; using Base::uint;  using Base::byte; using Base::boolean;

#include "newdelete.h"





