#pragma once

#include "Scene.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

//typedef Ptr<class Transform> TransformPtr;

//----------------------------------------------------------------------------//
// Transform
//----------------------------------------------------------------------------//

enum TransformSpace
{
	TS_World,
	TS_Local,
	TS_Parent,
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

	const Mat44& GetWorldMatrix(void);
	void SetWorldMatrix(const Mat44& _matrix);

	void SetPosition(const Vec3& _position, TransformSpace _space = TS_Local);
	Vec3 GetPosition(TransformSpace _space = TS_Local);
	//Translate
	
	void SetRotation(const Quat& _rotation, TransformSpace _space = TS_Local);
	Quat GetRotation(TransformSpace _space = TS_Local);
	void SetDirection(const Vec3& _direction, TransformSpace _space = TS_Local);
	Vec3 GetDirection(TransformSpace _space = TS_Local);
	//LookAt
	//Rotate
	//RotateAround

	void SetScale(const Vec3& _scale, TransformSpace _space = TS_Local);
	Vec3 GetScale(TransformSpace _space = TS_Local);
	//Scale

protected:
	friend class TransformSystem;

	void _SetScene(Scene* _scene) override;
	void _Invalidate(bool _fromChild = false);
	void _UpdateMatrix(void);
	void _Update(void);

	TransformSystem* m_system;
	Transform* m_parent;
	Transform* m_prev;
	Transform* m_next;
	Transform* m_child;

	Vec3 m_localPosition;
	Quat m_localRotation;
	Vec3 m_localScale; // todo: remove it
	Vec3 m_worldPosition;
	Quat m_worldRotation;
	Vec3 m_worldScale;
	Mat44 m_worldMatrix;

	bool m_inheritPosition : 1;
	bool m_inheritRotation : 1;
	bool m_inheritScale : 1;

	bool m_updated : 1;	// matrix updated
	bool m_invalidated : 1; // this or child require update
	bool m_changed : 1;	// matrix was changed
	bool m_updateLocalFromWorld : 1; // todo: remove it

};

//----------------------------------------------------------------------------//
// TransformSystem
//----------------------------------------------------------------------------//

class TransformSystem : public SceneSystem
{
public:

	TransformSystem(Scene* _scene);
	~TransformSystem(void);

	void Update(float _dt);

protected:
	friend class Transform;

	void _AddUpdate(Transform* _transform);
	void _RemoveUpdate(Transform* _transform);

	Transform*& _RootRef(void) { return m_root; }
	Transform* m_root;

	Array<Transform*> m_updates;
	bool m_locked;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
