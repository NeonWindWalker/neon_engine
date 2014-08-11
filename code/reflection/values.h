#pragma once

#include "treflection.h"

namespace ReflectionLib
{

template<class t_Type>
class PredefinedTypeID
{
};

template<> class PredefinedTypeID<uint8> { public: static const uint value = 1; };
template<> class PredefinedTypeID<int8> { public: static const uint value = 2; };
template<> class PredefinedTypeID<uint16> { public: static const uint value = 3; };
template<> class PredefinedTypeID<int16> { public: static const uint value = 4; };
template<> class PredefinedTypeID<uint32> { public: static const uint value = 5; };
template<> class PredefinedTypeID<int32> { public: static const uint value = 6; };
template<> class PredefinedTypeID<uint64> { public: static const uint value = 7; };
template<> class PredefinedTypeID<int64> { public: static const uint value = 8; };
template<> class PredefinedTypeID<float> { public: static const uint value = 9; };
template<> class PredefinedTypeID<double> { public: static const uint value = 10; };
template<> class PredefinedTypeID<String> { public: static const uint value = 11; };
template<> class PredefinedTypeID<ConstString> { public: static const uint value = 12; };
template<> class PredefinedTypeID<BinaryData> { public: static const uint value = 13; };
template<> class PredefinedTypeID<ConstBinaryData> { public: static const uint value = 14; };
template<> class PredefinedTypeID<Type> { public: static const uint value = 15; };
template<> class PredefinedTypeID<Reflection> { public: static const uint value = 16; };

template<> class PredefinedTypeID<Vector2i> { public: static const uint value = 17; };
template<> class PredefinedTypeID<Vector3i> { public: static const uint value = 18; };
template<> class PredefinedTypeID<Vector4i> { public: static const uint value = 19; };
template<> class PredefinedTypeID<Vector2f> { public: static const uint value = 20; };
template<> class PredefinedTypeID<Vector3f> { public: static const uint value = 21; };
template<> class PredefinedTypeID<Vector4f> { public: static const uint value = 22; };
template<> class PredefinedTypeID<Vector2d> { public: static const uint value = 23; };
template<> class PredefinedTypeID<Vector3d> { public: static const uint value = 24; };
template<> class PredefinedTypeID<Vector4d> { public: static const uint value = 25; };
template<> class PredefinedTypeID<Quaternionf> { public: static const uint value = 26; };
template<> class PredefinedTypeID<Quaterniond> { public: static const uint value = 27; };
template<> class PredefinedTypeID<Matrix22f> { public: static const uint value = 28; };
template<> class PredefinedTypeID<Matrix23f> { public: static const uint value = 29; };
template<> class PredefinedTypeID<Matrix33f> { public: static const uint value = 30; };
template<> class PredefinedTypeID<Matrix34f> { public: static const uint value = 31; };
template<> class PredefinedTypeID<Matrix44f> { public: static const uint value = 32; };
template<> class PredefinedTypeID<Matrix22d> { public: static const uint value = 33; };
template<> class PredefinedTypeID<Matrix23d> { public: static const uint value = 34; };
template<> class PredefinedTypeID<Matrix33d> { public: static const uint value = 35; };
template<> class PredefinedTypeID<Matrix34d> { public: static const uint value = 36; };
template<> class PredefinedTypeID<Matrix44d> { public: static const uint value = 37; };


template<>
class TType<Reflection> : public DummyType
{
public:
	static INLINE ReflectionLib::EMethaType	methaType()												{ return EMethaType_Value; }
	static INLINE Base::ConstString	name() 											{ return _CS("Reflection"); }
	static INLINE ReflectionLib::TypeId	id() 											{ return ReflectionLib::TypeId(PredefinedTypeID<Reflection>::value); }
	static Base::boolean construct(void* mem, Base::IAllocator& alc, void** args) { return false; }
	template<class t_Refl>
	static INLINE boolean valueGet(ReflectionLib::TypeId id, void* v, t_Refl& refl) 
	{
		if(id.number() == PredefinedTypeID<Reflection>::value)
			return refl.get(v);
		return false;
	}
	template<class t_Refl>
	static INLINE Base::boolean valueSet(t_Refl& refl, ReflectionLib::TypeId id, const void* v)
	{
		if(id.number() == PredefinedTypeID<Reflection>::value)
			return refl.set(v);
		return false;
	}
	static INLINE Base::boolean makeReflection(void* mem, void* ptr);
	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP
};

template<>
class TType<Type> : public DummyType
{
public:
	static INLINE ReflectionLib::EMethaType	methaType()												{ return EMethaType_Value; }\
	static INLINE Base::ConstString	name() 										{ return _CS("Type"); }\
	static INLINE ReflectionLib::TypeId	id() 											{ return ReflectionLib::TypeId(PredefinedTypeID<Type>::value); }\
	static Base::boolean construct(void* mem, Base::IAllocator& alc, void** args) { return false; }
	template<class t_Refl>
	static INLINE boolean valueGet(ReflectionLib::TypeId id, void* v, t_Refl& refl) 
	{
		if(id.number() == PredefinedTypeID<Type>::value)
			return refl.get(v);
		return false;
	}
	template<class t_Refl>
	static INLINE Base::boolean valueSet(t_Refl& refl, ReflectionLib::TypeId id, const void* v)
	{
		if(id.number() == PredefinedTypeID<Type>::value)
			return refl.set(v);

		if(id.number() == PredefinedTypeID<Reflection>::value){
			Type t = ((const Reflection*)v)->type();
			return refl.set(&t);
		}

		return false;
	}
	static INLINE Base::boolean makeReflection(void* mem, void* ptr);
	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP
};

}


#define DeclareValueType_Code0(mp_Type) template<> \
class TType<mp_Type> : public DummyType \
{ \
public: \
	\
	static INLINE ReflectionLib::EMethaType	methaType()												{ return EMethaType_Value; }\
	static INLINE Base::ConstString	name() 										{ return _CS(#mp_Type); }\
	static INLINE ReflectionLib::TypeId	id() 											{ return ReflectionLib::TypeId(PredefinedTypeID<mp_Type>::value); }\
	static Base::boolean construct(void* mem, Base::IAllocator& alc, void** args); \
	template<class t_Refl> \
	static INLINE boolean valueGet(ReflectionLib::TypeId id, void* v, t_Refl& refl) {


#define DeclareValueType_Code1(mp_Type) return false; } \
	template<class t_Refl> \
	static Base::boolean valueSet(t_Refl& refl, ReflectionLib::TypeId id, const void* v){


#define DeclareValueType_Code2() return false; } \
	static INLINE Base::boolean makeReflection(void* mem, void* ptr); \
	EXTENSION_TYPE_INTERFACES_VIRTUAL_WRAP \
};


#define ValueReflectionGetCase(mp_Type, type) case PredefinedTypeID<type>::value: { mp_Type vl; refl.get(&vl); *(type*)v = (type)vl; return true; }
#define ValueReflectionSetCase(mp_Type, type) case PredefinedTypeID<type>::value: { mp_Type vl = (mp_Type)*(const type*)v; refl.set(&vl); return true; }

#define DeclareScalarValueType(mp_Type) \
	DeclareValueType_Code0(mp_Type) \
	switch(id.number()) { \
		ValueReflectionGetCase(mp_Type, int8) \
		ValueReflectionGetCase(mp_Type, uint8) \
		ValueReflectionGetCase(mp_Type, int16) \
		ValueReflectionGetCase(mp_Type, uint16) \
		ValueReflectionGetCase(mp_Type, int32) \
		ValueReflectionGetCase(mp_Type, uint32) \
		ValueReflectionGetCase(mp_Type, int64) \
		ValueReflectionGetCase(mp_Type, uint64) \
		ValueReflectionGetCase(mp_Type, float) \
		ValueReflectionGetCase(mp_Type, double) \
		case PredefinedTypeID<String>::value: { mp_Type vl; refl.get(&vl); print(*(String*)v, vl); return true;  } \
	} \
	DeclareValueType_Code1(mp_Type) \
	switch(id.number()) { \
		ValueReflectionSetCase(mp_Type, int8) \
		ValueReflectionSetCase(mp_Type, uint8) \
		ValueReflectionSetCase(mp_Type, int16) \
		ValueReflectionSetCase(mp_Type, uint16) \
		ValueReflectionSetCase(mp_Type, int32) \
		ValueReflectionSetCase(mp_Type, uint32) \
		ValueReflectionSetCase(mp_Type, int64) \
		ValueReflectionSetCase(mp_Type, uint64) \
		ValueReflectionSetCase(mp_Type, float) \
		ValueReflectionSetCase(mp_Type, double) \
		case PredefinedTypeID<String>::value: { mp_Type vl; if(!parse(vl, *(const String*)v))return false; return refl.set(&vl); } \
		case PredefinedTypeID<ConstString>::value: { mp_Type vl; if(!parse(vl, *(const ConstString*)v))return false; return refl.set(&vl);  } \
	} \
	DeclareValueType_Code2();

#define DeclareVectorScalarValueType(mp_Type) \
	DeclareValueType_Code0(mp_Type) \
	if(id.number() == PredefinedTypeID<mp_Type>::value) { refl.get(v); return true; } \
	else if(id.number() == PredefinedTypeID<String>::value) { mp_Type vl; refl.get(&vl); print(*(String*)v, vl); return true; } \
	DeclareValueType_Code1(mp_Type) \
	if(id.number() == PredefinedTypeID<mp_Type>::value) { refl.set(v); return true; } \
	else if(id.number() == PredefinedTypeID<String>::value) { mp_Type vl; if(!parse(vl, *(const String*)v))return false; return refl.set(&vl); } \
	else if(id.number() == PredefinedTypeID<ConstString>::value) { mp_Type vl; if(!parse(vl, *(const ConstString*)v))return false; return refl.set(&vl); } \
	DeclareValueType_Code2();

namespace ReflectionLib
{
	DeclareScalarValueType(uint8);
	DeclareScalarValueType(int8);
	DeclareScalarValueType(uint16);
	DeclareScalarValueType(int16);
	DeclareScalarValueType(uint32);
	DeclareScalarValueType(int32);
	DeclareScalarValueType(uint64);
	DeclareScalarValueType(int64);
	DeclareScalarValueType(float);
	DeclareScalarValueType(double);

	DeclareVectorScalarValueType(Vector2i);
	DeclareVectorScalarValueType(Vector3i);
	DeclareVectorScalarValueType(Vector4i);
	DeclareVectorScalarValueType(Vector2f);
	DeclareVectorScalarValueType(Vector3f);
	DeclareVectorScalarValueType(Vector4f);
	DeclareVectorScalarValueType(Vector2d);
	DeclareVectorScalarValueType(Vector3d);
	DeclareVectorScalarValueType(Vector4d);
	DeclareVectorScalarValueType(Quaternionf);
	DeclareVectorScalarValueType(Quaterniond);
	DeclareVectorScalarValueType(Matrix22f);
	DeclareVectorScalarValueType(Matrix23f);
	DeclareVectorScalarValueType(Matrix33f);
	DeclareVectorScalarValueType(Matrix34f);
	DeclareVectorScalarValueType(Matrix44f);
	DeclareVectorScalarValueType(Matrix22d);
	DeclareVectorScalarValueType(Matrix23d);
	DeclareVectorScalarValueType(Matrix33d);
	DeclareVectorScalarValueType(Matrix34d);
	DeclareVectorScalarValueType(Matrix44d);

	DeclareValueType_Code0(String)
	switch(id.number()) 
	{
		case PredefinedTypeID<int8>::value:		{ String vl; refl.get(&vl);	return parse(*(int8*)v, vl); }
		case PredefinedTypeID<uint8>::value:		{ String vl; refl.get(&vl);	return parse(*(uint8*)v, vl); }
		case PredefinedTypeID<int16>::value:		{ String vl; refl.get(&vl);	return parse(*(int16*)v, vl); }
		case PredefinedTypeID<uint16>::value:	{ String vl; refl.get(&vl);	return parse(*(uint16*)v, vl); }
		case PredefinedTypeID<int32>::value:		{ String vl; refl.get(&vl);	return parse(*(int32*)v, vl); }
		case PredefinedTypeID<uint32>::value:	{ String vl; refl.get(&vl);	return parse(*(uint32*)v, vl); }
		case PredefinedTypeID<int64>::value:		{ String vl; refl.get(&vl);	return parse(*(int64*)v, vl); }
		case PredefinedTypeID<uint64>::value:	{ String vl; refl.get(&vl);	return parse(*(uint64*)v, vl); }
		case PredefinedTypeID<float>::value:		{ String vl; refl.get(&vl);	return parse(*(float*)v, vl); }
		case PredefinedTypeID<double>::value:	{ String vl; refl.get(&vl);	return parse(*(double*)v, vl); }
		case PredefinedTypeID<Vector2i>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector2i*)v, vl); }
		case PredefinedTypeID<Vector3i>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector3i*)v, vl); }
		case PredefinedTypeID<Vector4i>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector4i*)v, vl); }
		case PredefinedTypeID<Vector2f>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector2f*)v, vl); }
		case PredefinedTypeID<Vector3f>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector3f*)v, vl); }
		case PredefinedTypeID<Vector4f>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector4f*)v, vl); }
		case PredefinedTypeID<Vector2d>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector2d*)v, vl); }
		case PredefinedTypeID<Vector3d>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector3d*)v, vl); }
		case PredefinedTypeID<Vector4d>::value:	{ String vl; refl.get(&vl);	return parse(*(Vector4d*)v, vl); }
		case PredefinedTypeID<Quaternionf>::value:	{ String vl; refl.get(&vl);	return parse(*(Quaternionf*)v, vl); }
		case PredefinedTypeID<Quaterniond>::value:	{ String vl; refl.get(&vl);	return parse(*(Quaterniond*)v, vl); }
		case PredefinedTypeID<Matrix22f>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix22f*)v, vl); }
		case PredefinedTypeID<Matrix23f>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix23f*)v, vl); }
		case PredefinedTypeID<Matrix33f>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix33f*)v, vl); }
		case PredefinedTypeID<Matrix34f>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix34f*)v, vl); }
		case PredefinedTypeID<Matrix44f>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix44f*)v, vl); }
		case PredefinedTypeID<Matrix22d>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix22d*)v, vl); }
		case PredefinedTypeID<Matrix23d>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix23d*)v, vl); }
		case PredefinedTypeID<Matrix33d>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix33d*)v, vl); }
		case PredefinedTypeID<Matrix34d>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix34d*)v, vl); }
		case PredefinedTypeID<Matrix44d>::value:	{ String vl; refl.get(&vl);	return parse(*(Matrix44d*)v, vl); }
		case PredefinedTypeID<String>::value:		{ return refl.get(v); }
		case PredefinedTypeID<BinaryData>::value:	{ String vl; refl.get(&vl); binDataFromHexStr(*(BinaryData*)v, vl); return true; }
	}
	DeclareValueType_Code1(String)
	switch(id.number()) 
	{
	case PredefinedTypeID<int8>::value:			{ String s; print(s, *(const int8*)v); return refl.set(&s); }
		case PredefinedTypeID<uint8>::value:	{ String s; print(s, *(const uint8*)v); return refl.set(&s); }
		case PredefinedTypeID<int16>::value:	{ String s; print(s, *(const int16*)v); return refl.set(&s); }
		case PredefinedTypeID<uint16>::value:	{ String s; print(s, *(const uint16*)v); return refl.set(&s); }
		case PredefinedTypeID<int32>::value:	{ String s; print(s, *(const int32*)v); return refl.set(&s); }
		case PredefinedTypeID<uint32>::value:	{ String s; print(s, *(const uint32*)v); return refl.set(&s); }
		case PredefinedTypeID<int64>::value:	{ String s; print(s, *(const int64*)v); return refl.set(&s); }
		case PredefinedTypeID<uint64>::value:	{ String s; print(s, *(const uint64*)v); return refl.set(&s); }
		case PredefinedTypeID<float>::value:	{ String s; print(s, *(const float*)v); return refl.set(&s); }
		case PredefinedTypeID<double>::value:	{ String s; print(s, *(const double*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector2i>::value:	{ String s; print(s, *(const Vector2i*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector3i>::value:	{ String s; print(s, *(const Vector3i*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector4i>::value:	{ String s; print(s, *(const Vector4i*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector2f>::value:	{ String s; print(s, *(const Vector2f*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector3f>::value:	{ String s; print(s, *(const Vector3f*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector4f>::value:	{ String s; print(s, *(const Vector4f*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector2d>::value:	{ String s; print(s, *(const Vector2d*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector3d>::value:	{ String s; print(s, *(const Vector3d*)v); return refl.set(&s); }
		case PredefinedTypeID<Vector4d>::value:	{ String s; print(s, *(const Vector4d*)v); return refl.set(&s); }
		case PredefinedTypeID<Quaternionf>::value: { String s; print(s, *(const Quaternionf*)v); return refl.set(&s); }
		case PredefinedTypeID<Quaterniond>::value: { String s; print(s, *(const Quaterniond*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix22f>::value:	{ String s; print(s, *(const Matrix22f*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix23f>::value:	{ String s; print(s, *(const Matrix23f*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix33f>::value:	{ String s; print(s, *(const Matrix33f*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix34f>::value:	{ String s; print(s, *(const Matrix34f*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix44f>::value:	{ String s; print(s, *(const Matrix44f*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix22d>::value:	{ String s; print(s, *(const Matrix22d*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix23d>::value:	{ String s; print(s, *(const Matrix23d*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix33d>::value:	{ String s; print(s, *(const Matrix33d*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix34d>::value:	{ String s; print(s, *(const Matrix34d*)v); return refl.set(&s); }
		case PredefinedTypeID<Matrix44d>::value:	{ String s; print(s, *(const Matrix44d*)v); return refl.set(&s); }
		case PredefinedTypeID<String>::value:		return refl.set(v);
		case PredefinedTypeID<ConstString>::value:	{ String s = *(const ConstString*)v; return refl.set(&s); }
		case PredefinedTypeID<BinaryData>::value:	{ String s; hexStrFromBinData(s, *(const BinaryData*)v); return refl.set(&s); }
		case PredefinedTypeID<ConstBinaryData>::value:	{ String s; hexStrFromBinData(s, *(const ConstBinaryData*)v); return refl.set(&s); }
	}
	DeclareValueType_Code2();


	DeclareValueType_Code0(ConstString)
	switch(id.number()) 
	{
		case PredefinedTypeID<int8>::value:		{ ConstString vl; refl.get(&vl);	return parse(*(int8*)v, vl); }
		case PredefinedTypeID<uint8>::value:		{ ConstString vl; refl.get(&vl);	return parse(*(uint8*)v, vl); }
		case PredefinedTypeID<int16>::value:		{ ConstString vl; refl.get(&vl);	return parse(*(int16*)v, vl); }
		case PredefinedTypeID<uint16>::value:	{ ConstString vl; refl.get(&vl);	return parse(*(uint16*)v, vl); }
		case PredefinedTypeID<int32>::value:		{ ConstString vl; refl.get(&vl);	return parse(*(int32*)v, vl); }
		case PredefinedTypeID<uint32>::value:	{ ConstString vl; refl.get(&vl);	return parse(*(uint32*)v, vl); }
		case PredefinedTypeID<int64>::value:		{ ConstString vl; refl.get(&vl);	return parse(*(int64*)v, vl); }
		case PredefinedTypeID<uint64>::value:	{ ConstString vl; refl.get(&vl);	return parse(*(uint64*)v, vl); }
		case PredefinedTypeID<float>::value:		{ ConstString vl; refl.get(&vl);	return parse(*(float*)v, vl); }
		case PredefinedTypeID<double>::value:	{ ConstString vl; refl.get(&vl);	return parse(*(double*)v, vl); }
		case PredefinedTypeID<String>::value:	{ ConstString vl; refl.get(&vl); *(String*)v = vl; return true; }
		case PredefinedTypeID<ConstString>::value: return refl.get(v);
		case PredefinedTypeID<BinaryData>::value:	{ ConstString vl; refl.get(&vl); binDataFromHexStr(*(BinaryData*)v, vl); return true; }
	}
	DeclareValueType_Code1(ConstString)
	switch(id.number()) 
	{
		case PredefinedTypeID<ConstString>::value: return refl.set(v);
	}
	DeclareValueType_Code2();



	DeclareValueType_Code0(BinaryData)
		switch(id.number()) 
	{
		case PredefinedTypeID<String>::value:		{ BinaryData vl; refl.get(&vl); hexStrFromBinData(*(String*)v, vl); return true; }
		case PredefinedTypeID<BinaryData>::value:	{ return refl.get(v);  }
	}
	DeclareValueType_Code1(BinaryData)
		switch(id.number()) 
	{
		case PredefinedTypeID<String>::value:		{ BinaryData bd; binDataFromHexStr(bd, *(const String*)v); return refl.set(&bd); }
		case PredefinedTypeID<ConstString>::value:	{ BinaryData bd; binDataFromHexStr(bd, *(const ConstString*)v); return refl.set(&bd); }
		case PredefinedTypeID<BinaryData>::value:	return refl.set(v);
		case PredefinedTypeID<ConstBinaryData>::value:	{ BinaryData bd; bd.set(((const ConstBinaryData*)v)->first(), ((const ConstBinaryData*)v)->byteSize()); return refl.set(&bd); }
	}
	DeclareValueType_Code2();

	DeclareValueType_Code0(ConstBinaryData)
		switch(id.number()) 
	{
		case PredefinedTypeID<String>::value:		{ ConstBinaryData vl; refl.get(&vl); hexStrFromBinData(*(String*)v, vl); return true; }
		case PredefinedTypeID<BinaryData>::value:	{ ConstBinaryData vl; refl.get(&vl); ((BinaryData*)v)->set(vl.first(), vl.byteSize()); return true; }
		case PredefinedTypeID<ConstBinaryData>::value:	{ return refl.get(v);  }
	}
	DeclareValueType_Code1(ConstBinaryData)
		switch(id.number()) 
	{
		case PredefinedTypeID<ConstBinaryData>::value:	return refl.set(v);
	}
	DeclareValueType_Code2();


}



#define DeclareValueReflection(mp_Type) \
template<> \
class TReflection<mp_Type> : public IReflection \
{public: \
	mp_Type& obj; \
	TReflection(mp_Type& val) : obj(val)			{ static_assert(sizeof(*this) <= ReflectionLib::Reflection::gcMaxReflectionImplimentationSize, "big TReflection"); } \
	REFLECTION_DECLARE_TYPE_INTERFACES(TType<mp_Type>) \
	virtual boolean	valid()							{ return &obj != 0; }\
	virtual void*	objectPtr()						{ return &obj; }\
	virtual	boolean destruct() { delete &obj; return true; } \
	virtual boolean get(void* v) { *(mp_Type*)v = obj;  return true; } \
	virtual boolean set(const void* v) { obj = *(const mp_Type*)v;  return true; } \
	virtual boolean get(TypeId id, void* v) { return TType<mp_Type>::valueGet(id, v, *this); } \
	virtual boolean set(TypeId id, const void* v) { return TType<mp_Type>::valueSet(*this, id, v); } \
};


namespace ReflectionLib
{

DeclareValueReflection(uint8);
DeclareValueReflection(int8);
DeclareValueReflection(uint16);
DeclareValueReflection(int16);
DeclareValueReflection(uint32);
DeclareValueReflection(int32);
DeclareValueReflection(uint64);
DeclareValueReflection(int64);
DeclareValueReflection(float);
DeclareValueReflection(double);
DeclareValueReflection(String);
DeclareValueReflection(ConstString);
//DeclareValueReflection(BinaryData);
DeclareValueReflection(ConstBinaryData);
DeclareValueReflection(Type);
DeclareValueReflection(Reflection);

DeclareValueReflection(Vector2i);
DeclareValueReflection(Vector3i);
DeclareValueReflection(Vector4i);
DeclareValueReflection(Vector2f);
DeclareValueReflection(Vector3f);
DeclareValueReflection(Vector4f);
DeclareValueReflection(Vector2d);
DeclareValueReflection(Vector3d);
DeclareValueReflection(Vector4d);
DeclareValueReflection(Quaternionf);
DeclareValueReflection(Quaterniond);
DeclareValueReflection(Matrix22f);
DeclareValueReflection(Matrix23f);
DeclareValueReflection(Matrix33f);
DeclareValueReflection(Matrix34f);
DeclareValueReflection(Matrix44f);
DeclareValueReflection(Matrix22d);
DeclareValueReflection(Matrix23d);
DeclareValueReflection(Matrix33d);
DeclareValueReflection(Matrix34d);
DeclareValueReflection(Matrix44d);

template<>
class TReflection<BinaryData> : public IReflection
{public:
BinaryData& obj;
TReflection(BinaryData& val) : obj(val)			{ static_assert(sizeof(*this) <= ReflectionLib::Reflection::gcMaxReflectionImplimentationSize, "big TReflection"); }
REFLECTION_DECLARE_TYPE_INTERFACES(TType<BinaryData>)
virtual boolean	valid()							{ return &obj != 0; }
virtual void*	objectPtr()						{ return &obj; }
virtual	boolean destruct() { delete &obj; return true; }
virtual boolean get(void* v) { BinaryData* vv = (BinaryData*)v; vv->set(obj.first(), obj.byteSize()); return true; }
virtual boolean set(const void* v) { const BinaryData* vv = (const BinaryData*)v; obj.set(vv->first(), vv->byteSize()); return true; }
virtual boolean get(TypeId id, void* v) { return TType<BinaryData>::valueGet(id, v, *this); }
virtual boolean set(TypeId id, const void* v) { return TType<BinaryData>::valueSet(*this, id, v); }
};

}


#define DeclareValueReflectionConstrucMethod(mp_Type) INLINE Base::boolean TType<mp_Type>::construct(void* mem, Base::IAllocator& alc, void** args) { mp_Type* p = new(alc) mp_Type(); new (mem) ReflectionLib::TReflection<mp_Type>(*p); return true; }

namespace ReflectionLib
{
	DeclareValueReflectionConstrucMethod(int8)
	DeclareValueReflectionConstrucMethod(uint8)
	DeclareValueReflectionConstrucMethod(int16)
	DeclareValueReflectionConstrucMethod(uint16)
	DeclareValueReflectionConstrucMethod(int32)
	DeclareValueReflectionConstrucMethod(uint32)
	DeclareValueReflectionConstrucMethod(int64)
	DeclareValueReflectionConstrucMethod(uint64)
	DeclareValueReflectionConstrucMethod(float)
	DeclareValueReflectionConstrucMethod(double)
	DeclareValueReflectionConstrucMethod(Vector2i);
	DeclareValueReflectionConstrucMethod(Vector3i);
	DeclareValueReflectionConstrucMethod(Vector4i);
	DeclareValueReflectionConstrucMethod(Vector2f);
	DeclareValueReflectionConstrucMethod(Vector3f);
	DeclareValueReflectionConstrucMethod(Vector4f);
	DeclareValueReflectionConstrucMethod(Vector2d);
	DeclareValueReflectionConstrucMethod(Vector3d);
	DeclareValueReflectionConstrucMethod(Vector4d);
	DeclareValueReflectionConstrucMethod(Quaternionf);
	DeclareValueReflectionConstrucMethod(Quaterniond);
	DeclareValueReflectionConstrucMethod(Matrix22f);
	DeclareValueReflectionConstrucMethod(Matrix23f);
	DeclareValueReflectionConstrucMethod(Matrix33f);
	DeclareValueReflectionConstrucMethod(Matrix34f);
	DeclareValueReflectionConstrucMethod(Matrix44f);
	DeclareValueReflectionConstrucMethod(Matrix22d);
	DeclareValueReflectionConstrucMethod(Matrix23d);
	DeclareValueReflectionConstrucMethod(Matrix33d);
	DeclareValueReflectionConstrucMethod(Matrix34d);
	DeclareValueReflectionConstrucMethod(Matrix44d);
	DeclareValueReflectionConstrucMethod(Base::String)
	DeclareValueReflectionConstrucMethod(Base::ConstString)
	DeclareValueReflectionConstrucMethod(Base::BinaryData)
	DeclareValueReflectionConstrucMethod(Base::ConstBinaryData)
}


#define DeclareTypeMakeReflectionMethod(mp_Type)  \
INLINE Base::boolean TType<mp_Type>::makeReflection(void* mem, void* ptr) \
{ \
	if(ptr){ \
		new(mem) TReflection<mp_Type>(*(mp_Type*)ptr); \
		return true; \
	} \
	return false; \
}

namespace ReflectionLib
{
	DeclareTypeMakeReflectionMethod(uint8);
	DeclareTypeMakeReflectionMethod(int8);
	DeclareTypeMakeReflectionMethod(uint16);
	DeclareTypeMakeReflectionMethod(int16);
	DeclareTypeMakeReflectionMethod(uint32);
	DeclareTypeMakeReflectionMethod(int32);
	DeclareTypeMakeReflectionMethod(uint64);
	DeclareTypeMakeReflectionMethod(int64);
	DeclareTypeMakeReflectionMethod(float);
	DeclareTypeMakeReflectionMethod(double);
	DeclareTypeMakeReflectionMethod(Vector2i);
	DeclareTypeMakeReflectionMethod(Vector3i);
	DeclareTypeMakeReflectionMethod(Vector4i);
	DeclareTypeMakeReflectionMethod(Vector2f);
	DeclareTypeMakeReflectionMethod(Vector3f);
	DeclareTypeMakeReflectionMethod(Vector4f);
	DeclareTypeMakeReflectionMethod(Vector2d);
	DeclareTypeMakeReflectionMethod(Vector3d);
	DeclareTypeMakeReflectionMethod(Vector4d);
	DeclareTypeMakeReflectionMethod(Quaternionf);
	DeclareTypeMakeReflectionMethod(Quaterniond);
	DeclareTypeMakeReflectionMethod(Matrix22f);
	DeclareTypeMakeReflectionMethod(Matrix23f);
	DeclareTypeMakeReflectionMethod(Matrix33f);
	DeclareTypeMakeReflectionMethod(Matrix34f);
	DeclareTypeMakeReflectionMethod(Matrix44f);
	DeclareTypeMakeReflectionMethod(Matrix22d);
	DeclareTypeMakeReflectionMethod(Matrix23d);
	DeclareTypeMakeReflectionMethod(Matrix33d);
	DeclareTypeMakeReflectionMethod(Matrix34d);
	DeclareTypeMakeReflectionMethod(Matrix44d);
	DeclareTypeMakeReflectionMethod(String);
	DeclareTypeMakeReflectionMethod(ConstString);
	DeclareTypeMakeReflectionMethod(BinaryData);
	DeclareTypeMakeReflectionMethod(ConstBinaryData);
	DeclareTypeMakeReflectionMethod(Type);
	DeclareTypeMakeReflectionMethod(Reflection);
}
