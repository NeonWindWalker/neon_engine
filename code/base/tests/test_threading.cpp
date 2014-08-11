#include "../include.h"
#include "tests.h"


static Base::boolean gMayStartDo = false;

void thread_fun_m34(void* arg)
{
	Base::uint cyclesCount = 1000000;

#ifdef DEBUG
	cyclesCount /= 100;
#endif

	Base::Matrix34f m;
	m.setIdentity();

	for(Base::uint i=0; i < cyclesCount; ++i)
	{
		m[0][0] = Base::rand(0.0f, 1.0f);
		m *= m;
		m *= m;
		m *= m;
		m *= m;
		m *= m;
		m *= m;
		m *= m;
		m *= m;
		m *= m;
		m *= m;
	}

	logInfo << m.m33().x.lenghtSquare();
}


void thread_fun_mapops(void* arg)
{
	Base::uint Count = 10000;

#ifdef DEBUG
	Count /= 100;
#endif

	Base::uint* rang = new Base::uint[Count];
	for(Base::uint i=0; i < Count; i++)
		rang[i] = Base::rand(0,1000000000);

	int summ = 0;
	for(int nnn=0; nnn < 10; ++nnn)
	{
		Base::RedBlackTreeMap< Base::Pair<Base::uint, Base::uint> > mymap;

		Base::boolean isInserted;
		for(Base::uint i=0; i < Count; i++)
			mymap.insert(isInserted, rang[i])->val = rang[i];

		for(Base::uint i=0; i < Count; i+=2)
			mymap.erase(rang[i]);

		//mymap.defragment();

		for(Base::uint i=0; i < Count; i++){
			Base::RedBlackTreeMap< Base::Pair<Base::uint, Base::uint> >::Iterator n = mymap.find(rang[i]);
			if(n != mymap.end())
				Assert(n->val == rang[i]);
		}

		int iters = 0;
		for(Base::RedBlackTreeMap< Base::Pair<Base::uint, Base::uint> >::Iterator i = mymap.first(); i != mymap.end(); ++i, iters++)
			summ += i->val;

	}

	delete[] rang;

	logInfo << summ;
}


Base::ConditionVariable* gWorkerConditionVariable=0;
Base::Array<int> gWorkerList;
Base::Mutex* gWorkerListMutex;
bool gWorkDone = false;
volatile int gWorkThreadsDoneCount = 0;

void thread_worker(void* arg)
{
	Base::uint Count = 10000;

#ifdef DEBUG
	Count /= 100;
#endif

	int summ = 0;
	while(!gWorkDone)
	{
		gWorkerListMutex->lock();
		gWorkerConditionVariable->wait(*gWorkerListMutex);
		if(gWorkDone){
			gWorkerListMutex->unlock();
			Base::atomicIncrement(gWorkThreadsDoneCount);
			return;
		}
		summ += gWorkerList.popr();
		gWorkerListMutex->unlock();
		for(Base::uint i=0; i < Count; ++i)
			summ += (int)sinf(summ);
	}
	Base::atomicIncrement(gWorkThreadsDoneCount);
	logInfo << summ;
}

void emptyFunc()
{

}


void Base::threadingTest()
{
	logInfoSkim<<"<<<<< THREADING TESTS >>>>>";
	int summ = 0;

	Base::Timer timer;
	Base::uint nMaxThreads = Base::Thread::getHardwareThreadsCount();
	logInfoSkim<<"getHardwareThreadsCount = " << nMaxThreads;

	if(nMaxThreads < 2)
		nMaxThreads = 2;

	Base::Mutex mutex;
	timer.dt();
	for(int i=0; i < 100000; ++i)
	{
		mutex.lock();
		summ ++;
		mutex.unlock();
	}
	logInfoSkim<<"Mutex lock unlock ns = " << 1e9 * timer.dt() / 100000.0;

	Base::SpinLock spin;
	timer.dt();
	for(int i=0; i < 100000; ++i)
	{
		spin.lock();
		summ ++;
		spin.unlock();
	}
	logInfoSkim<<"SpinLock lock unlock ns = " << 1e9 * timer.dt() / 100000.0;

	volatile int incDecAtom;
	timer.dt();
	for(int i=0; i < 1000000; ++i){
		Base::atomicIncrement(incDecAtom);
		Base::atomicDecrement(incDecAtom);
		Base::atomicIncrement(incDecAtom);
		Base::atomicDecrement(incDecAtom);
		Base::atomicIncrement(incDecAtom);
		Base::atomicDecrement(incDecAtom);
		Base::atomicIncrement(incDecAtom);
		Base::atomicDecrement(incDecAtom);
		Base::atomicIncrement(incDecAtom);
		Base::atomicDecrement(incDecAtom);
	}
	logInfoSkim<<"atomic atomicIncrement/atomicDecrement time ns " << 1e9 * timer.dt() / 10000000.0;

	/*
	timer.dt();
	for(int i=0; i < 1000000; ++i){
		summ += Base::Thread::i();
		summ += Base::Thread::getThisThreadId();
		summ += Base::Thread::getThisThreadId();
		summ += Base::Thread::getThisThreadId();
		summ += Base::Thread::getThisThreadId();
		summ += Base::Thread::getThisThreadId();
		summ += Base::Thread::getThisThreadId();
		summ += Base::Thread::getThisThreadId();
		summ += Base::Thread::getThisThreadId();
		summ += Base::Thread::getThisThreadId();
	}
	logInfoSkim<<"getThisThreadId time ns " << 1e9 * timer.dt() / 10000000.0;
	*/

	double times[17];
	for(Base::uint nThr=1; nThr <= nMaxThreads; nThr ++)
	{
		gMayStartDo = false;
		Base::Thread* at[16];
		for(Base::uint i=0; i < nThr; ++i)
			at[i] = new Base::Thread(Base::makeDelegate(&thread_fun_m34), 0);

		timer.dt();
		gMayStartDo = true;

		for(Base::uint i=0; i < nThr; ++i)
			delete at[i];

		times[nThr] = timer.dt();
		logInfoSkim<<"num threads = " << nThr << " fp ops time=" << times[nThr] << " boost =" <<  times[1] / times[nThr] * nThr;
	}

	for(Base::uint nThr=1; nThr <= nMaxThreads; nThr ++)
	{
		gMayStartDo = false;
		Base::Thread* at[16];
		for(Base::uint i=0; i < nThr; ++i)
			at[i] = new Base::Thread(Base::makeDelegate(&thread_fun_mapops), 0);

		timer.dt();
		gMayStartDo = true;

		for(Base::uint i=0; i < nThr; ++i)
			delete at[i];

		times[nThr] = timer.dt();
		logInfoSkim<<"threads count = " << nThr << " map ops time = " << times[nThr] << " boost = " << times[1] / times[nThr] * nThr;
	}

	
	gWorkerConditionVariable = new Base::ConditionVariable();
	gWorkerListMutex = new Base::Mutex();
	for(Base::uint nThr=1; nThr <= nMaxThreads; nThr ++)
	{
		gWorkDone = false;
		Base::Thread* at[16];
		gWorkThreadsDoneCount = 0;
		for(Base::uint i=0; i < nThr; ++i)
			at[i] = new Base::Thread(Base::makeDelegate(&thread_worker), 0);

		for(uint i=0; i < 1000; ++i){
			gWorkerListMutex->lock();
			gWorkerList.push((int)i);
			gWorkerListMutex->unlock();
			gWorkerConditionVariable->notifyOne();
		}

		gWorkDone = true;
		while(gWorkThreadsDoneCount != (int)nThr)
			gWorkerConditionVariable->notifyAll();

		for(Base::uint i=0; i < nThr; ++i)
			delete at[i];
	}
	delete gWorkerListMutex;
	delete gWorkerConditionVariable;


	Base::SeparateThreadWorker stw1(Base::makeDelegate(&emptyFunc));
	timer.dt();
	for(uint i=0; i < 1000000; ++i)
	{
		stw1.begin();
		stw1.end();
	}
	logInfoSkim<<"SeparateThreadWorker itaration = " << timer.dt() * 1e9 / 1000000 << " ns";

	/*
	Base::SeparateThreadWorkerByMutexes stw2(Base::makeDelegate(&emptyFunc));
	timer.dt();
	for(uint i=0; i < 1000000; ++i)
	{
		stw2.begin();
		stw2.end();
	}
	logInfoSkim<<"SeparateThreadWorkerByMutexes itaration = " << timer.dt() * 1e9 / 1000000 << " ns";
		*/

	logInfoSkim<<"." << summ;
}
