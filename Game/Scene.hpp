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

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

enum ComponentType
{
	CT_Transform = 0,

	CT_Unknown,
	CT_MaxTypes,
};

class Component : public Object
{
public:

	static ComponentType GetComponentTypeStatic(void) { return CT_Unknown; }
	virtual ComponentType GetComponentType(void) { return CT_Unknown; }
	virtual bool IsSingleComponent(void) { return false; }

	Component(void);
	~Component(void);

	Entity* GetEntity(void) { return m_entity; }
	virtual Scene* GetScene(void);

	Component* GetNextComponent(void) { return m_next; }

	virtual void TransformUpdated(void) { }

	virtual void OnEvent(int _type, Entity* _sender, void* _arg) { }

protected:
	friend class Entity;

	virtual void _SetScene(Scene* _scene) { }
	virtual void _SetEntity(Entity* _entity)
	{ 
		//Scene* _oldScene = m_entity->GetScene();
		m_entity = _entity; 
		/*if (m_entity && m_entity->GetScene() != _oldScene)
			_SetScene(m_entity->GetScene());*/
	}

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

class Entity final : public Object
{
public:

	Entity(void);
	~Entity(void);

	Scene* GetScene(void) { return m_scene; }
	Entity* GetParent(void) { return m_parent; }
	void SetParent(Entity* _parent, bool _inheritTransform = true);
	void DetachAllChildren(bool _remove = false);
	void RemoveThis(bool _force = false);

	
	//void SendEvent(const Event& _event);

	template <class T> T* GetComponent(uint _index = 0) { return GetComponent(T::GetComponentTypeStatic(), _index); }
	Component* GetComponent(ComponentType _type, uint _index = 0);
	void AttachComponent(Component* _c);
	void DetachComponent(Component* _c, bool _removeFromScene = true);

protected:
	friend class Scene;
	friend class Component;

	void _SetScene(Scene* _scene);


	Scene* m_scene;
	Entity* m_parent;
	Entity* m_prev;
	Entity* m_next;
	Entity* m_child;

	Component* m_components[CT_MaxTypes];
	uint m_numComponents;


	bool m_persistent : 1;
};

//----------------------------------------------------------------------------//
// SceneSystem
//----------------------------------------------------------------------------//

enum SceneSystemType
{
	SST_Transform,

	// game specific
	SST_,

	SST_MaxTypes,
};

class SceneSystem : public NonCopyable
{
public:
	//static SceneSystemType GetSystemTypeStatic(void) { return CT_Unknown; }
	//virtual SceneSystemType GetSystemType(void) { return CT_Unknown; }

	SceneSystem(Scene* _scene) : m_scene(_scene) { }
	virtual ~SceneSystem(void) { }

	virtual void AddComponent(Component* _c);
	virtual void RemoveComponent(Component* _c);

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

	void AddEntity(Entity* _entity);
	void RemoveEntity(Entity* _entity);

protected:
	friend class Entity;

	Entity*& _RootEntityRef(void) { return m_rootEntity; }

	Entity* m_rootEntity;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
