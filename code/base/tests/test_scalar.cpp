#include "../include.h"
#include "tests.h"


void Base::testScalarOps()
{
	for(uint i=0; i<32; ++i){
		SeriosAssert(hiOneBit(1 << i) == (int)i);
		SeriosAssert(lowOneBit(1 << i) == i);
		SeriosAssert(hiZeroBit(~(1 << i)) == (int)i);
		SeriosAssert(lowZeroBit(~(1 << i)) == i);
	}

	SeriosAssert(hiOneBit(0xffffffff) == 31);
	SeriosAssert(lowOneBit(0xffffffff) == 0);
	SeriosAssert(hiZeroBit(0xffffffff) < 0);
	SeriosAssert(lowZeroBit(0xffffffff) > 32);

	SeriosAssert(hiOneBit(0) < 0);
	SeriosAssert(lowOneBit(0) > 32);
	SeriosAssert(hiZeroBit(0) == 31);
	SeriosAssert(lowZeroBit(0) == 0);
}


















class Command
{
public:
	int fa;
};





