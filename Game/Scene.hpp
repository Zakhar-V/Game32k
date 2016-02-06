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

	Node(Scene* _scene);
	~Node(void);

	void SetParent(Node* _parent);

protected:

	void _Link(Node*& _list);
	void _Unlink(Node*& _list);
	void _UpdateLocalTM(void);
	void _UpdateWorldTM(void);

	Scene* m_scene;
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
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class Scene : public NonCopyable
{
public:

protected:

	friend class Node;

	uint _RegisterNode(Node* _node);
	void _UnregisterNode(Node* _node);

	Array<Node*> m_nodes;
	Array<uint> m_freeIds;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
