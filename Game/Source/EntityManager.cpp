#include "EntityManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Map.h"

#include "Render.h"
#include "Textures.h"
#include "Input.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(Input* input, Render* render, Textures* tex) : Module()
{
	name.Create("entitymanager");
	this->ren = render;
	this->tex = tex;
	this->input = input;
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
	for (int i = 0; i < entities.Count(); i++)
	{
		if(entities.At(i)->data->destroy)
		entities.Del(entities.At(i));
	}

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// L13: Create the corresponding type entity
		case EntityType::PLAYER: ret = new Player(tex);  break;
		case EntityType::ENEMY: ret = new Enemy();  break;
		//case EntityType::ITEM: ret = new Item();  break;
		case EntityType::MAP: ret = new Map(tex);  break;
		default: break;
	}

	// Created entities are added to the list
	if (ret != nullptr) entities.Add(ret);

	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	for (int i = 0; i < entities.Count(); i++)
	{
		if (entities.At(i)->data->id == entity->id)
		{
			entities.Del(entities.At(i));
		}
	}
}

void EntityManager::DestroyEntityChecker(float dt)
{
	for (int i = 0; i < entities.Count(); i++)
	{
		if (entities.At(i)->data->destroy == true) entities.Del(entities.At(i));
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
		for (int i = 0; i < entities.Count(); i++)
		{
			entities.At(i)->data->Update(dt);
			entities.At(i)->data->Update(input,dt);
			entities.At(i)->data->Draw(ren);
		}

	}

	return true;
}

Entity* EntityManager::SearchEntity(SString name, uint32 id)
{
	for (int i = 0; i < entities.Count(); i++)
	{
		if (entities.At(i)->data->name == name)
		{
			return entities.At(i)->data;
		}
	}
	return nullptr;
}