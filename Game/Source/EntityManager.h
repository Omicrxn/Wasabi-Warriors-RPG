#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"

#include "List.h"
#include "Point.h"
#include "Notifier.h"

#include <vector>

class Render;
class AssetsManager;
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
class Lever;
class SecretWall;
class AudioManager;
class Static;

class EntityManager : public Module
{
public:

	EntityManager(Input* input, Render* ren, Textures* tex, AudioManager *audio, Collisions* collisions, Transitions* transitions, AssetsManager* assetsManager);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//// Load / Save
	//bool LoadState(pugi::xml_node&);
	//bool SaveState(pugi::xml_node&) const;

	bool LoadStateInfo(pugi::xml_node&, MapType currentMap);
	bool SaveStateInfo(pugi::xml_node&, MapType currentMap) const;

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

	// Draws the debug quad of an entity
	void DrawDebugQuad(Entity*);

	// Function that defines which sprite has to be rendered first
	static bool SortByYPos(const Entity* ent1, const Entity* ent2);

public:

	Render* render;
	AssetsManager* assetsManager;
	Textures* tex;
	Input* input;
	AudioManager* audio;
	Collisions* collisions;
	Transitions* transitions;
	List<Entity*> entityList;
	List<Player*> playerList;
	List<Enemy*> enemyList;
	List<NPC*> npcList;
	List<Teleport*> teleportList;
	List<Item*> itemList;
	List<Activator*> activatorList;
	List<Lever*> leverList;
	List<SecretWall*> secretWallList;
	SDL_Texture* texture = nullptr;
	SDL_Texture* itemsTexture = nullptr;
	SDL_Texture* secretWallTexture = nullptr;
	SDL_Texture* entitiesTexture = nullptr;
	SDL_Texture* leversTexture = nullptr;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	// Audio Fx for items
	int consumeFx, pickUpFx;
	int keyFx, footstepPlayerFx, leverFx;

	uint32 totalId = 0;

	// Vector to keep track of the order of the entities
	std::vector<Entity*> entities;
	// Bool for the entities debug
	bool entitiesBox = false;
};

#endif // __ENTITYMANAGER_H__
