#include "../include.h"
#include "tests.h"


class MyRefClass : public Base::RefCountable
{
public:

};

void Base::testPointers()
{
	Base::StrongPtr<MyRefClass> shptr = new MyRefClass();
	Base::StrongPtr<MyRefClass> shptr23 = shptr;
	Base::StrongPtrThreadSafe<MyRefClass> shptr2 = shptr;
	Base::WeakPtr<MyRefClass> wk = shptr2;
	Base::WeakPtr<MyRefClass> wk2 = wk;
	wk.lock();

	Base::swap(shptr23, shptr);
	Base::move(shptr23, shptr);
}
