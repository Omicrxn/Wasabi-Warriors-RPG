#include "Static.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"
#include "Map.h"
#include "EntityManager.h"

Static::Static(std::string name, Textures* tex, EntityManager* entityManager, iPoint position) : Entity(EntityType::STATIC)
{
	this->name.Create(name.c_str());
	this->tex = tex;
	this->entityManager = entityManager;
	this->position = position;

	//Assign type of static entity, texture rect and pivot
	//Orthogonal map ------------------------
	if (name == "tree_0")
	{
		type = Static::Type::TREE;
		SetRect(512, 546, 32, 46);
		SetPivot(16, 46);
	}

	else {
		LOG("There isn't any type assigned to %s name entity", name.data());
	}

	size = iPoint(frame.w, frame.h);

	if (((Map*)entityManager->SearchEntity("map"))->data.type == MAPTYPE_ISOMETRIC)
	{
		position.x /= ((Map*)entityManager->SearchEntity("map"))->data.tileWidth * 0.5f;
		position.y /= ((Map*)entityManager->SearchEntity("map"))->data.tileHeight;

		iPoint pos = ((Map*)entityManager->SearchEntity("map"))->MapToWorld(position.x + 1, position.y + 1);
		position.Create(pos.x, pos.y);
	}

	data.tileset.texture = tex->Load(((Map*)entityManager->SearchEntity("map"))->data.properties.objectsPath.data());
}

Static::~Static()
{
}

bool Static::Draw(Render* render)
{
	bool ret = true;

	render->scale = 3;
	if (!render->DrawTexture(data.tileset.texture, position.x, position.y, &frame))
	{
		render->scale = 1;
		LOG("Could not draw the static entity texture...");
		ret = false;
	}
	render->scale = 1;
	
	return ret;
}

void Static::SetRect(int x, int y, int w, int h)
{
	frame.x = x;
	frame.y = y;
	frame.w = w;
	frame.h = h;
}
