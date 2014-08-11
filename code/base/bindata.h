#pragma once

#include "scalar.h"
#include "smartPointers.h"
#include "memory.h"

namespace Base
{

template<uint t_ReservedSize>
class TBinaryData : public RefCountable, Uncopyable
{
public:
	typedef byte Value_t;
	static const uint ReservedSize_t = t_ReservedSize;

	TBinaryData()
	{
		_first = _reserved;
		_size = 0;
		_allocatedSize = ReservedSize_t;
	}

	~TBinaryData()
	{
		if(_first != _reserved)
			freeMemoryPages(_first);
	}

	INLINE void* first() { return _first; }
	INLINE const void* first()const { return _first; }
	INLINE void* end() { return _first + _size; }
	INLINE const void* end()const { return _first + _size; }
	INLINE uinteger byteSize()const { return _size; }
	INLINE void* at(uinteger n) { return _first + n; }
	INLINE const void* at(uinteger n)const { return _first + n; }

	INLINE void resize(uinteger s)
	{
		if(s <= t_ReservedSize)
		{
			if(_first == _reserved){
				_size = s;
				return;
			}else{
				Assert(s < _size);
				memoryCopy(_reserved, _first, s);
				freeMemoryPages(_first);
				_allocatedSize = ReservedSize_t;
				_first = _reserved;
				_size = s;
				return;
			}
		}
		else
		{
			if(s <= _allocatedSize){
				_size = s;
				return;
			}else{
				Assert(s > _size);
				byte* newmem = allocateMemoryPages(_allocatedSize, max(_allocatedSize * 2, s));
				memoryCopy(newmem, _first, _size);
				if(_first != _reserved)
					freeMemoryPages(_first);
				_first = newmem;
				_size = s;
				return;
			}
		}
	}

	INLINE void clear(){ resize(0); }

	INLINE void set(const void* ptr, uint size)
	{
		resize(size);
		memoryCopy(_first, ptr, size);
	}

private:
	byte _reserved[t_ReservedSize];
	byte* _first;
	uinteger _size;
	uinteger _allocatedSize;
};

class ConstBinaryData
{
public:
	typedef byte Value_t;

	INLINE ConstBinaryData() : _first(0), _size(0) {}
	INLINE ConstBinaryData(const void* srcFirst, const void* srcEnd) : _first((const byte*)srcFirst), _size((const byte*)srcEnd - (const byte*)srcFirst) {}
	INLINE ConstBinaryData(const void* srcFirst, uinteger srcSize) : _first((const byte*)srcFirst), _size(srcSize) {}
	template<class t_Other>
	INLINE ConstBinaryData(const t_Other& other) : _first((const byte*)other.first()), _size(other.byteSize()) {}
	template<class t_Other>
	INLINE ConstBinaryData& operator = (const t_Other& other) { this->_first = (const byte*)other.first(); this->_size = other.byteSize(); return *this; }

	INLINE const void* first()const { return _first; }
	INLINE const void* end()const { return _first + _size; }
	INLINE uinteger byteSize()const { return _size; }
	INLINE const void* at(uinteger n)const { return _first + n; }

private:
	const byte* _first;
	uinteger _size;
};


class BinaryDataProxy
{
public:
	typedef byte Value_t;

	INLINE BinaryDataProxy() : _first(0), _size(0) {}
	INLINE BinaryDataProxy(const void* srcFirst, void* srcEnd) : _first((byte*)srcFirst), _size((byte*)srcEnd - (byte*)srcFirst) {}
	INLINE BinaryDataProxy(const void* srcFirst, uinteger srcSize) : _first((byte*)srcFirst), _size(srcSize) {}
	template<class t_Other>
	INLINE BinaryDataProxy(const t_Other& other) : _first((byte*)other.first()), _size(other.byteSize()) {}
	template<class t_Other>
	INLINE BinaryDataProxy& operator = (const t_Other& other) { this->_first = (byte*)other.first(); this->_size = other.byteSize(); return *this; }

	INLINE void* first()const { return _first; }
	INLINE void* end()const { return _first + _size; }
	INLINE uinteger byteSize()const { return _size; }
	INLINE void* at(uinteger n)const { return _first + n; }

private:
	byte* _first;
	uinteger _size;
};

template<uint t_ReservedSize>
class TBinaryDataAccessor
{
protected:
	uinteger _seek;

public:
	typedef TBinaryData<t_ReservedSize> DataContainer_t;

	DataContainer_t& container;
	
	TBinaryDataAccessor(DataContainer_t& arr) : container(arr) { _seek = 0; }
	TBinaryDataAccessor(const TBinaryDataAccessor& other) : container(other.container) { _seek = other._seek; }
	TBinaryDataAccessor(const TBinaryDataAccessor& other, MoveInMemoryConstructorExtension flag) : container(other.container) { _seek = other._seek; }
	TBinaryDataAccessor& operator = (const TBinaryDataAccessor& other){ container = other.container; _seek = other._seek; return *this; }

	INLINE uinteger seek()const { return _seek; }
	INLINE void seek(uinteger s) { _seek = s; Assert(_seek <= container.byteSize()); }
	INLINE void dseek(integer s) { _seek += s; Assert(_seek <= container.byteSize()); }
	
	template<class t_Value>
	INLINE void read(t_Value& v)
	{
		Assert(_seek + sizeof(t_Value) <= container.byteSize());
		memoryCopy(&v, this->container.at(_seek), sizeof(t_Value));
		_seek += sizeof(t_Value);
	}

	template<class t_Value>
	NOINLINE void write(const t_Value& v)
	{
		if(_seek + sizeof(t_Value) > container.byteSize())
			this->container.resize(_seek + sizeof(t_Value));
		memoryCopy(this->container.at(_seek), &v, sizeof(t_Value));
		_seek += sizeof(t_Value);
	}

	INLINE void read(void* buff, uint size)
	{
		Assert(_seek + size <= this->container.byteSize());
		memoryCopy(buff, this->container.at(_seek), size);
		_seek += size;
	}

	NOINLINE void write(const void* buff, uint size)
	{
		if(_seek + size > this->container.byteSize())
			this->container.resize(_seek + size);
		memoryCopy(this->container.at(_seek), buff, size);
		_seek += size;
	}

	template<class t_Value>
	INLINE TBinaryDataAccessor& operator >> (t_Value& v)
	{
		read(v);
		return *this;
	}

	template<class t_Value>
	INLINE TBinaryDataAccessor& operator << (const t_Value& v)
	{
		write(v);
		return *this;
	}
};

template<uint t_ReservedSize>
class TEndianSafeBinaryDataAccessor
{
protected:
	uinteger _seek;

public:
	typedef TBinaryData<t_ReservedSize> DataContainer_t;

	DataContainer_t& container;

	TEndianSafeBinaryDataAccessor(DataContainer_t& con) : container(con) { this->_seek = 0; }
	TEndianSafeBinaryDataAccessor(const TEndianSafeBinaryDataAccessor& other) : container(other.container) { this->_seek = other._seek; }
	TEndianSafeBinaryDataAccessor(const TEndianSafeBinaryDataAccessor& other, MoveInMemoryConstructorExtension flag) : container(other.container) { this->_seek = other._seek; }
	TEndianSafeBinaryDataAccessor& operator = (const TEndianSafeBinaryDataAccessor& other){ container = other.container; _seek = other._seek; return *this; }

	INLINE uinteger seek()const { return this->_seek; }
	INLINE void seek(uinteger s) { this->_seek = s; Assert(this->_seek <= this->container.byteSize()); }
	INLINE void dseek(integer s) { this->_seek += s; Assert(this->_seek <= this->container.byteSize()); }

	template<class t_Value>
	INLINE void read(t_Value& v)
	{
		Assert(this->_seek + sizeof(t_Value) <= this->container.byteSize());
		t_Value uncasted;
		memoryCopy(&uncasted, this->container.at(_seek), sizeof(t_Value));
		v = nativeValueFromStandardValue(uncasted);
		this->_seek += sizeof(t_Value);
	}

	template<class t_Value>
	NOINLINE void write(const t_Value& v)
	{
		if(this->_seek + sizeof(t_Value) > this->container.byteSize())
			this->container.resize(_seek + sizeof(t_Value));
		t_Value casted = standardValueFromNativeValue(v);
		memoryCopy(this->container.at(_seek), &casted, sizeof(t_Value));
		this->_seek += sizeof(t_Value);
	}

	INLINE void read(void* buff, uint size)
	{
		Assert(this->_seek + size <= this->container.byteSize());
		memoryCopy(buff, this->container.at(_seek), size);
		this->_seek += size;
	}

	NOINLINE void write(const void* buff, uint size)
	{
		if(this->_seek + size > this->container.byteSize())
			this->container.resize(this->_seek + size);
		memoryCopy(this->container.at(_seek), buff, size);
		this->_seek += size;
	}

	template<class t_Value>
	INLINE TEndianSafeBinaryDataAccessor& operator >> (t_Value& v)
	{
		read(v);
		return *this;
	}

	template<class t_Value>
	INLINE TEndianSafeBinaryDataAccessor& operator << (const t_Value& v)
	{
		write(v);
		return *this;
	}
};

typedef TBinaryData<256> BinaryData;
typedef TBinaryDataAccessor<256> BinaryDataAccessor;
typedef TEndianSafeBinaryDataAccessor<256> EndianSafeBinaryDataAccessor;

class BinaryDataProxyAccessor
{
protected:
	uinteger _seek;

public:
	BinaryDataProxy container;

	BinaryDataProxyAccessor(const BinaryDataProxy& arr) : container(arr) { _seek = 0; }
	BinaryDataProxyAccessor(const BinaryDataProxyAccessor& other) : container(other.container) { _seek = other._seek; }
	BinaryDataProxyAccessor(const BinaryDataProxyAccessor& other, MoveInMemoryConstructorExtension flag) : container(other.container) { _seek = other._seek; }
	BinaryDataProxyAccessor& operator = (const BinaryDataProxyAccessor& other){ container = other.container; _seek = other._seek; return *this; }

	INLINE uinteger seek()const { return _seek; }
	INLINE void seek(uinteger s) { _seek = s; Assert(_seek <= container.byteSize()); }
	INLINE void dseek(integer s) { _seek += s; Assert(_seek <= container.byteSize()); }
	INLINE void* seekPtr()const { return (byte*)container.first() + this->_seek; }

	template<class t_Value>
	INLINE void read(t_Value& v)
	{
		Assert(_seek + sizeof(t_Value) <= container.byteSize());
		memoryCopy(&v, this->container.at(_seek), sizeof(t_Value));
		_seek += sizeof(t_Value);
	}

	template<class t_Value>
	INLINE void write(const t_Value& v)
	{
		Assert(_seek + sizeof(t_Value) <= container.byteSize());
		memoryCopy(this->container.at(_seek), &v, sizeof(t_Value));
		_seek += sizeof(t_Value);
	}

	INLINE void read(void* buff, uint size)
	{
		Assert(_seek + size <= this->container.byteSize());
		memcpy(buff, this->container.at(_seek), size);
		_seek += size;
	}

	INLINE void write(const void* buff, uint size)
	{
		Assert(_seek + size <= this->container.byteSize());
		memoryCopy(this->container.at(_seek), buff, size);
		_seek += size;
	}

	template<class t_Value>
	INLINE BinaryDataProxyAccessor& operator >> (t_Value& v)
	{
		read(v);
		return *this;
	}

	template<class t_Value>
	INLINE BinaryDataProxyAccessor& operator << (const t_Value& v)
	{
		write(v);
		return *this;
	}
};

class EndianSafeBinaryDataProxyAccessor
{
protected:
	uinteger _seek;

public:
	BinaryDataProxy container;

	EndianSafeBinaryDataProxyAccessor(const BinaryDataProxy& con) : container(con) { this->_seek = 0; }
	EndianSafeBinaryDataProxyAccessor(const EndianSafeBinaryDataProxyAccessor& other) : container(other.container) { this->_seek = other._seek; }
	EndianSafeBinaryDataProxyAccessor(const EndianSafeBinaryDataProxyAccessor& other, MoveInMemoryConstructorExtension flag) : container(other.container) { this->_seek = other._seek; }
	EndianSafeBinaryDataProxyAccessor& operator = (const EndianSafeBinaryDataProxyAccessor& other){ container = other.container; _seek = other._seek; return *this; }

	INLINE uinteger seek()const { return this->_seek; }
	INLINE void seek(uinteger s) { this->_seek = s; Assert(this->_seek <= this->container.byteSize()); }
	INLINE void dseek(integer s) { this->_seek += s; Assert(this->_seek <= this->container.byteSize()); }
	INLINE void* seekPtr()const { return (byte*)container.first() + this->_seek; }

	template<class t_Value>
	INLINE void read(t_Value& v)
	{
		Assert(this->_seek + sizeof(t_Value) <= this->container.byteSize());
		t_Value uncasted;
		memoryCopy(&uncasted, this->container.at(_seek), sizeof(t_Value));
		v = nativeValueFromStandardValue(uncasted);
		this->_seek += sizeof(t_Value);
	}

	template<class t_Value>
	INLINE void write(const t_Value& v)
	{
		Assert(this->_seek + sizeof(t_Value) <= this->container.byteSize());
		t_Value casted = standardValueFromNativeValue(v);
		memoryCopy(this->container.at(_seek), &casted, sizeof(t_Value));
		this->_seek += sizeof(t_Value);
	}

	INLINE void read(void* buff, uint size)
	{
		Assert(this->_seek + size <= this->container.byteSize());
		memoryCopy(buff, this->container.at(_seek), size);
		this->_seek += size;
	}

	INLINE void write(const void* buff, uint size)
	{
		Assert(this->_seek + size <= this->container.byteSize());
		memoryCopy(this->container.at(_seek), buff, size);
		this->_seek += size;
	}

	template<class t_Value>
	INLINE EndianSafeBinaryDataProxyAccessor& operator >> (t_Value& v)
	{
		read(v);
		return *this;
	}

	template<class t_Value>
	INLINE EndianSafeBinaryDataProxyAccessor& operator << (const t_Value& v)
	{
		write(v);
		return *this;
	}
};



class ConstBinaryDataAccessor
{
protected:
	uinteger _seek;

public:
	ConstBinaryData container;

	ConstBinaryDataAccessor(const ConstBinaryData& arr) : container(arr) { _seek = 0; }
	ConstBinaryDataAccessor(const ConstBinaryDataAccessor& other) : container(other.container) { _seek = other._seek; }
	ConstBinaryDataAccessor(const ConstBinaryDataAccessor& other, MoveInMemoryConstructorExtension flag) : container(other.container) { _seek = other._seek; }
	ConstBinaryDataAccessor& operator = (const ConstBinaryDataAccessor& other){ container = other.container; _seek = other._seek; return *this; }

	INLINE uinteger seek()const { return _seek; }
	INLINE void seek(uinteger s) { _seek = s; Assert(_seek <= container.byteSize()); }
	INLINE void dseek(integer s) { _seek += s; Assert(_seek <= container.byteSize()); }
	INLINE const void* seekPtr()const { return (const byte*)container.first() + this->_seek; }

	template<class t_Value>
	INLINE void read(t_Value& v)
	{
		Assert(_seek + sizeof(t_Value) <= container.byteSize());
		memoryCopy(&v, this->container.at(_seek), sizeof(t_Value));
		_seek += sizeof(t_Value);
	}

	INLINE void read(void* buff, uint size)
	{
		Assert(_seek + size <= this->container.byteSize());
		memcpy(buff, this->container.at(_seek), size);
		_seek += size;
	}


	template<class t_Value>
	INLINE ConstBinaryDataAccessor& operator >> (t_Value& v)
	{
		read(v);
		return *this;
	}
};

class EndianSafeConstBinaryDataAccessor
{
protected:
	uinteger _seek;

public:
	ConstBinaryData container;

	EndianSafeConstBinaryDataAccessor(const ConstBinaryData& con) : container(con) { this->_seek = 0; }
	EndianSafeConstBinaryDataAccessor(const EndianSafeConstBinaryDataAccessor& other) : container(other.container) { this->_seek = other._seek; }
	EndianSafeConstBinaryDataAccessor(const EndianSafeConstBinaryDataAccessor& other, MoveInMemoryConstructorExtension flag) : container(other.container) { this->_seek = other._seek; }
	EndianSafeConstBinaryDataAccessor& operator = (const EndianSafeConstBinaryDataAccessor& other){ container = other.container; _seek = other._seek; return *this; }

	INLINE uinteger seek()const { return this->_seek; }
	INLINE void seek(uinteger s) { this->_seek = s; Assert(this->_seek <= this->container.byteSize()); }
	INLINE void dseek(integer s) { this->_seek += s; Assert(this->_seek <= this->container.byteSize()); }
	INLINE const void* seekPtr()const { return (const byte*)container.first() + this->_seek; }

	template<class t_Value>
	INLINE void read(t_Value& v)
	{
		Assert(this->_seek + sizeof(t_Value) <= this->container.byteSize());
		t_Value uncasted;
		memoryCopy(&uncasted, this->container.at(_seek), sizeof(t_Value));
		v = nativeValueFromStandardValue(uncasted);
		this->_seek += sizeof(t_Value);
	}

	INLINE void read(void* buff, uint size)
	{
		Assert(this->_seek + size <= this->container.byteSize());
		memoryCopy(buff, this->container.at(_seek), size);
		this->_seek += size;
	}

	template<class t_Value>
	INLINE EndianSafeConstBinaryDataAccessor& operator >> (t_Value& v)
	{
		read(v);
		return *this;
	}
};


}
