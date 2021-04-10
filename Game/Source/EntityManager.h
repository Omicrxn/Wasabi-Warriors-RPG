#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"

#include "List.h"

class Render;
class Textures;
class Input;
class Player;
class Enemy;
class NPC;
class Entity;
enum class EntityType;
class Collisions;

class EntityManager : public Module
{
public:

	EntityManager(Input* input, Render* ren, Textures* tex, Collisions* collisions);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, SString name);
	void DestroyEntity(Entity* entity);
	void DestroyEntityChecker(float dt);

	/*void AddEntity(Entity* entity);*/

	bool UpdateAll(float dt, bool doLogic);

	// To search an specific entity in the list
	Entity* SearchEntity(SString name, uint32 id = 0);

	void OnCollision(Collider* c1, Collider* c2);

public:

	Render* ren;
	Textures* tex;
	Input* input;
	Collisions* collisions;
	List<Entity*> entityList;
	List<Player*> playerList;
	List<Enemy*> enemyList;
	List<NPC*> npcList;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __ENTITYMANAGER_H__
