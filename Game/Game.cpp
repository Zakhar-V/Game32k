#pragma once

#include "Game.hpp"
#include "Resources.hpp"

//----------------------------------------------------------------------------//
// Game
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Game::Game(void) :
	m_rseed(0)
{
	LOG("Create Game");

	m_scene = new Scene;

	_CreateTerrain();
	_CreateSky();
	_CreateSun();
	_CreatePilotless();
	_CreatePlayer();

	for (uint i = 0; i < 500; ++i)
	{
		_CreateDrone();
	}
}
//----------------------------------------------------------------------------//
Game::~Game(void)
{
	LOG("Destroy Game");
}
//----------------------------------------------------------------------------//
void Game::Update(float _dt)
{
	m_scene->Update(_dt);
}
//----------------------------------------------------------------------------//
void Game::Draw(void)
{
	gRenderer->Draw(m_scene);
}
//----------------------------------------------------------------------------//
void Game::_CreateTerrain(void)
{
	TerrainPtr _terrain = new Terrain;
	_terrain->Create(gResources->GetTerrainHeightmap(), 50, (G_TERRAIN_RADIUS + G_SKY_RADIUS + G_SKY_RADIUS * 0.5f) * 2, 254, 50);	// radius + sky_radius + x
	_terrain->SetScene(m_scene);
	_terrain->GetMaterial()->SetShader(FS_Texture);
	_terrain->GetMaterialParams().SetColor(G_TERRAIN_COLOR);
	_terrain->GetMaterialParams().SetShininess(G_TERRAIN_SHININESS);
}
//----------------------------------------------------------------------------//
void Game::_CreateSky(void)
{
	SkyDomePtr _sky = new SkyDome;
	_sky->Create(G_SKY_RADIUS);

	MaterialPtr _mtl = new Material;
	_sky->SetMaterial(_mtl);
	_sky->GetMaterialParams().SetColor(G_SKY_COLOR);
	_sky->GetMaterialParams().SetEmission(1);

	_sky->SetScene(m_scene);
}
//----------------------------------------------------------------------------//
void Game::_CreateSun(void)
{
	Ptr<DirectionalLight> _light = new DirectionalLight;
	//_light->SetParams();
	_light->SetScene(m_scene);
}
//----------------------------------------------------------------------------//
void Game::_CreatePlayer(void)
{
	Ptr<Player> _player = new Player;
	// TODO: set position
	_player->SetScene(m_scene);
	_player->SetWorldPosition({450, 0, 450});
}
//----------------------------------------------------------------------------//
void Game::_CreatePilotless(void)
{
	ModelPtr _mdl = new StaticModel;
	_mdl->SetMesh(gResources->GetMesh(R_MESH_Pilotless));
	float _height = m_scene->GetTerrainHeight(0, 0);
	m_targetPos.Set(0, _height, 0);
	_mdl->SetWorldPosition(m_targetPos);
	_mdl->SetScene(m_scene);
}
//----------------------------------------------------------------------------//
void Game::_CreateDrone(void)
{
	Ptr<Drone> _drone = new Drone(SRand(&m_rseed));
	_drone->SetLocalPosition({ SRand(&m_rseed) * (G_TERRAIN_RADIUS - 50), 0, SRand(&m_rseed) * (G_TERRAIN_RADIUS - 50) });
	_drone->SetScene(m_scene);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
