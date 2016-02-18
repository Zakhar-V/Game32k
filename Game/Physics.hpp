#pragma once

#include "Scene.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
// PhysicsBody
//----------------------------------------------------------------------------//

class PhysicsBody : public Component
{
public:
	static ComponentType GetComponentTypeStatic(void) { return CT_PhysicsBody; }
	ComponentType GetComponentType(void) override { return CT_PhysicsBody; }
	bool IsSingleComponent(void) override { return true; }

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

class PhysicsShape : public Component
{
public:
	static ComponentType GetComponentTypeStatic(void) { return CT_PhysicsShape; }
	ComponentType GetComponentType(void) override { return CT_PhysicsShape; }
	uint GetComponentSubType(void) override { return PST_Empty; }
	PhysicsShapeType GetShapeType(void) { return PST_Empty; }
};

//----------------------------------------------------------------------------//
// PhysicsJoint
//----------------------------------------------------------------------------//

class PhysicsJoint : public Component
{
public:
	static ComponentType GetComponentTypeStatic(void) { return CT_PhysicsJoint; }
	ComponentType GetComponentType(void) override { return CT_PhysicsJoint; }

};

//----------------------------------------------------------------------------//
// PhysicsWorld
//----------------------------------------------------------------------------//

class PhysicsWorld : public SceneSystem
{
public:
	PhysicsWorld(Scene* _scene);
	~PhysicsWorld(void);

protected:
};

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//
