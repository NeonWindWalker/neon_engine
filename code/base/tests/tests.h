#pragma once
#include "../log.h"

namespace Base
{

extern void testFloatingPointOperations();
extern void testAllocators();
extern void testContainers();
extern void testOctree();
extern void testQuadtree();
extern void testAlghoritms();
extern void threadingTest();
extern void testDelegates();
extern void testScalarOps();
extern void testPointers();
extern void testString();

static INLINE void testAll()
{
	Base::logCompileInfo();

	logInfoSkim<<"Base Tests Start";
	Base::testString();
	Base::testScalarOps();
	Base::testAlghoritms();
	Base::testFloatingPointOperations();
	Base::testAllocators();
	Base::testContainers();
	Base::testOctree();
	Base::testQuadtree();
	
	Base::threadingTest();
	Base::testPointers();
	Base::testDelegates();
	logInfoSkim<<"Base Tests Done!";
}

}
