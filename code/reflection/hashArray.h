#pragma once

#include "treflection.h"

namespace ReflectionLib
{

template<class t_Value, class t_AllocatorWrapper, uint t_nReservedCells, uint t_nFillFactor>
class TType< Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> > : public DummyType
{
public:
	typedef Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> Class_t;

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
		return EMethaType_Map;
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

	static INLINE boolean			construct(void* mem, IAllocator& alc, void** args) 
	{
		Class_t* p = new(alc) Class_t();
		new(mem) TReflection<Class_t>(*p);
		return true;
	}

	static INLINE const Semantic*	localConstructorSemantic(uint n)						
	{
		return desc()->localConstructorSemantic; 
	}

	static INLINE const Semantic*	localConstructorSemantic(const ConstString& name)						
	{
		return desc()->localConstructorSemantic;
	}

	static INLINE boolean makeReflection(void* mem, void* ptr);

	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP

	static void zrl_Init()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); 
		Base::String name;
		name << _CS("HashArray<") << TType<t_Value>::name() << _CS(">");
		Base::boolean bIns;
		ReflectionLib::Private::ContainerClassDesc* d = lib.giveContainerClassDesc(bIns, name);
		desc() = d;
		if(bIns){
			d->id = lib.getFreeTypeId();
			d->type = ReflectionLib::typeOf<t_Value>();
			d->constructorSemantic = &d->constructorSemanticRange;
			d->localConstructorSemantic = &d->localConstructorSemanticRange;
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

template<class t_Value, class t_AllocatorWrapper, uint t_nReservedCells, uint t_nFillFactor>
typename TType< Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> >::zrl_Initer TType< Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> >::initer;

template<class t_Value, class t_AllocatorWrapper, uint t_nReservedCells, uint t_nFillFactor>
class TReflection< Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> > : public IReflection
{
public:
	typedef Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> Class_t;

	TReflection(Class_t& o) : obj(o){}

	REFLECTION_DECLARE_TYPE_INTERFACES(TType<Class_t>)

	//************** Instance **************//
	virtual void*	objectPtr()					{ return &this->obj; }

	//************** Content **************//
	virtual uint		size()
	{
		return this->obj.size();
	}

	virtual boolean		reflection(void* mem, uint n)			
	{
		if(n < this->obj.size()){
			new(mem) TReflection<t_Value>(this->obj[n].val);
			return true;
		}
		return false;
	}

	virtual boolean		reflection(void* mem, const ConstString& name)			
	{
		typename Class_t::Iterator it = this->obj.find(name);
		if(it != this->obj.end()){
			new(mem) TReflection<t_Value>(it->val);
			return true;
		}
		return false;
	}

	virtual ConstString name(uint n)
	{
		if(n < this->obj.size())
			return this->obj[n].key;
		return _CS("");
	}

	virtual int	index(const ConstString& name)
	{
		typename Class_t::Iterator it = this->obj.find(name);
		if(it != this->obj.end())
			return this->obj.indexOf(it);
		return -1;
	}

	virtual EChildMode	mode(uint n)
	{
		return EChildType_Normal;
	}

	virtual EChildMode	mode(const ConstString& name)
	{
		return EChildType_Normal;
	}

	virtual boolean		type(void* mem, uint n) 					
	{ 
		new(mem) TType<t_Value>();
		return true;
	}

	virtual boolean		type(void* mem, const ConstString& name) 					
	{ 
		new(mem) TType<t_Value>();
		return true;
	}

	virtual boolean insert(const ConstString& name)
	{
		boolean bIns;
		this->obj.insert(bIns, name);
		return bIns;
	}

	virtual boolean		erase(uint n)
	{
		if(n <= this->obj.size()){
			this->obj.eraseByIndex(n);
			return true;
		}
		return false;
	}

	virtual boolean		erase(const ConstString& name)	
	{
		return this->obj.erase(name);
	}

protected:
	Class_t& obj;
};

template<class t_Value, class t_AllocatorWrapper, uint t_nReservedCells, uint t_nFillFactor>
INLINE boolean TType< Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> >::makeReflection(void* mem, void* ptr)
{
	if(ptr){
		new(mem) TReflection< Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> >(*(Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor>*)ptr);
		return true;
	}
	return false;
}




template<class t_Value, class t_SmartPtr, class t_AllocatorWrapper, uint t_nReservedElements>
class TType< HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements> > : public DummyType
{
public:
	typedef HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements> Class_t;

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
		return EMethaType_PolyMap;
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

	static INLINE boolean			construct(void* mem, IAllocator& alc, void** args) 
	{
		Class_t* p = new(alc) Class_t();
		new(mem) TReflection<Class_t>(*p);
		return true;
	}

	static INLINE const Semantic*	localConstructorSemantic(uint n)						
	{
		return desc()->localConstructorSemantic; 
	}

	static INLINE const Semantic*	localConstructorSemantic(const ConstString& name)						
	{
		return desc()->localConstructorSemantic;
	}

	static INLINE boolean makeReflection(void* mem, void* ptr);

	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP

	static void zrl_Init()
	{
		ReflectionLib::Private::Library& lib = ReflectionLib::Private::Library::instance(); 
		Base::String name;
		name << _CS("HashArrayOfConstructables<") << TType<t_Value>::name() << _CS(">");
		Base::boolean bIns;
		ReflectionLib::Private::ContainerClassDesc* d = lib.giveContainerClassDesc(bIns, name);
		desc() = d;
		if(bIns){
			d->id = lib.getFreeTypeId();
			d->type = ReflectionLib::typeOf<t_Value>();
			d->constructorSemantic = &d->constructorSemanticRange;
			d->localConstructorSemantic = &d->localConstructorSemanticRange;
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

template<class t_Value, class t_SmartPtr, class t_AllocatorWrapper, uint t_nReservedElements>
typename TType< HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements> >::zrl_Initer TType< HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements> >::initer;

template<class t_Value, class t_SmartPtr, class t_AllocatorWrapper, uint t_nReservedElements>
class TReflection< HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements> > : public IReflection
{
public:
	typedef HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements> Class_t;

	TReflection(Class_t& o) : obj(o){}

	REFLECTION_DECLARE_TYPE_INTERFACES(TType<Class_t>)

	//************** Instance **************//
	virtual void*	objectPtr()					{ return &this->obj; }

	//************** Content **************//
	virtual uint		size()
	{
		return this->obj.size();
	}

	virtual boolean		reflection(void* mem, uint n)			
	{
		if(n < this->obj.size()){
			t_Value* p = this->obj[n].val.get();
			if(p){
				new(mem) TReflection<t_Value>(*p);
				return true;
			}
		}
		return false;
	}

	virtual boolean		reflection(void* mem, const ConstString& name)			
	{
		typename Class_t::Iterator it = this->obj.find(name);
		if(it != this->obj.end()){
			t_Value* p = it->val.get();
			if(p){
				new(mem) TReflection<t_Value>(*p);
				return true;
			}
		}
		return false;
	}

	virtual ConstString name(uint n)
	{
		if(n < this->obj.size())
			return this->obj[n].key;
		return _CS("");
	}

	virtual int	index(const ConstString& name)
	{
		typename Class_t::Iterator it = this->obj.find(name);
		if(it != this->obj.end())
			return this->obj.indexOf(it);
		return -1;
	}

	virtual EChildMode	mode(uint n)
	{
		return EChildType_Polymorphic;
	}

	virtual EChildMode	mode(const ConstString& name)
	{
		return EChildType_Polymorphic;
	}

	virtual boolean		type(void* mem, uint n) 					
	{ 
		new(mem) TType<t_Value>();
		return true;
	}

	virtual boolean		type(void* mem, const ConstString& name) 					
	{ 
		new(mem) TType<t_Value>();
		return true;
	}

	virtual boolean insert(const ConstString& name)
	{
		boolean bIns;
		this->obj.insert(bIns, name);
		return bIns;
	}

	virtual boolean		erase(uint n)
	{
		if(n <= this->obj.size()){
			this->obj.eraseByIndex(n);
			return true;
		}
		return false;
	}

	virtual boolean		erase(const ConstString& name)	
	{
		return this->obj.erase(name);
	}

	boolean		construct(void* mem, const ConstString& name, const Type& type, EChildMode info, void** apArgs = 0) 
	{
		t_Value* baseptr;
		void* objptr = this->obj.construct(baseptr, name, type);
		if(objptr){
			boolean res = type.makeReflection(mem, objptr);
			Assert(res);
			return true;
		}
		return false;
	}

	boolean		construct(void* mem, uint n, const Type& type, EChildMode info, void** apArgs = 0) 
	{
		t_Value* baseptr;
		void* objptr = this->obj.construct(baseptr, n, type);
		if(objptr){
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

	boolean		destruct(const ConstString& name)
	{
		this->obj.destruct(name);
		return true;
	}

protected:
	Class_t& obj;
};

template<class t_Value, class t_SmartPtr, class t_AllocatorWrapper, uint t_nReservedElements>
INLINE boolean TType< HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements> >::makeReflection(void* mem, void* ptr)
{
	if(ptr){
		new(mem) TReflection< HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements> >(*(HashArrayOfConstructables< t_Value, t_SmartPtr, t_AllocatorWrapper, t_nReservedElements>*)ptr);
		return true;
	}
	return false;
}

}