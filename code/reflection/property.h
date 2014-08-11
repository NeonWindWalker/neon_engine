#pragma once

#include "treflection.h"
#include "enum.h"

#define ReflectionPropertyRef(mp_Type, mp_Name, mp_GetterCode, mp_SetterCode) \
	static Base::boolean zrl_reflection_##mp_Name(void* mem, void* thisptr){ \
		zrl_ThisClassType& obj = *(zrl_ThisClassType*)thisptr; \
		mp_Type* out;  \
		mp_GetterCode;  \
		ReflectionLib::constructReflection(mem, out); \
		return true; } \
	static Base::boolean zrl_setref_##mp_Name(void* thisptr, void* val){ \
		zrl_ThisClassType& obj = *(zrl_ThisClassType*)thisptr; \
		mp_Type* in = (mp_Type*)val; \
		mp_SetterCode;  \
		return true; } \
	static void zrl_ChildInitFunc_##mp_Name() \
	{ \
		ReflectionLib::Private::ClassDesc*& cd = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
		Base::boolean bNew; \
		ReflectionLib::Private::ChildDesc* d = lib.giveChildDesc(bNew, *cd, _CS(#mp_Name)); \
		if(bNew){ \
			d->type = ReflectionLib::typeOf<mp_Type>(); \
			d->info = ReflectionLib::EChildType_Reference; \
			d->reflection = &zrl_reflection_##mp_Name; \
			d->setref = &zrl_setref_##mp_Name; \
		} \
	} \
	class zrl_ChildIniter_##mp_Name \
	{ \
	public: \
	zrl_ChildIniter_##mp_Name() \
		{ \
			ReflectionLib::Private::Library::instance().registerChild(&zrl_ChildInitFunc_##mp_Name); \
		} \
	}; \
	THIS_IS_USED void* zrl_ChildIniterFunc_##mp_Name() \
	{ \
		return &::ReflectionLib::Private::StaticIniter<zrl_ChildIniter_##mp_Name>::initer; \
	}

#define ReflectionPropertyPoly(mp_Type, mp_Name, mp_GetterCode, mp_SetterCode, mp_ConstructCode) \
	static Base::boolean zrl_reflection_##mp_Name(void* mem, void* thisptr){ \
		zrl_ThisClassType& obj = *(zrl_ThisClassType*)thisptr; \
		mp_Type* out;  \
		mp_GetterCode;  \
		new(mem) ReflectionLib::TReflection<mp_NativeType>(out); \
		return true; } \
	static Base::boolean zrl_construct_##mp_Name(void* mem, void* thisptr, const Type& type){ \
		zrl_ThisClassType& obj = *(zrl_ThisClassType*)thisptr; \
		mp_Type* out; \
		mp_ConstructCode; \
		mp_Type* in = out; \
		mp_SetterCode; \
		new(mem) ReflectionLib::TReflection<mp_Type>(in); return true; } \
	static void zrl_ChildInitFunc_##mp_Name() \
	{ \
		ReflectionLib::Private::ClassDesc*& cd = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
		Base::boolean bNew; \
		ReflectionLib::Private::ChildDesc* d = lib.giveChildDesc(bNew, *cd, _CS(#mp_Name)); \
		if(bNew){ \
			d->type = ReflectionLib::typeOf<mp_NativeType>(); \
			d->info = ReflectionLib::EChildType_Polymorphic; \
			d->reflection = &zrl_reflection_##mp_Name; \
			d->construct = &zrl_construct_##mp_Name; \
		} \
	} \
class zrl_FiledIniter_##mp_Name \
	{ \
	public: \
	zrl_FiledIniter_##mp_Name() \
		{ \
		ReflectionLib::Private::Library::instance().registerChild(&zrl_ChildInitFunc_##mp_Name); \
		} \
	}; \
	THIS_IS_USED void* zrl_FiledIniterFunc_##mp_Name() \
	{ \
		return &::ReflectionLib::Private::StaticIniter<zrl_FiledIniter_##mp_Name>::initer; \
	}

#define ReflectionValueProperty(mp_Type, mp_Name, mp_Read, mp_GetterCode, mp_Write, mp_SetterCode) \
class zrl_PropertyReflection_##mp_Name : public ReflectionLib::IReflection \
{ \
public: \
	zrl_PropertyReflection_##mp_Name(zrl_ThisClassType& o) : obj(o) {} \
	REFLECTION_DECLARE_TYPE_INTERFACES(ReflectionLib::TType<mp_Type>) \
	virtual void*	objectPtr()					{ return 0; } \
	virtual Base::boolean get(ReflectionLib::TypeId id, void* v) { return ReflectionLib::TType<mp_Type>::valueGet(id, v, *this); } \
	virtual Base::boolean set(ReflectionLib::TypeId id, const void* v) { return ReflectionLib::TType<mp_Type>::valueSet(*this, id, v); } \
	virtual Base::boolean get(void* val) { mp_Type* out = (mp_Type*)val; mp_GetterCode; return mp_Read; } \
	virtual Base::boolean set(const void* val) { const mp_Type* in = (const mp_Type*)val; mp_SetterCode; return mp_Write; } \
	protected: \
		zrl_ThisClassType& obj; \
	}; \
	static Base::boolean zrl_reflection_##mp_Name(void* mem, void* thisptr){ new(mem) zrl_PropertyReflection_##mp_Name(*(zrl_ThisClassType*)thisptr); return true; } \
	static void zrl_PropertyInit_##mp_Name() \
	{ \
		ReflectionLib::Private::ClassDesc*& cd = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
		Base::boolean bNew; \
		ReflectionLib::Private::ChildDesc* d = lib.giveChildDesc(bNew, *cd, _CS(#mp_Name)); \
		if(bNew){ \
			d->type = ReflectionLib::typeOf<mp_Type>(); \
			d->info = ReflectionLib::EChildType_Normal; \
			d->reflection = &zrl_reflection_##mp_Name; \
		} \
	} \
	class zrl_PropertyIniter_##mp_Name \
	{ \
	public: \
		zrl_PropertyIniter_##mp_Name() \
		{ \
			ReflectionLib::Private::Library::instance().registerChild(&zrl_PropertyInit_##mp_Name); \
		} \
	}; \
	THIS_IS_USED void* zrl_PropertyIniterFunc_##mp_Name() \
	{ \
		return &::ReflectionLib::Private::StaticIniter<zrl_PropertyIniter_##mp_Name>::initer; \
	}

#define rl_val_property(mp_Type, mp_Name) ReflectionValueProperty(mp_Type, mp_Name, true, *out = obj.mp_Name(), true, obj.mp_Name(*in) )
#define rl_ref_property(mp_Type, mp_Name) ReflectionPropertyRef(mp_Type, mp_Name, out = obj.mp_Name().get(), obj.mp_Name(in) )
#define rl_ptr_property(mp_Type, mp_Name) ReflectionPropertyRef(mp_Type, mp_Name, out = obj.mp_Name(), obj.mp_Name(in) )

//#define rl_property_strong_poly_ex(mp_Type, mp_Name, mp_ConstructMethod) ReflectionPropertyPoly(mp_Type, mp_Name, out = obj.mp_Name().get(), obj.mp_Name(in), out = obj.mp_ConstructMethod(type) )
//#define rl_property_strong_poly(mp_Type, mp_Name) ReflectionPropertyPoly(mp_Type, mp_Name, out = obj.mp_Name().get(), obj.mp_Name(in), out = type.construct(Base::getGlobalAllocator(), 0).objectPtr() )

