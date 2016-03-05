
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
#include "Physics.cpp"
#include "Render.cpp"
#include "Level.cpp"

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

/*class GameMap
{

};

class SpawnPoint : public Entity
{

};

class Zone : public Entity
{

};

class Character : public Entity
{
	
};

class Player : public Entity
{

};	*/



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

double Time(void) // in seconds
{
	LARGE_INTEGER _f, _c;
	QueryPerformanceFrequency(&_f);
	QueryPerformanceCounter(&_c);
	return (double)_c.QuadPart / (double)_f.QuadPart;
}
double TimeCounter(void) // in seconds
{
	LARGE_INTEGER _c;
	QueryPerformanceCounter(&_c);
	return (double)_c.QuadPart;
}
double TimeFreq(void) // in seconds
{
	LARGE_INTEGER _f;
	QueryPerformanceFrequency(&_f);
	return 1 / (double)_f.QuadPart;
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


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

struct Settings_t
{
	float invCameraH = 1;
	float invCameraV = 1;
	float mouseSens = 1.5;
}
gSettings;


class FirstPersonCameraController : public Behavior
{
public:
	OBJECT("FirstPersonCameraController");

	float m_yaw = 0;
	float m_pitch = 0;

	FirstPersonCameraController(void)
	{

	}

	~FirstPersonCameraController(void)
	{

	}
	void Update(const FrameInfo& _frame) override
	{
		Quat _r = m_node->GetLocalRotation();
		Vec2 _rot = gDevice->GetCameraDelta() * Vec2(gSettings.invCameraH, gSettings.invCameraV) * gSettings.mouseSens * _frame.time;
		if (_rot.x || _rot.y)
		{
			m_pitch = Clamp<float>(m_pitch + _rot.y, -90, 90);
			m_yaw += _rot.x;

			Quat _rx, _ry;
			_rx.FromAxisAngle(VEC3_UNIT_X, m_pitch * RADIANS);
			_ry.FromAxisAngle(VEC3_UNIT_Y, m_yaw * RADIANS);

			Vec3 _dir = -VEC3_UNIT_Z * _r;

			m_node->SetLocalRotation(_rx * _ry);
		}
	}

protected:
};


Terrain* _terrain;
Vec3* _cubePos;

class PlayerController : public Behavior
{
public:
	OBJECT("PlayerController");

	void Update(const FrameInfo& _frame) override
	{
		Vec3 _vec = 0;
		if ((GetAsyncKeyState('W') & 0x8000) != 0)
			_vec.z -= 1;
		if ((GetAsyncKeyState('S') & 0x8000) != 0)
			_vec.z += 1;

		if ((GetAsyncKeyState('A') & 0x8000) != 0)
			_vec.x -= 1;
		if ((GetAsyncKeyState('D') & 0x8000) != 0)
			_vec.x += 1;

		if ((GetAsyncKeyState('Q') & 0x8000) != 0)
			_vec.y -= 1;
		if ((GetAsyncKeyState('E') & 0x8000) != 0)
			_vec.y += 1;

		_vec *= 25000.f / 3600.f; // 25 km/h
		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
			_vec *= 10;
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0)
			_vec *= 10;

		Vec3 _dir = _vec * m_node->GetChild()->GetWorldRotation().Copy().Inverse(); // camera direction

		Vec3 _pos = m_node->GetWorldPosition();
		_pos += _dir * _frame.time;
		_pos.y = Max<float>(_pos.y, _terrain->GetHeight(_pos.x, _pos.z) + 2);
		//_pos.y = _terrain->GetHeight(_pos.x, _pos.z) + 1.7f;
		m_node->SetWorldPosition(_pos);
		*_cubePos = _pos + Vec3(0, 0, -50) * m_node->GetChild()->GetWorldRotation().Copy().Inverse();
		_cubePos->y = _terrain->GetHeight(_cubePos->x, _cubePos->z);

		//printf("%f %f %f\n", _pos.x, _pos.y, _pos.z);
	}
																						 
protected:
};


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

CameraPtr CreateCamera(Scene* _scene)
{
	Ptr<Camera> _camera = new Camera;
	Ptr<FirstPersonCameraController> _controller = new FirstPersonCameraController;
	_camera->AddBehavior(_controller);
	_camera->SetScene(_scene);
	_scene->SetActiveCamera(_camera);

	return _camera;
}

void CreatePlayer(Scene* _scene)
{
	NodePtr _player = new Node;
	Ptr<PlayerController> _controller = new PlayerController;

	_player->AddBehavior(_controller);
	_player->SetScene(_scene);
	//_controller->Activate();

	CameraPtr _camera = CreateCamera(_scene);
	_camera->SetParent(_player);
	_camera->SetFar(2500);
}

void CreatScene(Scene* _scene)
{
	CreatePlayer(_scene);

	Mat44 _tm;
	GeometryPtr _geom = new Geometry();
	_geom->CreateSphere(1, 16, 8);
	_tm.CreateScale({ 1, 1.8f, 1 });
	_geom->Transform(_tm);
	//_geom->ComputeNormals();

	MeshPtr _bodyMesh = new Mesh;
	_bodyMesh->CreateFromGeometry(_geom);

	_geom->CreateCube(0.75f);
	_geom->ComputeNormals();
	MeshPtr _eyeMesh = new Mesh;
	_eyeMesh->CreateFromGeometry(_geom);
	//_tm.CreateScale({ 1, 2, 1 });
	//_geom->Transform(_tm);

	
	_geom->CreateCube({ 3.0f, 0.75f, 0.5f }, true);
	_geom->ComputeNormals();
	MeshPtr _shoulderMesh = new Mesh;
	_shoulderMesh->CreateFromGeometry(_geom);

	_geom->CreateCube({ 1.0f, 1.9f, 0.75f }, true);
	_geom->ComputeNormals();
	MeshPtr _engineMesh = new Mesh;
	_engineMesh->CreateFromGeometry(_geom);


	int _rseed = 0;
	for (uint i = 0; i < 1000; ++i)
	{
		StaticModelPtr _body = new StaticModel;
		_body->SetSleepingThreshold(1);
		_body->SetMesh(_bodyMesh);
		_body->SetLocalPosition( Vec3((Rand(_rseed) * 2 - 1) * 500, 300 + (Rand(_rseed) * 2 - 1) * 100, (Rand(_rseed) * 2 - 1) * 500));


		StaticModelPtr _eye = new StaticModel;
		_eye->SetMesh(_eyeMesh);
		_eye->SetParent(_body);
		_eye->SetLocalPosition({ 0, 0.6f, 0.8f });

		StaticModelPtr _shoulder = new StaticModel;
		_shoulder->SetMesh(_shoulderMesh);
		_shoulder->SetParent(_body);
		_shoulder->SetLocalPosition({ 0, 0.6f, 0 });


		StaticModelPtr _engineL = new StaticModel;
		_engineL->SetMesh(_engineMesh);
		_engineL->SetParent(_body);
		_engineL->SetLocalPosition({ -1.65f, 0.3f, 0 });


		StaticModelPtr _engineR = new StaticModel;
		_engineR->SetMesh(_engineMesh);
		_engineR->SetParent(_body);
		_engineR->SetLocalPosition({ +1.65f, 0.3f, 0 });



		_body->SetScene(_scene);
	}

	/*{
		Array<Node*> _nodes;
		for (uint i = 0; i < 200000; ++i)
		{
			NodePtr _test = new Node;
			_test->SetSleepingThreshold(15);
			_test->SetScene(_scene);
			_nodes.Push(_test);
		}

		//MessageBox(0, "", "", 0);

		for (uint i = 0; i < _nodes.Size(); ++i)
		{
			_nodes[i]->SetScene(nullptr);
		}
	}*/
	//MessageBox(0, "", "", 0);
}

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "opengl32.lib")

/*int __cdecl __stdio_common_vfprintf(
	_In_                                    unsigned __int64 _Options,
	_Inout_                                 FILE*            _Stream,
	_In_z_ _Printf_format_string_params_(2) char const*      _Format,
	_In_opt_                                _locale_t        _Locale,
	va_list          _ArgList
	)
{
	//vfprintf()
} */

#ifndef _DEBUG
#endif


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

class AIObject
{

};

class AIState
{
public:
};


class Entity : public Node
{
public:

	virtual void OnHit(Entity* _entity, const Vec3& _point, const Vec3& _normal, const Vec3& _force)
	{

	}

	virtual void Update(const FrameInfo& _frame)
	{
	}

protected:
	
	
	float m_health;
	AIObject* m_ai;
};

class Character : public Entity
{

};

class Player : public Character
{
public:

protected:
};



//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class DroneEngine : public Entity
{
	ModelPtr m_body;
	ParticleEmitterPtr m_exhaust;
	Vec3 m_dir;
};

class Drone : public Entity
{
public:

protected:
	
	DroneEngine* m_engineL;
	DroneEngine* m_engineR;
	ModelPtr m_body;
	ModelPtr m_forceField;
};

//----------------------------------------------------------------------------//
// EntityUpdater
//----------------------------------------------------------------------------//

class EntityUpdater : public Behavior
{
public:
	EntityUpdater(void) :
		m_entity(nullptr)
	{
	}
	~EntityUpdater(void)
	{
	}

	void Update(const FrameInfo& _frame) override
	{
		if (m_entity)
			m_entity->Update(_frame);
	}

protected:

	void _SetNode(Node* _node) override
	{
		m_node = _node;

		if (m_node && m_node->IsClass<Entity>())
			m_entity = static_cast<Entity*>(m_node);
		else
			m_entity = nullptr;
	}

	Entity* m_entity;
};

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
		
	{
		PRINT_SIZEOF(Node);
		LOG("1KK nodes(%d) = %d mb", (sizeof(Node)), ((sizeof(Node)) * 1000000) / 1024 / 1024);
		LOG("1KK nodes+dbvt(%d) = %d mb", (sizeof(Node) + sizeof(DbvtNode)), ((sizeof(Node) + sizeof(DbvtNode)) * 1000000) / 1024 / 1024);
/*#ifdef DEBUG
		system("pause");
#endif
		ExitProcess(0);*/
	} 

	new Device;
	new Graphics;
	new Renderer;
	Scene* _scene = new Scene;

	Vec3 _cubePosOnStack = 0;
	_cubePos = &_cubePosOnStack;

	CreatScene(_scene);

	_terrain = new Terrain;
	Ptr<Terrain> _terrainPtr = _terrain;
	{
		ImagePtr _hmap = new Image;
		_hmap->CreatePerlin(512, 0.05f);
		//_hmap->CreatePerlin(256, 0.1f);
		_terrain->Create(_hmap, 550, 7500, 254);
		//_terrain->Create(_hmap, 5, 75, 254);
		_terrain->SetScene(_scene);
	}

	const Color _clearColor(0x7f7f9fff);

	double _st, _et;
	float _dt = 0, _tt = 0, _pt = 0;
	float _fps = 0;
	int _frames = 0;

	bool _quit = false;

	uint _sampler;
	glGenSamplers(1, &_sampler);
	glBindSampler(1, _sampler);
	glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	gDevice->SetCursorMode(CM_Camera);

	/*RenderMeshPtr _cube;
	// temp
	{
		AlignedBox _box({ -0.15f, 10, -0.15f }, {0.15f, -10, 0.15f});

		BufferPtr _tempVB;
		BufferPtr _tempIB;

		_tempVB = new Buffer;
		_tempVB->Realloc(sizeof(Vertex) * 8);

		Vertex* _v = (Vertex*)_tempVB->Map(LM_WriteDiscard, 0, _tempVB->Size());
		_box.GetAllCorners(_v, sizeof(Vertex));
		_tempVB->Unmap();

		_tempIB = new Buffer;
		_tempIB->Realloc(sizeof(BOX_TRIANLGE_INDICES), BOX_TRIANLGE_INDICES);

		_cube = new RenderMesh;
		_cube->SetType(PT_Triangles);
		_cube->SetVertexBuffer(_tempVB);
		_cube->SetIndexBuffer(_tempIB);
		_cube->SetRange(0, 36);
	}	*/



	while (!_quit && !gDevice->ShouldClose())
	{
		_st = Time();
		gDevice->PollEvents();
		_et = Time();
		_pt += (float)(_et - _st);
		_st = _et;
		//_pt += (float)((TimeCounter() - _st) * TimeFreq());

		//_st = TimeCounter();

		_quit = GetAsyncKeyState(VK_ESCAPE) != 0;

		_scene->Update(_dt);

		gGraphics->BeginFrame();
		gGraphics->ClearFrameBuffers(FBT_Color | FBT_DepthStencil, _clearColor, 1, 0xff);

		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_CLAMP);
		//glDepthRange(-1, 1);

		//glEnable(GL_CULL_FACE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		gRenderer->Draw(_scene);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glDisable(GL_CULL_FACE);

		gGraphics->SetShader(VS_Terrain);
		gGraphics->SetShader(ST_Geometry, nullptr);
		gGraphics->SetShader(FS_Texture);
		gGraphics->SetTexture(0, _terrain->m_texture);
		gGraphics->SetTexture(1, _terrain->m_texture);
		glBindSampler(1, _sampler);

		//_terrain->m_mesh->Draw();



		gGraphics->SetShader(FS_NoTexture);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
		//glPolygonOffset(-0.75f, -4);
		//glEnable(GL_POLYGON_OFFSET_FILL);
		//glEnable(GL_POLYGON_OFFSET_LINE);
		//glEnable(GL_POLYGON_OFFSET_POINT);
		//glDisable(GL_DEPTH_TEST);
		glDepthMask(0);
		//glDepthFunc(GL_EQUAL);
		_terrain->m_mesh->Draw();
		glDepthMask(1);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		//glDepthFunc(GL_LEQUAL);
		//glDisable(GL_POLYGON_OFFSET_FILL);
		//glDisable(GL_POLYGON_OFFSET_LINE);
		//glDisable(GL_POLYGON_OFFSET_POINT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		  */

		{
			//glEnable(GL_DEPTH_TEST);
			Mat44 _cubeMat;
			_cubeMat.CreateTranslation(*_cubePos);
			gRenderer->m_instanceBuffer->Write(&_cubeMat, 0, sizeof(_cubeMat));
			gGraphics->SetShader(VS_StaticModel);
			gGraphics->SetShader(ST_Geometry, nullptr);

			//_cube->Draw();
		}


		wglSwapIntervalEXT(1);
		gGraphics->EndFrame();
		Sleep(1);

		_et = Time();
		_tt += (float)(_et - _st);
		++_frames;
		if (_tt > 0.2f)
		{
			if (_pt < _tt)
				_tt += _pt;
			_dt = _tt / _frames;
			_fps = 1 / _dt;
			_tt = 0;
			_pt = 0;
			_frames = 0;
		}
	}

	delete _scene;
	delete gRenderer;
	delete gGraphics;
	delete gDevice;

#ifdef DEBUG
	system("pause");
#endif
	ExitProcess(0);
}

#if 0
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
			{
				EntityPtr _entity = new Entity;
				Ptr<Transform> _transform = new Transform;
				_entity->AddComponent(_transform);
				_entity->SetScene(&_scene);

				EntityPtr _entity2 = new Entity;
				Ptr<Transform> _transform2 = new Transform;
				_entity2->AddComponent(_transform2);
				_entity2->SetScene(&_scene);

				_entity2->SetParent(_entity);

				_transform->SetWorldPosition(1);
				_transform2->SetWorldPosition(2);
			}

			BufferPtr _vb = new Buffer;
			{

				Vertex _v[3];
				_v[0].position.Set(250, 250, 0); // t	250 0
				_v[2].position.Set(0, 250, 0); // r
				_v[1].position.Set(250, 250, 0); // l
				_v[0].size[0] = 130, _v[0].size[1] = 130;
				_v[0].SetTexCoord(_fi.chars['A'].tc[0]);
				_v[0].SetTexCoord2(_fi.chars['A'].tc[1]);
				_v[0].color[0] = 0x00;
				_v[0].color[1] = 0x00;
				_v[0].color[2] = 0xff;
				_v[0].color[3] = 0xff;
				//_v[0].SetTexCoord(0);
				//_v[0].SetTexCoord2(1);
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
					_cam.NormMat = _view.Copy().Transpose(); // equal to inverse

					_cameraParams->Write(&_cam, 0, sizeof(_cam));

					uint _numInstances = MAX_INSTANCES;
					for (uint i = 0; i < _numInstances; ++i)
					{
						_model.SetIdentity();
						_model.SetTranslation(Vec3(-50, -50, 0) + Vec3(i, i, 0) * 0.1f);
						//_model.SetScale(0.01f);
						_worldMatrices->Write(&_model, sizeof(_model) * i, sizeof(_model));
					}


					/*_model.SetScale(1);
					_worldMatrices->Write(&_model, 0, sizeof(_model));
					_model.SetScale(2);
					_worldMatrices->Write(&_model, sizeof(_model), sizeof(_model));
					 */

					gGraphics->SetUniformBuffer(U_Camera, _cameraParams);
					gGraphics->SetUniformBuffer(U_InstanceMat, _worldMatrices);

					gGraphics->SetShader(VS_Particles);
					gGraphics->SetShader(GS_Billboard);
					gGraphics->SetShader(FS_NoTexture);

					gGraphics->SetTexture(0, _tex);

					gGraphics->SetVertexBuffer(_vb);
					gGraphics->Draw(PT_Points, 0, 1, _numInstances);
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
#endif

