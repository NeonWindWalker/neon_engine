#pragma once

#include "seed.h"

namespace Base 
{

class Timer
{
public:
	Timer();
	double dt();

	static double time();

protected:

#ifdef _WIN32
	uint64 _start;
	double _invfreq;
#endif

#if defined(__linux__) || defined(__APPLE__)
	void* data[4];
#endif
};

}
