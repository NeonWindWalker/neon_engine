#pragma once

#include "array.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"
#include "matrix.h"

namespace Base 
{

typedef Range<utf8> StringProxy;

template<class t_Value>
class TConstString : public ConstRange<t_Value>
{
public:
	typedef ConstRange<t_Value> Base_t;
	typedef const t_Value Value_t;

	INLINE TConstString() {}
	INLINE TConstString(Value_t* srcFirst) : ConstRange<t_Value>(srcFirst, findNullElementPtr(srcFirst)) {}
	INLINE TConstString(Value_t* srcFirst, Value_t* srcEnd) : ConstRange<t_Value>(srcFirst, srcEnd) {}
	INLINE TConstString(Value_t* srcFirst, uint srcSize) : ConstRange<t_Value>(srcFirst, srcSize) {}
	INLINE TConstString(const ConstRange<t_Value>& other) : ConstRange<t_Value>(other) {}
	INLINE TConstString& operator = (const ConstRange<t_Value>& other) { this->_first = const_cast<t_Value*>(other.first()); this->_size = other.size(); return *this; }
	INLINE TConstString& operator = (Value_t* srcFirst) { this->_first = const_cast<t_Value*>(srcFirst); this->_size = findNullElementPtr(srcFirst) - srcFirst; return *this; }

	template<class t_Comparator>
	INLINE const Value_t* find(const t_Value v, const t_Comparator& cmp)const { return Base::find(this->first(), this->end(), v, cmp); }
	INLINE const Value_t* find(const t_Value v)const { return Base::find(this->first(), this->end(), v); }

	template<class t_Comparator>
	INLINE const Value_t* rfind(const t_Value v, const t_Comparator& cmp)const { return Base::rfind(this->last(), this->begin(), v, cmp); }
	INLINE const Value_t* rfind(const t_Value v)const { return Base::rfind(this->last(), this->begin(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const Value_t* find(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::find(this->first(), this->end(), v.first(), v.end(), cmp); }
	template<class t_OtherValue>
	INLINE const Value_t* find(const ConstRange<t_OtherValue>& v)const { return Base::find(this->first(), this->end(), v.first(), v.end()); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const Value_t* rfind(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::rfind(this->last(), this->begin(), v.last(), v.begin(), cmp); }
	template<class t_OtherValue>
	INLINE const Value_t* rfind(const ConstRange<t_OtherValue>& v)const { return Base::rfind(this->last(), this->begin(), v.last(), v.begin()); }

	INLINE boolean contains(const t_Value v)const { return Base::find(this->first(), this->end(), v) != this->end(); }

	template<class t_OtherValue>
	INLINE boolean contains(const ConstRange<t_OtherValue>& v)const { return Base::find(this->first(), this->end(), v.first(), v.end()) != this->end(); }

	INLINE boolean endsWith(const t_Value v)const
	{
		return this->size() && *this->last() == v;
	}

	INLINE boolean startsWith(const t_Value v)const
	{
		return this->size() && *this->first() == v;
	}

	template<class t_OtherValue>
	INLINE boolean endsWith(const ConstRange<t_OtherValue>& other)const
	{
		return this->size() >= other.size() && !compare(other.first(), other.size(), this->end() - other.size(), other.size());
	}

	template<class t_OtherValue>
	INLINE boolean startsWith(const ConstRange<t_OtherValue>& other)const
	{
		return this->size() >= other.size() && !compare(other.first(), other.size(), this->first(), other.size());
	}
};

typedef TConstString<utf8> ConstString;

template<class t_ElementA, class t_ElementB>
class DefaultComparator<const TConstString<t_ElementA>, const ConstRange<t_ElementB> >
{
public:
	INLINE int operator() (const TConstString<t_ElementA>& a, const ConstRange<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_ElementB>
class DefaultComparator<const ConstRange<t_ElementA>, const TConstString<t_ElementB> >
{
public:
	INLINE int operator() (const ConstRange<t_ElementA>& a, const TConstString<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_ElementB>
class DefaultComparator<const TConstString<t_ElementA>, const Range<t_ElementB> >
{
public:
	INLINE int operator() (const TConstString<t_ElementA>& a, const Range<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_ElementB>
class DefaultComparator<const Range<t_ElementA>, const TConstString<t_ElementB> >
{
public:
	INLINE int operator() (const Range<t_ElementA>& a, const TConstString<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_ElementB>
class DefaultComparator<const TConstString<t_ElementA>, const TConstString<t_ElementB> >
{
public:
	INLINE int operator() (const TConstString<t_ElementA>& a, const TConstString<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};



template<class t_Value, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_ReservedCount = 48>
class TString : public Array<t_Value, t_AllocatorWrapper, t_ReservedCount, true>
{
public:
	typedef utf8* Iterator;
	typedef const utf8* ConstIterator;
	typedef t_Value Value_t;
	typedef t_AllocatorWrapper AllocatorWrapper_t;
	typedef Array<t_Value, t_AllocatorWrapper, t_ReservedCount, true> Base_t;

	INLINE TString() {}
	INLINE TString(typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad) {}
	INLINE TString(const TString& other) : Base_t(*(const Base_t*)&other) {}
	INLINE TString(const TString& other, typename t_AllocatorWrapper::Data_t& ad) : Base_t(other.range(), ad) {}
	INLINE TString(TString& other, MoveInMemoryConstructorExtension flag) : Base_t(*(Base_t*)&other, flag) {}
	INLINE TString(const ConstRange<t_Value>& other) : Base_t(other) {}
	INLINE TString(const ConstRange<t_Value>& other, typename t_AllocatorWrapper::Data_t& ad) : Base_t(other, ad) {}
	INLINE TString(const t_Value* srcFirst, const t_Value* srcEnd) : Base_t(srcFirst, srcEnd) {}
	INLINE TString(const t_Value* srcFirst, const t_Value* srcEnd, typename t_AllocatorWrapper::Data_t& ad) : Base_t(srcFirst, srcEnd, ad) {}
	INLINE TString(const t_Value* srcFirst, uint srcCount) : Base_t(srcFirst, srcCount) {}
	INLINE TString(const t_Value* srcFirst, uint srcCount, typename t_AllocatorWrapper::Data_t& ad)  : Base_t(srcFirst, srcCount, ad) {}
	INLINE TString(Value_t* srcFirst) : Base_t(srcFirst, findNullElementPtr(srcFirst)) {}

	template<class t_Comparator>
	INLINE const Value_t* find(const t_Value v, const t_Comparator& cmp)const { return Base::find(this->first(), this->end(), v, cmp); }
	INLINE const Value_t* find(const t_Value v)const { return Base::find(this->first(), this->end(), v); }

	template<class t_Comparator>
	INLINE const Value_t* rfind(const t_Value v, const t_Comparator& cmp)const { return Base::rfind(this->last(), this->begin(), v, cmp); }
	INLINE const Value_t* rfind(const t_Value v)const { return Base::rfind(this->last(), this->begin(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const Value_t* find(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::find(this->first(), this->end(), v.first(), v.end(), cmp); }
	template<class t_OtherValue>
	INLINE const Value_t* find(const ConstRange<t_OtherValue>& v)const { return Base::find(this->first(), this->end(), v.first(), v.end()); }

	template<class t_OtherValue, class t_Comparator>
	INLINE const Value_t* rfind(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp)const { return Base::rfind(this->last(), this->begin(), v.last(), v.begin(), cmp); }
	template<class t_OtherValue>
	INLINE const Value_t* rfind(const ConstRange<t_OtherValue>& v)const { return Base::rfind(this->last(), this->begin(), v.last(), v.begin()); }

	INLINE boolean contains(const t_Value v)const { return Base::find(this->first(), this->end(), v) != this->end(); }

	template<class t_OtherValue>
	INLINE boolean contains(const ConstRange<t_OtherValue>& v)const { return Base::find(this->first(), this->end(), v.first(), v.end()) != this->end(); }

	INLINE boolean endsWith(const t_Value v)const
	{
		return this->size() && *this->last() == v;
	}

	INLINE boolean startsWith(const t_Value v)const
	{
		return this->size() && *this->first() == v;
	}

	template<class t_OtherValue>
	INLINE boolean endsWith(const ConstRange<t_OtherValue>& other)const
	{
		return this->size() >= other.size() && !compare(other.first(), other.size(), this->end() - other.size(), other.size());
	}

	template<class t_OtherValue>
	INLINE boolean startsWith(const ConstRange<t_OtherValue>& other)const
	{
		return this->size() >= other.size() && !compare(other.first(), other.size(), this->first(), other.size());
	}

	template<class t_Comparator>
	INLINE Value_t* find(const t_Value v, const t_Comparator& cmp) { return Base::find(this->first(), this->end(), v, cmp); }
	INLINE Value_t* find(const t_Value v) { return Base::find(this->first(), this->end(), v); }

	template<class t_Comparator>
	INLINE Value_t* rfind(const t_Value v, const t_Comparator& cmp) { return Base::rfind(this->last(), this->begin(), v, cmp); }
	INLINE Value_t* rfind(const t_Value v) { return Base::rfind(this->last(), this->begin(), v); }

	template<class t_OtherValue, class t_Comparator>
	INLINE Value_t* find(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp) { return Base::find(this->first(), this->end(), v.first(), v.end(), cmp); }
	template<class t_OtherValue>
	INLINE Value_t* find(const ConstRange<t_OtherValue>& v) { return Base::find(this->first(), this->end(), v.first(), v.end()); }

	template<class t_OtherValue, class t_Comparator>
	INLINE Value_t* rfind(const ConstRange<t_OtherValue>& v, const t_Comparator& cmp) { return Base::rfind(this->last(), this->begin(), v.last(), v.begin(), cmp); }
	template<class t_OtherValue>
	INLINE Value_t* rfind(const ConstRange<t_OtherValue>& v) { return Base::rfind(this->last(), this->begin(), v.last(), v.begin()); }


	INLINE TString& operator = (const TString& s)
	{
		Base_t::set(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator = (const ConstRange<t_Value>& s)
	{
		Base_t::set(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator = (const t_Value* pstr)
	{
		Base_t::set(pstr, findNullElementPtr(pstr));
		return *this;
	}

	INLINE  TString& operator << (const t_Value* pstr)
	{
		Base_t::push(pstr, findNullElementPtr(pstr));
		return *this;
	}

	template <class t_OtherValue>
	INLINE TString& operator << (const t_OtherValue& v)
	{
		TString s;
		print(s, v);
		Base_t::push(s.first(), s.size());
		return *this;
	}

	template <class t_OtherValue>
	INLINE TString& operator << (t_OtherValue& v)
	{
		TString s;
		print(s, v);
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (TString& s)
	{
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (const TString& s)
	{
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (Base_t& s)
	{
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (const Base_t& s)
	{
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (TConstString<t_Value>& s)
	{
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (const TConstString<t_Value>& s)
	{
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (const Range<t_Value>& s)
	{
		Base_t::push(s.first(),s.size());
		return *this;
	}

	INLINE TString& operator << (Range<t_Value>& s)
	{
		Base_t::push(s.first(),s.size());
		return *this;
	}

	INLINE TString& operator << (const ConstRange<t_Value>& s)
	{
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (ConstRange<t_Value>& s)
	{
		Base_t::push(s.first(), s.size());
		return *this;
	}

	INLINE TString& operator << (Value_t c)
	{
		Base_t::push(c);
		return *this;
	}

	INLINE const char * c_str()
	{
		Base_t::reserve(Base_t::size() + 1);
		*Base_t::end() = 0;
		return (const char*)Base_t::first();
	}

	INLINE void resizeToCStr()
	{
		if(Base_t::empty())
			return;
		t_Value* e = findNullElementPtr(Base_t::first());
		Assert(e <= Base_t::end());
		Base_t::resize(e - Base_t::first());
	}

	INLINE TString& printTabs(uint n)
	{
		Base_t::reserve(Base_t::size() + n);
		for(uint i=0; i < n; ++i)
			Base_t::fpush(utf8('\t'));
		return *this;
	}
};

template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
static INLINE void move(TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& newObj, TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& oldObj) 
{
	newObj.move(oldObj);
}

template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
static INLINE void swap(TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& newObj, TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& oldObj) 
{
	newObj.swap(oldObj);
}

template<typename t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
static INLINE void construct_destroy(TString<t_Value, t_AllocatorWrapper, t_ReservedCount>* newObj, TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& oldObj) 
{
	new(newObj) TString<t_Value, t_AllocatorWrapper, t_ReservedCount>();
	newObj->move(oldObj);
	oldObj.~TString<t_Value, t_AllocatorWrapper, t_ReservedCount>();
}

template<class t_Value, class t_AllocatorWrapper, uint t_ReservedCount, class t_ElementB>
class DefaultComparator< const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>, const ConstRange<t_ElementB> >
{
public:
	INLINE int operator() (const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& a, const ConstRange<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
class DefaultComparator<const ConstRange<t_ElementA>, const TString<t_Value, t_AllocatorWrapper, t_ReservedCount> >
{
public:
	INLINE int operator() (const ConstRange<t_ElementA>& a, const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_Value, class t_AllocatorWrapper, uint t_ReservedCount, class t_ElementB>
class DefaultComparator< const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>, const Range<t_ElementB> >
{
public:
	INLINE int operator() (const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& a, const Range<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
class DefaultComparator<const Range<t_ElementA>, const TString<t_Value, t_AllocatorWrapper, t_ReservedCount> >
{
public:
	INLINE int operator() (const Range<t_ElementA>& a, const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_Value, class t_AllocatorWrapper, uint t_ReservedCount, class t_ElementB>
class DefaultComparator< const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>, const TConstString<t_ElementB> >
{
public:
	INLINE int operator() (const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& a, const TConstString<t_ElementB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ElementA, class t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
class DefaultComparator<const TConstString<t_ElementA>, const TString<t_Value, t_AllocatorWrapper, t_ReservedCount> >
{
public:
	INLINE int operator() (const TConstString<t_ElementA>& a, const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& b){ return compare(a.first(), a.size(), b.first(), b.size()); }
};

template<class t_ValueA, class t_AllocatorWrapperA, uint t_ReservedCountA, class t_ValueB, class t_AllocatorWrapperB, uint t_ReservedCountB >
class DefaultComparator< const TString<t_ValueA, t_AllocatorWrapperA, t_ReservedCountA >, const TString<t_ValueB, t_AllocatorWrapperB, t_ReservedCountB > >
{
public:
	INLINE int operator() (const TString<t_ValueA, t_AllocatorWrapperA, t_ReservedCountA>& a, const TString<t_ValueB, t_AllocatorWrapperB, t_ReservedCountB>& b)const { return compare(a.first(), a.size(), b.first(), b.size()); }
};


typedef TString<utf8, SystemAllocatorWrapper, 48> String;
typedef TString<utf8, SystemAllocatorWrapper, 96> LongString;

#define _CS(str) ::Base::ConstString((const ::Base::utf8*)str, ::Base::uint(sizeof(str) - 1))

INLINE boolean isSpace(const utf8 c)				{ return c == '\n' || c == '\r' || c == ' ' || c == '\t'; }
INLINE boolean isLetter(const utf8 c)				{ return isalpha(c); }
INLINE boolean isLetterOrDigit(const utf8 c)		{ return isalnum(c); }
INLINE boolean isDigit(const utf8 c)				{ return isdigit(c); }
INLINE boolean isXDigit(const utf8 c)				{ return isxdigit(c); }
INLINE boolean isSlesh(const utf8 c)				{ return c == '/' || c == '\\'; }
INLINE boolean isNewLine(const utf8 c)				{ return c == '\n' || c == '\r'; }
INLINE boolean isUpper(const utf8 c)				{ return isupper(c); }
INLINE boolean isLower(const utf8 c)				{ return islower(c); }
INLINE utf8 toLower(const utf8 c)					{ return tolower(c); }
INLINE utf8 toUpper(const utf8 c)					{ return toupper(c); }

INLINE boolean isSpace(const uint c)				{ return (c < 128) && isSpace((utf8)c); }
INLINE boolean isSlesh(const uint c)				{ return c == '/' || c == '\\'; }
INLINE boolean isNewLine(const uint c)				{ return c == '\n' || c == '\r'; }
INLINE boolean isUpper(const uint c)				{ return c < 128 ? isUpper((utf8)c) : false; }
INLINE boolean isLower(const uint c)				{ return c < 128 ? isLower((utf8)c) : false; }
INLINE uint toLower(const uint c)					{ return c < 128 ? toLower((utf8)c) : c; }
INLINE uint toUpper(const uint c)					{ return c < 128 ? toUpper((utf8)c) : c; }

INLINE StringProxy stringProxy(char* str)
{
	char* e = str;
	while(*e)++e;
	return StringProxy((utf8*)str, (utf8*)e);
}

INLINE ConstString constString(const char* str)
{
	const char* e = str;
	while(*e)++e;
	return ConstString((const utf8*)str, (const utf8*)e);
}

#define DeclareGlobalConstStringNameWrapperClass(name) \
class GlobalConstStringNameWrapper_##name\
{\
public:\
	static INLINE ConstString get() { return _CS(#name); }\
};

template<class t_Value>
String print(const t_Value& v)
{
	String s;
	print(s, v);
	return s;
}

extern void print(String& s, int8 value);
extern void print(String& s, uint8 value);
extern void print(String& s, int16 value);
extern void print(String& s, uint16 value);
extern void print(String& s, int32 value);
extern void print(String& s, uint32 value);
extern void print(String& s, int64 value);
extern void print(String& s, uint64 value);
extern void print(String& s, float value);
extern void print(String& s, double value);

extern String printTabs(uint n);

extern boolean parse(int8& dest, const ConstString& str);
extern boolean parse(uint8& dest, const ConstString& str);
extern boolean parse(int16& dest, const ConstString& str);
extern boolean parse(uint16& dest, const ConstString& str);
extern boolean parse(int32& dest, const ConstString& str);
extern boolean parse(uint32& dest, const ConstString& str);
extern boolean parse(int64& dest, const ConstString& str);
extern boolean parse(uint64& dest, const ConstString& str);
extern boolean parse(float& dest, const ConstString& str);
extern boolean parse(double& dest, const ConstString& str);

extern boolean parse(Vector2i& v, const ConstString& str);
extern boolean parse(Vector3i& v, const ConstString& str);
extern boolean parse(Vector4i& v, const ConstString& str);
extern boolean parse(Vector2f& v, const ConstString& str);
extern boolean parse(Vector3f& v, const ConstString& str);
extern boolean parse(Vector4f& v, const ConstString& str);
extern boolean parse(Vector2d& v, const ConstString& str);
extern boolean parse(Vector3d& v, const ConstString& str);
extern boolean parse(Vector4d& v, const ConstString& str);
extern boolean parse(Quaternionf& v, const ConstString& str);
extern boolean parse(Quaterniond& v, const ConstString& str);
extern boolean parse(Matrix22f& v, const ConstString& str);
extern boolean parse(Matrix23f& v, const ConstString& str);
extern boolean parse(Matrix33f& v, const ConstString& str);
extern boolean parse(Matrix34f& v, const ConstString& str);
extern boolean parse(Matrix44f& v, const ConstString& str);
extern boolean parse(Matrix22d& v, const ConstString& str);
extern boolean parse(Matrix23d& v, const ConstString& str);
extern boolean parse(Matrix33d& v, const ConstString& str);
extern boolean parse(Matrix34d& v, const ConstString& str);
extern boolean parse(Matrix44d& v, const ConstString& str);

extern void print(String& s, const Vector2i& v);
extern void print(String& s, const Vector3i& v);
extern void print(String& s, const Vector4i& v);
extern void print(String& s, const Vector4f& v);
extern void print(String& s, const Vector3f& v);
extern void print(String& s, const Vector2f& v);
extern void print(String& s, const Vector2d& v);
extern void print(String& s, const Vector3d& v);
extern void print(String& s, const Vector4d& v);
extern void print(String& s, const Quaternionf& v);
extern void print(String& s, const Quaterniond& v);
extern void print(String& s, const Matrix22f& m);
extern void print(String& s, const Matrix23f& m);
extern void print(String& s, const Matrix33f& m);
extern void print(String& s, const Matrix34f& m);
extern void print(String& s, const Matrix44f& m);
extern void print(String& s, const Matrix22d& m);
extern void print(String& s, const Matrix23d& m);
extern void print(String& s, const Matrix33d& m);
extern void print(String& s, const Matrix34d& m);
extern void print(String& s, const Matrix44d& m);


class SleshEqualComparator
{
public:
	INLINE int operator() (const utf8& a, const utf8& b)const
	{
		return a != '/' && a != '\\';
	}
};

class SpaceEqualComparator
{
public:
	INLINE int operator() (const utf8& a, const utf8& b)const
	{
		return !isSpace(a);
	}
};

static INLINE boolean isValidNameCharFirst(const utf8 c)
{
	return isLetter(c) || c == '_';
}

static INLINE boolean isValidNameChar(const utf8 c)
{
	return isLetterOrDigit(c) || c == '_';
}

static INLINE boolean isValidValueCharFirst(const utf8 c)
{
	return isDigit(c) || c == '-' || c == '.';
}

static INLINE boolean isValidValueChar(const utf8 c)
{
	return isDigit(c) || c == '-' || c == '.' || c == 'e' || c == 'E';
}

static INLINE ConstString trimUTF8FileHeader(ConstString str)
{
	if(str.size() >= 3)
	{
		if(str[0] == 0xef)
			str = str.afterLeftTrim(1);
		if(str[0] == 0xbb)
			str = str.afterLeftTrim(1);
		if(str[0] == 0xbf)
			str = str.afterLeftTrim(1);
	}
	return str;
}

static INLINE void binDataFromHexStr(void* dest, const ConstString& str) 
{ 
	utf8* d = reinterpret_cast<utf8*>(dest);
	for(const utf8* c = str.first(); c != str.end(); c += 2, ++d)
		*d = utf8(c[0] <= '9' ? c[0] - '0' : c[0] - 'A' + 10) | (utf8(c[1] <= '9' ? c[1] - '0' : c[1] - 'A' + 10)<<4);
}

static INLINE void hexStrFromBinData(utf8* dest, const void* data, uint dataByteSize) 
{
	for(const utf8* pb = (const utf8*)data; pb != (const utf8*)data + dataByteSize; pb++){
		byte low = byte(*pb) & 15;
		byte hi = byte(*pb) >> 4;
		*dest++ = low <= 9 ? '0' + low : 'A' + (low - 10);
		*dest++ = hi <= 9 ? '0' + hi : 'A' + (hi - 10);
	}
}

template<class t_Container>
static INLINE void binDataFromHexStr(t_Container& dest, const ConstString& str) 
{ 
	dest.resize(str.size() / (2 * sizeof(typename t_Container::Value_t)));
	binDataFromHexStr((void*)dest.first(), str);
}

template<class t_String, class t_Container>
static INLINE void hexStrFromBinData(t_String& dest, const t_Container& data) 
{
	dest.resize(data.byteSize() * 2);
	hexStrFromBinData(dest.first(), data.first(), data.byteSize());
}

extern boolean normalizePath(String& dest, const ConstString& basePath, const ConstString& path);

static INLINE void getShortPath(String& dest, const ConstString& fullPath, uint sleshesCount = 0)
{
	const utf8* c = fullPath.last();
	for(; c >= fullPath.first(); c --)
	{
		if(isSlesh(*c))
		{
			if(!sleshesCount)
				break;
			sleshesCount --;
		}
	}
	dest = ConstString(c + 1, fullPath.end());
}


static INLINE boolean isValidName(const ConstString& str)
{
	if(str.empty())
		return false;

	if(!isValidNameCharFirst(*str.first()))
		return false;

	for(const utf8* c = str.first() + 1; c < str.end(); ++c)
		if(!isValidNameChar(*c))
			return false;
	return true;
}

static INLINE boolean isValidValue(const ConstString& str)
{
	if(str.empty())
		return false;

	if(!isDigit(str[0]) && str[0] != utf8('-'))
		return false;

	for(const utf8* c = str.first() + 1; c < str.end(); ++c)
		if(!isDigit(*c) && *c != '.')
			return false;
	return true;
}

static INLINE void checkName(String& out, ConstString str)
{
	out.clear(str.size());
	if(!str.empty())
	{
		out.fpush(isValidNameCharFirst(*str.first()) ? *str.first() : utf8('_'));

		for(const utf8* c = str.first() + 1; c < str.end(); ++c)
			out.fpush(isValidNameChar(*c) ? *c : utf8('_'));
	}
	else
	{
		out = _CS("_");
	}
}


static INLINE void percentEncoding(String& res, ConstString src)
{
	res.reserve(src.size());
	for(const utf8* c = src.first(); c != src.end(); ++c)
	{
		switch(*c)
		{
		case '!':
			res << _CS("%21");
			break;
		case '#':
			res << _CS("%23");
			break;
		case '$':
			res << _CS("%24");
			break;
		case '&':
			res << _CS("%26");
			break;
		case '\'':
			res << _CS("%27");
			break;
		case '(':
			res << _CS("%28");
			break;
		case ')':
			res << _CS("%29");
			break;
		case '*':
			res << _CS("%2A");
			break;
		case '+':
			res << _CS("%2B");
			break;
		case ',':
			res << _CS("%2C");
			break;
		case '/':
			res << _CS("%2F");
			break;
		case ':':
			res << _CS("%3A");
			break;
		case ';':
			res << _CS("%3B");
			break;
		case '=':
			res << _CS("%3D");
			break;
		case '?':
			res << _CS("%3F");
			break;
		case '@':
			res << _CS("%40");
			break;
		case '[':
			res << _CS("%5B");
			break;
		case ']':
			res << _CS("%5D");
			break;
		default:
			res.push(*c);
		}        
	}
}

static INLINE String percentEncoding(ConstString src)
{
	String s;
	percentEncoding(s, src);
	return s;
}

static INLINE void backslashEncoding(String& res, ConstString src)
{
	res.reserve(src.size());
	for(const utf8* c = src.first(); c != src.end(); ++c)
	{
		if(*c == '"')
			res.push(utf8('\\'));

		res.push(*c);
	}
}

static INLINE String backslashEncoding(ConstString src)
{
	String s;
	backslashEncoding(s, src);
	return s;
}

template<>
class DefaultHashGen<const ConstString>
{
public:
	INLINE Hash operator () (const ConstString& str)
	{
		Hash h = 0;
		for(const utf8* c = str.first(); c != str.end(); ++c){
			h += (unsigned char)(*c);
			h -= (h << 13) | (h >> 19);
		}
		return h;
	}
};

template<class t_Value, class t_AllocatorWrapper, uint t_ReservedCount>
class DefaultHashGen<const TString<t_Value, t_AllocatorWrapper, t_ReservedCount> >
{
public:
	INLINE Hash operator () (const TString<t_Value, t_AllocatorWrapper, t_ReservedCount>& str)
	{
		return DefaultHashGen<const ConstString>()(str);
	}
};


class UnicodeReader
{
public:
	ConstString str;

	INLINE UnicodeReader(const ConstString& str)
	{
		this->str = str;
	}

	INLINE boolean empty()const { return str.empty(); }

	INLINE uint pop()
	{
		Assert(str.size() >= 1);
		if(!(str[0] & (1<<7))){
			uint c = str[0];
			str = str.afterLeftTrim(1);
			return c;
		}

		Assert(str.size() >= 2);
		if(!(str[0] & (1<<5))){
			uint c = (uint(str[0] & 0x1f)<<6) | uint(str[1] & 0x3f);
			str = str.afterLeftTrim(2);
			return c;
		}

		Assert(str.size() >= 3);
		if(!(str[0] & (1<<4))){
			uint c = (uint(str[0] & 0x0f)<<12) | (uint(str[1] & 0x3f)<<6) | uint(str[2] & 0x3f);
			str = str.afterLeftTrim(3);
			return c;
		}

		Assert(str.size() >= 4);
		if(!(str[0] & (1<<3))){
			uint c = (uint(str[0] & 0x07)<<18) | (uint(str[1] & 0x3f)<<12) | (uint(str[2] & 0x3f)<<6) | uint(str[3] & 0x3f);
			str = str.afterLeftTrim(4);
			return c;
		}

		Assert(str.size() >= 5);
		if(!(str[0] & (1<<2))){
			uint c = (uint(str[0] & 0x03)<<24) | (uint(str[1] & 0x3f)<<18) | (uint(str[2] & 0x3f)<<12) | (uint(str[3] & 0x3f)<<6) | uint(str[4] & 0x3f);
			str = str.afterLeftTrim(5);
			return c;
		}

		Assert(str.size() >= 6);
		if(!(str[0] & (1<<1))){
			uint c = (uint(str[0] & 0x01)<<30) | (uint(str[1] & 0x3f)<<24) | (uint(str[2] & 0x3f)<<18) | (uint(str[3] & 0x3f)<<12) | (uint(str[4] & 0x3f)<<6) | uint(str[5] & 0x3f);
			str = str.afterLeftTrim(6);
			return c;
		}
		
		Assert(0);
		str = ConstString(str.first(), str.first());
		return 0;
	}

	INLINE uint getFirst()const
	{
		Assert(str.size() >= 1);
		if(!(str[0] & (1<<7))){
			uint c = str[0];
			return c;
		}

		Assert(str.size() >= 2);
		if(!(str[0] & (1<<5))){
			uint c = (uint(str[0] & 0x1f)<<6) | uint(str[1] & 0x3f);
			return c;
		}

		Assert(str.size() >= 3);
		if(!(str[0] & (1<<4))){
			uint c = (uint(str[0] & 0x0f)<<12) | (uint(str[1] & 0x3f)<<6) | uint(str[2] & 0x3f);
			return c;
		}

		Assert(str.size() >= 4);
		if(!(str[0] & (1<<3))){
			uint c = (uint(str[0] & 0x07)<<18) | (uint(str[1] & 0x3f)<<12) | (uint(str[2] & 0x3f)<<6) | uint(str[3] & 0x3f);
			return c;
		}

		Assert(str.size() >= 5);
		if(!(str[0] & (1<<2))){
			uint c = (uint(str[0] & 0x03)<<24) | (uint(str[1] & 0x3f)<<18) | (uint(str[2] & 0x3f)<<12) | (uint(str[3] & 0x3f)<<6) | uint(str[4] & 0x3f);
			return c;
		}

		Assert(str.size() >= 6);
		if(!(str[0] & (1<<1))){
			uint c = (uint(str[0] & 0x01)<<30) | (uint(str[1] & 0x3f)<<24) | (uint(str[2] & 0x3f)<<18) | (uint(str[3] & 0x3f)<<12) | (uint(str[4] & 0x3f)<<6) | uint(str[5] & 0x3f);
			return c;
		}
		Assert(0);
		return 0;
	}
};



/*
#ifdef COMPILLER_SUPPORT_CPP0X
template<typename... Args>
static INLINE void print(String& s, const char * fmt, Args... args)
{
	do
	{
		uint s = this->_capasity - this->_size;
		int c = snprintf(this->_data + this->_size, s, args...);
		if(c < 0)
			return *this;
		if(uint(c) != s){
			this->_size = uint(c);
			return *this;
		}
		this->reserve(this->capasity() * 2);
	}while(true);
}

template<typename... Args>
static INLINE WString print(const wchar * fmt, Args... args)
{
	do
	{
		uint s = this->_capasity - this->_size;
		int c = wsnprintf(this->_data + this->_size, s, args...);
		if(c < 0)
			return *this;
		if(uint(c) != s){
			this->_size = uint(c);
			return *this;
		}
		this->reserve(this->capasity() * 2);
	}while(true);
}
#endif
*/

}

