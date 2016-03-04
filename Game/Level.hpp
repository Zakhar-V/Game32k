#pragma once

#include "Scene.hpp"

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gLevel Level::Get()

typedef Ptr<class Entity> EntityPtr;

//----------------------------------------------------------------------------//
// 
//----------------------------------------------------------------------------//

/*

//----------------------------------------------------------------------------//
// Entity
//----------------------------------------------------------------------------//

struct EntitySpawnParams
{
	Vec3 pos;
};

class Entity : public Object
{
public:
	OBJECT("Entity");

	Entity(void);
	~Entity(void);

	//void Spawn()

	void AddToLevel(bool _add = true);

protected:
	friend class Level;

	virtual void _OnRemoved(void) { }
	virtual void _OnAdded(void) { }

	Entity* m_prevEntity;
	Entity* m_nextEntity;

	bool m_inLevel;
};

//----------------------------------------------------------------------------//
// Level
//----------------------------------------------------------------------------//

class Level : public Singleton<Level>
{
public:

	Level(void);
	~Level(void);

protected:
	friend class Entity;

	void _AddEntity(Entity* _entity);
	void _RemoveEntity(Entity* _entity);

	Entity* m_entities;
};
*/
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
