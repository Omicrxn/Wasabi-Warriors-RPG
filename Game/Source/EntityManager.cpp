#include "EntityManager.h"

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Map.h"
#include "NPC.h"

#include "Render.h"
#include "Textures.h"
#include "Input.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(Input* input, Render* render, Textures* tex, Collisions* collisions) : Module()
{
	name.Create("entitymanager");
	this->ren = render;
	this->tex = tex;
	this->input = input;
	this->collisions = collisions;
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
		if(entityList.At(i)->data->destroy)
			entityList.Del(entityList.At(i));
	}

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, SString name)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// L13: Create the corresponding type entity
		case EntityType::PLAYER:
			ret = new Player(tex, collisions,this); 
			ret->type = EntityType::PLAYER;
			ret->name = name;
			playerList.Add((Player*)ret);
			break;
		case EntityType::ENEMY: 
			ret = new Enemy(collisions, this);
			ret->type = EntityType::ENEMY;
			ret->name = name;
			enemyList.Add((Enemy*)ret);
			break;
		//case EntityType::ITEM: ret = new Item(); break;
		case EntityType::MAP: 
			ret = new Map(tex); 
			break;
		case EntityType::NPC: 
			ret = new NPC();
			ret->type = EntityType::NPC;
			ret->name = name;
			npcList.Add((NPC*)ret);
			break;
		default: break;
	}

	// Created entities are added to the list
	if (ret != nullptr) entityList.Add(ret);

	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		if (entityList.At(i)->data->id == entity->id)
		{
			entityList.Del(entityList.At(i));
		}
	}
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