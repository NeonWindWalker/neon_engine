#pragma once

#include "treflection.h"

//#define TO_STR(x) TO_STR_EXECUTE(x)
//#define TO_STR_EXECUTE(x) #x

#define rl_class(mp_Class) typedef mp_Class zrl_ThisClassType; \
static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){} \
static INLINE Base::boolean zrl_construct(void* mem, Base::IAllocator& alc, void** args) \
{ \
	zrl_ThisClassType* p = new(alc) zrl_ThisClassType(); \
	new(mem) ReflectionLib::TReflection<zrl_ThisClassType>(*p); \
	return true; \
} \
static void zrl_ClassDescInit(){ \
	ReflectionLib::Private::ClassDesc*& d = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
	Base::boolean bNew; \
	ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
	d = lib.giveClassDesc(bNew, _CS(#mp_Class)); \
	if(bNew){ \
		d->id = lib.getFreeTypeId(); \
		d->name = _CS(#mp_Class); \
		d->size = sizeof(mp_Class); \
		d->type = ReflectionLib::Type::make< ReflectionLib::TType<zrl_ThisClassType> >(); \
	} \
} \
class zrl_ClassIniter \
{ \
public: \
	zrl_ClassIniter() \
	{ \
		ReflectionLib::Private::Library::instance().registerClass(&zrl_ClassDescInit); \
	} \
}; \
THIS_IS_USED static void* zrl_IniterFunc() \
{ \
	return &::ReflectionLib::Private::StaticIniter<zrl_ClassIniter>::initer; \
}


#define rl_abstract_class(mp_Class) typedef mp_Class zrl_ThisClassType; \
static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){} \
static INLINE Base::boolean zrl_construct(void* mem, Base::IAllocator& alc, void** args) \
{ \
	return false; \
} \
static void zrl_ClassDescInit(){  \
	ReflectionLib::Private::ClassDesc*& d = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
	Base::boolean bNew; \
	ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
	d = lib.giveClassDesc(bNew, _CS(#mp_Class)); \
	if(bNew){ \
		d->id = lib.getFreeTypeId(); \
		d->name = _CS(#mp_Class); \
		d->size = sizeof(mp_Class); \
		d->type = ReflectionLib::Type::make< ReflectionLib::TType<zrl_ThisClassType> >(); \
		d->constructorSemantic = 0; \
	} \
} \
class zrl_ClassIniter \
{ \
public: \
	zrl_ClassIniter() \
	{ \
	ReflectionLib::Private::Library::instance().registerClass(&zrl_ClassDescInit); \
	} \
}; \
THIS_IS_USED static void* zrl_IniterFunc() \
{ \
	return &::ReflectionLib::Private::StaticIniter<zrl_ClassIniter>::initer; \
}


#define rl_class1(mp_Class, mp_Arg0) typedef mp_Class zrl_ThisClassType; \
static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){} \
static INLINE Base::boolean zrl_construct(void* mem, Base::IAllocator& alc, void** args) \
{ \
	zrl_ThisClassType* p = new(alc) zrl_ThisClassType(*(mp_Arg0*)args[0]); \
	new(mem) ReflectionLib::TReflection<zrl_ThisClassType>(*p); \
	return true; \
} \
static void zrl_ClassDescInit(){ \
	ReflectionLib::Private::ClassDesc*& d = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
	Base::boolean bNew; \
	ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
	d = lib.giveClassDesc(bNew, _CS(#mp_Class)); \
	if(bNew){ \
		d->id = lib.getFreeTypeId(); \
		d->name = _CS(#mp_Class); \
		d->size = sizeof(mp_Class); \
		d->type = ReflectionLib::Type::make< ReflectionLib::TType<zrl_ThisClassType> >(); \
		ReflectionLib::Type* argtypes = new(ReflectionLib::Private::CustomAllocatorWrapper().allocator()) ReflectionLib::Type(); \
		argtypes[0] = ReflectionLib::typeOf<mp_Arg0>(); \
		d->constructorSemanticRange = ReflectionLib::Semantic(argtypes, 1); \
		d->constructorSemantic = &d->constructorSemanticRange; \
	} \
} \
class zrl_ClassIniter \
{ \
public: \
	zrl_ClassIniter() \
	{ \
	ReflectionLib::Private::Library::instance().registerClass(&zrl_ClassDescInit); \
	} \
}; \
THIS_IS_USED static void* zrl_IniterFunc() \
{ \
	return &::ReflectionLib::Private::StaticIniter<zrl_ClassIniter>::initer; \
}


#define rl_class2(mp_Class, mp_Arg0, mp_Arg1) typedef mp_Class zrl_ThisClassType; \
	static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){} \
	static INLINE Base::boolean zrl_construct(void* mem, Base::IAllocator& alc, void** args) \
	{ \
		zrl_ThisClassType* p = new(alc) zrl_ThisClassType(*(mp_Arg0*)args[0], *(mp_Arg1*)args[1]); \
		new(mem) ReflectionLib::TReflection<zrl_ThisClassType>(*p); \
		return true; \
	} \
	static void zrl_ClassDescInit(){ \
		ReflectionLib::Private::ClassDesc*& d = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
		Base::boolean bNew; \
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
		d = lib.giveClassDesc(bNew, _CS(#mp_Class)); \
		if(bNew){ \
		d->id = lib.getFreeTypeId(); \
		d->name = _CS(#mp_Class); \
		d->size = sizeof(mp_Class); \
		d->type = ReflectionLib::Type::make< ReflectionLib::TType<zrl_ThisClassType> >(); \
		ReflectionLib::Type* argtypes = new(ReflectionLib::Private::CustomAllocatorWrapper().allocator()) ReflectionLib::Type(); \
		argtypes[0] = ReflectionLib::typeOf<mp_Arg0>(); \
		argtypes[1] = ReflectionLib::typeOf<mp_Arg1>(); \
		d->constructorSemanticRange = ReflectionLib::Semantic(argtypes, 2); \
		d->constructorSemantic = &d->constructorSemanticRange; \
	} \
} \
class zrl_ClassIniter \
{ \
public: \
	zrl_ClassIniter() \
{ \
	ReflectionLib::Private::Library::instance().registerClass(&zrl_ClassDescInit); \
} \
}; \
	THIS_IS_USED static void* zrl_IniterFunc() \
{ \
	return &::ReflectionLib::Private::StaticIniter<zrl_ClassIniter>::initer; \
}

#define rl_abstract_class2(mp_Class, mp_Arg0, mp_Arg1) typedef mp_Class zrl_ThisClassType; \
	static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){} \
	static INLINE Base::boolean zrl_construct(void* mem, Base::IAllocator& alc, void** args) \
	{ \
		return false; \
	} \
	static void zrl_ClassDescInit(){ \
	ReflectionLib::Private::ClassDesc*& d = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
	Base::boolean bNew; \
	ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
	d = lib.giveClassDesc(bNew, _CS(#mp_Class)); \
	if(bNew){ \
	d->id = lib.getFreeTypeId(); \
	d->name = _CS(#mp_Class); \
	d->size = sizeof(mp_Class); \
	d->type = ReflectionLib::Type::make< ReflectionLib::TType<zrl_ThisClassType> >(); \
	ReflectionLib::Type* argtypes = new(ReflectionLib::Private::CustomAllocatorWrapper().allocator()) ReflectionLib::Type(); \
	argtypes[0] = ReflectionLib::typeOf<mp_Arg0>(); \
	argtypes[1] = ReflectionLib::typeOf<mp_Arg1>(); \
	d->constructorSemanticRange = ReflectionLib::Semantic(argtypes, 2); \
	d->constructorSemantic = &d->constructorSemanticRange; \
	} \
} \
class zrl_ClassIniter \
{ \
public: \
	zrl_ClassIniter() \
{ \
	ReflectionLib::Private::Library::instance().registerClass(&zrl_ClassDescInit); \
} \
}; \
	THIS_IS_USED static void* zrl_IniterFunc() \
{ \
	return &::ReflectionLib::Private::StaticIniter<zrl_ClassIniter>::initer; \
}


#define rl_base_class0(mp_Class) \
static Base::boolean zrl_BaseClass0Reflection(void* mem, void* thisptr){  \
	new(mem) ReflectionLib::TReflection<mp_Class>(*(mp_Class*)(thisptr)); \
	return true; \
} \
static void zrl_BaseClass0Init(){  \
	ReflectionLib::Private::ClassDesc*& cd = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
	ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
	Base::boolean bIns; \
	ReflectionLib::Private::BaseClassDesc* d = lib.giveBaseClassDesc(bIns,*cd,Base::constString(#mp_Class)); \
	if(bIns){ \
		zrl_ThisClassType* cl = reinterpret_cast<zrl_ThisClassType*>(16); \
		mp_Class* base = static_cast<mp_Class*>(cl); \
		Base::integer offset = reinterpret_cast<Base::integer>(base) - reinterpret_cast<Base::integer>(cl); \
		d->offset = (int)offset; \
		d->size = sizeof(mp_Class); \
		d->type = ReflectionLib::typeOf<mp_Class>(); \
		d->reflection = &zrl_BaseClass0Reflection; \
	} \
} \
class zrl_BaseClass0Initer \
{ \
public: \
	zrl_BaseClass0Initer() \
	{ \
		ReflectionLib::Private::Library::instance().registerBaseClass(&zrl_BaseClass0Init); \
	} \
}; \
THIS_IS_USED void* zrl_BaseClass0IniterFunc() \
{ \
	return &::ReflectionLib::Private::StaticIniter<zrl_BaseClass0Initer>::initer; \
}


#define rl_base_virtclass0(mp_Class) \
	static Base::boolean zrl_BaseClass0Reflection(void* mem, void* thisptr){  \
		new(mem) ReflectionLib::TReflection<mp_Class>(*static_cast<mp_Class*>(thisptr)); \
		return true; \
	} \
	static byte* zrl_BaseClass0VirtualClassObjectAddres(void* thisptr){  \
		return (byte*)static_cast<mp_Class*>(thisptr); \
	} \
	static void zrl_BaseClass0Init(){  \
		ReflectionLib::Private::ClassDesc*& cd = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
		Base::boolean bIns; \
		ReflectionLib::Private::BaseClassDesc* d = lib.giveBaseClassDesc(bIns,*cd,Base::constString(#mp_Class)); \
		if(bIns){ \
			d->size = sizeof(mp_Class); \
			d->type = ReflectionLib::typeOf<mp_Class>(); \
			d->reflection = &zrl_BaseClass0Reflection; \
			d->virtualAddresFunc = &zrl_BaseClass0VirtualClassObjectAddres; \
	} \
} \
class zrl_BaseClass0Initer \
{ \
public: \
	zrl_BaseClass0Initer() \
{ \
	ReflectionLib::Private::Library::instance().registerBaseClass(&zrl_BaseClass0Init); \
} \
}; \
	THIS_IS_USED void* zrl_BaseClass0IniterFunc() \
{ \
	return &::ReflectionLib::Private::StaticIniter<zrl_BaseClass0Initer>::initer; \
}
