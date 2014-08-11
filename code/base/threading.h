#pragma once

#include "sync.h"
#include "delegate.h"
#include "string.h"

namespace Base
{

class Thread : public Uncopyable
{
public:
	Thread(Delegate<void (void*)> func, void* arg);
	Thread(Delegate<void ()> func);
	~Thread();

	static void attachToExternalCreatedThread();
	static void detachFromExternalCreatedThread();
	static uint getHardwareThreadsCount();
	static void yield();
	static void sleep(float time);

	String name;
	uint id;
protected:
	static const uint gcDataSize = 8;
	void* _mem[gcDataSize];
	Thread();
	void preinit();
};

#ifndef __APPLE__
extern THREAD_LOCAL Thread* gtThisThread;
#endif


class SeparateThreadWorker
{
public:
	SeparateThreadWorker(Delegate<void ()> func)
	{
		this->func = func;
		init();
	}

	SeparateThreadWorker(Delegate<void (void*)> func, void* arg)
	{
		this->func2 = func;
		this->func2Arg = arg;
		init();
	}

	~SeparateThreadWorker()
	{
		stopFlag = true;
		conditionVariable.notifyOne();
		mutex.unlock();
		delete thread;
	}

	INLINE void begin()
	{
		conditionVariable.notifyOne();
		mutex.unlock();
	}

	INLINE void end()
	{
		while(!stepAccepted)
			Thread::yield();

		mutex.lock();
		stepAccepted = false;
	}

private:
	Thread* thread;
	Delegate<void ()> func;
	Delegate<void (void*)> func2;
	void* func2Arg;

	volatile boolean stopFlag;
	volatile boolean isStarted;
	volatile boolean stepAccepted;
	Mutex mutex;
	ConditionVariable conditionVariable;

	void init()
	{
		stopFlag = false;
		isStarted = false;
		stepAccepted = false;
		thread = new Thread(makeDelegate(this, &SeparateThreadWorker::threadFunc));
		while(!isStarted)
			Thread::yield();
		mutex.lock();
	}

	void threadFunc()
	{
		mutex.lock();
		isStarted = true;

		while(true)
		{
			conditionVariable.wait(mutex);

			stepAccepted = true;

			if(stopFlag){
				mutex.unlock();
				return;
			}

			if(func)
				func();
			else
				func2(func2Arg);
		}
	}
};


/*
// slow
class SeparateThreadWorkerByMutexes
{
public:
	SeparateThreadWorkerByMutexes(Delegate<void ()> func)
	{
		this->func = func;
		init();
	}

	SeparateThreadWorkerByMutexes(Delegate<void (void*)> func, void* arg)
	{
		this->func2 = func;
		this->func2Arg = arg;
		init();
	}

	~SeparateThreadWorkerByMutexes()
	{
		stopFlag = true;
		ownData.unlock();
		delete thread;
	}

	INLINE void begin()
	{
		postSimmulationWait.lock();
		ownData.unlock();
	}

	INLINE void end()
	{
		pedanticStart.lock();
		pedanticStart.unlock();

		ownData.lock();
		postSimmulationWait.unlock();

		pedanticEnd.lock();
		pedanticEnd.unlock();
	}

private:
	Thread* thread;
	Delegate<void ()> func;
	Delegate<void (void*)> func2;
	void* func2Arg;

	volatile boolean stopFlag;
	volatile boolean isStarted;
	Mutex ownData; // основная критическая секция распаралеливания симмуляции частиц
	Mutex postSimmulationWait; // для остановки потока после того как он сделал всю работу и ждет синхронизацию
	Mutex pedanticEnd, pedanticStart; // для педантичности так сказать.
	//исправляет ситуацию когда поток может остановится между строчками
	//вероятность то почти никакая, но для дущевного спокойствия

	void init()
	{
		ownData.lock();
		stopFlag = false;
		isStarted = false;
		thread = new Thread(makeDelegate(this, &SeparateThreadWorkerByMutexes::threadFunc));
		while(!isStarted)
			Thread::yield();
	}

	void threadFunc()
	{
		pedanticStart.lock();
		isStarted = true;

		while(true)
		{
			ownData.lock();
			pedanticStart.unlock();

			if(stopFlag){
				ownData.unlock();
				return;
			}

			if(func)
				func();
			else
				func2(func2Arg);

			pedanticEnd.lock();
			ownData.unlock();

			postSimmulationWait.lock();
			postSimmulationWait.unlock();

			pedanticStart.lock();
			pedanticEnd.unlock();
		}
	}
};
*/


}



