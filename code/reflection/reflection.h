#pragma once

#include "type.h"

namespace ReflectionLib 
{

#define REFLECTION_MAX_ARGUMENTS 16

class IReflection
{
public:
	//************** Type **************//
	virtual void			type(void* mem)const=0;
	virtual EMethaType			typeMethaType()const=0;
	virtual ConstString		typeName()const=0;
	virtual TypeId			typeId()const=0;
	virtual const MethodSemantic*	methodSemantic()const=0;
	virtual const Semantic*	constructorSemantic()const=0;
	virtual const Semantic*	localConstructorSemantic(uint n)const=0;
	virtual const Semantic*	localConstructorSemantic(const ConstString& name)const=0;

	//************** Instance **************//
	virtual boolean valid() { return true; }
	virtual void*	objectPtr()					{ return 0; }
	virtual void*	objectPtrAndSize(uint& s)	{ s = 0; return 0; }
	virtual boolean	setVersion(const uint64& v)	{ return false; }
	virtual boolean getVersion(uint64& v)		{ return false; }
	virtual boolean cast(void* mem, TypeId id){ return false; }
	virtual boolean endcast(void* mem){ return false; }
	
	virtual uint	constuctorValues(void** apArgs, IAllocator& alc){ return 0; }
	virtual uint	localConstuctorValues(void** apArgs, IAllocator& alc){ return 0; }
	virtual boolean	destruct() { return false; }

	//************** Value **************//
	virtual boolean get(void* val) 	{ return false; }
	virtual boolean set(const void* val)	{ return false; }
	virtual boolean get(TypeId id, void* val) 	{ return false; }
	virtual boolean set(TypeId id, const void* val)	{ return false; }

	//************** Method **************//
	virtual boolean call(void** apOut, void** apArgs)	{ return false; }

	//************** Content **************//
	virtual uint		size() 											{ return 0; }
	virtual boolean		resize(uint s) 									{ return false; }
	virtual boolean		reflection(void* mem, uint n) 					{ return false; }
	virtual boolean		reflection(void* mem, const ConstString& name)	{ return false; }
	virtual ConstString name(uint n) 									{ return _CS(""); }
	virtual int			index(const ConstString& name)					{ return -1; }
	virtual EChildMode	mode(uint n) 									{ return (EChildMode)0; }
	virtual EChildMode	mode(const ConstString& name)					{ return (EChildMode)0; }
	virtual boolean		insert(const ConstString& name)					{ return false; }
	virtual boolean		insert(uint n)									{ return false; }

	virtual boolean		erase(uint n) 								{ return false; }
	virtual boolean		erase(const ConstString& name) 				{ return false; }
	virtual boolean		xerase(uint n) 								{ return false; }
	virtual boolean		xerase(const ConstString& name) 			{ return false; }

	virtual boolean		type(void* mem, uint n) 					{ return false; }
	virtual boolean		type(void* mem, const ConstString& name) 	{ return false; }
	virtual boolean		setref(uint n, void* val) 					{ return false; }
	virtual boolean		setref(const ConstString& name, void* val) 	{ return false; }
	virtual boolean		construct(void* mem, uint n, const Type& type, EChildMode info, void** apArgs = 0) 					{ return false; }
	virtual boolean		construct(void* mem, const ConstString& name, const Type& type, EChildMode info, void** apArgs = 0) 	{ return false; }
	virtual boolean		destruct(uint n) 					{ return false; }
	virtual boolean		destruct(const ConstString& name) 	{ return false; }
};

#define REFLECTION_DECLARE_TYPE_INTERFACES(mp_TypeClass)\
virtual void						type(void* mem)const				{ new(mem) mp_TypeClass(); }\
virtual ReflectionLib::EMethaType			typeMethaType()const							{ return mp_TypeClass::methaType(); }\
virtual Base::ConstString			typeName()const							{ return mp_TypeClass::name(); }\
virtual ReflectionLib::TypeId			typeId()const							{ return mp_TypeClass::id(); }\
virtual const ReflectionLib::MethodSemantic*	methodSemantic()const			{ return mp_TypeClass::methodSemantic(); }\
virtual const ReflectionLib::Semantic*	constructorSemantic()const				{ return mp_TypeClass::constructorSemantic(); }\
virtual const ReflectionLib::Semantic*	localConstructorSemantic(Base::uint n)const			{ return mp_TypeClass::localConstructorSemantic(n); }\
virtual const ReflectionLib::Semantic*	localConstructorSemantic(const Base::ConstString& name)const			{ return mp_TypeClass::localConstructorSemantic(name); }

class InvalidReflection : public IReflection
{
public:
	virtual boolean			valid()												{ return false; }
	virtual boolean			destroy()const										{ return false; }

	REFLECTION_DECLARE_TYPE_INTERFACES(InvalidType)
};

class Reflection
{
public:
	static const uint gcVoidPtrCount = 2;
	static const uint gcMaxReflectionImplimentationSize = gcVoidPtrCount * sizeof(void*);
	void* data[gcVoidPtrCount];

	INLINE Reflection()
	{ 
		new(data) InvalidReflection();
	}

	INLINE Reflection(const NoInitConstructoExtensionFlag& x)
	{
#ifdef DEBUG
		data[0] = 0;
		data[1] = 0;
#endif
	}

	INLINE Reflection(const Reflection& other)
	{
		data[0] = other.data[0];
		data[1] = other.data[1];
	}

	INLINE Reflection& operator = (const Reflection& other)
	{
		data[0] = other.data[0];
		data[1] = other.data[1];
		return *this;
	}

	//************** Type **************//
	INLINE Type			type()const
	{ 
		Type t;
		impl()->type(t.data);
		return t;
	}
	INLINE EMethaType		typeMethaType()const							{ return impl()->typeMethaType(); }
	INLINE ConstString		typeName()const								{ return impl()->typeName(); }
	INLINE TypeId			typeId()const								{ return impl()->typeId(); }
	INLINE const MethodSemantic*	methodSemantic()const						{ return impl()->methodSemantic(); }
	INLINE const Semantic*	constructorSemantic()const					{ return impl()->constructorSemantic(); }
	INLINE const Semantic*	localConstructorSemantic(uint n)const				{ return impl()->localConstructorSemantic(n); }
	INLINE const Semantic*	localConstructorSemantic(const ConstString& name)const				{ return impl()->localConstructorSemantic(name); }

	INLINE boolean validType() { return !typeId().number(); } 

	//************** Instance **************//
	INLINE boolean valid()const 				{ return impl()->valid(); }
	INLINE void* objectPtr()const				{ return impl()->objectPtr(); }
	INLINE void* objectPtrAndSize(uint& s)const	{ return impl()->objectPtrAndSize(s); }
	INLINE boolean setVersion(const uint64& v)	{ return impl()->setVersion(v); }
	INLINE boolean getVersion(uint64& v)		{ return impl()->getVersion(v); }
	INLINE Reflection cast(const Type& type)const //move to treflection
	{
		Reflection ret;
		if(impl()->cast(ret.data, type.id()))
			return ret;
		new (ret.data) InvalidReflection();
		return ret;
	}

	INLINE Reflection endcast()const
	{
		Reflection ret;
		if(impl()->endcast(ret.data))
			return ret;
		return *this;
	}

	template<class t_SomeType>
	INLINE Reflection cast()const;

	//************** Value **************//
	template<class t_Value>
	INLINE boolean get(t_Value& v)const 		{ return impl()->get(TType<t_Value>::id(), &v); }

	template<class t_Value>
	INLINE boolean set(const t_Value& v)const 	{ return impl()->set(TType<t_Value>::id(), &v); }

	//************** Method **************//
	//#pragma region Method
	INLINE boolean methodCall(void** apOut, void** apIn)const { return impl()->call(apOut, apIn); }

	INLINE boolean methodCall(ConstRange< Reflection> args)const
	{
		const MethodSemantic* sem = methodSemantic();
		if(!sem || sem->in.size() != args.size() || sem->out.size() != 0)
			return false;

		Array<void*> ptrs;
		ptrs.resize(args.size());
		for(uint n = 0; n < args.size(); ++n)
			ptrs[n] = args[n].objectPtr();

		return impl()->call(0, ptrs.first());
	}

	INLINE boolean call()const
	{ 
		const MethodSemantic* sem = methodSemantic();
		if(!sem || sem->in.size() != 0 || sem->out.size() != 0)
			return false;
		return methodCall(0,0);
	}

	template<typename t_Arg0>
	INLINE boolean call(t_Arg0& arg0)const;

	template<class t_Arg0, class t_Arg1>
	INLINE boolean call(t_Arg0& arg0, t_Arg1& arg1)const;

	template<class t_Arg0, class t_Arg1, class t_Arg2>
	INLINE boolean call(t_Arg0& arg0, t_Arg1& arg1, t_Arg2& arg2)const;


	template<class t_Out0>
	INLINE boolean rcall(t_Out0& ret)const;

	template<class t_Out0, class t_Arg0>
	INLINE boolean rcall(t_Out0& ret, t_Arg0& arg0)const;

	template<class t_Out0, class t_Arg0, class t_Arg1>
	INLINE boolean rcall(t_Out0& ret, t_Arg0& arg0, t_Arg1& arg1)const;

	template<class t_Out0, class t_Arg0, class t_Arg1, class t_Arg2>
	INLINE boolean rcall(t_Out0& ret, t_Arg0& arg0, t_Arg1& arg1, t_Arg2& arg2)const;

//#pragma endregion

	//************** Content **************//
	INLINE uint size()const
	{ 
		return impl()->size();
	}

	INLINE boolean resize(uint s)const
	{ 
		return impl()->resize(s);
	}
	
	INLINE Reflection reflection(const ConstString& name)const 							
	{
		Reflection ret;
		if(impl()->reflection(ret.data, name))
			return ret;

		new (ret.data) InvalidReflection();
		return ret;
	}

	INLINE Reflection reflection(uint n)const							
	{
		Reflection ret;
		if(impl()->reflection(ret.data, n))
			return ret;
		new (ret.data) InvalidReflection();
		return ret;
	}

	INLINE String name(uint n)const 					
	{
		return impl()->name(n);
	}

	INLINE int index(const ConstString& name)const
	{ 
		return impl()->index(name);
	}

	INLINE EChildMode mode(uint n)const 					
	{
		return impl()->mode(n);
	}

	INLINE EChildMode mode(const ConstString& name)const
	{ 
		return impl()->mode(name);
	}

	INLINE uint constuctorValues(void** apArgs, IAllocator& alc)const
	{ 
		return impl()->constuctorValues(apArgs, alc);
	}

	INLINE uint localConstuctorValues(void** apArgs, IAllocator& alc)const
	{ 
		return impl()->localConstuctorValues(apArgs, alc);
	}
	
	INLINE boolean insert(const ConstString& name)const
	{
		return impl()->insert(name);
	}

	INLINE boolean insert(uint n)const
	{
		return impl()->insert(n);
	}

	INLINE boolean erase(const ConstString& name)const
	{ 
		return impl()->erase(name);
	}

	INLINE boolean erase(uint n)const
	{ 
		return impl()->erase(n);
	}

	INLINE boolean xerase(const ConstString& name)const
	{ 
		return impl()->xerase(name);
	}

	INLINE boolean xerase(uint n)const
	{ 
		return impl()->xerase(n);
	}

	INLINE boolean destruct()
	{
		boolean ret = impl()->destruct();
		new(data) InvalidReflection();
		return ret;
	}


	INLINE void clear()const
	{ 
		int s = size();
		for(int i = s - 1; i >= 0; --i)
			erase(i);
	}

	INLINE boolean setref(uint n, const Reflection& val)
	{
		return impl()->setref(n, val.cast(type()).objectPtr());
	}

	INLINE boolean setref(const ConstString& name, const Reflection& val)
	{
		return impl()->setref(name, val.cast(type()).objectPtr());
	}

	template<class t_Value>
	INLINE boolean setref(const ConstString& name, t_Value* val);

	INLINE Type		type(uint n)
	{ 
		Type ret;
		if(impl()->type(ret.data, n))
			return ret;
		new(ret.data) InvalidType();
		return ret;
	}
	INLINE Type		type(const ConstString& name)
	{ 
		Type ret;
		if(impl()->type(ret.data, name))
			return ret;
		new(ret.data) InvalidType();
		return ret;
	}

	INLINE Reflection		construct(uint n, const Type& type, EChildMode info = (EChildMode)0, void** apArgs = 0)
	{
		Reflection ret;
		if(impl()->construct(ret.data, n, type, info, apArgs))
			return ret;
		
		new (ret.data) InvalidReflection();
		return ret;
	}

	INLINE Reflection		construct( const ConstString& name, const Type& type, EChildMode info = (EChildMode)0, void** apArgs = 0)
	{
		Reflection ret;
		if(impl()->construct(ret.data, name, type, info, apArgs))
			return ret;

		new (ret.data) InvalidReflection();
		return ret;
	}

	INLINE boolean		destruct(uint n)
	{
		return impl()->destruct(n);
	}

	INLINE boolean		destruct(const ConstString& name)
	{
		return impl()->destruct(name);
	}

	//************** Operators and Helpers **************//
	INLINE Reflection operator [] (uint n)const
	{ 
		Reflection ret;
		if(impl()->reflection(ret.data, n))
			return ret;
		new (ret.data) InvalidReflection();
		return ret;
	}

	Reflection operator () (const ConstString& name)const
	{
		return reflection(name);
	}

	Reflection reflectionByPath(const ConstString& name)const
	{
		Reflection pr = *this;
		const utf8* end = name.begin();
		do{
			const utf8* first = end + 1;
			end = find(first, name.end(), '.');
			pr = pr.reflection(ConstString(first, end));
		}while(end != name.end());
		return pr;
	}

	boolean deserialize(ConstString tokens);
	boolean serialize(String& str);
	boolean assign(Reflection other);

	boolean deserializeJSON(ConstString tokens);
	boolean serializeJSON(String& str);

	boolean serializeURLEncoded(String& str);

	IReflection* impl()const { return (IReflection*)data; }
};


INLINE Reflection Type::construct(IAllocator& alc, void** args)const
{
	Reflection ret;
	if(impl()->_construct(ret.data, alc, args))
		return ret;
	new (ret.data) InvalidReflection;
	return ret;
}




class IReflective
{
public:
	virtual ~IReflective(){}
	virtual void constructReflection(void* mem)=0;
	virtual void constructType(void* mem)const=0;

	INLINE Reflection reflection()
	{
		Reflection ret;
		constructReflection(ret.data);
		return ret;
	}

	INLINE Type type()const
	{
		Type ret;
		constructType(ret.data);
		return ret;
	}
};

#define rl_reflective \
	virtual void constructReflection(void* mem) { new(mem) ReflectionLib::TReflection<zrl_ThisClassType>(*this); } \
	virtual void constructType(void* mem)const { new(mem) ReflectionLib::TType<zrl_ThisClassType>(); }

}
