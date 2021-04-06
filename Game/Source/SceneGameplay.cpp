#include "SceneGameplay.h"

#include "EntityManager.h"
#include "GuiManager.h"
#include "Window.h"
#include "BattleSystem.h"

SceneGameplay::SceneGameplay()
{
	type = SceneType::GAMEPLAY;
}

SceneGameplay::~SceneGameplay()
{
}

bool SceneGameplay::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager)
{
	
	if (entityManager->SearchEntity("Map") == nullptr) {
		map = (Map*)entityManager->CreateEntity(EntityType::MAP);
		// L03: DONE: Load map
		// L12b: Create walkability map on map loading
		if (map->Load("Cemetery", "Cemetery.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}
	}
	else {
		map = (Map*)entityManager->SearchEntity("Map");
	}


	// Load music
	// AudioManager::PlayMusic("Assets/Audio/Music/music_spy.ogg");

	// Load textures
	texture = tex->Load("Assets/Textures/Characters/characters_spritesheet.png");

	// Initialize players (party members)
	// Party member 1

		if (entityManager->SearchEntity("DaBaby") == nullptr) {
			players.Add((Player*)entityManager->CreateEntity(EntityType::PLAYER));
			players.At(0)->data->position = iPoint(12 * 32, 6 * 32);
			players.At(0)->data->SetTexture(texture, 3);
			players.At(0)->data->SetName("DaBaby");
			// Set this party member as the current one
			currentPlayer = players.At(0)->data;
			players.At(0)->data->SetState(true);
			
		}
		else {
			players.Add((Player*)entityManager->SearchEntity("DaBaby"));
			players.At(0)->data->SetTexture(texture, 3);
			currentPlayer = players.At(0)->data;
		}
		

		if (entityManager->SearchEntity("John") == nullptr) {
			// Party member 2
			players.Add((Player*)entityManager->CreateEntity(EntityType::PLAYER));
			players.At(1)->data->position = iPoint(12 * 32, 6 * 32);
			players.At(1)->data->SetTexture(texture, 4);
			players.At(1)->data->SetName("John");

		}
		else 
		{
			players.Add((Player*)entityManager->SearchEntity("John"));
		players.At(0)->data->SetTexture(texture, 3);
		}
		
		if (entityManager->SearchEntity("DaBoss") == nullptr) {
			// Create enemy
			enemy1 = (Enemy*)entityManager->CreateEntity(EntityType::ENEMY);
			enemy1->position = iPoint(10 * 32, 6 * 32);
			enemy1->SetTexture(texture, 7);
			enemy1->SetName("DaBoss");
		}
		else
		{

			enemy1 = (Enemy*)entityManager->SearchEntity("DaBoss");
			enemy1->SetTexture(texture, 7);
		}

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

	// Press B to enter the battle scene, just for debug purposes [remove later]
	if (input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		// Set the players and the enemy(ies) data in the battle system
		BattleSystem::GetInstance()->SetupBattle(players, enemy1);

		TransitionToScene(SceneType::BATTLE);
	}
	return true;
}

bool SceneGameplay::Draw(Render* render)
{
    return true;
}

bool SceneGameplay::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	// TODO: Unload all resources
	/*map->CleanUp();*/
	tex->UnLoad(texture);

	return true;
}

Player* SceneGameplay::GetCurrentPlayer()
{
	return currentPlayer;
}