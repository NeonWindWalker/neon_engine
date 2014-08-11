#include "include.h"
#include <time.h>

namespace Base
{

SystemAllocator gSystemAllocator;
byte gSplineMatrixesCollectionMemForFixAligningInCLANG[sizeof(SplineMatrixesCollection) + 32];

class GlobalInitializerStaticClass
{
public:
	GlobalInitializerStaticClass()
	{
		new(gSplineMatrixesCollection()) SplineMatrixesCollection();
		srand ( (uint)::time(0) );
		int x = ::rand();
	}

	~GlobalInitializerStaticClass()
	{
	}
};

GlobalInitializerStaticClass gGlobalInitializerStaticClass;


TempAllocatorPageHeader* TempAllocatorPageHeader::reorderBySizeAndGetUsedSize(uinteger& usedSize, TempAllocatorPageHeader* first)
{
	usedSize = 0;
	if(!first)
		return 0;

	Array< Pair<uinteger,TempAllocatorPageHeader*>, SystemAllocatorWrapper, 64> arr;
	for(TempAllocatorPageHeader* p = first; p; p = p->next){
		uinteger s = p->usedBytes();
		usedSize += s;
		arr.push(makePair(s, p));
	}

	arr.sort();

	for(uint i=1; i < arr.size(); ++i)
		arr[i-1].val->next = arr[i].val;

	arr.last()->val->next = 0;
	return arr.first()->val;
}



}
