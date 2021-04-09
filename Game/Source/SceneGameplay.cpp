#include "SceneGameplay.h"

#include "EntityManager.h"
#include "GuiManager.h"
#include "Window.h"
#include "BattleSystem.h"

SceneGameplay::SceneGameplay()
{
	type = SceneType::GAMEPLAY;

	this->name = "scenegameplay";

	map = nullptr;
	currentPlayer = nullptr;

	camera = { 0,0,1280,720 };

	texture = nullptr;
}

SceneGameplay::~SceneGameplay()
{
}

bool SceneGameplay::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager)
{
	
		map = (Map*)entityManager->CreateEntity(EntityType::MAP,"Map");
		// L03: DONE: Load map
		// L12b: Create walkability map on map loading
		if (map->Load("Cemetery", "Cemetery.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}


	// Load music
	// AudioManager::PlayMusic("Assets/Audio/Music/music_spy.ogg");

	// Load textures
	texture = tex->Load("Assets/Textures/Characters/characters_spritesheet.png");
	Entity* entity;
	entity = entityManager->CreateEntity(EntityType::PLAYER,"DaBaby");
	entity->position = iPoint(12 * 32, 6 * 32);
	entity->SetTexture(texture,3);
	entity->SetState(true);
	entity = nullptr;
	entity = entityManager->CreateEntity(EntityType::ENEMY, "DaBoss");
	entity->position = iPoint(10 * 32, 6 * 32);
	entity->SetTexture(texture, 5);
	entity = nullptr;

	entity = entityManager->CreateEntity(EntityType::NPC, "DaBot");
	entity->position = iPoint(10 * 32, 6 * 32);
	entity->SetTexture(texture, 4);
	entity = nullptr;
	return true;
}

inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}

bool SceneGameplay::Update(Input* input, float dt)
{
	// L02: DONE 3: Request Load / Save when pressing L/S
	//if (input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) app->LoadGameRequest();
	//if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) app->SaveGameRequest();

	return true;
}

bool SceneGameplay::Draw(Render* render)
{
    return true;
}

bool SceneGameplay::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	// TODO: Unload all resources
	map->CleanUp();
	tex->UnLoad(texture);

	return true;
}

bool SceneGameplay::LoadState(pugi::xml_node& scenegameplay)
{
	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& scenegameplay) const
{
	/* THE BELOW CODE IS FOR TESTING PURPOSES*/

	// Check if ITWORKS node exists
	SString testName(scenegameplay.child("ITWORKS").name());
	if (testName == "ITWORKS")
	{
		// Node ITWORKS exists
		pugi::xml_node testNode = scenegameplay.child("ITWORKS");

		testNode.attribute("A").set_value(12);
		testNode.attribute("B").set_value(12);

	}
	else
	{
		// Node ITWORKS does not exist
		pugi::xml_node testNode = scenegameplay.append_child("ITWORKS");

		testNode.append_attribute("A").set_value(100);
		testNode.append_attribute("B").set_value(100);
	}
	return true;
}

Player* SceneGameplay::GetCurrentPlayer()
{
	return currentPlayer;
}