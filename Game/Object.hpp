#pragma once

#include "Base.hpp"


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
// RefCounted
//----------------------------------------------------------------------------//

class RefCounted : public NonCopyable
{
public:
	virtual ~RefCounted(void)
	{
		ASSERT(m_refCount == 0);
	}
	void AddRef(void) const
	{
		++m_refCount;
	}
	void Release(void) const
	{
		if (!--m_refCount)
			delete this;
	}
	int GetRefCount(void) const { return m_refCount; }

protected:
	mutable int m_refCount = 0;
};

//----------------------------------------------------------------------------//
// WeakReference
//----------------------------------------------------------------------------//

class WeakReference	final : public RefCounted
{
public:

	class Object* GetPtr(void) const { return m_object; }

protected:
	friend class Object;

	WeakReference(Object* _object) :
		m_object(_object)
	{
		AddRef();
	}
	void _Reset(void)
	{
		m_object = nullptr;
		Release();
	}

	Object* m_object;
};

//----------------------------------------------------------------------------//
// Object
//----------------------------------------------------------------------------//

#if CPP < 14
#define STATIC_CLASS_ID() static uint StaticClassID(void) { static const uint _id = StrHash(StaticClassName()); return _id; }
#else
#define STATIC_CLASS_ID() static uint StaticClassID(void) { return ConstStrHash(StaticClassName()); }
#endif

#define OBJECT(Name) \
	static constexpr const char* StaticClassName(void) { return Name; } \
	STATIC_CLASS_ID() \
	const char* ClassName(void) const override { return StaticClassName(); } \
	uint ClassID(void) const override { return StaticClassID(); } \
	bool IsClass(uint _id) const override { return _id == StaticClassID() || __super::IsClass(_id); } \
	template <class T> bool IsClass(void) { return IsClass(T::StaticClassID()); }

class Object : public RefCounted
{
public:

	static constexpr const char* StaticClassName(void) { return "Object"; }
	STATIC_CLASS_ID();
	virtual const char* ClassName(void) const { return StaticClassName(); }
	virtual uint ClassID(void) const { return StaticClassID(); }
	virtual bool IsClass(uint _id) const { return _id == StaticClassID(); }
	bool IsClass(const char* _name) { return IsClass(StrHash(_name)); }
	template <class T> bool IsClass(void) { return IsClass(T::StaticClassID()); }

	~Object(void)
	{
		if (m_weakRef)
			m_weakRef->Release();
	}
	WeakReference* GetRef(void) const
	{
		if (!m_weakRef)
			m_weakRef = new WeakReference(const_cast<Object*>(this));
		return m_weakRef;
	}

protected:

	mutable WeakReference* m_weakRef = nullptr;
};

//----------------------------------------------------------------------------//
// Ptr
//----------------------------------------------------------------------------//

template <class T> class Ptr
{
public:
	Ptr(void) : p(nullptr) { }
	~Ptr(void) { if (p) p->Release(); }
	Ptr(const T* _p) : p(const_cast<T*>(_p)) { if (p) p->AddRef(); }
	Ptr(const Ptr& _p) : Ptr(_p.p) {}
	Ptr& operator = (const T* _p) { if (_p) _p->AddRef(); if (p) p->Release(); p = const_cast<T*>(_p); return *this; }
	Ptr& operator = (const Ptr& _p) { return *this = _p.p; }
	operator T* (void) const { return const_cast<T*>(p); }
	T* operator & (void) const { return const_cast<T*>(p); }
	T* operator -> (void) const { return const_cast<T*>(p); }
	T& operator * (void) const { return *const_cast<T*>(p); }
	template <class X> X* Cast(void) const { return static_cast<X*>(const_cast<T*>(p)); }
private:
	T* p;
};

//----------------------------------------------------------------------------//
// Ref
//----------------------------------------------------------------------------//

template <class T> class Ref
{
public:
	typedef Ptr<WeakReference> WeakRef;

	Ref(void) { }
	~Ref(void) { }
	Ref(const T* _p) : p(_p ? _p->GetRef() : nullptr) { }
	Ref(const Ptr<T>& _p) : p(_p ? _p->GetRef() : nullptr) { }
	Ref(const Ref& _p) : p(_p) { }
	Ref& operator = (const T* _p) { p = _p ? _p->GetRef() : nullptr; return *this; }
	Ref& operator = (const Ptr<T>& _p) { p = _p ? _p->GetRef() : nullptr; return *this; }
	Ref& operator = (const Ref& _p) { p = _p.p; return *this; }
	operator T* (void) { return p ? p->GetPtr() : nullptr; }
	T* operator & (void) const { return static_cast<T*>(p ? p->GetPtr() : nullptr); }
	T* operator -> (void) const { ASSERT(p && p->GetPtr()); return static_cast<T*>(p->GetPtr()); }
	T& operator * (void) const { ASSERT(p && p->GetPtr()); return *static_cast<T*>(p->GetPtr()); }
	template <class X> X* Cast(void) const { return static_cast<X*>(p ? p->GetPtr() : nullptr); }

	bool operator == (const Ref& _rhs) const { return p == _rhs.p; }
	bool operator == (const T* _rhs) const { return p && p->GetPtr() == _rhs.p; }

protected:
	WeakRef p;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
