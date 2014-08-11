#include "../include.h"
#include "tests.h"

void Base::testAllocators()
{
	logInfoSkim<<"<<<<< CONTAINERS TESTS >>>>>";
	Base::uint Count = 100000;

#ifdef DEBUG
	Count /= 100;
#endif
        
	int** mem = new int* [Count];
	Base::Timer timer;
	int summ = 0;

	Base::TempAllocator pa;
	timer.dt();
	for(Base::uint i=0; i < Count; i++)
		mem[i] = new(pa) int;
	logInfoSkim<<"TempAllocator new int time ns = "<< timer.dt() * 1e9 / Count;

	for(int i=0; i < Count; i++)
		summ += *mem[i];

	for(Base::uint i=0; i < Count; i++){
		summ += *mem[i];
		delete mem[i];
	}
	pa.free();

	Base::TCellDefragmentAllocator<int> ca;
	timer.dt();
	for(Base::uint i=0; i < Count; i++)
		mem[i] = new(ca) int;
	logInfoSkim<<"TCellDefragmentAllocator new int time ns = "<<timer.dt() * 1e9 / Count;

	for(int i=0; i < Count; i++)
		summ += *mem[i];

	for(Base::uint i=0; i < Count; i++){
		summ += *mem[i];
		delete mem[i];
	}

	timer.dt();
	for(Base::uint i=0; i < Count; i++)
		mem[i] = (int*)malloc(sizeof(int));
	logInfoSkim<<"malloc int time ns = " << timer.dt() * 1e9 / Count;

	for(int i=0; i < Count; i++)
		summ += *mem[i];

	for(Base::uint i=0; i < Count; i++){
		summ += *mem[i];
		free(mem[i]);
	}

	timer.dt();
	for(Base::uint i=0; i < Count; i++)
		mem[i] = new int;
	logInfoSkim<<"new int time ns = "<<timer.dt() * 1e9 / Count;

	for(int i=0; i < Count; i++)
		summ += *mem[i];

	for(Base::uint i=0; i < Count; i++){
		summ += *mem[i];
		delete mem[i];
	}

	delete[] mem;

	logInfoSkim<<"summ " << summ;
	logInfoSkim;
}
