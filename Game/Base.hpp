#pragma once

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

void InitLog(void);
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
		return m_data + m_used - 1;
	}
	const T& Top(void) const
	{
		ASSERT(m_used > 0);
		return m_data + m_used - 1;
	}
	uint Size(void) const
	{
		return m_size;
	}
	Array& Reserve(uint _size)
	{
		_Realloc(_size, false);
		return *this;
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
	Array& Pop(uint _count)
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
		for (T *_p = m_data, *_end = m_data + m_used; _p < _end)
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
		if (m_size > _newSize)
			return;
		if (_quantize)
			_newSize = (_newSize + 1) << 1;
		T* _newData = reinterpret_cast<T*>(new uint8[_newSize]);

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
// NonCopyable
//----------------------------------------------------------------------------//

class NonCopyable
{
public:
	NonCopyable(void) { }
	~NonCopyable(void) { }

private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator = (const NonCopyable&) = delete;
};

//----------------------------------------------------------------------------//
// Singleton
//----------------------------------------------------------------------------//

template <class T> class Singleton : public NonCopyable
{
public:

	Singleton(void)
	{
		ASSERT(s_instance == nullptr);
		s_instance = static_cast<T*>(this);
	}
	~Singleton(void)
	{
		s_instance = nullptr;
	}
	static T* Get(void) { return s_instance; }

protected:
	static T* s_instance;
};

template <class T> T* Singleton<T>::s_instance = nullptr;

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
