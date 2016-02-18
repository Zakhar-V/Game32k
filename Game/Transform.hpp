#pragma once

#include "Scene.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

//typedef Ptr<class Transform> TransformPtr;

//----------------------------------------------------------------------------//
// Transform
//----------------------------------------------------------------------------//

enum TransformFlags
{
	TF_KeepWorldTransform,

};

class Transform : public Component
{
public:
	static ComponentType GetComponentTypeStatic(void) { return CT_Transform; }
	ComponentType GetComponentType(void) override { return CT_Transform; }
	bool IsSingleComponent(void) override { return true; }

	Transform(void);
	~Transform(void);

	Transform* GetParent(void) { return m_parent; }
	Transform* GetNext(void) { return m_next; }
	Transform* GetChild(void) { return m_child; }

	void SetParent(Transform* _parent);
	void DetachAllChildren(void);

	void UpdateChildMatrices(void);

	void SetKinematicMode(void);

protected:
	friend class TransformSystem;

	void _SetScene(Scene* _scene) override;
	void _InvalidateMatrix(bool _fromParent, bool _recursive);

	TransformSystem* m_system;
	Transform* m_parent;
	Transform* m_prev;
	Transform* m_next;
	Transform* m_child;

	Vec3 m_localPosition;
	Quat m_localRotation;
	Vec3 m_localScale;
	Mat44 m_worldMatrix;

	bool m_updated : 1;
	bool m_invalidated : 1;
	bool m_childrenUpdated : 1;
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

	void _AddComponent(Transform* _transform);
	void _RemoveComponent(Transform* _transform);
	void _AddUpdate(Transform* _transform);
	void _RemoveUpdate(Transform* _transform);
	void _Update(void);

	Transform*& _RootRef(void) { return m_root; }
	Transform* m_root;

	Array<Transform*> m_updates;
	bool m_locked;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
