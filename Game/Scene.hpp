#pragma once

#include "Math.hpp"
#include "Object.hpp"
#include "Graphics.hpp"
#include "Image.hpp"
#include "Geometry.hpp"
#include "Render.hpp"

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

	virtual void _SetNode(Node* _node) { m_node = _node; }
	virtual void _SetScene(Scene* _scene) { }

	Node* m_node = nullptr;
	Behavior* m_prev = nullptr;
	Behavior* m_next = nullptr;
};

//----------------------------------------------------------------------------//
// Node
//----------------------------------------------------------------------------//

enum NodeType : uint
{
	NT_Node = 0x1,
	NT_Camera = 0x2,


	
	NT_Light = 0x4,
	NT_PointLight = 0x8,
	NT_SpotLight = 0x10,
	NT_DirectionalLight = 0x20,

	NT_RenderNode = 0x80,

	NT_Model = 0x100,
	NT_StaticModel = 0x200,
	NT_SkinnedModel = 0x400,

	NT_BillboardSet = 0x800,
	NT_ParticleEmitter = 0x1000,
	NT_Terrain = 0x2000,
	NT_SkyDome = 0x4000, 
	NT_Vegetation = 0x8000,
	NT_WaterPlane = 0x10000,

	NT_Entity = 0x80000,

};

class Node : public Object
{
public:
	OBJECT("Node");

	Node(void);
	~Node(void);

	uint GetTypeMask(void) { return m_typeMask; }

	void SetScene(Scene* _scene);
	void SetParent(Node* _parent);

	void DetachChildren(bool _remove = false);
	Node* GetParent(void) { return m_parent; }
	Node* GetChild(void) { return m_child; }
	Node* GetNext(void) { return m_next; }

	void AddBehavior(Behavior* _b);
	void RemoveBehavior(Behavior* _b);
	void RemoveAllBehaviors(void);
	Behavior* GetBehavior(uint _class, uint _index = 0);
	template <class T> T* GetBehavior(uint _index = 0) { return static_cast<T*>(GetBehavior(_index)); }

	void WakeUp(void);
	void SetSleepingThreshold(float _seconds) { m_sleepingThreshold = _seconds; }
	float GetSleepingThreshold(float _seconds) { return m_sleepingThreshold; }

	bool IsEnabled(void) { return true; } // TODO

	void Update(const FrameInfo& _frame);
	void PostUpdate(const FrameInfo& _frame);

	Vec3 LocalToWorld(const Vec3& _v) { return GetWorldTransform().Transform(_v); }
	Vec3 WorldToLocal(const Vec3& _v) { return GetWorldTransform().Copy().Inverse().Transform(_v); }

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

	void _InvalidateWorldBBox(void);
	void _CreateDbvtNode(void);
	void _DeleteDbvtNode(void);
	void _InvalidateDbvtNode(void);
	void _UpdateDbvtNode(void);
	virtual void _GetWorldBBox(AlignedBox& _bbox) { _bbox.Reset(); }

	String m_name;
	int m_id;
	uint16 m_layerMask;
	uint16 m_tags;
	uint m_typeMask; // NodeType

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

	DbvtNode* m_dbvtNode;
	PhysicsEntity* m_physics;

	bool m_active : 1;
	bool m_behaviorAllowSleep : 1;

	bool m_transformUpdated : 1;
	bool m_transformChanged : 1;
	
	bool m_worldBBoxUpdated : 1;
	bool m_dbvtUpdated : 1;
	bool m_dbvtNodeInTree : 1;

	bool m_inheritPosition : 1;
	bool m_inheritRotation : 1;
	bool m_inheritScale : 1;

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
// Light
//----------------------------------------------------------------------------//

typedef Ptr<class Light> LightPtr;

class Light : public Node
{
public:
	OBJECT("Light");

	Light(void);
	~Light(void);

protected:
};

//----------------------------------------------------------------------------//
// PointLight
//----------------------------------------------------------------------------//

class PointLight : public Light
{
public:
	OBJECT("PointLight");

	PointLight(void);
	~PointLight(void);

protected:
};

//----------------------------------------------------------------------------//
// SpotLight
//----------------------------------------------------------------------------//

class SpotLight : public Light
{
public:
	OBJECT("SpotLight");

	SpotLight(void);
	~SpotLight(void);

protected:
};

//----------------------------------------------------------------------------//
// DirectionalLight
//----------------------------------------------------------------------------//

class DirectionalLight : public Light
{
public:
	OBJECT("DirectionalLight");

	DirectionalLight(void);
	~DirectionalLight(void);

	DirectionalLight* GetNextLight(void) { return m_nextLight; }

protected:
	friend class Scene;

	DirectionalLight* m_prevLight;
	DirectionalLight* m_nextLight;
};

//----------------------------------------------------------------------------//
// DecalSet
//----------------------------------------------------------------------------//

class DecalSet : public NonCopyable
{
public:

protected:

	Array<Vertex> m_decals;
};

//----------------------------------------------------------------------------//
// RenderNode
//----------------------------------------------------------------------------//

typedef Ptr<class RenderNode> RenderNodePtr;

class RenderNode : public Node
{
public:
	OBJECT("RenderNode");

	RenderNode(void);
	~RenderNode(void);

	virtual void GetRenderItems(Array<RenderItem>& _items) { }
};

//----------------------------------------------------------------------------//
// Model
//----------------------------------------------------------------------------//

typedef Ptr<class Model> ModelPtr;

class Model : public RenderNode
{
public:
	OBJECT("Model");

	Model(void);
	~Model(void);

	void SetMesh(Mesh* _mesh);
	Mesh* GetMesh(void) { return m_mesh; }
	void GetRenderItems(Array<RenderItem>& _items) override;

protected:

	MeshPtr m_mesh;
};

//----------------------------------------------------------------------------//
// StaticModel
//----------------------------------------------------------------------------//

typedef Ptr<class StaticModel> StaticModelPtr;

class StaticModel : public Model
{
public:
	OBJECT("StaticModel");

	StaticModel(void);
	~StaticModel(void);

protected:

	void _GetWorldBBox(AlignedBox& _bbox) override;
};

//----------------------------------------------------------------------------//
// SkinnedModel
//----------------------------------------------------------------------------//

typedef Ptr<class SkinnedModel> SkinnedModelPtr;

class SkinnedModel : public Model
{
public:
	OBJECT("SkinnedModel");

	SkinnedModel(void);
	~SkinnedModel(void);

protected:

	void _GetWorldBBox(AlignedBox& _bbox) override;

	//m_bones;
};

//----------------------------------------------------------------------------//
// BillboardSet
//----------------------------------------------------------------------------//

typedef Ptr<class BillboardSet> BillboardSetPtr;

class BillboardSet : public RenderNode
{
public:
	OBJECT("BillboardSet");

};

//----------------------------------------------------------------------------//
// ParticleEmitter
//----------------------------------------------------------------------------//

typedef Ptr<class ParticleEmitter> ParticleEmitterPtr;

class ParticleEmitter : public BillboardSet
{
public:
	OBJECT("ParticleEmitter");

};

//----------------------------------------------------------------------------//
// SkyDome
//----------------------------------------------------------------------------//

typedef Ptr<class SkyDome> SkyDomePtr;

class SkyDome : public RenderNode
{
public:
	OBJECT("SkyDome");

};

//----------------------------------------------------------------------------//
// Terrain
//----------------------------------------------------------------------------//

typedef Ptr<class Terrain> TerrainPtr;

class Terrain : public RenderNode
{
public:
	OBJECT("Terrain");

	Terrain(void);
	~Terrain(void);

	Terrain* GetNextTerrain(void) { return m_nextTerrain; }

	float GetHeight(float _x, float _z);

	void Create(Image* _heightmap, float _yScale, float _xzScale, uint _numSectors);

	void GetRenderItems(Array<RenderItem>& _items) override;

	void _GetWorldBBox(AlignedBox& _bbox) override;

	friend class Scene;

	Vec3 m_scale;
	uint m_numSectors;
	ImagePtr m_heightmap;
	VertexArrayPtr m_mesh;
	TexturePtr m_texture;
	AlignedBox m_localBBox;

	Terrain* m_prevTerrain;
	Terrain* m_nextTerrain;
};

//----------------------------------------------------------------------------//
// Vegetation
//----------------------------------------------------------------------------//

typedef Ptr<class Vegetation> VegetationPtr;

class Vegetation : public RenderNode
{
public:
	OBJECT("Vegetation");

	Vegetation(void);
	~Vegetation(void);


	//void place(float dencity)

	Vegetation* m_prevVegetation;
	Vegetation* m_nextVegetation;
};

//----------------------------------------------------------------------------//
// WaterPlane
//----------------------------------------------------------------------------//

typedef Ptr<class WaterPlane> WaterPlanePtr;

class WaterPlane : public RenderNode
{
public:
	OBJECT("WaterPlane");


	//void place(float dencity)
};

//----------------------------------------------------------------------------//
// Scene
//----------------------------------------------------------------------------//

class Scene	: public NonCopyable
{
public:
	Scene(void);
	~Scene(void);

	Dbvt* GetDbvt(void) { return &m_dbvt; }
	void UpdateDbvt(void);

	void UpdateTransform(void);

	void Update(float _seconds);

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
	uint _NewID(Node* _node);
	void _FreeID(uint _id);


	Dbvt m_dbvt;

	uint16 m_frame;

	Node* m_rootNodes;
	Node* m_activeNodes;
	uint m_numNodes;
	uint m_numActiveNodes;
	uint m_numRootNodes;
	Array<Node*> m_nodes;
	Array<uint> m_freeIds;

	CameraPtr m_activeCamera;

	DirectionalLight* m_directionalLights;
	Terrain* m_terrain;
	Vegetation* m_vegetation;

	//SkyDome* m_sky;
	//Wheater
	//float m_waterHeight

};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
