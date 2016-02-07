#pragma once

#include "Math.hpp"
#include "Collision.hpp"
#include "Object.hpp"


//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

class Scene;
class Entity;
typedef Ptr<class Node> NodePtr;
typedef Ref<class Node> NodeRef;

class PhysicsEntity;
class PhysicsWorld;
class SoundWorld;
//class VegetationSystem;
//class WeatherSystem;

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

enum ComponentType
{
	CT_Camera,
	CT_Light,
	//CT_
	CT_Terrain,


	CT_Shape,
	CT_Body,
	//CT_Joint,

	CT_MaxTypes,
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class SceneObject : public Object // SerializableObject
{
public:

protected:
	Scene* m_scene;
	uint16 m_layer;
	uint16 m_group;
	uint m_id;
	//m_name;
};

//----------------------------------------------------------------------------//
// Animator
//----------------------------------------------------------------------------//

class Animator : public Object
{
public:

protected:

	//Node* m_node;
};

//----------------------------------------------------------------------------//
// Node
//----------------------------------------------------------------------------//

class Node : public Object
{
public:

	Node(void);
	~Node(void);

	Scene* GetScene(void) { return m_scene; }
	void SetParent(Node* _parent);
	void DetachAllChildren(bool _remove = false);
	void RemoveThis(void);

protected:

	void _SetScene(Scene* _scene);

	virtual void _OnChangeScene(Scene* _scene) { }
	virtual void _OnChangeParent(Node* _node) { }
	virtual void _Register(void) { }
	virtual void _Unregister(void) { }

	void _Link(Node*& _list);
	void _Unlink(Node*& _list);
	void _UpdateLocalTM(void);
	void _UpdateWorldTM(void);

	Scene* m_scene;
	Entity* m_entity;
	uint16 m_layer;
	uint16 m_group;
	uint m_id;

	Node* m_parent;
	Node* m_prev;
	Node* m_next;
	Node* m_child;

	Vec3 m_position;
	Quat m_rotation;
	Vec3 m_scale;
	Mat44 m_matrix;

	PhysicsEntity* m_physics;
	DbvTreeNode* m_dbvtNode;

	//bool m_removeAllChildrenWhenDestroy : 1; // default is true
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class TerrainNode : public Node
{
public:

protected:


};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class Scene : public NonCopyable
{
public:

	void AddNode(Node* _node);
	void RemoveNode(Node* _node);

protected:

	friend class Node;

	Node*& _Root(void) { return m_rootNodes; }

	uint _RegisterNode(Node* _node);
	void _UnregisterNode(Node* _node);

	Node* m_rootNodes;

	Array<Node*> m_nodes;
	Array<uint> m_freeIds;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
