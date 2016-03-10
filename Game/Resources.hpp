#pragma once

#include "Render.hpp"
#include "Image.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gResources Resources::Get()

//----------------------------------------------------------------------------//
// Resources
//----------------------------------------------------------------------------//

enum MeshResourceID
{
	R_MESH_Sphere,
	R_MESH_DroneBody,
	R_MESH_DroneEye,
	R_MESH_DroneEngine,
	R_MESH_MaxCount,
};

class Resources : public Singleton<Resources>
{
public:

	Resources(void);
	~Resources(void);

	Image* GetTerrainHeightmap(void) { return m_terrainHeightmap; }

protected:

	MeshPtr m_meshes[R_MESH_MaxCount];

	ImagePtr m_terrainHeightmap;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
