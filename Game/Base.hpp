#pragma once

#ifdef _MSC_VER
//#	pragma warning(disable : 4996) // The POSIX name
#endif

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <new>

//----------------------------------------------------------------------------//
// Settings
//----------------------------------------------------------------------------//

#ifndef _DEBUG
//#	define _DEBUG
#endif

#define USE_CRT 1
#define USE_LOG 3 // 0 - disabled, 1 - stdio, 2 - file, 3 - file + stdio
#define USE_ASSERT 1 // use ASSERT when _DEBUG defined

//----------------------------------------------------------------------------//
// Types
//----------------------------------------------------------------------------//

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef unsigned int uint;

//----------------------------------------------------------------------------//
// Debug
//----------------------------------------------------------------------------//

#define _ASSERT(cond, ...) (cond) ? ((void)0) : Fatal("Assertion failed: "#cond " (" ##__VA_ARGS__ ")") 
#define _CHECK(cond, err) (cond) ? ((void)0) : Fatal(err) 

#if defined(_DEBUG) && USE_ASSERT
#	define ASSERT(cond, ...) _ASSERT(cond, ##__VA_ARGS__) 
#	define CHECK(cond, ...) _CHECK(cond, ##__VA_ARGS__) 
#	define DLOG(cond, ...) LOG(cond, ##__VA_ARGS__) 
#else
#	define ASSERT(cond, ...) {}
#	define CHECK(cond, ...) {}
#	define DLOG(cond, ...) {}
#endif

#if USE_LOG
#	define LOG(msg, ...) LogMsg(msg, ##__VA_ARGS__)
#else
#	define LOG(msg, ...) {}
#endif

void LogMsg(const char* _msg, ...);
void Fatal(const char* _error);

//----------------------------------------------------------------------------//
// Memory
//----------------------------------------------------------------------------//

#if USE_CRT == 0

#pragma optimize("", off)
void MemSet(void* _dst, int _val, size_t _size)
{
	for (char *d = (char*)_dst, *e = (char*)_dst + _size; d < e;)
		*d++ = (char)_val;
}
void MemCopy(void* _dst, const void* _src, size_t _size)
{
	for (char *d = (char*)_dst, *s = (char*)_src, *e = (char*)_dst + _size; d < e;)
		*d++ = *s++;
}
#pragma optimize("", on)

#ifdef NDEBUG

#define memset MemSet
#define memcpy MemCopy

void* operator new (size_t _size){ return malloc(_size); }
void* operator new[](size_t _size) { return malloc(_size); }
void operator delete(void* _p) { if (_p) free(_p); }
void operator delete[](void* _p) { if (_p) free(_p); }

#endif // NDEBUG

#endif // USE_CRT

//----------------------------------------------------------------------------//
// Hash
//----------------------------------------------------------------------------//

uint Hash(const void* _data, uint _size, uint _hash = 0)
{
	for (uint i = 0; i < _size; ++i)
		_hash = ((const uint8*)_data)[i] + (_hash << 6) + (_hash << 16) - _hash;
	return _hash;
}

uint StrHash(const char* _str, uint _hash = 0)
{
	if (_str)
	{
		while (*_str)
			_hash = *_str++ + (_hash << 6) + (_hash << 16) - _hash;
	}
	return _hash;
}

/*constexpr uint ConstHash(const char* _str, uint _hash = 0)
{
return *_str ? ConstHash(_str + 1, *_str + (_hash << 6) + (_hash << 16) - _hash) : _hash;
} */

//----------------------------------------------------------------------------//
// Rvalue
//----------------------------------------------------------------------------//

template <class T> struct RRef { typedef T Type; };
template <class T> struct RRef<T&> { typedef T Type; };
template <class T> struct RRef<T&&> { typedef T Type; };

template <class T> inline typename T&& Forward(typename RRef<T>::Type& _ref) { return static_cast<T&&>(_ref); }
template <class T> inline typename T&& Forward(typename RRef<T>::Type&& _ref) { return static_cast<T&&>(_ref); }
template <class T> inline typename RRef<T>::Type&& Move(T&& _ref) { return static_cast<RRef<T>::Type&&>(_ref); }

template <typename T> void Swap(T& _a, T& _b)
{
	T _c = Move(_a);
	_a = Move(_b);
	_b = Move(_c);
}

//----------------------------------------------------------------------------//
// Array
//----------------------------------------------------------------------------//

template <class T> class Array
{
public:
	Array(void)
	{
	}
	Array(const Array& _rhs)
	{
		Push(_rhs.m_data, _rhs.m_used, false);
	}
	Array(Array&& _rhs) : m_data(_rhs.m_data), m_used(_rhs.m_used), m_size(_rhs.m_size)
	{
		_rhs.m_data = nullptr;
		_rhs.m_used = 0;
		_rhs.m_size = 0;
	}
	~Array(void)
	{
		Clear();
		delete[] reinterpret_cast<uint8*>(m_data);
	}
	Array(const T* _e, uint _size)
	{
		Push(_e, _size, false);
	}
	/*Array(const T& _e)
	{
		Push(_e);
	}*/
	Array(uint _size)
	{
		Push(T(), _size, false);
	}
	Array& operator = (const Array& _rhs)
	{
		if (m_data != _rhs.m_data)
			Clear().Push(_rhs.m_data, _rhs.m_used, false);
		return *this;
	}
	Array& operator = (Array&& _rhs)
	{
		Swap(m_data, _rhs.m_data);
		Swap(m_used, _rhs.m_used);
		Swap(m_size, _rhs.m_size);
		return *this;
	}
	T& operator [] (uint _index)
	{
		ASSERT(_index < m_used);
		return m_data[_index];
	}
	const T& operator [] (uint _index) const
	{
		ASSERT(_index < m_used);
		return m_data[_index];
	}
	T* operator * (void)
	{
		return m_data;
	}
	const T* operator * (void) const
	{
		return m_data;
	}
	T* Ptr(void)
	{
		return m_data;
	}
	const T* Ptr(void) const
	{
		return m_data;
	}
	T& Top(void)
	{
		ASSERT(m_used > 0);
		return m_data[m_used - 1];
	}
	const T& Top(void) const
	{
		ASSERT(m_used > 0);
		return m_data[m_used - 1];
	}
	uint Size(void) const
	{
		return m_used;
	}
	Array& Reserve(uint _size)
	{
		_Realloc(_size, false);
		return *this;
	}
	T* Upsize(uint _append, bool _quantize = false)
	{
		uint _offset = m_used;
		Resize(m_used + _append, T(), _quantize);
		return m_data + _offset;
	}
	Array& Resize(uint _newSize, const T& _sample = T(), bool _quantize = false)
	{
		if (_newSize > m_used)
			Push(_sample, _newSize - m_used, _quantize);
		else if (_newSize < m_used)
			Pop(m_used - _newSize);
		return *this;
	}
	Array& Clear(void)
	{
		return Pop(m_used);
	}
	Array& Push(const T* _e, uint _count, bool _quantize = true)
	{
		uint _newUsed = m_used + _count;
		_Realloc(_newUsed, _quantize);
		for (T *_dst = m_data + m_used, *_end = m_data + _newUsed; _dst < _end;)
			*_dst++ = *_e++;
		m_used = _newUsed;
		return *this;
	}
	Array& Push(const T& _e, uint _count = 1, bool _quantize = true)
	{
		uint _newUsed = m_used + _count;
		_Realloc(_newUsed, _quantize);
		for (T *_dst = m_data + m_used, *_end = m_data + _newUsed; _dst < _end;)
			*_dst++ = _e;
		m_used = _newUsed;
		return *this;
	}
	Array& Pop(uint _count = 1)
	{
		if (_count > m_used)
			_count = m_used;
		uint _newUsed = m_used - _count;
		for (T *_p = m_data + m_used, *_end = m_data + _newUsed; _p < _end; --_p)
			_p->~T();
		m_used = _newUsed;
		return *this;
	}
	Array& Remove(const T& _e)
	{
		for (T *_p = m_data, *_end = m_data + m_used; _p < _end;)
		{
			if (*_p == _e)
			{
				*_p = Move(*_end);
				_end->~T();
				--m_used;
				break;
			}
		}
		return *this;
	}

protected:
	void _Realloc(uint _newSize, bool _quantize)
	{
		if (_newSize < m_size)
			return;
		if (_quantize)
			_newSize = (_newSize + 1) << 1;
		T* _newData = reinterpret_cast<T*>(new uint8[_newSize * sizeof(T)]);
		for (T *_new = _newData, *_old = m_data, *_end = m_data + m_used; _old < _end; ++_new, ++_old)
		{
			new(reinterpret_cast<void*>(_new)) T(Move(*_old));
			_old->~T();
		}

		delete[] reinterpret_cast<uint8*>(m_data);
		m_data = _newData;
		m_size = _newSize;
	}

	T* m_data = nullptr;
	uint m_used = 0;
	uint m_size = 0;
};

//----------------------------------------------------------------------------//
// List 
//----------------------------------------------------------------------------//

void** _Field(void* _ptr, size_t _offset)
{
	return (void**)((uint8*)_ptr + _offset);
}

void _Link(void* _head, void* _this, void* _node)
{

	/*next = head;
	if(next) next->prev = this;
	head = this;*/

	size_t _offset = (uint8*)_node - (uint8*)_this;
	void** _next = 1 + (void**)_node;
	*_next = *(void**)_head;
	if (*_next)
		_Field(*_next, _offset)[0] = _this;
	*(void**)_head = _this;
}

void _Unlink(void* _head, void* _this, void* _node)
{
	/*
	if (m_next) m_next->m_prev = m_prev;
	if (m_prev) m_prev->m_next = m_next;
	else if(head == this) head = m_next;
	*/

	size_t _offset = (uint8*)_node - (uint8*)_this;
	void** _prev = 0 + (void**)_node;
	void** _next = 1 + (void**)_node;

	if (*_next)
		_Field(*_next, _offset)[0] = *_prev;
	if (*_prev)
		_Field(*_prev, _offset)[1] = *_next;
	else if(*(void**)_head == _this)
		*(void**)_head = *_next;

	*_prev = nullptr;
	*_next = nullptr;
}

template <class T> void Link(T*& _head, T* _this, T*& _node)
{
	_Link(&_head, _this, &_node);
}

template <class T> void Unlink(T*& _head, T* _this, T*& _node)
{
	_Unlink(&_head, _this, &_node);
}

template <class T> struct LinkedListNode
{
	T* prev = nullptr;
	T* next = nullptr;
};

template <class T> void Link(T*& _head, T* _this, LinkedListNode<T>& _node)
{
	_Link(&_head, _this, &_node.prev);
}

template <class T> void Unlink(T*& _head, T* _this, LinkedListNode<T>& _node)
{
	_Unlink(&_head, _this, &_node.prev);
}

//----------------------------------------------------------------------------//
// String
//----------------------------------------------------------------------------//

class String
{
public:
	String(void) : m_buffer(s_null.AddRef()) { }
	String(const char* _str, int _length = -1) : m_buffer(s_null.AddRef()) { Append(_str, _length); }
	String(const char* _str, const char* _end) : String(_str, (int)(_end - _str)) { }
	String(const char _ch) : String(&_ch, 1) { }
	~String(void) { m_buffer->Release(); }

	String& operator = (const String& _str);
	String& operator = (const char* _str) { Clear().Append(_str, -1, false); }
	String& operator = (char _ch) { Clear().Append(&_ch, 1, false); }

	String& operator += (const String& _str) { return Append(_str); }
	String& operator += (const char* _str) { return Append(_str); }
	String& operator += (char _ch) { return Append(_ch); }

	String operator + (const String& _str) { return String(*this).Append(_str); }
	String operator + (const char* _str) { return String(*this).Append(_str); }
	String operator + (char _ch) { return String(*this).Append(_ch); }

	friend String operator + (const char* _lhs, const String& _rhs) { return String(_lhs).Append(_rhs); }
	friend String operator + (char _lhs, const String& _rhs) { return String(_lhs).Append(_rhs); }

	char operator [] (int _index) const { return _index < 0 || _index > (int)m_buffer->length ? 0 : m_buffer->str[_index]; }
	operator const char* (void) const { return m_buffer->str; }

	bool operator == (const char* _rhs) const { return strcmp(m_buffer->str, _rhs ? _rhs : "") == 0; }
	bool operator != (const char* _rhs) const { return strcmp(m_buffer->str, _rhs ? _rhs : "") != 0; }

	uint Length(void) const { return m_buffer->length; }
	const char* CStr(void) const { return m_buffer->str; }

	String& Clear(void);

	String& Append(const String& _str) { return Append(_str.m_buffer->str, _str.m_buffer->length); }
	String& Append(const char* _str, int _length = -1, bool _quantize = true);
	String& Append(char _ch) { return Append(&_ch, 1); }

protected:


	struct Buffer
	{
		Buffer* AddRef(void) { ++refs; return this; }
		void Release(void) { if (!--refs) delete[] reinterpret_cast<uint8*>(this); }

		uint length = 0;
		uint size = 1;
		int refs = 1;
		union
		{
			char ch = 0;
			char str[1];
		};
	};

	static int _Length(const char* _str, int _length);
	static Buffer* _New(uint _maxLength);
	void _Realloc(uint _newSize, bool _quantize);

	Buffer* m_buffer;

	static Buffer s_null;
};

//----------------------------------------------------------------------------//
// Event
//----------------------------------------------------------------------------//

/*struct EventArg
{
	EventArg(void* _p = nullptr) : p(_p) { }
	EventArg(int _i) : i(_i) { }
	EventArg(float _f) : f(_f) { }
	union
	{
		void* p = nullptr;
		int i;
		float f;
	};
};

struct Event
{
	Event(void) { }
	Event(uint _type, Entity* _sender, const EventArg& _arg0 = 0, const EventArg& _arg1 = 0, const EventArg& _arg2 = 0, const EventArg& _arg3 = 0) :
		type(_type), sender(_sender), arg0(_arg0), arg1(_arg1), arg2(_arg2), arg3(_arg3) { }

	uint type = 0;
	Entity* sender = nullptr;
	EventArg arg0;
	EventArg arg1;
	EventArg arg2;
	EventArg arg3;
};*/

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//


