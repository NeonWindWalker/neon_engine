#pragma once

#include "../base/include.h"

namespace ReflectionLib 
{

USING_BASE_LIB

typedef char yes_value;
typedef void* no_value;

#define MPL_CLASS_HAS_METHOD(func) \
	template< typename t_type > \
struct class_has_method_##func \
{ \
struct base_mixin { void func(); }; \
	template< typename tt_type > \
struct base : public tt_type, public base_mixin {}; \
	template< typename ttt_type, ttt_type > class tester{}; \
	template< typename tttt_type > \
	static ReflectionLib::no_value test( tttt_type*, tester<void (base_mixin::*)(), &base<tttt_type>::func>* = 0 ){ return 0; } \
	static ReflectionLib::yes_value test(...){ return 0; } \
	enum { value = sizeof(test((t_type*)(0))) == sizeof(ReflectionLib::yes_value)}; \
};


template< typename T >
struct is_constant
{
	static yes_value test(const T*){ return 0; }
	static no_value test(...){ return 0; }
	enum { value = sizeof(test((T*)(0))) == sizeof(yes_value)};
};

class NoInitConstructoExtensionFlag
{
public:
	NoInitConstructoExtensionFlag(){}
};



#define _commafix1(v0, v1) v0, v1
#define _commafix2(v0, v1, v2) v0, v1, v2
#define _commafix3(v0, v1, v2, v3) v0, v1, v2, v3

class Type;
class Reflection;
template<class T> class TType;
template<class T> class TReflection;

static const uint gcMaxArgumntsCount = 16;

enum EMethaType
{
	EMethaType_Null = 0,
	EMethaType_Value = 1,
	EMethaType_EnumValue = 2,
	EMethaType_Method = 4,
	EMethaType_Array = 8,
	EMethaType_PolyArray = 16,
	EMethaType_RefArray = 32,
	EMethaType_Class = 64,
	EMethaType_Map = 128,
	EMethaType_PolyMap = 256,
	EMethaType_RefMap = 512
};

class GetterSetterInfo
{
public:
	boolean getter;
	boolean setter;

	GetterSetterInfo()
	{
		getter = setter = false;
	}

	GetterSetterInfo(boolean getter, boolean setter)
	{
		this->getter = getter;
		this->setter = getter;
	}
};

enum EChildMode
{
	EChildType_Normal = 1,
	EChildType_Polymorphic,
	EChildType_Reference
};

struct StringDeserializerErrorMsg
{
	const utf8* pos;
	String msg;

	boolean isError(){ return pos != 0; }

	StringDeserializerErrorMsg()
	{
		pos = 0;
	}
};

//template<class t_Enum>
//static INLINE t_Enum invalidEnumValue(){ return (t_Enum)0; }

typedef Range<Reflection> Arguments;
typedef ConstRange<Type> Semantic;

class MethodSemantic
{
public:
	Semantic in;
	Semantic out;
};

static INLINE boolean isValue(EMethaType type)
{
	return type == EMethaType_Value || type == EMethaType_EnumValue;
}

static INLINE boolean isNamed(EMethaType type)
{
	return type & (EMethaType_Class | EMethaType_Map | EMethaType_PolyMap | EMethaType_RefMap);
}

static INLINE boolean isContainer(EMethaType type)
{
	return type & (EMethaType_Array | EMethaType_PolyArray | EMethaType_RefArray | EMethaType_Class | EMethaType_Map | EMethaType_PolyMap | EMethaType_RefMap);
}

static INLINE boolean isInsertEraseAllowed(EMethaType type)
{
	return type & (EMethaType_Array | EMethaType_PolyArray | EMethaType_RefArray | EMethaType_Map | EMethaType_PolyMap | EMethaType_RefMap);
}

static INLINE boolean isReferenceConatainer(EMethaType type)
{
	return type & (EMethaType_RefArray | EMethaType_RefMap);
}

}
