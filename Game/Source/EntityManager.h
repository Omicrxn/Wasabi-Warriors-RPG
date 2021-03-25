#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"

#include "List.h"
class Render;
class Textures;
class Input;
class EntityManager : public Module
{
public:

	EntityManager(Input* input, Render* ren, Textures* tex);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);
	void DestroyEntityChecker(float dt);

	/*void AddEntity(Entity* entity);*/

	bool UpdateAll(float dt, bool doLogic);

	Entity* SearchEntity(uint32 id, SString name);

public:
	Render* ren;
	Textures* tex;
	Input* input;
	List<Entity*> entities;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __ENTITYMANAGER_H__
