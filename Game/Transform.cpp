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
	m_worldPosition(VEC3_ZERO),
	m_worldRotation(QUAT_IDENTITY),
	m_worldScale(VEC3_ONE),
	m_worldMatrix(MAT44_IDENTITY),
	m_inheritPosition(true),
	m_inheritRotation(true),
	m_inheritScale(true),
	m_updated(true),
	m_invalidated(false),
	m_changed(false),
	m_updateLocalFromWorld(false)
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
const Mat44& Transform::GetWorldMatrix(void)
{
	if (!m_updated)
		_UpdateMatrix();
	return m_worldMatrix;
}
//----------------------------------------------------------------------------//
void Transform::SetWorldMatrix(const Mat44& _matrix)
{
	_Invalidate();
	m_worldMatrix = _matrix;
	m_updateLocalFromWorld = true;
}
//----------------------------------------------------------------------------//
void Transform::SetPosition(const Vec3& _position, TransformSpace _space)
{

}
//----------------------------------------------------------------------------//
Vec3 Transform::GetPosition(TransformSpace _space)
{
	return VEC3_ZERO;
}
//----------------------------------------------------------------------------//
void Transform::SetRotation(const Quat& _rotation, TransformSpace _space)
{

}
//----------------------------------------------------------------------------//
Quat Transform::GetRotation(TransformSpace _space)
{
	return QUAT_IDENTITY;
}
//----------------------------------------------------------------------------//
void Transform::SetDirection(const Vec3& _direction, TransformSpace _space)
{

}
//----------------------------------------------------------------------------//
Vec3 Transform::GetDirection(TransformSpace _space)
{
	return VEC3_ZERO;
}
//----------------------------------------------------------------------------//
void Transform::SetScale(const Vec3& _scale, TransformSpace _space)
{

}
//----------------------------------------------------------------------------//
Vec3 Transform::GetScale(TransformSpace _space)
{
	return VEC3_ONE;
}
//----------------------------------------------------------------------------//
void Transform::_SetScene(Scene* _scene)
{
	TransformSystem* _system = _scene ? _scene->GetTransformSystem() : nullptr;
	
	if (m_system == _system)
		return;

	if (m_parent && m_parent->m_system != _system)
		return;

	if (m_system)
	{
		if(!m_parent)
			Unlink(m_system->_RootRef(), this, m_prev);

		if(m_invalidated)
			m_system->_RemoveUpdate(this);
	}

	m_system = _system;

	if (m_system)
	{
		if(m_parent)
			Link(m_system->_RootRef(), this, m_prev);

		if (m_invalidated)
			m_system->_AddUpdate(this);
	}
}
//----------------------------------------------------------------------------//
void Transform::_Invalidate(bool _fromChild)
{
	if (!m_invalidated)
	{
		if (m_system)
			m_system->_AddUpdate(this);
		m_invalidated = true;
	}

	if (!_fromChild)
	{
		if (!m_changed)
		{
			m_changed = true;
			SendEvent(ET_TransformChanged);
		}

		m_updated = false;
		for (Transform* i = m_child; i; i = i->m_next)
		{
			if (i->m_updated)
				i->_Invalidate(false);
		}
	}
	else if (m_parent && !m_parent->m_invalidated)
	{
		m_parent->_Invalidate(true);
	}
}
//----------------------------------------------------------------------------//
void Transform::_UpdateMatrix(void)
{
	ASSERT(m_updated == false);
	
	m_updated = true;
	m_worldMatrix.CreateTransform(m_worldPosition, m_worldRotation, m_worldScale);

	/*if (m_updateLocalFromWorld)
	{
		m_localPosition = m_worldMatrix.GetTranslation();
		m_localRotation = m_worldMatrix.GetRotation();
		m_localScale = m_worldMatrix.GetScale();

		if (m_parent)
		{
			Mat44 _m = m_parent->GetWorldMatrix().Copy().Inverse();
			m_localPosition = _m.Transform(m_localPosition);
			m_localRotation = _m.GetRotation() * m_localRotation;
			m_localScale /= m_parent->GetWorldMatrix().GetScale();
		}
	}
	else
	{
		m_worldMatrix.CreateTransform(m_localPosition, m_localRotation, m_localScale);
		if (m_parent)
			m_worldMatrix = m_parent->GetWorldMatrix() * m_worldMatrix;
	}*/
}
//----------------------------------------------------------------------------//
void Transform::_Update(void)
{
	ASSERT(m_invalidated == true);

	if(!m_updated)
		_UpdateMatrix();

	m_invalidated = false;
	m_updateLocalFromWorld = false;

	for (Transform* i = m_child; i; i = i->m_next)
	{
		if (i->m_invalidated)
			i->_Update();
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
void TransformSystem::Update(float _dt)
{
	m_locked = true;
	for (uint i = 0; i < m_updates.Size(); ++i)
	{
		Transform* _t = m_updates[i];
		if (_t->m_invalidated)
			_t->_Update();
	}
	m_updates.Clear();
	m_locked = false;
}
//----------------------------------------------------------------------------//
void TransformSystem::_AddUpdate(Transform* _transform)
{
	if (!m_locked)
		m_updates.Push(_transform);
}
//----------------------------------------------------------------------------//
void TransformSystem::_RemoveUpdate(Transform* _transform)
{
	ASSERT(m_locked == false);
	m_updates.Remove(_transform);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
