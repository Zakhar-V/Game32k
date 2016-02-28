#pragma once

#include "Render.hpp"
#include "Device.hpp"


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
void Renderer::Draw(Scene* _scene)
{
	ASSERT(_scene != nullptr);

	Camera* _cam = _scene->GetActiveCamera();
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


	Mat44 _wm = MAT44_IDENTITY;
	_wm.SetTranslation({ 0, 0, 0 });
	//_wm.SetScale({ 5, 1, 1 });

	m_cameraBuffer->Write(&_cp, 0, sizeof(_cp));
	m_instanceBuffer->Write(&_wm, 0, sizeof(_wm));

	gGraphics->SetShader(VS_StaticModel);
	gGraphics->SetShader(ST_Geometry, nullptr);
	gGraphics->SetShader(FS_NoTexture);

	if(_frustum.Intersects(AlignedBox(-0.5f, 0.5f) * _wm))
		m_testCube->Draw();


	_wm = MAT44_IDENTITY;
	_wm.SetTranslation({ 0, -10, 0 });
	_wm.SetScale({ 100, 0.1f, 100 });
	m_instanceBuffer->Write(&_wm, 0, sizeof(_wm));
	m_testCube->Draw();

	m_instanceBuffer->Write(&MAT44_IDENTITY, 0, sizeof(MAT44_IDENTITY));
}

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

