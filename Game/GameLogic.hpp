#pragma once

#include "Scene.hpp"
#include "GameSettings.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gPlayer Player::Get()
#define gPilotless Pilotless::Get()

enum EntityTag : uint16
{
	ET_DRONE = 0x1,
};

//----------------------------------------------------------------------------//
// Follow
//----------------------------------------------------------------------------//

class Follow : public Behavior
{
public:
	OBJECT("Follow");

	void SetTarget(Node* _target) { m_target = _target; }
	void SetDelay(float _delay) { m_delay = _delay; }

	void Update(const FrameInfo& _frame) override;

protected:

	float m_delay = 0;
	Vec3 m_offset = 0;
	Ref<Node> m_target;
};

//----------------------------------------------------------------------------//
// Patrol
//----------------------------------------------------------------------------//

class Patrol : public Behavior
{
public:
	OBJECT("Patrol");

	void SetVelocity(float _v) { m_velocity = _v; }
	void SetCoeff(float _c) { m_coeff = _c; }

	void Update(const FrameInfo& _frame) override;

protected:

	float m_stage = 0;
	float m_velocity = 1;
	float m_coeff = 1;
	bool m_moveToCenter = false;
};

//----------------------------------------------------------------------------//
// FirstPersonCameraController
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
class FirstPersonCameraController : public Behavior
{
public:
	OBJECT("FirstPersonCameraController");

	void Update(const FrameInfo& _frame) override;

protected:

	float m_yaw = 0;
	float m_pitch = 0;
};
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// PlayerMovementController
//----------------------------------------------------------------------------//

class PlayerMovementController : public Behavior
{
public:
	OBJECT("PlayerMovementController");

	void Update(const FrameInfo& _frame);

protected:

};

//----------------------------------------------------------------------------//
// Bullet
//----------------------------------------------------------------------------//

class Bullet : public Behavior
{
public:
	void Update(const FrameInfo& _frame) override;

protected:
};

//----------------------------------------------------------------------------//
// Character
//----------------------------------------------------------------------------//

class Character : public Entity
{
public:
	OBJECT("Character");

	Character(void);
	~Character(void);
	virtual void Hit(float _hit);
	float GetHealth(void) { return m_health; }

protected:

	void _LogicUpdate(const FrameInfo& _frame) override;
	void _PreUpdateImpl(const FrameInfo& _frame) override;
	void _UpdateImpl(const FrameInfo& _frame) override;
	void _PostUpdateImpl(const FrameInfo& _frame) override;

	float m_health;
	float m_minHeight;
};

//----------------------------------------------------------------------------//
// Player
//----------------------------------------------------------------------------//

class Player : public Character, public Singleton<Player>
{
public:
	OBJECT("Player");

	Player(void);
	~Player(void);

protected:

	void _SetSceneImpl(Scene* _scene) override;
	void _LogicUpdate(const FrameInfo& _frame) override;
	void _PreUpdateImpl(const FrameInfo& _frame) override;
	void _UpdateImpl(const FrameInfo& _frame) override;
	void _PostUpdateImpl(const FrameInfo& _frame) override;

	CameraPtr m_camera;
};

//----------------------------------------------------------------------------//
// Player
//----------------------------------------------------------------------------//

class Drone : public Character
{
public:
	OBJECT("Drone");

	enum State
	{
		S_Initial,
		S_Patrol,
		S_Follow,
		S_Attack,
	};

	Drone(float _rand);
	~Drone(void);

protected:

	void _LogicUpdate(const FrameInfo& _frame) override;
	void _PreUpdateImpl(const FrameInfo& _frame) override;
	void _UpdateImpl(const FrameInfo& _frame) override;
	void _PostUpdateImpl(const FrameInfo& _frame) override;
	void _ChangeState(State _state);

	State m_state;
	float m_rand;
	float m_time;
	float m_lastAttackTime;
	float m_attackStage;

	StaticModelPtr m_eye;
	StaticModelPtr m_sphere;
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
