#pragma once

#include "Scene.hpp"
#include "Transform.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

class Light;
class Camera;
class Terrain;

//----------------------------------------------------------------------------//
// Camera
//----------------------------------------------------------------------------//

class Camera : public Component
{
public:

protected:

	Camera* m_prev;
	Camera* m_next;
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

class Light : public Component
{
public:

protected:

	Light* m_prev;
	Light* m_next;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class RenderableComponent : public Component
{
public:

protected:

	RenderableComponent* m_prev;
	RenderableComponent* m_next;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class Model : public RenderableComponent
{
public:

protected:

	bool m_skinned;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

class DecalSet : public RenderableComponent
{
public:

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

class Terrain : public RenderableComponent
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

protected:
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
