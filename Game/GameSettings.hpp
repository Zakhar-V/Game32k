#pragma once

#include "Object.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gSettings Settings::Get()

#define G_DRONE_HEIGHT 2.75f
#define G_PLAYER_HEIGHT 1.75f
#define G_PLAYER_HEIGHT_LOW 0.75f
#define G_CHARACTER_VELOCITY 20000.f / 3600.f // 20 km/h
#define G_PLAYER_VELOCITY 20100.f / 3600.f // 20.1 km/h 
#define G_PLAYER_SPRINT_VELOCITY_MULTIPLIER 2.5f
#define G_PLAYER_STEP_VELOCITY_MULTIPLIER 0.5f

#define G_DRONE_SCAN_RADIUS 15
#define G_DRON_ATTACK_RADIUS 10.5f
#define G_DRONE_ATTACK_INTERVAL 3.f


#define G_TERRAIN_RADIUS 500.0f

#define G_TERRAIN_COLOR {0.3f, 0.3f, 0.3f}
#define G_TERRAIN_SHININESS 0.4f

#define G_DRONE_SPHERE_TRANSPARENCY 0.15f
#define G_DRONE_EYE_INACTIVE_COLOR { 0.0f, 0.0f, 0.9f }
#define G_DRONE_EYE_ACTIVE_COLOR { 0.9f, 0.0f, 0.0f }
#define G_DRONE_EYE_EMISSION 0.7f

#define G_DRONE_COLOR { 0.2f, 0.2f, 0.2f }
#define G_DRONE_SHININESS 0.7f

#define G_SKY_RADIUS 200.f
#define G_SKY_COLOR { 0.0f, 0.0f, 0.1f }

//----------------------------------------------------------------------------//
// Settings
//----------------------------------------------------------------------------//

struct Settings : public Singleton<Settings>
{
	float invCameraH = 1;
	float invCameraV = 1;
	float mouseSens = 1.5;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
