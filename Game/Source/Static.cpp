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

	if (name == "roof_0")
	{
		type = Static::Type::ROOF;
		SetRect(32, 320, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_1")
	{
		type = Static::Type::ROOF;
		SetRect(224, 544, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_2")
	{
		type = Static::Type::ROOF;
		SetRect(224, 320, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "cup_0")
	{
		type = Static::Type::CUP;
		SetRect(299, 493, 10, 9);
		SetPivot(10, 36);
	}

	if (name == "roof_3")
	{
		type = Static::Type::ROOF;
		SetRect(352, 320, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_4")
	{
		type = Static::Type::ROOF;
		SetRect(320, 544, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_5")
	{
		type = Static::Type::ROOF;
		SetRect(32, 352, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_6")
	{
		type = Static::Type::ROOF;
		SetRect(352, 352, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_7")
	{
		type = Static::Type::ROOF;
		SetRect(160, 640, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_8")
	{
		type = Static::Type::ROOF;
		SetRect(160, 416, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "signe_0")
	{
		type = Static::Type::SIGNE;
		SetRect(614, 706, 15, 28);
		SetPivot(8, 28);
	}

	if (name == "machine_0")
	{
		type = Static::Type::MACHINE;
		SetRect(422, 597, 19, 26);
		SetPivot(10, 26);
	}

	if (name == "roof_9")
	{
		type = Static::Type::ROOF;
		SetRect(320, 576, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "window_0")
	{
		type = Static::Type::WINDOW;
		SetRect(102, 116, 19, 34);
		SetPivot(19, 136);
	}

	if (name == "light_0")
	{
		type == Static::Type::LIGHT;
		SetRect(711, 309, 18, 105);
		SetPivot(9, 105);
	}

	if (name == "bin_0")
	{
		type = Static::Type::BIN;
		SetRect(458, 650, 12, 18);
		SetPivot(6, 18);
	}

	if (name == "bin_1")
	{
		type = Static::Type::BIN;
		SetRect(426, 649, 12, 18);
		SetPivot(6, 18);
	}

	if (name == "roof_10")
	{
		type = Static::Type::ROOF;
		SetRect(32, 544, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "statue_0")
	{
		type = Static::Type::STATUE;
		SetRect(486, 147, 20, 40);
		SetPivot(20, 40);
	}

	if (name == "tree_1")
	{
		type = Static::Type::TREE;
		SetRect(68, 193, 25, 31);
		SetPivot(13, 31);
	}

	if (name == "tree_2")
	{
		type = Static::Type::TREE;
		SetRect(194, 160, 62, 38);
		SetPivot(31, 38);
	}

	if (name == "light_1")
	{
		type = Static::Type::TREE;
		SetRect(115, 145, 11, 23);
		SetPivot(6, 23);
	}

	if (name == "stone_0")
	{
		type = Static::Type::STONE;
		SetRect(236, 21, 8, 25);
		SetPivot(4, 25);
	}

	if (name == "light_2")
	{
		type = Static::Type::LIGHT;
		SetRect(168, 130, 16, 25);
		SetPivot(8, 25);
	}

	if (name == "stone_2")
	{
		type = Static::Type::STONE;
		SetRect(194, 25, 28, 14);
		SetPivot(14, 14);
	}

	if (name == "stone_3")
	{
		type = Static::Type::STONE;
		SetRect(256, 143, 32, 17);
		SetPivot(16, 17);
	}

	if (name == "roof_red")
	{
		type = Static::Type::ROOF;
		SetRect(416, 236, 32, 20);
		SetPivot(16, 40);
	}

	if (name == "roof_red2")
	{
		type = Static::Type::ROOF;
		SetRect(448, 236, 32, 20);
		SetPivot(16, 40);
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

	SString mapTexturePath;
	mapTexturePath.Create(((Map*)entityManager->SearchEntity("map"))->data.properties.objectsPath.data());

	if (mapTexturePath == "Maps/Town/Tilemap.png")
	{
		data.tileset.texture = entityManager->townTexture;
	}
	else if (mapTexturePath == "Maps/MediumCity/mediumcity2.png")
	{
		data.tileset.texture = entityManager->mediumCityTexture;
	}
	else if (mapTexturePath == "Maps/Cemetery/GK_FG_B_2.png")
	{
		data.tileset.texture = entityManager->cemeteryTexture;
	}
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
