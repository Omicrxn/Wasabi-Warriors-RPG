#include "SceneGameplay.h"

#include "EntityManager.h"
#include "BattleSystem.h"
#include "GuiManager.h"
#include "Window.h"
#include "DialogSystem.h"

#include "Notifier.h"
#include "Easing.h"
#include "Timer.h"
#include "Transitions.h"

#include "GuiButton.h"
#include "GuiIcon.h"
#include "App.h"
#include "Log.h"

#include "ScreenRoaming.h"
#include "ScreenPause.h"
#include "ScreenSettings.h"
#include "ScreenBattle.h"
#include "ScreenInventory.h"

#include "Item.h"

SceneGameplay::SceneGameplay(bool hasStartedFromContinue)
{
	this->hasStartedFromContinue = hasStartedFromContinue;
	if (this->hasStartedFromContinue)
		type = SceneType::GAMEPLAY_LOAD;
	else
		type = SceneType::GAMEPLAY;

	this->name = "scenegameplay";

	// Needed modules
	entityManager = nullptr;
	guiManager = nullptr;
	win = nullptr;
	dialogSystem = nullptr;
	audio = nullptr;

	charactersSpritesheet = nullptr;

	camera = { 0,0,1280,720 };

	map = nullptr;
	currentPlayer = nullptr;

	battleSystem = new BattleSystem();

	currentState = GameState::ROAMING;

	// Battle system textures
	backgroundTex = nullptr;
	titlesTex = nullptr;
	guiAtlasTex = nullptr;

	backgroundRect = { 0, 0, 1280, 720 };
	pauseBackgroundRect = { 1544, 217, 113, 295 };

	// Rects for titles
	settingsTitleRect = { 0, 149, 530, 81 };

	// Fonts
	titleFont = nullptr;
	buttonFont = nullptr;

	// Audio Fx for buttons
	hoverFx = -1;
	clickFx = -1;
	returnFx = -1;

	// Buttons
	btnAttack = nullptr;
	btnDefend = nullptr;
	btnItem = nullptr;
	btnRun = nullptr;

	// Gamepad's menu focused button
	focusedButtonId = 0;

	notifier = nullptr;

	// Screens
	screenPause = nullptr;
	screenRoaming = nullptr;
	screenSettings = nullptr;
	screenBattle = nullptr;
	screenInventory = nullptr;

	currentMap = MapType::NONE;
}

SceneGameplay::~SceneGameplay()
{
}

bool SceneGameplay::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem, Easing* easing, Transitions* transitions, App* app)
{
	audio->StopMusic();

	notifier = Notifier::GetInstance();

	// Needed modules
	this->entityManager = entityManager;
	this->guiManager = guiManager;
	this->win = win;
	this->dialogSystem = dialogSystem;
	this->audio = audio;
	this->transitions = transitions;

	map = (Map*)entityManager->CreateEntity(EntityType::MAP, "Map", EntitySubtype::UNKNOWN);

	// Load texture
	charactersSpritesheet = tex->Load("Assets/Textures/Characters/characters_spritesheet.png");
	titlesTex = tex->Load("Assets/Textures/Scenes/titles.png");
	entityManager->texture = charactersSpritesheet;

	// Load battle system textures
	backgroundTex = tex->Load("Assets/Textures/Scenes/battle_scene.jpg");
	guiAtlasTex = tex->Load("Assets/Textures/UI/ui_spritesheet.png");
	aura = tex->Load("Assets/Textures/Scenes/aura.png");
	cast1 = tex->Load("Assets/Textures/Effects/cast_001.png");
	enemyCast = tex->Load("Assets/Textures/Effects/cast_008.png");
	indicator = tex->Load("Assets/Textures/Effects/fire_003.png");

	// Set battle animations
	// Aura
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			auraAnim.PushBack({ i * 128, j * 128, 128, 128 });
		}
	}
	auraAnim.speed = 1.0f;
	auraAnim.loop = true;

	// Cast 1
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			cast1Anim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	cast1Anim.speed = 0.5f;

	// Enemy cast
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			enemyCastAnim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	enemyCastAnim.speed = 0.5f;

	// Indicator
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			indicatorAnim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	indicatorAnim.speed = 1.0f;
	indicatorAnim.loop = true;

	// Create fonts
	titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
	buttonFont = new Font("Assets/Fonts/showg.xml", tex);
	menuFont = new Font("Assets/Fonts/poppins.xml", tex);

	// Load buttons Fx
	hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
	clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");
	returnFx = audio->LoadFx("Assets/Audio/Fx/back.ogg");

	// Gui id goes from 0 to 2
	screenRoaming = new ScreenRoaming();
	screenRoaming->Load(0, 2, this, win, guiManager, entityManager, easing, guiAtlasTex, buttonFont, hoverFx, clickFx);
	screenRoaming->isActive = true;
	screenRoaming->ShowButtons();

	// Gui id goes from 3 to 5
	screenPause = new ScreenPause();
	screenPause->Load(3, 5, this, win, guiManager, NULL, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);
	screenPause->isActive = false;
	screenPause->HideButtons();

	// Gui id goes from 6 to 10
	screenSettings = new ScreenSettings();
	screenSettings->Load(6, 10, this, win, guiManager, NULL, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);
	screenSettings->isActive = false;
	screenSettings->HideButtons();

	// Gui id goes from 11 to 14
	screenBattle = new ScreenBattle();
	screenBattle->Load(11, 14, this, battleSystem, tex, win, audio, guiManager, entityManager, charactersSpritesheet, guiAtlasTex, titleFont, buttonFont, menuFont, hoverFx, clickFx, returnFx);
	screenBattle->isActive = false;

	// Gui id goes from 15 to 16
	screenInventory = new ScreenInventory();
	screenInventory->Load(15, 16, this, win, guiManager, entityManager, easing, guiAtlasTex, buttonFont, hoverFx, clickFx);
	screenInventory->isActive = false;
	screenInventory->HideButtons();

	if (hasStartedFromContinue)
	{
		// LOAD FROM THE SAVE FILE
		app->LoadGameRequest();
		currentPlayer = nullptr;
	}
	else
	{
		// Create party member 1
		Player* player;
		player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "DaBaby", EntitySubtype::PLAYER_HUNTER, iPoint(19 * 32, 1 * 32));
		player->SetState(true);
		player = nullptr;
		currentPlayer = entityManager->playerList.At(0)->data;

		// Create party member 2
		player = (Player*)entityManager->CreateEntity(EntityType::PLAYER,  "DaCrack", EntitySubtype::PLAYER_WIZARD, iPoint(19 * 32, 1 * 32));
		player = nullptr;

		// Create party member 3
		player = (Player*)entityManager->CreateEntity(EntityType::PLAYER,  "DaBug", EntitySubtype::PLAYER_WIZARD,  iPoint(19 * 32, 1 * 32));
		player = nullptr;

		RELEASE(player);
		// LOAD FROM MAP_XML
		notifier->NotifyMapChange(MapType::TOWN);
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
	// Player god mode
	if (input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN)
	{
		entityManager->TooglePlayerGodMode();
	}

	// Checking if we have to change the map
	if (notifier->OnMapChange() && notifier->GetNextMap() != currentMap)
	{
		SetUpTp();
	}

	// Checking if we have to add any item
	if (notifier->GetItemAddition())
	{
		AddItemToInvItemsList(notifier->GetItem());
	}

	// Checking if we have to change the player position
	if (notifier->OnPositionChange())
	{
		// Reset the request position change to false
		notifier->NotifyPositionChange();

		// Iterate all players and change position
		for (int i = 0; i < entityManager->playerList.Count(); i++)
		{
			entityManager->playerList.At(i)->data->position = notifier->GetNextPosition();
			entityManager->playerList.At(i)->data->collider->SetPos(notifier->GetNextPosition().x, notifier->GetNextPosition().y);
		}
	}

	if ((input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_Y) == KeyState::KEY_DOWN) && screenBattle->isActive == false)
	{
		if (currentPlayer == nullptr)
		{
			currentPlayer = entityManager->playerList.At(0)->next->data;
			((ScreenRoaming*)screenRoaming)->SetCurrentPlayer(currentPlayer);
		}

		if (entityManager->playerList.At(entityManager->playerList.Find(currentPlayer))->next != nullptr)
		{
			currentPlayer->SetState(false);
			currentPlayer = entityManager->playerList.At(entityManager->playerList.Find(currentPlayer))->next->data;
			currentPlayer->SetState(true);
			((ScreenRoaming*)screenRoaming)->SetCurrentPlayer(currentPlayer);
		}
		else 
		{
			currentPlayer->SetState(false);
			currentPlayer = entityManager->playerList.At(0)->data;
			currentPlayer->SetState(true);
			((ScreenRoaming*)screenRoaming)->SetCurrentPlayer(currentPlayer);
		}
	}

	if (input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && screenBattle->isActive == false)
	{
		screenBattle->isActive = true; 
		battleSystem->SetupBattle(&entityManager->playerList, (Enemy*)entityManager->SearchEntity(notifier->GetEnemy()), &entityManager->itemList);

		((ScreenBattle*)screenBattle)->EnableBattleButtons();

		currentState = GameState::BATTLE;
		screenBattle->isActive = true;

		focusedButtonId = 6;

		audio->StopMusic();
		audio->PlayMusic("Assets/Audio/Music/battle.ogg");
	}

	if (notifier->GetBattle())
	{
		for (int i = 0; i < entityManager->playerList.Count(); i++)
		{
			entityManager->playerList.At(i)->data->stopPlayer = true;
		}
		screenBattle->isActive = true;
		battleSystem->SetupBattle(&entityManager->playerList, (Enemy*)entityManager->SearchEntity(notifier->GetEnemy()), &entityManager->itemList);

		((ScreenBattle*)screenBattle)->EnableBattleButtons();

		notifier->NotifyBattle();

		currentState = GameState::BATTLE;
		screenBattle->isActive = true;

		focusedButtonId = 6;

		audio->StopMusic();
		audio->PlayMusic("Assets/Audio/Music/battle.ogg");
	}

	if (notifier->OnDialog() && (input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN /*|| input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN*/))
	{
		for (int i = 0; i < entityManager->playerList.Count(); i++)
		{
			entityManager->playerList.At(i)->data->stopPlayer = true;
		}
		dialogSystem->NewDialog();
	}
	if (dialogSystem->DialogHasFinished())
	{
		for (int i = 0; i < entityManager->playerList.Count(); i++)
		{
			entityManager->playerList.At(i)->data->stopPlayer = false;
		}
		notifier->SetDialogMode(false);
		dialogSystem->SetDialogFinished(false);
	}

	switch (currentState)
	{
	case GameState::ROAMING:
		screenRoaming->Update(input, dt, focusedButtonId);
		break;
	case GameState::PAUSE:
		screenPause->Update(input, dt, focusedButtonId);
		break;
	case GameState::SETTINGS:
		screenSettings->Update(input, dt, focusedButtonId);
		break;
	case GameState::EXIT:
		TransitionToScene(SceneType::TITLE);
		break;
	case GameState::BATTLE:
		screenBattle->Update(input, dt, focusedButtonId);
		break;
	case GameState::INVENTORY:
		screenInventory->Update(input, dt, focusedButtonId);
		break;
	default:
		break;
	}

	if (battleSystem->IsTurnChanging()) ((ScreenBattle*)screenBattle)->ResetOneTimeAnimations();

	return true;
}

bool SceneGameplay::Draw(Render* render)
{
	switch (currentState)
	{
	case GameState::ROAMING:
		screenRoaming->Draw(render);
		break;
	case GameState::PAUSE:
		screenPause->Draw(render);
		break;
	case GameState::SETTINGS:
		screenSettings->Draw(render);
		break;
	case GameState::BATTLE:
		screenBattle->Draw(render);
		break;
	case GameState::INVENTORY:
		screenInventory->Draw(render);
		break;
	default:
		break;
	}

	return true;
}

bool SceneGameplay::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	// TODO: Unload all resources
	RELEASE(battleSystem);
	battleSystem = nullptr;

	for (int i = 0; i < invItemsList.Count(); ++i)
	{
		invItemsList.Del(invItemsList.At(0));
	}
	invItemsList.Clear();

	// Remove map
	map->CleanUp();

	// Stop music (GitHub)

	// Release fonts
	RELEASE(titleFont);
	titleFont = nullptr;
	RELEASE(buttonFont);
	buttonFont = nullptr;
	RELEASE(menuFont);
	menuFont = nullptr;

	// Unload textures
	tex->UnLoad(charactersSpritesheet);
	charactersSpritesheet = nullptr;
	tex->UnLoad(backgroundTex);
	backgroundTex = nullptr;
	tex->UnLoad(guiAtlasTex);
	guiAtlasTex = nullptr;
	tex->UnLoad(titlesTex);
	titlesTex = nullptr;
	tex->UnLoad(cast1);
	cast1 = nullptr;
	tex->UnLoad(enemyCast);
	enemyCast = nullptr;
	tex->UnLoad(indicator);
	indicator = nullptr;

	// Unload Fx
	audio->UnloadFx(clickFx);
	audio->UnloadFx(hoverFx);
	audio->UnloadFx(returnFx);

	// Destory GUI Controls
	guiManager->DestroyGuiControl(btnAttack);
	btnAttack = nullptr;
	guiManager->DestroyGuiControl(btnDefend);
	btnDefend = nullptr;
	guiManager->DestroyGuiControl(btnItem);
	btnItem = nullptr;
	guiManager->DestroyGuiControl(btnRun);
	btnRun = nullptr;

	notifier = nullptr;

	// Unload screens
	screenRoaming->Unload(tex, audio, guiManager);
	screenPause->Unload(tex, audio, guiManager);
	screenSettings->Unload(tex, audio, guiManager);
	screenBattle->Unload(tex, audio, guiManager);
	screenInventory->Unload(tex, audio, guiManager);

	// Delete screens
	RELEASE(screenRoaming);
	screenRoaming = nullptr;
	RELEASE(screenPause);
	screenPause = nullptr;
	RELEASE(screenSettings);
	screenSettings = nullptr;
	RELEASE(screenBattle);
	screenBattle = nullptr;
	RELEASE(screenInventory);
	screenInventory = nullptr;

	entityManager->CleanUp();

	// Removing used modules
	this->entityManager = nullptr;
	this->guiManager = nullptr;
	this->win = nullptr;
	this->dialogSystem = nullptr;
	this->audio = nullptr;

	return true;
}

bool SceneGameplay::LoadState(pugi::xml_node& scenegameplay)
{
	if ((int)currentMap != scenegameplay.attribute("currentMap").as_int())
	{
		MapType nextMap = (MapType)scenegameplay.attribute("currentMap").as_int();
		this->notifier->NotifyMapChange(nextMap);
	}
	pugi::xml_node screenNode = scenegameplay.parent().parent().child("screen");
	screenSettings->LoadState(screenNode);
	hasStartedFromContinue = true;
	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& scenegameplay) const
{
	/* ---------- CHECKS IF THE Attribute WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	SString tempName = scenegameplay.attribute("currentMap").name();
	if (tempName == "currentMap")
	{
		// Attribute currentMap exists
		scenegameplay.attribute("currentMap").set_value((int)this->currentMap);
	}
	else
	{
		// Attribute currentMap does not exist
		scenegameplay.append_attribute("currentMap").set_value((int)this->currentMap);
	}
	pugi::xml_node screenNode = scenegameplay.parent().parent().child("screen");
	screenSettings->SaveState(screenNode);
	return true;
}

Player* SceneGameplay::GetCurrentPlayer()
{
	return currentPlayer;
}

MapType SceneGameplay::GetCurrentMap()
{
	return currentMap;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::ICON:
	{
		if (control->id == 0)
		{
			// Entering pause from roaming
			currentState = GameState::PAUSE;
			audio->ChangeMusicVolume(SDL_MIX_MAXVOLUME / 10);

			screenRoaming->isActive = false;
			screenRoaming->HideButtons();

			screenPause->isActive = true;
			screenPause->ShowButtons();

			for (int i = 0; i < entityManager->playerList.Count(); i++)
			{
				entityManager->playerList.At(i)->data->stopPlayer = true;
			}
			
			pauseTimer.Start();
		}
		else if (control->id == 1)
		{
			currentState = GameState::INVENTORY;

			screenRoaming->isActive = false;
			screenRoaming->HideButtons();

			screenInventory->isActive = true;
			screenInventory->ShowButtons();
		}
		else if (control->id == 2) {} //currentState = GameState::PHONE;
		else if (control->id == 3)
		{
			if (pauseTimer.ReadSec() > 0.5f)
			{
				currentState = GameState::ROAMING;

				screenPause->isActive = false;
				screenPause->HideButtons();
				
				screenRoaming->isActive = true;
				screenRoaming->ShowButtons();

				for (int i = 0; i < entityManager->playerList.Count(); i++)
				{
					entityManager->playerList.At(i)->data->stopPlayer = false;
				}

				PlayMapMusic();
			}
		}
		else if (control->id == 4) 
		{
			// Entering settings from pause
			currentState = GameState::SETTINGS;

			screenPause->isActive = false;
			screenPause->HideButtons();

			screenSettings->isActive = true;
			screenSettings->ShowButtons();

			// Fullscreen and vsync controls are disabled if you acces from gameplay
			this->guiManager->controls.At(6)->data->state = GuiControlState::DISABLED;
			this->guiManager->controls.At(7)->data->state = GuiControlState::DISABLED;

			audio->StopMusic();
			audio->PlayMusic("Assets/Audio/Music/menu_settings.ogg");
		}
		else if (control->id == 5)
		{
			// Exiting to menu
			currentState = GameState::EXIT;

			screenRoaming->isActive = false;
			screenRoaming->HideButtons();

			screenPause->isActive = false;
			screenPause->HideButtons();
		}
		else if (control->id == 10)
		{
			// Returning from settings to pause
			currentState = GameState::PAUSE;

			screenPause->isActive = true;
			screenPause->ShowButtons();

			screenSettings->isActive = false;
			screenSettings->HideButtons();
		}

		break;
	}
	case GuiControlType::BUTTON:
	{
		if (control->id == 11) battleSystem->playerState = PlayerState::ATTACK;
		else if (control->id == 12) battleSystem->playerState = PlayerState::DEFEND;
		else if (control->id == 13) battleSystem->playerState = PlayerState::ITEM;
		else if (control->id == 14)
		{
			battleSystem->playerState = PlayerState::RUN;
			ExitBattle();
		}
		else if (control->id == 15) {}
		else if(control->id == 16)
		{
			currentState = GameState::ROAMING;

			screenInventory->isActive = false;
			screenInventory->HideButtons();

			screenRoaming->isActive = true;
			screenRoaming->ShowButtons();

			PlayMapMusic();
		}

		break;
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 8)
		{
			GuiSlider* tempSlider = (GuiSlider*)this->guiManager->controls.At(8)->data;
			int value = tempSlider->GetValue();
			audio->ChangeMusicVolume(value);
		}
		else if (control->id == 9)
		{
			GuiSlider* tempSlider = (GuiSlider*)this->guiManager->controls.At(9)->data;
			int value = tempSlider->GetValue();
			audio->ChangeFxVolume(value);
		}
	}
	default: break;
	}

	return true;
}

void SceneGameplay::ExitBattle()
{
	PlayMapMusic();

	for (int i = 11; i <= 14; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
	}

	screenBattle->isActive = false;
	battleSystem->ResetBattle();

	// Change game state to roaming
	currentState = GameState::ROAMING;

	screenRoaming->isActive = true;
	screenRoaming->ShowButtons();

	for (int i = 0; i < entityManager->playerList.Count(); i++)
	{
		entityManager->playerList.At(i)->data->stopPlayer = false;
	}
}

void SceneGameplay::SetUpTp()
{
	map->CleanUp();

	MapType previousMap = MapType::NONE;
	previousMap = currentMap;
	currentMap = notifier->GetNextMap();

	// Create map
	switch (currentMap)
	{
	case MapType::CEMETERY:
		if (map->Load("Cemetery", "Cemetery.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
			audio->PlayMusic("Assets/Audio/Music/cemetery.ogg");
		}
		break;
	case MapType::HOUSE:
		if (map->Load("House", "house.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
			audio->PlayMusic("Assets/Audio/Music/house.ogg");
		}
		break;
	case MapType::MEDIUM_CITY:
		if (map->Load("MediumCity", "mediumcity.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
			audio->PlayMusic("Assets/Audio/Music/city_background.ogg");
		}
		break;
	case MapType::RESTAURANT:
		if (map->Load("Restaurant", "restaurant.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
			audio->PlayMusic("Assets/Audio/Music/restaurant.ogg");
		}
		break;
	case MapType::TOWN:
		if (map->Load("Town", "townMap.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
			audio->PlayMusic("Assets/Audio/Music/city_background.ogg");
		}
		break;
	case MapType::BIG_CITY:
		if (map->Load("BigCity", "bigCity.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
			audio->PlayMusic("Assets/Audio/Music/city_background.ogg");
		}
		break;
	case MapType::SKYSCRAPER:
		if (map->Load("Dungeon", "dungeon.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
			audio->PlayMusic("Assets/Audio/Music/city_background.ogg");
		}
		break;
	default:
		break;
	}

	pugi::xml_document docData;
	pugi::xml_node mapNode;

	pugi::xml_parse_result result = docData.load_file("map_info.xml");

	// Check result for loading errors
	if (result == NULL)
	{
		LOG("Could not load map info xml file map_info.xml. pugi error: %s", result.description());
	}
	else
	{
		mapNode = docData.child("map");
		// GET THE NODE TO THE NEW MAP
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
		default:
			break;
		}
		//FIRST CHANGE PLAYERS POSITION TO NEW POSITION BASED ON THE PREVIOUS MAP 
		pugi::xml_node previousMapNode;
		switch (previousMap)
		{
		case MapType::NONE:
			break;
		case MapType::CEMETERY:
			previousMapNode = mapNode.child("prevCemetery");
			break;
		case MapType::HOUSE:
			previousMapNode = mapNode.child("prevHouse");
			break;
		case MapType::MEDIUM_CITY:
			previousMapNode = mapNode.child("prevMediumCity");
			break;
		case MapType::RESTAURANT:
			previousMapNode = mapNode.child("prevRestaurant");
			break;
		case MapType::TOWN:
			previousMapNode = mapNode.child("prevTown");
			break;
		case MapType::BIG_CITY:
			previousMapNode = mapNode.child("prevBigCity");
			break;
		case MapType::SKYSCRAPER:
			previousMapNode = mapNode.child("prevSkyscraper");
			break;
		default:
			break;
		}

		if (previousMap != MapType::NONE)
		{
			int newPosX = 0; int newPosY = 0;
			newPosX = previousMapNode.child("player").attribute("posX").as_int();
			newPosY = previousMapNode.child("player").attribute("posY").as_int();

			// Iterate all players and change position
			ListItem<Player*>* list1;
			for (list1 = entityManager->playerList.start; list1 != NULL; list1 = list1->next)
			{
				list1->data->position.x = newPosX;
				list1->data->position.y = newPosY;
				list1->data->collider->SetPos(newPosX, newPosY);
				list1->data->transitioning = true;
			}
			RELEASE(list1);
		}

		if (hasStartedFromContinue == false)
		{
			// DELETE ALL ENTITIES EXCEPT PLAYER
			entityManager->DeleteAllEntitiesExceptPlayer();

			// LOAD ENEMIES
			int enemyCount = mapNode.attribute("enemyCount").as_int();

			pugi::xml_node enemyNode = mapNode.child("enemy");
			Enemy* enemy = nullptr;
			for (int i = 0; i < enemyCount; ++i)
			{
				int subtype = enemyNode.attribute("subtype").as_int();
				iPoint position = { enemyNode.attribute("posX").as_int(),enemyNode.attribute("posY").as_int() };

				enemy = (Enemy*)entityManager->CreateEntity(EntityType::ENEMY, enemyNode.attribute("name").as_string(), (EntitySubtype)subtype, position);

				enemy->id = enemyNode.attribute("id").as_uint();

				enemy = nullptr;
				enemyNode = enemyNode.next_sibling("enemy");
			}

			// LOAD NPCs
			int npcCount = mapNode.attribute("npcCount").as_int();

			pugi::xml_node npcNode = mapNode.child("npc");
			NPC* npc = nullptr;
			for (int i = 0; i < npcCount; ++i)
			{
				npc = (NPC*)entityManager->CreateEntity(EntityType::NPC, npcNode.attribute("name").as_string(), EntitySubtype::UNKNOWN);

				npc->id = npcNode.attribute("id").as_uint();
				npc->spritePos = npcNode.attribute("spritePos").as_int();
				npc->position.x = npcNode.attribute("posX").as_int();
				npc->position.y = npcNode.attribute("posY").as_int();


				npc = nullptr;
				npcNode = npcNode.next_sibling("npc");
			}

			// LOAD TELEPORTS
			int teleportCount = mapNode.attribute("teleportCount").as_int();

			pugi::xml_node teleportNode = mapNode.child("teleport");
			Teleport* teleport = nullptr;
			for (int i = 0; i < teleportCount; ++i)
			{
				teleport = (Teleport*)entityManager->CreateEntity(EntityType::TELEPORT, teleportNode.attribute("name").as_string(), EntitySubtype::UNKNOWN);

				teleport->id = teleportNode.attribute("id").as_uint();
				teleport->spritePos = teleportNode.attribute("spritePos").as_int();
				teleport->position.x = teleportNode.attribute("posX").as_int();
				teleport->position.y = teleportNode.attribute("posY").as_int();
				teleport->width = teleportNode.attribute("width").as_int();
				teleport->height = teleportNode.attribute("height").as_int();

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
				teleportNode = teleportNode.next_sibling("teleport");
			}

			// LOAD ITEMS
			int itemCount = mapNode.attribute("itemCount").as_int();

			pugi::xml_node itemNode = mapNode.child("item");
			Item* item = nullptr;
			for (int i = 0; i < itemCount; ++i)
			{
				int subtype = itemNode.attribute("subtype").as_int();
				iPoint position = { itemNode.attribute("posX").as_int(),itemNode.attribute("posY").as_int() };

				item = (Item*)entityManager->CreateEntity(EntityType::ITEM, itemNode.attribute("name").as_string(), (EntitySubtype)subtype, position);

				item->id = itemNode.attribute("id").as_uint();

				item = nullptr;
				itemNode = itemNode.next_sibling("item");
			}
		}
		else 
		{
			hasStartedFromContinue = false;
		}
	}
}

void SceneGameplay::AddItemToInvItemsList(Item* item)
{
	notifier->GetInstance()->NotifyItemAddition();

	for (ListItem<InvItem*>* invItem = invItemsList.start; invItem; invItem = invItem->next)
	{
		if (invItem->data->item->subtype == item->subtype)
		{
			invItem->data->count++;
			entityManager->DestroyEntity((Entity*)item);
			return;
		}
	}

	InvItem* invItem = new InvItem();
	invItem->item = item;
	invItem->count = 1;
	invItemsList.Add(invItem);
	//RELEASE(invItem);
	
	((ScreenInventory*)screenInventory)->SetInventory(this->invItemsList);
}

void SceneGameplay::PlayMapMusic()
{
	audio->StopMusic();
	switch (currentMap)
	{
	case MapType::CEMETERY:
		audio->PlayMusic("Assets/Audio/Music/cemetery.ogg");
		break;
	case MapType::HOUSE:
		audio->PlayMusic("Assets/Audio/Music/house.ogg");
		break;
	case MapType::MEDIUM_CITY:
		audio->PlayMusic("Assets/Audio/Music/city_background.ogg");
		break;
	case MapType::RESTAURANT:
		audio->PlayMusic("Assets/Audio/Music/restaurant.ogg");
		break;
	case MapType::TOWN:
		audio->PlayMusic("Assets/Audio/Music/city_background.ogg");
		break;
	case MapType::BIG_CITY:
		audio->PlayMusic("Assets/Audio/Music/city_background.ogg");
		break;
	case MapType::SKYSCRAPER:
		audio->PlayMusic("Assets/Audio/Music/cemetry.ogg");
		break;
	default:
		break;
	}
}
