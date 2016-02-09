#pragma once

#include "Scene.hpp"

//----------------------------------------------------------------------------//
// Node
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Node::Node(void) :
	m_scene(nullptr),
	m_entity(nullptr),
	m_layer(0),
	m_group(0),
	m_id(0),
	m_parent(nullptr),
	m_prev(nullptr),
	m_next(nullptr),
	m_child(nullptr),
	m_position(VEC3_ZERO),
	m_rotation(QUAT_IDENTITY),
	m_scale(VEC3_ONE),
	m_matrix(MAT44_IDENTITY),
	m_physics(nullptr),
	m_dbvtNode(nullptr)
{
}
//----------------------------------------------------------------------------//
Node::~Node(void)
{
	DetachAllChildren(true);
	// TODO: remove physics, dbvt ... ?
}
//----------------------------------------------------------------------------//
void Node::SetParent(Node* _parent)
{
	ASSERT(_parent != this);
	if (m_parent == _parent)
		return;

	if (!_parent && !m_scene)
		AddRef();

	if (m_parent)
	{
		_OnChangeParent(nullptr);
		_Unlink(m_parent->m_child);
	}
	else if(m_scene)
	{
		_Unlink(m_scene->_Root());
	}

	//TODO: update world TM

	m_parent = _parent;

	if (m_parent)
	{
		_Link(m_parent->m_child);
		_OnChangeParent(_parent);

		if (m_parent->m_scene && m_parent->m_scene != m_scene)
			_SetScene(m_parent->m_scene);
	}
	else if(m_scene)
	{
		_Link(m_scene->_Root());
	}

	if (!_parent && !m_scene)
		Release();
}
//----------------------------------------------------------------------------//
void Node::DetachAllChildren(bool _remove)
{
	for (Node *n, *i = m_child; i;)
	{
		n = i;
		i = i->m_next;
		if (_remove && !i->m_entity)
			n->RemoveThis();
		else
			n->SetParent(nullptr);
	}
}
//----------------------------------------------------------------------------//
void Node::RemoveThis(void)
{
	NodePtr _addref(this);
	SetParent(nullptr);
	_SetScene(nullptr);
}
//----------------------------------------------------------------------------//
void Node::_SetScene(Scene* _scene)
{
	if (!m_parent && !_scene)
		AddRef();

	Scene* _oldScene = m_scene;

	if (m_scene != _scene)
	{
		if (m_scene)
		{
			_OnChangeScene(nullptr);
			m_scene->_FreeID(m_id);
			if (!m_parent)
				_Unlink(m_scene->_Root());
		}

		m_scene = _scene;

		if (m_scene)
		{
			m_id = m_scene->_NewID(this);
			if (!m_parent)
				_Link(m_scene->_Root());
			_OnChangeScene(m_scene);
		}
	}

	if (_oldScene != m_scene || _scene == nullptr)
	{
		for (Node* i = m_child; i; i = i->m_next)
		{
			i->_SetScene(m_scene);
		}
	}

	if (!m_parent && !_scene)
		Release();
}
//----------------------------------------------------------------------------//
void Node::_Link(Node*& _list)
{
	ASSERT(m_prev == nullptr && m_next == nullptr && _list != this);

	m_next = _list;
	_list = this;
	if (m_next)
		m_next->m_prev = this;
}
//----------------------------------------------------------------------------//
void Node::_Unlink(Node*& _list)
{
	if (m_next)
		m_next->m_prev = m_prev;
	if (m_prev)
		m_prev->m_next = m_next;
	else
		_list = m_next;

	m_prev = nullptr;
	m_next = nullptr;
}
//----------------------------------------------------------------------------//
void Node::_UpdateLocalTM(void)
{
	if (m_parent)
	{

	}
	else
	{

	}
}
//----------------------------------------------------------------------------//
void Node::_UpdateWorldTM(void)
{

}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Scene::Scene(void) :
	m_rootNodes(nullptr)
{
	m_nodes.Push(nullptr); // 0
}
//----------------------------------------------------------------------------//
Scene::~Scene(void)
{
}
//----------------------------------------------------------------------------//
void Scene::AddNode(Node* _node)
{
	if (_node)
		_node->_SetScene(this);
}
//----------------------------------------------------------------------------//
uint Scene::_NewID(Node* _node)
{
	uint _id = 0;
	if (m_freeIds.Size())
	{
		_id = m_freeIds.Top();
		m_freeIds.Pop();
	}
	else
	{
		_id = m_nodes.Size();
		m_nodes.Push(nullptr);
	}
	m_nodes[_id] = _node;
	return _id;
}
//----------------------------------------------------------------------------//
void Scene::_FreeID(uint _id)
{
	ASSERT(_id < m_nodes.Size());
	m_nodes[_id] = nullptr;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
