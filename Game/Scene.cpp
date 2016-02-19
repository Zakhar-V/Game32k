#pragma once

#include "Scene.hpp"
#include "Transform.hpp"
#include "Physics.hpp"
#include "Render.hpp"

//----------------------------------------------------------------------------//
// Component
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Component::Component(void) :
	m_entity(nullptr),
	m_prev(nullptr),
	m_next(nullptr),
	m_eventMask(0)
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
Component* Component::GetEntityComponent(ComponentType _type, uint _index)
{
	return m_entity ? m_entity->GetComponent(_type, _index) : nullptr;
}
//----------------------------------------------------------------------------//
void Component::SendEvent(uint _type, void* _arg)
{
	if (m_entity)
		m_entity->SendEvent(_type, this, _arg);
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
	m_removed(false),
	m_manualTransformHierarchy(false)
{
	memset(m_components, 0, sizeof(m_components));
}
//----------------------------------------------------------------------------//
Entity::~Entity(void)
{
	RemoveAllComponents();
	DetachAllChildren(true);
}
//----------------------------------------------------------------------------//
bool Entity::SetScene(Scene* _scene)
{
	if (m_scene == _scene)
		return true;

	if (m_parent && m_parent->m_scene != _scene)
		return false;

	if (!m_parent && !m_scene)
		AddRef();

	if (m_scene)
	{
		//m_scene->_FreeID(m_id);
		if (!m_parent)
			Unlink(m_scene->_RootEntityRef(), this, m_prev);
	}

	m_scene = _scene;

	if (m_scene)
	{
		//m_id = m_scene->_NewID(this);
		if (!m_parent)
			Link(m_scene->_RootEntityRef(), this, m_prev);
	}

	for (uint i = 0; i < CT_MaxTypes; ++i)
	{
		for (Component* c = m_components[i]; c; c = c->GetNextComponent())
		{
			c->_SetScene(m_scene);
		}
	}

	for (Entity* i = m_child; i; i = i->m_next)
	{
		i->SetScene(m_scene);
	}

	if (!m_parent && !m_scene)
		Release();

	return true;
}
//----------------------------------------------------------------------------//
bool Entity::SetParent(Entity* _parent)
{
	if (m_parent == _parent)
		return true;

	for (Entity* i = _parent; i; i = i->m_parent)
	{
		if (i == this)
			return false;
	}

	if (!m_parent && !m_scene)
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

		if (m_parent->m_scene)
			SetScene(m_parent->m_scene);
	}
	else if (m_scene)
	{
		Link(m_scene->_RootEntityRef(), this, m_prev);
	}

	if (!m_manualTransformHierarchy)
		SetParentTransform(m_parent);

	if (!m_parent && !m_scene)
		Release();

	return true;
}
//----------------------------------------------------------------------------//
void Entity::DetachAllChildren(bool _remove)
{
	for (Entity *n, *i = m_child; i;)
	{
		n = i;
		i = i->m_next;
		n->DetachThis(_remove);
	}
}
//----------------------------------------------------------------------------//
void Entity::DetachThis(bool _remove)
{
	EntityPtr _addref(this);
	SetParent(nullptr);
	if(_remove)
		SetScene(nullptr);
}
//----------------------------------------------------------------------------//
Entity* Entity::GetParentTransform(void)
{
	Transform* _t = GetComponent<Transform>();
	Transform* _parent = _t ? _t->GetParent() : nullptr;
	return _parent ? _parent->GetEntity() : nullptr;
}
//----------------------------------------------------------------------------//
void Entity::SetParentTransform(Entity* _parent)
{
	Transform* _t = GetComponent<Transform>();
	if (_t)
		_t->SetParent(_parent ? _parent->GetComponent<Transform>() : nullptr);
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
bool Entity::AddComponent(Component* _c)
{
	if (!_c)
		return false;

	if (_c->m_entity)
		return _c->m_entity == this;

	ComponentType _type = _c->GetComponentType();

	if (m_components[_type] && m_components[_type]->IsSingleComponent())
		return false;

	_c->AddRef();

	/*if (_c->m_entity)
	{
		Unlink(_c->m_entity->m_components[_type], _c, _c->m_prev);
		_c->Release();
	}*/

	Link(m_components[_type], _c, _c->m_prev);

	_c->_SetEntity(this);
	_c->_SetScene(m_scene);
	
	return true;
}
//----------------------------------------------------------------------------//
void Entity::RemoveComponent(Component* _c)
{
	if (_c && _c->m_entity == this)
	{
		ComponentType _type = _c->GetComponentType();
		Unlink(m_components[_type], _c, _c->m_prev);
		_c->_SetEntity(nullptr);
		_c->_SetScene(nullptr);
		_c->Release();
	}
}
//----------------------------------------------------------------------------//
void Entity::RemoveAllComponents(void)
{
	for (uint i = 0; i < CT_MaxTypes; ++i)
	{
		for (Component* c = m_components[i]; c; c = c->GetNextComponent())
		{
			c->_SetEntity(nullptr);
			c->_SetScene(nullptr);
		}
	}
}
//----------------------------------------------------------------------------//
void Entity::SendEvent(uint _type, Component* _sender, void* _arg)
{
	uint _mask = 1 << _type;
	for (uint i = 0; i < CT_MaxTypes; ++i)
	{
		for (Component* c = m_components[i]; c;)
		{
			Component* _next = c->GetNextComponent();
			if (c->GetEventMask() & _mask)
				c->OnEvent(_type, this, _sender, _arg);
			c = _next;
		}
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Scene::Scene(void) :
	m_rootEntity(nullptr)
{
	m_transformSystem = new TransformSystem(this);
	m_physicsWorld = new PhysicsWorld(this);
	m_renderWorld = new RenderWorld(this);
}
//----------------------------------------------------------------------------//
Scene::~Scene(void)
{
	while (m_rootEntity)
		m_rootEntity->DetachThis(true);

	// ...

	delete m_renderWorld;
	delete m_physicsWorld;
	delete m_transformSystem;
}
//----------------------------------------------------------------------------//
void Scene::Update(float _dt)
{
	//m_updateSystem->Update(_dt);
	//m_physicsWorld->Update(_dt);
	m_transformSystem->Update(_dt);

	/*
	1. анимация объектов (любое перемещение объектов)
	2. обновление иерархии трансформаций
	3. передача трансформаций в физический движок
	4. обновление физ. движка
	5. получение трансформаций из физ. движка
	6. обновление иерархии трансформаций
	*/
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
