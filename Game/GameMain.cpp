
//----------------------------------------------------------------------------//
// Modules
//----------------------------------------------------------------------------//

#include "Base.cpp"
#include "File.cpp"
#include "Window.cpp"
#include "Graphics.cpp"
#include "Renderer.cpp"
#include "BuiltinData.cpp"
#include "Scene.cpp"

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

class String
{
public:

protected:

	struct Buffer
	{
		uint length = 0;
		uint size = 1;
		int refs = 1;
		//char str[1] = { 0 };
	};

	static Buffer s_null;
};

String::Buffer String::s_null;

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

template <class F> void* FuncPtr(F _func) { union { F f; void* p; }_fp = { _func }; return _fp.p; }
template <class F> F FuncCast(void* _func) { union { void* p; F f; }_fp = { _func }; return _fp.f; }
/*
	template <class F> struct Function;
	template <class R, class... A> struct Function<R(A...)>
	{
		// TODO: calling convention
		typedef R(*Invoke)(void*, void*, A&&...);

		typedef R(*Ptr)(A...);
		typedef R(Type)(A...);

		Invoke invoke;
		void* func;
		void* self;

		Function(void) : invoke(nullptr), func(nullptr), self(nullptr) { }
		Function(R(*_func)(A...)) : invoke(InvokeFunc), func(FuncPtr(_func)), self(nullptr) { }
		template <class C> Function(C* _self, R(C::*_func)(A...)) : invoke(InvokeMethod<C>), func(FuncPtr(_func)), self(_self) { ASSERT(_self != nullptr); }
		template <class C> Function(const C* _self, R(C::*_func)(A...) const) : invoke(InvokeConstMethod<C>), func(FuncPtr(_func)), self(const_cast<C*>(_self)) { ASSERT(_self != nullptr); }
		operator bool(void) const { return func != nullptr; }

		R operator () (A... _args) const
		{
			ASSERT(func != nullptr);
			return invoke(self, func, Forward<A>(_args)...);
		}

		static R InvokeFunc(void* _self, void* _func, A&&... _args)
		{
			typedef R(*Func)(A...);
			return FuncCast<Func>(_func)(Forward<A>(_args)...);
		}

		template <class C> static R InvokeMethod(void* _self, void* _func, A&&... _args)
		{
			ASSERT(_self != nullptr);
			typedef R(C::*Func)(A...);
			return (*((C*)_self).*FuncCast<Func>(_func))(Move(_args)...);
		}

		template <class C> static R InvokeConstMethod(void* _self, void* _func, A&&... _args)
		{
			ASSERT(_self != nullptr);
			typedef R(C::*Func)(A...) const;
			return (*((const C*)_self).*FuncCast<Func>(_func))(Move(_args)...);
		}
	};

	template <class R, class... A> Function<R(A...)> MakeFunction(R(*_func)(A...))
	{
		return Function<R(A...)>(_func);
	}
	template <class C, class R, class... A> Function<R(A...)> MakeFunction(C* _self, R(C::*_func)(A...))
	{
		return Function<R(A...)>(_self, _func);
	}
	template <class C, class R, class... A> Function<R(A...)> MakeFunction(const C* _self, R(C::*_func)(A...) const)
	{
		return Function<R(A...)>(_self, _func);
	}

*/

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

#define PRINT_SIZEOF(T) LOG("sizeof(%s) = %d", #T, sizeof(T))

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//


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
	LOG("Startup ...");
	LOG("Build %s", __DATE__);
	CreateDirectory("Data", nullptr);
	SetCurrentDirectory("Data");
	bool _bdOutOfDate = ExtractBuiltinData();
	if (_bdOutOfDate)
	{
		LOG("Generate resources ...");
	}

	PRINT_SIZEOF(Node);
	LOG("1KK nodes = %d mb", (sizeof(Node) * 1000000) / 1024 / 1024);

	{
		const Color _clearColor(0x7f7f9fff);
		Window _window;
		Graphics _graphics;	  
		Renderer _renderer;
		{
			VertexBuffer _vb(3 * sizeof(SimpleVertex));

			SimpleVertex _vd[3] =  // xyz, tc, argb
			{
				{ Vec3(150.0f,  50.0f, 0.5f), Vec2(0.5f, 1.0f), 0xff0000ff, }, // t
				{ Vec3(250.0f, 250.0f, 0.5f), Vec2(1.0f,0.0f), 0x00ff00ff, },  // r
				{ Vec3(50.0f, 250.0f, 0.5f), Vec2(0.0f, 0.0f), 0x0000ffff, },  // l
			};
			_vb.Write(_vd, 0, sizeof(_vd));

			Mat44 _worldMatrix;
			_worldMatrix.SetIdentity() ;

			Mat44 _viewProjMatrix;

			while (!gWindow->ShouldClose())
			{
				gWindow->PollEvents();

				Vec2i _size = gWindow->Size();
				_viewProjMatrix.CreateOrtho2D(_size.x, _size.y);
				_viewProjMatrix.Inverse().Inverse() ;
				 
				gGraphics->BeginFrame();
				gGraphics->ClearFrameBuffers(FBT_Color, _clearColor);
				gGraphics->SetVertexShader(0);
				gGraphics->SetPixelShader(0);
				gGraphics->SetVertexFormat(VF_Simple);
				gGraphics->SetVertexBuffer(&_vb, 0, sizeof(SimpleVertex));

				gGraphicsDevice->SetVertexShaderConstantF(0, _viewProjMatrix.v, 4);

				gGraphicsDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
				gGraphics->EndFrame();
			}
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

