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

	void SetColor(const Vec4& _color) { m_diffuse = _color; }
	void SetSpecular(const Vec4& _specular) { m_diffuse = _specular; }
	void SetEmissive(const Vec4& _emission) { m_emission = _emission; }
	void SetIntensity(const Vec4& _intensity) { m_intensity = _intensity; }

	bool IsTransparent(void) { return m_diffuse.a < 1; }

	void SetShader(ShaderID _shader) { m_shader = _shader; }
	ShaderID GetShader(void) { return m_shader; }

	void SetTexture(Texture* _texture) { m_texture = _texture; }
	Texture* GetTexture(void) { return m_texture; }
	//SamplerID GetSampler(void) { return m_sampler; }

	const Mat44& GetParams(void) { return *(const Mat44*)&m_diffuse; }

protected:

	Vec4 m_diffuse;
	Vec4 m_specular;
	Vec4 m_emission;
	Vec4 m_intensity;
	ShaderID m_shader;
	TexturePtr m_texture;
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
	MeshPart(uint _start = 0, uint _count = 0, uint _material = 0) :
		start(_start), count(_count), material(_material) {}
	uint start = 0;
	uint count = 0;
	uint material = 0;
};

typedef Ptr<class Mesh> MeshPtr;

class Mesh : public RefCounted
{
public:

	Mesh(void) { }
	~Mesh(void) { }

	void CreateFromGeometry(Geometry* _geom)
	{
		ASSERT(_geom != nullptr);
		SetGeometry(_geom);
		SetPartCount(1);
		uint _count = _geom->GetIndexCount();
		SetPart(0, 0, _count ? _count: _geom->GetVertexCount());
	}

	void SetGeometry(Geometry* _geom)
	{
		m_geom = _geom;
		if (m_geom)
		{
			m_data = m_geom->CreateVertexArray();
			m_bbox = m_geom->ComputeBBox();
		}
		else
		{
			m_data = nullptr;
			m_bbox.Reset();
		}
	}

	VertexArray* GetVertexArray(void) { return m_data; }
	const AlignedBox& GetBBox(void) { return m_bbox; }

	void SetPartCount(uint _count) { m_parts.Resize(_count); }
	uint GetPartCount(void) { return m_parts.Size(); }
	void SetPart(uint _index, const MeshPart& _part) { m_parts[_index] = _part; }
	void SetPart(uint _index, uint _start, uint _count, uint _material = 0) { SetPart(_index, {_start, _count, _material}); }
	const MeshPart& GetPart(uint _index) { return m_parts[_index]; }

	void SetMaterialCount(uint _count) { m_materials.Resize(_count); }
	uint GetMaterialCount(void) { return m_materials.Size(); }
	Material* GetMaterial(uint _index) { return _index < m_materials.Size() ? m_materials[_index] : nullptr; }

	void GetItem(uint _index, RenderItem& _item)
	{
		ASSERT(_index < m_parts.Size());
		const MeshPart& _part = m_parts[_index];
		_item.data = m_data;
		_item.material = GetMaterial(_part.material);
		_item.start = _part.start;
		_item.count = _part.count;
	}

protected:

	GeometryPtr m_geom;
	VertexArrayPtr m_data;
	Array<MeshPart> m_parts;
	Array<MaterialPtr> m_materials;
	AlignedBox m_bbox;
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

	DepthStencilStateID m_depthStencilEnabled;
	DepthStencilStateID m_depthStencilDisabled;

	RenderBuffer* m_colorBuffer;
	RenderBuffer* m_depthStencilBuffer;


	TexturePtr m_gBufferColorTexture;
	TexturePtr m_gBufferNormalTexture; // 
	TexturePtr m_gBufferDepthTexture;



	//Array<RenderableObject*> m_objects;
	//Array<Light*> m_lights;

	//RenderMeshPtr m_testCube;

	BufferPtr m_cameraBuffer;
	BufferPtr m_instanceBuffer;
	BufferPtr m_materialBuffer;
	BufferPtr m_skinBuffer;

	UMaterial m_materialStorage;
	UInstancing m_instanceStorage;
	USkinning m_skinStorage;

	RenderContainer m_renderContainer;
	Array<RenderItem> m_renderItems;
	Array<Light*> m_lights;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
