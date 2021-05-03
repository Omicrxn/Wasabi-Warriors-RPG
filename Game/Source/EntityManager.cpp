#include "EntityManager.h"

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Map.h"
#include "NPC.h"
#include "Teleport.h"

#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Transitions.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(Input* input, Render* render, Textures* tex, Collisions* collisions, Transitions* transitions) : Module()
{
	name.Create("entitymanager");
	this->ren = render;
	this->tex = tex;
	this->input = input;
	this->collisions = collisions;
	this->transitions = transitions;
	texture = nullptr;
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

// Called before quitting
bool EntityManager::CleanUp()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		this->DestroyEntity(entityList.At(i)->data);
		/*if (->data->collider != nullptr)
			entityList.At(i)->data->collider->pendingToDelete = true;

		entityList.Del(entityList.At(i));*/
	}
	entityList.Clear();
	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, SString name)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// L13: Create the corresponding type entity
	case EntityType::PLAYER:
		ret = new Player(tex, collisions, this);
		ret->type = EntityType::PLAYER;
		ret->name = name;
		playerList.Add((Player*)ret);
		break;
	case EntityType::ENEMY:
		ret = new Enemy(collisions, this, transitions);
		ret->type = EntityType::ENEMY;
		ret->name = name;
		enemyList.Add((Enemy*)ret);
		break;
		//case EntityType::ITEM: ret = new Item(); break;
	case EntityType::MAP:
		ret = new Map(tex);
		break;
	case EntityType::NPC:
		ret = new NPC(collisions,this);
		ret->type = EntityType::NPC;
		ret->name = name;
		npcList.Add((NPC*)ret);
		break;
	case EntityType::TELEPORT:
		ret = new Teleport(collisions,this);
		ret->type = EntityType::TELEPORT;
		ret->name = name;
		teleportList.Add((Teleport*)ret);
		break;
	case EntityType::ITEM:
		ret = new Item();
		ret->type = EntityType::ITEM;
		ret->name = name;
		itemList.Add((Item*)ret);
		break;
	default: break;
	}

	// Created entities are added to the list
	if (ret != nullptr) entityList.Add(ret);

	if (type == EntityType::ITEM)
	{

	}
	else
		ret->SetTexture(this->texture);

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
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
		// Delete all entities except the map
		if (list1->data->type != EntityType::MAP)
		{
			DestroyEntity(list1->data);
		}
		
		playerList.Clear();
		enemyList.Clear();
		npcList.Clear();
		teleportList.Clear();
	}
	RELEASE(list1);
	/* ---------- SECOND LOAD PLAYERS FROM THE SAVE FILE ----------*/
	pugi::xml_node playerListNode;
	playerListNode = data.child("playerList");
	int playerCount = playerListNode.attribute("playerCount").as_int();

	pugi::xml_node playerNode = playerListNode.child("player");
	Player* player = nullptr;
	for (int i = 0; i < playerCount; ++i)
	{
		player = (Player*)CreateEntity(EntityType::PLAYER, playerNode.attribute("name").as_string());

		player->id = playerNode.attribute("id").as_uint();
		player->spritePos = playerNode.attribute("spritePos").as_int();
		player->position.x = playerNode.attribute("posX").as_int();
		player->position.y = playerNode.attribute("posY").as_int();
		player->renderable = playerNode.attribute("renderable").as_bool();
		player->SetState(playerNode.attribute("isActive").as_bool());

		player->classType = playerNode.attribute("class").as_string();
		player->SetUpClass(player->classType.GetString());

		player->stats.level = playerNode.attribute("level").as_int();
		player->stats.damage = playerNode.attribute("damage").as_int();
		player->stats.maxHP = playerNode.attribute("maxHP").as_int();
		player->stats.currentHP = playerNode.attribute("currentHP").as_int();
		player->stats.strength = playerNode.attribute("strength").as_int();
		player->stats.defense = playerNode.attribute("defense").as_int();
		player->stats.attackSpeed = playerNode.attribute("attackSpeed").as_int();
		player->stats.criticalRate = playerNode.attribute("criticalRate").as_int();
		player->name = playerNode.attribute("statsName").as_string();

		player->SetUpTexture();

		player = nullptr;
		playerNode = playerNode.next_sibling();
	}
	/* ---------- THIRD LOAD ENEMIES FROM THE SAVE FILE ----------*/

	pugi::xml_node enemyListNode;
	enemyListNode = data.child("enemyList");
	int enemyCount = enemyListNode.attribute("enemyCount").as_int();

	pugi::xml_node enemyNode = enemyListNode.child("enemy");
	Enemy* enemy = nullptr;
	for (int i = 0; i < enemyCount; ++i)
	{
		enemy = (Enemy*)CreateEntity(EntityType::ENEMY, enemyNode.attribute("name").as_string());

		enemy->id = enemyNode.attribute("id").as_uint();
		enemy->spritePos = enemyNode.attribute("spritePos").as_int();
		enemy->position.x = enemyNode.attribute("posX").as_int();
		enemy->position.y = enemyNode.attribute("posY").as_int();
		enemy->renderable = enemyNode.attribute("renderable").as_bool();

		enemy->classType = enemyNode.attribute("class").as_string();
		enemy->SetUpClass(enemy->classType.GetString());

		enemy->stats.level = enemyNode.attribute("level").as_int();
		enemy->stats.damage = enemyNode.attribute("damage").as_int();
		enemy->stats.maxHP = enemyNode.attribute("maxHP").as_int();
		enemy->stats.currentHP = enemyNode.attribute("currentHP").as_int();
		enemy->stats.strength = enemyNode.attribute("strength").as_int();
		enemy->stats.defense = enemyNode.attribute("defense").as_int();
		enemy->stats.attackSpeed = enemyNode.attribute("attackSpeed").as_int();
		enemy->stats.criticalRate = enemyNode.attribute("criticalRate").as_int();
		enemy->name = enemyNode.attribute("statsName").as_string();

		enemy->SetUpTexture();
		enemy = nullptr;
		enemyNode = enemyNode.next_sibling();
	}

	/* ---------- FOURTH LOAD NPCs FROM THE SAVE FILE ----------*/
	pugi::xml_node npcListNode;
	npcListNode = data.child("npcList");
	int npcCount = npcListNode.attribute("npcCount").as_int();

	pugi::xml_node npcNode = npcListNode.child("npc");
	NPC* npc = nullptr;
	for (int i = 0; i < npcCount; ++i)
	{
		npc = (NPC*)CreateEntity(EntityType::NPC, npcNode.attribute("name").as_string());

		npc->id = npcNode.attribute("id").as_uint();
		npc->spritePos = npcNode.attribute("spritePos").as_int();
		npc->position.x = npcNode.attribute("posX").as_int();
		npc->position.y = npcNode.attribute("posY").as_int();
		npc->renderable = npcNode.attribute("renderable").as_bool();

		npc->SetUpTexture();
		npc = nullptr;
		npcNode = npcNode.next_sibling();
	}

	pugi::xml_node teleportListNode;
	teleportListNode = data.child("teleportList");
	int teleportCount = teleportListNode.attribute("teleportCount").as_int();

	pugi::xml_node teleportNode = teleportListNode.child("teleport");
	Teleport* teleport = nullptr;
	for (int i = 0; i < teleportCount; ++i)
	{
		teleport = (Teleport*)CreateEntity(EntityType::TELEPORT, teleportNode.attribute("name").as_string());

		teleport->id = teleportNode.attribute("id").as_uint();
		teleport->spritePos = teleportNode.attribute("spritePos").as_int();
		teleport->position.x = teleportNode.attribute("posX").as_int();
		teleport->position.y = teleportNode.attribute("posY").as_int();
		teleport->renderable = teleportNode.attribute("renderable").as_bool();

		/*newTeleportNode.append_attribute("nameTeleport").set_value(list4->data->GetName().GetString());
		newTeleportNode.append_attribute("name").set_value(list4->data->name.GetString());
		newTeleportNode.append_attribute("destination").set_value((int)list4->data->GetDestination());*/

		//teleport->SetName((SString)teleportNode.attribute("nameTeleport").as_string());
		teleport->name = teleportNode.attribute("name").as_string();
		teleport->SetUpDestination((MapType)teleportNode.attribute("destination").as_int());

		teleport->SetUpTexture();
		teleport = nullptr;
		teleportNode = teleportNode.next_sibling();
	}
	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	/* ---------- CHANGE THE ENTITY COUNT FIRST ----------*/
	pugi::xml_node entityManagerNode;
	entityManagerNode = data;
	int count = entityList.Count();
	

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	SString tempName(entityManagerNode.child("entitiesCount").name());
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
	tempName = data.child("playerList").name();
	if (tempName == "playerList")
	{
		// Node playerList exists
		playerListNode = data.child("playerList");
	}
	else
	{
		// Node playerList does not exist
		playerListNode = data.append_child("playerList");
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
		newPlayerNode.append_attribute("class").set_value(list1->data->classType.GetString());
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

	tempName = data.child("enemyList").name();
	if (tempName == "enemyList")
	{
		// Node playerList exists
		enemyListNode = data.child("enemyList");
	}
	else
	{
		// Node playerList does not exist
		enemyListNode = data.append_child("enemyList");
	}

	enemyListNode = data.child("enemyList");
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
		// Node entitiesCount exists
		enemyListNode.attribute("enemyCount").set_value(enemyList.Count());
	}
	else
	{
		// Node entitiesCount does not exist
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
		newEnemyNode.append_attribute("class").set_value(list2->data->classType.GetString());
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

	tempName = data.child("npcList").name();
	if (tempName == "npcList")
	{
		// Node playerList exists
		npcListNode = data.child("npcList");
	}
	else
	{
		// Node playerList does not exist
		npcListNode = data.append_child("npcList");
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
		// Node entitiesCount exists
		npcListNode.attribute("npcCount").set_value(npcList.Count());
	}
	else
	{
		// Node entitiesCount does not exist
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
		newNPCNode.append_attribute("name").set_value(list3->data->name.GetString());
		newNPCNode.append_attribute("posX").set_value(list3->data->position.x);
		newNPCNode.append_attribute("posY").set_value(list3->data->position.y);
		newNPCNode.append_attribute("isActive").set_value(list3->data->IsActive());
		newNPCNode.append_attribute("isRenderable").set_value(list3->data->renderable);
	}

	/* ---------- FOURTH SAVE THE TELEPORTS ----------*/
	// Erase the Teleports in the XML
	pugi::xml_node teleportListNode;

	tempName = data.child("teleportList").name();
	if (tempName == "teleportList")
	{
		// Node playerList exists
		teleportListNode = data.child("teleportList");
	}
	else
	{
		// Node playerList does not exist
		teleportListNode = data.append_child("teleportList");
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
		// Node entitiesCount exists
		teleportListNode.attribute("teleportCount").set_value(teleportList.Count());
	}
	else
	{
		// Node entitiesCount does not exist
		teleportListNode.append_attribute("teleportCount").set_value(teleportList.Count());
	}


	// Add the NPCs in the XML
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

		newTeleportNode.append_attribute("posX").set_value(list4->data->position.x);
		newTeleportNode.append_attribute("posY").set_value(list4->data->position.y);
		newTeleportNode.append_attribute("isActive").set_value(list4->data->IsActive());
		newTeleportNode.append_attribute("isRenderable").set_value(list4->data->renderable);
	}

	return true;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	if(entity->collider != nullptr)
		entity->collider->pendingToDelete = true;

	entityList.Del(entityList.At(entityList.Find(entity)));
	RELEASE(entity);
}

void EntityManager::DestroyEntityChecker(float dt)
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		if (entityList.At(i)->data->destroy == true) entityList.Del(entityList.At(i));
	}
}

bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

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
			entityList.At(i)->data->Update(input,dt);
			entityList.At(i)->data->Draw(ren);
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

		// Delete all entities except the map and player
		if (list1->data->type != EntityType::MAP && list1->data->type != EntityType::PLAYER)
		{
			DestroyEntity(list1->data);
		}

	}
	RELEASE(list1);
}
