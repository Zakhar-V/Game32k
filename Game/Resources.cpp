#pragma once
#include "Resources.hpp"

//----------------------------------------------------------------------------//
// Resources
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Resources::Resources(void)
{
	LOG("Create Resources");

	// terrain heightmap
	{
		m_terrainHeightmap = new Image;
		m_terrainHeightmap->CreatePerlin(512, 0.05f);
	}

	//R_MESH_Sphere,
	//R_MESH_DroneBody,
	//R_MESH_DroneEye,
	//R_MESH_DroneEngine,
	//R_MESH_MaxCount,
}
//----------------------------------------------------------------------------//
Resources::~Resources(void)
{
	LOG("Destroy Resources");
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
