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
void Transform::SetWorldMatrix(const Mat44& _m, bool _orderIndependent)
{
	m_worldMatrix = _m;
	_UpdateLocalFromWorld();
	m_updateLocalFromWorld = _orderIndependent;
	_Invalidate();
}
//----------------------------------------------------------------------------//
void Transform::SetLocalPosition(const Vec3& _position)
{
	m_localPosition = _position;
	_Invalidate();
}
//----------------------------------------------------------------------------//
const Vec3& Transform::GetLocalPosition(void)
{
	return m_localPosition;
}
//----------------------------------------------------------------------------//
void Transform::SetLocalRotation(const Quat& _rotation)
{
	m_localRotation = _rotation;
	_Invalidate();
}
//----------------------------------------------------------------------------//
const Quat& Transform::GetLocalRotation(void)
{
	return m_localRotation;
}
//----------------------------------------------------------------------------//
void Transform::SetLocalScale(const Vec3& _scale)
{
	m_localScale = _scale;
	_Invalidate();
}
//----------------------------------------------------------------------------//
const Vec3& Transform::GetLocalScale(void)
{
	return m_localScale;
}
//----------------------------------------------------------------------------//
void Transform::SetWorldPosition(const Vec3& _position)
{
	m_localPosition = _position;
	if (m_inheritPosition && m_parent)
		m_localPosition = m_parent->GetWorldMatrix().Copy().Inverse().Transform(m_localPosition);
	_Invalidate();
}
//----------------------------------------------------------------------------//
const Vec3& Transform::GetWorldPosition(void)
{
	if (!m_updated)
		_UpdateMatrix();
	return m_worldMatrix.GetTranslation();
}
//----------------------------------------------------------------------------//
void Transform::SetWorldRotation(const Quat& _rotation)
{
	m_localRotation = _rotation;
	if (m_inheritRotation && m_parent)
		m_localRotation = m_parent->GetWorldMatrix().Copy().Inverse().GetRotation() * m_localRotation;
	_Invalidate();
}
//----------------------------------------------------------------------------//
const Quat& Transform::GetWorldRotation(void)
{
	if (!m_updated)
		_UpdateMatrix();
	return m_worldMatrix.GetRotation();
}
//----------------------------------------------------------------------------//
void Transform::SetWorldScale(const Vec3& _scale)
{
	m_localScale = _scale;
	if (m_inheritScale && m_parent)
		m_localScale /= m_parent->GetWorldMatrix().GetScale();
	_Invalidate();
}
//----------------------------------------------------------------------------//
const Vec3& Transform::GetWorldScale(void)
{
	if (!m_updated)
		_UpdateMatrix();
	return m_worldMatrix.GetScale();
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
void Transform::_UpdateRecursive(void)
{
	ASSERT(m_invalidated == true);

	if(!m_updated)
		_UpdateMatrix();

	m_invalidated = false;
	m_updateLocalFromWorld = false;

	for (Transform* i = m_child; i; i = i->m_next)
	{
		if (i->m_invalidated)
			i->_UpdateRecursive();
	}
}
//----------------------------------------------------------------------------//
void Transform::_UpdateMatrix(void)
{
	ASSERT(m_updated == false);
	
	m_updated = true;

	if (m_updateLocalFromWorld)
		_UpdateLocalFromWorld();
	else
		_UpdateWorldFromLocal();
}
//----------------------------------------------------------------------------//
void Transform::_UpdateLocalFromWorld(void)
{
	m_localPosition = m_worldMatrix.GetTranslation();
	m_localRotation = m_worldMatrix.GetRotation();
	m_localScale = m_worldMatrix.GetScale();

	if (m_parent)
	{
		Mat44 _m = m_parent->GetWorldMatrix().Copy().Inverse();
		if(m_inheritPosition)
			m_localPosition = _m.Transform(m_localPosition);
		if(m_inheritRotation)
			m_localRotation = _m.GetRotation() * m_localRotation;
		if(m_inheritScale)
			m_localScale /= m_parent->GetWorldMatrix().GetScale();
	}
}
//----------------------------------------------------------------------------//
void Transform::_UpdateWorldFromLocal(void)
{
	Vec3 _pos = m_localPosition;
	Quat _rot = m_localRotation;
	Vec3 _scl = m_localScale;

	if (m_parent)
	{
		const Mat44& _m = m_parent->GetWorldMatrix();
		if (m_inheritPosition)
			_pos = _m.Transform(m_localPosition);
		if (m_inheritRotation)
			_rot = _m.GetRotation() * m_localRotation;
		if (m_inheritScale)
			_scl *= m_parent->GetWorldMatrix().GetScale();
	}

	m_worldMatrix.CreateTransform(_pos, _rot, _scl);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// TransformSystem
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
TransformSystem::TransformSystem(Scene* _scene) :
	SceneSystem(_scene),
	m_root(nullptr),
	m_locked(false)
{
}
//----------------------------------------------------------------------------//
TransformSystem::~TransformSystem(void)
{
	while (m_root)
		m_root->_SetScene(nullptr);
}
//----------------------------------------------------------------------------//
void TransformSystem::Update(float _dt)
{
	m_locked = true;
	for (uint i = 0; i < m_updates.Size(); ++i)
	{
		Transform* _t = m_updates[i];
		if (_t->m_invalidated)
			_t->_UpdateRecursive();
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
