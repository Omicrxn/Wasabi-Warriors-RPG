#include "EntityManager.h"

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Map.h"
#include "NPC.h"
#include "Teleport.h"
#include "Activator.h"
#include "SecretWall.h"
#include "Lever.h"

#include "Render.h"
#include "AssetsManager.h"
#include "Textures.h"
#include "Input.h"
#include "Transitions.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(Input* input, Render* render, Textures* tex, AudioManager* audio, Collisions* collisions, Transitions* transitions, AssetsManager* assetsManager) : Module()
{
	name.Create("entitymanager");
	this->render = render;
	this->assetsManager = assetsManager;
	this->tex = tex;
	this->input = input;
	this->collisions = collisions;
	this->transitions = transitions;
	this->audio = audio;
	texture = nullptr;

	// Audio Fx for items
	consumeFx = -1;
	pickUpFx = -1;
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool EntityManager::Start()
{
	LOG("entitymanager start");

	if (itemsTexture == nullptr)
		itemsTexture = tex->Load("Assets/Textures/Items/items_equipment.png");
	if (entitiesTexture == nullptr)
		entitiesTexture = tex->Load("Assets/Textures/Characters/characters_spritesheet.png");
	if (secretWallTexture == nullptr)
		secretWallTexture = tex->Load("Assets/Textures/secret_wall_texture.png");

	consumeFx = audio->LoadFx("Assets/Audio/Fx/consume.ogg");
	pickUpFx = audio->LoadFx("Assets/Audio/Fx/pickup.ogg");

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		// Also delete them from their own lists
		if (entityList.At(i)->data->type == EntityType::PLAYER)
		{
			playerList.Del(playerList.At(playerList.Find((Player*)entityList.At(i)->data)));
		}
		else if (entityList.At(i)->data->type == EntityType::ENEMY)
		{
			enemyList.Del(enemyList.At(enemyList.Find((Enemy*)entityList.At(i)->data)));
		}
		else if (entityList.At(i)->data->type == EntityType::NPC)
		{
			npcList.Del(npcList.At(npcList.Find((NPC*)entityList.At(i)->data)));
		}
		else if (entityList.At(i)->data->type == EntityType::TELEPORT)
		{
			teleportList.Del(teleportList.At(teleportList.Find((Teleport*)entityList.At(i)->data)));
		}
		else if (entityList.At(i)->data->type == EntityType::ITEM)
		{
			itemList.Del(itemList.At(itemList.Find((Item*)entityList.At(i)->data)));
		}
		else if (entityList.At(i)->data->type == EntityType::ACTIVATOR)
		{
			activatorList.Del(activatorList.At(activatorList.Find((Activator*)entityList.At(i)->data)));
		}
		else if (entityList.At(i)->data->type == EntityType::SECRET_WALL)
		{
			//secretWallList.Del(activatorList.At(activatorList.Find((Activator*)entityList.At(i)->data)));
		}
		else if (entityList.At(i)->data->type == EntityType::LEVER)
		{
			leverList.Del(leverList.At(leverList.Find((Lever*)leverList.At(i)->data)));
		}

		this->DestroyEntity(entityList.At(i)->data);
	}
	playerList.Clear();
	enemyList.Clear();
	npcList.Clear();
	teleportList.Clear();
	itemList.Clear();
	activatorList.Clear();
	entityList.Clear();

	// Freeing textures
	tex->UnLoad(itemsTexture);
	itemsTexture = nullptr;
	tex->UnLoad(entitiesTexture);
	entitiesTexture = nullptr;
	tex->UnLoad(secretWallTexture);
	secretWallTexture = nullptr;

	audio->UnloadFx(consumeFx);
	audio->UnloadFx(pickUpFx);

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, SString name, EntitySubtype subtype, iPoint position)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// L13: Create the corresponding type entity
	case EntityType::PLAYER:
		ret = new Player(name, tex, collisions, this,type, subtype,position);
		playerList.Add((Player*)ret);
		break;
	case EntityType::ENEMY:
		ret = new Enemy(name, tex, collisions, this, transitions, type, subtype,position);
		enemyList.Add((Enemy*)ret);
		break;
		//case EntityType::ITEM: ret = new Item(); break;
	case EntityType::MAP:
		ret = new Map(tex, assetsManager);
		break;
	case EntityType::NPC:
		ret = new NPC(name, input, tex, collisions, this, type, subtype, position);
		npcList.Add((NPC*)ret);
		break;
	case EntityType::TELEPORT:
		ret = new Teleport(name, collisions, this,type,position);
		teleportList.Add((Teleport*)ret);
		break;
	case EntityType::ITEM:
		ret = new Item(name, tex, audio, collisions, this, type, subtype,position);
		((Item*)ret)->pickUpFx = pickUpFx;
		((Item*)ret)->consumeFx = consumeFx;
		itemList.Add((Item*)ret);
		break;
	case EntityType::ACTIVATOR:
		ret = new Activator(name, input, tex, collisions, this, type, subtype, position);
		activatorList.Add((Activator*)ret);
		break;
	case EntityType::SECRET_WALL:
		ret = new SecretWall(name,collisions,tex, this, type, position);
		break;
	case EntityType::LEVER:
		ret = new Lever(name,collisions, input,tex, this, type, position);
		leverList.Add((Lever*)ret);
		break;
	default:
		break;
	}

	// Created entities are added to the list
	if (ret != nullptr) entityList.Add(ret);

	return ret;
}

//bool EntityManager::LoadState(pugi::xml_node& data)
//{
//	LOG("START LOAD ENTITYMANAGER");
//	LOG("DELETE ALL");
//	/* ---------- FIRST DELETE ALL THE ENTITIES ----------*/
//	ListItem<Entity*>* list1;
//	for (list1 = entityList.start; list1 != NULL; list1 = list1->next)
//	{
//		// Also delete them from their own lists
//		if (list1->data->type == EntityType::PLAYER)
//		{
//			playerList.Del(playerList.At(playerList.Find((Player*)list1->data)));
//		}
//		else if (list1->data->type == EntityType::ENEMY)
//		{
//			enemyList.Del(enemyList.At(enemyList.Find((Enemy*)list1->data)));
//		}
//		else if (list1->data->type == EntityType::NPC)
//		{
//			npcList.Del(npcList.At(npcList.Find((NPC*)list1->data)));
//		}
//		else if (list1->data->type == EntityType::TELEPORT)
//		{
//			teleportList.Del(teleportList.At(teleportList.Find((Teleport*)list1->data)));
//		}
//		else if (list1->data->type == EntityType::ITEM)
//		{
//			itemList.Del(itemList.At(itemList.Find((Item*)list1->data)));
//		}
//		else if (list1->data->type == EntityType::ACTIVATOR)
//		{
//			activatorList.Del(activatorList.At(activatorList.Find((Activator*)list1->data)));
//		}
//
//		// Delete all entities except the map
//		if (list1->data->type != EntityType::MAP)
//		{
//			DestroyEntity(list1->data);
//		}
//	}
//
//	playerList.Clear();
//	enemyList.Clear();
//	npcList.Clear();
//	teleportList.Clear();
//	itemList.Clear();
//	activatorList.Clear();
//
//	RELEASE(list1);
//
//	LOG("LOAD ALL PLAYERS");
//	/* ---------- SECOND LOAD PLAYERS FROM THE SAVE FILE ----------*/
//	pugi::xml_node playerListNode;
//	playerListNode = data.child("playerList");
//	int playerCount = playerListNode.attribute("playerCount").as_int();
//
//	pugi::xml_node playerNode = playerListNode.child("player");
//	Player* player = nullptr;
//	for (int i = 0; i < playerCount; ++i)
//	{
//		LOG("LOADING PLAYER NUMBER: %i", i);
//		EntitySubtype subtype = (EntitySubtype)playerNode.attribute("entitySubType").as_int();
//		iPoint pos = { playerNode.attribute("posX").as_int(), playerNode.attribute("posY").as_int() };
//		player = (Player*)CreateEntity(EntityType::PLAYER, playerNode.attribute("name").as_string(), subtype, pos);
//
//		player->id = playerNode.attribute("id").as_uint();
//		player->spritePos = playerNode.attribute("spritePos").as_int();
//		player->renderable = playerNode.attribute("renderable").as_bool();
//		player->SetState(playerNode.attribute("isActive").as_bool());
//
//		player->stats.level = playerNode.attribute("level").as_int();
//		player->stats.damage = playerNode.attribute("damage").as_int();
//		player->stats.maxHP = playerNode.attribute("maxHP").as_int();
//		player->stats.currentHP = playerNode.attribute("currentHP").as_int();
//		player->stats.strength = playerNode.attribute("strength").as_int();
//		player->stats.defense = playerNode.attribute("defense").as_int();
//		player->stats.attackSpeed = playerNode.attribute("attackSpeed").as_int();
//		player->stats.criticalRate = playerNode.attribute("criticalRate").as_int();
//		player->name = playerNode.attribute("statsName").as_string();
//
//		player = nullptr;
//		playerNode = playerNode.next_sibling();
//	}
//
//	LOG("LOADING ENEMIES");
//	/* ---------- THIRD LOAD ENEMIES FROM THE SAVE FILE ----------*/
//	pugi::xml_node enemyListNode;
//	enemyListNode = data.child("enemyList");
//	int enemyCount = enemyListNode.attribute("enemyCount").as_int();
//
//	pugi::xml_node enemyNode = enemyListNode.child("enemy");
//	Enemy* enemy = nullptr;
//	for (int i = 0; i < enemyCount; ++i)
//	{
//		LOG("LOADING ENEMY NUMBER: %i", i);
//		EntitySubtype subtype = (EntitySubtype)enemyNode.attribute("entitySubType").as_int();
//		iPoint pos = { enemyNode.attribute("posX").as_int(), enemyNode.attribute("posY").as_int() };
//		enemy = (Enemy*)CreateEntity(EntityType::ENEMY, enemyNode.attribute("name").as_string(), subtype, pos);
//
//		enemy->id = enemyNode.attribute("id").as_uint();
//		enemy->spritePos = enemyNode.attribute("spritePos").as_int();
//		enemy->renderable = enemyNode.attribute("renderable").as_bool();
//
//		enemy->stats.level = enemyNode.attribute("level").as_int();
//		enemy->stats.damage = enemyNode.attribute("damage").as_int();
//		enemy->stats.maxHP = enemyNode.attribute("maxHP").as_int();
//		enemy->stats.currentHP = enemyNode.attribute("currentHP").as_int();
//		enemy->stats.strength = enemyNode.attribute("strength").as_int();
//		enemy->stats.defense = enemyNode.attribute("defense").as_int();
//		enemy->stats.attackSpeed = enemyNode.attribute("attackSpeed").as_int();
//		enemy->stats.criticalRate = enemyNode.attribute("criticalRate").as_int();
//		enemy->name = enemyNode.attribute("statsName").as_string();
//
//		enemy = nullptr;
//		enemyNode = enemyNode.next_sibling();
//	}
//
//	LOG("LOADING NPCs");
//	/* ---------- FOURTH LOAD NPCs FROM THE SAVE FILE ----------*/
//	pugi::xml_node npcListNode;
//	npcListNode = data.child("npcList");
//	int npcCount = npcListNode.attribute("npcCount").as_int();
//
//	pugi::xml_node npcNode = npcListNode.child("npc");
//	NPC* npc = nullptr;
//	for (int i = 0; i < npcCount; ++i)
//	{
//		LOG("LOADING NPC NUMBER: %i", i);
//		iPoint pos = { npcNode.attribute("posX").as_int(), npcNode.attribute("posY").as_int() };
//		npc = (NPC*)CreateEntity(EntityType::NPC, npcNode.attribute("name").as_string(), EntitySubtype::UNKNOWN, pos);
//
//		npc->id = npcNode.attribute("id").as_uint();
//		npc->spritePos = npcNode.attribute("spritePos").as_int();
//		npc->renderable = npcNode.attribute("renderable").as_bool();
//
//		npc->stop = npcNode.attribute("stop").as_bool();
//		if (npc->stop) npc->stopForever = true;
//		npc->dialogIndex = npcNode.attribute("dialogIndex").as_int();
//
//		npc = nullptr;
//		npcNode = npcNode.next_sibling();
//	}
//
//	LOG("LOADING TELEPORT");
//	/* ---------- FIFTH LOAD TELEPORT FROM THE SAVE FILE ----------*/
//	pugi::xml_node teleportListNode;
//	teleportListNode = data.child("teleportList");
//	int teleportCount = teleportListNode.attribute("teleportCount").as_int();
//
//	pugi::xml_node teleportNode = teleportListNode.child("teleport");
//	Teleport* teleport = nullptr;
//	for (int i = 0; i < teleportCount; ++i)
//	{
//		LOG("LOADING TELEPORT NUMBER: %i", i);
//		iPoint pos = { teleportNode.attribute("posX").as_int(), teleportNode.attribute("posY").as_int() };
//		teleport = (Teleport*)CreateEntity(EntityType::TELEPORT, teleportNode.attribute("name").as_string(), EntitySubtype::UNKNOWN, pos);
//
//		teleport->id = teleportNode.attribute("id").as_uint();
//		teleport->spritePos = teleportNode.attribute("spritePos").as_int();
//		teleport->renderable = teleportNode.attribute("renderable").as_bool();
//
//		teleport->name = teleportNode.attribute("name").as_string();
//
//		if (teleportNode.attribute("destination").as_int() == -1)
//		{
//			teleport->SetAsSimpleTP();
//			teleport->SetNextPosition(teleportNode.attribute("nextPosX").as_int(), teleportNode.attribute("nextPosY").as_int());
//		}
//		else
//		{
//			teleport->SetUpDestination((MapType)teleportNode.attribute("destination").as_int());
//		}
//
//		teleport = nullptr;
//		teleportNode = teleportNode.next_sibling();
//	}
//
//	LOG("LOADING ITEMS");
//	/* ---------- SIXTH LOAD ITEMS FROM THE SAVE FILE ----------*/
//	pugi::xml_node itemListNode;
//	itemListNode = data.child("itemList");
//	int itemCount = itemListNode.attribute("itemCount").as_int();
//
//	pugi::xml_node itemNode = itemListNode.child("item");
//	Item* item = nullptr;
//	for (int i = 0; i < itemCount; ++i)
//	{
//		LOG("LOADING ITEM NUMBER: %i", i);
//		EntitySubtype subtype = (EntitySubtype)itemNode.attribute("entitySubType").as_int();
//		iPoint pos = { itemNode.attribute("posX").as_int(), itemNode.attribute("posY").as_int() };
//		item = (Item*)CreateEntity(EntityType::ITEM, itemNode.attribute("name").as_string(), subtype, pos);
//
//		item->id = itemNode.attribute("id").as_uint();
//		item->spritePos = itemNode.attribute("spritePos").as_int();
//		item->renderable = itemNode.attribute("renderable").as_bool();
//		if (itemNode.attribute("onMap"))
//
//		item = nullptr;
//		itemNode = itemNode.next_sibling();
//	}
//
//	LOG("LOADING ACTIVATORS");
//	/* ---------- SEVENTH LOAD ACTIVATORS FROM THE SAVE FILE ----------*/
//	pugi::xml_node activatorListNode;
//	activatorListNode = data.child("activatorList");
//	int activatorCoun = activatorListNode.attribute("activatorCount").as_int();
//
//	pugi::xml_node activatorNode = activatorListNode.child("activator");
//	Activator* activator = nullptr;
//	for (int i = 0; i < itemCount; ++i)
//	{
//		LOG("LOADING ITEM NUMBER: %i", i);
//		EntitySubtype subtype = (EntitySubtype)activatorNode.attribute("entitySubType").as_int();
//		iPoint pos = { activatorNode.attribute("posX").as_int(), activatorNode.attribute("posY").as_int() };
//		activator = (Activator*)CreateEntity(EntityType::ACTIVATOR, activatorNode.attribute("name").as_string(), subtype, pos);
//
//		activator->id = activatorNode.attribute("id").as_uint();
//		activator->spritePos = activatorNode.attribute("spritePos").as_int();
//		activator->renderable = activatorNode.attribute("renderable").as_bool();
//
//		if (activatorNode.attribute("drawState").as_int() != -1) activator->SetDrawState((DrawState)activatorNode.attribute("drawState").as_int());
//
//		activator = nullptr;
//		itemNode = activatorNode.next_sibling();
//	}
//
//	return true;
//}
//
//bool EntityManager::SaveState(pugi::xml_node& data) const
//{
//	/* ---------- CHANGE THE ENTITY COUNT FIRST ----------*/
//	pugi::xml_node entityManagerNode;
//	entityManagerNode = data;
//	int count = entityList.Count();
//	
//
//	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
//	SString tempName(entityManagerNode.child("entitiesCount").name());
//	if (tempName == "entitiesCount")
//	{
//		// Node entitiesCount exists
//		pugi::xml_node entityCountNode = entityManagerNode.child("entitiesCount");
//		entityManagerNode.child("entitiesCount").attribute("Num").set_value(count);
//	}
//	else
//	{
//		// Node entitiesCount does not exist
//		pugi::xml_node entityCountNode = entityManagerNode.append_child("entitiesCount");
//		entityManagerNode.child("entitiesCount").append_attribute("Num").set_value(count);
//	}
//
//	/* ---------- FIRST SAVE THE PLAYERS ----------*/
//	// Erase the players in the XML
//	pugi::xml_node playerListNode;
//	tempName = data.child("playerList").name();
//	if (tempName == "playerList")
//	{
//		// Node playerList exists
//		playerListNode = data.child("playerList");
//	}
//	else
//	{
//		// Node playerList does not exist
//		playerListNode = data.append_child("playerList");
//	}
//
//	for (int i = 0; i < playerListNode.attribute("playerCount").as_int(); ++i)
//	{
//		bool remove = playerListNode.remove_child("player");
//		if (remove == false)
//			break;
//	}
//
//	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
//	SString playerCountName(playerListNode.attribute("playerCount").name());
//	if (playerCountName == "playerCount")
//	{
//		// Node entitiesCount exists
//		playerListNode.attribute("playerCount").set_value(playerList.Count());
//	}
//	else
//	{
//		// Node entitiesCount does not exist
//		playerListNode.append_attribute("playerCount").set_value(playerList.Count());
//	}
//	
//
//	// Add the players in the XML
//	ListItem<Player*>* list1;
//	for (list1 = playerList.start; list1 != NULL; list1 = list1->next)
//	{
//		// Creates a new node for the player
//		pugi::xml_node newPlayerNode = playerListNode;
//		newPlayerNode = newPlayerNode.append_child("player");
//		
//		// Fill in the info in order to save
//		newPlayerNode.append_attribute("id").set_value(list1->data->id);
//		newPlayerNode.append_attribute("spritePos").set_value(list1->data->spritePos);
//		newPlayerNode.append_attribute("name").set_value(list1->data->name.GetString());
//		newPlayerNode.append_attribute("posX").set_value(list1->data->position.x);
//		newPlayerNode.append_attribute("posY").set_value(list1->data->position.y);
//		newPlayerNode.append_attribute("isActive").set_value(list1->data->IsActive());
//		newPlayerNode.append_attribute("isRenderable").set_value(list1->data->renderable);
//
//		// Fill in the stats in order to save
//		newPlayerNode.append_attribute("entitySubType").set_value((int)list1->data->subtype);
//		newPlayerNode.append_attribute("level").set_value(list1->data->stats.level);
//		newPlayerNode.append_attribute("damage").set_value(list1->data->stats.damage);
//		newPlayerNode.append_attribute("maxHP").set_value(list1->data->stats.maxHP);
//		newPlayerNode.append_attribute("currentHP").set_value(list1->data->stats.currentHP);
//		newPlayerNode.append_attribute("strength").set_value(list1->data->stats.strength);
//		newPlayerNode.append_attribute("defense").set_value(list1->data->stats.defense);
//		newPlayerNode.append_attribute("attackSpeed").set_value(list1->data->stats.attackSpeed);
//		newPlayerNode.append_attribute("criticalRate").set_value(list1->data->stats.criticalRate);
//		newPlayerNode.append_attribute("statsName").set_value(list1->data->name.GetString());
//	}
//
//	/* ---------- SECOND SAVE THE ENEMIES ----------*/
//	// Erase the enemies in the XML
//	pugi::xml_node enemyListNode;
//
//	tempName = data.child("enemyList").name();
//	if (tempName == "enemyList")
//	{
//		// Node enemyList exists
//		enemyListNode = data.child("enemyList");
//	}
//	else
//	{
//		// Node enemyList does not exist
//		enemyListNode = data.append_child("enemyList");
//	}
//
//	enemyListNode = data.child("enemyList");
//	for (int i = 0; i < enemyListNode.attribute("enemyCount").as_int(); ++i)
//	{
//		bool remove = enemyListNode.remove_child("enemy");
//		if (remove == false)
//			break;
//	}
//	
//	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
//	tempName = enemyListNode.attribute("enemyCount").name();
//	if (tempName == "enemyCount")
//	{
//		// Node enemyCount exists
//		enemyListNode.attribute("enemyCount").set_value(enemyList.Count());
//	}
//	else
//	{
//		// Node enemyCount does not exist
//		enemyListNode.append_attribute("enemyCount").set_value(enemyList.Count());
//	}
//
//	// Add the enemies in the XML
//	ListItem<Enemy*>* list2;
//	for (list2 = enemyList.start; list2 != NULL; list2 = list2->next)
//	{
//		// Creates a new node for the enemy
//		pugi::xml_node newEnemyNode = enemyListNode;
//		newEnemyNode = newEnemyNode.append_child("enemy");
//
//		// Fill in the info in order to save
//		newEnemyNode.append_attribute("id").set_value(list2->data->id);
//		newEnemyNode.append_attribute("spritePos").set_value(list2->data->spritePos);
//		newEnemyNode.append_attribute("name").set_value(list2->data->name.GetString());
//		newEnemyNode.append_attribute("posX").set_value(list2->data->position.x);
//		newEnemyNode.append_attribute("posY").set_value(list2->data->position.y);
//		newEnemyNode.append_attribute("isActive").set_value(list2->data->IsActive());
//		newEnemyNode.append_attribute("isRenderable").set_value(list2->data->renderable);
//
//		// Fill in the stats in order to save
//		newEnemyNode.append_attribute("entitySubType").set_value((int)list2->data->subtype);
//		newEnemyNode.append_attribute("level").set_value(list2->data->stats.level);
//		newEnemyNode.append_attribute("damage").set_value(list2->data->stats.damage);
//		newEnemyNode.append_attribute("maxHP").set_value(list2->data->stats.maxHP);
//		newEnemyNode.append_attribute("currentHP").set_value(list2->data->stats.currentHP);
//		newEnemyNode.append_attribute("strength").set_value(list2->data->stats.strength);
//		newEnemyNode.append_attribute("defense").set_value(list2->data->stats.defense);
//		newEnemyNode.append_attribute("attackSpeed").set_value(list2->data->stats.attackSpeed);
//		newEnemyNode.append_attribute("criticalRate").set_value(list2->data->stats.criticalRate);
//		newEnemyNode.append_attribute("statsName").set_value(list2->data->name.GetString());
//	}
//
//	/* ---------- THIRD SAVE THE NPCs ----------*/
//	// Erase the NPCs in the XML
//	pugi::xml_node npcListNode;
//
//	tempName = data.child("npcList").name();
//	if (tempName == "npcList")
//	{
//		// Node npcList exists
//		npcListNode = data.child("npcList");
//	}
//	else
//	{
//		// Node npcList does not exist
//		npcListNode = data.append_child("npcList");
//	}
//
//	for (int i = 0; i < npcListNode.attribute("npcCount").as_int(); ++i)
//	{
//		bool remove = npcListNode.remove_child("npc");
//		if (remove == false)
//			break;
//	}
//
//	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
//	tempName = npcListNode.attribute("npcCount").name();
//	if (tempName == "npcCount")
//	{
//		// Node npcCount exists
//		npcListNode.attribute("npcCount").set_value(npcList.Count());
//	}
//	else
//	{
//		// Node npcCount does not exist
//		npcListNode.append_attribute("npcCount").set_value(npcList.Count());
//	}
//
//	// Add the NPCs in the XML
//	ListItem<NPC*>* list3;
//	for (list3 = npcList.start; list3 != NULL; list3 = list3->next)
//	{
//		// Creates a new node for the npc
//		pugi::xml_node newNPCNode = npcListNode;
//		newNPCNode = newNPCNode.append_child("npc");
//
//		// Fill in the info in order to save
//		newNPCNode.append_attribute("id").set_value(list3->data->id);
//		newNPCNode.append_attribute("spritePos").set_value(list3->data->spritePos);
//		newNPCNode.append_attribute("stop").set_value(list3->data->stop);
//		newNPCNode.append_attribute("name").set_value(list3->data->name.GetString());
//		newNPCNode.append_attribute("posX").set_value(list3->data->position.x);
//		newNPCNode.append_attribute("posY").set_value(list3->data->position.y);
//		newNPCNode.append_attribute("isActive").set_value(list3->data->IsActive());
//		newNPCNode.append_attribute("isRenderable").set_value(list3->data->renderable);
//	}
//
//	/* ---------- FOURTH SAVE THE TELEPORTS ----------*/
//	// Erase the Teleports in the XML
//	pugi::xml_node teleportListNode;
//
//	tempName = data.child("teleportList").name();
//	if (tempName == "teleportList")
//	{
//		// Node teleportList exists
//		teleportListNode = data.child("teleportList");
//	}
//	else
//	{
//		// Node teleportList does not exist
//		teleportListNode = data.append_child("teleportList");
//	}
//
//	for (int i = 0; i < teleportListNode.attribute("teleportCount").as_int(); ++i)
//	{
//		bool remove = teleportListNode.remove_child("teleport");
//		if (remove == false)
//			break;
//	}
//
//	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
//	tempName = teleportListNode.attribute("teleportCount").name();
//	if (tempName == "teleportCount")
//	{
//		// Node teleportCount exists
//		teleportListNode.attribute("teleportCount").set_value(teleportList.Count());
//	}
//	else
//	{
//		// Node teleportCount does not exist
//		teleportListNode.append_attribute("teleportCount").set_value(teleportList.Count());
//	}
//
//
//	// Add the Teleports in the XML
//	ListItem<Teleport*>* list4;
//	for (list4 = teleportList.start; list4 != NULL; list4 = list4->next)
//	{
//		// Creates a new node for the npc
//		pugi::xml_node newTeleportNode = teleportListNode;
//		newTeleportNode = newTeleportNode.append_child("teleport");
//
//		// Fill in the info in order to save
//		newTeleportNode.append_attribute("id").set_value(list4->data->id);
//		newTeleportNode.append_attribute("spritePos").set_value(list4->data->spritePos);
//
//		newTeleportNode.append_attribute("nameTeleport").set_value(list4->data->GetName().GetString());
//		newTeleportNode.append_attribute("name").set_value(list4->data->name.GetString());
//		newTeleportNode.append_attribute("destination").set_value((int)list4->data->GetDestination());
//
//		if ((int)list4->data->GetDestination() == -1)
//		{
//			newTeleportNode.append_attribute("nextPosX").set_value((int)list4->data->GetNextPosition().x);
//			newTeleportNode.append_attribute("nextPosY").set_value((int)list4->data->GetNextPosition().y);
//		}
//		
//		newTeleportNode.append_attribute("posX").set_value(list4->data->position.x);
//		newTeleportNode.append_attribute("posY").set_value(list4->data->position.y);
//		newTeleportNode.append_attribute("isActive").set_value(list4->data->IsActive());
//		newTeleportNode.append_attribute("isRenderable").set_value(list4->data->renderable);
//	}
//
//	/* ---------- FIFTH SAVE THE ITEMS ----------*/
//	// Erase the Items in the XML
//	pugi::xml_node itemListNode;
//
//	tempName = data.child("itemList").name();
//	if (tempName == "itemList")
//	{
//		// Node ItemsList exists
//		itemListNode = data.child("itemList");
//	}
//	else
//	{
//		// Node ItemsList does not exist
//		itemListNode = data.append_child("itemList");
//	}
//
//	for (int i = 0; i < itemListNode.attribute("itemCount").as_int(); ++i)
//	{
//		bool remove = itemListNode.remove_child("item");
//		if (remove == false)
//			break;
//	}
//
//	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
//	tempName = itemListNode.attribute("itemCount").name();
//	if (tempName == "itemCount")
//	{
//		// Node Items exists
//		itemListNode.attribute("itemCount").set_value(itemList.Count());
//	}
//	else
//	{
//		// Node Items does not exist
//		itemListNode.append_attribute("itemCount").set_value(itemList.Count());
//	}
//
//	// Add the Items in the XML
//	ListItem<Item*>* list5;
//	int onMapCount = 0;
//	for (list5 = itemList.start; list5 != NULL; list5 = list5->next)
//	{
//		// The load of items in the inventory will be done in the scenegameplay
//		if (list5->data->onMap == false)
//			continue;
//		// Creates a new node for the Items
//		pugi::xml_node newItemNode = itemListNode;
//		newItemNode = newItemNode.append_child("item");
//
//		// Fill in the info in order to save
//		newItemNode.append_attribute("id").set_value(list5->data->id);
//		newItemNode.append_attribute("spritePos").set_value(list5->data->spritePos);
//		newItemNode.append_attribute("entitySubType").set_value((int)list5->data->subtype);
//		newItemNode.append_attribute("name").set_value(list5->data->name.GetString());
//
//		newItemNode.append_attribute("posX").set_value(list5->data->position.x);
//		newItemNode.append_attribute("posY").set_value(list5->data->position.y);
//		newItemNode.append_attribute("isActive").set_value(list5->data->IsActive());
//		newItemNode.append_attribute("isRenderable").set_value(list5->data->renderable);
//
//		++onMapCount;
//	}
//	// Modify again the count to not include the onMap == false items
//	itemListNode.attribute("itemCount").set_value(onMapCount);
//
//	/* ---------- SIXTH SAVE THE ACTIVATOR ----------*/
//	// Erase the Items in the XML
//	pugi::xml_node activatorListNode;
//
//	tempName = data.child("activatorList").name();
//	if (tempName == "activatorList")
//	{
//		// Node ItemsList exists
//		activatorListNode = data.child("activatorList");
//	}
//	else
//	{
//		// Node ItemsList does not exist
//		activatorListNode = data.append_child("activatorList");
//	}
//
//	for (int i = 0; i < activatorListNode.attribute("activatorCount").as_int(); ++i)
//	{
//		bool remove = activatorListNode.remove_child("activator");
//		if (remove == false)
//			break;
//	}
//
//	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
//	tempName = activatorListNode.attribute("activatorCount").name();
//	if (tempName == "activatorCount")
//	{
//		// Node Items exists
//		activatorListNode.attribute("activatorCount").set_value(activatorList.Count());
//	}
//	else
//	{
//		// Node Items does not exist
//		activatorListNode.append_attribute("activatorCount").set_value(activatorList.Count());
//	}
//
//	// Add the Items in the XML
//	ListItem<Activator*>* list6;
//	for (list6 = activatorList.start; list6 != NULL; list6 = list6->next)
//	{
//		// The load of items in the inventory will be done in the scenegameplay
//		// Creates a new node for the Items
//		pugi::xml_node newActivatorNode = activatorListNode;
//		newActivatorNode = newActivatorNode.append_child("activator");
//
//		// Fill in the info in order to save
//		newActivatorNode.append_attribute("id").set_value(list6->data->id);
//		newActivatorNode.append_attribute("spritePos").set_value(list6->data->spritePos);
//		newActivatorNode.append_attribute("entitySubType").set_value((int)list6->data->subtype);
//		newActivatorNode.append_attribute("name").set_value(list6->data->name.GetString());
//
//		newActivatorNode.append_attribute("posX").set_value(list6->data->position.x);
//		newActivatorNode.append_attribute("posY").set_value(list6->data->position.y);
//		newActivatorNode.append_attribute("isActive").set_value(list6->data->IsActive());
//		newActivatorNode.append_attribute("isRenderable").set_value(list6->data->renderable);
//
//		newActivatorNode.append_attribute("drawState").set_value((int)list6->data->GetDrawState());
//	}
//
//	return true;
//}

bool EntityManager::LoadStateInfo(pugi::xml_node& scenegameplay, MapType currentMap)
{
	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	pugi::xml_node mapNode;
	mapNode = scenegameplay.child("map");

	switch (currentMap)
	{
	case MapType::NONE:
		break;
	case MapType::CEMETERY:
		mapNode = mapNode.child("cemetery");
		break;
	case MapType::HOUSE:
		mapNode = mapNode.child("house");
		break;
	case MapType::MEDIUM_CITY:
		mapNode = mapNode.child("mediumCity");
		break;
	case MapType::RESTAURANT:
		mapNode = mapNode.child("restaurant");
		break;
	case MapType::TOWN:
		mapNode = mapNode.child("town");
		break;
	case MapType::BIG_CITY:
		mapNode = mapNode.child("bigCity");
		break;
	case MapType::SKYSCRAPER:
		mapNode = mapNode.child("dungeon");
		break;
	case MapType::SECRET_ROOM:
		mapNode = mapNode.child("secretRoom");
		break;
	default:
		break;
	}

	LOG("START LOAD ENTITYMANAGER");
	LOG("DELETE ALL");
	/* ---------- FIRST DELETE ALL THE ENTITIES ----------*/
	ListItem<Entity*>* list1;
	for (list1 = entityList.start; list1 != NULL; list1 = list1->next)
	{
		// Also delete them from their own lists
		if (list1->data->type == EntityType::PLAYER)
		{
			playerList.Del(playerList.At(playerList.Find((Player*)list1->data)));
		}
		else if (list1->data->type == EntityType::ENEMY)
		{
			enemyList.Del(enemyList.At(enemyList.Find((Enemy*)list1->data)));
		}
		else if (list1->data->type == EntityType::NPC)
		{
			npcList.Del(npcList.At(npcList.Find((NPC*)list1->data)));
		}
		else if (list1->data->type == EntityType::TELEPORT)
		{
			teleportList.Del(teleportList.At(teleportList.Find((Teleport*)list1->data)));
		}
		else if (list1->data->type == EntityType::ITEM)
		{
			itemList.Del(itemList.At(itemList.Find((Item*)list1->data)));
		}
		else if (list1->data->type == EntityType::ACTIVATOR)
		{
			activatorList.Del(activatorList.At(activatorList.Find((Activator*)list1->data)));
		}
		else if (list1->data->type == EntityType::LEVER)
		{
			leverList.Del(leverList.At(leverList.Find((Lever*)list1->data)));
		}
	
		// Delete all entities except the map
		if (list1->data->type != EntityType::MAP)
		{
			DestroyEntity(list1->data);
		}
	}
	
	playerList.Clear();
	enemyList.Clear();
	npcList.Clear();
	teleportList.Clear();
	itemList.Clear();
	activatorList.Clear();
	leverList.Clear();
	
	RELEASE(list1);
	
	LOG("LOAD ALL PLAYERS");
	/* ---------- SECOND LOAD PLAYERS FROM THE SAVE FILE ----------*/
	pugi::xml_node playerListNode;
	playerListNode = mapNode.child("playerList");
	int playerCount = playerListNode.attribute("playerCount").as_int();
	
	pugi::xml_node playerNode = playerListNode.child("player");
	Player* player = nullptr;
	for (int i = 0; i < playerCount; ++i)
	{
		LOG("LOADING PLAYER NUMBER: %i", i);
		EntitySubtype subtype = (EntitySubtype)playerNode.attribute("entitySubType").as_int();
		iPoint pos = { playerNode.attribute("posX").as_int(), playerNode.attribute("posY").as_int() };
		player = (Player*)CreateEntity(EntityType::PLAYER, playerNode.attribute("name").as_string(), subtype, pos);
	
		player->id = 0;
		player->id = playerNode.attribute("id").as_uint();
		player->spritePos = playerNode.attribute("spritePos").as_int();
		player->renderable = playerNode.attribute("renderable").as_bool();
		player->SetState(playerNode.attribute("isActive").as_bool());
	
		player->stats.level = playerNode.attribute("level").as_int();
		player->stats.damage = playerNode.attribute("damage").as_int();
		player->stats.maxHP = playerNode.attribute("maxHP").as_int();
		player->stats.currentHP = playerNode.attribute("currentHP").as_int();
		player->stats.strength = playerNode.attribute("strength").as_int();
		player->stats.defense = playerNode.attribute("defense").as_int();
		player->stats.attackSpeed = playerNode.attribute("attackSpeed").as_int();
		player->stats.criticalRate = playerNode.attribute("criticalRate").as_int();
		player->name = playerNode.attribute("statsName").as_string();
	
		player = nullptr;
		playerNode = playerNode.next_sibling();
	}
	
	LOG("LOADING ENEMIES");
	/* ---------- THIRD LOAD ENEMIES FROM THE SAVE FILE ----------*/
	pugi::xml_node enemyListNode;
	enemyListNode = mapNode.child("enemyList");
	int enemyCount = enemyListNode.attribute("enemyCount").as_int();
	
	pugi::xml_node enemyNode = enemyListNode.child("enemy");
	Enemy* enemy = nullptr;
	for (int i = 0; i < enemyCount; ++i)
	{
		LOG("LOADING ENEMY NUMBER: %i", i);
		EntitySubtype subtype = (EntitySubtype)enemyNode.attribute("entitySubType").as_int();
		iPoint pos = { enemyNode.attribute("posX").as_int(), enemyNode.attribute("posY").as_int() };
		enemy = (Enemy*)CreateEntity(EntityType::ENEMY, enemyNode.attribute("name").as_string(), subtype, pos);
	
		enemy->id = 0;
		enemy->id = enemyNode.attribute("id").as_uint();
		enemy->spritePos = enemyNode.attribute("spritePos").as_int();
		enemy->renderable = enemyNode.attribute("renderable").as_bool();
	
		enemy->stats.level = enemyNode.attribute("level").as_int();
		enemy->stats.damage = enemyNode.attribute("damage").as_int();
		enemy->stats.maxHP = enemyNode.attribute("maxHP").as_int();
		enemy->stats.currentHP = enemyNode.attribute("currentHP").as_int();
		enemy->stats.strength = enemyNode.attribute("strength").as_int();
		enemy->stats.defense = enemyNode.attribute("defense").as_int();
		enemy->stats.attackSpeed = enemyNode.attribute("attackSpeed").as_int();
		enemy->stats.criticalRate = enemyNode.attribute("criticalRate").as_int();
		enemy->name = enemyNode.attribute("statsName").as_string();
	
		enemy = nullptr;
		enemyNode = enemyNode.next_sibling();
	}
	
	LOG("LOADING NPCs");
	/* ---------- FOURTH LOAD NPCs FROM THE SAVE FILE ----------*/
	pugi::xml_node npcListNode;
	npcListNode = mapNode.child("npcList");
	int npcCount = npcListNode.attribute("npcCount").as_int();
	
	pugi::xml_node npcNode = npcListNode.child("npc");
	NPC* npc = nullptr;
	for (int i = 0; i < npcCount; ++i)
	{
		LOG("LOADING NPC NUMBER: %i", i);
		iPoint pos = { npcNode.attribute("posX").as_int(), npcNode.attribute("posY").as_int() };
		npc = (NPC*)CreateEntity(EntityType::NPC, npcNode.attribute("name").as_string(), EntitySubtype::UNKNOWN, pos);
	
		npc->id = 0;
		npc->id = npcNode.attribute("id").as_uint();
		npc->spritePos = npcNode.attribute("spritePos").as_int();
		npc->renderable = npcNode.attribute("renderable").as_bool();
	
		npc->stop = npcNode.attribute("stop").as_bool();
		if (npc->stop) npc->stopForever = true;
		npc->dialogIndex = npcNode.attribute("dialogIndex").as_int();
	
		npc = nullptr;
		npcNode = npcNode.next_sibling();
	}
	
	LOG("LOADING TELEPORT");
	/* ---------- FIFTH LOAD TELEPORT FROM THE SAVE FILE ----------*/
	pugi::xml_node teleportListNode;
	teleportListNode = mapNode.child("teleportList");
	int teleportCount = teleportListNode.attribute("teleportCount").as_int();
	
	pugi::xml_node teleportNode = teleportListNode.child("teleport");
	Teleport* teleport = nullptr;
	for (int i = 0; i < teleportCount; ++i)
	{
		LOG("LOADING TELEPORT NUMBER: %i", i);
		iPoint pos = { teleportNode.attribute("posX").as_int(), teleportNode.attribute("posY").as_int() };
		teleport = (Teleport*)CreateEntity(EntityType::TELEPORT, teleportNode.attribute("name").as_string(), EntitySubtype::UNKNOWN, pos);
	
		teleport->id = teleportNode.attribute("id").as_uint();
		teleport->spritePos = teleportNode.attribute("spritePos").as_int();
		teleport->renderable = teleportNode.attribute("renderable").as_bool();
	
		teleport->name = teleportNode.attribute("name").as_string();
	
		if (teleportNode.attribute("destination").as_int() == -1)
		{
			teleport->SetAsSimpleTP();
			teleport->SetNextPosition(teleportNode.attribute("nextPosX").as_int(), teleportNode.attribute("nextPosY").as_int());
		}
		else
		{
			teleport->SetUpDestination((MapType)teleportNode.attribute("destination").as_int());
		}
	
		teleport = nullptr;
		teleportNode = teleportNode.next_sibling();
	}
	
	LOG("LOADING ITEMS");
	/* ---------- SIXTH LOAD ITEMS FROM THE SAVE FILE ----------*/
	pugi::xml_node itemListNode;
	itemListNode = mapNode.child("itemList");
	int itemCount = itemListNode.attribute("itemCount").as_int();
	
	pugi::xml_node itemNode = itemListNode.child("item");
	Item* item = nullptr;
	for (int i = 0; i < itemCount; ++i)
	{
		LOG("LOADING ITEM NUMBER: %i", i);
		EntitySubtype subtype = (EntitySubtype)itemNode.attribute("entitySubType").as_int();
		iPoint pos = { itemNode.attribute("posX").as_int(), itemNode.attribute("posY").as_int() };
		item = (Item*)CreateEntity(EntityType::ITEM, itemNode.attribute("name").as_string(), subtype, pos);
		
		item->id = 0;
		item->id = itemNode.attribute("id").as_uint();
		item->spritePos = 0;
		item->spritePos = itemNode.attribute("spritePos").as_int();
		item->renderable = itemNode.attribute("renderable").as_bool();
		if (itemNode.attribute("onMap"))
	
		item = nullptr;
		itemNode = itemNode.next_sibling();
	}
	
	LOG("LOADING ACTIVATORS");
	/* ---------- SEVENTH LOAD ACTIVATORS FROM THE SAVE FILE ----------*/
	pugi::xml_node activatorListNode;
	activatorListNode = mapNode.child("activatorList");
	int activatorCount = activatorListNode.attribute("activatorCount").as_int();
	
	pugi::xml_node activatorNode = activatorListNode.child("activator");
	Activator* activator = nullptr;
	for (int i = 0; i < activatorCount; ++i)
	{
		LOG("LOADING ITEM NUMBER: %i", i);
		EntitySubtype subtype = (EntitySubtype)activatorNode.attribute("entitySubType").as_int();
		iPoint pos = { activatorNode.attribute("posX").as_int(), activatorNode.attribute("posY").as_int() };
		activator = (Activator*)CreateEntity(EntityType::ACTIVATOR, activatorNode.attribute("name").as_string(), subtype, pos);
	
		activator->id = activatorNode.attribute("id").as_uint();
		activator->spritePos = activatorNode.attribute("spritePos").as_int();
		activator->renderable = activatorNode.attribute("renderable").as_bool();
	
		DrawState drawState = (DrawState)activatorNode.attribute("drawState").as_int();
		activator->SetDrawState(drawState);
	
		activator = nullptr;
		activatorNode = activatorNode.next_sibling();
	}


	LOG("LOADING LEVERS");
	/* ---------- EIGHT LOAD ACTIVATORS FROM THE SAVE FILE ----------*/
	pugi::xml_node leverListNode;
	leverListNode = mapNode.child("leverList");
	int leverCount = leverListNode.attribute("leverCount").as_int();

	pugi::xml_node leverNode = leverListNode.child("lever");
	Lever* lever = nullptr;
	for (int i = 0; i < leverCount; ++i)
	{
		LOG("LOADING lever NUMBER: %i", i);
		EntitySubtype subtype = (EntitySubtype)leverNode.attribute("entitySubType").as_int();
		iPoint pos = { leverNode.attribute("posX").as_int(), leverNode.attribute("posY").as_int() };
		lever = (Lever*)CreateEntity(EntityType::ACTIVATOR, leverNode.attribute("name").as_string(), subtype, pos);

		lever->id = leverNode.attribute("id").as_uint();
		lever->spritePos = leverNode.attribute("spritePos").as_int();
		lever->renderable = leverNode.attribute("renderable").as_bool();

		lever = nullptr;
		leverNode = leverNode.next_sibling();
	}

	return true;
}

bool EntityManager::SaveStateInfo(pugi::xml_node& scenegameplay, MapType currentMap) const
{
	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	pugi::xml_node mapNode;
	SString tempName = scenegameplay.child("map").name();
	if (tempName == "map")
	{
		mapNode = scenegameplay.child("map");

		tempName = mapNode.attribute("currentMap").name();
		if (tempName == "currentMap")
			mapNode.attribute("currentMap").set_value((int)currentMap);
		else
			mapNode.append_attribute("currentMap").set_value((int)currentMap);
	}
	else
	{
		mapNode = scenegameplay.append_child("map");
		mapNode.append_attribute("currentMap").set_value((int)currentMap);
	}
		

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	switch (currentMap)
	{
	case MapType::NONE:
		break;
	case MapType::CEMETERY:
		tempName = mapNode.child("cemetery").name();
		if (tempName == "cemetery")
			mapNode = mapNode.child("cemetery");
		else
			mapNode = mapNode.append_child("cemetery");
		break;
	case MapType::HOUSE:
		tempName = mapNode.child("house").name();
		if (tempName == "house")
			mapNode = mapNode.child("house");
		else
			mapNode = mapNode.append_child("house");
		break;
	case MapType::MEDIUM_CITY:
		tempName = mapNode.child("mediumCity").name();
		if (tempName == "mediumCity")
			mapNode = mapNode.child("mediumCity");
		else
			mapNode = mapNode.append_child("mediumCity");
		break;
	case MapType::RESTAURANT:
		tempName = mapNode.child("restaurant").name();
		if (tempName == "restaurant")
			mapNode = mapNode.child("restaurant");
		else
			mapNode = mapNode.append_child("restaurant");
		break;
	case MapType::TOWN:
		tempName = mapNode.child("town").name();
		if (tempName == "town")
			mapNode = mapNode.child("town");
		else
			mapNode = mapNode.append_child("town");
		break;
	case MapType::BIG_CITY:
		tempName = mapNode.child("bigCity").name();
		if (tempName == "bigCity")
			mapNode = mapNode.child("bigCity");
		else
			mapNode = mapNode.append_child("bigCity");
		break;
	case MapType::SKYSCRAPER:
		tempName = mapNode.child("dungeon").name();
		if (tempName == "dungeon")
			mapNode = mapNode.child("dungeon");
		else
			mapNode = mapNode.append_child("dungeon");
		break;
	case MapType::SECRET_ROOM:
		tempName = mapNode.child("secretRoom").name();
		if (tempName == "secretRoom")
			mapNode = mapNode.child("secretRoom");
		else
			mapNode = mapNode.append_child("secretRoom");
		break;
	default:
		break;
	}

	/* ---------- CHANGE THE ENTITY COUNT FIRST ----------*/
	pugi::xml_node entityManagerNode;
	entityManagerNode = mapNode;
	int count = entityList.Count();
	

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	tempName = entityManagerNode.child("entitiesCount").name();
	if (tempName == "entitiesCount")
	{
		// Node entitiesCount exists
		pugi::xml_node entityCountNode = entityManagerNode.child("entitiesCount");
		entityManagerNode.child("entitiesCount").attribute("Num").set_value(count);
	}
	else
	{
		// Node entitiesCount does not exist
		pugi::xml_node entityCountNode = entityManagerNode.append_child("entitiesCount");
		entityManagerNode.child("entitiesCount").append_attribute("Num").set_value(count);
	}

	/* ---------- FIRST SAVE THE PLAYERS ----------*/
	// Erase the players in the XML
	pugi::xml_node playerListNode;
	tempName = mapNode.child("playerList").name();
	if (tempName == "playerList")
	{
		// Node playerList exists
		playerListNode = mapNode.child("playerList");
	}
	else
	{
		// Node playerList does not exist
		playerListNode = mapNode.append_child("playerList");
	}

	for (int i = 0; i < playerListNode.attribute("playerCount").as_int(); ++i)
	{
		bool remove = playerListNode.remove_child("player");
		if (remove == false)
			break;
	}

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	SString playerCountName(playerListNode.attribute("playerCount").name());
	if (playerCountName == "playerCount")
	{
		// Node entitiesCount exists
		playerListNode.attribute("playerCount").set_value(playerList.Count());
	}
	else
	{
		// Node entitiesCount does not exist
		playerListNode.append_attribute("playerCount").set_value(playerList.Count());
	}
	

	// Add the players in the XML
	ListItem<Player*>* list1;
	for (list1 = playerList.start; list1 != NULL; list1 = list1->next)
	{
		// Creates a new node for the player
		pugi::xml_node newPlayerNode = playerListNode;
		newPlayerNode = newPlayerNode.append_child("player");
		
		// Fill in the info in order to save
		newPlayerNode.append_attribute("id").set_value(list1->data->id);
		newPlayerNode.append_attribute("spritePos").set_value(list1->data->spritePos);
		newPlayerNode.append_attribute("name").set_value(list1->data->name.GetString());
		newPlayerNode.append_attribute("posX").set_value(list1->data->position.x);
		newPlayerNode.append_attribute("posY").set_value(list1->data->position.y);
		newPlayerNode.append_attribute("isActive").set_value(list1->data->IsActive());
		newPlayerNode.append_attribute("isRenderable").set_value(list1->data->renderable);

		// Fill in the stats in order to save
		newPlayerNode.append_attribute("entitySubType").set_value((int)list1->data->subtype);
		newPlayerNode.append_attribute("level").set_value(list1->data->stats.level);
		newPlayerNode.append_attribute("damage").set_value(list1->data->stats.damage);
		newPlayerNode.append_attribute("maxHP").set_value(list1->data->stats.maxHP);
		newPlayerNode.append_attribute("currentHP").set_value(list1->data->stats.currentHP);
		newPlayerNode.append_attribute("strength").set_value(list1->data->stats.strength);
		newPlayerNode.append_attribute("defense").set_value(list1->data->stats.defense);
		newPlayerNode.append_attribute("attackSpeed").set_value(list1->data->stats.attackSpeed);
		newPlayerNode.append_attribute("criticalRate").set_value(list1->data->stats.criticalRate);
		newPlayerNode.append_attribute("statsName").set_value(list1->data->name.GetString());
	}

	/* ---------- SECOND SAVE THE ENEMIES ----------*/
	// Erase the enemies in the XML
	pugi::xml_node enemyListNode;

	tempName = mapNode.child("enemyList").name();
	if (tempName == "enemyList")
	{
		// Node enemyList exists
		enemyListNode = mapNode.child("enemyList");
	}
	else
	{
		// Node enemyList does not exist
		enemyListNode = mapNode.append_child("enemyList");
	}

	enemyListNode = mapNode.child("enemyList");
	for (int i = 0; i < enemyListNode.attribute("enemyCount").as_int(); ++i)
	{
		bool remove = enemyListNode.remove_child("enemy");
		if (remove == false)
			break;
	}
	
	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	tempName = enemyListNode.attribute("enemyCount").name();
	if (tempName == "enemyCount")
	{
		// Node enemyCount exists
		enemyListNode.attribute("enemyCount").set_value(enemyList.Count());
	}
	else
	{
		// Node enemyCount does not exist
		enemyListNode.append_attribute("enemyCount").set_value(enemyList.Count());
	}

	// Add the enemies in the XML
	ListItem<Enemy*>* list2;
	for (list2 = enemyList.start; list2 != NULL; list2 = list2->next)
	{
		// Creates a new node for the enemy
		pugi::xml_node newEnemyNode = enemyListNode;
		newEnemyNode = newEnemyNode.append_child("enemy");

		// Fill in the info in order to save
		newEnemyNode.append_attribute("id").set_value(list2->data->id);
		newEnemyNode.append_attribute("spritePos").set_value(list2->data->spritePos);
		newEnemyNode.append_attribute("name").set_value(list2->data->name.GetString());
		newEnemyNode.append_attribute("posX").set_value(list2->data->position.x);
		newEnemyNode.append_attribute("posY").set_value(list2->data->position.y);
		newEnemyNode.append_attribute("isActive").set_value(list2->data->IsActive());
		newEnemyNode.append_attribute("isRenderable").set_value(list2->data->renderable);

		// Fill in the stats in order to save
		newEnemyNode.append_attribute("entitySubType").set_value((int)list2->data->subtype);
		newEnemyNode.append_attribute("level").set_value(list2->data->stats.level);
		newEnemyNode.append_attribute("damage").set_value(list2->data->stats.damage);
		newEnemyNode.append_attribute("maxHP").set_value(list2->data->stats.maxHP);
		newEnemyNode.append_attribute("currentHP").set_value(list2->data->stats.currentHP);
		newEnemyNode.append_attribute("strength").set_value(list2->data->stats.strength);
		newEnemyNode.append_attribute("defense").set_value(list2->data->stats.defense);
		newEnemyNode.append_attribute("attackSpeed").set_value(list2->data->stats.attackSpeed);
		newEnemyNode.append_attribute("criticalRate").set_value(list2->data->stats.criticalRate);
		newEnemyNode.append_attribute("statsName").set_value(list2->data->name.GetString());
	}

	/* ---------- THIRD SAVE THE NPCs ----------*/
	// Erase the NPCs in the XML
	pugi::xml_node npcListNode;

	tempName = mapNode.child("npcList").name();
	if (tempName == "npcList")
	{
		// Node npcList exists
		npcListNode = mapNode.child("npcList");
	}
	else
	{
		// Node npcList does not exist
		npcListNode = mapNode.append_child("npcList");
	}

	for (int i = 0; i < npcListNode.attribute("npcCount").as_int(); ++i)
	{
		bool remove = npcListNode.remove_child("npc");
		if (remove == false)
			break;
	}

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	tempName = npcListNode.attribute("npcCount").name();
	if (tempName == "npcCount")
	{
		// Node npcCount exists
		npcListNode.attribute("npcCount").set_value(npcList.Count());
	}
	else
	{
		// Node npcCount does not exist
		npcListNode.append_attribute("npcCount").set_value(npcList.Count());
	}

	// Add the NPCs in the XML
	ListItem<NPC*>* list3;
	for (list3 = npcList.start; list3 != NULL; list3 = list3->next)
	{
		// Creates a new node for the npc
		pugi::xml_node newNPCNode = npcListNode;
		newNPCNode = newNPCNode.append_child("npc");

		// Fill in the info in order to save
		newNPCNode.append_attribute("id").set_value(list3->data->id);
		newNPCNode.append_attribute("spritePos").set_value(list3->data->spritePos);
		newNPCNode.append_attribute("stop").set_value(list3->data->stop);
		newNPCNode.append_attribute("name").set_value(list3->data->name.GetString());
		newNPCNode.append_attribute("posX").set_value(list3->data->position.x);
		newNPCNode.append_attribute("posY").set_value(list3->data->position.y);
		newNPCNode.append_attribute("isActive").set_value(list3->data->IsActive());
		newNPCNode.append_attribute("isRenderable").set_value(list3->data->renderable);
	}

	/* ---------- FOURTH SAVE THE TELEPORTS ----------*/
	// Erase the Teleports in the XML
	pugi::xml_node teleportListNode;

	tempName = mapNode.child("teleportList").name();
	if (tempName == "teleportList")
	{
		// Node teleportList exists
		teleportListNode = mapNode.child("teleportList");
	}
	else
	{
		// Node teleportList does not exist
		teleportListNode = mapNode.append_child("teleportList");
	}

	for (int i = 0; i < teleportListNode.attribute("teleportCount").as_int(); ++i)
	{
		bool remove = teleportListNode.remove_child("teleport");
		if (remove == false)
			break;
	}

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	tempName = teleportListNode.attribute("teleportCount").name();
	if (tempName == "teleportCount")
	{
		// Node teleportCount exists
		teleportListNode.attribute("teleportCount").set_value(teleportList.Count());
	}
	else
	{
		// Node teleportCount does not exist
		teleportListNode.append_attribute("teleportCount").set_value(teleportList.Count());
	}


	// Add the Teleports in the XML
	ListItem<Teleport*>* list4;
	for (list4 = teleportList.start; list4 != NULL; list4 = list4->next)
	{
		// Creates a new node for the npc
		pugi::xml_node newTeleportNode = teleportListNode;
		newTeleportNode = newTeleportNode.append_child("teleport");

		// Fill in the info in order to save
		newTeleportNode.append_attribute("id").set_value(list4->data->id);
		newTeleportNode.append_attribute("spritePos").set_value(list4->data->spritePos);

		newTeleportNode.append_attribute("nameTeleport").set_value(list4->data->GetName().GetString());
		newTeleportNode.append_attribute("name").set_value(list4->data->name.GetString());
		newTeleportNode.append_attribute("destination").set_value((int)list4->data->GetDestination());

		if ((int)list4->data->GetDestination() == -1)
		{
			newTeleportNode.append_attribute("nextPosX").set_value((int)list4->data->GetNextPosition().x);
			newTeleportNode.append_attribute("nextPosY").set_value((int)list4->data->GetNextPosition().y);
		}
		
		newTeleportNode.append_attribute("posX").set_value(list4->data->position.x);
		newTeleportNode.append_attribute("posY").set_value(list4->data->position.y);
		newTeleportNode.append_attribute("isActive").set_value(list4->data->IsActive());
		newTeleportNode.append_attribute("isRenderable").set_value(list4->data->renderable);
	}

	/* ---------- FIFTH SAVE THE ITEMS ----------*/
	// Erase the Items in the XML
	pugi::xml_node itemListNode;

	tempName = mapNode.child("itemList").name();
	if (tempName == "itemList")
	{
		// Node ItemsList exists
		itemListNode = mapNode.child("itemList");
	}
	else
	{
		// Node ItemsList does not exist
		itemListNode = mapNode.append_child("itemList");
	}

	for (int i = 0; i < itemListNode.attribute("itemCount").as_int(); ++i)
	{
		bool remove = itemListNode.remove_child("item");
		if (remove == false)
			break;
	}

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	tempName = itemListNode.attribute("itemCount").name();
	if (tempName == "itemCount")
	{
		// Node Items exists
		itemListNode.attribute("itemCount").set_value(itemList.Count());
	}
	else
	{
		// Node Items does not exist
		itemListNode.append_attribute("itemCount").set_value(itemList.Count());
	}

	// Add the Items in the XML
	ListItem<Item*>* list5;
	int onMapCount = 0;
	for (list5 = itemList.start; list5 != NULL; list5 = list5->next)
	{
		// The load of items in the inventory will be done in the scenegameplay
		if (list5->data->onMap == false)
			continue;
		// Creates a new node for the Items
		pugi::xml_node newItemNode = itemListNode;
		newItemNode = newItemNode.append_child("item");

		// Fill in the info in order to save
		newItemNode.append_attribute("id").set_value(list5->data->id);
		newItemNode.append_attribute("spritePos").set_value(list5->data->spritePos);
		newItemNode.append_attribute("entitySubType").set_value((int)list5->data->subtype);
		newItemNode.append_attribute("name").set_value(list5->data->name.GetString());

		newItemNode.append_attribute("posX").set_value(list5->data->position.x);
		newItemNode.append_attribute("posY").set_value(list5->data->position.y);
		newItemNode.append_attribute("isActive").set_value(list5->data->IsActive());
		newItemNode.append_attribute("isRenderable").set_value(list5->data->renderable);

		++onMapCount;
	}
	// Modify again the count to not include the onMap == false items
	itemListNode.attribute("itemCount").set_value(onMapCount);

	/* ---------- SIXTH SAVE THE ACTIVATOR ----------*/
	// Erase the Items in the XML
	pugi::xml_node activatorListNode;

	tempName = mapNode.child("activatorList").name();
	if (tempName == "activatorList")
	{
		// Node ItemsList exists
		activatorListNode = mapNode.child("activatorList");
	}
	else
	{
		// Node ItemsList does not exist
		activatorListNode = mapNode.append_child("activatorList");
	}

	for (int i = 0; i < activatorListNode.attribute("activatorCount").as_int(); ++i)
	{
		bool remove = activatorListNode.remove_child("activator");
		if (remove == false)
			break;
	}

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	tempName = activatorListNode.attribute("activatorCount").name();
	if (tempName == "activatorCount")
	{
		// Node Items exists
		activatorListNode.attribute("activatorCount").set_value(activatorList.Count());
	}
	else
	{
		// Node Items does not exist
		activatorListNode.append_attribute("activatorCount").set_value(activatorList.Count());
	}

	// Add the Items in the XML
	ListItem<Activator*>* list6;
	for (list6 = activatorList.start; list6 != NULL; list6 = list6->next)
	{
		// The load of items in the inventory will be done in the scenegameplay
		// Creates a new node for the Items
		pugi::xml_node newActivatorNode = activatorListNode;
		newActivatorNode = newActivatorNode.append_child("activator");

		// Fill in the info in order to save
		newActivatorNode.append_attribute("id").set_value(list6->data->id);
		newActivatorNode.append_attribute("spritePos").set_value(list6->data->spritePos);
		newActivatorNode.append_attribute("entitySubType").set_value((int)list6->data->subtype);
		newActivatorNode.append_attribute("name").set_value(list6->data->name.GetString());

		newActivatorNode.append_attribute("posX").set_value(list6->data->position.x);
		newActivatorNode.append_attribute("posY").set_value(list6->data->position.y);
		newActivatorNode.append_attribute("isActive").set_value(list6->data->IsActive());
		newActivatorNode.append_attribute("isRenderable").set_value(list6->data->renderable);

		newActivatorNode.append_attribute("drawState").set_value((int)list6->data->GetDrawState());
	}

	/* ---------- SEVENTH SAVE THE ACTIVATOR ----------*/
	// Erase the Items in the XML
	pugi::xml_node leverListNode;

	tempName = mapNode.child("leverList").name();
	if (tempName == "leverList")
	{
		// Node ItemsList exists
		leverListNode = mapNode.child("leverList");
	}
	else
	{
		// Node ItemsList does not exist
		leverListNode = mapNode.append_child("leverList");
	}

	for (int i = 0; i < leverListNode.attribute("leverCount").as_int(); ++i)
	{
		bool remove = leverListNode.remove_child("lever");
		if (remove == false)
			break;
	}

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	tempName = leverListNode.attribute("leverCount").name();
	if (tempName == "leverCount")
	{
		// Node Items exists
		activatorListNode.attribute("leverCount").set_value(leverList.Count());
	}
	else
	{
		// Node Items does not exist
		activatorListNode.append_attribute("leverCount").set_value(leverList.Count());
	}

	// Add the Items in the XML
	ListItem<Lever*>* list7;
	for (list7 = leverList.start; list7 != NULL; list7 = list7->next)
	{
		// The load of items in the inventory will be done in the scenegameplay
		// Creates a new node for the Items
		pugi::xml_node newLeverNode = leverListNode;
		newLeverNode = newLeverNode.append_child("lever");

		// Fill in the info in order to save
		newLeverNode.append_attribute("id").set_value(list7->data->id);
		newLeverNode.append_attribute("spritePos").set_value(list7->data->spritePos);
		newLeverNode.append_attribute("entitySubType").set_value((int)list7->data->subtype);
		newLeverNode.append_attribute("name").set_value(list7->data->name.GetString());

		newLeverNode.append_attribute("posX").set_value(list7->data->position.x);
		newLeverNode.append_attribute("posY").set_value(list7->data->position.y);
		newLeverNode.append_attribute("isActive").set_value(list7->data->IsActive());
		newLeverNode.append_attribute("isRenderable").set_value(list7->data->renderable);
		newLeverNode.append_attribute("number").set_value(list7->data->GetNumber());
		newLeverNode.append_attribute("width").set_value(list7->data->rect.w);
		newLeverNode.append_attribute("width").set_value(list7->data->rect.h);
	}
	return true;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	if (entity->collider != nullptr)
	{
		entity->collider->pendingToDelete = true;
		entity->collider = nullptr;
	}
	entityList.Del(entityList.At(entityList.Find(entity)));
	RELEASE(entity);
}

void EntityManager::DestroyEntityChecker(float dt)
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		if (entityList.At(i)->data->destroy == true)
		{
			if (entityList.At(i)->data->collider != nullptr)
			{
				entityList.At(i)->data->collider->pendingToDelete = true;
				entityList.At(i)->data->collider = nullptr;
			}

			entityList.Del(entityList.At(i));
		}
	}
}

bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	if(entityList.Count() != 0) UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	DestroyEntityChecker(dt);

	return true;
}

bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		// TODO: Update all entities
		for (int i = 0; i < entityList.Count(); i++)
		{
			entityList.At(i)->data->Update(dt);
			entityList.At(i)->data->Update(input, dt);
			entityList.At(i)->data->Draw(render);
		}
	}

	return true;
}

Entity* EntityManager::SearchEntity(SString name, uint32 id)
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		if (entityList.At(i)->data->name == name)
		{
			return entityList.At(i)->data;
		}
	}
	return nullptr;
}

void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (ListItem<Entity*>* i = entityList.start; i != NULL; i = i->next)
	{
		if (i->data->GetCollider() == c1)
		{
			i->data->OnCollision(c2);
		}
	}
}

void EntityManager::TooglePlayerGodMode()
{
	for (int i = 0; i < playerList.Count(); i++)
	{
		playerList.At(i)->data->isGod = !playerList.At(i)->data->isGod;
	}
}

void EntityManager::DeleteAllEntitiesExceptPlayer()
{
	ListItem<Entity*>* list1;
	for (list1 = entityList.start; list1 != NULL; list1 = list1->next)
	{
		if (list1->data->type == EntityType::ENEMY)
		{
			enemyList.Del(enemyList.At(enemyList.Find((Enemy*)list1->data)));
		}
		else if (list1->data->type == EntityType::NPC)
		{
			npcList.Del(npcList.At(npcList.Find((NPC*)list1->data)));
		}
		else if (list1->data->type == EntityType::TELEPORT)
		{
			teleportList.Del(teleportList.At(teleportList.Find((Teleport*)list1->data)));
		}
		else if (list1->data->type == EntityType::ITEM)
		{
			itemList.Del(itemList.At(itemList.Find((Item*)list1->data)));
		}

		// Delete all entities except the map and player
		if (list1->data->type != EntityType::MAP && list1->data->type != EntityType::PLAYER)
		{
			if (list1->data->type == EntityType::ACTIVATOR)
			{
				if (((Activator*)list1->data)->GetDrawState() == DrawState::NONE)
					DestroyEntity(list1->data);
			}
			else if (list1->data->type != EntityType::ACTIVATOR)
			{
				DestroyEntity(list1->data);
			}
		}
	}
	RELEASE(list1);
}
