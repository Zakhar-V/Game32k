#pragma once

//#include "Scene.hpp"
#include "Graphics.hpp"
#include "Geometry.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

class Scene;
class Node;
class RenderNode;
class Light;
class Camera;
class Terrain;

//----------------------------------------------------------------------------//
// Material
//----------------------------------------------------------------------------//

typedef Ptr<class Material> MaterialPtr;

class Material : public RefCounted
{
public:

protected:

	Texture* m_diffuse;
	Texture* m_bump;
};

//----------------------------------------------------------------------------//
// RenderItem
//----------------------------------------------------------------------------//

struct RenderItem
{
	RenderNode* node;
	VertexArray* data;
	Material* material;
	uint start;
	uint count;
};

//----------------------------------------------------------------------------//
// Mesh
//----------------------------------------------------------------------------//

struct MeshPart
{
	uint start = 0;
	uint count = 0;
	uint material = 0;
};

typedef Ptr<class Mesh> MeshPtr;

class Mesh : public RefCounted
{
public:

	void SetData(VertexArray* _data) { m_data = _data; }
	VertexArray* GetData(void) { return m_data; }

	void SetPartCount(uint _count) { m_parts.Resize(_count); }
	uint GetPartCount(void) { return m_parts.Size(); }
	const MeshPart& GetPart(uint _index) { return m_parts[_index]; }

	void SetMaterialCount(uint _count) { m_materials.Resize(_count); }
	uint GetMaterialCount(void) { return m_materials.Size(); }
	Material* GetMaterial(uint _index) { return _index < m_materials.Size() ? m_materials[_index] : nullptr; }

	void GetItem(RenderItem& _item, uint _index)
	{
		ASSERT(_index < m_parts.Size());
		const MeshPart& _part = m_parts[_index];
		_item.data = m_data;
		_item.material = GetMaterial(_part.material);
		_item.start = _part.start;
		_item.count = _part.count;
	}

protected:

	VertexArrayPtr m_data;
	Array<MeshPart> m_parts;
	Array<MaterialPtr> m_materials;
};

//----------------------------------------------------------------------------//
// RenderContainer
//----------------------------------------------------------------------------//

struct RenderContainer : public DbvtContainer
{
	Array<Node*> objects;
	uint mask = 0;

	void AddObject(void* _object, const AlignedBox& _bbox, bool _withTest) override;
};

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

//protected:

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



	//Array<RenderableObject*> m_objects;
	//Array<Light*> m_lights;

	//RenderMeshPtr m_testCube;

	BufferPtr m_cameraBuffer;
	BufferPtr m_instanceBuffer;
	BufferPtr m_skinBuffer;

	RenderContainer m_renderContainer;
	Array<RenderItem> m_renderItems;
	Array<Light*> m_lights;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
