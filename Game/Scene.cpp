#pragma once

#include "Scene.hpp"
#include "Physics.hpp"
#include "Render.hpp"

//----------------------------------------------------------------------------//
// Node
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Node::Node(void) :
	m_nameHash(0),
	m_id(0),
	m_layerMask(0xffff),
	m_tags(0),
	m_typeMask(NT_Node),
	m_localPosition(VEC3_ZERO),
	m_localRotation(QUAT_IDENTITY),
	m_localScale(VEC3_ONE),
	m_worldTransform(MAT44_IDENTITY),
	m_worldRotation(QUAT_IDENTITY),
	m_scene(nullptr),
	m_entity(nullptr),
	m_parent(nullptr),
	m_prev(nullptr),
	m_next(nullptr),
	m_child(nullptr),
	m_prevActive(nullptr),
	m_nextActive(nullptr),
	m_behaviors(nullptr),
	m_dbvtNode(nullptr),
	m_physics(nullptr),
	m_enabled(true),
	m_active(false),
	m_transformUpdated(true),
	m_transformChanged(false),
	m_worldBBoxUpdated(true),
	m_dbvtUpdated(true),
	m_dbvtNodeInTree(false),
	m_inheritPosition(true),
	m_inheritRotation(true),
	m_inheritScale(true),
	m_lastUpdateFrame(0),
	m_lastViewFrame(0),
	m_sleepingThreshold(0.2f), // 200 ms = ~12 frames on 60 fps
	m_activeTime(0)
{
}
//----------------------------------------------------------------------------//
Node::~Node(void)
{
	DetachChildren(true);
	RemoveAllBehaviors();
	m_dbvtNode = new DbvtNode;
	delete m_dbvtNode;
}
//----------------------------------------------------------------------------//
void Node::SetScene(Scene* _scene)
{
	if (m_scene == _scene)
		return;

	if (m_parent && m_parent->m_scene != _scene)
		return;

	if (!m_parent && !m_scene)
		AddRef();

	if (m_scene)
		m_scene->_RemoveNode(this);

	_SetSceneImpl(_scene);

	if (m_scene)
		m_scene->_AddNode(this);

	for (Behavior* i = m_behaviors; i; i = i->m_next)
	{
		i->_SetScene(m_scene);
	}

	for (Node* i = m_child; i; i = i->m_next)
	{
		i->SetScene(m_scene);
	}

	if (!m_parent && !m_scene)
		Release();
}
//----------------------------------------------------------------------------//
void Node::SetParent(Node* _parent)
{
	if (m_parent == _parent)
		return;

	for (Node* i = _parent; i; i = i->m_parent)
	{
		if (i == this) // child cannot be parent
			return;
	}

	if (!m_parent && !m_scene)
		AddRef();

	Mat44 _worldTM = GetWorldTransform(); // get current world transform

	if (m_parent)
	{
		Unlink(m_parent->m_child, this, m_prev);
		m_parent->_OnChildRemoved(this);
	}
	else if (m_scene)
	{
		m_scene->_RemoveRootNode(this);
	}

	m_parent = _parent;

	if (m_parent)
	{
		Link(m_parent->m_child, this, m_prev);

		if (m_parent->m_scene)
			SetScene(m_parent->m_scene);

		m_parent->_OnChildAdded(this);
	}
	else if (m_scene)
	{
		m_scene->_AddRootNode(this);
	}

	SetWorldTransform(_worldTM); // keep world transform

	if (!m_parent && !m_scene)
		Release();
}
//----------------------------------------------------------------------------//
void Node::DetachChildren(bool _remove)
{
	for (Node *i = m_child; i;)
	{
		NodePtr n = i;
		i = i->m_next;

		n->SetParent(nullptr);
		if (_remove && !n->m_entity)
			n->SetScene(nullptr);
	}
}
//----------------------------------------------------------------------------//
void Node::AddBehavior(Behavior* _b)
{
	if (!_b || _b->m_node)
		return;

	Link(m_behaviors, _b, _b->m_prev);
	_b->AddRef();
	_b->_SetNode(this);
	_b->_SetScene(m_scene);

	if (!m_active && _b->IsEnabled() && !_b->IsSleepingAllowed())
		WakeUp();
}
//----------------------------------------------------------------------------//
void Node::RemoveBehavior(Behavior* _b)
{
	if (!_b || _b->m_node != this)
		return;

	Unlink(m_behaviors, _b, _b->m_prev);
	_b->_SetNode(nullptr);
	_b->_SetScene(nullptr);
	_b->Release();
}
//----------------------------------------------------------------------------//
void Node::RemoveAllBehaviors(void)
{
	while (m_behaviors)
		RemoveBehavior(m_behaviors);
}
//----------------------------------------------------------------------------//
Behavior* Node::GetBehavior(uint _class, uint _index)
{
	for (Behavior* i = m_behaviors; i; i = i->m_next)
	{
		if (i->IsClass(_class) && _index-- == 0)
			return i;
	}
	return nullptr;
}
//----------------------------------------------------------------------------//
void Node::WakeUp(void)
{
	m_activeTime = 0;

	if (m_scene && !m_active)
	{
		m_scene->_AddActiveNode(this);
		m_active = true;
	}
}
//----------------------------------------------------------------------------//
void Node::Update(const FrameInfo& _frame)
{
	if (m_lastUpdateFrame == _frame.id)
		return;

	m_lastUpdateFrame = _frame.id;
	m_activeTime += _frame.time;

	if (m_parent && m_parent->m_lastUpdateFrame != _frame.id) // parent must be updated before it
	{
		m_parent->Update(_frame);
	}

	_PreUpdateImpl(_frame);

	m_behaviorAllowSleep = true;
	for (Behavior* i = m_behaviors; i;)
	{
		BehaviorPtr b = i;
		i = i->m_next;
		if (b->IsEnabled())
		{
			m_behaviorAllowSleep = m_behaviorAllowSleep && b->IsSleepingAllowed();
			b->Update(_frame);
		}
	}

	_UpdateImpl(_frame);
}
//----------------------------------------------------------------------------//
void Node::PostUpdate(const FrameInfo& _frame)
{
	_PostUpdateImpl(_frame);

	if (m_activeTime >= m_sleepingThreshold)
	{
		m_activeTime = 0;

		if (m_active && m_scene && m_sleepingThreshold != 0 && m_behaviorAllowSleep)
		{
			m_scene->_RemoveActiveNode(this);
			m_active = false;
		}
	}
	
	if(!m_dbvtUpdated)
		_UpdateDbvtNode();

	m_transformChanged = false;
}
//----------------------------------------------------------------------------//
const Mat44& Node::GetWorldTransform(void)
{
	_UpdateTransform();
	return m_worldTransform;
}
//----------------------------------------------------------------------------//
void Node::SetWorldTransform(const Mat44& _tm)
{
	SetWorldPosition(_tm.GetTranslation());
	SetWorldRotation(_tm.GetRotation());
	SetWorldScale(_tm.GetScale());
}
//----------------------------------------------------------------------------//
void Node::SetLocalPosition(const Vec3& _position)
{
	m_localPosition = _position;
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
const Vec3& Node::GetLocalPosition(void)
{
	return m_localPosition;
}
//----------------------------------------------------------------------------//
void Node::SetLocalRotation(const Quat& _rotation)
{
	m_localRotation = _rotation;
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
const Quat& Node::GetLocalRotation(void)
{
	return m_localRotation;
}
//----------------------------------------------------------------------------//
void Node::SetLocalScale(const Vec3& _scale)
{
	m_localScale = _scale;
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
const Vec3& Node::GetLocalScale(void)
{
	return m_localScale;
}
//----------------------------------------------------------------------------//
void Node::SetWorldPosition(const Vec3& _position)
{
	m_localPosition = _position;
	if (m_inheritPosition && m_parent)
		m_localPosition = m_parent->GetWorldTransform().Copy().Inverse().Transform(m_localPosition);
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
Vec3 Node::GetWorldPosition(void)
{
	_UpdateTransform();
	return m_worldTransform.GetTranslation();
}
//----------------------------------------------------------------------------//
void Node::SetWorldRotation(const Quat& _rotation)
{
	m_localRotation = _rotation;
	if (m_inheritRotation && m_parent)
		//m_localRotation = m_parent->GetWorldMatrix().Copy().Inverse().GetRotation() * m_localRotation;
		m_localRotation = m_parent->GetWorldRotation().Copy().Inverse() * m_localRotation;
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
const Quat& Node::GetWorldRotation(void)
{
	_UpdateTransform();
	return m_worldRotation;
}
//----------------------------------------------------------------------------//
void Node::SetWorldScale(const Vec3& _scale)
{
	m_localScale = _scale;
	if (m_inheritScale && m_parent)
		m_localScale /= m_parent->GetWorldTransform().GetScale();
	_InvalidateTransform();
}
//----------------------------------------------------------------------------//
Vec3 Node::GetWorldScale(void)
{
	_UpdateTransform();
	return m_worldTransform.GetScale();
}
//----------------------------------------------------------------------------//
void Node::SetInheritPosition(bool _enabled)
{
	if (m_inheritPosition != _enabled)
	{
		Vec3 _pos = GetWorldPosition();
		m_inheritPosition = _enabled;
		SetWorldPosition(_pos);
	}
}
//----------------------------------------------------------------------------//
void Node::SetInheritRotation(bool _enabled)
{
	if (m_inheritRotation != _enabled)
	{
		Quat _rot = GetWorldRotation();
		m_inheritRotation = _enabled;
		SetWorldRotation(_rot);
	}
}
//----------------------------------------------------------------------------//
void Node::SetInheritScale(bool _enabled)
{
	if (m_inheritScale != _enabled)
	{
		Vec3 _scl = GetWorldScale();
		m_inheritScale = _enabled;
		SetWorldScale(_scl);
	}
}
//----------------------------------------------------------------------------//
void Node::_InvalidateTransform(void)
{
	if (m_transformUpdated)
	{
		WakeUp();
		m_transformUpdated = false;
		m_transformChanged = true;
		_InvalidateWorldBBox();
		for (Node* i = m_child; i; i = i->m_next) // invalidate and activate all children
			i->_InvalidateTransform();
	}
}
//----------------------------------------------------------------------------//
void Node::_UpdateTransform(void)
{
	if (!m_transformUpdated)
	{
		Vec3 _pos = m_localPosition;
		Quat _rot = m_localRotation;
		Vec3 _scl = m_localScale;

		if (m_parent)
		{
			if (m_inheritPosition)
				_pos = m_parent->GetWorldTransform().Transform(_pos);
			if (m_inheritRotation)
				_rot = m_parent->GetWorldRotation() * _rot;
			if (m_inheritScale)
				_scl *= m_parent->GetWorldScale();
		}

		m_worldTransform.CreateTransform(_pos, _rot, _scl);
		m_worldRotation = m_worldTransform.GetRotation();
		m_transformUpdated = true;
	}
}
//----------------------------------------------------------------------------//
void Node::_InvalidateWorldBBox(void)
{
	if (m_worldBBoxUpdated)
	{
		WakeUp();
		m_worldBBoxUpdated = false;
		if (m_dbvtNode)
			m_dbvtUpdated = false;
	}
}
//----------------------------------------------------------------------------//
void Node::_CreateDbvtNode(void)
{
	if (!m_dbvtNode)
	{
		WakeUp();
		m_dbvtNode = new DbvtNode;
		m_dbvtNode->object = this;
		_GetWorldBBox(m_dbvtNode->box);
		m_dbvtNodeInTree = false;
		m_dbvtUpdated = false;
	}
}
//----------------------------------------------------------------------------//
void Node::_DeleteDbvtNode(void)
{
	if (m_dbvtNode)
	{
		WakeUp();
		if (m_scene && m_dbvtNodeInTree)
			m_scene->GetDbvt()->Remove(m_dbvtNode);
		delete m_dbvtNode;
		m_dbvtNode = nullptr;
		m_dbvtNodeInTree = false;
		m_dbvtUpdated = true;
	}
}
//----------------------------------------------------------------------------//
void Node::_InvalidateDbvtNode(void)
{
	if (m_dbvtUpdated && m_dbvtNode)
	{
		WakeUp();
		m_dbvtUpdated = false;
	}
}
//----------------------------------------------------------------------------//
void Node::_UpdateDbvtNode(void)
{
	ASSERT(m_dbvtUpdated == false);

	if (m_dbvtNode)
	{
		if (!m_worldBBoxUpdated)
		{
			_GetWorldBBox(m_dbvtNode->box);
			m_worldBBoxUpdated = true;
		}
		if (m_scene)
		{
			if (m_dbvtNode->box.IsFinite())
			{
				if (m_dbvtNodeInTree)
				{
					m_scene->GetDbvt()->Update(m_dbvtNode);
				}
				else
				{
					m_scene->GetDbvt()->Insert(m_dbvtNode);
					m_dbvtNodeInTree = true;
				}
			}
			else if (m_dbvtNodeInTree)
			{
				m_scene->GetDbvt()->Remove(m_dbvtNode);
				m_dbvtNodeInTree = false;
			}
		}
	}
	m_dbvtUpdated = true;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Entity
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Entity::Entity(void) :
	m_prevEntity(nullptr),
	m_nextEntity(nullptr)
{
	m_typeMask |= NT_Entity;
}
//----------------------------------------------------------------------------//
Entity::~Entity(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Camera::Camera(void) :
	m_orthographic(false),
	m_fov(QUAD_PI), // 45
	m_near(1.0f),
	m_far(1000),
	m_zoom(1)
{
	m_typeMask |= NT_Camera;
}
//----------------------------------------------------------------------------//
Camera::~Camera(void)
{
}
//----------------------------------------------------------------------------//
void Camera::GetParams(float _x, float _y, float _w, float _h, UCamera& _params, Frustum& _frustum)
{
	const Mat44& _worldTM = GetWorldTransform();

	_params.CameraPos = _worldTM.GetTranslation();
	_params.ViewMat.CreateTransform(_worldTM.GetTranslation(), _worldTM.GetRotation(), 1).Inverse();
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
	//_proj.m22 = 0;
	//_proj.m32 = -1;
	_params.InvViewProjMat = (_proj * _params.ViewMat).Inverse();

	// depth params (near, far, C=constant, FC = 1.0/log(far*C + 1))
	// the camera depth is: (exp(depth/FC)-1.0)/C
	_params.Depth.x = m_near;
	_params.Depth.y = m_far;
	_params.Depth.z = m_near;
	_params.Depth.w = 1 / Log(m_far * _params.Depth.z + 1);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Light
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Light::Light(void)
{
	m_typeMask |= NT_Light;
}
//----------------------------------------------------------------------------//
Light::~Light(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// PointLight
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
PointLight::PointLight(void)
{
	m_typeMask |= NT_PointLight;
}
//----------------------------------------------------------------------------//
PointLight::~PointLight(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// SpotLight
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
SpotLight::SpotLight(void)
{
	m_typeMask |= NT_SpotLight;
}
//----------------------------------------------------------------------------//
SpotLight::~SpotLight(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// DirectionalLight
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
DirectionalLight::DirectionalLight(void) :
	m_prevLight(nullptr),
	m_nextLight(nullptr)
{
	m_typeMask |= NT_DirectionalLight;
}
//----------------------------------------------------------------------------//
DirectionalLight::~DirectionalLight(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// RenderNode
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
RenderNode::RenderNode(void)
{
	m_typeMask |= NT_RenderNode;
}
//----------------------------------------------------------------------------//
RenderNode::~RenderNode(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Model
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Model::Model(void)
{
	m_typeMask |= NT_Model;
}
//----------------------------------------------------------------------------//
Model::~Model(void)
{
}
//----------------------------------------------------------------------------//
void Model::SetMesh(Mesh* _mesh)
{
	m_mesh = _mesh;
	if (m_mesh)
	{
		_CreateDbvtNode();
		m_materialParams = m_mesh->GetMaterialParams();
	}
	else
	{
		_DeleteDbvtNode();
	}
}
//----------------------------------------------------------------------------//
void Model::GetRenderItems(Array<RenderItem>& _items)
{
	if (m_mesh)
	{
		RenderItem _item;
		m_mesh->GetItem(_item);
		_item.node = this;
		if (m_material)
			_item.material = m_material;

		_items.Push(_item);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// StaticModel
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
StaticModel::StaticModel(void)
{
	m_typeMask |= NT_StaticModel;
}
//----------------------------------------------------------------------------//
StaticModel::~StaticModel(void)
{
}
//----------------------------------------------------------------------------//
void StaticModel::_GetWorldBBox(AlignedBox& _bbox)
{
	if (m_mesh)
		_bbox = m_mesh->GetBBox() * GetWorldTransform();
	else
		_bbox.Reset();
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// SkinnedModel
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
SkinnedModel::SkinnedModel(void)
{
	m_typeMask |= NT_SkinnedModel;
}
//----------------------------------------------------------------------------//
SkinnedModel::~SkinnedModel(void)
{
}
//----------------------------------------------------------------------------//
void SkinnedModel::_GetWorldBBox(AlignedBox& _bbox)
{
	// get from bones
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// SkyDome
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
SkyDome::SkyDome(void) :
	m_prevSky(nullptr),
	m_nextSky(nullptr)
{
	m_typeMask |= NT_SkyDome;
}
//----------------------------------------------------------------------------//
SkyDome::~SkyDome(void)
{
}
//----------------------------------------------------------------------------//
void SkyDome::Create(float _radius)
{
	GeometryPtr _geom = new Geometry();
	_geom->CreateSphere(_radius, 32, 16);
	m_mesh = _geom->CreateVertexArray();
}
//----------------------------------------------------------------------------//
void SkyDome::GetRenderItems(Array<RenderItem>& _items)
{
	RenderItem _item;
	_item.node = this;
	_item.data = m_mesh;
	_item.material = m_material;
	_item.start = 0;
	_item.count = m_mesh->GetIndexCount();
	_items.Push(_item);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Terrain
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Terrain::Terrain(void) :
	m_scale(VEC3_ONE),
	m_numSectors(1),
	m_prevTerrain(nullptr),
	m_nextTerrain(nullptr)
{
	m_typeMask |= NT_Terrain;
}
//----------------------------------------------------------------------------//
Terrain::~Terrain(void)
{
}
//----------------------------------------------------------------------------//
float Terrain::GetHeight(float _x, float _z)
{
	ASSERT(m_heightmap != nullptr);

	Vec3 _local = WorldToLocal({ _x, 0, _z });
	Vec3 _point = _local / m_scale;
	_point.x += 0.5f;
	_point.z += 0.5f;
	return m_scale.y * m_heightmap->Sample({_point.x, _point.z}, ISF_Clamp | ISF_Sparse | ISF_Linear | ISF_Triangle, (float)m_numSectors).x;
}
//----------------------------------------------------------------------------//
void Terrain::Create(Image* _heightmap, float _yScale, float _xzScale, uint _numSectors, const Vec2& _texcoordScale)
{
	ASSERT(_heightmap != nullptr);
	ASSERT(_numSectors > 0);

	if (!_numSectors)
		_numSectors = 1;
	m_heightmap = _heightmap;
	m_scale.Set(_xzScale, _yScale, _xzScale);
	m_numSectors = _numSectors;

	Geometry _geom;
	_geom.CreateGridXZ(_numSectors, m_scale.x / _numSectors);
	Vertex* _v = _geom.Vertices().Ptr();
	for (uint i = 0; i < _geom.GetVertexCount(); ++i)
		_v[i].position.y = m_scale.y * (_heightmap->Sample(_v[i].GetTexCoord(), ISF_Clamp | ISF_Linear).x);
	_geom.ComputeNormals();
	_geom.ScaleTexCoord(_texcoordScale);
	m_mesh = _geom.CreateVertexArray();
	m_localBBox = _geom.ComputeBBox();

	// temp
	m_texture = new Texture(TT_2D, PF_RGB8, true);
	m_texture->Realloc(_heightmap->Width(), _heightmap->Height());
	m_texture->Write(_heightmap->Format(), _heightmap->Data());
	m_texture->GenerateMipmap();
	m_material = new Material;
	m_material->SetTexture(m_texture);
	m_material->SetSampler(gGraphics->AddSampler(Sampler::Desc(TW_Repeat, TF_Trilinear)));

	_CreateDbvtNode();
}
//----------------------------------------------------------------------------//
void Terrain::GetRenderItems(Array<RenderItem>& _items)
{
	RenderItem _item;
	_item.node = this;
	_item.data = m_mesh;
	_item.material = m_material;
	_item.start = 0;
	_item.count = m_mesh->GetIndexCount();
	_items.Push(_item);
}
//----------------------------------------------------------------------------//
void Terrain::_GetWorldBBox(AlignedBox& _bbox)
{
	_bbox = m_localBBox * GetWorldTransform();
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Vegetation
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Vegetation::Vegetation(void) :
	m_prevVegetation(nullptr),
	m_nextVegetation(nullptr)
{
	m_typeMask |= NT_Vegetation;
}
//----------------------------------------------------------------------------//
Vegetation::~Vegetation(void)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Scene::Scene(void) :
	m_frame(0),
	m_rootNodes(nullptr),
	m_activeNodes(nullptr),
	m_numNodes(0),
	m_numActiveNodes(0),
	m_numRootNodes(0),
	m_directionalLights(nullptr),
	m_terrain(nullptr),
	m_sky(nullptr),
	m_vegetation(nullptr),
	m_entities(nullptr)
{
}
//----------------------------------------------------------------------------//
Scene::~Scene(void)
{
	m_activeCamera = nullptr;

	while (m_rootNodes)
		m_rootNodes->SetScene(nullptr);

	// ...
}
//----------------------------------------------------------------------------//
void Scene::UpdateDbvt(void)
{
	for (Node* i = m_activeNodes; i; i = i->m_nextActive)
	{
		if (!i->m_dbvtUpdated)
			i->_UpdateDbvtNode();
	}
}
//----------------------------------------------------------------------------//
void Scene::UpdateTransform(void)
{
	for (Node* i = m_activeNodes; i; i = i->m_nextActive)
	{
		if (!i->m_transformUpdated)
			i->_UpdateTransform();
	}
}
//----------------------------------------------------------------------------//
void Scene::Update(float _seconds)
{
	FrameInfo _frame;
	_frame.id = ++m_frame;
	_frame.time = _seconds;

	for (Entity* i = m_entities; i; )
	{
		EntityPtr e = i;
		i = i->m_nextEntity;
		if(e->IsActive() && e->IsEnabled())
			e->_LogicUpdate(_frame);
	}

	for (Node* i = m_activeNodes; i; i = i->m_nextActive)
	{
		i->Update(_frame);
	}

	for (Node* i = m_activeNodes; i; )
	{
		NodePtr n = i;
		i = i->m_nextActive;
		n->PostUpdate(_frame);
	}

	//printf("active nodes: %d --> %d (%f) ", _active, m_numActiveNodes, m_activeNodes->m_activeTime);
}
//----------------------------------------------------------------------------//
void Scene::GetNodes(Array<Node*>& _nodes, const Vec3& _center, float _radius, uint _typeMask, uint _tags)
{
	// TODO
}
//----------------------------------------------------------------------------//
void Scene::_AddActiveNode(Node* _node)
{
	Link(m_activeNodes, _node, _node->m_prevActive);
	++m_numActiveNodes;
}
//----------------------------------------------------------------------------//
void Scene::_RemoveActiveNode(Node* _node)
{
	Unlink(m_activeNodes, _node, _node->m_prevActive);
	--m_numActiveNodes;
}
//----------------------------------------------------------------------------//
void Scene::_AddRootNode(Node* _node)
{
	Link(m_rootNodes, _node, _node->m_prev);
	++m_numRootNodes;
}
//----------------------------------------------------------------------------//
void Scene::_RemoveRootNode(Node* _node)
{
	Unlink(m_rootNodes, _node, _node->m_prev);
	--m_numRootNodes;
}
//----------------------------------------------------------------------------//
void Scene::_AddNode(Node* _node)
{
	// activate
	_AddActiveNode(_node);
	_node->m_activeTime = 0;
	_node->m_active = true;

	if (!_node->m_parent)
		_AddRootNode(_node);

	_node->m_id = _NewID(_node);

	if (_node->GetTypeMask() & NT_Entity)
		Link(m_entities, static_cast<Entity*>(_node), static_cast<Entity*>(_node)->m_prevEntity);
	else if (_node->GetTypeMask() & NT_DirectionalLight)
		Link(m_directionalLights, static_cast<DirectionalLight*>(_node), static_cast<DirectionalLight*>(_node)->m_prevLight);
	else if (_node->GetTypeMask() & NT_Terrain)
		Link(m_terrain, static_cast<Terrain*>(_node), static_cast<Terrain*>(_node)->m_prevTerrain);
	else if (_node->GetTypeMask() & NT_SkyDome)
		Link(m_sky, static_cast<SkyDome*>(_node), static_cast<SkyDome*>(_node)->m_prevSky);
	else if (_node->GetTypeMask() & NT_Vegetation)
		Link(m_vegetation, static_cast<Vegetation*>(_node), static_cast<Vegetation*>(_node)->m_prevVegetation);

	_node->m_dbvtUpdated = false;
	_node->_UpdateDbvtNode();

	++m_numNodes;
}
//----------------------------------------------------------------------------//
void Scene::_RemoveNode(Node* _node)
{
	if (_node->m_active)
		_RemoveActiveNode(_node);
	if (!_node->m_parent)
		_RemoveRootNode(_node);

	_FreeID(_node->m_id);

	if (_node->m_dbvtNode && _node->m_dbvtNodeInTree)
	{
		m_dbvt.Remove(_node->m_dbvtNode);
		_node->m_dbvtNodeInTree = false;
	}

	if (_node->GetTypeMask() & NT_Entity)
		Unlink(m_entities, static_cast<Entity*>(_node), static_cast<Entity*>(_node)->m_prevEntity);
	else if (_node->GetTypeMask() & NT_DirectionalLight)
		Unlink(m_directionalLights, static_cast<DirectionalLight*>(_node), static_cast<DirectionalLight*>(_node)->m_prevLight);
	else if (_node->GetTypeMask() & NT_Terrain)
		Unlink(m_terrain, static_cast<Terrain*>(_node), static_cast<Terrain*>(_node)->m_prevTerrain);
	else if (_node->GetTypeMask() & NT_SkyDome)
		Unlink(m_sky, static_cast<SkyDome*>(_node), static_cast<SkyDome*>(_node)->m_prevSky);
	else if (_node->GetTypeMask() & NT_Vegetation)
		Unlink(m_vegetation, static_cast<Vegetation*>(_node), static_cast<Vegetation*>(_node)->m_prevVegetation);

	--m_numNodes;
}
//----------------------------------------------------------------------------//
uint Scene::_NewID(Node* _node)
{
	uint _id;
	if (m_freeIds.Size())
	{
		_id = m_freeIds.Top();
		m_freeIds.Pop();
	}
	else
	{
		_id = m_nodes.Size();
		m_nodes.Upsize(1, true);
	}
	m_nodes[_id] = _node;
	return _id;
}
//----------------------------------------------------------------------------//
void Scene::_FreeID(uint _id)
{
	ASSERT(_id < m_nodes.Size());
	ASSERT(m_nodes[_id] != nullptr);
	m_nodes[_id] = nullptr;
	m_freeIds.Push(_id);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
