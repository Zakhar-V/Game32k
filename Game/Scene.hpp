#pragma once

#include "Math.hpp"
#include "Object.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

class Component;
class Entity;
class SceneSystem;
class Scene;

class Transform;
class TransformSystem;

class PhysicsBody;
class PhysicsShape;
class PhysicsJoint;
class PhysicsWorld;

class RenderWorld;

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

enum ComponentType
{
	CT_Transform = 0,

	CT_PhysicsBody,
	CT_PhysicsShape,
	CT_PhysicsJoint,

	CT_Unknown,
	CT_MaxTypes,
};

class Component : public Object
{
public:

	static ComponentType GetComponentTypeStatic(void) { return CT_Unknown; }
	virtual ComponentType GetComponentType(void) { return CT_Unknown; }
	virtual bool IsSingleComponent(void) { return false; }
	virtual uint GetComponentSubType(void) { return 0; }

	Component(void);
	~Component(void);

	Entity* GetEntity(void) { return m_entity; }
	Scene* GetScene(void);

	Component* GetNextComponent(void) { return m_next; }

	template <class T> T* GetEntityComponent(uint _index = 0) { return static_cast<T*>(GetEntityComponent(T::GetComponentTypeStatic(), _index)); }
	Component* GetEntityComponent(ComponentType _type, uint _index = 0);


	virtual void TransformUpdated(void) { }

	virtual void OnEvent(int _type, Entity* _sender, void* _arg) { }

protected:
	friend class Entity;

	virtual void _SetScene(Scene* _scene) { }
	virtual void _SetEntity(Entity* _entity) { m_entity = _entity; }

	Entity* m_entity;
	Component* m_prev;
	Component* m_next;
};

//----------------------------------------------------------------------------//
// Entity
//----------------------------------------------------------------------------//

/*enum EntityEventType
{
	EET_SetScene,
	EET_SetParent,
	EET_AttachChild,
	EET_DetachChild,
};*/

enum EntityAttachFlags
{
	EAF_InheritTransform,
};

enum EntityEvent
{
	EE_,
};

typedef Ptr<Entity> EntityPtr;
typedef Ref<Entity> EntityRef;

class Entity final : public Object
{
public:

	Entity(void);
	~Entity(void);

	Scene* GetScene(void) { return m_scene; }
	bool SetScene(Scene* _scene);
	Entity* GetParent(void) { return m_parent; }
	bool SetParent(Entity* _parent);
	void DetachAllChildren(bool _remove = false);
	void DetachThis(bool _remove = false);

	//void SendEvent(const Event& _event);

	bool SetManualTransformHierarchy(bool _state = true) { m_manualTransformHierarchy = _state; }
	bool GetManualTransformHierarchy(void) { return m_manualTransformHierarchy; }
	Entity* GetParentTransform(void);
	void SetParentTransform(Entity* _parent);

	template <class T> T* GetComponent(uint _index = 0) { return static_cast<T*>(GetComponent(T::GetComponentTypeStatic(), _index)); }
	Component* GetComponent(ComponentType _type, uint _index = 0);
	bool AddComponent(Component* _c);
	void RemoveComponent(Component* _c);
	void RemoveAllComponents(void);

protected:
	friend class Scene;
	friend class Component;

	Scene* m_scene;
	Entity* m_parent;
	Entity* m_prev;
	Entity* m_next;
	Entity* m_child;

	Component* m_components[CT_MaxTypes];

	bool m_removed : 1;

	//bool m_persistent : 1;
	bool m_manualTransformHierarchy : 1;
};

//----------------------------------------------------------------------------//
// SceneSystem
//----------------------------------------------------------------------------//

class SceneSystem : public NonCopyable
{
public:
	SceneSystem(Scene* _scene) : m_scene(_scene) { }
	virtual ~SceneSystem(void) { }

protected:

	Scene* m_scene;
};

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

class Scene : public NonCopyable
{
public:

	Scene(void);
	~Scene(void);

	TransformSystem* GetTransformSystem(void) { return m_transformSystem; }
	PhysicsWorld* GetPhysicsWorld(void) { return m_physicsWorld; }
	RenderWorld* GetRenderWorld(void) { return m_renderWorld; }

	void Update(float _dt);
	

protected:
	friend class Entity;

	Entity*& _RootEntityRef(void) { return m_rootEntity; }

	Entity* m_rootEntity;

	TransformSystem* m_transformSystem;
	PhysicsWorld* m_physicsWorld;
	RenderWorld* m_renderWorld;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
