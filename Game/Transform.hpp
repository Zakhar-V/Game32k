#pragma once

#include "Scene.hpp"

//----------------------------------------------------------------------------//
// Transform
//----------------------------------------------------------------------------//

class Transform : public Component
{
public:
	static ComponentType GetComponentTypeStatic(void) { return CT_Transform; }
	ComponentType GetComponentType(void) override { return CT_Transform; }
	bool IsSingleComponent(void) override { return true; }

protected:
	friend class TransformSystem;

	Vec3 m_localPosition;
	Quat m_localRotation;
	Vec3 m_localScale;
	Mat44 m_worldMatrix;

	TransformSystem* m_system;
	Transform* m_parent;
	Transform* m_prev;
	Transform* m_next;
	Transform* m_child;

	uint state;
};

//----------------------------------------------------------------------------//
// TransformSystem
//----------------------------------------------------------------------------//

class TransformSystem : public SceneSystem
{
public:

	TransformSystem(Scene* _scene);
	~TransformSystem(void);

protected:
	friend class Transform;

	Transform*& _RootRef(void) { return m_root; }
	Transform* m_root;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
