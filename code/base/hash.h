#pragma once

#include "seed.h"

namespace Base
{

typedef uint32 Hash;

template<class t_Val>
class DefaultHashGen
{
public:
	INLINE Hash operator () (const t_Val& key)const
	{
		uint32 a = (uint32)key;
		a = (a+0x7ed55d16) + (a<<12);
		a = (a^0xc761c23c) ^ (a>>19);
		a = (a+0x165667b1) + (a<<5);
		a = (a+0xd3a2646c) ^ (a<<9);
		a = (a+0xfd7046c5) + (a<<3);
		a = (a^0xb55a4f09) ^ (a>>16);
		return a;
	}
};

template<>
class DefaultHashGen<const int8>
{
public:
	INLINE Hash operator () (const int8& key)const { return uint32(key) * uint32(0x9E3779B1); }
};

template<>
class DefaultHashGen<const uint8>
{
public:
	INLINE Hash operator () (const int8& key)const { return uint32(key) * uint32(0x9E3779B1); }
};

template<>
class DefaultHashGen<const uint64>
{
public:
	INLINE Hash operator () (const uint64& k)const
	{
		uint64 key = k;
		key = (~key) + (key << 18); // key = (key << 18) - key - 1;
		key = key ^ (key >> 31);
		key = key * 21; // key = (key + (key << 2)) + (key << 4);
		key = key ^ (key >> 11);
		key = key + (key << 6);
		key = key ^ (key >> 22);
		return (Hash)key;
	}
};

template<>
class DefaultHashGen<const int64>
{
public:
	INLINE Hash operator () (const int64& k)const
	{
		return DefaultHashGen<const uint64>()((uint64)k);
	}
};

template<>
class DefaultHashGen<const float>
{
public:
	INLINE Hash operator () (const float& key)const
	{
		return DefaultHashGen<const uint32>()(*reinterpret_cast<const uint32*>(&key));
	}
};

template<>
class DefaultHashGen<const double>
{
public:
	INLINE Hash operator () (const double& key)const
	{
		return DefaultHashGen<const uint64>()(*reinterpret_cast<const uint64*>(&key));
	}
};

template<class t_Val>
class DefaultHashGen<t_Val*>
{
public:
	INLINE Hash operator () (t_Val* key)const { return DefaultHashGen<uinteger>()(reinterpret_cast<uinteger>(key)); }
};

template<class T>
struct HashGenT
{
	typedef DefaultHashGen<const T> Type;
};

}
