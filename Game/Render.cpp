#pragma once

#include "Render.hpp"
#include "Device.hpp"
#include "Scene.hpp"


//----------------------------------------------------------------------------//
// RenderContainer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void RenderContainer::AddObject(void* _object, const AlignedBox& _bbox, bool _withTest)
{
	Node* _node = reinterpret_cast<Node*>(_object);
	if (_node->GetTypeMask() & mask)
		objects.Push(_node);
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
	/*{
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
	} */

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


	// get visible objects

	m_renderContainer.objects.Clear();
	m_renderContainer.mask = NT_RenderNode | NT_Light;

	DbvtFrustumCallback _callback;

	_callback.container = &m_renderContainer;
	_callback.volume = _frustum;

	_scene->GetDbvt()->Enum(&_callback);

	m_renderItems.Clear();
	m_lights.Clear();
	for (uint i = 0; i < m_renderContainer.objects.Size(); ++i)
	{
		Node* _node = m_renderContainer.objects[i];
		if (_node->IsEnabled())
		{
			_node->WakeUp();
			//_node->SetLastViewFrame(_scene->GetFrame());

			if (_node->GetTypeMask() & NT_RenderNode)
			{
				static_cast<RenderNode*>(_node)->GetRenderItems(m_renderItems);
			}
			else if (_node->GetTypeMask() & NT_Light)
			{
				m_lights.Push(static_cast<Light*>(_node));
			}
		}
	}
	/*for (DirectionalLight* i = _scene->GetDirectionalLight(); i; i = i->GetNextLight())
	{
		if (i->IsEnabled())
		{
			m_lights.Push(i);
		}
	} */



	//printf("nodes: %d\n", _container.objects.Size());


	// sort objects


	// draw objects

	m_cameraBuffer->Write(&_cp, 0, sizeof(_cp));

	Mat44 _wm = MAT44_IDENTITY;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (RenderItem* i = m_renderItems.Ptr(), *e = m_renderItems.Ptr() + m_renderItems.Size(); i < e; ++i)
	{
		if (i->node->GetTypeMask() & NT_Terrain)
		{
			gGraphics->SetShader(VS_Terrain);
			gGraphics->SetShader(ST_Geometry, nullptr);
		}

		m_instanceBuffer->Write(&i->node->GetWorldTransform(), 0, sizeof(MAT44_IDENTITY));
		
		gGraphics->SetShader(FS_NoTexture);

		i->data->Bind();
		i->data->Draw(i->start, i->count);
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// get lights

	// draw lights
	{
		// draw shadows
		{
		
		}

		// lighting

	}

	// postprocess


	//----------------------------------------------------------------------------//



	_wm.SetTranslation({ 0, 0, 0 });
	//_wm.SetScale({ 5, 1, 1 });


	gGraphics->SetShader(VS_StaticModel);
	gGraphics->SetShader(ST_Geometry, nullptr);
	gGraphics->SetShader(FS_Texture);

	/*AlignedBox _aabb = AlignedBox(-0.5f, 0.5f) * _wm;
	if (_frustum.Intersects(_aabb))
	{
		m_testCube->Draw();
		//printf("v (%f %f %f  %f %f %f) (%f %f %f  %f %f %f)\n", _aabb.mn.x, _aabb.mn.y, _aabb.mn.z, _aabb.mx.x, _aabb.mx.y, _aabb.mx.z, _frustum.box.mn.x, _frustum.box.mn.y, _frustum.box.mn.z, _frustum.box.mx.x, _frustum.box.mx.y, _frustum.box.mx.z);
	}
	//else
	//	printf("n\n");
				 */

	/*_wm = MAT44_IDENTITY;
	_wm.SetTranslation({ 0, -10, 0 });
	_wm.SetScale({ 100, 0.1f, 100 });
	m_instanceBuffer->Write(&_wm, 0, sizeof(_wm));
	m_testCube->Draw();

	m_instanceBuffer->Write(&MAT44_IDENTITY, 0, sizeof(MAT44_IDENTITY)); */
}

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

