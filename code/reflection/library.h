#pragma once

#include "type.h"
#include "reflection.h"
#include "values.h"
#include "makers.h"
#include "constructable.h"

namespace ReflectionLib { namespace Private {

USING_BASE_LIB

typedef void (*InitFunctionType) ();
typedef byte* (*AddresingFunctionType) (void* mem);
typedef boolean (*TypeFunctionType) (byte*mem);
typedef boolean (*ReflectionFunctionType) (void* mem, void* thisptr);
typedef boolean (*ConstructFunctionType) (void* mem, void* thisptr, const Type& type, void** apArgs);
typedef boolean (*SetReferenceFunctionType) (void* thisptr, void* refval);

class CustomAllocatorWrapper
{
public:
	struct Data_t{};
	INLINE CustomAllocatorWrapper(){}
	INLINE CustomAllocatorWrapper(Data_t& ad){}
	static TempAllocator& allocator();
};

typedef TString<utf8, CustomAllocatorWrapper > CustomString;

class EnumDesc
{
public:
	typedef Base::HashArray< Pair<uint, CustomString>, CustomAllocatorWrapper > NameByValue_t;
	typedef Base::HashArray< Pair<CustomString, uint>, CustomAllocatorWrapper > ValueByName_t;
	NameByValue_t nameByValue;
	ValueByName_t valueByName;
	TypeId id;
	CustomString name;
	Type type;
};

class MethodDesc
{
public:
	TypeId id;
	CustomString name;
	Type type;
	MethodSemantic semantic;
	Array<Type, CustomAllocatorWrapper> inTypes;
	Array<Type, CustomAllocatorWrapper> outTypes;
	void build()
	{
		semantic.in = inTypes.range();
		semantic.out = outTypes;
	}
};

struct ChildDesc
{
	Type type;
	EChildMode info;
	int offset;
	uint deep;
	AddresingFunctionType addresing;
	ReflectionFunctionType reflection;
	SetReferenceFunctionType setref;
	ConstructFunctionType construct;
	Attributes attributes;
	const Semantic* localConstructorSemantic;

	ChildDesc()
	{
		offset = 0;
		deep = 0;
		addresing = 0;
		reflection = 0;
		setref = 0;
		construct = 0;
		localConstructorSemantic = 0;
	}
};

struct BaseClassDesc
{
	class ClassDesc* desc;
	Type type;
	uint offset;
	uint size;
	uint deep; // 0 - base class, 1 - base class of base class ....
	AddresingFunctionType virtualAddresFunc;
	ReflectionFunctionType reflection;
	BaseClassDesc()
	{
		desc = 0;
		offset = 0;
		size = 0;
		deep = 0;
		virtualAddresFunc = 0;
		reflection = 0;
	}

	INLINE boolean operator == (const BaseClassDesc& other)const
	{
		return desc == other.desc && 
			type == other.type && 
			offset == other.offset && 
			size == other.size && 
			deep == other.deep && 
			virtualAddresFunc == other.virtualAddresFunc && 
			reflection == other.reflection;
	}
};
typedef  Array< BaseClassDesc, CustomAllocatorWrapper > BaseClassDescArray;

typedef HashArray< Pair<CustomString, ChildDesc*>, CustomAllocatorWrapper > ChildDictionary;
typedef HashArray< Pair<CustomString, BaseClassDescArray>, CustomAllocatorWrapper > BaseClassDictionary;
typedef HashArray< Pair<uint, BaseClassDescArray>, CustomAllocatorWrapper > BaseClassIdDictionary;

class ClassDesc
{
public:
	boolean inited;

	/*struct BaseDesc
	{
		ClassDesc* desc;
		int offset;
		BaseDesc()
		{
			desc = 0;
			offset = 0;
		}
	};*/
	//Array<BaseDesc> baseClassesDescs;
	//Array<InitFunctionType> childsInitFunctions;

	CustomString name;
	TypeId id;
	Type type;
	uint size;
	BaseClassDictionary baseClasses;
	BaseClassIdDictionary baseClassesById;
	uint trueBaseClassesCount;
	ChildDictionary childs;
	Attributes attributes;
	Semantic constructorSemanticRange;
	const Semantic* constructorSemantic;

	ClassDesc()
	{
		inited = false;
		size = 0;
		trueBaseClassesCount = 0;
		constructorSemantic = &constructorSemanticRange;
	}
};

class ContainerClassDesc
{
public:
	CustomString name;
	TypeId id;
	Type type;
	Array< Type, CustomAllocatorWrapper > constructorSemanticArray;
	Array< Type, CustomAllocatorWrapper > localConstructorSemanticArray;
	Semantic constructorSemanticRange;
	Semantic localConstructorSemanticRange;
	const Semantic* constructorSemantic;
	const Semantic* localConstructorSemantic;
	ContainerClassDesc()
	{
		constructorSemantic = &constructorSemanticRange;
		localConstructorSemantic = &localConstructorSemanticRange;
	}
};


class Library
{
public:
	typedef HashArray< Pair<CustomString, Type>, CustomAllocatorWrapper > TypeDictionary;
	typedef HashArray< Pair<uint, Type>, CustomAllocatorWrapper > TypeIdDictionary;

	typedef HashArray< Pair<CustomString, ClassDesc*>, CustomAllocatorWrapper > ClassDescDictionary;
	typedef HashArray< Pair<uint, ClassDesc*>, CustomAllocatorWrapper > ClassDescIdDictionary;
	typedef HashArray< Pair<CustomString, EnumDesc*>, CustomAllocatorWrapper > EnumDescMap;
	typedef HashArray< Pair<CustomString, MethodDesc*>, CustomAllocatorWrapper > MethodDescMap;
	typedef HashArray< Pair<CustomString, ContainerClassDesc*>, CustomAllocatorWrapper > ContainerClassDescMap;
	typedef Array<InitFunctionType, CustomAllocatorWrapper> InitFuncArray;
	
	TypeDictionary typeDictionary;
	TypeIdDictionary typeIdDictionary;
	ClassDescDictionary classDescMap;
	ClassDescIdDictionary classDescIdMap;
	EnumDescMap enumDescMap;
	MethodDescMap methodDescMap;
	ContainerClassDescMap containerClassDescMap;
	InitFuncArray enumRegistrations;
	InitFuncArray methodRegistrations;
	InitFuncArray classRegistrations;
	InitFuncArray baseClassRegistrations;
	InitFuncArray childRegistrations;
	InitFuncArray classConstructoSementicRegistrations;
	InitFuncArray containerClassRegistrations;
	
	Library();
	~Library();
	void init();
	void deinit();
	void clearTemp();
	static Library& instance();
	//static PoolAllocator& allocator();

	void registerEnumType(InitFunctionType fun);
	void registerMethodType(InitFunctionType fun);
	void registerClass(InitFunctionType fun);
	void registerBaseClass(InitFunctionType fun);
	void registerChild(InitFunctionType fun);
	void registerClassConstructoSementic(InitFunctionType fun);
	void registerContainerClass(InitFunctionType fun);
	TypeId getFreeTypeId();

	EnumDesc* giveEnumDesc(boolean& outNew, const ConstString& name);
	MethodDesc* giveMethodDesc(boolean& outNew, const ConstString& name);
	ClassDesc* giveClassDesc(boolean& outNew, const ConstString& name);
	BaseClassDesc* giveBaseClassDesc(boolean& outNew, ClassDesc& owner, const ConstString& name);
	ChildDesc* giveChildDesc(boolean& outNew, ClassDesc& owner, const ConstString& name);
	ContainerClassDesc* giveContainerClassDesc(boolean& outNew, const ConstString& name);
};

template<class T>
class StaticIniter
{
public:
	static T initer;
};

template< typename T >
T StaticIniter<T>::initer;

}}


namespace ReflectionLib
{

static INLINE void initReflectionLibrary()
{
	ReflectionLib::Private::Library::instance().init();
}

static INLINE void deinitReflectionLibrary()
{
	ReflectionLib::Private::Library::instance().deinit();
}

}
