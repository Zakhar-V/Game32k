
//----------------------------------------------------------------------------//
// Modules
//----------------------------------------------------------------------------//

#include "Base.cpp"
#include "Window.cpp"
#include "Graphics.cpp"
#include "BuiltinData.cpp"

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
// sandbox
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

class RefCounted : public NonCopyable
{
public:
	virtual ~RefCounted(void) { }
	void AddRef(void) const { ++m_refCount; }
	void Release(void) const { if (!--m_refCount) delete this; }

protected:
	mutable int m_refCount = 0;
};
template <class T> class Ptr
{
public:
	Ptr(void) : p(nullptr) { }
	Ptr(const T* _p) : p(const_cast<T*>(_p)) { if (p) p->AddRef(); }
	Ptr(const Ptr& _p) : Ptr(_p.p) {}
	Ptr& operator = (const T* _p) { if (_p) _p->AddRef(); if (p) p->Release(); p = const_cast<T*>(_p); return *this; }
	Ptr& operator = (const Ptr& _p) { return *this = _p.p; }
	operator T* (void) const { return const_cast<T*>(p); }
	T* operator & (void) const { return const_cast<T*>(p); }
	T* operator -> (void) const { return const_cast<T*>(p); }
	T& operator * (void) const { return *const_cast<T*>(p); }

private:
	T* p;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

typedef Array<uint8> DataBuffer;

DataBuffer LoadFile(const char* _name)
{
	FILE* _file = fopen(_name, "rb");
	Array<uint8> _buff;
	if (_file)
	{
		fseek(_file, 0, SEEK_END);
		uint _size = (uint)ftell(_file);
		fseek(_file, 0, SEEK_SET);
		_buff.Resize(_size);
		fread(*_buff, 1, _size, _file);
		fclose(_file);
	}
	else
		LOG("Couldn't open file \"%s\"", _name);
	return Move(_buff);
}

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

void main(void)
{
	InitLog();

	uint _hash = 0;
	for (uint i = 0; i < sizeof(g_builtinData); ++i)
	{
		_hash = g_builtinData[i] + (_hash << 6) + (_hash << 16) - _hash;
	}
	LOG("size = %d, hash = 0x%08x", sizeof(g_builtinData), _hash);

	//DataBuffer _db = LoadFile("GameData.pack");

	{
		const Color _clearColor(0x7f7f9fff);
		Window _window;
		Graphics _graphics;
		while (!gWindow->ShouldClose())
		{
			gWindow->PollEvents();
			gGraphics->BeginFrame();
			gGraphics->ClearFrameBuffers(FBT_Color, _clearColor);
			gGraphics->EndFrame();
		} 
	}

#ifdef DEBUG
	system("pause");
#endif

	/*
	Device _device;

	LoadOpenGL();

	Font _font("Courier New TT", 16, 0.5f);
	while (_device.PollEvents())
	{
		glClearColor(0.98f, 0.98f, 0.98f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_device.Begin2D();
		_font.SetColor(Color(0x00, 0x00, 0xff));
		_font.BeginDraw();
		_font.Draw(Vec2(50, 50), "text\nтекст");
		_font.EndDraw();
		//glRectf(0, 0, 100, 100);
		_device.Present();
	}*/
	ExitProcess(0);
}

