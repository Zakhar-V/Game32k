
//----------------------------------------------------------------------------//
// Modules
//----------------------------------------------------------------------------//

#include "Base.cpp"
#include "File.cpp"
#include "Math.cpp"
#include "Device.cpp"
#include "Graphics.cpp"
#include "BuiltinData.cpp"
#include "Scene.cpp"
#include "Image.cpp"

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


//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

class String
{
public:

	//void Append(const char)
	void Append(const char* _str, int _length = -1)
	{

	}

protected:

	static int _Length(const char* _str, int _length)
	{
		return (uint)(_length >= 0 ? _length : strlen(_str ? _str : ""));
	}

	struct Buffer
	{
		Buffer* AddRef(void)
		{
			++refs;
			return this;
		}
		void Release(void)
		{
			if (!--refs)
				delete [] reinterpret_cast<uint8*>(this);
		}

		uint length = 0;
		uint size = 1;
		int refs = 1;
		union
		{
			char ch = 0;
			char str[1];
		};
	};

	Buffer* _New(uint _maxLength)
	{
		if (!_maxLength)
			return s_null.AddRef();

		Buffer* _buff = reinterpret_cast<Buffer*>(new uint8[sizeof(Buffer) + _maxLength]);
		new(_buff) Buffer();
		_buff->size = _maxLength + 1;
		return _buff;
	}

	Buffer* m_buffer;

	static Buffer s_null;
};

String::Buffer String::s_null;

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
		Device _device;
		Graphics _graphics;	  
		{

			/*Texture _tex(TT_2D, PF_RGB8);
			const uint8 _img[2*2*3] =
			{
				0xff, 0x00, 0x00,  0xff, 0xff, 0xff,
				//0x7f7f7fff, 0x7f7f7fff,
				0x00, 0x00, 0x00,  0x00, 0x00, 0x00,
			};
			_tex.SetSize(2, 2, 1, 1);
			_tex.SetData(_img);
			_tex.GenerateLods(); */
			
			SamplerID _sampler = gGraphics->AddSampler({ TF_Trilinear, TW_Clamp, TW_Clamp });

			Texture _tex(TT_2D, PF_RGBA8, TU_Default);
			FontInfo _fi;
			//ImagePtr _img = CreateBitmapFont(_fi, "Courier New TT", 32, 0.5f);
			ImagePtr _img = new Image;
			_img->CreateBitmapFont(_fi, "Courier New TT", 32, 0.5f);
			//_img->CreateNoize(512, 0);
			//_img->CreatePerlin(256, 0.05f, 0, 777, 4);
			//_tex.SetSize(_img->Width(), _img->Height());
			//_tex.SetData(0, _img->RawData());
			//_tex.GenerateLods();
			_tex.SetImage(_img);


			VertexBuffer _vb(4 * sizeof(SimpleVertex));

			{
				Vec2 _p(100, 100);
				Vec2 _s(600, 600);
				/*SimpleVertex _vd[3] =  // xyz, tc, argb
				{
					{ Vec3(150.0f,  50.0f, 0.5f), Vec2(0.5f, 1.0f), 0xff0000ff, }, // t
					{ Vec3(250.0f, 250.0f, 0.5f), Vec2(1.0f,0.0f), 0x00ff00ff, },  // r
					{ Vec3(50.0f, 250.0f, 0.5f), Vec2(0.0f, 0.0f), 0x0000ffff, },  // l
				};
				_vb.Write(_vd, 0, sizeof(_vd));*/	

				/*
				1 0
				3 2
				*/
				SimpleVertex _vd[4] =  // xyz, tc, rgba
				{
					//{ Vec3(_p.x,  _p.y, 0.5f), Vec2(0, 1), 0xff0000ff, }, // lt
					//{ Vec3(_p.x,  _p.y + _s.y, 0.5f), Vec2(0, 0), 0xff0000ff, }, // lb
					//{ Vec3(_p.x + _s.x,  _p.y, 0.5f), Vec2(1, 1), 0xff0000ff, }, // rt
					//{ Vec3(_p.x + _s.x,  _p.y + _s.y, 0.5f), Vec2(1, 0), 0xff0000ff, }, // rb
					
					// argb xyzw
					// rgba
					{ Vec3(_p.x + _s.x,  _p.y, 0.5f), Vec2(1, 0), 0xff0000ff, }, // rt
					{ Vec3(_p.x,  _p.y, 0.5f), Vec2(0, 0), 0xffffffff, }, // lt
					{ Vec3(_p.x + _s.x,  _p.y + _s.y, 0.5f), Vec2(1, 1), 0x00ff00ff, }, // rb
					{ Vec3(_p.x,  _p.y + _s.y, 0.5f), Vec2(0, 1), 0x0000ffff, }, // lb
				};
				_vb.Write(_vd, 0, sizeof(_vd));	

			}

			Mat44 _worldMatrix;
			_worldMatrix.SetIdentity() ;

			Mat44 _viewProjMatrix;

			//gDevice->SetCursorMode(CM_Camera);

			while (!gDevice->ShouldClose())
			{
				gDevice->PollEvents();

				Vec2 _size = gDevice->WindowSize();
				_viewProjMatrix.CreateOrtho2D(_size.x, _size.y);
				//_viewProjMatrix.Inverse().Inverse();
				 
				gGraphics->BeginFrame();
				gGraphics->ClearFrameBuffers(FBT_Color, _clearColor);
				gGraphics->SetVertexShader(VS_Test);
				gGraphics->SetPixelShader(PS_Test);
				gGraphics->SetVertexFormat(VF_Simple);
				gGraphics->SetVertexBuffer(&_vb, 0, sizeof(SimpleVertex));

				gGraphics->SetFloatUniformVS(0, &_viewProjMatrix, 4);

				gGraphics->SetTexture(0, &_tex);
				gGraphics->SetSampler(0, _sampler);


				gGraphicsDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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

