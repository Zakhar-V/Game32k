#pragma once

#include "Math.hpp"
#include "Object.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

class Scene;
class Entity;
class PhysicsEntity;

typedef Ptr<class Behavior> BehaviorPtr;
typedef Ptr<class Node> NodePtr;
typedef Ptr<class Camera> CameraPtr;

struct FrameInfo
{
	float time; // in seconds
	uint16 id;
};

enum UpdatePolicy : uint8
{
	UP_Auto, // update if necessary; deactivate after some time 
	UP_Always, // update each frame; never not deactivate
};

//----------------------------------------------------------------------------//
// Behavior
//----------------------------------------------------------------------------//

class Behavior : public Object
{
public:
	OBJECT("Behavior");

	virtual bool IsSleepingAllowed(void) { return true; }
	virtual bool IsEnabled(void) { return true; }
	virtual void Update(const FrameInfo& _frame) { }

protected:
	friend class Node;

	void _SetNode(Node* _node) { m_node = _node; }

	void _SetScene(Scene* _scene) { }

	Node* m_node = nullptr;
	Behavior* m_prev = nullptr;
	Behavior* m_next = nullptr;
};

//----------------------------------------------------------------------------//
// Node
//----------------------------------------------------------------------------//

class Node : public Object
{
public:
	OBJECT("Node");

	Node(void);
	~Node(void);

	void SetScene(Scene* _scene);
	void SetParent(Node* _parent);

	void DetachChildren(bool _remove = false);

	void AddBehavior(Behavior* _b);
	void RemoveBehavior(Behavior* _b);
	void RemoveAllBehaviors(void);
	Behavior* GetBehavior(uint _class, uint _index = 0);
	template <class T> T* GetBehavior(uint _index = 0) { return static_cast<T*>(GetBehavior(_index)); }

	void WakeUp(void);
	void SetSleepingThreshold(float _seconds) { m_sleepingThreshold = _seconds; }
	void SetUpdatePolicy(UpdatePolicy _policy) { m_updatePolicy = _policy; }
	UpdatePolicy GetUpdatePolicy(void) { return m_updatePolicy; }

	void Update(const FrameInfo& _frame);
	void PostUpdate(const FrameInfo& _frame);

	const Mat44& GetWorldTransform(void);
	void SetWorldTransform(const Mat44& _tm);
	void SetLocalPosition(const Vec3& _position);
	const Vec3& GetLocalPosition(void);
	void SetLocalRotation(const Quat& _rotation);
	const Quat& GetLocalRotation(void);
	void SetLocalScale(const Vec3& _scale);
	const Vec3& GetLocalScale(void);
	void SetWorldPosition(const Vec3& _position);
	Vec3 GetWorldPosition(void);
	void SetWorldRotation(const Quat& _rotation);
	const Quat& GetWorldRotation(void);
	void SetWorldScale(const Vec3& _scale);
	Vec3 GetWorldScale(void);

	void SetInheritPosition(bool _enabled = true);
	bool GetInheritPosition(void) { return m_inheritPosition; }
	void SetInheritRotation(bool _enabled = true);
	bool GetInheritRotation(void) { return m_inheritRotation; }
	void SetInheritScale(bool _enabled = true);
	bool GetInheritScale(void) { return m_inheritScale; }


protected:
	friend class Scene;

	virtual void _SetSceneImpl(Scene* _scene) { m_scene = _scene; }
	virtual void _UpdateImpl(const FrameInfo& _frame) { }

	void _InvalidateTransform(void);
	void _UpdateTransform(void);

	String m_name;
	int m_id;
	uint16 m_layerMask;
	uint16 m_tags;

	Vec3 m_localPosition;
	Quat m_localRotation;
	Vec3 m_localScale;
	Mat44 m_worldTransform;
	Quat m_worldRotation;

	Scene* m_scene;
	Entity* m_entity;
	Node* m_parent;
	Node* m_prev;
	Node* m_next;
	Node* m_child;

	Node* m_prevActive;
	Node* m_nextActive;

	Behavior* m_behaviors;

	DbvTreeNode* m_dbvtNode;
	PhysicsEntity* m_physics;

	bool m_active : 1;
	bool m_behaviorAllowSleep : 1;
	bool m_transformUpdated : 1;
	bool m_transformChanged : 1;
	bool m_inheritPosition : 1;
	bool m_inheritRotation : 1;
	bool m_inheritScale : 1;

	UpdatePolicy m_updatePolicy;

	uint16 m_lastUpdateFrame;
	uint16 m_lastViewFrame;

	float m_sleepingThreshold;
	float m_activeTime;
};

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//

class Camera : public Node
{
public:
	Camera(void);
	~Camera(void);

	void SetFov(float _fovY) { m_fov = _fovY; }
	float GetFov(void) { return m_fov; }
	void SetNear(float _near) { m_near = _near; }
	float GetNear(void) { return m_near; }
	void SetFar(float _far) { m_far = _far; }
	float GetFar(void) { return m_far; }
	void SetZoom(float _zoom) { m_zoom = _zoom; }
	float GetZoom(void) { return m_zoom; }
	void SetOrtho(bool _ortho = true) { m_orthographic = _ortho; }
	bool IsOrhto(void) { return m_orthographic; }

	void GetParams(float _x, float _y, float _w, float _h, UCamera& _params, Frustum& _frustum);

protected:

	bool m_orthographic;
	//bool m_flipY;
	float m_fov; // in radians
	float m_near;
	float m_far;
	float m_zoom;
};

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

class Scene	: public NonCopyable
{
public:
	Scene(void);
	~Scene(void);

	void Update(float _dt);

	void SetActiveCamera(Camera* _camera) { m_activeCamera = _camera; }
	Camera* GetActiveCamera(void) { return m_activeCamera; }

protected:
	friend class Node;

	void _AddActiveNode(Node* _node);
	void _RemoveActiveNode(Node* _node);
	void _AddRootNode(Node* _node);
	void _RemoveRootNode(Node* _node);
	void _AddNode(Node* _node);
	void _RemoveNode(Node* _node);

	uint16 m_frame;
	Node* m_rootNodes;
	Node* m_activeNodes;
	uint m_numNodes;
	uint m_numActiveNodes;
	uint m_numRootNodes;

	CameraPtr m_activeCamera;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
