#pragma once

#include "seed.h"

namespace Base
{

template<typename t_Val0, typename t_Val1>
class DefaultComparator;
/*
{
public:
	INLINE int operator() (const t_Val0& a, const t_Val1& b)const
	{
		if(a < b)
			return -1;
		if(a == b)
			return 0;
		return 1;
	}
};*/

template<class t_Val0, class t_Val1> DefaultComparator<const t_Val0, const t_Val1> makeComparator()
{
	return DefaultComparator<const t_Val0, const t_Val1>();
}

template<class t_Val0, class t_Val1> DefaultComparator<const t_Val0, const t_Val1> makeComparator(t_Val0& , t_Val1&)
{
	return DefaultComparator<const t_Val0, const t_Val1>();
}

template<>
class DefaultComparator<const int8, const int8>
{
public:
	INLINE int operator() (const int8& a, const int8& b) const
	{
		return int(a) - int(b);
	}
};

template<>
class DefaultComparator<const uint8, const uint8>
{
public:
	INLINE int operator() (const uint8& a, const uint8& b)const
	{
		return int(a) - int(b);
	}
};


template<>
class DefaultComparator<const int16, const int16>
{
public:
	INLINE int operator() (const int16& a, const int16& b)const
	{
		return int(a) - int(b);
	}
};

template<>
class DefaultComparator<const uint16, const uint16>
{
public:
	INLINE int operator() (const uint16& a, const uint16& b)const
	{
		return int(a) - int(b);
	}
};

template<>
class DefaultComparator<const int32, const int32>
{
public:
	INLINE int operator() (const int32& a, const int32& b)const
	{
		if(a < b)
			return -1;
		if(a == b)
			return 0;
		return 1;
	}
};

template<>
class DefaultComparator<const uint32, const uint32>
{
public:
	INLINE int operator() (const uint32& a, const uint32& b)const
	{
		if(a < b)
			return -1;
		if(a == b)
			return 0;
		return 1;
	}
};

class FastInt32Comparator
{
public:
	INLINE int operator() (const int32& a, const int32& b)const
	{
		return a - b;
	}
};

class FastUint32Comparator
{
public:
	INLINE int operator() (const uint32& a, const uint32& b)const
	{
		return int(a) - int(b);
	}
};

template<>
class DefaultComparator<const float, const float>
{
public:
	INLINE int operator() (const float& a, const float& b)const
	{
		float d = a - b;
		return *reinterpret_cast<int32*>(&d);
	}
};

template<>
class DefaultComparator<const double, const double>
{
public:
	INLINE int operator() (const double& a, const double& b)const
	{
		float d = float(a - b);
		return *reinterpret_cast<int32*>(&d);
	}
};
    
template<>
class DefaultComparator<const uint64, const uint64>
{
public:
    INLINE int operator() (const uint64& a, const uint64& b)const
    {
        if(a < b)
            return -1;
        if(a == b)
            return 0;
        return 1;
    }
};
    
template<>
class DefaultComparator<const int64, const int64>
{
public:
    INLINE int operator() (const int64& a, const int64& b)const
    {
        if(a < b)
            return -1;
        if(a == b)
            return 0;
        return 1;
    }
};

}



