#pragma once

#include "GameLogic.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gGame Game::Get()
#define gScene gGame->GetScene()

//----------------------------------------------------------------------------//
// Game
//----------------------------------------------------------------------------//

class Game : public Singleton<Game>
{
public:

	Game(void);
	~Game(void);

	Scene* GetScene(void) { return m_scene; }

protected:

	void _CreateTerrain(void);
	void _CreatePlayer(void);
	void _CreatePilotless(void);
	void _CreateDrone(void);

	int m_rseed;
	Scene* m_scene;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//


