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
// MaterialParams
//----------------------------------------------------------------------------//

struct MaterialParams
{
	void SetColor(const Vec3& _color) { diffuse.Set(_color.x, _color.y, _color.z, diffuse.w); }
	const Vec3& GetColor(void) const { return *(const Vec3*)&diffuse; }
	//void SetSpecularColor(const Vec3& _color) { specular.Set(_color.x, _color.y, _color.z, specular.w); }
	//const Vec3& GetSpecularColor(void) const { return *(const Vec3*)&specular; }
	void SetTransparency(float _transparency) { diffuse.w = _transparency; }
	float GetTransparency(void) const { return diffuse.w; }
	bool IsTranslucent(void) const { return diffuse.w < 1; }
	void SetShininess(float _shininess) { params.x = _shininess; }
	float GetShininess(void) const { return params.x; }
	void SetEmission(float _emission) { params.y = _emission; }
	float GetEmission(void) const { return params.y; }
	//void SetIntensity(float _intencity) { params.z = _intencity; }
	//float GetIntensity(void) const { return params.z; }

	operator const Mat24& (void) const { return *(const Mat24*)&diffuse; }

	Vec4 diffuse = { 1, 1, 1, 1 };
	//Vec4 specular = { 1, 1, 1, 0.5f };
	Vec4 params = { 0.5f, 0, 1, 0 }; // shininess, emission, intensity
};

//----------------------------------------------------------------------------//
// Material
//----------------------------------------------------------------------------//

typedef Ptr<class Material> MaterialPtr;

class Material : public RefCounted
{
public:
	Material(void) { }
	Material(const Material& _other) : m_shader(_other.m_shader), m_sampler(_other.m_sampler), m_texture(_other.m_texture) { }

	void SetShader(ShaderID _shader) { m_shader = _shader; }
	ShaderID GetShader(void) { return m_shader; }
	void SetSampler(SamplerID _sampler) { m_sampler = _sampler; }
	SamplerID GetSampler(void) { return m_sampler; }
	void SetTexture(Texture* _texture) { m_texture = _texture; }
	Texture* GetTexture(void) { return m_texture; }
	MaterialPtr Clone(void) { return new Material(*this); }

protected:

	ShaderID m_shader = FS_NoTexture;
	SamplerID m_sampler = 0;
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

typedef Ptr<class Mesh> MeshPtr;

class Mesh : public RefCounted
{
public:

	Mesh(void) { }
	~Mesh(void) { }

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
	void SetMaterial(Material* _mtl) { m_material = _mtl; }
	Material* GetMaterial(void) { return m_material; }
	MaterialParams& GetMaterialParams(void) { return m_materialParams; }

	void GetItem(RenderItem& _item)
	{
		ASSERT(m_data != nullptr);
		_item.data = m_data;
		_item.material = m_material;
		_item.start = 0;
		_item.count = m_data->IsIndexed() ? m_data->GetIndexCount() : m_data->GetVertexCount();
	}

protected:

	GeometryPtr m_geom;
	VertexArrayPtr m_data;
	MaterialPtr m_material;
	MaterialParams m_materialParams;
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

protected:

	void _BindUniformBuffers(void);
	void _SetupViewport(Camera* _cam);
	void _GetVisibleObjects(Scene* _scene, const Frustum& _frustum);
	void _SetupVertexShaders(uint _type);
	void _FillGBuffer(void);
	void _DrawBatchs(RenderItem* _items, uint _numItems, Shader* _fragmentShader);
	//void _DrawSolid(void);
	//void _DrawTransparent(void);

	VertexArrayPtr m_fsQuad;

	DepthStencilStateID m_depthStencilEnabled;
	DepthStencilStateID m_depthStencilDisabled;
	DepthStencilStateID m_depthStencilTestOnly;
	DepthStencilStateID m_depthStencilWriteOnly;

	RenderBuffer* m_colorBuffer;
	RenderBuffer* m_normalBuffer;
	RenderBuffer* m_materialBuffer;
	RenderBuffer* m_depthStencilBuffer;

	TexturePtr m_gBufferColorTexture;
	TexturePtr m_gBufferNormalTexture;
	TexturePtr m_gBufferMaterialTexture;
	TexturePtr m_gBufferDepthTexture;

	SamplerID m_samplerEdgePoint;

	BufferPtr m_cameraBuffer;
	UCamera m_cameraParams;
	Frustum m_frustum;
	Vec2i m_screenSize;

	BufferPtr m_instanceMatBuffer;
	UInstanceMat m_instanceMatStorage;

	BufferPtr m_skinBuffer;
	USkinMat m_skinMatStorage;

	BufferPtr m_instanceMtlBuffer;
	UInstanceMtl m_instanceMtlStorage;
	MaterialPtr m_defaultMtl;
	MaterialParams m_defaultMtlParams;

	BufferPtr m_rasterizerParamsBuffer;
	URasterizerParams m_rasterizerParamsStorage;
	
	DbvtFrustumCallback m_dbvtFrustumCallback;
	RenderContainer m_renderContainer;

	//Array<RenderItem> m_renderItems;
	//Array<RenderNode*> m_renderObjects;

	Array<Light*> m_lights;
	Array<RenderItem> m_solidObjects;
	Array<RenderItem> m_transparentObjects;
	Array<RenderItem> m_tempObjects;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
