#pragma once
#include "Resources.hpp"
#include "GameSettings.hpp"

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

	// R_MESH_Sphere
	{
		GeometryPtr _geom = new Geometry;
		_geom->CreateSphere(1, 32, 16);
		m_meshes[R_MESH_Sphere] = new Mesh();
		m_meshes[R_MESH_Sphere]->SetGeometry(_geom);
	}

	//R_MESH_DroneBody,
	{
		MaterialPtr _mtl = new Material;
		_mtl->SetShader(FS_NoTexture);

		GeometryPtr _geom = new Geometry;
		_geom->CreateSphere(0.5f, 32, 16);

		MeshPtr _mesh = new Mesh;
		_mesh->SetGeometry(_geom);
		_mesh->SetMaterial(_mtl);
		_mesh->GetMaterialParams().SetColor(G_DRONE_COLOR);
		_mesh->GetMaterialParams().SetShininess(G_DRONE_SHININESS);
		
		m_meshes[R_MESH_DroneBody] = _mesh;
	}

	//R_MESH_DroneEye,
	{
		MaterialPtr _mtl = new Material;
		_mtl->SetShader(FS_NoTexture);

		GeometryPtr _geom = new Geometry;
		_geom->CreateCylinder(0.25f, 0.25f, 32);
		_geom->Transform(Mat44().CreateRotation(Quat().FromAxisAngle(VEC3_UNIT_X, HALF_PI)));


		MeshPtr _mesh = new Mesh;
		_mesh->SetGeometry(_geom);
		_mesh->SetMaterial(_mtl);
		_mesh->GetMaterialParams().SetColor(G_DRONE_EYE_INACTIVE_COLOR);
		_mesh->GetMaterialParams().SetEmission(G_DRONE_EYE_EMISSION);

		m_meshes[R_MESH_DroneEye] = _mesh;
	}

	// R_MESH_Pilotless
	{
		MaterialPtr _mtl = new Material;
		_mtl->SetShader(FS_NoTexture);

		GeometryPtr _geom = new Geometry;
		GeometryPtr _geom2 = new Geometry;
		_geom2->CreateCylinder(0.25f, 2.25f, 16);
		_geom2->Transform(Mat44().CreateRotation(Quat().FromAxisAngle(VEC3_UNIT_X, HALF_PI)));
		//_geom->SetT
		//_geom->Merge(_geom2->Vertices().Ptr(), _geom2->Vertices().Size(), _geom2->Indices().Ptr(), _geom2->Indices().Size());
		
		MeshPtr _mesh = new Mesh;
		_mesh->SetGeometry(_geom2);
		_mesh->SetMaterial(_mtl);
		//_mesh->GetMaterialParams().SetColor(G_DRONE_EYE_INACTIVE_COLOR);
		//_mesh->GetMaterialParams().SetEmission(G_DRONE_EYE_EMISSION);

		m_meshes[R_MESH_Pilotless] = _mesh;
	}

	//,
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
