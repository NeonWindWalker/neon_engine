#pragma once

#include "makers.h"

namespace ReflectionLib
{

template<class t_Type>
class ConstructableBase
{
public:
	typedef Delegate<void*(t_Type*&, const Type&, IAllocator&)> ConstructorDelegate_t;
	typedef Delegate<void(t_Type* p)> PreDestructorDelegate_t;

	static void* defaultConstructor(t_Type*& baseptr, const Type& type, IAllocator& alc)
	{
		Reflection rl = type.construct(alc, 0);
		void* derivedptr = rl.objectPtr();
		baseptr = (t_Type*)rl.cast<t_Type>().objectPtr();
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	static void defaultPreDestructor(t_Type* p) {}
};

template<class t_Type, class t_Arg0>
class ConstructableBase1
{
public:
	typedef Delegate<void*(t_Type*&, const Type&, IAllocator&, t_Arg0&)> ConstructorDelegate_t;
	typedef Delegate<void(t_Type* p)> PreDestructorDelegate_t;

	static void* defaultConstructor(t_Type*& baseptr, const Type& type, IAllocator& alc, t_Arg0& arg0)
	{
		void* apArgs[1];
        apArgs[0] = (void*)&arg0;
		Reflection rl = type.construct(alc, apArgs);
		void* derivedptr = rl.objectPtr();
		baseptr = (t_Type*)rl.cast<t_Type>().objectPtr();
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	static void defaultPreDestructor(t_Type* p) {}
};


template<class t_Type, class t_SmartPoiter, class t_AllocatorWrapper = SystemAllocatorWrapper>
class Constructable : public t_SmartPoiter, public t_AllocatorWrapper
{
public:
	static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){}

	typedef t_SmartPoiter SmartPoiter_t;
	typedef t_SmartPoiter Base_t;
	typedef typename ConstructableBase<t_Type>::ConstructorDelegate_t ConstructorDelegate_t;
	typedef typename ConstructableBase<t_Type>::PreDestructorDelegate_t PreDestructorDelegate_t;
	ConstructorDelegate_t constructor;
	PreDestructorDelegate_t predestructor;

	Constructable() : constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	Constructable(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), constructor(con), predestructor(des) {}

	Constructable(const ConstructorDelegate_t& con) : constructor(con), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	Constructable(typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	Constructable(const PreDestructorDelegate_t& des) : constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(des) {}

	Constructable(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des) : constructor(con), predestructor(des) {}
	Constructable(const ConstructorDelegate_t& con, typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), constructor(con), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	Constructable(const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(des) {}
	
	INLINE void* construct(t_Type*& baseptr, const Type& type)
	{
		void* derivedptr = constructor(baseptr, type, this->allocator());
		if(baseptr)
			*(Base_t*)this = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void* construct(t_Type*& baseptr)
	{
		void* derivedptr = constructor(baseptr, typeOf<t_Type>(), this->allocator());
		if(baseptr)
			*(Base_t*)this = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void destruct()
	{
		predestructor(this->get());
		*(Base_t*)this = (t_Type*)0;
	}

	INLINE void operator = (t_Type* p)
	{
		*static_cast<t_SmartPoiter*>(this) = p;
	}
};

template<class t_Type, class t_SmartPoiter, class t_Arg0, class t_AllocatorWrapper = SystemAllocatorWrapper>
class Constructable1 : public t_SmartPoiter, public t_AllocatorWrapper
{
public:
	static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){}

	typedef t_SmartPoiter SmartPoiter_t;
	typedef t_SmartPoiter Base_t;
	typedef t_Arg0 Arg0_t;
	typedef typename ConstructableBase1<t_Type, t_Arg0>::ConstructorDelegate_t ConstructorDelegate_t;
	typedef typename ConstructableBase1<t_Type, t_Arg0>::PreDestructorDelegate_t PreDestructorDelegate_t;
	ConstructorDelegate_t constructor;
	PreDestructorDelegate_t predestructor;

	Constructable1() : constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	Constructable1(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), constructor(con), predestructor(des) {}

	Constructable1(const ConstructorDelegate_t& con) : constructor(con), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	Constructable1(typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	Constructable1(const PreDestructorDelegate_t& des) : constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(des) {}

	Constructable1(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des) : constructor(con), predestructor(des) {}
	Constructable1(const ConstructorDelegate_t& con, typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), constructor(con), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	Constructable1(const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad), constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(des) {}

	INLINE void* construct(t_Type*& baseptr, const Type& type, t_Arg0& arg0)
	{
		void* derivedptr = constructor(baseptr, type, this->allocator(), arg0);
		if(baseptr)
			*(Base_t*)this = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void* construct(t_Type*& baseptr, t_Arg0& arg0)
	{
		void* derivedptr = constructor(baseptr, typeOf<t_Type>(), this->allocator(), arg0);
		if(baseptr)
			*(Base_t*)this = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void destruct()
	{
		predestructor(this->get());
		*(Base_t*)this = (t_Type*)0;
	}
};


template<class t_Type, class t_SmartPoiter, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_nReserved = Base::DefaultArrayReservedElementsCount<t_SmartPoiter>::gcReservedElementsCount >
class ArrayOfConstructables : public Array< t_SmartPoiter, t_AllocatorWrapper, t_nReserved >
{
public:
	static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){}

	typedef t_SmartPoiter SmartPoiter_t;
	typedef Array< t_SmartPoiter, t_AllocatorWrapper, t_nReserved > Base_t;
	typedef typename ConstructableBase<t_Type>::ConstructorDelegate_t ConstructorDelegate_t;
	typedef typename ConstructableBase<t_Type>::PreDestructorDelegate_t PreDestructorDelegate_t;
	ConstructorDelegate_t constructor;
	PreDestructorDelegate_t predestructor;

	ArrayOfConstructables() : constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	ArrayOfConstructables(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(con), predestructor(des) {}

	ArrayOfConstructables(const ConstructorDelegate_t& con) : constructor(con), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	ArrayOfConstructables(typename t_AllocatorWrapper::Data_t& ad) :Base_t(ad),  constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	ArrayOfConstructables(const PreDestructorDelegate_t& des) : constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(des) {}

	ArrayOfConstructables(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des) : constructor(con), predestructor(des) {}
	ArrayOfConstructables(const ConstructorDelegate_t& con, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(con), predestructor(this, &ConstructableBase<t_Type>::defaultPreDestructor) {}
	ArrayOfConstructables(const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(des) {}

	INLINE void* construct(t_Type*& baseptr, uint n, const Type& type)
	{
		Assert(n < this->size());
		void* derivedptr = constructor(baseptr, type, this->allocator());
		if(baseptr)
			this->at(n) = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
		
	}

	INLINE void* construct(t_Type*& baseptr, uint n)
	{
		Assert(n < this->size());
		void* derivedptr = constructor(baseptr, typeOf<t_Type>(), this->allocator());
		if(baseptr)
			this->at(n) = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void destruct(uint n)
	{
		Assert(n < this->size());
		predestructor(this->at(n).get());
		this->at(n) = (t_Type*)0;
	}

	INLINE void* insertAndConstruct(t_Type*& baseptr, uint n, const Type& type)
	{
		Assert(n <= this->size());
		Base_t::insertDefault(n);
		return construct(baseptr, n, type);
	}

	INLINE t_Type* insertAndConstruct(uint n)
	{
		Assert(n <= this->size());
		Base_t::insertDefault(n);
		t_Type* baseptr = 0;
		construct(baseptr, n);
		return baseptr;
	}

	INLINE void* add(t_Type*& baseptr, const Type& type)
	{
		return insertAndConstruct(baseptr, this->size(), type);
	}

	INLINE t_Type* add()
	{
		return insertAndConstruct(this->size());
	}

	template<class t_DerivedType>
	INLINE t_DerivedType* addEx()
	{
		t_Type* baseptr;
		return (t_DerivedType*)insertAndConstruct(baseptr, this->size(), typeOf<t_DerivedType>());
	}

	class Comparator
	{
		public:
		INLINE boolean operator () (const t_SmartPoiter& sp, const t_Type* p) 
		{
			return DefaultComparator<const t_Type*,const t_Type*>(sp.get(), p);
		}
	};

	
	template<class tt_Comparator>
	INLINE typename Base_t::Iterator find(const typename Base_t::Value_t& v, tt_Comparator cmp) { return Base_t::find(v, cmp); }
	INLINE typename Base_t::Iterator find(const typename Base_t::Value_t& v) { return Base_t::find(v); }
	INLINE typename Base_t::Iterator find(const t_Type* obj){ return Base_t::find(obj, Comparator()); }

	template<class tt_Comparator>
	INLINE typename Base_t::Iterator rfind(const typename Base_t::Value_t& v, tt_Comparator cmp) { return Base_t::rfind(v, cmp); }
	INLINE typename Base_t::Iterator rfind(const typename Base_t::Value_t& v) { return Base_t::rfind(v); }
	INLINE typename Base_t::Iterator rfind(const t_Type* obj){ return Base_t::rfind(obj, Comparator()); }

	template<class tt_Comparator>
	INLINE typename Base_t::ConstIterator find(const typename Base_t::Value_t& v, tt_Comparator cmp)const { return Base_t::find(v, cmp); }
	INLINE typename Base_t::ConstIterator find(const typename Base_t::Value_t& v)const { return Base_t::find(v); }
	INLINE typename Base_t::ConstIterator find(const t_Type* obj)const { return Base_t::find(obj, Comparator()); }

	template<class tt_Comparator>
	INLINE typename Base_t::ConstIterator rfind(const typename Base_t::Value_t& v, tt_Comparator cmp)const { return Base_t::rfind(v, cmp); }
	INLINE typename Base_t::ConstIterator rfind(const typename Base_t::Value_t& v)const { return Base_t::rfind(v); }
	INLINE typename Base_t::ConstIterator rfind(const t_Type* obj)const { return Base_t::rfind(obj, Comparator()); }


	INLINE void erase(uint position) { Base_t::erase(position);}
	INLINE void erase(typename Base_t::Iterator it) { Base_t::erase(it); }

	INLINE boolean erase(t_Type* obj)
	{
		typename Base_t::Iterator it = Base_t::find(obj);
		if(it != Base_t::end()){
			Base_t::erase(it);
			return true;
		}
		return false;
	}

	INLINE boolean erase(t_SmartPoiter& obj)
	{
		return erase(obj.get());
	}
};

template<class t_Type, class t_SmartPoiter, class t_Arg0, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_nReserved = Base::DefaultArrayReservedElementsCount<t_SmartPoiter>::gcReservedElementsCount >
class ArrayOfConstructables1 : public Array< t_SmartPoiter, t_AllocatorWrapper, t_nReserved >
{
public:
	static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){}

	typedef t_Type Type_t;
	typedef t_SmartPoiter SmartPoiter_t;
	typedef t_Arg0 Arg0_t;
	typedef Array< t_SmartPoiter, t_AllocatorWrapper, t_nReserved > Base_t;
	typedef typename ConstructableBase1<t_Type, t_Arg0>::ConstructorDelegate_t ConstructorDelegate_t;
	typedef typename ConstructableBase1<t_Type, t_Arg0>::PreDestructorDelegate_t PreDestructorDelegate_t;
	ConstructorDelegate_t constructor;
	PreDestructorDelegate_t predestructor;

	ArrayOfConstructables1() : constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	ArrayOfConstructables1(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(con), predestructor(des) {}

	ArrayOfConstructables1(const ConstructorDelegate_t& con) : constructor(con), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	ArrayOfConstructables1(typename t_AllocatorWrapper::Data_t& ad) :Base_t(ad),  constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	ArrayOfConstructables1(const PreDestructorDelegate_t& des) : constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(des) {}

	ArrayOfConstructables1(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des) : constructor(con), predestructor(des) {}
	ArrayOfConstructables1(const ConstructorDelegate_t& con, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(con), predestructor(this, &ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	ArrayOfConstructables1(const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(des) {}

	INLINE void* construct(t_Type*& baseptr, uint n, const Type& type, t_Arg0& arg0)
	{
		Assert(n < this->size());
		void* derivedptr = constructor(baseptr, type, this->allocator(), arg0);
		if(baseptr)
			this->at(n) = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void* construct(t_Type*& baseptr, uint n, t_Arg0& arg0)
	{
		Assert(n < this->size());
		void* derivedptr = constructor(baseptr, typeOf<t_Type>(), this->allocator(), arg0);
		if(baseptr)
			this->at(n) = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void destruct(uint n)
	{
		Assert(n < this->size());
		predestructor(this->at(n).get());
		this->at(n) = (t_Type*)0;
	}
};


template<class t_Type, class t_SmartPoiter, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_nReserved = 4 >
class HashArrayOfConstructables : public HashArray< Pair<String, t_SmartPoiter>, t_AllocatorWrapper, t_nReserved * 4, 4 >
{
public:
	static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){}

	typedef t_SmartPoiter SmartPoiter_t;
	typedef HashArray< Pair<String, t_SmartPoiter>, t_AllocatorWrapper, t_nReserved * 4, 4 > Base_t;
	typedef typename ConstructableBase<t_Type>::ConstructorDelegate_t ConstructorDelegate_t;
	typedef typename ConstructableBase<t_Type>::PreDestructorDelegate_t PreDestructorDelegate_t;
	ConstructorDelegate_t constructor;
	PreDestructorDelegate_t predestructor;

	HashArrayOfConstructables() : constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	HashArrayOfConstructables(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(con), predestructor(des) {}

	HashArrayOfConstructables(const ConstructorDelegate_t& con) : constructor(con), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	HashArrayOfConstructables(typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad),  constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	HashArrayOfConstructables(const PreDestructorDelegate_t& des) : constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(des) {}

	HashArrayOfConstructables(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des) : constructor(con), predestructor(des) {}
	HashArrayOfConstructables(const ConstructorDelegate_t& con, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(con), predestructor(&ConstructableBase<t_Type>::defaultPreDestructor) {}
	HashArrayOfConstructables(const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(&ConstructableBase<t_Type>::defaultConstructor), predestructor(des) {}


	INLINE void* construct(t_Type*& baseptr, uint n, const Type& type)
	{
		Assert(n < this->size());
		void* derivedptr = constructor(baseptr, type, this->allocator());
		if(baseptr)
			this->at(n).val = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void* construct(t_Type*& baseptr, uint n)
	{
		Assert(n < this->size());
		void* derivedptr = constructor(baseptr, typeOf<t_Type>(), this->allocator());
		if(baseptr)
			this->at(n).val = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void* construct(t_Type*& baseptr, ConstString name, const Type& type)
	{
		typename Base_t::Iterator it = this->find(name);
		if(it != this->end()){
			void* derivedptr = constructor(baseptr, type, this->allocator());
			if(baseptr)
				it->val = baseptr;
			Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
			return derivedptr;
		}
		return 0;
	}

	INLINE void* construct(t_Type*& baseptr, ConstString name)
	{
		typename Base_t::Iterator it = this->find(name);
		if(it != this->end()){
			void* derivedptr = constructor(baseptr, typeOf<t_Type>(), this->allocator());
			if(baseptr)
				it->val = baseptr;
			Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
			return derivedptr;
		}
		return 0;
	}

	INLINE void destruct(uint n)
	{
		predestructor(this->at(n).val.get());
		this->at(n).val = (t_Type*)0;
	}

	INLINE boolean destruct(ConstString name)
	{
		typename Base_t::Iterator it = this->find(name);
		if(it != this->end()){
			predestructor(it->val.get());
			it->val = (t_Type*)0;
			return true;
		}
		return false;
	}

	INLINE void* insertAndConstruct(t_Type*& baseptr, const Type& type, ConstString name)
	{
		boolean bIns;
		typename Base_t::Iterator it = this->insert(bIns, name);
		if(!bIns){
			baseptr = 0;
			return 0;
		}
		void* derivedptr = this->constructor(baseptr, type, this->allocator());
		if(baseptr)
			it->val = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}
};


template<class t_Type, class t_SmartPoiter, class t_Arg0, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_nReserved = 4 >
class HashArrayOfConstructables1 : public HashArray< Pair<String, t_SmartPoiter>, t_AllocatorWrapper, t_nReserved * 4, 4 >
{
public:
	static inline void zrl_ThisClassIsReflected_CompileTimeGuardFlag(){}

	typedef t_Type Type_t;
	typedef t_SmartPoiter SmartPoiter_t;
	typedef t_Arg0 Arg0_t;
	typedef HashArray< Pair<String, t_SmartPoiter>, t_AllocatorWrapper, t_nReserved * 4, 4 > Base_t;
	typedef typename ConstructableBase1<t_Type, t_Arg0>::ConstructorDelegate_t ConstructorDelegate_t;
	typedef typename ConstructableBase1<t_Type, t_Arg0>::PreDestructorDelegate_t PreDestructorDelegate_t;
	ConstructorDelegate_t constructor;
	PreDestructorDelegate_t predestructor;

	HashArrayOfConstructables1() : constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	HashArrayOfConstructables1(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(con), predestructor(des) {}

	HashArrayOfConstructables1(const ConstructorDelegate_t& con) : constructor(con), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	HashArrayOfConstructables1(typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad),  constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	HashArrayOfConstructables1(const PreDestructorDelegate_t& des) : constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(des) {}

	HashArrayOfConstructables1(const ConstructorDelegate_t& con, const PreDestructorDelegate_t& des) : constructor(con), predestructor(des) {}
	HashArrayOfConstructables1(const ConstructorDelegate_t& con, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(con), predestructor(&ConstructableBase1<t_Type, t_Arg0>::defaultPreDestructor) {}
	HashArrayOfConstructables1(const PreDestructorDelegate_t& des, typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad), constructor(&ConstructableBase1<t_Type, t_Arg0>::defaultConstructor), predestructor(des) {}


	INLINE void* construct(t_Type*& baseptr, uint n, const Type& type, t_Arg0& arg0)
	{
		Assert(n < this->size());
		void* derivedptr = constructor(baseptr, type, this->allocator(), arg0);
		if(baseptr)
			this->at(n).val = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void* construct(t_Type*& baseptr, uint n, t_Arg0& arg0)
	{
		Assert(n < this->size());
		void* derivedptr = constructor(baseptr, typeOf<t_Type>, this->allocator(), arg0);
		if(baseptr)
			this->at(n).val = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

	INLINE void* construct(t_Type*& baseptr, ConstString name, const Type& type, t_Arg0& arg0)
	{
		typename Base_t::Iterator it = this->find(name);
		if(it != this->end()){
			void* derivedptr = constructor(baseptr, type, this->allocator(), arg0);
			if(baseptr)
				it->val = baseptr;
			Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
			return derivedptr;
		}
		return 0;
	}

	INLINE void* construct(t_Type*& baseptr, ConstString name, t_Arg0& arg0)
	{
		typename Base_t::Iterator it = this->find(name);
		if(it != this->end()){
			void* derivedptr = constructor(baseptr, typeOf<t_Type>(), this->allocator(), arg0);
			if(baseptr)
				it->val = baseptr;
			Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
			return derivedptr;
		}
		return 0;
	}

	INLINE void destruct(uint n)
	{
		predestructor(this->at(n).val.get());
		this->at(n).val = (t_Type*)0;
	}

	INLINE boolean destruct(ConstString name)
	{
		typename Base_t::Iterator it = this->find(name);
		if(it != this->end()){
			predestructor(it->val.get());
			it->val = (t_Type*)0;
			return true;
		}
		return false;
	}

	INLINE void* insertAndConstruct(t_Type*& baseptr, const Type& type, ConstString name, t_Arg0& arg0)
	{
		boolean bIns;
		typename Base_t::Iterator it = this->insert(bIns, name);
		if(!bIns){
			baseptr = 0;
			return 0;
		}
		void* derivedptr = this->constructor(baseptr, type, this->allocator(), arg0);
		if(baseptr)
			it->val = baseptr;
		Assert((derivedptr && baseptr) || (!derivedptr && !baseptr));
		return derivedptr;
	}

};

}
