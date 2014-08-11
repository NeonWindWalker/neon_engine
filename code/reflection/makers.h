#pragma once

#include "ttype.h"
#include "treflection.h"

namespace ReflectionLib
{

template<>
class TType<IReflective> : public DummyType
{
public:
	TType(){}

	static const uint gId = 127;
	static INLINE EMethaType	methaType()								{ return EMethaType_Class; }
	static INLINE ConstString	name() 											{ return _CS("IReflection"); }
	static INLINE ReflectionLib::TypeId	id() 											{ return TypeId(gId); }
	static INLINE boolean construct(void* mem, IAllocator& alc, void** args) { return false; }
	static INLINE boolean makeReflection(void* mem, void* ptr);
	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP
};

template<>
class TReflection<IReflective> : public IReflection
{
public:
	IReflective& obj;
	TReflection(IReflective& val) : obj(val)			{ static_assert(sizeof(*this) <= ReflectionLib::Reflection::gcMaxReflectionImplimentationSize, "big TReflection"); }
	REFLECTION_DECLARE_TYPE_INTERFACES(TType<IReflective>)
	virtual void*	objectPtr()					{ return &obj; }
	virtual	boolean destruct()						{ delete &obj; return true; }
};

INLINE boolean TType<IReflective>::makeReflection(void* mem, void* ptr)
{
	if(ptr){
		new(mem) TReflection<IReflective>(*(IReflective*)ptr);
		return true;
	}
	return false;
}


template<class t_Class>
class ReflectionCompileTimeGuard
{
public:
	static inline void guardTest()
	{
		t_Class::zrl_ThisClassIsReflected_CompileTimeGuardFlag();
	}
};

template<typename t_Value, typename t_Allocator, uint t_ReservedCount,  boolean t_IsRawValueType>
class ReflectionCompileTimeGuard< Base::Array<t_Value, t_Allocator, t_ReservedCount, t_IsRawValueType> >
{
	public: static inline void guardTest(){}
};


template<class t_Value, class t_AllocatorWrapper, uint t_nReservedCells, uint t_nFillFactor>
class ReflectionCompileTimeGuard< Base::HashArray< Pair<String, t_Value>, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor> >
{
	public: static inline void guardTest(){}
};

#define DeclareValueReflectionCompileTimeGuard(mp_Class) template<> class ReflectionCompileTimeGuard<mp_Class> { public: static inline void guardTest(){} };
DeclareValueReflectionCompileTimeGuard(int8)
DeclareValueReflectionCompileTimeGuard(uint8)
DeclareValueReflectionCompileTimeGuard(int16)
DeclareValueReflectionCompileTimeGuard(uint16)
DeclareValueReflectionCompileTimeGuard(int32)
DeclareValueReflectionCompileTimeGuard(uint32)
DeclareValueReflectionCompileTimeGuard(int64)
DeclareValueReflectionCompileTimeGuard(uint64)
DeclareValueReflectionCompileTimeGuard(float)
DeclareValueReflectionCompileTimeGuard(double)
DeclareValueReflectionCompileTimeGuard(Base::String)
DeclareValueReflectionCompileTimeGuard(Base::ConstString)
DeclareValueReflectionCompileTimeGuard(Base::BinaryData)
DeclareValueReflectionCompileTimeGuard(Base::ConstBinaryData)
DeclareValueReflectionCompileTimeGuard(Type)
DeclareValueReflectionCompileTimeGuard(Reflection)

DeclareValueReflectionCompileTimeGuard(Vector2i);
DeclareValueReflectionCompileTimeGuard(Vector3i);
DeclareValueReflectionCompileTimeGuard(Vector4i);
DeclareValueReflectionCompileTimeGuard(Vector2f);
DeclareValueReflectionCompileTimeGuard(Vector3f);
DeclareValueReflectionCompileTimeGuard(Vector4f);
DeclareValueReflectionCompileTimeGuard(Vector2d);
DeclareValueReflectionCompileTimeGuard(Vector3d);
DeclareValueReflectionCompileTimeGuard(Vector4d);
DeclareValueReflectionCompileTimeGuard(Quaternionf);
DeclareValueReflectionCompileTimeGuard(Quaterniond);
DeclareValueReflectionCompileTimeGuard(Matrix22f);
DeclareValueReflectionCompileTimeGuard(Matrix23f);
DeclareValueReflectionCompileTimeGuard(Matrix33f);
DeclareValueReflectionCompileTimeGuard(Matrix34f);
DeclareValueReflectionCompileTimeGuard(Matrix44f);
DeclareValueReflectionCompileTimeGuard(Matrix22d);
DeclareValueReflectionCompileTimeGuard(Matrix23d);
DeclareValueReflectionCompileTimeGuard(Matrix33d);
DeclareValueReflectionCompileTimeGuard(Matrix34d);
DeclareValueReflectionCompileTimeGuard(Matrix44d);

template<class t_Class>
class NullReflection : public IReflection
{
public:
	NullReflection(){}
	boolean valid(){ return false; }
	REFLECTION_DECLARE_TYPE_INTERFACES(TType<t_Class>)
};

template<class t_Class>
class TypeOf
{
public:
	static INLINE Type value()
	{
		Type t;
		value(t.data);
		return t;
	}
	static INLINE void value(void* mem)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TType<t_Class>();
	}
};

template<class t_Class>
class TypeOf<t_Class&>
{
public:
	static INLINE Type value()
	{
		Type t;
		value(t.data);
		return t;
	}
	static INLINE void value(void* mem)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TType<t_Class>();
	}
};

template<class t_Class>
class TypeOf<t_Class*>
{
public:
	static INLINE Type value()
	{
		Type t;
		value(t.data);
		return t;
	}
	static INLINE void value(void* mem)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TType<t_Class>();
	}
};

template<class t_Class>
class TypeOf<t_Class*&>
{
public:
	static INLINE Type value()
	{
		Type t;
		value(t.data);
		return t;
	}
	static INLINE void value(void* mem)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TType<t_Class>();
	}
};

template<class t_Class>
class TypeOf<const t_Class>
{
public:
	static INLINE Type value()
	{
		Type t;
		value(t.data);
		return t;
	}
	static INLINE void value(void* mem)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TType<t_Class>();
	}
};

template<class t_Class>
class TypeOf<const t_Class&>
{
public:
	static INLINE Type value()
	{
		Type t;
		value(t.data);
		return t;
	}
	static INLINE void value(void* mem)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TType<t_Class>();
	}
};

template<class t_Class>
class TypeOf<const t_Class*>
{
public:
	static INLINE Type value()
	{
		Type t;
		value(t.data);
		return t;
	}
	static INLINE void value(void* mem)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TType<t_Class>();
	}
};

template<class t_Class>
class TypeOf<const t_Class*&>
{
public:
	static INLINE Type value()
	{
		Type t;
		value(t.data);
		return t;
	}
	static INLINE void value(void* mem)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TType<t_Class>();
	}
};


template<class t_Class>
static INLINE Type typeOf()
{
	return TypeOf<t_Class>::value();
}

template<class t_Class>
INLINE void constructType(void* mem, t_Class& obj)
{
	TypeOf<t_Class>::value(mem);
}

template<class t_Class>
INLINE void constructType(void* mem, t_Class* obj)
{
	TypeOf<t_Class>::value(mem);
}

INLINE void constructType(void* mem, IReflective& obj)
{
	obj.constructType(mem);
}

INLINE void constructType(void* mem, const IReflective& obj)
{
	obj.constructType(mem);
}

INLINE void constructType(void* mem, IReflective* obj)
{
	if(obj)
		obj->constructType(mem);
	else
		new(mem) TType<IReflective>();
}

INLINE void constructType(void* mem, const IReflective* obj)
{
	if(obj)
		obj->constructType(mem);
	else
		new(mem) TType<IReflective>();
}





template<class t_Class>
class ReflectionOf
{
public:
	static INLINE Reflection value(t_Class& obj)
	{
		Reflection ret;
		value(ret.data, obj);
		return ret;
	}
	static INLINE void value(void* mem, t_Class& obj)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TReflection<t_Class>(obj);
	}
};

template<class t_Class>
class ReflectionOf<t_Class&>
{
public:
	static INLINE Reflection value(t_Class& obj)
	{
		Reflection ret;
		value(ret.data, obj);
		return ret;
	}
	static INLINE void value(void* mem, t_Class& obj)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TReflection<t_Class>(obj);
	}
};

template<class t_Class>
class ReflectionOf<const t_Class>
{
public:
	static INLINE Reflection value(const t_Class& obj)
	{
		Reflection ret;
		value(ret.data, obj);
		return ret;
	}
	static INLINE void value(void* mem, const t_Class& obj)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TReflection<t_Class>(*const_cast<t_Class*>(&obj));
	}
};

template<class t_Class>
class ReflectionOf<const t_Class&>
{
public:
	static INLINE Reflection value(const t_Class& obj)
	{
		Reflection ret;
		value(ret.data, obj);
		return ret;
	}
	static INLINE void value(void* mem, const t_Class& obj)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		new (mem) TReflection<t_Class>(*const_cast<t_Class*>(&obj));
	}
};

template<class t_Class>
class ReflectionOf<t_Class*>
{
public:
	static INLINE Reflection value(t_Class* obj)
	{
		Reflection ret;
		value(ret.data, obj);
		return ret;
	}
	static INLINE void value(void* mem, t_Class* obj)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		if (obj)
			new (mem) TReflection<t_Class>(*obj);
		else
			new (mem) NullReflection<t_Class>();
	}
};

template<class t_Class>
class ReflectionOf<t_Class*&>
{
public:
	static INLINE Reflection value(t_Class* obj)
	{
		Reflection ret;
		value(ret.data, obj);
		return ret;
	}
	static INLINE void value(void* mem, t_Class* obj)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		if(obj)
			new (mem) TReflection<t_Class>(*obj);
		else
			new (mem) NullReflection<t_Class>();
	}
};

template<class t_Class>
class ReflectionOf<const t_Class*>
{
public:
	static INLINE Reflection value(const t_Class* obj)
	{
		Reflection ret;
		value(ret.data, obj);
		return ret;
	}
	static INLINE void value(void* mem, const t_Class* obj)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		if(obj)
			new (mem) TReflection<t_Class>(const_cast<t_Class*>(obj));
		else
			new (mem) NullReflection<t_Class>();
	}
};

template<class t_Class>
class ReflectionOf<const t_Class*&>
{
public:
	static INLINE Reflection value(const t_Class* obj)
	{
		Reflection ret;
		value(ret.data, obj);
		return ret;
	}
	static INLINE void value(void* mem, const t_Class* obj)
	{
		ReflectionCompileTimeGuard<t_Class>::guardTest();
		if(obj)
			new (mem) TReflection<t_Class>(const_cast<t_Class*>(obj));
		else
			new (mem) NullReflection<t_Class>();
	}
};


template<class t_Class>
INLINE Reflection reflectionOfImpl(void* obj)
{
	return ReflectionOf<t_Class*>::value((t_Class*)obj);
}

template<class t_Class>
INLINE Reflection reflectionOfImplRef(void* obj)
{
	return ReflectionOf<t_Class>::value(*(t_Class*)obj);
}

template<class t_Class>
INLINE Reflection reflectionOfImpl(IReflective* obj)
{
	Reflection ret;
	if(obj)
		obj->constructReflection(ret.data);
	else
		new(ret.data) NullReflection<IReflective>();
	return ret;
}

template<class t_Class>
INLINE Reflection reflectionOfImplRef(IReflective* obj)
{
	Reflection ret;
	obj->constructReflection(ret.data);
	return ret;
}

template<class t_Class>
INLINE Reflection reflectionOf(t_Class& obj)
{
	return reflectionOfImplRef<t_Class>(&obj);
}

template<class t_Class>
INLINE Reflection reflectionOf(t_Class* obj)
{
	return reflectionOfImpl<t_Class>(obj);
}


template<class t_Value>
INLINE boolean Reflection::setref(const ConstString& name, t_Value* val)
{
	return setref(name, reflectionOf(val));
}

template<class t_SomeType>
INLINE Reflection Reflection::cast()const
{
	return cast(typeOf<t_SomeType>());
}



template<typename t_Arg0>
INLINE boolean Reflection::call(t_Arg0& arg0)const
{ 
	const MethodSemantic* sem = methodSemantic();
	if(!sem || sem->in.size() != 1 || sem->out.size() != 0)
		return false;
	if(typeOf<t_Arg0>() != sem->in[0])
		return false;
	void* inArgs[] = {(void*)&arg0};
	return methodCall(0, &inArgs[0]);
}

template<class t_Arg0, class t_Arg1>
INLINE boolean Reflection::call(t_Arg0& arg0, t_Arg1& arg1)const
{ 
	const MethodSemantic* sem = methodSemantic();
	if(!sem || sem->in.size() != 2 || sem->out.size() != 0)
		return false;
	if(typeOf<t_Arg0>() != sem->in[0] || typeOf<t_Arg1>() != sem->in[1])
		return false;
	void* inArgs[] = {(void*)&arg0, (void*)&arg1};
	return methodCall(0, &inArgs[0]);
}

template<class t_Arg0, class t_Arg1, class t_Arg2>
INLINE boolean Reflection::call(t_Arg0& arg0, t_Arg1& arg1, t_Arg2& arg2)const
{
	const MethodSemantic* sem = methodSemantic();
	if(!sem || sem->in.size() != 3 || sem->out.size() != 0)
		return false;
	if(typeOf<t_Arg0>() != sem->in[0] || typeOf<t_Arg1>() != sem->in[1] || typeOf<t_Arg2>() != sem->in[2])
		return false;
	void* inArgs[] = { (void*)&arg0, (void*)&arg1, (void*)&arg2 };
	return methodCall(0, &inArgs[0]);
}


template<class t_Out0>
INLINE boolean Reflection::rcall(t_Out0& ret)const
{
	const MethodSemantic* sem = methodSemantic();
	if(!sem || sem->in.size() != 0 || sem->out.size() != 1)
		return false;
	if(typeOf<t_Out0>() != sem->out[0])
		return false;
	void* outArgs[] = { (void*)&ret };
	void* inArgs[1] = { 0 };
	return methodCall(outArgs, &inArgs[0]);
}

template<class t_Out0, class t_Arg0>
INLINE boolean Reflection::rcall(t_Out0& ret, t_Arg0& arg0)const
{ 
	const MethodSemantic* sem = methodSemantic();
	if(!sem || sem->in.size() != 1 || sem->out.size() != 1)
		return false;
	if(typeOf<t_Arg0>() != sem->in[0] || typeOf<t_Out0>() != sem->out[0])
		return false;
	void* outArgs[] = {(void*)&ret};
	void* inArgs[] = {(void*)&arg0};
	return methodCall(outArgs, &inArgs[0]);
}

template<class t_Out0, class t_Arg0, class t_Arg1>
INLINE boolean Reflection::rcall(t_Out0& ret, t_Arg0& arg0, t_Arg1& arg1)const
{
	const MethodSemantic* sem = methodSemantic();
	if(!sem || sem->in.size() != 2 || sem->out.size() != 1)
		return false;
	if(typeOf<t_Arg0>() != sem->in[0] || typeOf<t_Arg1>() != sem->in[1] || typeOf<t_Out0>() != sem->out[0])
		return false;
	void* outArgs[] = { (void*)&ret };
	void* inArgs[] = { (void*)&arg0, (void*)&arg1 };
	return methodCall(outArgs, &inArgs[0]);
}

template<class t_Out0, class t_Arg0, class t_Arg1, class t_Arg2>
INLINE boolean Reflection::rcall(t_Out0& ret, t_Arg0& arg0, t_Arg1& arg1, t_Arg2& arg2)const
{
	const MethodSemantic* sem = methodSemantic();
	if(!sem || sem->in.size() != 3 || sem->out.size() != 1)
		return false;
	if(typeOf<t_Arg0>() != sem->in[0] || typeOf<t_Arg1>() != sem->in[1] || typeOf<t_Arg2>() != sem->in[2] || typeOf<t_Out0>() != sem->out[0])
		return false;
	void* outArgs[] = { (void*)&ret };
	void* inArgs[] = { (void*)&arg0, (void*)&arg1, (void*)&arg2 };
	return methodCall(outArgs, &inArgs[0]);
}


template<class t_Arg0>
INLINE Reflection Type::construct(IAllocator& alc, t_Arg0& arg0)const
{
	const Semantic* sem = constructorSemantic();
	if(!sem || sem->size() != 1)
		return Reflection();
	if(typeOf<t_Arg0>() != sem->at(0))
		return Reflection();

	void* args[] = {(void*)&arg0};
	return construct(alc, args);
}

template<class t_Arg0, class t_Arg1>
INLINE Reflection	Type::construct(IAllocator& alc, t_Arg0& arg0, t_Arg0& arg1)const
{
	const Semantic* sem = constructorSemantic();
	if(!sem || sem->size() != 2)
		return Reflection();
	if(typeOf<t_Arg0>() != sem->at(0) || typeOf<t_Arg1>() != sem->at(1))
		return Reflection();

	void* args[] = {(void*)&arg0,(void*)&arg1};
	return construct(alc, args);
}

template<class t_Type>
static INLINE t_Type* dynamicCast(t_Type* obj)
{
	return obj;
}

template<class t_Type>
static INLINE t_Type* dynamicCast(IReflective* obj)
{
	if(obj)
		return (t_Type*)reflectionOf(*obj).cast<t_Type>().objectPtr();
	else
		return 0;
}

}