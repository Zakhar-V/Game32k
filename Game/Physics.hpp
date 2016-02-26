#pragma once

#include "Scene.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// PhysicsBody
//----------------------------------------------------------------------------//

class PhysicsBody : public Object
{
public:

};

//----------------------------------------------------------------------------//
// PhysicsShape
//----------------------------------------------------------------------------//

enum PhysicsShapeType : uint
{
	PST_Empty,

	PST_Box,
	PST_Sphere,
	PST_Capsule,
	PST_Cyliner,
	PST_Heightfield,
};

class PhysicsShape : public Object
{
public:
};

//----------------------------------------------------------------------------//
// PhysicsJoint
//----------------------------------------------------------------------------//

class PhysicsJoint : public Object
{
public:

};

//----------------------------------------------------------------------------//
// PhysicsWorld
//----------------------------------------------------------------------------//

class PhysicsWorld : public NonCopyable
{
public:
	PhysicsWorld(Scene* _scene);
	~PhysicsWorld(void);

protected:

	Scene* m_scene;
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
