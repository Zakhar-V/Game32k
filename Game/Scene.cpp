#pragma once

#include "Scene.hpp"
#include "Physics.hpp"
#include "Render.hpp"

//----------------------------------------------------------------------------//
// Node
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Node::Node(void) :
	m_id(0),
	m_layerMask(0xffff),
	m_tags(0),
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
	m_active(false),
	m_transformUpdated(true),
	m_transformChanged(false),
	m_lastUpdateFrame(0),
	m_lastViewFrame(0),
	m_updatePolicy(UP_Auto),
	m_sleepingThreshold(0.2f), // 200 ms = ~12 frames on 60 fps
	m_activeTime(0)
{
}
//----------------------------------------------------------------------------//
Node::~Node(void)
{
	DetachChildren(true);
	RemoveAllBehaviors();
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
		Unlink(m_parent->m_child, this, m_prev);
	else if (m_scene)
		m_scene->_RemoveRootNode(this);

	m_parent = _parent;

	if (m_parent)
	{
		Link(m_parent->m_child, this, m_prev);

		if (m_parent->m_scene)
			SetScene(m_parent->m_scene);
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
		if (i->IsClass(_class) && _index--)
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
	if (m_activeTime >= m_sleepingThreshold)
	{
		m_activeTime = 0;

		if (m_active && m_scene && m_updatePolicy != UP_Always && m_behaviorAllowSleep)
		{
			m_scene->_RemoveActiveNode(this);
			m_active = false;
		}
	}
	/*
	if(m_worldBoundsChanged || m_transformChanged)
	_UpdateDbvtNode();
	*/
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
		for (Node* i = m_child; i; i = i->m_next) // invalidate and activate all children
			i->_InvalidateTransform();
	}
}
//----------------------------------------------------------------------------//
void Node::_UpdateTransform(void)
{
	if (!m_transformUpdated)
	{
		m_worldTransform.CreateTransform(m_localPosition, m_localRotation, m_localScale);
		if (m_parent)
			m_worldTransform = m_parent->GetWorldTransform() * m_worldTransform;
		m_worldRotation = m_worldTransform.GetRotation();
		m_transformUpdated = true;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Camera::Camera(void) :
	m_orthographic(false),
	m_fov(QUAD_PI), // 45
	m_near(1.01f),
	m_far(1000),
	m_zoom(1)
{
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
// Scene
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Scene::Scene(void) :
	m_frame(0),
	m_rootNodes(nullptr),
	m_activeNodes(nullptr),
	m_numNodes(0),
	m_numActiveNodes(0),
	m_numRootNodes(0)
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
void Scene::Update(float _seconds)
{
	FrameInfo _frame;
	_frame.id = ++m_frame;
	_frame.time = _seconds;

	uint _active = m_numActiveNodes;
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

	//printf("active nodes: %d --> %d (%f)\n", _active, m_numActiveNodes, m_activeNodes->m_activeTime);
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

	//if (_node->m_dbvtNode)
	//	m_dbvt->AddNode(_node->m_dbvtNode);

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

	// ...

	//if (_node->m_dbvtNode)
	//	m_dbvt->RemoveNode(_node->m_dbvtNode);

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
