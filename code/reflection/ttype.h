#pragma once

#include "library.h"

namespace ReflectionLib
{

MPL_CLASS_HAS_METHOD(rl_construct)

template<class t_Class, bool hasconstructor, bool hasmethod>
class rl_construct_switcher
{
public:
	static INLINE boolean construct(void* mem, IAllocator& alc, void** args)
	{
		t_Class* p = t_Class::rl_construct(alc,args);
		constructReflection(mem, p);
		return true;
	}
};

template<class t_Class, bool hasmethod>
class rl_construct_switcher<t_Class, false, hasmethod>
{
public:
	static INLINE boolean construct(void* mem, IAllocator& alc, void** args)
	{
		new(mem) ReflectionLib::NullReflection<t_Class>();
		return true;
	}
};

template<class t_Class>
class rl_construct_switcher<t_Class, true, false>
{
public:
	static INLINE boolean construct(void* mem, IAllocator& alc, void** args)
	{
		t_Class* p = new(alc) t_Class();
		new(mem) ReflectionLib::TReflection<t_Class>(*p);
		return true;
	}
};


template<class t_Class>
class TType : public DummyType
{
public:

	static ReflectionLib::Private::ClassDesc*& desc()
	{
		static ReflectionLib::Private::ClassDesc* gpDescInstance;
		return gpDescInstance;
	}

	TType(){}

	virtual void* _getClassDesc()const { return desc(); }

	static INLINE EMethaType	methaType()
	{
		return EMethaType_Class;
	}

	static INLINE ConstString	name()
	{
		return desc()->name;
	}

	static INLINE TypeId		id()
	{
		return desc()->id;
	}

	static INLINE uint			childsCount()
	{ 
		return desc()->childs.size();
	}

	static INLINE ConstString	childName(uint i)
	{ 
		ReflectionLib::Private::ClassDesc* d = desc();
		if(i < d->childs.size())
			return d->childs[i].key;
		return _CS("");
	}

	static INLINE uint			childIndex(const ConstString& name)
	{ 
		ReflectionLib::Private::ClassDesc* d = desc();
		ReflectionLib::Private::ChildDictionary::Iterator it = d->childs.find(name);
		if(it != d->childs.end())
			return d->childs.indexOf(it);
		return d->childs.size();
	}

	static INLINE boolean		child(void* mem, uint n)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		if(n < d->childs.size()){
			*(Type*)mem = d->childs[n].val->type;
			return true;
		}
		return false;
	}

	static INLINE boolean		child(void* mem, const ConstString& name)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		ReflectionLib::Private::ChildDictionary::Iterator it = d->childs.find(name);
		if(it != d->childs.end()){
			*(Type*)mem = it->val->type;
			return true;
		}
		return false;
	}

	static INLINE EChildMode		childMode(uint i)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		if(i < d->childs.size())
			return d->childs[i].val->info;
		return (EChildMode)0;
	}

	static INLINE const Attributes*	childAttributes(const ConstString& name)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		ReflectionLib::Private::ChildDictionary::Iterator it = d->childs.find(name);
		if(it != d->childs.end())
			return &it->val->attributes;
		return 0;
	}

	static INLINE const Attributes*	childAttributes(uint i)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		if(i < d->childs.size())
			return &d->childs[i].val->attributes;
		return 0;
	}

	static INLINE EChildMode		childMode(const ConstString& name)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		ReflectionLib::Private::ChildDictionary::Iterator it = d->childs.find(name);
		if(it != d->childs.end())
			return it->val->info;
		return (EChildMode)0;
	}

	static INLINE uint			basesCount() 								
	{ 
		return desc()->trueBaseClassesCount;
	}

	static INLINE ConstString	baseName(uint i)							
	{ 
		ReflectionLib::Private::ClassDesc* d = desc();
		if(i < d->trueBaseClassesCount)
			return d->baseClasses[i].key;
		return _CS("");
	}

	static INLINE uint			baseIndex(const ConstString& name)			
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		ReflectionLib::Private::BaseClassDictionary::Iterator it = d->baseClasses.find(name);
		if(d->baseClasses.indexOf(it) < d->trueBaseClassesCount)
			return d->baseClasses.indexOf(it);
		return d->baseClasses.size();
	}

	static INLINE boolean		base(void* mem, uint i)						
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		if(i < d->trueBaseClassesCount){
			*(Type*)mem = d->baseClasses[i].val[0].type;
			return true;
		}
		return false;
	}

	static INLINE boolean		base(void* mem, const ConstString& name)	
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		ReflectionLib::Private::BaseClassDictionary::Iterator it = d->baseClasses.find(name);
		if(d->baseClasses.indexOf(it) < d->trueBaseClassesCount){
			*(Type*)mem = it->val[0].type;
			return true;
		}
		return false;
	}

	static INLINE Base::boolean		base(void* mem, ReflectionLib::TypeId id)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		ReflectionLib::Private::BaseClassIdDictionary::Iterator it = d->baseClassesById.find(id.number());
		if(d->baseClassesById.indexOf(it) < d->trueBaseClassesCount){
			*(Type*)mem = it->val[0].type;
			return true;
		}
		return false;
	}

	static INLINE const Attributes*	attributes()
	{
		return &desc()->attributes;
	}

	static INLINE const Semantic*	constructorSemantic()
	{
		return desc()->constructorSemantic;
	}

	static INLINE const Semantic*	localConstructorSemantic(uint n)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		if(n < d->childs.size())
			return d->childs[n].val->localConstructorSemantic;
		return 0;
	}

	static INLINE const Semantic*	localConstructorSemantic(const ConstString& name)
	{
		ReflectionLib::Private::ClassDesc* d = desc();
		ReflectionLib::Private::ChildDictionary::Iterator it = d->childs.find(name);
		if(it != d->childs.end())
			return it->val->localConstructorSemantic;
		return 0;
	}

	static INLINE boolean			construct(void* mem, IAllocator& alc, void** args)
	{
		return t_Class::zrl_construct(mem, alc, args);
	}

	static INLINE boolean makeReflection(void* mem, void* ptr);

	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP
};

}