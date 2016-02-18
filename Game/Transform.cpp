#pragma once

#include "Transform.hpp"

//----------------------------------------------------------------------------//
// Transform
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Transform::Transform(void) :
	m_system(nullptr),
	m_parent(nullptr),
	m_prev(nullptr),
	m_next(nullptr),
	m_child(nullptr),
	m_localPosition(VEC3_ZERO),
	m_localRotation(QUAT_IDENTITY),
	m_localScale(VEC3_ONE),
	m_worldMatrix(MAT44_IDENTITY)
{
}
//----------------------------------------------------------------------------//
Transform::~Transform(void)
{
	DetachAllChildren();
}
//----------------------------------------------------------------------------//
void Transform::SetParent(Transform* _parent)
{
	if (m_parent == _parent)
		return;

	for (Transform* i = _parent; i; i = i->m_parent)
	{
		if (i == this) // child cannot be parent
			return;
	}

	// TODO: update matrix

	if (m_parent)
	{
		Unlink(m_parent->m_child, this, m_prev);
	}
	else if (m_system)
	{
		Unlink(m_system->_RootRef(), this, m_prev);
	}

	m_parent = _parent;

	if (m_parent)
	{
		Link(m_parent->m_child, this, m_prev);
	}
	else if (m_system)
	{
		Link(m_system->_RootRef(), this, m_prev);
	}
}
//----------------------------------------------------------------------------//
void Transform::DetachAllChildren(void)
{
	while (m_child)
		m_child->SetParent(nullptr);
}
//----------------------------------------------------------------------------//
void Transform::_SetScene(Scene* _scene)
{
	TransformSystem* _system = _scene ? _scene->GetTransformSystem() : nullptr;
	
	if (m_system == _system)
		return;

	if (m_system)
	{
		if(!m_parent)
			Unlink(m_system->_RootRef(), this, m_prev);
		m_system->_RemoveComponent(this);
	}

	m_system = _system;

	if (m_system)
	{
		if(m_parent)
			Link(m_system->_RootRef(), this, m_prev);
		m_system->_AddComponent(this);
	}
}
//----------------------------------------------------------------------------//
void Transform::_InvalidateMatrix(bool _fromParent, bool _recursive)
{
	if (m_updated)
	{

	}

	if (!m_invalidated)
	{
		//SendEvent(EE_TranformChanged);


	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// TransformSystem
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
TransformSystem::TransformSystem(Scene* _scene) :
	SceneSystem(_scene),
	m_locked(false)
{
}
//----------------------------------------------------------------------------//
TransformSystem::~TransformSystem(void)
{
}
//----------------------------------------------------------------------------//
void TransformSystem::_AddComponent(Transform* _transform)
{
	ASSERT(m_locked == false);
	if (_transform->m_invalidated)
	{
		m_updates.Push(_transform);
		//_transform->SendEvent(EE_TranformChanged);
	}
}
//----------------------------------------------------------------------------//
void TransformSystem::_RemoveComponent(Transform* _transform)
{
	ASSERT(m_locked == false);
	if (_transform->m_invalidated)
		m_updates.Remove(_transform);
}
//----------------------------------------------------------------------------//
void TransformSystem::_AddUpdate(Transform* _transform)
{
	if (!m_locked && !_transform->m_invalidated)
	{
		m_updates.Push(_transform);
		_transform->m_invalidated = true;
		//_transform->SendEvent(EE_TranformChanged);

		//Entity* _e = _transform->GetEntity();
		//if (_e) _e->SendEvent(EE_TranformChanged);
	}
}
//----------------------------------------------------------------------------//
void TransformSystem::_RemoveUpdate(Transform* _transform)
{
	ASSERT(m_locked == false);
	if (_transform->m_invalidated)
	{
		m_updates.Remove(_transform);
		_transform->m_invalidated = false;
	}
}
//----------------------------------------------------------------------------//
void TransformSystem::_Update(void)
{
	m_locked = true;
	for (uint i = 0; i < m_updates.Size(); ++i)
	{
		Transform* _t = m_updates[i];
		//_t->UpdateWorldMatrix();
	}
	m_locked = false;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
