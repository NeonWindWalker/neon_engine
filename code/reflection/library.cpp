#include "include.h"

namespace ReflectionLib { namespace Private 
{

TempAllocator& CustomAllocatorWrapper::allocator()
{
	static TempAllocator tempAllocator;
	return tempAllocator;
}

Library::Library()
{
	//Base::initBaseLib();
}

Library::~Library()
{
}

void Library::deinit()
{
	typeDictionary.clear();
	typeIdDictionary.clear();
	classDescMap.clear();
	classDescIdMap.clear();
	enumDescMap.clear();
	methodDescMap.clear();
	containerClassDescMap.clear();
	enumRegistrations.clear();
	methodRegistrations.clear();
	classRegistrations.clear();
	baseClassRegistrations.clear();
	childRegistrations.clear();
	classConstructoSementicRegistrations.clear();
	containerClassRegistrations.clear();
}


Library& Library::instance()
{
	static Library gInst;
	return gInst;
}

TypeId Library::getFreeTypeId()
{
	static uint gIndex = 256;
	gIndex ++;
	return TypeId(gIndex);
}

void Library::registerEnumType(InitFunctionType fun)
{
	enumRegistrations.push(fun);
}
void Library::registerMethodType(InitFunctionType fun)
{
	methodRegistrations.push(fun);
}
void Library::registerClass(InitFunctionType fun)
{
	classRegistrations.push(fun);
}
void Library::registerBaseClass(InitFunctionType fun)
{
	baseClassRegistrations.push(fun);
}
void Library::registerChild(InitFunctionType fun)
{
	childRegistrations.push(fun);
}
void Library::registerClassConstructoSementic(InitFunctionType fun)
{
	classConstructoSementicRegistrations.push(fun);
}
void Library::registerContainerClass(InitFunctionType fun)
{
	containerClassRegistrations.push(fun);
}


EnumDesc* Library::giveEnumDesc(boolean& outNew, const ConstString& name)
{
	EnumDescMap::Iterator it = enumDescMap.insert(outNew, name);
	if(outNew){
		it->val = new EnumDesc();
		it->val->name = name;
	}
	return it->val;
}

MethodDesc* Library::giveMethodDesc(boolean& outNew, const ConstString& name)
{
	MethodDescMap::Iterator it = methodDescMap.insert(outNew, name);
	if(outNew){
		it->val = new MethodDesc();
		it->val->name = name;
	}
	return it->val;
}

ClassDesc* Library::giveClassDesc(boolean& outNew, const ConstString& name)
{
	ClassDescDictionary::Iterator it = classDescMap.insert(outNew, name);
	if(outNew){
		it->val = new ClassDesc();
		it->val->name = name;
	}
	return it->val;
}


//no save returned pointer globally
BaseClassDesc* Library::giveBaseClassDesc(boolean& outNew, ClassDesc& owner, const ConstString& name)
{
	ClassDescDictionary::Iterator it = classDescMap.find(owner.name);
	if(it == classDescMap.end())
		return 0;

	ClassDescDictionary::Iterator it3 = classDescMap.find(name);
	if(it == classDescMap.end())
		return 0;

	BaseClassDictionary::Iterator it2 = it->val->baseClasses.insert(outNew, name);
	if(outNew){
		it2->val.push()->desc = it3->val;
	}
	return &it2->val[0];
}

ChildDesc* Library::giveChildDesc(boolean& outNew, ClassDesc& owner, const ConstString& name)
{
	ClassDescDictionary::Iterator it = classDescMap.find(owner.name);
	if(it == classDescMap.end())
		return 0;

	ChildDictionary::Iterator it2 = it->val->childs.insert(outNew, name);
	if(outNew){
		it2->val = new ChildDesc();
		//it2->val->name = name;
	}
	return it2->val;
}

ContainerClassDesc* Library::giveContainerClassDesc(boolean& outNew, const ConstString& name)
{
	ContainerClassDescMap::Iterator it = containerClassDescMap.insert(outNew, name);
	if(outNew){
		it->val = new ContainerClassDesc();
		it->val->name = name;
	}
	return it->val;
}

void Library::init()
{
	for(uint i=0; i < enumRegistrations.size(); ++i)
	{
		(enumRegistrations[i])();
	}

	for(uint i=0; i < classRegistrations.size(); ++i)
	{
		(classRegistrations[i])();
	}

	for(uint i=0; i < methodRegistrations.size(); ++i)
	{
		(methodRegistrations[i])();
	}

	for(uint i=0; i < childRegistrations.size(); ++i)
	{
		(childRegistrations[i])();
	}

	for(uint i=0; i < classConstructoSementicRegistrations.size(); ++i)
	{
		(classConstructoSementicRegistrations[i])();
	}

	for(uint i=0; i < baseClassRegistrations.size(); ++i)
	{
		(baseClassRegistrations[i])();
	}
	uint initedCount = 0;
	while(initedCount != classDescMap.size())
	{
		initedCount = 0;
		for(ClassDescDictionary::Iterator it = classDescMap.first(); it != classDescMap.end(); ++it)
		{
			ClassDesc* desc = it->val;
			if(desc->inited){
				initedCount ++;
				continue;
			}

			boolean needWaitBaseClassInit = false;
			for(uint i=0; i < desc->baseClasses.size(); ++i){
				Assert(desc->baseClasses[i].val.size());
				if(!desc->baseClasses[i].val[0].desc->inited){
					needWaitBaseClassInit = true;
					break;
				}
			}
			if(needWaitBaseClassInit)
				continue;

			BaseClassDictionary baseClassesInitial;
			foreach(e, desc->baseClasses){
				boolean bNew;
				baseClassesInitial.insert(bNew, e->key)->val = e->val;
			}
			desc->trueBaseClassesCount = desc->baseClasses.size();

			foreach(baseClassesKVP, baseClassesInitial)
			{
				foreach(baseClass, baseClassesKVP->val)
				{
					foreach(baseClassBaseClassesKVP, baseClass->desc->baseClasses)
					{
						foreach(baseClassBaseClass, baseClassBaseClassesKVP->val)
						{
							//insert base base class
							boolean bNew;
							BaseClassDictionary::Iterator destArrKVP = desc->baseClasses.insert(bNew, baseClassBaseClass->desc->name);
							BaseClassDesc d = *baseClassBaseClass;
							d.deep ++;
							d.offset += baseClass->offset;
							
							if(destArrKVP->val.find(d) == destArrKVP->val.end())
								destArrKVP->val.push(d);
						}
					}
				}
			}

			foreach(baseClassesKVP, desc->baseClasses)
			{
				boolean bNew;
				desc->baseClassesById.insert(bNew, baseClassesKVP->val[0].type.id().number())->val = baseClassesKVP->val;
			}

			foreach(baseClassesKVP, desc->baseClasses)
			{
				foreach(baseClass, baseClassesKVP->val)
				{
					for(uint nChild = 0; nChild < baseClass->desc->childs.size(); ++nChild){
						ChildDesc* baseChildDesc = baseClass->desc->childs[nChild].val;
						String baseChildName = baseClass->desc->childs[nChild].key;
						boolean bNew;
						ChildDictionary::Iterator it = desc->childs.insert(bNew, baseChildName);
						if(bNew){
							it->val = new ChildDesc(*baseChildDesc);
							ChildDesc* d = it->val;
							d->offset += baseClass->offset;
							d->deep ++;
							d->addresing = baseClass->virtualAddresFunc;
						}
					}
				}
			}

			desc->inited = true;
		}
	}

	for(uint i=0; i < containerClassRegistrations.size(); ++i)
	{
		(containerClassRegistrations[i])();
	}



	for(ClassDescDictionary::Iterator it = classDescMap.first(); it != classDescMap.end(); ++it)
	{
		typeDictionary(it->val->name)->val = it->val->type;
		typeIdDictionary(it->val->id.number())->val = it->val->type;
	}

	for(ContainerClassDescMap::Iterator it = containerClassDescMap.first(); it != containerClassDescMap.end(); ++it)
	{
		typeDictionary(it->val->name)->val = it->val->type;
		typeIdDictionary(it->val->id.number())->val = it->val->type;
	}

	for(MethodDescMap::Iterator it = methodDescMap.first(); it != methodDescMap.end(); ++it)
	{
		typeDictionary(it->val->name)->val = it->val->type;
		typeIdDictionary(it->val->id.number())->val = it->val->type;
	}

	for(EnumDescMap::Iterator it = enumDescMap.first(); it != enumDescMap.end(); ++it)
	{
		typeDictionary(it->val->name)->val = it->val->type;
		typeIdDictionary(it->val->id.number())->val = it->val->type;
	}

}

}}


namespace ReflectionLib
{

Type Type::getType(ConstString name)
{
	Private::Library& lib = Private::Library::instance();
	Private::Library::TypeDictionary::Iterator it = lib.typeDictionary.find(name);
	if(it != lib.typeDictionary.end())
		return it->val;
	return Type();
}

Type Type::getType(TypeId id)
{
	Private::Library& lib = Private::Library::instance();
	Private::Library::TypeIdDictionary::Iterator it = lib.typeIdDictionary.find(id.number());
	if(it != lib.typeIdDictionary.end())
		return it->val;
	return Type();
}

}
