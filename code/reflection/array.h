#pragma once

#include "treflection.h"

namespace ReflectionLib
{

template<typename t_Value, typename t_Allocator, uint t_ReservedCount,  boolean t_IsRawValueType>
class TType< Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> > : public DummyType
{
public:
	typedef Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> Class_t;

	TType()
	{
		initer;
	}

	static ReflectionLib::Private::ContainerClassDesc*& desc()
	{
		static ReflectionLib::Private::ContainerClassDesc* gpDescInstance;
		return gpDescInstance;
	}

	static INLINE EMethaType	methaType()
	{
		return EMethaType_Array;
	}

	static INLINE ConstString	name()
	{
		return desc()->name;
	}

	static INLINE TypeId	id()
	{
		return desc()->id;
	}

	static INLINE EChildMode		childMode(uint i)
	{
		return EChildType_Normal;
	}

	static INLINE EChildMode		childMode(const ConstString& name)
	{
		return EChildType_Normal;
	}

	static INLINE const Semantic*	constructorSemantic()						
	{
		return desc()->constructorSemantic; 
	}

	static INLINE const Semantic*	localConstructorSemantic(uint n)						
	{
		return desc()->localConstructorSemantic; 
	}

	static INLINE const Semantic*	localConstructorSemantic(const ConstString& name)						
	{
		return desc()->localConstructorSemantic;
	}

	static INLINE boolean			construct(void* mem, IAllocator& alc, void** args) 
	{
		Class_t* p = new(alc) Class_t();
		new(mem) TReflection<Class_t>(*p);
		return true;
	}

	static INLINE const Semantic*	localConstructorSemantic()				
	{
		return desc()->localConstructorSemantic; 
	}

	static INLINE boolean makeReflection(void* mem, void* ptr);

	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP

	static void zrl_Init()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); 
		static Base::String name;
		name << _CS("Array<") << TType<t_Value>::name() << _CS(">");
		Base::boolean bIns; 
		ReflectionLib::Private::ContainerClassDesc* d = lib.giveContainerClassDesc(bIns, name); 
		desc() = d;
		if(bIns){
			d->id = lib.getFreeTypeId();
			d->type = ReflectionLib::typeOf<t_Value>();
		} 
	}

	class zrl_Initer 
	{ 
	public: 
		zrl_Initer() 
		{ 
			ReflectionLib::Private::Library::instance().registerContainerClass(&zrl_Init); 
		} 
	};

	static zrl_Initer initer;
};

template<typename t_Value, typename t_Allocator, uint t_ReservedCount,  boolean t_IsRawValueType>
typename TType< Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> >::zrl_Initer TType< Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> >::initer;


template<typename t_Value, typename t_Allocator, uint t_ReservedCount, boolean t_IsRawValueType>
class TReflection< Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> > : public IReflection
{
public:
	typedef Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> Class_t;

	TReflection(Class_t& o) : obj(o){}

	REFLECTION_DECLARE_TYPE_INTERFACES(TType<Class_t>)

	//************** Instance **************//
	virtual void*	objectPtr()					{ return &this->obj; }

	//************** Content **************//
	virtual uint		size()
	{
		return this->obj.size();
	}
	virtual boolean		resize(uint s)
	{
		this->obj.resize(s);
		return true;
	}
	virtual boolean		reflection(void* mem, uint n)			
	{
		if(n < this->obj.size()){
			new(mem) TReflection<t_Value>(this->obj[n]);
			return true;
		}
		return false;
	}

	virtual EChildMode	mode(uint n) { return EChildType_Normal; }

	virtual boolean		type(void* mem, uint n) 					
	{ 
		new(mem) TType<t_Value>();
		return true;
	}

	virtual boolean insert(uint n)
	{
		if(n <= this->obj.size()){
			this->obj.insertDefault(n);
			return true;
		}
		return false;
	}

	virtual boolean		erase(uint n) 								
	{
		if(n <= this->obj.size()){
			this->obj.erase(n);
			return true;
		}
		return false;
	}

	virtual boolean		xerase(uint n) 								
	{
		if(n <= this->obj.size()){
			this->obj.xerase(n);
			return true;
		}
		return false;
	}


protected:
	Class_t& obj;
};

template<typename t_Value, typename t_Allocator, uint t_ReservedCount,  boolean t_IsRawValueType>
INLINE boolean TType< Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> >::makeReflection(void* mem, void* ptr)
{
	if(ptr){
		new(mem) TReflection< Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> >(*(Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType>*)ptr);
		return true;
	}
	return false;
}


template<typename t_Value, class t_SmartPtr, typename t_AllocatorWrapper, uint t_ReservedCount>
class TType< ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount> > : public DummyType
{
public:
	typedef ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount> Class_t;

	TType()
	{
		initer;
	}

	static ReflectionLib::Private::ContainerClassDesc*& desc()
	{
		static ReflectionLib::Private::ContainerClassDesc* gpDescInstance;
		return gpDescInstance;
	}

	static INLINE EMethaType	methaType()
	{
		return EMethaType_Array;
	}

	static INLINE ConstString	name()
	{
		return desc()->name;
	}

	static INLINE TypeId	id()
	{
		return desc()->id;
	}

	static INLINE EChildMode		childMode(uint i)
	{
		return EChildType_Polymorphic;
	}

	static INLINE EChildMode		childMode(const ConstString& name)
	{
		return EChildType_Polymorphic;
	}

	static INLINE const Semantic*	constructorSemantic()						
	{
		return desc()->constructorSemantic; 
	}

	static INLINE const Semantic*	localConstructorSemantic(uint n)						
	{
		return desc()->localConstructorSemantic; 
	}

	static INLINE const Semantic*	localConstructorSemantic(const ConstString& name)						
	{
		return desc()->localConstructorSemantic;
	}

	static INLINE boolean			construct(void* mem, IAllocator& alc, void** args) 
	{
		Class_t* p = new(alc) Class_t();
		new(mem) TReflection<Class_t>(*p);
		return true;
	}

	static INLINE const Semantic*	localConstructorSemantic()				
	{
		return desc()->localConstructorSemantic;
	}

	static INLINE boolean makeReflection(void* mem, void* ptr);

	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP

	static void zrl_Init()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); 
		static Base::String name;
		name << _CS("ArrayOfConstructables<") << TType<t_Value>::name() << _CS(">>");
		Base::boolean bIns; 
		ReflectionLib::Private::ContainerClassDesc* d = lib.giveContainerClassDesc(bIns, name);
		desc() = d;
		if(bIns){
			d->id = lib.getFreeTypeId();
			d->type = ReflectionLib::typeOf<t_Value>();
		} 
	}

	class zrl_Initer 
	{ 
	public: 
		zrl_Initer() 
		{ 
			ReflectionLib::Private::Library::instance().registerContainerClass(&zrl_Init); 
		} 
	};

	static zrl_Initer initer;
};

template<typename t_Value, class t_SmartPtr, typename t_AllocatorWrapper, uint t_ReservedCount>
typename TType< ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount> >::zrl_Initer TType< ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount> >::initer;


template<typename t_Value, class t_SmartPtr, typename t_AllocatorWrapper, uint t_ReservedCount>
class TReflection< ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount> > : public IReflection
{
public:
	typedef ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount> Class_t;

	TReflection(Class_t& o) : obj(o){}

	REFLECTION_DECLARE_TYPE_INTERFACES(TType<Class_t>)

	//************** Instance **************//
	virtual void*	objectPtr()					{ return &this->obj; }

	//************** Content **************//
	virtual uint		size()
	{
		return this->obj.size();
	}
	virtual boolean		resize(uint s)
	{
		this->obj.resize(s);
		return true;
	}
	virtual boolean		reflection(void* mem, uint n)			
	{
		if(n < this->obj.size()){
			t_Value* p = this->obj[n].get();
			if(p){
				new(mem) TReflection<t_Value>(*p);
				return true;
			}
		}
		return false;
	}

	virtual EChildMode	mode(uint n) { return EChildType_Polymorphic; }

	virtual boolean		type(void* mem, uint n) 					
	{ 
		new(mem) TType<t_Value>();
		return true;
	}

	virtual boolean insert(uint n)
	{
		if(n <= this->obj.size()){
			this->obj.insertDefault(n);
			return true;
		}
		return false;
	}

	virtual boolean		erase(uint n) 								
	{
		if(n <= this->obj.size()){
			this->obj.erase(n);
			return true;
		}
		return false;
	}

	virtual boolean		xerase(uint n) 								
	{
		if(n <= this->obj.size()){
			this->obj.xerase(n);
			return true;
		}
		return false;
	}

	boolean		construct(void* mem, uint n, const Type& type, EChildMode info, void** apArgs = 0) 
	{
		void* objptr = 0;
		t_Value* baseptr = 0;
		if(n < this->obj.size() && (objptr = this->obj.construct(baseptr, n, type))){
			boolean res = type.makeReflection(mem, objptr);
			Assert(res);
			return true;
		}
		return false;
	}

	boolean		destruct(uint n)
	{
		if(n < this->obj.size()){
			this->obj.destruct(n);
			return true;
		}
		return false;
	}

protected:
	Class_t& obj;
};

template<typename t_Value, class t_SmartPtr, typename t_AllocatorWrapper, uint t_ReservedCount>
INLINE boolean TType< ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount> >::makeReflection(void* mem, void* ptr)
{
	if(ptr){
		new(mem) TReflection< ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount> >(*(ReflectionLib::ArrayOfConstructables<t_Value, t_SmartPtr, t_AllocatorWrapper, t_ReservedCount>*)ptr);
		return true;
	}
	return false;
}

}