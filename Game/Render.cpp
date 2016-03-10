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
	if (_node->IsEnabled() && (_node->GetTypeMask() & mask))
	{
		objects.Push(_node);
		_node->WakeUp();
		//_node->SetLastViewFrame(...)
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

struct DbvtDebugRenderer
{
	Array<Vertex> vertices;

	void AddBox(const AlignedBox& _bbox, const Color& _color)
	{
		Vertex _v[8];
		_bbox.GetAllCorners(_v, sizeof(Vertex));
		for (uint i = 0; i < 8; ++i)
		{
			_v[i].SetColor(_color);
		}
		for (uint i = 0; i < 24; ++i)
		{
			vertices.Push(_v[BOX_LINE_INDICES[i]]);
		}
	}

	void AddBoxes(DbvtNode* _root)
	{
		Array<DbvtNode*> _stack;
		_stack.Push(_root);
		//DbvtNode* _stackBase[1000];
		//DbvtNode** _stack = _stackBase;
		//*_stack++ = _root;
		//int _depth = 1;
		if (_root) do
		{
			DbvtNode* _node = _stack.Top();
			_stack.Pop();
			float _color = ((_stack.Size() + 1) / 4.f);

			if (_node->IsNode())
			{
				_stack.Push(_node->child0);
				_stack.Push(_node->child1);
			}
			else
				_color = 1;

			AddBox(_node->box, Vec4(_color, _color, _color, _color));

		} while (_stack.Size() > 0);
	}
};

//----------------------------------------------------------------------------//
// Renderer
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Renderer::Renderer(void)
{
	LOG("Create Renderer");

	ASSERT(gGraphics != nullptr); // the render context must be created before it

	// fullscreen quad
	{
		Vertex _fsq[4];
		// TODO
		uint16_t _idx[6];
		// TODO

		m_fsQuad = new VertexArray(PT_Triangles, true);
		m_fsQuad->GetVertexBuffer()->Realloc(sizeof(_fsq), _fsq);
		m_fsQuad->GetIndexBuffer()->Realloc(sizeof(_idx), _idx);
	}

	// depthstencil state
	{
		m_depthStencilEnabled = gGraphics->AddDepthStencilState({ true });
		m_depthStencilDisabled = gGraphics->AddDepthStencilState({ false });
	}

	// renderbuffers
	{
		m_colorBuffer = new RenderBuffer(PF_RGBA8);
		m_depthStencilBuffer = new RenderBuffer(PF_D24S8);
	}

	// gbuffer
	{
		m_gBufferColorTexture = new Texture(TT_Array, PF_RGBA8, false);
		m_gBufferNormalTexture = new Texture(TT_Array, PF_RGB10A2, false);
		m_gBufferMaterialTexture = new Texture(TT_Array, PF_RGBA8, false);
		m_gBufferDepthTexture = new Texture(TT_Array, PF_D24S8, false);
	}

	// camera params
	{
		m_cameraBuffer = new Buffer(BU_Dynamic);
		m_cameraBuffer->Realloc(sizeof(UCamera));
		//m_cameraParams;
		//m_frustum;
		m_screenSize = 0;
	}

	// instance matrices
	{
		m_instanceMatBuffer = new Buffer(BU_Dynamic);
		m_instanceMatBuffer->Realloc(sizeof(UInstanceMat));
	}

	// skin matrices
	{
		m_skinBuffer = new Buffer(BU_Dynamic);
		m_skinBuffer->Realloc(sizeof(USkinMat));
	}

	// instance materials
	{
		m_instanceMtlBuffer = new Buffer(BU_Dynamic);
		m_instanceMtlBuffer->Realloc(sizeof(UInstanceMtl));
	}

	// default material params
	{
		//m_defaultMaterialParams;
		m_defaultMtl = new Material;
	}

	// rasterizer params
	{
		m_rasterizerParamsBuffer = new Buffer(BU_Dynamic);
		m_rasterizerParamsBuffer->Realloc(sizeof(URasterizerParams));
		m_rasterizerParamsStorage.DepthBias = 0;
		m_rasterizerParamsStorage.SilhouetteOffset = 0;
	}

	// dbvt callbacks
	{
		m_dbvtFrustumCallback.container = &m_renderContainer;
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

	_BindUniformBuffers();

	Camera* _cam = _scene->GetActiveCamera();
	if (!_cam)
		return;

	_SetupViewport(_cam);
	_GetVisibleObjects(_scene, m_frustum);

	m_cameraBuffer->Write(&m_cameraParams, 0, sizeof(m_cameraParams));
	gGraphics->SetDepthStencilState(m_depthStencilEnabled);

	_DrawBatchs(m_renderItems.Ptr(), m_renderItems.Size(), nullptr);
	
#if 0
	uint _wndWidth = gDevice->WindowWidth();
	uint _wndHeight = gDevice->WindowHeight();
	Vec2 _wndSize = gDevice->WindowSize();
	_cam->GetParams(0, 0, _wndSize.x, _wndSize.y, m_cameraParams, m_frustum);


	// bind uniforms
	gGraphics->SetUniformBuffer(U_Camera, m_cameraBuffer);
	gGraphics->SetUniformBuffer(U_InstanceMat, m_instanceMatBuffer);
	gGraphics->SetUniformBuffer(U_InstanceMtl, m_instanceMtlBuffer);
	gGraphics->SetUniformBuffer(U_SkinMat, m_skinBuffer);
	gGraphics->SetUniformBuffer(U_RasterizerParams, m_rasterizerParamsBuffer);


	// get visible objects
	{
#if 0
		int _depth = _scene->GetDbvt()->GetMaxDepth();
		gDevice->SetTitle("max dbvt depth = %d\n", _depth);
#endif
	}

	m_renderContainer.objects.Clear();
	m_renderContainer.mask = NT_RenderNode | NT_Light;

	DbvtFrustumCallback _callback;

	_callback.container = &m_renderContainer;
	_callback.volume = m_frustum;

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


	for (DirectionalLight* i = _scene->GetDirectionalLight(); i; i = i->GetNextLight())
	{
		if (i->IsEnabled())
		{
			m_lights.Push(i);
		}
	}



	//printf("nodes: %d\n", _container.objects.Size());


	// sort objects


	// draw objects

	m_cameraBuffer->Write(&m_cameraParams, 0, sizeof(m_cameraParams));

	Mat44 _wm = MAT44_IDENTITY;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_DEPTH_TEST);

	gGraphics->SetDepthStencilState(m_depthStencilEnabled);

	uint _numDrawCalls = 0;

	for (RenderItem* i = m_renderItems.Ptr(), *e = m_renderItems.Ptr() + m_renderItems.Size(); i < e;)
	{
		gGraphics->SetShader(FS_NoTexture);

		//LOG("%#p %f %f %f", i->node, i->node->GetWorldPosition().x, i->node->GetWorldPosition().y, i->node->GetWorldPosition().z);

		if (i->node->GetTypeMask() & NT_Terrain)
		{
			gGraphics->SetShader(VS_StaticModel);
			gGraphics->SetShader(ST_Geometry, nullptr);

			m_instanceMatBuffer->Write(&i->node->GetWorldTransform(), 0, sizeof(MAT44_IDENTITY));
			i->data->Bind();
			i->data->Draw(i->start, i->count);
			++_numDrawCalls;

			++i;
		}
		else if (i->node->GetTypeMask() & NT_StaticModel)
		{
			gGraphics->SetShader(VS_StaticModel);
			gGraphics->SetShader(ST_Geometry, nullptr);

			VertexArray* _vao = i->data;
			uint _start = i->start;
			uint _count = i->count;
			Material* _material = i->material;
			uint _numInstances = 0;
			while (i < e && _numInstances < MAX_INSTANCES)
			{
				if (i->data == _vao && i->start == _start && i->count == _count && i->material == _material)
				{
					m_instanceMatStorage.WorldMat[_numInstances] = i->node->GetWorldTransform();
					m_instanceMtlStorage.InstanceMtl[_numInstances] = i->node->_GetMaterialParams();
					++_numInstances;
					++i;
				}
				else
					break;
			}

			m_instanceMatBuffer->Write(&m_instanceMatStorage, 0, _numInstances * sizeof(Mat44));
			m_instanceMtlBuffer->Write(&m_instanceMtlStorage, 0, _numInstances * sizeof(Mat34));
			_vao->Bind();
			_vao->Draw(_start, _count, _numInstances);
			++_numDrawCalls;
		}
		else
			++i;
	}
	//printf("\n");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	printf("visible objects: %d, draw calls: %d\n", m_renderContainer.objects.Size(), _numDrawCalls);

#if 1

	{

		DbvtDebugRenderer _dbg;
		_dbg.AddBoxes(_scene->GetDbvt()->Root());

		Buffer _buffer;
		_buffer.Realloc(_dbg.vertices.Size() * sizeof(Vertex),_dbg.vertices.Ptr());
		
		//glDisable(GL_DEPTH_TEST);

		m_instanceMatBuffer->Write(&MAT44_IDENTITY, 0, sizeof(MAT44_IDENTITY));
		gGraphics->SetVertexBuffer(&_buffer);
		gGraphics->SetShader(VS_StaticModel);
		gGraphics->SetShader(ST_Geometry, nullptr);
		gGraphics->SetShader(FS_Texture);
		gGraphics->Draw(PT_Lines, 0, _dbg.vertices.Size());
	}
#endif

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



	/*_wm.SetTranslation({ 0, 0, 0 });
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

#endif
}
//----------------------------------------------------------------------------//
void Renderer::_BindUniformBuffers(void)
{
	gGraphics->SetUniformBuffer(U_Camera, m_cameraBuffer);
	gGraphics->SetUniformBuffer(U_InstanceMat, m_instanceMatBuffer);
	gGraphics->SetUniformBuffer(U_InstanceMtl, m_instanceMtlBuffer);
	gGraphics->SetUniformBuffer(U_SkinMat, m_skinBuffer);
	gGraphics->SetUniformBuffer(U_RasterizerParams, m_rasterizerParamsBuffer);
}
//----------------------------------------------------------------------------//
void Renderer::_SetupViewport(Camera* _cam)
{
	m_screenSize.Set(gDevice->WindowWidth(), gDevice->WindowHeight());
	m_cameraParams.ScreenSize = gDevice->WindowSize();
	m_cameraParams.InvScreenSize = 1 / gDevice->WindowSize();
	_cam->GetParams(0, 0, (float)m_cameraParams.ScreenSize.x, (float)m_cameraParams.ScreenSize.y, m_cameraParams, m_frustum);

	//gGraphics->SetViewport(...);
}
//----------------------------------------------------------------------------//
void Renderer::_GetVisibleObjects(Scene* _scene, const Frustum& _frustum)
{
	m_renderContainer.objects.Clear();
	m_renderContainer.mask = NT_RenderNode | NT_Light;
	m_dbvtFrustumCallback.volume = m_frustum;
	_scene->GetDbvt()->Enum(&m_dbvtFrustumCallback);

	m_lights.Clear();
	

	m_staticModelItems.Clear();
	m_skinnedModelItems.Clear();
	m_skyItems.Clear();
	m_terrainItems.Clear();

	m_renderItems.Clear();
	m_lights.Clear();
	for (uint i = 0; i < m_renderContainer.objects.Size(); ++i)
	{
		Node* _node = m_renderContainer.objects[i];
		uint _type = _node->GetTypeMask();


		if (_node->GetTypeMask() & NT_RenderNode)
		{
			static_cast<RenderNode*>(_node)->GetRenderItems(m_renderItems);
		}
		else
		{
			ASSERT(_type & NT_Light);
			m_lights.Push(static_cast<Light*>(_node));
		}
	}


	for (DirectionalLight* i = _scene->GetDirectionalLight(); i; i = i->GetNextLight())
	{
		if (i->IsEnabled())
		{
			m_lights.Push(i);
		}
	}



	/*m_lights = Move(m_renderContainer.lights);
	for (DirectionalLight* i = _scene->GetDirectionalLight(); i; i = i->GetNextLight())
	{
		if (i->IsEnabled())
		{
			m_renderContainer.objects.Push(i);
		}
	}*/
}
//----------------------------------------------------------------------------//
void Renderer::_SetupVertexShaders(uint _type)
{
	Shader* _vs = nullptr;
	Shader* _gs = nullptr;

	if (_type & NT_StaticModel)
	{
		_vs = gGraphics->GetShader(VS_StaticModel);
	}
	else if (_type & NT_SkinnedModel)
	{
		_vs = gGraphics->GetShader(VS_SkinnedModel);
	}
	/*else if (_type & (NT_ParticleEmitter))
	{
		_vs = gGraphics->GetShader(VS_SkinnedModel);
		_gs = gGraphics->GetShader(VS_SkinnedModel);
	}*/
	//else if(_type & NT_)

	gGraphics->SetShader(ST_Vertex, _vs);
	gGraphics->SetShader(ST_Geometry, _gs);
}
//----------------------------------------------------------------------------//
void Renderer::_FillGBuffer(void)
{
	//for()
}
//----------------------------------------------------------------------------//
void Renderer::_DrawBatchs(RenderItem* _items, uint _numItems, Shader* _fragmentShader)
{
	RenderItem* _end = _items + _numItems;
	Shader* _vs = nullptr;
	Shader* _gs = nullptr;
	Shader* _fs = nullptr;

	uint _start, _count;
	VertexArray* _vao;
	Material *_mtl, *_cmtl;

	while (_items < _end)
	{
		RenderNode* _node = _items->node;
		uint _type = _node->GetTypeMask();
		uint _numInstances = 0;
		
		// geometry
		_vao = _items->data;
		_start = _items->start;
		_count = _items->count;
		
		// material
		_mtl = _items->material;
		if (!_mtl)
			_mtl = m_defaultMtl;


		// shaders

		if (_fragmentShader)
			_fs = _fragmentShader;
		else
			_fs = gGraphics->GetShader(_mtl->GetShader());

		_gs = nullptr;

		if (_type & (NT_StaticModel | NT_Terrain))
		{
			_vs = gGraphics->GetShader(VS_StaticModel);
		}
		else if (_type & NT_SkinnedModel)
		{

		}
		else if (_type & NT_SkyDome)
		{
			//m_instanceMatStorage.WorldMat[_numInstances].CreateTransform(_camera->GetWorldPosition(), QUAT_IDENTITY, _node->GetWorldScale());
		}


		// batch
		_cmtl = _items->material;
		do
		{
			m_instanceMatStorage.WorldMat[_numInstances] = _items->node->GetWorldTransform();
			m_instanceMtlStorage.InstanceMtl[_numInstances] = _items->node->GetMaterialParams();
			++_numInstances;
			++_items;

		} while (_items < _end && _numInstances < MAX_INSTANCES &&
			(_items->node->GetTypeMask() & (NT_StaticModel | NT_Terrain)) && // instanceable
			_items->node->GetTypeMask() == _type &&
			_items->data == _vao &&
			_items->start == _start &&
			_items->count == _count && _items->material == _cmtl);


		/*if (_type & (NT_StaticModel | NT_Terrain))
		{

			while (_items < _end && _numInstances < MAX_INSTANCES)
			{
				if (_items->node->GetTypeMask() == _type && _items->data == _vao && _items->start == _start && _items->count == _count && (_items->material == _cmtl))
				{
					m_instanceMatStorage.WorldMat[_numInstances] = _items->node->GetWorldTransform();
					m_instanceMtlStorage.InstanceMtl[_numInstances] = _items->node->_GetMaterialParams();
					++_numInstances;
					++_items;
				}
				else
					break;
			}
		}*/

		// draw
		gGraphics->SetShader(ST_Vertex, _vs);
		gGraphics->SetShader(ST_Geometry, _gs);
		gGraphics->SetShader(ST_Fragment, _fs);
		gGraphics->SetSampler(0, _mtl->GetSampler());
		gGraphics->SetTexture(0, _mtl->GetTexture());
		//m_mtlBuffer->Write(&_mtl->_GetParams(), 0, sizeof(Mat34));
		m_instanceMtlBuffer->Write(&m_instanceMtlStorage, 0, _numInstances * sizeof(Mat34));
		m_instanceMatBuffer->Write(&m_instanceMatStorage, 0, _numInstances * sizeof(Mat44));
		_vao->Bind();
		_vao->Draw(_start, _count, _numInstances);
		//++_numDrawCalls;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

