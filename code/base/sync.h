#pragma once

#include "seed.h"

namespace Base
{

#ifdef _MSC_VER
	//#pragma intrinsic (_atomicIncrement)
	//#pragma intrinsic (_InterlockedDecrement)

	static INLINE int atomicIncrement(volatile int& val)
	{
		return _InterlockedIncrement((volatile long*)&val);
	}
	static INLINE int atomicDecrement(volatile int& val)
	{
		return _InterlockedDecrement((volatile long*)&val);
	}
	static INLINE int atomicExchange(volatile int& val, int newval)
	{
		return _InterlockedExchange((volatile long*)&val, newval);
	}
	static INLINE int atomicAdd(volatile int& val, int d)
	{
		return _InterlockedExchangeAdd((volatile long*)&val, d) + d;
	}
	static INLINE int atomicGetAndAdd(volatile int& val, int d)
	{
		return _InterlockedExchangeAdd((volatile long*)&val, d);
	}
#endif

#ifdef __GNUG__
	static INLINE int atomicIncrement(volatile int& val)
	{
		return __sync_add_and_fetch(&val, 1);
	}

	static INLINE int atomicDecrement(volatile int& val)
	{
		return __sync_sub_and_fetch(&val, 1);
	}

	static INLINE int atomicExchange(volatile int& val, int newval)
	{
		return __sync_lock_test_and_set((volatile long*)&val, newval);
	}
	
	static INLINE int atomicAdd(volatile int& val, int d)
	{
		return __sync_add_and_fetch(&val, d);
	}
	
	static INLINE int atomicGetAndAdd(volatile int& val, int d)
	{
		return __sync_fetch_and_add(&val, d);
	}
#endif


class SpinLock
{
public:
	INLINE SpinLock() : flag(1) {} 

	void lock();

	INLINE boolean tryLock()
	{
		int prev = atomicExchange(flag, 0);
		return flag == 0 && prev == 1;
	}
	
	INLINE void unlock()
	{
		atomicExchange(flag, 1);
	}

protected:
	SpinLock(const SpinLock& other) {}
	void operator = (const SpinLock& other) {}

private:
	volatile int flag;
};


class Mutex
{
public:
	Mutex();
	~Mutex();
	void lock();
	void unlock();
	boolean tryLock();

protected:
	Mutex(const Mutex& other) {}
	void operator = (const Mutex& other) {}

	void* _mem[16];

	friend class ConditionVariable;
};

struct GuardLockableIsLocked {};

class LockGuard
{
public:
	INLINE LockGuard(Mutex& m, GuardLockableIsLocked) : mutex(m) {}
	INLINE LockGuard(Mutex& m) : mutex(m) { mutex.lock(); }
	INLINE ~LockGuard() { mutex.unlock(); }
	Mutex& mutex;
};

class SpinLockGuard
{
public:
	INLINE SpinLockGuard(SpinLock& m, GuardLockableIsLocked) : spinlock(m) {}
	INLINE SpinLockGuard(SpinLock& m) : spinlock(m) { spinlock.lock(); }
	INLINE ~SpinLockGuard() { spinlock.unlock(); }
	SpinLock& spinlock;
};

class ConditionVariable
{
public:
	ConditionVariable();
	~ConditionVariable();

	void notifyOne();
	void notifyAll();
	void wait(Mutex& mutex);

protected:
	ConditionVariable(const ConditionVariable& other) {}
	void operator = (const ConditionVariable& other) {}

	void* _mem[16];
};

}

