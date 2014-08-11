#include "../include.h"
#include "tests.h"


class DelegateTest
{
public:
	int count;
	DelegateTest() : count(0) {}
	int inc(int x) { return count ++; }

	static int sfun(int i) { return i; }
};

class DelegateTest2
{
public:
	int count;
	DelegateTest2() : count(0) {}
	virtual int inc(int x) { return count ++; }
};

class DelegateTest3
{
public:
	int count2;
	DelegateTest3() : count2(0) {}
	virtual int inc(int x) { return count2 ++; }
};

struct A
{
	int a;
	void VFunc1() { a = 1; }
};

struct B
{
	int b;
	void VFunc2() { b = 2; }
};

struct C : public A, public B
{
};





template<class t_Class, class t_Ret, class t_Arg0>
class Closure1
{	
public:

	class IClosure
	{
	public:
		virtual t_Ret dosume(t_Arg0 a)=0;
	};

	typedef t_Ret (t_Class::*t_MethodPtr)(t_Arg0);
	t_Class* obj;
	t_MethodPtr met;
	Closure1(t_Class* o, t_MethodPtr m)
	{
		obj = o;
		met = m;
	}
	t_Ret operator() (t_Arg0 arg0) { return (obj->*met)(arg0); }
};

void Base::testDelegates()
{
	DelegateTest inst;
	DelegateTest2 inst2;
	DelegateTest3 inst3;

	Base::Delegate<int(int)> xcl(&inst, &DelegateTest::inc);
	Base::Delegate<int(int)> xcl2(&inst2, &DelegateTest2::inc);

	Base::Event<int (int)> delegate;
	delegate += xcl;
	delegate += Base::makeDelegate(&inst2, &DelegateTest2::inc);
	delegate += Base::makeDelegate((DelegateTest2*)&inst3, &DelegateTest2::inc);
	delegate += Base::makeDelegate(&DelegateTest::sfun);
	

	delegate(3);
	delegate -= xcl2;
	delegate -= xcl;
	delegate -= Base::makeDelegate(&inst2, &DelegateTest2::inc);

	C c;
	Base::Event<void (void)> delegate2;
	delegate2 += Base::makeDelegate(static_cast<B*>(&c), &C::VFunc2);
	delegate2();
}
