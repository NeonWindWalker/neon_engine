#include "../include.h"

//TODO EChildMode

enum TestEnum
{
	TestEnum_A = 0,
	TestEnum_B,
	TestEnum_C,
	TestEnum_EnumCount
};


#define ReflectionEnum_EnumStrings {"A",			"B",				"C"}
#define ReflectionEnum_EnumValues  {TestEnum_A,		TestEnum_B,			TestEnum_C}
ReflectionEnum(TestEnum, TestEnum_EnumCount)
#undef ReflectionEnum_EnumStrings
#undef ReflectionEnum_EnumValues


class TestClass : public virtual Base::RefCountable
{
public:
	int i;
	float f;
	Base::String s;
	float get_f(){ return f * 2; }
	void set_f(float v) { f = v / 2; }
	TestClass()
	{
		i = 1;
		f = 3.14f;
		s = _CS("bla bla bla");
		enm = TestEnum_B; 
		k = 3.14f;
	}

	TestClass( const TestClass& other)
	{

	}

	TestClass& operator = ( const TestClass& other)
	{
		return *this;
	}

	rl_class(TestClass)

	//ReflectionDynamicField(i,i, new(mem) ReflectionLib::TType<int>(), new(mem) ReflectionLib::TReflection<int>(i), return false; , return false;)
	
	//ReflectionMethod2(meth, meth, int, float)

	rl_field(float, k)

	Base::ConstString str()
	{
		return s;
	}

	void str(const Base::ConstString& in)
	{
		s = in;
	}

	rl_val_property(Base::String, str)

	rl_field(TestEnum, enm)

	rl_field(int, pi)

	rl_field(Base::Array<int>, intarr)

	rl_method_1(met, float, f)
	{
		f *= 4;
	}

	rl_method_1r(met2, float, float, f)
	{
		return f * 2;
	}
	

	//rl_field(Base::Array<int>, intarr)
	
	
};

class TestCompositionClass
{
public:
	rl_class(TestCompositionClass)

	rl_field(TestClass, a);
	rl_field(TestClass, b);
	rl_field(TestClass, c);
};

#define rl_strong_constructable(mp_type) ReflectionLib::Constructable<mp_type, Base::StrongPtr<mp_type> >

template<class t_Type>
class StrogConstructableType
{
public:
	typedef ReflectionLib::Constructable<t_Type, Base::StrongPtr<t_Type> > value;
};

template<class t_Type>
class ArrayOfStrogConstructablesType
{
public:
	typedef ReflectionLib::ArrayOfConstructables< t_Type, Base::StrongPtr<t_Type> > value;
};

template<class t_Type>
class HashArrayOfStrogConstructablesType
{
public:
	typedef ReflectionLib::HashArrayOfConstructables< t_Type, Base::StrongPtr<t_Type> > value;
};

class TestDerivationClass : public Base::Vector3f, public TestClass
{
public:
	rl_class(TestDerivationClass)
	rl_base_class0(TestClass)
	
	rl_field(TestClass, a);
	
	typedef ReflectionLib::Constructable<TestClass, Base::StrongPtr<TestClass> > Type1_t;

	rl_field(TestDerivationClass::Type1_t, obj1);

	rl_field(StrogConstructableType<TestClass>::value, obj1_2);

	rl_field(Base::StrongPtr<TestClass>, strong1);
	rl_field(Base::StrongPtrThreadSafe<TestClass>, strongTS1);
	rl_field(Base::WeakPtr<TestClass>, weak1);

	TestClass* _obj2;
	TestClass* obj2()
	{
		return _obj2;
	}

	void obj2(TestClass* p)
	{
		_obj2 = p;
	}

	rl_ptr_property(TestClass, obj2)

	rl_field(ArrayOfStrogConstructablesType<TestClass>::value, objarr )

	rl_field(HashArrayOfStrogConstructablesType<TestClass>::value, objharr )


	TestDerivationClass()
	{
		_obj2 = 0;
	}

};


class TestClass3
{
public:
	int* a;
	void met(int i, float f){ f = (float)i; }
};


namespace ReflectionLib
{

class MyClassProp
{
public:
	int i;
	void initProperty(void* mem)
	{
		new(mem) TReflection<int>(i);
	}
};

class MyGlobalCreator
{
public:
	MyClassProp* create(const ConstString& base)
	{
		return new MyClassProp();
	}
};

MyGlobalCreator gMyGlobalCreator;

void prefomanceTest();

void serializationTest();

void testReflection()
{
	ReflectionLib::Private::Library::instance().init();

	String cs;
	ReflectionLib::typeOf<TestEnum>().enumPrint(cs, TestEnum_B);
	//TestClass::zrl_ClassIniterFunc();

	TestDerivationClass tc;
	Reflection p = reflectionOf(tc);
	String s;
	p.reflection(_CS("k")).get(s);
	int i = 0;
	float f = 1;
	float fr;
	p.reflection(_CS("met")).call(f);
	p.reflection(_CS("met2")).rcall(fr, f);
	//logInfoSkim("meth = %s", s.c_str());

	p.reflection(_CS("str")).get(s);
	logInfoSkim<<"str = " << s;

	p.reflection(_CS("enm")).get(s);
	logInfoSkim<<"enm = " << s;

	//bool b1 = typeOf<TestClass>() == typeOf<TestClass*>();
	//bool b2 = typeOf<TestClass>() == typeOf<const TestClass>();
	//bool b3 = typeOf<TestClass>() == typeOf<const TestClass*>();
	//bool b4 = typeOf<TestClass>() == typeOf<TestClass&>();

	p.reflection(_CS("intarr")).insert(0);
	p.reflection(_CS("intarr")).reflection(0).set(_CS("314"));
	p.reflection(_CS("intarr")).reflection(0).get(s);
	logInfoSkim<<"intarr[0] = " << s;

	Reflection nullrefl;
	nullrefl.reflection(_CS("str")).get(s);

	//Reflection r = p.construct(_CS("obj1"), ReflectionLib::typeOf<TestClass>());
	//logInfo << r.valid();
	//r = r.reflection(_CS("str"));
	//logInfo << r.valid();
	//r.get(s);
	p.construct(_CS("obj1"), ReflectionLib::typeOf<TestClass>()).reflection(_CS("str")).get(s);


	p.setref(_CS("obj2"), (TestClass*)0);

	p.reflection(_CS("objarr")).insert(0);
	p.reflection(_CS("objarr")).construct(0, ReflectionLib::typeOf<TestClass>());
	p.reflection(_CS("objarr")).reflection(0).reflection(_CS("enm")).get(s);
	logInfoSkim<<"objarr[0].enm = " << s;


	int xxx=0;

	serializationTest();

	/*
	Command cmd;
	PoolAllocator pa;
	float flt;
	Command::make(pa, "cmd", flt);

	Array<const Command* > arr;
	arr.push(0);
	*/

	/*
	{
		Base::String name(TEnumStringMap<TestEnum>::print(TestEnum_B));
		logInfoSkim("TestEnum_B name == %s", name.c_str());
	}

	{
		Reflection prop = xxfun();
	}
	
	{
		logInfoSkim("Test1");
		TestClass tc;
		Reflection prop = initProperty(tc);

		Base::String str;
		logInfoSkim("i = %s", prop.property(_CS("i")).print().c_str());
		prop.property("f").print(str);
		logInfoSkim("f = %s", str.c_str());
		prop.property("s").print(str);
		logInfoSkim("s = %s", str.c_str());

		prop.property("i").parse(_CS("10"));
		prop.property("f").set(314.0f);
		prop.property("s").parse(_CS("xaxaxa"));

		prop.property("i").print(str);
		logInfoSkim("i = %s", str.c_str());
		prop.property("f").print(str);
		logInfoSkim("f = %s", str.c_str());
		prop.property("s").print(str);
		logInfoSkim("s = %s", str.c_str());

		char code[] = "i 0 f 1.1 s \"refl\"";
		constructReflectiveObjectByStringDefinition(prop, code);

		prop.property("i").print(str);
		logInfoSkim("i = %s", str.c_str());
		prop.property("f").print(str);
		logInfoSkim("f = %s", str.c_str());
		prop.property("s").print(str);
		logInfoSkim("s = %s", str.c_str());
	}

	{
		logInfoSkim("Test2");
		TestClass2 tc2;
		Reflection prop = initProperty(tc2);

		logInfoSkim("a.f = %s", prop.property(_CS("a")).property(_CS("f")).print().c_str());
		logInfoSkim("a.i = %s", prop.property(_CS("a")).property(_CS("i")).print().c_str());
		logInfoSkim("a.s = %s", prop.property(_CS("a")).property(_CS("s")).print().c_str());
		logInfoSkim("b.f = %s", prop.property(_CS("b")).property(_CS("f")).print().c_str());
		logInfoSkim("b.i = %s", prop.property(_CS("b")).property(_CS("i")).print().c_str());
		logInfoSkim("b.s = %s", prop.property(_CS("b")).property(_CS("s")).print().c_str());
		logInfoSkim("c.f = %s", prop.property(_CS("c")).property(_CS("f")).print().c_str());
		logInfoSkim("c.i = %s", prop.property(_CS("c")).property(_CS("i")).print().c_str());
		logInfoSkim("c.s = %s", prop.property(_CS("c")).property(_CS("s")).print().c_str());

		Array<Base::byte> data;
		File::load(data, "reflection.txt");
		ReflectionLib::constructReflectiveObjectByStringDefinition(prop, Base::ConstString((char*)data.first(), data.size()));

		logInfoSkim("a.f = %s", prop.property(_CS("a")).property(_CS("f")).print().c_str());
		logInfoSkim("a.i = %s", prop.property(_CS("a")).property(_CS("i")).print().c_str());
		logInfoSkim("a.s = %s", prop.property(_CS("a")).property(_CS("s")).print().c_str());
		logInfoSkim("b.f = %s", prop.property(_CS("b")).property(_CS("f")).print().c_str());
		logInfoSkim("b.i = %s", prop.property(_CS("b")).property(_CS("i")).print().c_str());
		logInfoSkim("b.s = %s", prop.property(_CS("b")).property(_CS("s")).print().c_str());
		logInfoSkim("c.f = %s", prop.property(_CS("c")).property(_CS("f")).print().c_str());
		logInfoSkim("c.i = %s", prop.property(_CS("c")).property(_CS("i")).print().c_str());
		logInfoSkim("c.s = %s", prop.property(_CS("c")).property(_CS("s")).print().c_str());

	}


	

	{
		logInfoSkim("Test3");
	ReflectionLib::DinamicComposition<int> dincomp("myarr", "int");
	ReflectionLib::Reflection p = initProperty(dincomp);
	EConstructResult cr;
	p.construct(cr, _CS("int"), _CS("odin")).set(int(1));
	p.construct(cr, _CS("int"), _CS("dva")).set(int(2));
	p.construct(cr, _CS("int"), _CS("tri")).set(int(3));

	for(int i=0; i < p.size(); ++i)
		logInfoSkim("compos %s %s", p.name(i).c_str(), p.property(i).print().c_str());

	}

	{
		Base::Closure<MyClassProp* (MyGlobalCreator::*)(const ConstString& )> cl = Base::makeClosure(&gMyGlobalCreator, &MyGlobalCreator::create);
		logInfoSkim("Test4");
		DinamicCompositionEx<MyClassProp> dincomp(_CS("myarrex"), Base::makeClosure(&gMyGlobalCreator, &MyGlobalCreator::create));
		Reflection p = initProperty(dincomp);
		EConstructResult cr;
		p.construct(cr, _CS("MyClassProp"), _CS("odin")).set(int(1));
		p.construct(cr, _CS("MyClassProp"), _CS("dva")).set(int(2));
		p.construct(cr, _CS("MyClassProp"), _CS("tri")).set(int(3));

		for(int i=0; i < p.size(); ++i)
			logInfoSkim("compos %s %s", p.name(i).c_str(), p.property(i).print().c_str());

	}

	prefomanceTest();

	*/

	ReflectionLib::Private::Library::instance().deinit();
}







void serializationTest()
{
	TestDerivationClass tc;
	Reflection r = reflectionOf(tc);
	r.deserialize(_CS("k 55 a { k 33 str \"xxxxx\" } obj1_2 !TestClass { k 11 }"));

	String savedStr;
	r.serialize(savedStr);

	FILE* file = fopen("x.txt", "wb");
	fwrite(savedStr.first(), savedStr.size(), 1, file);
	fclose(file);

	TestDerivationClass tc2;

	reflectionOf(tc2).assign(r);
	savedStr.clear();
	reflectionOf(tc2).serialize(savedStr);
	file = fopen("x2.txt", "wb");
	fwrite(savedStr.first(), savedStr.size(), 1, file);
	fclose(file);

	logInfoSkim << "serialized TestDerivationClass:" << savedStr;
}

/*
struct BirthDate
{
	int year;
	int month;
	int day;
};

struct PersonName
{
	String firstName;
	String middleName;
	String lastName;
};

struct Person
{
	uint id;
	PersonName name;
	BirthDate birthDay;
	String birthPlace;
	int age;
	float stature;
	float weight;
	float volume;
	uint socialCardId;
	uint driverLicence;
};

typedef Array<Person> PersonArray;

struct Test
{
	PersonArray personArray;
};

DeclareCompositionClassProperty3(BirthDate, BirthDate, , 0, ,
	day, constructReflection(mem, _obj.day),
	month, constructReflection(mem, _obj.month),
	year, constructReflection(mem, _obj.year))

DeclareCompositionClassProperty3(PersonName, PersonName, , 0, ,
	firstName, constructReflection(mem, _obj.firstName),
	lastName, constructReflection(mem, _obj.lastName),
	middleName, constructReflection(mem, _obj.middleName))

DeclareCompositionClassProperty10(Person, Person, , 0, ,
	age, constructReflection(mem, _obj.age),
	birthDay, constructReflection(mem, _obj.birthDay),
	birthPlace, constructReflection(mem, _obj.birthPlace),
	driverLicence, constructReflection(mem, _obj.driverLicence),
	id, constructReflection(mem, _obj.id),
	name, constructReflection(mem, _obj.name),
	socialCardId, constructReflection(mem, _obj.socialCardId),
	stature, constructReflection(mem, _obj.stature),
	volume, constructReflection(mem, _obj.volume),
	weight, constructReflection(mem, _obj.weight))

void prefomanceTest()
{
	char str[255];
	DinamicArray<Person> personArray("Persons","Person");
	personArray.reserve(100000);

	Timer timer;
	for (uint p = 0; p < 1000; p++)
	{
		personArray.push(Person());
		Reflection personProp = initProperty(personArray[p]);
		personProp.property("id").set(p);

		Reflection nameProp = personProp.property("name");
		sprintf(str, "First Name %d", p);
		nameProp.property("firstName").set(str);
		sprintf(str, "MIddle Name %d", p);
		nameProp.property("middleName").set(str);
		sprintf(str, "Last Name %d", p);
		nameProp.property("lastName").set(str);

		Reflection dateProp = personProp.property("birthDay");
		dateProp.property("year").set(int(1900 + p));
		dateProp.property("month").set(int(p));
		dateProp.property("day").set(int(p * 3));

		sprintf(str, "Birth Place %d", p);
		personProp.property("birthPlace").set(str);

		personProp.property("age").set(int(20 + p));
		personProp.property("stature").set(2.0f + 0.1f * p);
		personProp.property("weight").set(60.0f + 0.5f * p);
		personProp.property("volume").set(100.0f + p);
		personProp.property("socialCardId").set((uint)rand());
		personProp.property("driverLicence").set((uint)rand());
	}

	logInfoSkim("classic test %lf", timer.dt());


	personArray.clear();
	String code;
	File::load(code, "D:/code.txt");
	Array<Token> toks;
	ReflectionLib::lexAnalize(toks, code);
	timer.dt();
	for (uint p = 0; p < 1; p++)
	{
		
		personArray.clear();
		Reflection personProp = initProperty(personArray);
		//ReflectionLib::constructReflectiveObject(personProp, toks);
		ReflectionLib::constructReflectiveObjectByStringDefinition(personProp, code);
	}
	logInfoSkim("parse test %lf", timer.dt());


}

*/





}
