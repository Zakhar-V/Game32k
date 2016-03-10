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
}
//----------------------------------------------------------------------------//
Game::~Game(void)
{
	LOG("Destroy Game");
}
//----------------------------------------------------------------------------//
void Game::_CreateTerrain(void)
{
	TerrainPtr _terrain = new Terrain;
	_terrain->Create(gResources->GetTerrainHeightmap(), 75, 1000, 254);
	_terrain->SetScene(m_scene);
}
//----------------------------------------------------------------------------//
void Game::_CreatePlayer(void)
{
	Ptr<Player> _player = new Player;
	// TODO: set position
	_player->SetScene(m_scene);
}
//----------------------------------------------------------------------------//
void Game::_CreatePilotless(void)
{
	
}
//----------------------------------------------------------------------------//
void Game::_CreateDrone(void)
{
	/*Ptr<Drone> _drone;// = new Drone(&_res, SRand(&_rseed));
	_drone->SetLocalPosition({ SRand(&m_rseed) * 450, 0, SRand(&m_rseed) * 450 });
	//LOG("%f %f %f", _drone->GetWorldPosition().x, _drone->GetWorldPosition().y, _drone->GetWorldPosition().z);
	_drone->SetScene(m_scene);*/
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
