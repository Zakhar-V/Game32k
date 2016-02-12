#pragma once

#include "Math.hpp"
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

enum NodeType
{
	NT_Unknown = 0,
	NT_Camera = 0x1,
	NT_Renderable = 0x2,
	NT_Light = NT_Renderable | 0x4,
	NT_Terrain = NT_Renderable | 0x8,

	NT_Test = NT_Renderable | 0x10,
};

class Node : public Object
{
public:

	Node(void);
	~Node(void);

	virtual NodeType Type(void) { return NT_Unknown; }

	Scene* GetScene(void) { return m_scene; }
	void SetParent(Node* _parent);
	void DetachAllChildren(bool _remove = false);
	void RemoveThis(void);

protected:
	friend class Scene;

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
// CameraNode 
//----------------------------------------------------------------------------//

class CameraNode : public Node
{
public:

	virtual NodeType Type(void) { return NT_Camera; }

protected:

	Mat44 m_view;
	Mat44 m_proj;
};

//----------------------------------------------------------------------------//
// RenderableNode
//----------------------------------------------------------------------------//

class Geometry;

struct RenderOp
{
	Mat44* matrix;
	Geometry* geom;
	Material* material;
	void* deform; 
	uint baseVertex;
	uint start;
	uint count;
	float distance;
};

class RenderableNode : public Node
{
public:

	virtual NodeType Type(void) { return NT_Renderable; }

	virtual void GetRenderOps(Array<RenderOp>& _dst, uint _flags, uint _mask) { }
};

//----------------------------------------------------------------------------//
// LightNode
//----------------------------------------------------------------------------//

class LightNode : public RenderableNode
{
public:
	virtual NodeType Type(void) { return NT_Light; }

protected:
};

//----------------------------------------------------------------------------//
// TestNode
//----------------------------------------------------------------------------//

class TestNode : public RenderableNode
{
public:

	virtual NodeType Type(void) { return NT_Test; }

protected:
};

//----------------------------------------------------------------------------//
// TerrainNode
//----------------------------------------------------------------------------//

class TerrainNode : public RenderableNode
{
public:

	virtual NodeType Type(void) { return NT_Terrain; }

	//void CreateFromHeightMap(Image* _image, const Vec2& _size, float _heightScale);
protected:

};

//----------------------------------------------------------------------------//
// SkyDome
//----------------------------------------------------------------------------//

class SkyDome : public NonCopyable
{
public:

protected:

	float m_timeOfDay;
};

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

class Scene : public NonCopyable
{
public:

	Scene(void);
	~Scene(void);

	void AddNode(Node* _node);

	void Update(float _dt);
	void Render(float _dt);

	DbvTree& GetDbvt(void) { return m_dbvt; }

protected:

	friend class Node;

	Node*& _Root(void) { return m_rootNodes; }

	uint _NewID(Node* _node);
	void _FreeID(uint _id);

	Node* m_rootNodes;

	Array<Node*> m_nodes;
	Array<uint> m_freeIds;

	DbvTree m_dbvt;

	// [renderer]

	void _GetRenderableNodes();

	Array<RenderableNode*> m_visibleNodes;
	//Array<RenderableNode*> m_mainCameraPvs;
	//Array<RenderableNode*>
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
