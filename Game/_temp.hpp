/*
struct _PackedVertex // 7/10 bytes (255 vertices = 1785/2550 bytes)
{
uint8 pos[3];
uint8 norm[2]; // normal.x normal.y
uint8 tc[2];
uint8 skin[3]; // weight, bone1, bone2
};
*/


//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

// geometry shader input
#if USE_GS && (COMPILE_VS || COMPILE_GS)
struct VsOut_t
{

};
#if COMPILE_VS
out Out;
#else
in In;
#endif 
#endif // USE_GS && (COMPILE_VS || COMPILE_GS)


// pixel shader input
struct PsIn_t
{

}
#if (COMPILE_VS && !USE_GS) || COMPILE_GS
out Out;
#elif COMPILE_FS
in In;
#endif

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

class Component
{
public:

	Component(Scene* _scene);

protected:
};


class Entity
{
public:

	Entity(Scene* _scene);

	void AttachComponent(Component* _c)
	{

	}

protected:

	Component* m_components;
	Array<Entity*> m_childs;
};


class Transform
{
public:

	void SetParent(Transform*) { }

protected:

	Array<Transform*> m_childs;
};


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

struct FrameInfo
{

};

class SceneSystem
{
public:

	virtual void Update(FrameInfo& _frame) { }
	Scene* GetScene(void) { return m_scene; }

protected:

	Scene* m_scene;
};

enum ComponentType
{
	CT_Unknown,

	CT_Updateable,
	CT_Behaviour,
	CT_Logic,

	CT_Renderable,
	CT_Light,
	CT_Camera,

	CT_Skeleton,

	CT_PhysicsBody,
	CT_PhysicsShape,
	CT_PhysicsJoint,

	MAX_COMPONENT_TYPES,
};

class Component : public Object
{
public:
	virtual bool IsSingle(void) { return false; }

	virtual uint GetComponentType(void) = 0;
	static uint GetComponentTypeStatic(void) { return CT_Unknown; }

	Component* GetNextComponent(void) { return m_nextComponent; }

	//virtual Actor* GetActor(void) { return nullptr; }
	//virtual Entity* GetEntity(void) { return nullptr; }

protected:
	friend class Actor;

	Component* m_prevComponent;
	Component* m_nextComponent;
};

class Actor : public Object
{
public:

	Actor(void);
	~Actor(void);

	void SetParentActor(Actor* _actor) { }
	Actor* GetParentActor(void) { return m_parentActor; }
	Actor* GetNextActor(void) { return m_nextActor; }
	Actor* GetChildActor(void) { return m_childActor; }
	void DetachAllChildren(bool _remove = false);
	void RemoveThis(void);

	void SetParentEntity(Entity* _parent) { }

protected:
	friend class Scene;
	friend class Entity;

	void _SetScene(Scene* _scene);

	virtual void _OnChangeScene(Scene* _scene) { }
	virtual void _OnChangeParent(Node* _node) { }

	//m_childs[]

	Actor* m_parentActor;
	Actor* m_prevActor;
	Actor* m_nextActor;
	Actor* m_childActor;

	//	Actor* m_parentActor;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class Entity : public Actor
{
public:

protected:

	//m_childs[]
};


//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class CUpdateable : public Component
{
public:

protected:

	bool m_enabled;
	CUpdateable* m_prevUpdateable;
	CUpdateable* m_nextUpdateable;
};

class CBehaviour : public CUpdateable
{
public:

	virtual void PreUpdate(void) = 0;
	virtual void Update(void) = 0;

protected:

};

class UpdateManager : public SceneSystem
{

};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class CRenderable : public Component
{
public:

protected:

	CRenderable* m_prevRenderable;
	CRenderable* m_nextRederable;
};

class RenderWorld : public SceneSystem
{
public:

	void Update(FrameInfo& _frame) override { }

	void AddComponent(CRenderable* _component) { }
	void RemoveComponent(CRenderable* _component) { }

protected:

	CRenderable* m_renderables;
	DbvTree m_dbvt;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class EntityChild
{
public:

protected:
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

	template <class T> T* GetComponent(uint _index = 0)
	{
		Component* _c = m_components[T::GetComponentTypeStatic()];
		while (_c && _index--)
			_c = _c->GetNextComponent();
		return _c;
	}

	Node(void);
	~Node(void);

	virtual NodeType Type(void) { return NT_Unknown; }

	Scene* GetScene(void) { return m_scene; }
	void SetParent(Node* _parent);
	void DetachAllChildren(bool _remove = false);
	void RemoveThis(void);

	const Mat44& GetWorldMatrix(void);

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

	Component* m_components[MAX_COMPONENT_TYPES];
};

//----------------------------------------------------------------------------//
//  
//----------------------------------------------------------------------------//

/*class Entity : public Object
{
public:

protected:

Entity* m_parent;
Entity* m_prev;
Entity* m_next;
Entity* m_child;
}; */

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
	//Material* material;
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

	friend class Actor;
	friend class Node;

	Node*& _Root(void) { return m_rootNodes; }
	Actor*& _RootActor(void) { return m_rootActor; }

	uint _NewID(Node* _node);
	void _FreeID(uint _id);

	Node* m_rootNodes;

	Array<Node*> m_nodes;
	Array<uint> m_freeIds;

	DbvTree m_dbvt;

	// [renderer]

	void _GetRenderableNodes();

	Array<RenderableNode*> m_visibleNodes;

	Actor* m_rootActor;
	//Array<RenderableNode*> m_mainCameraPvs;
	//Array<RenderableNode*>
};

//----------------------------------------------------------------------------//
// Node
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Node::Node(void) :
	m_scene(nullptr),
	m_entity(nullptr),
	m_layer(0),
	m_group(0),
	m_id(0),
	m_parent(nullptr),
	m_prev(nullptr),
	m_next(nullptr),
	m_child(nullptr),
	m_position(VEC3_ZERO),
	m_rotation(QUAT_IDENTITY),
	m_scale(VEC3_ONE),
	m_matrix(MAT44_IDENTITY),
	m_physics(nullptr),
	m_dbvtNode(nullptr)
{
}
//----------------------------------------------------------------------------//
Node::~Node(void)
{
	DetachAllChildren(true);
	// TODO: remove physics, dbvt ... ?
}
//----------------------------------------------------------------------------//
void Node::SetParent(Node* _parent)
{
	ASSERT(_parent != this);
	if (m_parent == _parent)
		return;

	if (!_parent && !m_scene)
		AddRef();

	if (m_parent)
	{
		_OnChangeParent(nullptr);
		_Unlink(m_parent->m_child);
	}
	else if (m_scene)
	{
		_Unlink(m_scene->_Root());
	}

	//TODO: update world TM

	m_parent = _parent;

	if (m_parent)
	{
		_Link(m_parent->m_child);
		_OnChangeParent(_parent);

		if (m_parent->m_scene && m_parent->m_scene != m_scene)
			_SetScene(m_parent->m_scene);
	}
	else if (m_scene)
	{
		_Link(m_scene->_Root());
	}

	if (!_parent && !m_scene)
		Release();
}
//----------------------------------------------------------------------------//
void Node::DetachAllChildren(bool _remove)
{
	for (Node *n, *i = m_child; i;)
	{
		n = i;
		i = i->m_next;
		if (_remove && !i->m_entity)
			n->RemoveThis();
		else
			n->SetParent(nullptr);
	}
}
//----------------------------------------------------------------------------//
void Node::RemoveThis(void)
{
	NodePtr _addref(this);
	SetParent(nullptr);
	_SetScene(nullptr);
}
//----------------------------------------------------------------------------//
void Node::_SetScene(Scene* _scene)
{
	if (!m_parent && !_scene)
		AddRef();

	Scene* _oldScene = m_scene;

	if (m_scene != _scene)
	{
		if (m_scene)
		{
			_OnChangeScene(nullptr);
			m_scene->_FreeID(m_id);
			if (!m_parent)
				_Unlink(m_scene->_Root());
		}

		m_scene = _scene;

		if (m_scene)
		{
			m_id = m_scene->_NewID(this);
			if (!m_parent)
				_Link(m_scene->_Root());
			_OnChangeScene(m_scene);
		}
	}

	if (_oldScene != m_scene || _scene == nullptr)
	{
		for (Node* i = m_child; i; i = i->m_next)
		{
			i->_SetScene(m_scene);
		}
	}

	if (!m_parent && !_scene)
		Release();
}
//----------------------------------------------------------------------------//
void Node::_Link(Node*& _list)
{
	ASSERT(m_prev == nullptr && m_next == nullptr && _list != this);

	m_next = _list;
	_list = this;
	if (m_next)
		m_next->m_prev = this;

	//Link((uint8**)&_list, (uint8*)this, offsetof(Node, m_prev), offsetof(Node, m_next));
}
//----------------------------------------------------------------------------//
void Node::_Unlink(Node*& _list)
{
	if (m_next)
		m_next->m_prev = m_prev;
	if (m_prev)
		m_prev->m_next = m_next;
	else
		_list = m_next;

	m_prev = nullptr;
	m_next = nullptr;

	//Unlink((uint8**)&_list, (uint8*)this, offsetof(Node, m_prev), offsetof(Node, m_next));
}
//----------------------------------------------------------------------------//
void Node::_UpdateLocalTM(void)
{
	if (m_parent)
	{

	}
	else
	{

	}
}
//----------------------------------------------------------------------------//
void Node::_UpdateWorldTM(void)
{

}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Scene::Scene(void) :
	m_rootNodes(nullptr)
{
	m_nodes.Push(nullptr); // 0
}
//----------------------------------------------------------------------------//
Scene::~Scene(void)
{
}
//----------------------------------------------------------------------------//
void Scene::AddNode(Node* _node)
{
	if (_node)
		_node->_SetScene(this);
}
//----------------------------------------------------------------------------//
void Scene::Update(float _dt)
{

}
//----------------------------------------------------------------------------//
void Scene::Render(float _dt)
{

}
//----------------------------------------------------------------------------//
uint Scene::_NewID(Node* _node)
{
	uint _id = 0;
	if (m_freeIds.Size())
	{
		_id = m_freeIds.Top();
		m_freeIds.Pop();
	}
	else
	{
		_id = m_nodes.Size();
		m_nodes.Push(nullptr);
	}
	m_nodes[_id] = _node;
	return _id;
}
//----------------------------------------------------------------------------//
void Scene::_FreeID(uint _id)
{
	ASSERT(_id < m_nodes.Size());
	m_nodes[_id] = nullptr;
}
//----------------------------------------------------------------------------//
void Scene::_GetRenderableNodes()
{

}
//----------------------------------------------------------------------------//

