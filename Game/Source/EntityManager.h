#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"

#include "List.h"
#include "Point.h"

class Render;
class Textures;
struct SDL_Texture;
class Input;
class Player;
class Enemy;
class Teleport;
class NPC;
class Entity;
enum class EntityType;
enum class EntitySubtype;
class Collisions;
class Transitions;
class Item;
class Activator;

class EntityManager : public Module
{
public:

	EntityManager(Input* input, Render* ren, Textures* tex, Collisions* collisions, Transitions* transitions);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	// Additional methods
	Entity* CreateEntity(EntityType type, SString name, EntitySubtype subtype, iPoint position = iPoint(0,0));
	void DestroyEntity(Entity* entity);
	void DestroyEntityChecker(float dt);

	/*void AddEntity(Entity* entity);*/
	bool UpdateAll(float dt, bool doLogic);

	// To search an specific entity in the list
	Entity* SearchEntity(SString name, uint32 id = 0);

	void OnCollision(Collider* c1, Collider* c2);

	void TooglePlayerGodMode();
	void DeleteAllEntitiesExceptPlayer();

public:

	Render* render;
	Textures* tex;
	Input* input;
	Collisions* collisions;
	Transitions* transitions;
	List<Entity*> entityList;
	List<Player*> playerList;
	List<Enemy*> enemyList;
	List<NPC*> npcList;
	List<Teleport*> teleportList;
	List<Item*> itemList;
	List<Activator*> activatorList;
	SDL_Texture* texture;
	SDL_Texture* itemsTexture;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __ENTITYMANAGER_H__
