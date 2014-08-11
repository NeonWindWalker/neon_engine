#include "sync.h"
#include "threading.h"

#ifdef _WIN32

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

#ifdef _USING_V110_SDK71_
	#define WIN32_EMULATE_THREADING_SYNCRONIZATION_PRIMITIVES
#endif

#else

	#include <pthread.h>
	#include <unistd.h>

#endif

#ifdef __ANDROID__
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace Base 
{

void SpinLock::lock()
{
	while(true)
	{
		int prev = atomicExchange(flag, 0);
		if(flag == 0 && prev == 1)
			break;
		Thread::yield();
	}
}

Mutex::Mutex()
{
#ifdef _WIN32
	static_assert(sizeof(_mem) >= sizeof(CRITICAL_SECTION), "small Mutex size");
	InitializeCriticalSection((CRITICAL_SECTION*)_mem);
#else
	static_assert(sizeof(_mem) >= sizeof(pthread_mutex_t) + sizeof(pthread_mutexattr_t), "small Mutex size");
	pthread_mutexattr_t* attr = (pthread_mutexattr_t*)((byte*)_mem + sizeof(pthread_mutex_t));
	pthread_mutexattr_init(attr);
	pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init((pthread_mutex_t*)_mem, attr);
#endif
}

Mutex::~Mutex()
{
#ifdef _WIN32
	DeleteCriticalSection((CRITICAL_SECTION*)_mem);
#else
	pthread_mutexattr_t* attr = (pthread_mutexattr_t*)((byte*)_mem + sizeof(pthread_mutex_t));
	pthread_mutex_destroy((pthread_mutex_t*)_mem);
	pthread_mutexattr_destroy(attr);
#endif
}

void Mutex::lock()
{
#ifdef _WIN32
	EnterCriticalSection((CRITICAL_SECTION*)_mem);
#else
	pthread_mutex_lock((pthread_mutex_t*)_mem);
#endif
}

void Mutex::unlock()
{
#ifdef _WIN32
	LeaveCriticalSection((CRITICAL_SECTION*)_mem);
#else
	pthread_mutex_unlock((pthread_mutex_t*)_mem);
#endif
}

boolean Mutex::tryLock()
{
#ifdef _WIN32
	return TryEnterCriticalSection((CRITICAL_SECTION*)_mem);
#else
	return pthread_mutex_trylock((pthread_mutex_t*)_mem) == 0;
#endif
}

#ifdef _WIN32
	#ifdef WIN32_EMULATE_THREADING_SYNCRONIZATION_PRIMITIVES
	//CONDIDIONAL VARIABLE IMPLIMENTATION
	typedef struct
	{
		int waiters_count_;
		// Count of the number of waiters.

		CRITICAL_SECTION waiters_count_lock_;
		// Serialize access to <waiters_count_>.

		int release_count_;
		// Number of threads to release via a <pthread_cond_broadcast> or a
		// <pthread_cond_signal>.

		int wait_generation_count_;
		// Keeps track of the current "generation" so that we don't allow
		// one thread to steal all the "releases" from the broadcast.

		HANDLE event_;
		// A manual-reset event that's used to block and release waiting
		// threads.
	} my_pthread_cond_t;
	#endif
#endif

ConditionVariable::ConditionVariable()
{
#ifdef _WIN32
	#ifndef WIN32_EMULATE_THREADING_SYNCRONIZATION_PRIMITIVES
		static_assert(sizeof(_mem) >= sizeof(CONDITION_VARIABLE), "small ConditionVariable size");
		InitializeConditionVariable((CONDITION_VARIABLE*)_mem);
	#else
		static_assert(sizeof(_mem) >= sizeof(my_pthread_cond_t), "small ConditionVariable size");
		my_pthread_cond_t* cv = (my_pthread_cond_t*)_mem;

		cv->waiters_count_ = 0;
		cv->wait_generation_count_ = 0;
		cv->release_count_ = 0;

		InitializeCriticalSection(&cv->waiters_count_lock_);

		// Create a manual-reset event.
		cv->event_ = CreateEvent (NULL,  // no security
			TRUE,  // manual-reset
			FALSE, // non-signaled initially
			NULL); // unnamed
	#endif
#else
	static_assert(sizeof(_mem) >= sizeof(pthread_cond_t), "small ConditionVariable size");
	pthread_cond_init((pthread_cond_t*)_mem, 0);
#endif
}

ConditionVariable::~ConditionVariable()
{
#ifdef _WIN32
	#ifndef WIN32_EMULATE_THREADING_SYNCRONIZATION_PRIMITIVES
	#else
	my_pthread_cond_t* cv = (my_pthread_cond_t*)_mem;
	CloseHandle(cv->event_);
	DeleteCriticalSection(&cv->waiters_count_lock_);
	#endif
#else
	pthread_cond_destroy((pthread_cond_t*)_mem);
#endif
}

void ConditionVariable::notifyOne()
{
#ifdef _WIN32
	#ifndef WIN32_EMULATE_THREADING_SYNCRONIZATION_PRIMITIVES
		WakeConditionVariable((CONDITION_VARIABLE*)_mem);
	#else
		my_pthread_cond_t* cv = (my_pthread_cond_t*)_mem;

		EnterCriticalSection (&cv->waiters_count_lock_);
		if (cv->waiters_count_ > cv->release_count_) {
			SetEvent (cv->event_); // Signal the manual-reset event.
			cv->release_count_++;
			cv->wait_generation_count_++;
		}
		LeaveCriticalSection (&cv->waiters_count_lock_);
	#endif
#else
	pthread_cond_signal((pthread_cond_t*)_mem);
#endif
}

void ConditionVariable::notifyAll()
{
#ifdef _WIN32
	#ifndef WIN32_EMULATE_THREADING_SYNCRONIZATION_PRIMITIVES
		WakeAllConditionVariable((CONDITION_VARIABLE*)_mem);
	#else
		my_pthread_cond_t* cv = (my_pthread_cond_t*)_mem;

		EnterCriticalSection (&cv->waiters_count_lock_);
		if (cv->waiters_count_ > 0) {
			SetEvent (cv->event_);
			// Release all the threads in this generation.
			cv->release_count_ = cv->waiters_count_;

			// Start a new generation.
			cv->wait_generation_count_++;
		}
		LeaveCriticalSection (&cv->waiters_count_lock_);
	#endif
#else
	pthread_cond_broadcast((pthread_cond_t*)_mem);
#endif
}

void ConditionVariable::wait(Mutex& mutex)
{
#ifdef _WIN32
	#ifndef WIN32_EMULATE_THREADING_SYNCRONIZATION_PRIMITIVES
		SleepConditionVariableCS((CONDITION_VARIABLE*)_mem, (CRITICAL_SECTION*)mutex._mem, INFINITE);
	#else
		my_pthread_cond_t* cv = (my_pthread_cond_t*)_mem;

		// Avoid race conditions.
		EnterCriticalSection (&cv->waiters_count_lock_);

		// Increment count of waiters.
		cv->waiters_count_++;

		// Store current generation in our activation record.
		int my_generation = cv->wait_generation_count_;

		LeaveCriticalSection (&cv->waiters_count_lock_);
		LeaveCriticalSection ((CRITICAL_SECTION*)mutex._mem);

		for (;;) {
			// Wait until the event is signaled.
			WaitForSingleObject (cv->event_, INFINITE);

			EnterCriticalSection (&cv->waiters_count_lock_);
			// Exit the loop when the <cv->event_> is signaled and
			// there are still waiting threads from this <wait_generation>
			// that haven't been released from this wait yet.
			int wait_done = cv->release_count_ > 0
				&& cv->wait_generation_count_ != my_generation;
			LeaveCriticalSection (&cv->waiters_count_lock_);

			if (wait_done)
				break;
		}

		EnterCriticalSection ((CRITICAL_SECTION*)mutex._mem);
		EnterCriticalSection (&cv->waiters_count_lock_);
		cv->waiters_count_--;
		cv->release_count_--;
		int last_waiter = cv->release_count_ == 0;
		LeaveCriticalSection (&cv->waiters_count_lock_);

		if (last_waiter)
			// We're the last waiter to be notified, so reset the manual event.
			ResetEvent (cv->event_);
	#endif
#else
	pthread_cond_wait((pthread_cond_t*)_mem, (pthread_mutex_t*)mutex._mem);
#endif
}



#ifndef __APPLE__
THREAD_LOCAL Thread* gtThisThread = 0;
#endif
volatile int gThreadIdIncremental = 0;

struct ThreadStartFuncArgs
{
	Thread* owner;
	Delegate<void (void*)> func;
	void* arg;
	Delegate<void ()> func2;
};

#ifdef _WIN32
DWORD WINAPI thread_stat_func(LPVOID args)
#else
void* thread_stat_func(void* args)
#endif
{
	ThreadStartFuncArgs* d = (ThreadStartFuncArgs*)args;
	ThreadStartFuncArgs dc = *d;
	delete d;

#ifndef __APPLE__
	gtThisThread = dc.owner;
#endif
    
	if(dc.func)
		dc.func(dc.arg);
	else
		dc.func2();

	return 0;
}

void Thread::preinit()
{
	id = atomicIncrement(gThreadIdIncremental);
	//print(name, id);
	for(uint i=0; i < gcDataSize; ++i)
		_mem[i] = 0;
}

Thread::Thread(Delegate<void (void*)> func, void* arg)
{
	preinit();

	ThreadStartFuncArgs* args = new ThreadStartFuncArgs();
	args->owner = this;
	args->func = func;
	args->arg = args;

#ifdef _WIN32
	static_assert(sizeof(_mem) >= sizeof(HANDLE), "small Thread size");
	*(HANDLE*)_mem = CreateThread(0, 0, thread_stat_func, args, 0, 0);
	Assert(*(HANDLE*)_mem != INVALID_HANDLE_VALUE);
#else
	static_assert(sizeof(_mem) >= sizeof(pthread_t), "small Thread size");
	pthread_create((pthread_t*)&_mem, 0, thread_stat_func, args);
#endif
}

Thread::Thread(Delegate<void ()> func)
{
	preinit();

	ThreadStartFuncArgs* args = new ThreadStartFuncArgs();
	args->owner = this;
	args->func2 = func;

#ifdef _WIN32
	static_assert(sizeof(_mem) >= sizeof(HANDLE), "small Thread size");
	*(HANDLE*)_mem = CreateThread(0, 0, thread_stat_func, args, 0, 0);
	Assert(*(HANDLE*)_mem != INVALID_HANDLE_VALUE);
#else
	static_assert(sizeof(_mem) >= sizeof(pthread_t), "small Thread size");
	pthread_create((pthread_t*)&_mem, 0, thread_stat_func, args);
#endif
}

Thread::Thread()
{
	preinit();
}

Thread::~Thread()
{
	uint i=0;
	for(; i < gcDataSize; ++i)
		if(_mem[i])
			break;
	if(i == gcDataSize)
		return;

#ifdef _WIN32
	WaitForSingleObject(*(HANDLE*)_mem, INFINITE);
	CloseHandle(*(HANDLE*)_mem);
#else
	void* ret;
	pthread_join(*(pthread_t*)_mem, &ret);
#endif
}

void Thread::attachToExternalCreatedThread()
{
#ifndef __APPLE__
	if(gtThisThread)
		return;
	gtThisThread = new Thread();
#endif
}

void Thread::detachFromExternalCreatedThread()
{
#ifndef __APPLE__
	if(!gtThisThread)
		return;
	delete gtThisThread;
	gtThisThread = 0;
#endif
}

void Thread::yield()
{
#ifdef _WIN32
	Yield();
#else
	#if !defined(__ANDROID__) && !defined(__APPLE__)
		pthread_yield();
	#else
		sched_yield();
	#endif
#endif
}

void Thread::sleep(float time)
{
#ifdef _WIN32
	Sleep(uint(time * 1000.0f));
#else
	usleep(uint(time * 1000000.0f));
#endif
}

uint Thread::getHardwareThreadsCount()
{
#ifdef _WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#endif

#ifdef __linux__
	int onln = sysconf(_SC_NPROCESSORS_ONLN);
	int conf = sysconf(_SC_NPROCESSORS_CONF);
	int num = onln > conf ? onln : conf;
#ifdef __ANDROID__
	int i=0;
	while(true)
	{
		struct stat st;
		char path[64];
		sprintf(path, "/sys/devices/system/cpu/cpu%d", i);
		if(stat(path, &st))
			break;
		i++;
	}
	num = num > i ? num : i;
#endif
	return num;
#endif
    
#ifdef __APPLE__
	return max(max((int)sysconf( _SC_NPROCESSORS_ONLN ), (int)sysconf(_SC_NPROCESSORS_CONF)), (int)0);
#endif

}


}
