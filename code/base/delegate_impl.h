template<class t_Ret DELEGATE_CODE_GEN_DECLARATION_PLUS>
class IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>
{
public:
	virtual t_Ret call(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const=0;
	virtual void copyconstruct(void* mem)const=0;
	virtual void destruct()=0;
	virtual Base::boolean equal(const IDelegate& sameTypeDelegate)const=0;
	virtual Base::boolean valid()const=0;
};



template<class t_Class, class t_Ret DELEGATE_CODE_GEN_DECLARATION_PLUS>
class MethodDelegate<t_Ret (t_Class::*)(DELEGATE_CODE_GEN_METHOD_TYPES)> : IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)> 
{
public:
	typedef t_Ret (t_Class::*MethodPtr_t)(DELEGATE_CODE_GEN_METHOD_TYPES);

	t_Class* ptr;
	MethodPtr_t method;

	MethodDelegate(t_Class* p, MethodPtr_t m) : ptr(p), method(m)
	{
		static_assert(gcDelegateSize >= sizeof(MethodDelegate), "delegate container small size");
	}

	t_Ret call(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{
		return (ptr->*method)(DELEGATE_CODE_GEN_METHOD_VARIABLES);
	}

	void copyconstruct(void* mem)const
	{
		new(mem) MethodDelegate(*this);
	}

	void destruct()
	{
	}

	Base::boolean equal(const IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>& sameTypeDelegate)const
	{
		const MethodDelegate* other = static_cast<const MethodDelegate*>(&sameTypeDelegate);
		return this->ptr == other->ptr && this->method == other->method;
	}

	Base::boolean valid()const
	{
		return ptr != 0;
	}
};

template<class t_Ret DELEGATE_CODE_GEN_DECLARATION_PLUS>
class FunctionDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)> : IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)> 
{
public:
	typedef t_Ret (*FunctionPtr_t)(DELEGATE_CODE_GEN_METHOD_TYPES);

	FunctionPtr_t func;

	FunctionDelegate(FunctionPtr_t fun) : func(fun)
	{
		static_assert(gcDelegateSize >= sizeof(FunctionDelegate), "function delegate container small size");
	}

	t_Ret call(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{
		return (*func)(DELEGATE_CODE_GEN_METHOD_VARIABLES);
	}

	void copyconstruct(void* mem)const
	{
		new(mem) FunctionDelegate(*this);
	}

	void destruct()
	{
	}

	Base::boolean equal(const IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>& sameTypeDelegate)const
	{
		const FunctionDelegate* other = static_cast<const FunctionDelegate*>(&sameTypeDelegate);
		return this->func == other->func;
	}

	Base::boolean valid()const
	{
		return true;
	}
};

template<class t_Ret DELEGATE_CODE_GEN_DECLARATION_PLUS>
class DummyDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)> : IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)> 
{
public:
	t_Ret call(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{
		return t_Ret();
	}

	void copyconstruct(void* mem)const
	{
		new(mem) DummyDelegate(*this);
	}

	void destruct()
	{
	}

	Base::boolean equal(const IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>& sameTypeDelegate)const
	{
		return true;
	}

	Base::boolean valid()const
	{
		return false;
	}
};

template<class t_Ret DELEGATE_CODE_GEN_DECLARATION_PLUS>
class Delegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>
{
public:
	static_assert(gcDelegateSize % sizeof(void*) == 0, "delegate size broken");
	void* mem[gcDelegateSize / sizeof(void*)];
	
	IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>* impl()const { return (IDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>*)mem; }

	INLINE Delegate()
	{
		setEmpty();
	}

	INLINE Delegate(const Delegate& other)
	{
		other.impl()->copyconstruct(this->mem);
	}

	INLINE Delegate& operator = (const Delegate& other)
	{ 
		this->impl()->destruct();
		other.impl()->copyconstruct(this->mem);
		return *this;
	}

	INLINE t_Ret operator() (DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{ 
		return this->impl()->call(DELEGATE_CODE_GEN_METHOD_VARIABLES);
	}

	INLINE Base::boolean operator == (const Delegate& other)const
	{
		return other.mem[0] == mem[0] && this->impl()->equal(*other.impl());
	}

	INLINE Base::boolean operator != (const Delegate& other)const
	{ 
		return !(*this == other);
	}

	INLINE operator Base::boolean () const
	{ 
		return this->impl()->valid();
	}

	INLINE void setEmpty()
	{ 
		for(uint i=0; i < gcDelegateSize / sizeof(void*); ++i)
			mem[i] = 0;
		new(mem) DummyDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>();
	}

	template<class tt_Class>
	Delegate(tt_Class* o, t_Ret (tt_Class::*m)(DELEGATE_CODE_GEN_METHOD_TYPES) )
	{
		typedef t_Ret (tt_Class::*MethodPtr_tt)(DELEGATE_CODE_GEN_METHOD_TYPES);
		new(mem) MethodDelegate<MethodPtr_tt>(o, m);
	}

	Delegate(t_Ret (*fun)(DELEGATE_CODE_GEN_METHOD_TYPES) )
	{
		new(mem) FunctionDelegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>(fun);
	}
};

template<class t_Ret DELEGATE_CODE_GEN_DECLARATION_PLUS>
class Event<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>
{	
public:
	typedef typename Base::Delegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)> Delegate_t;

	Base::Array< Delegate_t > delegates;
	
	void operator() (DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{ 
		for(const Delegate_t* i = this->delegates.first(); i != this->delegates.end(); ++i)
			(*i)(DELEGATE_CODE_GEN_METHOD_VARIABLES);
	}

	boolean callAnd(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{ 
		for(const Delegate_t* i = this->delegates.first(); i != this->delegates.end(); ++i)
			if(!(*i)(DELEGATE_CODE_GEN_METHOD_VARIABLES))
				return false;
		return true;
	}

	boolean callOr(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{ 
		for(const Delegate_t* i = this->delegates.first(); i != this->delegates.end(); ++i)
			if((*i)(DELEGATE_CODE_GEN_METHOD_VARIABLES))
				return true;
		return false;
	}

	boolean callAndAll(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{
		boolean ret = false;
		for(const Delegate_t* i = this->delegates.first(); i != this->delegates.end(); ++i)
			ret &= boolean01FromBoolean((*i)(DELEGATE_CODE_GEN_METHOD_VARIABLES));
		return ret;
	}

	boolean callOrAll(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{
		boolean ret = false;
		for(const Delegate_t* i = this->delegates.first(); i != this->delegates.end(); ++i)
			ret |= (*i)(DELEGATE_CODE_GEN_METHOD_VARIABLES);
		return ret;
	}

	t_Ret callReturnFirstNotNull(DELEGATE_CODE_GEN_METHOD_TYPES_AND_VARIABLES)const
	{ 
		for(const Delegate_t* i = this->delegates.first(); i != this->delegates.end(); ++i){
			t_Ret r = (*i)(DELEGATE_CODE_GEN_METHOD_VARIABLES);
			if(r)
				return r;
		}
		return 0;
	}

	void operator += (const Delegate_t& d)
	{
		this->delegates.push(d);
	}

	void operator -= (const Delegate_t& d)
	{
		Delegate_t* it = this->delegates.find(d);
		if(it == this->delegates.end())
			return;
		this->delegates.erase(it);
	}

	void clear()
	{
		this->delegates.clear();
	}
};

template<class t_Ret, class t_Class DELEGATE_CODE_GEN_DECLARATION_PLUS>
Delegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)> makeDelegate(t_Class* obj, t_Ret(t_Class::*method)(DELEGATE_CODE_GEN_METHOD_TYPES))
{
	return Delegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>(obj, method);
}

template<class t_Ret DELEGATE_CODE_GEN_DECLARATION_PLUS>
Delegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)> makeDelegate(t_Ret (*func)(DELEGATE_CODE_GEN_METHOD_TYPES))
{
	return Delegate<t_Ret(DELEGATE_CODE_GEN_METHOD_TYPES)>(func);
}
