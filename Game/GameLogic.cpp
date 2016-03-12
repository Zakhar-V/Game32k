#pragma once 

#include "GameLogic.hpp"
#include "Resources.hpp"
#include "Device.hpp"

//----------------------------------------------------------------------------//
// Follow
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void Follow::Update(const FrameInfo& _frame)
{
	if (!m_target)
		return;

	Vec3 _pos = m_node->GetWorldPosition();
	Vec3 _targetPos = m_target->GetWorldPosition();
	Vec3 _dir = _targetPos-_pos;
	m_node->SetWorldPosition(_pos + (_dir / m_delay) * _frame.time);
	m_node->SetWorldRotation(Quat().FromLookRotation(_dir));
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Patrol
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void Patrol::Update(const FrameInfo& _frame) 
{
	Vec3 _dir, _pos = m_node->GetWorldPosition();

	float _dist = _pos.Distance(0);
	if (_dist > G_TERRAIN_RADIUS)
		m_moveToCenter = true;
	else if (_dist < G_TERRAIN_RADIUS * 0.8f)
		m_moveToCenter = false;

	if (m_moveToCenter)
		_dir = (-_pos).Normalize();
	else
		_dir = Vec3(Cos(m_stage), 0, Sin(m_stage * 0.5f));

	m_stage += _frame.time * m_coeff;
	_pos += _dir * m_velocity * _frame.time;
	m_node->SetWorldPosition(_pos);
	m_node->SetWorldRotation(Quat().FromLookRotation(_dir));
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// FirstPersonCameraController
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void FirstPersonCameraController::Update(const FrameInfo& _frame)
{
	Quat _r = m_node->GetLocalRotation();
	Vec2 _rot = gDevice->GetCameraDelta() * Vec2(gSettings->invCameraH, gSettings->invCameraV) * gSettings->mouseSens * _frame.time;
	if (_rot.x || _rot.y)
	{
		m_pitch = Clamp<float>(m_pitch + _rot.y, -90, 90);
		m_yaw += _rot.x;

		Quat _rx, _ry;
		_rx.FromAxisAngle(VEC3_UNIT_X, m_pitch * RADIANS);
		_ry.FromAxisAngle(VEC3_UNIT_Y, m_yaw * RADIANS);

		Vec3 _dir = -VEC3_UNIT_Z * _r;

		m_node->SetLocalRotation(_rx * _ry);
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// PlayerMovementController
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void PlayerMovementController::Update(const FrameInfo& _frame)
{
	Vec3 _vec = 0;
	if ((GetAsyncKeyState('W') & 0x8000) != 0)
		_vec.z -= 1;
	if ((GetAsyncKeyState('S') & 0x8000) != 0)
		_vec.z += 1;

	if ((GetAsyncKeyState('A') & 0x8000) != 0)
		_vec.x -= 1;
	if ((GetAsyncKeyState('D') & 0x8000) != 0)
		_vec.x += 1;

	if ((GetAsyncKeyState('Q') & 0x8000) != 0)
		_vec.y -= 1;
	if ((GetAsyncKeyState('E') & 0x8000) != 0)
		_vec.y += 1;

	// TODO

	float _speed = G_PLAYER_VELOCITY;
	if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
		_speed *= G_PLAYER_SPRINT_VELOCITY_MULTIPLIER;
	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0)
		_speed *= G_PLAYER_STEP_VELOCITY_MULTIPLIER;
	_vec *= _speed;

	Vec3 _dir = _vec * m_node->GetWorldRotation().Copy().Inverse();
	_dir.y = 0; // no flying
	Vec3 _pos = m_node->GetWorldPosition();
	_pos += _dir * _frame.time;
	_pos.x = Clamp<float>(_pos.x, -G_TERRAIN_RADIUS, G_TERRAIN_RADIUS);
	_pos.z = Clamp<float>(_pos.z, -G_TERRAIN_RADIUS, G_TERRAIN_RADIUS);
	m_node->SetWorldPosition(_pos);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Bullet
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void Bullet::Update(const FrameInfo& _frame)
{
	// m_scene->GetNodes()
	// foreach ... if(...) _node->GetEntity()->SetScene(0)
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Character
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Character::Character(void) :
	m_health(100)
{
	m_minHeight = 2;
}
//----------------------------------------------------------------------------//
Character::~Character(void)
{
}
//----------------------------------------------------------------------------//
void Character::Hit(float _hit)
{
	m_health -= _hit;
}
//----------------------------------------------------------------------------//
void Character::_LogicUpdate(const FrameInfo& _frame)
{
}
//----------------------------------------------------------------------------//
void Character::_PreUpdateImpl(const FrameInfo& _frame)
{
}
//----------------------------------------------------------------------------//
void Character::_UpdateImpl(const FrameInfo& _frame)
{
	Vec3 _pos = GetWorldPosition();
	//_pos.y = Max<float>(_pos.y, m_scene->GetTerrainHeight(_pos.x, _pos.z) + m_minHeight);
	_pos.y = m_scene->GetTerrainHeight(_pos.x, _pos.z) + m_minHeight;
	SetWorldPosition(_pos);
}
//----------------------------------------------------------------------------//
void Character::_PostUpdateImpl(const FrameInfo& _frame)
{
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Player
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Player::Player(void)
{
	m_minHeight = G_PLAYER_HEIGHT;
	m_camera = new Camera;
	m_camera->SetParent(this);
	m_camera->SetInheritRotation(false);
	m_camera->AddBehavior(new FirstPersonCameraController);
	AddBehavior(new PlayerMovementController);
}
//----------------------------------------------------------------------------//
Player::~Player(void)
{
}
//----------------------------------------------------------------------------//
void Player::_SetSceneImpl(Scene* _scene)
{
	m_scene = _scene;
	if (m_scene)
		m_scene->SetActiveCamera(m_camera);
}
//----------------------------------------------------------------------------//
void Player::_LogicUpdate(const FrameInfo& _frame)
{
	Character::_LogicUpdate(_frame);

	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0)
		m_minHeight = G_PLAYER_HEIGHT_LOW;
	else
		m_minHeight = G_PLAYER_HEIGHT;
}
//----------------------------------------------------------------------------//
void Player::_PreUpdateImpl(const FrameInfo& _frame)
{
	Character::_PreUpdateImpl(_frame);
}
//----------------------------------------------------------------------------//
void Player::_UpdateImpl(const FrameInfo& _frame)
{
	Character::_UpdateImpl(_frame);

	Vec3 _dir = m_camera->GetWorldDirection();
	_dir.y = 0; // no flying
	SetWorldDirection(_dir);
}
//----------------------------------------------------------------------------//
void Player::_PostUpdateImpl(const FrameInfo& _frame)
{
	Character::_PostUpdateImpl(_frame);
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Drone
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
Drone::Drone(float _rand)
{
	m_state = S_Initial;
	m_rand = _rand;
	m_health = 10;
	m_minHeight = G_DRONE_HEIGHT;
	m_time = _rand;
	m_lastAttackTime = 0;
	m_attackStage = 0;

	StaticModelPtr _mdl = new StaticModel;
	_mdl->SetMesh(gResources->GetMesh(R_MESH_DroneBody));
	_mdl->SetParent(this);
	_mdl->_SetEntity(this);
	_mdl->SetTags(ET_DRONE);

	m_eye = new StaticModel;
	m_eye->SetMesh(gResources->GetMesh(R_MESH_DroneEye));
	m_eye->SetParent(_mdl);
	m_eye->SetLocalPosition({0, 0, 0.4f});
	m_eye->_SetEntity(this);

	m_sphere = new StaticModel;
	m_sphere->SetMesh(gResources->GetMesh(R_MESH_Sphere));
	m_sphere->Disable();
	m_sphere->SetParent(this);
	m_sphere->_SetEntity(this);

	_ChangeState(S_Patrol);
}
//----------------------------------------------------------------------------//
Drone::~Drone(void)
{
}
//----------------------------------------------------------------------------//
void Drone::_LogicUpdate(const FrameInfo& _frame)
{
	Character::_LogicUpdate(_frame);

	Vec3 _playerPos = gPlayer->GetWorldPosition();
	Vec3 _pos = GetWorldPosition();
	float _dist = _pos.Distance(_playerPos);
	m_time += _frame.time;

	switch (m_state)
	{
	case S_Patrol:
	{
		if (_dist < G_DRONE_SCAN_RADIUS)
			_ChangeState(S_Follow);
	} break;
	case S_Follow:
	{
		if (_dist > G_DRONE_SCAN_RADIUS)
			_ChangeState(S_Patrol);
	}break;
	}


	m_attackStage += G_DRONE_ATTACK_INTERVAL * _frame.time;
	m_attackStage = Clamp<float>(m_attackStage, 0, 1);
	m_attackStage = (m_time - m_lastAttackTime) / G_DRONE_ATTACK_INTERVAL;

	if (m_time - m_lastAttackTime > G_DRONE_ATTACK_INTERVAL)
	{
		m_lastAttackTime = m_time;
		m_attackStage = 0;
	}

	//printf("statge %f\n", m_attackStage);

	float _maxRadius = 0;

	if (m_state == S_Patrol)
	{
		_maxRadius = G_DRONE_SCAN_RADIUS;
	}
	else
	{
		_maxRadius = G_DRON_ATTACK_RADIUS;
		if (_dist < G_DRON_ATTACK_RADIUS)
		{
			if (m_attackStage == 0)
			{
				gPlayer->Hit(5);
			}
		}
	}

	float _radius = _maxRadius * m_attackStage;

	if (_radius > 0)
	{
		m_sphere->Enable();
		m_sphere->SetWorldScale(_radius);
		m_sphere->GetMaterialParams().SetTransparency((1 - m_attackStage) * G_DRONE_SPHERE_TRANSPARENCY);
	}
	else
	{
		m_sphere->Disable();
	}

	//m_sphere->Disable();  // temp
}
//----------------------------------------------------------------------------//
void Drone::_PreUpdateImpl(const FrameInfo& _frame)
{
	Character::_PreUpdateImpl(_frame);
}
//----------------------------------------------------------------------------//
void Drone::_UpdateImpl(const FrameInfo& _frame)
{
	Character::_UpdateImpl(_frame);
}
//----------------------------------------------------------------------------//
void Drone::_PostUpdateImpl(const FrameInfo& _frame)
{
	Character::_PostUpdateImpl(_frame);
}
//----------------------------------------------------------------------------//
void Drone::_ChangeState(State _state)
{
	if (m_state == _state)
		return;

	switch (m_state)
	{
	case S_Patrol:
		RemoveBehavior(GetBehavior<Patrol>());
		break;
	case S_Follow:
		RemoveBehavior(GetBehavior<Follow>());
		break;
	}

	m_state = _state;

	switch (m_state)
	{
	case S_Patrol:
	{
		Ptr<Patrol>	_patrol = new Patrol;
		_patrol->SetVelocity(7);
		_patrol->SetCoeff(0.1f + m_rand);
		AddBehavior(_patrol);

		m_eye->GetMaterialParams().SetColor(G_DRONE_EYE_INACTIVE_COLOR);
		m_sphere->GetMaterialParams().SetColor(G_DRONE_EYE_INACTIVE_COLOR);

	} break;
	case S_Follow:
	{
		Ptr<Follow>	_follow = new Follow;
		_follow->SetTarget(gPlayer);
		_follow->SetDelay(1);
		AddBehavior(_follow);

		m_eye->GetMaterialParams().SetColor(G_DRONE_EYE_ACTIVE_COLOR);
		m_sphere->GetMaterialParams().SetColor(G_DRONE_EYE_ACTIVE_COLOR);

	}break;
	}
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
