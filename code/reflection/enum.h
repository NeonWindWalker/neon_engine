#pragma once

#include "treflection.h"

namespace ReflectionLib 
{

//#define ReflectionEnum_EnumStrings {"a","b","c","d"}
//#define ReflectionEnum_EnumValues {a,b,c,d}

//#define DeclareEnumTypeInstance(mp_EnumType, mp_AnyName) \
	//uint ReflectionLib::TType<mp_EnumType>::gInitRefCount = 0;\
	//ReflectionLib::TType<mp_EnumType>::Data* ReflectionLib::TType<mp_EnumType>::gData = 0;\
	//ReflectionLib::GlobalIniter< ReflectionLib::TType<mp_EnumType> > mp_AnyName;

#define ReflectionEnum(mp_EnumType, mp_EnumSize) \
namespace ReflectionLib { \
template<> \
class TType<mp_EnumType> : public ReflectionLib::DummyType \
{ \
public: \
	\
	static ReflectionLib::Private::EnumDesc*& desc() \
	{ \
		static ReflectionLib::Private::EnumDesc* gpDescInstance; \
		return gpDescInstance; \
	} \
	static INLINE ReflectionLib::EMethaType	methaType()							{ return ReflectionLib::EMethaType_EnumValue; } \
	static INLINE Base::ConstString	name()									{ return desc()->name; } \
	static INLINE ReflectionLib::TypeId	id()									{ return desc()->id; } \
	static INLINE Base::uint						enumsCount()			{ return mp_EnumSize; } \
	static INLINE Base::ConstString				enumName(uint i) \
	{ \
		ReflectionLib::Private::EnumDesc* gDesc = desc(); \
		if(i < gDesc->nameByValue.size()) \
			return gDesc->nameByValue[i].val; \
		return _CS(""); \
	} \
	static INLINE Base::uint						enumValue(Base::uint i) \
	{ \
		ReflectionLib::Private::EnumDesc* gDesc = desc(); \
		if(i < gDesc->valueByName.size()) \
			return gDesc->valueByName[i].val; \
		return 0;\
	} \
	static INLINE Base::boolean 					enumPrint(Base::String& out, Base::uint value) \
	{ \
		ReflectionLib::Private::EnumDesc* gDesc = desc(); \
		ReflectionLib::Private::EnumDesc::NameByValue_t::ConstIterator it = gDesc->nameByValue.find(value);\
		if(it == gDesc->nameByValue.end())\
			return false;\
		out = it->val;\
		return true;\
	}\
	static INLINE Base::boolean 					enumParse(Base::uint& out, const Base::ConstString& name)\
	{ \
		ReflectionLib::Private::EnumDesc* gDesc = desc(); \
		ReflectionLib::Private::EnumDesc::ValueByName_t::ConstIterator it = gDesc->valueByName.find(name);\
		if(it == gDesc->valueByName.end())\
			return false;\
		out = (uint)it->val;\
		return true;\
	}\
	template<class t_Refl> \
	static INLINE boolean valueGet(ReflectionLib::TypeId in_id, void* v, t_Refl& refl)  \
	{ \
		if(in_id == id()) \
			return refl.get(v); \
			 \
		if(in_id.number() == ReflectionLib::PredefinedTypeID<Base::String>::value){ \
			Base::uint nVal; \
			if(!refl.get(&nVal)) \
				return false; \
			return enumPrint(*(Base::String*)v, nVal); \
		} \
		if(in_id.number() == ReflectionLib::PredefinedTypeID<Base::uint>::value){ \
			Base::uint nVal; \
			if(!refl.get(&nVal)) \
				return false; \
			*(Base::uint*)v = nVal; \
			return true; \
		} \
		return false; \
	} \
	template<class t_Refl> \
	static INLINE Base::boolean valueSet(t_Refl& refl, ReflectionLib::TypeId in_id, const void* v) \
	{ \
		if(in_id == id()) \
			return refl.set(v); \
			 \
		if(in_id.number() == ReflectionLib::PredefinedTypeID<Base::String>::value){ \
			Base::uint nVal; \
			if(!enumParse(nVal, *(const Base::String*)v)) \
				return false; \
			return refl.set(&nVal); \
		} \
		 \
		if(in_id.number() == ReflectionLib::PredefinedTypeID<Base::ConstString>::value){ \
			Base::uint nVal; \
			if(!enumParse(nVal, *(const Base::ConstString*)v)) \
				return false; \
			return refl.set(&nVal); \
		} \
		 \
		return false; \
	} \
	static INLINE Base::boolean makeReflection(void* mem, void* ptr); \
	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP \
	\
	NOINLINE static void zrl_IniterFunc() \
	{ \
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
		Base::boolean bNew; \
		desc() = lib.giveEnumDesc(bNew, _CS(#mp_EnumType));\
		if(!bNew)return; \
		const char* names[] = ReflectionEnum_EnumStrings;\
		mp_EnumType values[] = ReflectionEnum_EnumValues;\
		for(uint i=0; i<mp_EnumSize; ++i)\
		{\
			desc()->nameByValue((uint)values[i])->val = constString(names[i]);\
			desc()->valueByName(constString(names[i]))->val = (uint)values[i];\
		}\
		desc()->id = lib.getFreeTypeId();\
		desc()->type = ReflectionLib::Type::make< TType<mp_EnumType> >();\
	}\
	class zrl_Initer \
	{ \
	public: \
		zrl_Initer() \
		{ \
			ReflectionLib::Private::Library::instance().registerEnumType(&zrl_IniterFunc); \
		} \
	}; \
	THIS_IS_USED void* zrl_Initer_IniterFunc() \
	{ \
		return &::ReflectionLib::Private::StaticIniter<zrl_Initer>::initer; \
	}\
};\
template<> \
class TReflection<mp_EnumType> : public ReflectionLib::IReflection \
{public: \
	mp_EnumType& obj; \
	TReflection(mp_EnumType& val) : obj(val)			{ static_assert(sizeof(*this) <= ReflectionLib::Reflection::gcMaxReflectionImplimentationSize, "big TReflection"); } \
	REFLECTION_DECLARE_TYPE_INTERFACES(ReflectionLib::TType<mp_EnumType>) \
	virtual void*	objectPtr()						{ return &obj; }\
	virtual Base::boolean valid()					{ return &obj != 0; }\
	virtual Base::boolean get(void* v)				{ *(mp_EnumType*)v = obj; return true; } \
	virtual Base::boolean set(const void* v)		{ obj = *(const mp_EnumType*)v; return true; } \
	virtual Base::boolean get(ReflectionLib::TypeId id, void* v) \
	{ \
		return ReflectionLib::TType<mp_EnumType>::valueGet(id, v, *this); \
	} \
	virtual Base::boolean set(ReflectionLib::TypeId id, const void* v) \
	{ \
		return ReflectionLib::TType<mp_EnumType>::valueSet(*this, id, v); \
	} \
}; \
INLINE Base::boolean TType<mp_EnumType>::makeReflection(void* mem, void* ptr) \
{ \
	if(ptr){ \
		new(mem) ReflectionLib::TReflection<mp_EnumType>(*(mp_EnumType*)ptr); \
		return true; \
	} \
	return false; \
} \
template<> class ReflectionCompileTimeGuard<mp_EnumType> { public: static inline void guardTest(){} }; }



template<class t_Enum>
static INLINE Base::boolean eparse(t_Enum& val, const Base::ConstString& str)
{
	return TType<t_Enum>::enumParse(val, str);
}

template<class t_Enum>
static INLINE Base::String eprint(t_Enum val)
{
	String out;
	if(TType<t_Enum>::enumPrint(out, val))
		return out;
	return _CS("");
}

template<class t_Enum>
static INLINE Base::boolean eprint(Base::String& str, t_Enum val)
{
	return TType<t_Enum>::enumPrint(str, val);
}

//#define ReflectionEnum_spec(mp_EnumType, mp_EnumSize) namespace Reflection { ReflectionEnum(mp_EnumType, mp_EnumSize) }

}


#define ReflectionEnum_EnumStrings {"Value",	"EnumValue", "Method", "Array",	"PolyArray", "RefArray", "Class", "Map", "PolyMap", "RefMap" }
#define ReflectionEnum_EnumValues  { ReflectionLib::EMethaType_Value, ReflectionLib::EMethaType_EnumValue, ReflectionLib::EMethaType_Method, ReflectionLib::EMethaType_Array, ReflectionLib::EMethaType_PolyArray, ReflectionLib::EMethaType_RefArray, ReflectionLib::EMethaType_Class, ReflectionLib::EMethaType_Map, ReflectionLib::EMethaType_PolyMap, ReflectionLib::EMethaType_RefMap }
ReflectionEnum(ReflectionLib::EMethaType, 10)
#undef ReflectionEnum_EnumStrings
#undef ReflectionEnum_EnumValues

//	
