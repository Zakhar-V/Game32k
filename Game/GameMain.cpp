
//----------------------------------------------------------------------------//
// Modules
//----------------------------------------------------------------------------//

#include "Base.cpp"
#include "File.cpp"
#include "Math.cpp"
#include "Device.cpp"
#include "Graphics.cpp"
#include "BuiltinData.cpp"
#include "Image.cpp"
#include "Geometry.cpp"

#include "Scene.cpp"
#include "Transform.cpp"
#include "Physics.cpp"
#include "Render.cpp"

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



//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

class TestCamera
{
public:

	void Update(float _dt)
	{
		Vec3 _vec = 0;
		if ((GetAsyncKeyState('W') & 0x8000) != 0)
			_vec.z += m_speed.z;
		if ((GetAsyncKeyState('S') & 0x8000) != 0)
			_vec.z -= m_speed.z;

		if ((GetAsyncKeyState('A') & 0x8000) != 0)
			_vec.x += m_speed.x;
		if ((GetAsyncKeyState('D') & 0x8000) != 0)
			_vec.x -= m_speed.x;

		if ((GetAsyncKeyState('Q') & 0x8000) != 0)
			_vec.y += m_speed.y;
		if ((GetAsyncKeyState('E') & 0x8000) != 0)
			_vec.y -= m_speed.y;

		/*if ((GetAsyncKeyState(VK_ADD) & 0x8000) != 0)
			m_fov += RADIANS;
		if ((GetAsyncKeyState(VK_SUBTRACT) & 0x8000) != 0)
			m_fov -= RADIANS;*/

		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
			_vec *= 5;

		_vec = _vec * m_viewMatrix.GetRotation().Inverse();
		m_pos += _vec * _dt;
		//printf("pos (%f %f %f)\n", m_pos.x, m_pos.y, m_pos.z);
		Mat44 _tm;
		_tm.CreateTranslation(m_pos);

		m_rot += gDevice->GetCameraDelta() * m_rspeed * _dt;
		m_rot.y = Clamp<float>(m_rot.y, -90, 90);
		//printf("rot (%f %f)\n", m_rot.x, m_rot.y);

		Quat _rx, _ry, _r;
		_rx.FromAxisAngle(VEC3_UNIT_X, -m_rot.y * RADIANS);
		_ry.FromAxisAngle(VEC3_UNIT_Y, -m_rot.x * RADIANS);
		_r = _ry * _rx;
		Mat44 _rm;
		_rm.CreateRotation(_r);
		m_viewMatrix = (_rm * _tm);

		Vec2 _wndSize = gDevice->WindowSize();
		//
		m_projMatrix.CreatePerspective(m_fov, _wndSize.x / _wndSize.y, 0.01f, 100);
		//printf("fov (%f %f %f)\n", m_fov, m_projMatrix[0][0], m_projMatrix[1][1]);

		m_viewProjMatrix = m_projMatrix * m_viewMatrix;
	}

	float m_fov = PI / 4; // 45
	Vec3 m_speed = 1;
	Vec2 m_rspeed = 1;
	Vec3 m_pos = 0;
	Vec2 m_rot = 0;
	Mat44 m_viewMatrix = MAT44_IDENTITY;
	Mat44 m_projMatrix;
	Mat44 m_viewProjMatrix;
protected:

};

double Time(void)
{
	LARGE_INTEGER _f, _c;
	QueryPerformanceFrequency(&_f);
	QueryPerformanceCounter(&_c);
	return (double)_c.QuadPart / (double)_f.QuadPart;
}



////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

enum : uint
{
	MAX_TERRAIN_LODS = 7, // 64x64, 32x32 ...
	TERRAIN_SECTOR_SIDE_M1 = (1 << (MAX_TERRAIN_LODS - 1)),
	TERRAIN_SECTOR_SIDE = TERRAIN_SECTOR_SIDE_M1 + 1,
	TERRAIN_SECTOR_VERTICES = TERRAIN_SECTOR_SIDE * TERRAIN_SECTOR_SIDE + TERRAIN_SECTOR_SIDE * 4,
};

struct HeightMap
{
	Array<uint16> incides;
	uint lods[MAX_TERRAIN_LODS][2]; // start, count
	uint sectors[2];
	uint numSectors;

};

uint CreateTerrainSectorIndices(uint16* _dst, uint _side)
{
	uint16* _startP = _dst;



	return (uint)(_dst - _startP);
}

void CreateHeightMap(Image* _img, float _scaleY, float _finalSize, float _desiredStep)
{
	// 65x65 33x33 17x17 9x9 5x5 3x3 2x2

	// sectors

	int _s = FirstPow2((int)(_finalSize / _desiredStep)) + 1;
	if (_s < TERRAIN_SECTOR_SIDE)
		_s = TERRAIN_SECTOR_SIDE;

	uint _numSectors = _s / TERRAIN_SECTOR_SIDE_M1;
	uint _numQuads = _numSectors * TERRAIN_SECTOR_SIDE_M1;
	uint _numVertices = _numQuads + 1;
	float _step = _finalSize / _numQuads;
	float _texStep = 1.0f / _numVertices;
	float _length = _step * _numQuads;
	float _startPos = _length * -0.5f;

	for (uint sy = 0; sy < _numSectors; ++sy)
	{
		for (uint sx = 0; sx < _numSectors; ++sx)
		{
			for (uint vy = 0; vy <= TERRAIN_SECTOR_SIDE; ++vy)
			{
				uint y = (sy * TERRAIN_SECTOR_SIDE_M1 + vy);
				for (uint vx = 0; vx <= TERRAIN_SECTOR_SIDE; ++vx)
				{
					uint x = (sx * TERRAIN_SECTOR_SIDE_M1 + vx);
					float tx = _texStep * x;
					float ty = _texStep * y;
					float px = _startPos + x * _step;
					float py = _startPos + y * _step;
					printf("%d %d {%f %f} {%f %f}\n", x, y, px, py, tx, ty);


				}
			}
			system("pause");
		}
	}

	printf("num sectors: %d, step: %f, size: %f\n", _numSectors, _step, _length);

	// indices

}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

void CreateTerrain()
{
}

void CreateTree(int _type)
{
}

void CreateBuilding(int _type)
{
}



////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

class LogicComponent : public Component
{
};

class SpawnZone : public LogicComponent
{
};

class Weapon : public LogicComponent
{

};

class Character : public LogicComponent
{
};

class Player : public Character
{
};


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "opengl32.lib")

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

	PRINT_SIZEOF(Component);
	PRINT_SIZEOF(Entity);
	LOG("sizeof(Transform) = %d (-28) = %d", sizeof(Transform), sizeof(Transform) - 28);
	PRINT_SIZEOF(PhysicsBody);
	PRINT_SIZEOF(PhysicsShape);
	PRINT_SIZEOF(PhysicsJoint);
	LOG("1KK nodes = %d mb", ((sizeof(Entity) + sizeof(Transform) - 28) * 1000000) / 1024 / 1024);

#if defined(_DEBUG) && 0

	{
		system("pause");
		ExitProcess(0);
	}
#endif

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

			ImagePtr _img = new Image;
			FontInfo _fi;
			_img->CreateBitmapFont(_fi, "Courier New TT", 16, 0.5f);

			TexturePtr _tex = new Texture(TT_2D, PF_RGBA8);
			_tex->Realloc(_img->Width(), _img->Height());
			_tex->Write(_img->Format(), _img->Data());
			_tex->GenerateMipmap();
			
			/*SamplerID _sampler = gGraphics->AddSampler({ TF_Trilinear, TW_Clamp, TW_Clamp });

			Texture _tex(TT_2D, PF_RGBA8, TU_Default);
			//ImagePtr _img = CreateBitmapFont(_fi, "Courier New TT", 32, 0.5f);
			//_img->CreateNoize(512, 0);
			//_img->CreatePerlin(512, 0.05f, 0, 777, 4);
			//_tex.SetSize(_img->Width(), _img->Height());
			//_tex.SetData(0, _img->RawData());
			//_tex.GenerateLods();
			_tex.SetImage(_img);


			VertexBuffer _vb(VF_Simple);
			IndexBuffer _ib(IF_UShort);	*/

			{
				//_vb.Realloc(4 * sizeof(SimpleVertex));
				Vec2 _p(-1,-1);
				Vec2 _s(2, 2);
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
				/*float _z = 0;
				SimpleVertex _vd[4] =  // xyz, tc, rgba
				{
					//{ Vec3(_p.x,  _p.y, 0.5f), Vec2(0, 1), 0xff0000ff, }, // lt
					//{ Vec3(_p.x,  _p.y + _s.y, 0.5f), Vec2(0, 0), 0xff0000ff, }, // lb
					//{ Vec3(_p.x + _s.x,  _p.y, 0.5f), Vec2(1, 1), 0xff0000ff, }, // rt
					//{ Vec3(_p.x + _s.x,  _p.y + _s.y, 0.5f), Vec2(1, 0), 0xff0000ff, }, // rb
					
					// argb xyzw
					// rgba
					{ Vec3(_p.x + _s.x,  _p.y, _z), Vec2(1, 0), 0xff0000ff, }, // rt
					{ Vec3(_p.x,  _p.y, _z), Vec2(0, 0), 0xffffffff, }, // lt
					{ Vec3(_p.x + _s.x,  _p.y + _s.y, _z), Vec2(1, 1), 0x00ff00ff, }, // rb
					{ Vec3(_p.x,  _p.y + _s.y, _z), Vec2(0, 1), 0x0000ffff, }, // lb
				};
				_vb.Write(_vd, 0, sizeof(_vd));	

				_ib.Realloc(6 * 2);

				const uint16 _idx[] =
				{
					0, 1, 3, 3, 1, 2,
				};
				_ib.Write(_idx, 0, 12);	*/

			}

			Mat44 _worldMatrix;
			_worldMatrix.CreateTransform({0, 0, -5}, QUAT_IDENTITY);

			//gDevice->SetCursorMode(CM_Camera);


			double _st, _et;
			float _dt = 0;

			TestCamera _camera;

			//SpriteBatch _spriteBatch;
			/*RenderModel _spriteBatch(VF_Sprite, PT_Triangles, BU_Dynamic);
			Sprite _sprite({ 0, 0, -3 }, { 3 }, 1, 0, { 0 }, { 1 }, 0xffffffff);
			Sprite _sprite2({ 250, 250, 0 }, { 300 }, 1, 0, _fi.chars['A'].tc[0], _fi.chars['A'].tc[1], 0xffffffff);
			*/


			//CreateHeightMap(_img, 1, 1000, 1);
			//CreateHeightMap(_img, 1, 1, 1);

			Scene _scene;
			EntityPtr _entity = new Entity;
			Ptr<Transform> _transform = new Transform;
			_entity->AddComponent(_transform);
			_entity->SetScene(&_scene);


			BufferPtr _vb = new Buffer;
			{

				Vertex _v[3];
				_v[0].position.Set(250, 250, 0); // t	250 0
				_v[2].position.Set(0, 250, 0); // r
				_v[1].position.Set(250, 250, 0); // l
				_v[0].size[0] = 300, _v[0].size[1] = 300;
				_v[0].SetTexCoord(_fi.chars['A'].tc[0]);
				_v[0].SetTexCoord2(_fi.chars['A'].tc[1]);
				_vb->Realloc(sizeof(_v), &_v);
			}
			BufferPtr _cameraParams = new Buffer(BU_Dynamic);
			BufferPtr _worldMatrices = new Buffer(BU_Dynamic);
			{
				_cameraParams->Realloc(sizeof(UCamera));
				_worldMatrices->Realloc(sizeof(UInstanceMat));

				gGraphics->SetUniformBuffer(U_Camera, _cameraParams);
				gGraphics->SetUniformBuffer(U_InstanceMat, _worldMatrices);
			}

			bool _quit = false;
			while (!_quit && !gDevice->ShouldClose())
			{
				gDevice->PollEvents();
				_st = Time();

				_quit = GetAsyncKeyState(VK_ESCAPE) != 0;

				//Vec2 _size = gDevice->WindowSize();
				//Mat44 _viewProjMatrix;
				//_viewProjMatrix.CreateOrtho2D(_size.x, _size.y);
				//_viewProjMatrix.CreateOrtho(0, _size.x, _size.y, 0, -1, 1);
				//_viewProjMatrix.CreatePerspective(90 * RADIANS, _size.x / _size.y, 0.01, 100);
				//_viewProjMatrix.Inverse().Inverse();

				_camera.Update(_dt);


				_scene.Update(_dt);

				gGraphics->BeginFrame();
				gGraphics->ClearFrameBuffers(FBT_Color, _clearColor);


				{
					Vec2 _size = gDevice->WindowSize();

					Mat44 _view, _proj, _model;
					_view.SetIdentity();
					_proj.CreateOrtho(0, _size.x, _size.y, 0, -1, 1);
					_model.SetIdentity();

					UCamera _cam;
					_cam.ViewMat = _view;
					_cam.ProjMat = _proj;
					_cam.ViewProjMat = _proj * _view;

					_cameraParams->Write(&_cam, 0, sizeof(_cam));
					_worldMatrices->Write(&_model, 0, sizeof(_model));

					gGraphics->SetShader(VS_Sprite);
					gGraphics->SetShader(GS_Sprite);
					gGraphics->SetShader(FS_NoTexture);

					gGraphics->SetVertexBuffer(_vb);
					gGraphics->Draw(PT_Points, 0, 1, 1);
				}




				/*gGraphics->SetVertexShader(VS_Test);
				gGraphics->SetPixelShader(PS_Test);
				//gGraphics->SetVertexFormat(VF_Simple);

				//gGraphics->SetFloatUniformVS(0, &_viewProjMatrix, 4);
				gGraphics->SetFloatUniformVS(0, &_camera.m_viewProjMatrix, 4);
				gGraphics->SetFloatUniformVS(4, &_worldMatrix, 4);

				gGraphics->SetTexture(0, &_tex);
				gGraphics->SetSampler(0, _sampler);


				{
					//_spri
					SpriteVertex* _v = (SpriteVertex*)_spriteBatch.LockVertices(LM_WriteDiscard, 0, 2 * 4, true, 2 * 6);
					uint16* _i = _spriteBatch.LockIndices(LM_WriteDiscard, 0, 2 * 6, true, 2 * 6);
					Sprite::AddToBatch(_i, 0, _v, &_sprite, 1);
					Sprite::AddToBatch(_i + 6, 4, _v + 4, &_sprite2, 1);

				}

				_spriteBatch.Bind();
				_spriteBatch.Draw(0, 0, 6);

				//gGraphics->SetVertexBuffer(&_vb);
				//gGraphicsDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
				//gGraphics->SetIndexBuffer(&_ib);
				//gGraphicsDevice->SetIndices(_ib.Handle());
				//gGraphicsDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);


				{
					Vec2 _size = gDevice->WindowSize();
					Mat44 _viewProjMatrix, _model;
					_model.SetIdentity();
					//_viewProjMatrix.CreateOrtho2D(_size.x, _size.y);
					_viewProjMatrix.CreateOrtho(0, _size.x, _size.y, 0, -1, 1);

					gGraphics->SetFloatUniformVS(0, &_viewProjMatrix, 4);
					gGraphics->SetFloatUniformVS(4, &_model, 4);

					_spriteBatch.Draw(0, 6, 6);
				} */

				gGraphics->EndFrame();

				_et = Time();
				float _odt = _dt;
				_dt = (float)(_et - _st);
				_dt = _dt * 0.6f + _odt * 0.4f;

				//printf("dt %f\n", _dt);

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

