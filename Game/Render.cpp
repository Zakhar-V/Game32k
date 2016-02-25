#pragma once

#include "Render.hpp"
#include "Device.hpp"

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Camera::Camera(void) :
	m_world(nullptr),
	m_orthographic(false),
	m_fov(PI / 4),
	m_near(1),
	m_far(1000),
	m_zoom(1)
{
	//m_eventMask = 1 << ET_TransformChanged;
}
//----------------------------------------------------------------------------//
Camera::~Camera(void)
{
}
//----------------------------------------------------------------------------//
void Camera::GetParams(float _x, float _y, float _w, float _h, UCamera& _params, Frustum& _frustum)
{
	Transform* _transform = GetEntityComponent<Transform>();
	Mat44 _worldTM = _transform ? _transform->GetWorldMatrix() : MAT44_IDENTITY;

	_params.CameraPos = _worldTM.GetTranslation();
	_params.ViewMat.CreateInverseTransform(_worldTM.GetTranslation(), _worldTM.GetRotation(), 1);
	_params.NormMat = _params.ViewMat.Copy().Inverse();

	if (m_orthographic)
	{
		_params.ProjMat.CreateOrtho(_x, _x + _w, _y + _h, _h, m_near, m_far);
	}
	else
	{
		_params.ProjMat.CreatePerspective(m_fov, _w / _h, m_near, m_far);
	}
	_params.ProjMat.m00 *= m_zoom;
	_params.ProjMat.m11 *= m_zoom;

	_frustum.FromCameraMatrices(_params.ViewMat, _params.ProjMat);

	_params.ViewProjMat = _params.ProjMat * _params.ViewMat;

	Mat44 _proj = _params.ProjMat;
	_proj.m22 = 0; // no near clip plane
	_params.InvViewProjMat = (_proj * _params.ViewMat).Inverse();

	// todo: depth params (near, far, C=constant, FC = 1.0/log(far*C + 1))
}
//----------------------------------------------------------------------------//
void Camera::_SetScene(Scene* _scene)
{
	RenderWorld* _world = _scene ? _scene->GetRenderWorld() : nullptr;
	if (m_world != _world)
	{
		bool _active = m_world && m_world->GetActiveCamera() == this;
		if (_active)
			m_world->SetActiveCamera(nullptr);

		m_world = _world;

		if(_active && m_world)
			m_world->SetActiveCamera(this);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RenderableObject
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
RenderableObject::RenderableObject(RenderableType _type) :
	m_prev(nullptr),
	m_next(nullptr),
	m_world(nullptr),
	m_type(_type)
{
}
//----------------------------------------------------------------------------//
RenderableObject::~RenderableObject(void)
{
}
//----------------------------------------------------------------------------//
void RenderableObject::_SetScene(Scene* _scene)
{
	RenderWorld* _world = _scene ? _scene->GetRenderWorld() : nullptr;
	if (m_world == _world)
		return;

	if (m_world)
	{
		Unlink(m_world->_ObjectList(), this, m_prev);
	}

	_SetWorldImpl(_world);

	if (m_world)
	{
		Link(m_world->_ObjectList(), this, m_prev);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Mesh
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Mesh::Mesh(void) :
	RenderableObject(RT_StaticMesh)
{

}
//----------------------------------------------------------------------------//
Mesh::~Mesh(void)
{

}
//----------------------------------------------------------------------------//
void Mesh::SetSkinned(bool _skinned)
{
	m_type = _skinned ? RT_SkinnedMesh : RT_StaticMesh;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// RenderWorld
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
RenderWorld::RenderWorld(Scene* _scene) : 
	SceneSystem(_scene),
	m_activeCamera(nullptr)
{
}
//----------------------------------------------------------------------------//
RenderWorld::~RenderWorld(void)
{
	// ...

	//if (m_activeCamera)
	//	m_activeCamera->_SetScene(nullptr);
}
//----------------------------------------------------------------------------//
void RenderWorld::Draw(void)
{
	if (!m_activeCamera)
		return;

	uint _wndWidth = gDevice->WindowWidth();
	uint _wndHeight = gDevice->WindowHeight();
	Vec2 _wndSize = gDevice->WindowSize();

	gRenderer->Draw(this);



}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Renderer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Renderer::Renderer(void)
{
	LOG("Create Renderer");

	ASSERT(gGraphics != nullptr); // the render context must be created before it

	m_cameraBuffer = new Buffer(BU_Dynamic);
	m_cameraBuffer->Realloc(sizeof(UCamera));
	m_instanceBuffer = new Buffer(BU_Dynamic);
	m_instanceBuffer->Realloc(sizeof(UInstanceMat));
	m_skinBuffer = new Buffer(BU_Dynamic);
	m_skinBuffer->Realloc(sizeof(USkinMat));



	//m_primaryRenderBuffer = new RenderBuffer(PF_RGBA8);
	//m_primaryDepthStencilBuffer = new RenderBuffer(PF_D24S8);

	m_gBufferColorTexture = new Texture(TT_Array, PF_RGBA8, false);
	m_gBufferNormalTexture = new Texture(TT_Array, PF_RGB10A2, false);
	m_gBufferDepthTexture = new Texture(TT_Array, PF_D24S8, false);

	// temp
	{
		AlignedBox _box(-0.5f, 0.5f);

		BufferPtr _tempVB;
		BufferPtr _tempIB;

		_tempVB = new Buffer;
		_tempVB->Realloc(sizeof(Vertex) * 8);

		Vertex* _v = (Vertex*)_tempVB->Map(LM_WriteDiscard, 0, _tempVB->Size());
		_box.GetAllCorners(_v, sizeof(Vertex));
		_tempVB->Unmap();

		_tempIB = new Buffer;
		_tempIB->Realloc(sizeof(BOX_TRIANLGE_INDICES), BOX_TRIANLGE_INDICES);

			
		m_testCube = new RenderMesh;
		m_testCube->SetType(PT_Triangles);
		m_testCube->SetVertexBuffer(_tempVB);
		m_testCube->SetIndexBuffer(_tempIB);
		m_testCube->SetRange(0, 36);
	}

}
//----------------------------------------------------------------------------//
Renderer::~Renderer(void)
{
	LOG("Destroy Renderer");

	//delete m_primaryRenderBuffer;
	//delete m_primaryDepthStencilBuffer;
}
//----------------------------------------------------------------------------//
void Renderer::Draw(RenderWorld* _world)
{
	ASSERT(_world != nullptr);

	Camera* _cam = _world->GetActiveCamera();
	if (!_cam)
		return;

	uint _wndWidth = gDevice->WindowWidth();
	uint _wndHeight = gDevice->WindowHeight();
	Vec2 _wndSize = gDevice->WindowSize();
	UCamera _cp;
	Frustum _frustum;
	_cam->GetParams(0, 0, _wndSize.x, _wndSize.y, _cp, _frustum);


	gGraphics->SetUniformBuffer(U_Camera, m_cameraBuffer);
	gGraphics->SetUniformBuffer(U_InstanceMat, m_instanceBuffer);
	gGraphics->SetUniformBuffer(U_SkinMat, m_skinBuffer);


	m_cameraBuffer->Write(&_cp, 0, sizeof(_cp));
	m_instanceBuffer->Write(&MAT44_IDENTITY, 0, sizeof(MAT44_IDENTITY));

	gGraphics->SetShader(VS_StaticModel);
	gGraphics->SetShader(ST_Geometry, nullptr);
	gGraphics->SetShader(FS_NoTexture);
	m_testCube->Draw();

}

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

