#pragma once

#include "treflection.h"

namespace ReflectionLib
{

template<class t_Type>
class FieldReflectionMethods
{
public:
	static INLINE boolean reflection(void* mem, t_Type& obj)
	{
		new(mem) TReflection<t_Type>(obj);
		return true;
	}

	static INLINE boolean construct(void* mem, t_Type& obj, const Type& type, void** apArgs)
	{
		return false;
	}

	static INLINE boolean setref(t_Type& obj, void* val)
	{
		return false;
	}

	static INLINE void setChildDescParams(ReflectionLib::Private::ChildDesc& d)
	{
		d.info = EChildType_Normal;
		d.type = typeOf<t_Type>();
	}
};

template<class t_Type>
class FieldReflectionMethods<t_Type*>
{
public:
	static INLINE boolean reflection(void* mem, t_Type*& obj)
	{
		if(obj){
			ReflectionOf<t_Type>::value(mem, *obj);
			return true;
		}
		return false;
	}

	static INLINE boolean construct(void* mem, t_Type*& obj, const Type& type, void** apArgs)
	{
		return false;
	}

	static INLINE boolean setref(t_Type*& obj, void* val)
	{
		obj = (t_Type*)val;
		return true;
	}

	static INLINE void setChildDescParams(ReflectionLib::Private::ChildDesc& d)
	{
		d.info = EChildType_Reference;
		d.type = typeOf<t_Type>();
	}
};

template<class t_Type>
class FieldReflectionMethods< StrongPtr<t_Type> >
{
public:
	static INLINE boolean reflection(void* mem, StrongPtr<t_Type>& obj)
	{
		if(obj){
			ReflectionOf<t_Type>::value(mem, *obj);
			return true;
		}
		return false;
	}

	static INLINE boolean construct(void* mem, StrongPtr<t_Type>& obj, const Type& type, void** apArgs)
	{
		return false;
	}

	static INLINE boolean setref(StrongPtr<t_Type>& obj, void* val)
	{
		obj = (t_Type*)val;
		return true;
	}

	static INLINE void setChildDescParams(ReflectionLib::Private::ChildDesc& d)
	{
		d.info = EChildType_Reference;
		d.type = typeOf<t_Type>();
	}
};

template<class t_Type>
class FieldReflectionMethods< StrongPtrThreadSafe<t_Type> >
{
public:
	static INLINE boolean reflection(void* mem, StrongPtrThreadSafe<t_Type>& obj)
	{
		if(obj){
			ReflectionOf<t_Type>::value(mem, *obj);
			return true;
		}
		return false;
	}

	static INLINE boolean construct(void* mem, StrongPtrThreadSafe<t_Type>& obj, const Type& type, void** apArgs)
	{
		return false;
	}

	static INLINE boolean setref(StrongPtrThreadSafe<t_Type>& obj, void* val)
	{
		obj = (t_Type*)val;
		return true;
	}

	static INLINE void setChildDescParams(ReflectionLib::Private::ChildDesc& d)
	{
		d.info = EChildType_Reference;
		d.type = typeOf<t_Type>();
	}
};

template<class t_Type>
class FieldReflectionMethods< WeakPtr<t_Type> >
{
public:
	static INLINE boolean reflection(void* mem, WeakPtr<t_Type>& obj)
	{
		if(obj){
			ReflectionOf<t_Type>::value(mem, *obj.get());
			return true;
		}
		return false;
	}

	static INLINE boolean construct(void* mem, WeakPtr<t_Type>& obj, const Type& type, void** apArgs)
	{
		return false;
	}

	static INLINE boolean setref(WeakPtr<t_Type>& obj, void* val)
	{
		obj = (t_Type*)val;
		return true;
	}

	static INLINE void setChildDescParams(ReflectionLib::Private::ChildDesc& d)
	{
		d.info = EChildType_Reference;
		d.type = typeOf<t_Type>();
	}
};



template<class t_Type, class t_SmartPtr, class t_AllocatorWrapper>
class FieldReflectionMethods< Constructable<t_Type, t_SmartPtr, t_AllocatorWrapper> >
{
public:
	static INLINE boolean reflection(void* mem, Constructable<t_Type, t_SmartPtr, t_AllocatorWrapper>& obj)
	{
		if(obj.get()){
			ReflectionOf<t_Type>::value(mem, *obj.get());
			return true;
		}
		return false;
	}

	static INLINE boolean construct(void* mem, Constructable<t_Type, t_SmartPtr, t_AllocatorWrapper>& obj, const Type& type, void** apArgs)
	{
		t_Type* baseptr;
		void* derivedptr = obj.construct(baseptr, type);
		if(baseptr){
			boolean res = type.makeReflection(mem, derivedptr);
			Assert(res);
			return true;
		}
		return false; 
	}

	static INLINE boolean setref(Constructable<t_Type, t_SmartPtr, t_AllocatorWrapper>& obj, void* val)
	{
		return false;
	}

	static INLINE void setChildDescParams(ReflectionLib::Private::ChildDesc& d)
	{
		d.info = EChildType_Polymorphic;
		d.type = typeOf<t_Type>();
		ConstRange< Type>* semrange = new(ReflectionLib::Private::CustomAllocatorWrapper().allocator()) ConstRange< Type>();
		*semrange = ConstRange< Type>();
		d.localConstructorSemantic = semrange;
	}
};

template<class t_Type, class t_SmartPtr, class t_Arg0, class t_AllocatorWrapper>
class FieldReflectionMethods< Constructable1<t_Type, t_SmartPtr, t_Arg0, t_AllocatorWrapper> >
{
public:
	static INLINE boolean reflection(void* mem, Constructable1<t_Type, t_SmartPtr, t_Arg0, t_AllocatorWrapper>& obj)
	{
		if(obj.get()){
			ReflectionOf<t_Type>::value(mem, *obj.get());
			return true;
		}
		return false;
	}

	static INLINE boolean construct(void* mem, Constructable1<t_Type, t_SmartPtr, t_Arg0, t_AllocatorWrapper>& obj, const Type& type, void** apArgs)
	{
		t_Type* baseptr;
		void* derivedptr = obj.construct(baseptr, type, *(t_Arg0*)apArgs[0]);
		if(baseptr){
			boolean res = type.makeReflection(mem, derivedptr);
			Assert(res);
			return true;
		}
		return false; 
	}

	static INLINE boolean setref(Constructable1<t_Type, t_SmartPtr, t_Arg0, t_AllocatorWrapper>& obj, void* val)
	{
		return false;
	}

	static INLINE void setChildDescParams(ReflectionLib::Private::ChildDesc& d)
	{
		d.info = EChildType_Polymorphic;
		d.type = typeOf<t_Type>();
		Type* argtypes = new(ReflectionLib::Private::CustomAllocatorWrapper::allocator()) Type();
		argtypes[0] = typeOf<t_Arg0>();
		ConstRange< Type>* semrange = new(ReflectionLib::Private::CustomAllocatorWrapper::allocator()) ConstRange< Type>();
		*semrange = ConstRange< Type>(argtypes, 1);
		d.localConstructorSemantic = semrange;
	}
};

}

#define ReflectionField(mp_Name, mp_NativeType, mp_NativeName) \
	static Base::boolean zrl_reflection_##mp_Name(void* mem, void* thisptr){ \
		return ReflectionLib::FieldReflectionMethods<mp_NativeType>::reflection(mem, ((zrl_ThisClassType*)thisptr)->mp_NativeName); } \
	static Base::boolean zrl_construct_##mp_Name(void* mem, void* thisptr, const ReflectionLib::Type& type, void** apArgs){ \
		return ReflectionLib::FieldReflectionMethods<mp_NativeType>::construct(mem, ((zrl_ThisClassType*)thisptr)->mp_NativeName, type, apArgs ); } \
	static Base::boolean zrl_setref_##mp_Name(void* thisptr, void* val){ \
		return ReflectionLib::FieldReflectionMethods<mp_NativeType>::setref(((zrl_ThisClassType*)thisptr)->mp_NativeName, val ); } \
	static void zrl_FieldInitFunc_##mp_Name() \
	{ \
		ReflectionLib::Private::ClassDesc*& cd = ReflectionLib::TType<zrl_ThisClassType>::desc(); \
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); \
		Base::boolean bNew; \
		ReflectionLib::Private::ChildDesc* d = lib.giveChildDesc(bNew, *cd, _CS(#mp_Name)); \
		if(bNew){ \
			ReflectionLib::FieldReflectionMethods<mp_NativeType>::setChildDescParams(*d); \
			d->reflection = &zrl_reflection_##mp_Name; \
			d->construct = &zrl_construct_##mp_Name; \
			d->setref = &zrl_setref_##mp_Name; \
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
	THIS_IS_USED void* zrl_FiledIniterFunc_##mp_Name() \
	{ \
		return &::ReflectionLib::Private::StaticIniter<zrl_FiledIniter_##mp_Name>::initer; \
	}

#define rl_field(mp_Type, mp_Name) mp_Type mp_Name; ReflectionField(mp_Name, mp_Type, mp_Name)
