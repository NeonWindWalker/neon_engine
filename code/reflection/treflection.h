#pragma once

#include "ttype.h"

namespace ReflectionLib
{

MPL_CLASS_HAS_METHOD(rl_setVersion)
MPL_CLASS_HAS_METHOD(rl_getVersion)
MPL_CLASS_HAS_METHOD(rl_constuctorValues)

template<class t_Class, bool hasmethod>
class rl_constuctorValues_switcher
{
public:
	static INLINE uint constuctorValues(void** apArgs, IAllocator& alc)
	{
		return t_Class::rl_constuctorValues(apArgs, alc);
	}
};

template<class t_Class>
class rl_constuctorValues_switcher<t_Class, false>
{
public:
	static INLINE uint constuctorValues(void** apArgs, IAllocator& alc)
	{
		return 0;
	}
};

template<class t_Class, bool hasmethod>
class rl_setVersion_switcher
{
public:
	static INLINE boolean setVersion(const uint64& v)
	{
		return t_Class::rl_setVersion(v);
	}
};

template<class t_Class>
class rl_setVersion_switcher<t_Class, false>
{
public:
	static INLINE boolean setVersion(const uint64& v)
	{
		return false;
	}
};

template<class t_Class, bool hasmethod>
class rl_getVersion_switcher
{
public:
	static INLINE boolean getVersion(uint64& v)
	{
		return t_Class::rl_getVersion(v);
	}
};

template<class t_Class>
class rl_getVersion_switcher<t_Class, false>
{
public:
	static INLINE boolean getVersion(uint64& v)
	{
		return false;
	}
};

namespace Private
{
	static INLINE boolean endcastImpl(void* mem, void* ptr)
	{
		return false;
	}

	static INLINE boolean endcastImpl(void* mem, IReflective* ptr)
	{
		ptr->constructReflection(mem);
		return true;
	}

	static INLINE boolean makeBaseClassReflectionImpl(void* mem, const BaseClassDesc& desc, void* ptr)
	{
		byte* thisptr = (byte*)ptr + desc.offset;
		return (*desc.reflection)(mem, thisptr);
	}

	static INLINE boolean castImpl(void* mem, const ReflectionLib::Private::BaseClassDescArray& arr, void* ptr, void* fromObj, uint fromObjSize)
	{
		integer offset = reinterpret_cast<integer>(fromObj) - reinterpret_cast<integer>(ptr);
		foreach(e, arr)
		{
			if(((int)offset - (int)e->offset >= 0 && (int)offset + (int)fromObjSize - (int)e->offset <= (int)e->size) || ((int)e->offset - (int)offset >= 0 && (int)e->offset + e->size - (int)offset <= (int)fromObjSize))
				return makeBaseClassReflectionImpl(mem, *e, ptr);
		}
		return false;
	}
}


template<class t_Class>
class TReflection : public IReflection
{
public:
	TReflection(t_Class& obj)
	{
		this->ptr = &obj;
	}

	REFLECTION_DECLARE_TYPE_INTERFACES(TType<t_Class>)

	//************** Instance **************//
	virtual void*	objectPtr()					{ return ptr; }
	virtual void*	objectPtrAndSize(uint& s)	{ s = sizeof(t_Class); return ptr; }
	virtual boolean	setVersion(const uint64& v)	{ return rl_setVersion_switcher<t_Class, class_has_method_rl_setVersion<t_Class>::value>::setVersion(v); }
	virtual boolean getVersion(uint64& v)		{ return rl_getVersion_switcher<t_Class, class_has_method_rl_getVersion<t_Class>::value>::getVersion(v); }
	
	

	virtual boolean endcast(void* mem)
	{
		return ReflectionLib::Private::endcastImpl(mem, ptr);
	}

	virtual boolean cast(void* mem, TypeId typeId)
	{
		ReflectionLib::Private::ClassDesc* d = TType<t_Class>::desc();
		if(d->id == typeId){
			new(mem) TReflection<t_Class>(*this);
			return true;
		}
		
		Reflection e;
		if(ReflectionLib::Private::endcastImpl(e.data, this->ptr))
		{
			ReflectionLib::Private::ClassDesc* ed = (ReflectionLib::Private::ClassDesc*)e.type().getClassDesc();
			if(ed)
			{
				if(ed->id == typeId){
					for(uint i=0; i < Reflection::gcVoidPtrCount; ++i)
						((void**)mem)[i] = e.data[i];
					return true;
				}

				ReflectionLib::Private::BaseClassIdDictionary::Iterator it = ed->baseClassesById.find(typeId.number());
				if(it != ed->baseClassesById.end()){
					uint s;
					void* p = e.objectPtrAndSize(s);
					return Private::castImpl(mem, it->val, this->ptr, p, s);
				}
			}
		}
		
		ReflectionLib::Private::BaseClassIdDictionary::Iterator it = d->baseClassesById.find(typeId.number());
		if(it != d->baseClassesById.end())
			return ReflectionLib::Private::makeBaseClassReflectionImpl(mem, it->val[0], this->ptr);
		
		return false;
	}
	

	uint	constuctorValues(void** apArgs, IAllocator& alc)
	{
		return rl_constuctorValues_switcher<t_Class, class_has_method_rl_constuctorValues<t_Class>::value>::constuctorValues(apArgs, alc);
	}

	boolean	destruct()
	{
		delete ptr; //FIXME
		return true;
	}

	//************** Content **************//
	uint		size()
	{
		return TType<t_Class>::childsCount();
	}
	ConstString name(uint n) 									
	{
		return TType<t_Class>::childName(n);
	}
	int		index(const ConstString& name)					
	{
		return TType<t_Class>::childIndex(name);
	}

	EChildMode mode(uint n)
	{
		return TType<t_Class>::childMode(n);
	}
	EChildMode mode(const ConstString& name)					
	{
		return TType<t_Class>::childMode(name);
	}

	boolean		reflection(void* mem, uint n)			
	{
		ReflectionLib::Private::ClassDesc* d = TType<t_Class>::desc();
		if(n < d->childs.size()){
			const ReflectionLib::Private::ChildDesc& ff = *d->childs[n].val;
			byte* thisptr = ff.addresing == 0 ? (byte*)ptr + ff.offset : ff.addresing((byte*)ptr + ff.offset);
			return (*ff.reflection)(mem, thisptr);
		}
		return false;
	}

	boolean		reflection(void* mem, const ConstString& name)	
	{
		ReflectionLib::Private::ClassDesc* d = TType<t_Class>::desc();
		ReflectionLib::Private::ChildDictionary::Iterator it = d->childs.find(name);
		if(it != d->childs.end()){
			const ReflectionLib::Private::ChildDesc& ff = *it->val;
			byte* thisptr = ff.addresing == 0 ? (byte*)ptr + ff.offset : ff.addresing((byte*)ptr + ff.offset);
			return (*ff.reflection)(mem, thisptr);
		}
		return false;
	}

	boolean		type(void* mem, uint n)
	{
		return ReflectionLib::TType<t_Class>::child(mem, n);
	}

	boolean		type(void* mem, const ConstString& name)
	{
		return ReflectionLib::TType<t_Class>::child(mem, name);
	}

	boolean		construct(void* mem, uint n, const Type& type, EChildMode info, void** apArgs = 0)
	{
		ReflectionLib::Private::ClassDesc* d = TType<t_Class>::desc();
		if(n < d->childs.size()){
			const ReflectionLib::Private::ChildDesc& ff = *d->childs[n].val;
			byte* thisptr = ff.addresing == 0 ? (byte*)ptr + ff.offset : ff.addresing((byte*)ptr + ff.offset);
			return (*ff.construct)(mem, thisptr, type, apArgs);
		}
		return false;
	}

	boolean		construct(void* mem, const ConstString& name, const Type& type, EChildMode info, void** apArgs = 0)
	{
		ReflectionLib::Private::ClassDesc* d = TType<t_Class>::desc();
		ReflectionLib::Private::ChildDictionary::Iterator it = d->childs.find(name);
		if(it != d->childs.end()){
			const ReflectionLib::Private::ChildDesc& ff = *it->val;
			byte* thisptr = ff.addresing == 0 ? (byte*)ptr + ff.offset : ff.addresing((byte*)ptr + ff.offset);
			return (*ff.construct)(mem, thisptr, type, apArgs);
		}
		return false;
	}
	

	boolean		setref(uint n, void* val)
	{
		ReflectionLib::Private::ClassDesc* d = TType<t_Class>::desc();
		if(n < d->childs.size()){
			const ReflectionLib::Private::ChildDesc& ff = *d->childs[n].val;
			byte* thisptr = ff.addresing == 0 ? (byte*)ptr + ff.offset : ff.addresing((byte*)ptr + ff.offset);
			return (*ff.setref)(thisptr, val);
		}
		return false;
	}

	boolean		setref(const ConstString& name, void* val) 	
	{
		ReflectionLib::Private::ClassDesc* d = TType<t_Class>::desc();
		ReflectionLib::Private::ChildDictionary::Iterator it = d->childs.find(name);
		if(it != d->childs.end()){
			const ReflectionLib::Private::ChildDesc& ff = *it->val;
			byte* thisptr = ff.addresing == 0 ? (byte*)ptr + ff.offset : ff.addresing((byte*)ptr + ff.offset);
			return (*ff.setref)(thisptr, val);
		}
		return false;
	}

protected:
	t_Class* ptr;
};

template<class t_Class>
INLINE boolean TType<t_Class>::makeReflection(void* mem, void* ptr)
{
	if(ptr){
		new(mem) TReflection<t_Class>(*(t_Class*)ptr);
		return true;
	}
	return false;
}


}