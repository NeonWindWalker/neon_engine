#pragma once

#include "treflection.h"

namespace ReflectionLib
{

	template<class t_Type>
	class MethodArgumentCaster
	{
	public:
		static INLINE t_Type& cast(void* p){ return *(t_Type*)p; }
		static INLINE boolean verify(void* p){ return p != 0; }
	};

	template<class t_Type>
	class MethodArgumentCaster<const t_Type>
	{
	public:
		static INLINE const t_Type& cast(void* p){ return *(const t_Type*)p; }
		static INLINE boolean verify(void* p){ return p != 0; }
	};

	template<class t_Type>
	class MethodArgumentCaster<t_Type*>
	{
	public:
		static INLINE t_Type* cast(void* p){ return (t_Type*)p; }
		static INLINE boolean verify(void* p){ return true; }
	};

	template<class t_Type>
	class MethodArgumentCaster<const t_Type*>
	{
	public:
		static INLINE const t_Type* cast(void* p){ return (const t_Type*)p; }
		static INLINE boolean verify(void* p){ return true; }
	};

	template<class t_Type>
	class MethodArgumentCaster<t_Type&>
	{
	public:
		static INLINE t_Type& cast(void* p){ return *(t_Type*)p; }
		static INLINE boolean verify(void* p){ return p != 0; }
	};

	template<class t_Type>
	class MethodArgumentCaster<const t_Type&>
	{
	public:
		static INLINE const t_Type& cast(void* p){ return *(const t_Type*)p; }
		static INLINE boolean verify(void* p){ return p != 0; }
	};




#define METHOD_TYPE_BODY \
static ReflectionLib::Private::MethodDesc*& desc() \
{ \
	static ReflectionLib::Private::MethodDesc* gpDescInstance; \
	return gpDescInstance; \
} \
static INLINE ReflectionLib::EMethaType	methaType()									{ return ReflectionLib::EMethaType_Method; } \
static INLINE Base::ConstString	name()									{ return desc()->name; } \
static INLINE ReflectionLib::TypeId	id()									{ return desc()->id; } \
static INLINE const MethodSemantic*	methodSemantic()					{ return &desc()->semantic; } \
static INLINE Base::boolean makeReflection(void* mem, void* ptr) { return false; } \
EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP \
class zrl_Initer \
{ \
public: \
	zrl_Initer()  \
	{ \
		ReflectionLib::Private::Library::instance().registerMethodType(&zrl_IniterFunc); \
	} \
};

class TMethodType0r0 : public ReflectionLib::DummyType
{
public:
	METHOD_TYPE_BODY

	NOINLINE static void zrl_IniterFunc()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance();
		Base::boolean bNew;
		String name;
		name << _CS("<><>");
		desc() = lib.giveMethodDesc(bNew, name);
		if(!bNew)return;
		desc()->name = name;
		desc()->id = lib.getFreeTypeId();
		desc()->build();
	}
};


template<class t_InType0>
class TMethodType0r1 : public ReflectionLib::DummyType
{
public:
	NOINLINE static void zrl_IniterFunc()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance();
		Base::boolean bNew;
		String name;
		name << _CS("<><") << typeOf<t_InType0>().name() << _CS(">");
		desc() = lib.giveMethodDesc(bNew, name);
		if(!bNew)return;
		desc()->name = name;
		desc()->id = lib.getFreeTypeId();
		desc()->inTypes.resize(1);
		desc()->inTypes[0] = typeOf<t_InType0>();
		desc()->build();
	}

	METHOD_TYPE_BODY
};

template<class t_InType0, class t_InType1>
class TMethodType0r2 : public ReflectionLib::DummyType
{
public:
	NOINLINE static void zrl_IniterFunc()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance();
		Base::boolean bNew;
		String name;
		name << _CS("<><") << typeOf<t_InType0>().name() << _CS(",") << typeOf<t_InType1>().name() << _CS(">");
		desc() = lib.giveMethodDesc(bNew, name);
		if(!bNew)return;
		desc()->name = name;
		desc()->id = lib.getFreeTypeId();
		desc()->inTypes.resize(2);
		desc()->inTypes[0] = typeOf<t_InType0>();
		desc()->inTypes[1] = typeOf<t_InType1>();
		desc()->build();
	}

	METHOD_TYPE_BODY
};

template<class t_OutType0>
class TMethodType1r0 : public ReflectionLib::DummyType
{
public:
	NOINLINE static void zrl_IniterFunc()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance();
		Base::boolean bNew;
		String name;
		name << _CS("<") << typeOf<t_OutType0>().name() << _CS("><") << _CS(">");
		desc() = lib.giveMethodDesc(bNew, name);
		if (!bNew)return;
		desc()->name = name;
		desc()->id = lib.getFreeTypeId();
		desc()->outTypes.resize(1);
		desc()->outTypes[0] = typeOf<t_OutType0>();
		desc()->build();
	}

	METHOD_TYPE_BODY
};

template<class t_OutType0, class t_InType0>
class TMethodType1r1 : public ReflectionLib::DummyType
{
public:
	NOINLINE static void zrl_IniterFunc()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance();
		Base::boolean bNew;
		String name;
		name << _CS("<") << typeOf<t_OutType0>().name() << _CS("><") << typeOf<t_InType0>().name() << _CS(">");
		desc() = lib.giveMethodDesc(bNew, name);
		if(!bNew)return;
		desc()->name = name;
		desc()->id = lib.getFreeTypeId();
		desc()->inTypes.resize(1);
		desc()->inTypes[0] = typeOf<t_InType0>();
		desc()->outTypes.resize(1);
		desc()->outTypes[0] = typeOf<t_OutType0>();
		desc()->build();
	}

	METHOD_TYPE_BODY
};

#undef METHOD_TYPE_BODY

}

#define ReflectionMethod0( mp_ReflectionClassName, mp_Method ) \
class mp_ReflectionClassName : public ReflectionLib::IReflection \
{ \
public: \
	typedef ReflectionLib::TMethodType0r0 Type_t; \
	mp_ReflectionClassName(zrl_ThisClassType& o) : obj(o){} \
	void*	objectPtr()		{ return &obj; } \
	Base::boolean call(void** apOut, void** apIn)	{ obj.mp_Method(); return true; } \
	REFLECTION_DECLARE_TYPE_INTERFACES(Type_t) \
protected: \
	zrl_ThisClassType& obj; \
};

#define ReflectionMethod1( mp_ReflectionClassName, mp_Method, mp_InType0 ) \
class mp_ReflectionClassName : public ReflectionLib::IReflection \
{ \
public: \
	typedef ReflectionLib::TMethodType0r1<mp_InType0> Type_t; \
	mp_ReflectionClassName(zrl_ThisClassType& o) : obj(o){} \
	void*	objectPtr()		{ return &obj; } \
	Base::boolean call(void** apOut, void** apIn){ \
		if(!ReflectionLib::MethodArgumentCaster<mp_InType0>::verify(apIn[0]))return false; \
		obj.mp_Method(ReflectionLib::MethodArgumentCaster<mp_InType0>::cast(apIn[0])); \
		return true; \
	} \
	REFLECTION_DECLARE_TYPE_INTERFACES(Type_t) \
protected: \
	zrl_ThisClassType& obj; \
};

#define ReflectionMethod2(mp_ReflectionClassName, mp_Method, mp_InType0, mp_InType1) \
class mp_ReflectionClassName : public ReflectionLib::IReflection \
{ \
public: \
	typedef ReflectionLib::TMethodType0r2<mp_InType0, mp_InType1> Type_t; \
	mp_ReflectionClassName(zrl_ThisClassType& o) : obj(o){} \
	void*	objectPtr()		{ return &obj; } \
	Base::boolean call(void** apOut, void** apIn){ \
		if(!ReflectionLib::MethodArgumentCaster<mp_InType0>::verify(apIn[0]))return false; \
		if(!ReflectionLib::MethodArgumentCaster<mp_InType1>::verify(apIn[1]))return false; \
		obj.mp_Method(ReflectionLib::MethodArgumentCaster<mp_InType0>::cast(apIn[0]), ReflectionLib::MethodArgumentCaster<mp_InType1>::cast(apIn[1])); \
		return true; \
	} \
	REFLECTION_DECLARE_TYPE_INTERFACES(Type_t) \
protected: \
	zrl_ThisClassType& obj; \
};

#define ReflectionMethod0r( mp_ReflectionClassName, mp_Method, mp_OutType0) \
class mp_ReflectionClassName : public ReflectionLib::IReflection \
{ \
public: \
	typedef ReflectionLib::TMethodType1r0<mp_OutType0> Type_t; \
	mp_ReflectionClassName(zrl_ThisClassType& o) : obj(o){} \
	void*	objectPtr()		{ return &obj; } \
	Base::boolean call(void** apOut, void** apIn){\
		*(mp_OutType0*)apOut[0] = obj.mp_Method(); \
		return true; \
	} \
	REFLECTION_DECLARE_TYPE_INTERFACES(Type_t) \
protected: \
	zrl_ThisClassType& obj; \
};

#define ReflectionMethod1r( mp_ReflectionClassName, mp_Method, mp_OutType0, mp_InType0 ) \
class mp_ReflectionClassName : public ReflectionLib::IReflection \
{ \
public: \
	typedef ReflectionLib::TMethodType1r1<mp_OutType0, mp_InType0> Type_t; \
	mp_ReflectionClassName(zrl_ThisClassType& o) : obj(o){} \
	void*	objectPtr()		{ return &obj; } \
	Base::boolean call(void** apOut, void** apIn){ \
		if(!ReflectionLib::MethodArgumentCaster<mp_InType0>::verify(apIn[0]))return false; \
		*(mp_OutType0*)apOut[0] = obj.mp_Method(ReflectionLib::MethodArgumentCaster<mp_InType0>::cast(apIn[0])); \
		return true; \
	} \
	REFLECTION_DECLARE_TYPE_INTERFACES(Type_t) \
protected: \
	zrl_ThisClassType& obj; \
};


#define ReflectionMethod(mp_Name, mp_TReflection) \
	static Base::boolean zrl_reflection_##mp_Name(void* mem, void* thisptr){ new(mem) mp_TReflection(*(zrl_ThisClassType*)thisptr); return true; } \
	static void zrl_FieldInitFunc_##mp_Name() \
	{ \
		ReflectionLib::Private::ClassDesc*& cd = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
		Base::boolean bNew; \
		ReflectionLib::Private::ChildDesc* d = lib.giveChildDesc(bNew, *cd, _CS(#mp_Name)); \
		if(bNew){ \
			d->type = ReflectionLib::Type::make<mp_TReflection::Type_t>(); \
			d->reflection = &zrl_reflection_##mp_Name; \
		} \
	} \
	class zrl_FiledIniter_##mp_Name \
	{ \
	public: \
		zrl_FiledIniter_##mp_Name() \
		{ \
			ReflectionLib::Private::Library::instance().registerChild(&zrl_FieldInitFunc_##mp_Name); \
		} \
	}; \
	THIS_IS_USED uinteger zrl_FiledIniterFunc_##mp_Name() \
	{ \
		return (uinteger)&::ReflectionLib::Private::StaticIniter<zrl_FiledIniter_##mp_Name>::initer +  \
				(uinteger)&::ReflectionLib::Private::StaticIniter<mp_TReflection::Type_t::zrl_Initer>::initer; \
	}

#define rl_method_0(mp_Name) \
	ReflectionMethod0(zrl_MethodReflection_##mp_Name, mp_Name) \
	ReflectionMethod(mp_Name, zrl_MethodReflection_##mp_Name)\
	void mp_Name()

#define rl_method_1(mp_Name, mp_InType0, mp_InVal0 ) \
	ReflectionMethod1(zrl_MethodReflection_##mp_Name, mp_Name, mp_InType0 ) \
	ReflectionMethod(mp_Name, zrl_MethodReflection_##mp_Name)\
	void mp_Name(mp_InType0& mp_InVal0)

#define rl_method_2(mp_Name, mp_InType0, mp_InVal0, mp_InType1, mp_InVal1) \
	ReflectionMethod2(zrl_MethodReflection_##mp_Name, mp_Name, mp_InType0, mp_InType1) \
	ReflectionMethod(mp_Name, zrl_MethodReflection_##mp_Name)\
	void mp_Name(mp_InType0& mp_InVal0, mp_InType1& mp_InVal1)

#define rl_method_0r(mp_OutType0, mp_Name) \
	ReflectionMethod0r(zrl_MethodReflection_##mp_Name, mp_Name, mp_OutType0) \
	ReflectionMethod(mp_Name, zrl_MethodReflection_##mp_Name) \
	mp_OutType0 mp_Name()

#define rl_method_1r(mp_OutType0, mp_Name, mp_InType0, mp_InVal0 ) \
	ReflectionMethod1r(zrl_MethodReflection_##mp_Name, mp_Name, mp_OutType0, mp_InType0 ) \
	ReflectionMethod(mp_Name, zrl_MethodReflection_##mp_Name) \
	mp_OutType0 mp_Name(mp_InType0& mp_InVal0)

