#pragma once

#include "seed.h"

namespace ReflectionLib 
{

struct Attributes
{
	String description;
};

struct TypeId
{
	uint32 id;
	INLINE TypeId(const TypeId& other) : id(other.id) {}
	INLINE TypeId() : id(0) {}
	INLINE TypeId(uint32 nid) : id(nid) {}
	INLINE TypeId& operator = (const TypeId& other) { id = other.id; return *this; }

	INLINE boolean operator == (const TypeId& other)
	{
		return id == other.id;
	}

	INLINE boolean operator != (const TypeId& other)
	{
		return id != other.id;
	}

	INLINE uint32 number()const { return id; }
	INLINE boolean valid()const { return id; }
};

class IType
{
public:
	virtual boolean			_valid()const 									{ return true; }
	virtual EMethaType		_methaType()const=0;
	virtual ConstString		_name()const=0;
	virtual TypeId			_id()const=0;

	virtual void*			_getClassDesc()const							{ return 0; }

	virtual uint			_childsCount()const 							{ return 0; }
	virtual ConstString		_childName(uint n)const							{ return _CS(""); }
	virtual int				_childIndex(const ConstString& name)const		{ return -1; }
	virtual boolean			_child(void* mem, uint n)const					{ return false; }
	virtual boolean			_child(void* mem, const ConstString& name)const	{ return false; }
	virtual EChildMode		_childMode(uint n)const							{ return (EChildMode)0; }
	virtual EChildMode		_childMode(const ConstString& name)const		{ return (EChildMode)0; }
	virtual const Attributes*	_childAttributes(uint n)const { return 0; }
	virtual const Attributes*	_childAttributes(const ConstString& name)const { return 0; }

	virtual uint			_basesCount()const 								{ return 0; }
	virtual ConstString		_baseName(uint i)const							{ return _CS(""); }
	virtual int				_baseIndex(const ConstString& name)const		{ return -1; }
	virtual boolean			_base(void* mem, uint n)const					{ return false; }
	virtual boolean			_base(void* mem, const ConstString& name)const	{ return false; }
	virtual boolean			_base(void* mem, TypeId id)const				{ return false; }

	virtual const MethodSemantic*	_methodSemantic()const					{ return 0; }
	virtual const Semantic*	_constructorSemantic()const						{ return 0; }
	virtual boolean			_construct(void* mem, IAllocator& alc, void** args)const { return false; }
	virtual const Semantic*	_localConstructorSemantic(uint n)const				{ return 0; }
	virtual const Semantic*	_localConstructorSemantic(const ConstString& name)const				{ return 0; }

	virtual uint			_enumsCount()const								{ return 0; }
	virtual ConstString		_enumName(uint i)const							{ return _CS(""); }
	virtual uint			_enumValue(uint i)const							{ return 0; }
	virtual boolean			_enumPrint(String& out, uint value)const		{ return false; }
	virtual boolean			_enumParse(uint& out, const ConstString& name)const	{ return false; }

	virtual const Attributes*	_attributes()const 				{ return 0; }

	virtual boolean _makeReflection(void* mem, void* ptr)const=0;
};


#define EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP \
virtual ReflectionLib::EMethaType		_methaType()const														{ return methaType(); } \
virtual Base::ConstString			_name()const 														{ return name(); } \
virtual ReflectionLib::TypeId			_id()const															{ return id(); } \
virtual Base::uint					_childsCount()const 												{ return childsCount(); } \
virtual Base::ConstString			_childName(Base::uint i)const										{ return childName(i); } \
virtual int							_childIndex(const Base::ConstString& name)const						{ return childIndex(name); } \
virtual Base::boolean				_child(void* mem, Base::uint n)const							{ return child(mem, n); } \
virtual Base::boolean				_child(void* mem, const Base::ConstString& name)const			{ return child(mem, name); } \
virtual ReflectionLib::EChildMode		_childMode(Base::uint n)const										{ return childMode(n); } \
virtual ReflectionLib::EChildMode		_childMode(const Base::ConstString& name)const						{ return childMode(name); } \
virtual const ReflectionLib::Attributes*	_childAttributes(Base::uint n)const	{ return childAttributes(n); } \
virtual const ReflectionLib::Attributes*	_childAttributes(const ConstString& name)const { return childAttributes(name); } \
virtual Base::uint					_basesCount()const 													{ return basesCount(); } \
virtual Base::ConstString			_baseName(Base::uint i)const										{ return baseName(i); } \
virtual int							_baseIndex(const Base::ConstString& name)const						{ return baseIndex(name); } \
virtual Base::boolean				_base(void* mem, Base::uint n)const							{ return base(mem, n); } \
virtual Base::boolean				_base(void* mem, const Base::ConstString& name)const			{ return base(mem, name); } \
virtual Base::boolean				_base(void* mem, ReflectionLib::TypeId id)const					{ return base(mem, id); } \
virtual const ReflectionLib::MethodSemantic*	_methodSemantic()const													{ return methodSemantic(); } \
virtual const ReflectionLib::Semantic*	_constructorSemantic()const											{ return constructorSemantic(); } \
virtual Base::boolean				_construct(void* mem, Base::IAllocator& alc, void** args)const			{ return construct(mem, alc, args); } \
virtual const ReflectionLib::Semantic*	_localConstructorSemantic(Base::uint n)const									{ return localConstructorSemantic(n); } \
virtual const ReflectionLib::Semantic*	_localConstructorSemantic(const Base::ConstString& name)const									{ return localConstructorSemantic(name); } \
virtual Base::uint					_enumsCount()const													{ return enumsCount(); } \
virtual Base::ConstString			_enumName(Base::uint i)const										{ return enumName(i); } \
virtual Base::uint					_enumValue(Base::uint i)const										{ return enumValue(i); } \
virtual Base::boolean				_enumPrint(Base::String& out, Base::uint value)const			{ return enumPrint(out, value); } \
virtual Base::boolean				_enumParse(Base::uint& out, const Base::ConstString& name)const		{ return enumParse(out, name); } \
virtual const ReflectionLib::Attributes*	_attributes()const 						{ return attributes(); } \
virtual Base::boolean				_makeReflection(void* mem, void* ptr)const 							{ return makeReflection(mem, ptr); }

class DummyType : public IType
{
public:
	static INLINE uint				childsCount() 								{ return 0; }
	static INLINE ConstString		childName(uint i)							{ return _CS(""); }
	static INLINE int				childIndex(const ConstString& name)			{ return -1; }
	static INLINE boolean			child(void* mem, uint i)					{ return false; }
	static INLINE boolean			child(void* mem, const ConstString& name)	{ return false; }
	static INLINE EChildMode		childMode(uint i)							{ return (EChildMode)0; }
	static INLINE EChildMode		childMode(const ConstString& name)			{ return (EChildMode)0; }
	static INLINE const Attributes*	childAttributes(Base::uint n)	{ return 0; }
	static INLINE const Attributes*	childAttributes(const ConstString& name) { return 0; }

	static INLINE uint				basesCount() 								{ return 0; }
	static INLINE ConstString		baseName(uint i)							{ return _CS(""); }
	static INLINE int				baseIndex(const ConstString& name)			{ return -1; }
	static INLINE boolean			base(void* mem, Base::uint n)			{ return false; }
	static INLINE boolean			base(void* mem, const Base::ConstString& name){ return false; }
	static INLINE boolean			base(void* mem, ReflectionLib::TypeId id)	{ return false; }

	static INLINE const MethodSemantic*	methodSemantic()									{ return 0; }

	static INLINE const Semantic*	constructorSemantic()						{ return 0; }
	static INLINE boolean			construct(void* mem, IAllocator& alc, void** args) { return false; }

	static INLINE const Semantic*	localConstructorSemantic(Base::uint n)					{ return 0; }
	static INLINE const Semantic*	localConstructorSemantic(const Base::ConstString& name)	{ return 0; }

	static INLINE uint				enumsCount()								{ return 0; }
	static INLINE ConstString		enumName(uint i)							{ return _CS(""); }
	static INLINE uint				enumValue(uint i)							{ return 0; }
	static INLINE boolean 			enumPrint(String& out, uint value)		{ return false; }
	static INLINE boolean 			enumParse(uint& out, const ConstString& name){ return false; }

	static INLINE const Attributes*	attributes() 								{ return 0; }

	/*
	template<class t_Refl>
	static INLINE boolean			valueSet(t_Refl& refl, ReflectionLib::TypeId id, const void* v) { return false; }

	template<class t_Refl>
	static INLINE boolean			valueGet(ReflectionLib::TypeId id, void* v, t_Refl& refl) { return false; }
	*/
};

class InvalidType : public DummyType
{
public:
	virtual boolean					_valid() 									{ return false; }

	static INLINE EMethaType				methaType()										{ return EMethaType_Null; }
	static INLINE ConstString		name() 										{ return _CS(""); }
	static INLINE TypeId			id() 										{ return TypeId(); }

	static INLINE boolean makeReflection(void* mem, void* ptr) { return false; }

	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP
};

class Type
{
public:
	static const uint gcMaxTypeImplimentationSize = sizeof(IType);
	void* data[1];

	INLINE Type(){ new(data) InvalidType(); }
	INLINE Type(const NoInitConstructoExtensionFlag& x)
	{
#ifdef DEBUG
		data[0] = 0;
#endif
	}
	INLINE Type(const Type& other){ data[0] = other.data[0]; }
	INLINE Type& operator = (const Type& other){ data[0] = other.data[0]; return *this; }

	template<class t_TType>
	static INLINE Type make()
	{
		Type t;
		new (t.data) t_TType();
		return t;
	}

	INLINE boolean		valid()const 									{ return impl()->_valid(); }
	INLINE EMethaType	methaType()const								{ return impl()->_methaType(); }
	INLINE ConstString	name()const 									{ return impl()->_name(); }
	INLINE TypeId		id()const										{ return impl()->_id(); }

	INLINE void* getClassDesc()const									{ return impl()->_getClassDesc(); }

	INLINE uint			childsCount()const 								{ return impl()->_childsCount(); }
	INLINE ConstString	childName(uint n)const							{ return impl()->_childName(n); }
	INLINE int			childIndex(const ConstString& name)const		{ return impl()->_childIndex(name); }
	INLINE Type			child(uint n)const								
	{
		Type t;
		if(impl()->_child(t.data, n))
			return t;
		new (t.data) InvalidType;
		return t;
	}
	INLINE Type			child(const ConstString& name)const				
	{
		Type t;
		if(impl()->_child(t.data, name))
			return t;
		new (t.data) InvalidType;
		return t;
	}
	INLINE EChildMode	childMode(uint i)const								{ return impl()->_childMode(i); }
	INLINE EChildMode	childMode(const ConstString& name)const				{ return impl()->_childMode(name); }
	INLINE const Attributes*		childAttributes(Base::uint n)const	{ return impl()->_childAttributes(n); }
	INLINE const Attributes*		childAttributes(const ConstString& name)const { return impl()->_childAttributes(name); }

	INLINE uint			basesCount()const 								{ return impl()->_basesCount(); }
	INLINE ConstString	baseName(uint i)const							{ return impl()->_baseName(i); }
	INLINE int			baseIndex(const ConstString& name)const			{ return impl()->_baseIndex(name); }
	INLINE Type			base(uint n)const
	{
		Type t;
		if(!impl()->_base(t.data, n))
			new (t.data) InvalidType;
		return t;
	}
	INLINE Type			base(const ConstString& name)const
	{
		Type t;
		if(!impl()->_base(t.data, name))
			new (t.data) InvalidType;
		return t;
	}

	INLINE const MethodSemantic*	methodSemantic()const									{ return impl()->_methodSemantic(); }
	
	INLINE const Semantic*	constructorSemantic()const						{ return impl()->_constructorSemantic(); }
	INLINE class Reflection	construct(IAllocator& alc, void** args)const;
	template<class t_Arg0>
	INLINE Reflection	construct(IAllocator& alc, t_Arg0& arg0)const;
	template<class t_Arg0, class t_Arg1>
	INLINE Reflection	construct(IAllocator& alc, t_Arg0& arg0, t_Arg0& arg1)const;
	
	INLINE const Semantic*	localConstructorSemantic(uint n)const					{ return impl()->_localConstructorSemantic(n); }
	INLINE const Semantic*	localConstructorSemantic(const ConstString& name)const					{ return impl()->_localConstructorSemantic(name); }

	//INLINE boolean constructorSemanticsIsSame()const;

	INLINE uint			enumsCount()const								{ return impl()->_enumsCount(); }
	INLINE ConstString	enumName(uint i)const							{ return impl()->_enumName(i); }
	INLINE uint			enumValue(uint i)const							{ return impl()->_enumValue(i); }
	INLINE boolean 		enumPrint(String& out, uint value)const	{ return impl()->_enumPrint(out, value); }
	INLINE boolean 		enumParse(uint& out, const ConstString& name)const	{ return impl()->_enumParse(out, name); }

	INLINE const Attributes*	attributes(Attributes& attr)const 				{ return impl()->_attributes(); }

	INLINE boolean operator == (const Type& b)const
	{
		return id() == b.id();
	}

	INLINE boolean operator != (const Type& b)const
	{
		return id() != b.id();
	}

	INLINE boolean makeReflection(void* mem, void* ptr)const { return impl()->_makeReflection(mem, ptr); }

	static Type getType(ConstString name);
	static Type getType(TypeId id);

	IType* impl()const { return (IType*)data; }
};

//extern Array<Type> getBaseTypes(const ConstString& name);
//extern Type getDerivedTypes(const ConstString& name);


}

namespace Base
{
	template<>
	class DefaultComparator< const ReflectionLib::Type, const ReflectionLib::Type >
	{
	public:
		INLINE int operator() (const ReflectionLib::Type& a, const ReflectionLib::Type& b)const
		{ 
			return int( a.id().number() - b.id().number() );
		}
	};
};

namespace ReflectionLib
{
	/*INLINE boolean Type::constructorSemanticsIsSame()const
	{
		const Semantic* a = constructorSemantic();
		const Semantic* b = localConstructorSemantic();
		if ( !a || !b )
			return false;
		return *a == *b;
	}*/
};
