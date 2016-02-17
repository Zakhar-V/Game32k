#pragma once

#include "Scene.hpp"
#include "Transform.hpp"

//----------------------------------------------------------------------------//
// Component
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Component::Component(void) :
	m_entity(nullptr),
	m_prev(nullptr),
	m_next(nullptr)
{
}
//----------------------------------------------------------------------------//
Component::~Component(void)
{
}
//----------------------------------------------------------------------------//
Scene* Component::GetScene(void)
{
	return m_entity ? m_entity->GetScene() : nullptr;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Entity 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Entity::Entity(void) :
	m_scene(nullptr),
	m_parent(nullptr),
	m_prev(nullptr),
	m_next(nullptr),
	m_child(nullptr),
	m_numComponents(0)
{
	memset(m_components, 0, sizeof(m_components));
}
//----------------------------------------------------------------------------//
Entity::~Entity(void)
{
	DetachAllChildren(true);
}
//----------------------------------------------------------------------------//
void Entity::SetParent(Entity* _parent, bool _inheritTransform)
{
	if (m_parent == _parent)
		return;

	if (_parent && _parent->m_scene != m_scene)
		return;

	if (_parent)
	{
		for (Entity* i = this; i; i = i->m_parent)
		{
			if (i == this)
				return;
		}
	}

	if (!_parent && !m_scene)
		AddRef();

	if (m_parent)
	{
		Unlink(m_parent->m_child, this, m_prev);
	}
	else if (m_scene)
	{
		Unlink(m_scene->_RootEntityRef(), this, m_prev);
	}

	m_parent = _parent;

	if (m_parent)
	{
		Link(m_parent->m_child, this, m_prev);

		if (m_parent->m_scene && m_parent->m_scene != m_scene)
			_SetScene(m_parent->m_scene);
	}
	else if (m_scene)
	{
		Link(m_scene->_RootEntityRef(), this, m_prev);
	}

	/*if (_inheritTransform)
	{
		Transform* _parentTransform = m_parent ? m_parent->GetComponent<Transform>() : nullptr;

	}*/

	if (!_parent && !m_scene)
		Release();
}
//----------------------------------------------------------------------------//
void Entity::DetachAllChildren(bool _remove)
{
	for (Entity *n, *i = m_child; i;)
	{
		n = i;
		i = i->m_next;
		if (_remove && !i->m_persistent)
			n->RemoveThis();
		else
			n->SetParent(nullptr);
	}
}
//----------------------------------------------------------------------------//
void Entity::RemoveThis(bool _force)
{
	EntityPtr _addref(this);
	SetParent(nullptr);
	if(_force || !m_persistent)
		_SetScene(nullptr);
}
//----------------------------------------------------------------------------//
Component* Entity::GetComponent(ComponentType _type, uint _index)
{
	ASSERT(_type < CT_MaxTypes);
	Component* _c = m_components[_type];
	while (_c && _index--)
		_c = _c->m_next;
	return _c;
}
//----------------------------------------------------------------------------//
void Entity::AttachComponent(Component* _c)
{
	if (!_c || _c->m_entity == this)
		return;

	if (_c->GetScene() != m_scene)
		return;

	ComponentType _type = _c->GetComponentType();

	if (m_components[_type] && m_components[_type]->IsSingleComponent())
		return;

	_c->AddRef();
	if (_c->m_entity)
		_c->m_entity->DetachComponent(_c, _c->m_entity->m_scene == m_scene);

	Link(m_components[_type], _c, _c->m_prev);
	_c->_SetEntity(this);
	_c->_SetScene(m_scene);
}
//----------------------------------------------------------------------------//
void Entity::DetachComponent(Component* _c, bool _removeFromScene)
{
	if (_c && _c->m_entity == this)
	{
		ComponentType _type = _c->GetComponentType();
		//Unlink(m_components[_c->GetComponentType()])
	}
}
//----------------------------------------------------------------------------//
void Entity::_SetScene(Scene* _scene)
{
	if (!m_parent && !_scene)
		AddRef();

	Scene* _oldScene = m_scene;

	if (m_scene != _scene)
	{
		ASSERT(m_scene == nullptr || _scene == nullptr);

		// remove from scene
		if (m_scene)
		{
			//m_scene->_FreeID(m_id);
			if (!m_parent)
				Unlink(m_scene->_RootEntityRef(), this, m_prev);
		}

		// notify components
		for (uint i = 0; i < CT_MaxTypes; ++i)
		{
			for (Component* c = m_components[i]; c; c = c->GetNextComponent())
			{
				c->_SetScene(m_scene);
			}
		}

		m_scene = _scene;

		// add to scene
		if (m_scene)
		{
			//m_id = m_scene->_NewID(this);
			if (!m_parent)
				Link(m_scene->_RootEntityRef(), this, m_prev);
		}
	}

	// notify children
	if (_oldScene != m_scene || _scene == nullptr)
	{
		for (Entity* i = m_child; i; i = i->m_next)
		{
			i->_SetScene(m_scene);
		}
	}

	if (!m_parent && !_scene)
		Release();
}
//----------------------------------------------------------------------------//
void _ChangeScene(Scene* _scene)
{
	
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// SceneSystem
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void SceneSystem::AddComponent(Component* _c)
{
}
//----------------------------------------------------------------------------//
void SceneSystem::RemoveComponent(Component* _c)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Scene::Scene(void) :
	m_rootEntity(nullptr)
{
}
//----------------------------------------------------------------------------//
Scene::~Scene(void)
{
	while (m_rootEntity)
		m_rootEntity->RemoveThis(true);

	// ...
}
//----------------------------------------------------------------------------//
void Scene::AddEntity(Entity* _entity)
{
	if (_entity && (!_entity->GetParent() || _entity->GetParent()->GetScene() == this))
		_entity->_SetScene(this);
}
//----------------------------------------------------------------------------//
void Scene::RemoveEntity(Entity* _entity)
{
	if (_entity && _entity->GetScene() == this && !_entity->GetParent())
		_entity->RemoveThis(true);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
