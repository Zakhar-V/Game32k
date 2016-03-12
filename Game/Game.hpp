#pragma once

#include "GameLogic.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gGame Game::Get()
#define gScene gGame->GetScene()
#define gTargetPos gGame->GetTargetPos()

//----------------------------------------------------------------------------//
// Game
//----------------------------------------------------------------------------//

class Game : public Singleton<Game>
{
public:

	Game(void);
	~Game(void);

	Scene* GetScene(void) { return m_scene; }
	const Vec3& GetTargetPos(void) { return m_targetPos; }

	void Update(float _dt);
	void Draw(void);


protected:

	void _CreateTerrain(void);
	void _CreateSky(void);
	void _CreateSun(void);
	void _CreatePlayer(void);
	void _CreatePilotless(void);
	void _CreateDrone(void);

	int m_rseed;
	Scene* m_scene;
	Vec3 m_targetPos;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//


