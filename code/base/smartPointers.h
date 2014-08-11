#pragma once

#include "alghoritms.h"
#include "threading.h"
#include "list.h"

namespace Base
{

class WeakPtrBase
{
protected:
	ListElement<WeakPtrBase> lel;

public:
	virtual void onObjectDelete()=0;
	WeakPtrBase() : lel(this) {}
};

template<class t_Class> class StrongPtr;
template<class t_Class> class WeakPtr;
template<class t_Class> class StrongPtrThreadSafe;
template<class t_Class> class WeakPtrThreadSafe;

class RefCountable
{
private:
	SpinLock lock;
	int count;
	RefCountable* root;
	ListRoot<WeakPtrBase> weaks;

	RefCountable(const RefCountable& other){}
	void operator = (const RefCountable& other){}

	INLINE void onDeleteNotifyWeaks()
	{
		for(ListRoot<WeakPtrBase>::Iterator it = weaks.first(); it != weaks.end(); )
		{
			WeakPtrBase* p = &(*it);
			++it;
			p->onObjectDelete();
		}
	}

	template<class> friend class StrongPtrBase;
	template<class> friend class StrongPtr;
	template<class> friend class WeakPtr;
	template<class> friend class StrongPtrThreadSafe;
	template<class> friend class WeakPtrThreadSafe;

public:
	INLINE RefCountable()
	{
		count = 0;
		root = this;
	}

	INLINE RefCountable(RefCountable* father)
	{
		count = 0;
		root = father;
	}

	virtual ~RefCountable()
	{
	}

	virtual void selfDelete()
	{
		delete this;
	}
};


template<class t_Class>
class StrongPtrBase
{
protected:
	t_Class* ptr;

public:
	typedef t_Class Class_t;

	StrongPtrBase() : ptr(0) {}

	INLINE t_Class* operator -> ()const { return this->ptr; }
	INLINE t_Class& operator * ()const { return *this->ptr; }
	INLINE t_Class* get()const { return this->ptr; }
	INLINE operator boolean ()const { return this->ptr != 0; }
	INLINE boolean operator ! ()const { return this->ptr == 0; }
	INLINE operator t_Class* ()const { return this->ptr; }

	/*
	INLINE friend boolean operator == (const StrongPtrBase& a, const t_Class* b) { return a.ptr == b; }
	INLINE friend boolean operator != (const StrongPtrBase& a, const t_Class* b) { return a.ptr != b; }
	INLINE friend boolean operator == (const t_Class* b, const StrongPtrBase& a) { return a.ptr == b; }
	INLINE friend boolean operator != (const t_Class* b, const StrongPtrBase& a) { return a.ptr != b; }
	INLINE friend boolean operator == (const StrongPtrBase& a, const StrongPtrBase& b) { return a.ptr == b.ptr; }
	INLINE friend boolean operator != (const StrongPtrBase& a, const StrongPtrBase& b) { return a.ptr != b.ptr; }
	*/

	INLINE uint getRefCount() { return ptr ? ptr->count : 0; }
};



template<class t_Class>
class StrongPtr : public StrongPtrBase<t_Class>
{
public:
	typedef t_Class Class_t;

	INLINE ~StrongPtr()
	{
		set(0);
	}

	INLINE StrongPtr()
	{
	}

	INLINE StrongPtr(const StrongPtr& other)
	{
		set(other.get());
	}

	INLINE StrongPtr& operator = (const StrongPtr& other)
	{
		set(other.get());
		return *this;
	}

	INLINE StrongPtr(t_Class* p)
	{
		set(p);
	}

	INLINE StrongPtr& operator = (t_Class* other)
	{
		set(other);
		return *this;
	}

	template<class t_OtherClass>
	INLINE StrongPtr(const StrongPtrBase<t_OtherClass>& other)
	{
		set(static_cast<t_Class*>(other.get()));
	}

	template<class t_OtherClass>
	INLINE StrongPtr& operator = (const StrongPtrBase<t_OtherClass>& other)
	{
		set(static_cast<t_Class*>(other.get()));
		return *this;
	}

	INLINE void set(t_Class* other)
	{
		RefCountable* old = static_cast<RefCountable*>(this->ptr);

		this->ptr = other;
		if(this->ptr)
			++ static_cast<RefCountable*>(this->ptr)->count;

		if(old && -- old->count == 0)
		{
			old->onDeleteNotifyWeaks();
			old->selfDelete();
		}
	}

	INLINE void move(StrongPtr& oldObj) 
	{
		set(nullptr);
		this->ptr = oldObj.ptr;
		oldObj.ptr = nullptr;
	}

	INLINE void swap(StrongPtr& b) 
	{
		Base::swap(this->ptr, b.ptr);
	}
};

template<class t_Class>
static INLINE void move(StrongPtr<t_Class>& newObj, StrongPtr<t_Class>& oldObj) 
{
	newObj.move(oldObj);
}

template<class t_Class>
static INLINE void swap(StrongPtr<t_Class>& a, StrongPtr<t_Class>& b) 
{
	a.swap(b);
}

template<class t_Class>
static INLINE void construct_destroy(StrongPtr<t_Class>* newObj, StrongPtr<t_Class>& oldObj) 
{
	new(newObj) StrongPtr<t_Class>();
	move(*newObj, oldObj);
	oldObj.~StrongPtr<t_Class>();
}



template<class t_Class>
class StrongPtrThreadSafe : public StrongPtrBase<t_Class>
{
public:
	typedef t_Class Class_t;

	INLINE ~StrongPtrThreadSafe()
	{
		set(0);
	}

	INLINE StrongPtrThreadSafe()
	{
	}

	INLINE StrongPtrThreadSafe(const StrongPtrThreadSafe& other)
	{
		set(other.get());
	}

	INLINE StrongPtrThreadSafe& operator = (const StrongPtrThreadSafe& other)
	{
		set(other.get());
		return *this;
	}

	INLINE StrongPtrThreadSafe(t_Class* p)
	{
		set(p);
	}

	INLINE StrongPtrThreadSafe& operator = (t_Class* other)
	{
		set(other);
		return *this;
	}

	template<class t_OtherClass>
	INLINE StrongPtrThreadSafe(const StrongPtrBase<t_OtherClass>& other)
	{
		set(static_cast<t_Class*>(other.get()));
	}

	template<class t_OtherClass>
	INLINE StrongPtrThreadSafe& operator = (const StrongPtrBase<t_OtherClass>& other)
	{
		set(static_cast<t_Class*>(other.get()));
		return *this;
	}

	INLINE void set(t_Class* other)
	{
		RefCountable* old = static_cast<RefCountable*>(this->ptr);

		this->ptr = other;
		if(this->ptr)
			atomicIncrement(static_cast<RefCountable*>(this->ptr)->count);

		if(old && atomicDecrement(old->count) == 0)
		{
			old->lock.lock();
			old->onDeleteNotifyWeaks();
			old->lock.unlock();
			old->selfDelete();
		}
	}

	INLINE void move(StrongPtrThreadSafe& oldObj) 
	{
		set(nullptr);
		this->ptr = oldObj.ptr;
		oldObj.ptr = nullptr;
	}

	INLINE void swap(StrongPtrThreadSafe& b) 
	{
		Base::swap(this->ptr, b.ptr);
	}
};

template<class t_Class>
static INLINE void move(StrongPtrThreadSafe<t_Class>& newObj, StrongPtrThreadSafe<t_Class>& oldObj) 
{
	newObj.move(oldObj);
}

template<class t_Class>
static INLINE void swap(StrongPtrThreadSafe<t_Class>& a, StrongPtrThreadSafe<t_Class>& b) 
{
	a.swap(b);
}

template<class t_Class>
static INLINE void construct_destroy(StrongPtrThreadSafe<t_Class>* newObj, StrongPtrThreadSafe<t_Class>& oldObj) 
{
	new(newObj) StrongPtrThreadSafe<t_Class>();
	move(*newObj, oldObj);
	oldObj.~StrongPtrThreadSafe<t_Class>();
}


template<class t_Class>
class WeakPtr : public WeakPtrBase
{
private:
	t_Class* ptr;

public:
	typedef t_Class Class_t;

	template<class t_OtherClass>
	INLINE boolean lock(StrongPtr<t_OtherClass>& sh)
	{
		if(this->ptr){
			sh.set(ptr);
			return true;
		}
		return false;
	}

	template<class t_OtherClass>
	INLINE boolean lock(StrongPtrThreadSafe<t_OtherClass>& sh)
	{
		if(this->ptr){
			sh.set(ptr);
			return true;
		}
		return false;
	}

	INLINE StrongPtr<t_Class> lock()
	{
		StrongPtr<t_Class> sh;
		lock(sh);
		return sh;
	}

	INLINE WeakPtr() : ptr(0)
	{
	}

	INLINE WeakPtr(t_Class* p) : ptr(0)
	{
		set(p);
	}

	INLINE ~WeakPtr()
	{
		set(0);
	}

	INLINE WeakPtr(const WeakPtr& other)
	{
		set(other.get());
	}

	INLINE WeakPtr& operator = (const WeakPtr& other)
	{
		set(other.get());
		return *this;
	}

	template<class t_OtherClass>
	INLINE WeakPtr(const StrongPtrBase<t_OtherClass>& other) : ptr(0)
	{
		set(static_cast<t_Class*>(other.get()));
	}

	template<class t_OtherClass>
	INLINE WeakPtr& operator = (const StrongPtrBase<t_OtherClass>& other)
	{
		set(static_cast<t_Class*>(other.get()));
		return *this;
	}

	INLINE t_Class* get()const
	{
		return this->ptr;
	}

	INLINE void set(t_Class* other)
	{
		this->lel.erase();
		this->ptr = other;
		if(this->ptr)
			static_cast<RefCountable*>(this->ptr)->weaks.insert(this->lel);
	}

	void onObjectDelete()
	{
		this->ptr = 0;
		this->lel.erase();
	}

	INLINE friend boolean operator == (const WeakPtr& a, const t_Class* b) { return a.ptr == b; }
	INLINE friend boolean operator != (const WeakPtr& a, const t_Class* b) { return a.ptr != b; }
	INLINE friend boolean operator == (const t_Class* b, const WeakPtr& a) { return a.ptr == b; }
	INLINE friend boolean operator != (const t_Class* b, const WeakPtr& a) { return a.ptr != b; }
};




template<class t_Class>
class WeakPtrThreadSafe : public WeakPtrBase
{
private:
	volatile t_Class* ptr;
	SpinLock spinLock;

public:
	typedef t_Class Class_t;

	template<class t_OtherClass>
	INLINE boolean lock(StrongPtr<t_OtherClass>& sh)
	{
		if(this->ptr){
			sh.set(ptr);
			return true;
		}
		return false;
	}

	template<class t_OtherClass>
	INLINE boolean lock(StrongPtrThreadSafe<t_OtherClass>& sh)
	{
		while(true)
		{
			SpinLockGuard myGuard(this->spinLock);

			RefCountable* p = static_cast<RefCountable*>(this->ptr);
			if(p)
			{
				if(p->lock.tryLock())
				{
					SpinLockGuard lg(p->lock, GuardLockableIsLocked());
					if(p->count){
						sh.set(this->ptr);
						return true;
					}
					return false;
				}
			}
			else
				return false;
		}
	}

	INLINE StrongPtrThreadSafe<t_Class> lock()
	{
		StrongPtrThreadSafe<t_Class> sh;
		lock(sh);
		return sh;
	}

	INLINE WeakPtrThreadSafe() : ptr(0)
	{
	}

	INLINE WeakPtrThreadSafe(t_Class* p) : ptr(0)
	{
		set(p);
	}

	template<class t_OtherClass>
	INLINE WeakPtrThreadSafe(const StrongPtrBase<t_OtherClass>& other) : ptr(0)
	{
		set(static_cast<t_Class*>(other.get()));
	}

	template<class t_OtherClass>
	INLINE WeakPtrThreadSafe& operator = (const StrongPtrBase<t_OtherClass>& other)
	{
		set(static_cast<t_Class*>(other.get()));
		return *this;
	}

	INLINE ~WeakPtrThreadSafe()
	{
		set(0);
	}

	INLINE WeakPtrThreadSafe(const WeakPtrThreadSafe& other)
	{
		set(other.get());
	}

	INLINE WeakPtrThreadSafe& operator = (const WeakPtrThreadSafe& other)
	{
		set(other.get());
		return *this;
	}

	INLINE void set(t_Class* other)
	{
		while(true)
		{
			SpinLockGuard myGuard(this->spinLock);

			RefCountable* p = static_cast<RefCountable*>(other);
			if(!p)
				return;
		
			if(p->lock.tryLock())
			{
				this->lel.erase();
				if(p->count){
					p->weaks.insert(this->lel);
					this->ptr = other;
				}
				else
					this->ptr = 0;
				p->lock.unlock();
				return;
			}
		}
	}

	void onObjectDelete()
	{
		SpinLockGuard myGuard(this->spinLock);
		this->lel.erase();
		this->ptr = 0;
	}
};


template<class t_Class>
static INLINE void safeDelete(t_Class*& p)
{ 
	if(p){
		delete p;
		p = 0;
	}
}

template<class t_Class>
static INLINE void safeDeleteArray(t_Class*& p)
{ 
	if(p){
		delete[] p;
		p = 0;
	}
}



template<class t_Class>
class UniquePtr
{
public:
	typedef t_Class Class_t;
private:
	Class_t* ptr;
public:
	INLINE UniquePtr()
	{
		ptr = nullptr;
	}

	INLINE UniquePtr(Class_t* p)
	{
		ptr = p;
	}

	INLINE ~UniquePtr()
	{
		delete ptr;
	}

private:
	INLINE UniquePtr(const UniquePtr& other)
	{
		delete ptr;
		ptr = other.ptr;
		const_cast<UniquePtr*>(&other)->ptr = nullptr;
	}

	INLINE void operator = (const UniquePtr& other)
	{
		delete ptr;
		ptr = other.ptr;
		const_cast<UniquePtr*>(&other)->ptr = nullptr;
	}
public:

	INLINE void operator = (Class_t* p)
	{
		delete ptr;
		ptr = p;
	}

	INLINE t_Class* operator -> ()const { return this->ptr; }
	INLINE t_Class& operator * ()const { return *this->ptr; }
	INLINE t_Class* get()const { return this->ptr; }
	INLINE operator boolean ()const { return this->ptr != 0; }
	INLINE boolean operator ! ()const { return this->ptr == 0; }
	INLINE operator t_Class* ()const { return this->ptr; }

	INLINE void move(UniquePtr& oldObj)
	{
		delete ptr;
		ptr = oldObj.ptr;
		oldObj.ptr = nullptr;
	}

	INLINE void swap(UniquePtr& b)
	{
		Base::swap(this->ptr, b.ptr);
	}
};

template<class t_Class>
static INLINE void move(UniquePtr<t_Class>& newObj, UniquePtr<t_Class>& oldObj)
{
	newObj.move(oldObj);
}

template<class t_Class>
static INLINE void swap(UniquePtr<t_Class>& a, UniquePtr<t_Class>& b)
{
	a.swap(b);
}

template<class t_Class>
static INLINE void construct_destroy(UniquePtr<t_Class>* newObj, UniquePtr<t_Class>& oldObj)
{
	new(newObj)UniquePtr<t_Class>();
	move(*newObj, oldObj);
	oldObj.~UniquePtr<t_Class>();
}


template<class t_Class>
class UniqueArrPtr
{
public:
	typedef t_Class Class_t;
private:
	Class_t* ptr;
public:
	INLINE UniqueArrPtr()
	{
		ptr = nullptr;
	}

	INLINE UniqueArrPtr(Class_t* p)
	{
		ptr = p;
	}

	INLINE ~UniqueArrPtr()
	{
		delete[] ptr;
	}

private:
	INLINE UniqueArrPtr(const UniqueArrPtr& other)
	{
		delete[] ptr;
		ptr = other.ptr;
		const_cast<UniqueArrPtr*>(&other)->ptr = nullptr;
	}

	INLINE void operator = (const UniqueArrPtr& other)
	{
		delete[] ptr;
		ptr = other.ptr;
		const_cast<UniqueArrPtr*>(&other)->ptr = nullptr;
	}
public:

	INLINE void operator = (Class_t* p)
	{
		delete[] ptr;
		ptr = p;
	}

	INLINE t_Class& operator [] (uint i)const { return this->ptr[i]; }
	INLINE t_Class* get()const { return this->ptr; }
	INLINE operator boolean ()const { return this->ptr != 0; }
	INLINE boolean operator ! ()const { return this->ptr == 0; }

	INLINE void move(UniqueArrPtr& oldObj)
	{
		delete[] ptr;
		ptr = oldObj.ptr;
		oldObj.ptr = nullptr;
	}

	INLINE void swap(UniqueArrPtr& b)
	{
		Base::swap(this->ptr, b.ptr);
	}
};

template<class t_Class>
static INLINE void move(UniqueArrPtr<t_Class>& newObj, UniqueArrPtr<t_Class>& oldObj)
{
	newObj.move(oldObj);
}

template<class t_Class>
static INLINE void swap(UniqueArrPtr<t_Class>& a, UniqueArrPtr<t_Class>& b)
{
	a.swap(b);
}

template<class t_Class>
static INLINE void construct_destroy(UniqueArrPtr<t_Class>* newObj, UniqueArrPtr<t_Class>& oldObj)
{
	new(newObj)UniqueArrPtr<t_Class>();
	move(*newObj, oldObj);
	oldObj.~UniqueArrPtr<t_Class>();
}


}

