#pragma once

#include "Scene.hpp"
#include "Graphics.hpp"
#include "Geometry.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

class Scene;
class Light;
class Camera;
class Terrain;
class RenderWorld;

class RenderableObject;

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class Material : public RefCounted
{
public:

protected:

	Texture* m_diffuse;
	Texture* m_bump;
};

typedef Ptr<class Material> MaterialPtr;

//----------------------------------------------------------------------------//
// RenderItem
//----------------------------------------------------------------------------//

struct RenderItem
{
	RenderableObject* object;
	RenderMesh* mesh;
	Material* material;
	Mat44* matrix;
	float distance;

	//bool operator < (const RenderItem& _rhs) const
};

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// RenderableObject
//----------------------------------------------------------------------------//

/*enum RenderableType	: uint
{
	RT_PointLight = 0x1,
	RT_SpotLight = 0x2,
	RT_DirectionalLight = 0x4,

	RT_StaticMesh = 0x8,
	RT_SkinnedMesh = 0x10,

	RT_Particles = 0x20,
	RT_BillboardSet = 0x40,
	RT_DecalSet = 0x80,

	RT_Terrain = 0x100,

	RT_Water = 0x200,

	RT_SkyDome = 0x400,
	RT_SkyClouds = 0x800,
};

class RenderableObject : public Component
{
public:
	RenderableObject(RenderableType _type);
	~RenderableObject(void);

	RenderableType GetType(void) { return m_type; }

	virtual void GetRenderItems(Array<RenderItem>& _dst, uint _flags) { }

protected:

	void _SetScene(Scene* _scene) override;
	virtual void _SetWorldImpl(RenderWorld* _world) { m_world = _world; }

	RenderableObject* m_prev; // todo: remove
	RenderableObject* m_next; // todo: remove


	RenderWorld* m_world;
	RenderableType m_type;

	AlignedBox m_bbox; // in world space
};

//----------------------------------------------------------------------------//
// Light
//----------------------------------------------------------------------------//

enum LightType
{
	LT_Point,
	LT_Spot,
	LT_Directional,
};

class Light : public RenderableObject
{
public:
	static ComponentType GetComponentTypeStatic(void) { return CT_Light; }
	ComponentType GetComponentType(void) override { return CT_Light; }

protected:

	Light* m_prevLight;
	Light* m_nextLight;
};

//----------------------------------------------------------------------------//
// Mesh
//----------------------------------------------------------------------------//

class Mesh : public RenderableObject
{
public:
	static ComponentType GetComponentTypeStatic(void) { return CT_Mesh; }
	ComponentType GetComponentType(void) override { return CT_Mesh; }
	//bool IsSingleComponent(void) override { return true; }

	Mesh(void);
	~Mesh(void);

	void SetSkinned(bool _skinned = true);
	bool IsSkinned(void) { return m_type == RT_SkinnedMesh; }

protected:

	GeometryPtr m_mesh;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class DecalSet : public RenderableObject
{
public:
	static ComponentType GetComponentTypeStatic(void) { return CT_DecalSet; }
	ComponentType GetComponentType(void) override { return CT_DecalSet; }

protected:

	
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class TerrainNode
{
public:

protected:
};

class TerrainSector : public TerrainNode
{
public:

protected:
	AlignedBox m_bbox;
};

class Terrain : public RenderableObject
{
public:

protected:

	TerrainNode* m_rootNode;

	//Terrain* m_prev;
	//Terrain* m_next;
};

//----------------------------------------------------------------------------//
// RenderWorld
//----------------------------------------------------------------------------//

class RenderWorld : SceneSystem
{
public:
	RenderWorld(Scene* _scene);
	~RenderWorld(void);

	void SetActiveCamera(Camera* _camera) { m_activeCamera = _camera; }
	Camera* GetActiveCamera(void) { return m_activeCamera; }

	void GetObjects(Array<RenderableObject*>& _dst, const Frustum& _frustum, uint _mask);
	void GetLights(Array<Light*>& _dst, const Frustum& _frustum, uint _mask);

	void Draw(void);

protected:
	friend class RenderableObject;


	Camera* m_activeCamera;

	RenderableObject*& _ObjectList(void) { return m_objects; }

	RenderableObject* m_objects;
};*/

//----------------------------------------------------------------------------//
// Renderer
//----------------------------------------------------------------------------//

#define gRenderer Renderer::Get()

#define MAX_TRANSPARENCY_LAYERS	3

class Renderer : public Singleton<Renderer>
{
public:

	Renderer(void);
	~Renderer(void);

	void Draw(Scene* _scene);

protected:

	/*RenderBuffer* m_primaryRenderBuffer;
	RenderBuffer* m_primaryDepthStencilBuffer;

	RenderBuffer* m_secondaryRenderBuffer;
	RenderBuffer* m_secondaryDepthStencilBuffer;

	// gbuffer
	TexturePtr m_primaryRenderTexture;
	TexturePtr m_primaryDepthStencilTexture;
	TexturePtr m_auxDepth;
	TexturePtr m_auxColor;*/

	TexturePtr m_gBufferColorTexture;
	TexturePtr m_gBufferNormalTexture; // 
	TexturePtr m_gBufferDepthTexture;



	Array<RenderableObject*> m_objects;
	Array<Light*> m_lights;

	RenderMeshPtr m_testCube;

	BufferPtr m_cameraBuffer;
	BufferPtr m_instanceBuffer;
	BufferPtr m_skinBuffer;

};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
