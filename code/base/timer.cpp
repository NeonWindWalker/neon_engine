#include "timer.h"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <Mmsystem.h>


namespace Base 
{

Timer::Timer()
{
	uint64 f;
	QueryPerformanceFrequency((LARGE_INTEGER*)&f);
	_invfreq = 1.0 / double(f);
	QueryPerformanceCounter((LARGE_INTEGER*)&_start);
}
double Timer::dt()
{
	uint64 t;
	QueryPerformanceCounter((LARGE_INTEGER*)&t);
	double deltha = double(t - _start) * _invfreq;
	_start = t;
	return deltha;
}

double Timer::time()
{
	double t = double(::timeGetTime()) * 1e-3;
	return t;
}

}
#endif


#if defined(__linux__) && !defined(__APPLE__)

#include <sys/time.h>

namespace Base {

Timer::Timer()
{
	static_assert(sizeof(data) >= sizeof(struct timeval), "small Thread size");
	static_assert(sizeof(data) >= sizeof(clock_t), "small Thread size");
	clock_t* p1 = reinterpret_cast<clock_t*>(data);
	*p1 = clock();
	if(*p1 == (clock_t)-1)
	{
		logWarning << "clock unsupported";
		struct timeval* p2 = reinterpret_cast<struct timeval*>(data);
		::gettimeofday(p2, 0);
	}
}

double Timer::dt()
{
	clock_t* p1 = reinterpret_cast<clock_t*>(data);
	clock_t t = clock();
	if(*p1 != (clock_t)-1)
	{
		double deltha = double(t - *p1) * double(1.0 / double(CLOCKS_PER_SEC));
		*p1 = t;
		return deltha;
	}
	else
	{
		struct timeval* p = reinterpret_cast<struct timeval*>(data);
		struct timeval t;
		::gettimeofday(&t, 0);
		double deltha = double(t.tv_sec - p->tv_sec) + double(t.tv_usec - p->tv_usec) * 0.000001;
		*p = t;
		return deltha;
	}
}

double Timer::time()
{
	struct timeval t;
	::gettimeofday(&t, NULL);
	return double(t.tv_sec) + double(t.tv_usec) * 0.000001;
}

}
#endif



#if defined(__APPLE__)

#include <mach/mach_time.h>

namespace Base
{

Timer::Timer()
{
	static_assert(sizeof(data) >= sizeof(double) * 2, "small Timer size");
	uint64* t = reinterpret_cast<uint64*>(data);
	double* s = reinterpret_cast<double*>(data) + 1;
	
    *t = mach_absolute_time();
    mach_timebase_info_data_t tbInfo;
    mach_timebase_info(&tbInfo);
    *s = tbInfo.numer / (1e9*tbInfo.denom);
}

double Timer::dt()
{
    uint64* t = reinterpret_cast<uint64*>(data);
	double* s = reinterpret_cast<double*>(data) + 1;
    uint64 oldT = *t;
    *t = mach_absolute_time();
    return (*t - oldT)*(*s);
}

double Timer::time()
{
	uint64 t = mach_absolute_time();
	mach_timebase_info_data_t tbInfo;
	mach_timebase_info(&tbInfo);
	return double(t) * double(tbInfo.numer) / (1e9*tbInfo.denom);
}

}
#endif
