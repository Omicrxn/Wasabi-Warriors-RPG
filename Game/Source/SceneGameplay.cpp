#include "SceneGameplay.h"

#include "AssetsManager.h"
#include "EntityManager.h"
#include "BattleSystem.h"
#include "GuiManager.h"
#include "Window.h"
#include "DialogSystem.h"
#include "QuestManager.h"

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
#include "Activator.h"
#include "Lever.h"
#include "Stats.h"
#include "SecretWall.h"

SceneGameplay::SceneGameplay(bool hasStartedFromContinue)
{
	this->hasStartedFromContinue = hasStartedFromContinue;

	if (this->hasStartedFromContinue)
		type = SceneType::GAMEPLAY_LOAD;
	else
	{
		type = SceneType::GAMEPLAY;
		
		pugi::xml_document docData;
		pugi::xml_parse_result result = docData.load_file("save_game.xml");

		// Delete map node from save file has we have started a new game!!
		pugi::xml_node mapNode;
		mapNode = docData.first_child().child("scenemanager").child("scenegameplay");
		mapNode.remove_child("map");

		// Delete inventory node
		pugi::xml_node inventoryNode;
		inventoryNode = docData.first_child().child("screen");
		inventoryNode.remove_child("screeninventory");
		
		docData.save_file("save_game.xml");
	}
		
	this->name = "scenegameplay";

	// Needed modules
	assetsManager = nullptr;
	entityManager = nullptr;
	guiManager = nullptr;
	win = nullptr;
	dialogSystem = nullptr;
	audio = nullptr;
	transitions = nullptr;
	questManager = nullptr;
	app = nullptr;

	// Screens
	screenPause = nullptr;
	screenRoaming = nullptr;
	screenSettings = nullptr;
	screenBattle = nullptr;
	screenInventory = nullptr;

	charactersSpritesheet = nullptr;

	camera = { 0,0,1280,720 };

	map = nullptr;
	currentPlayer = nullptr;

	battleSystem = nullptr;

	currentState = GameState::ROAMING;

	// Battle system textures
	backgroundTex = nullptr;
	titlesTex = nullptr;
	guiAtlasTex = nullptr;
	guiAtlasTex2 = nullptr;
	guiAtlasOut = nullptr;
	cast1 = nullptr;
	enemyCast = nullptr;
	indicator = nullptr;
	signal = nullptr;

	backgroundRect = { 0, 0, 1280, 720 };

	// Rects for titles
	settingsTitleRect = { 0, 149, 530, 81 };

	// Fonts
	titleFont = nullptr;
	battleFont = nullptr;
	menuFont = nullptr;
	buttonFont = nullptr;

	// Audio Fx for buttons
	hoverFx = -1;
	clickFx = -1;
	returnFx = -1;
	doorOpenFx = -1;
	bagOpenFx = -1;

	// Buttons
	btnAttack = nullptr;
	btnDefend = nullptr;
	btnItem = nullptr;
	btnRun = nullptr;

	// Gamepad's menu focused button
	focusedButtonId = 0;

	notifier = nullptr;

	currentMap = MapType::NONE;

	// Bools
	readyToChangeMap = false;

	controller = false;
}

SceneGameplay::~SceneGameplay()
{
}

bool SceneGameplay::Load(Input* input, Render* render, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem, Easing* easing, Transitions* transitions, AssetsManager* assetsManager, App* app)
{
	audio->StopMusic();

	entityManager->Start();

	battleSystem = new BattleSystem();

	notifier = Notifier::GetInstance();
	notifier->SetSceneGameplay(this);

	// Needed modules
	this->assetsManager = assetsManager;
	this->entityManager = entityManager;
	this->guiManager = guiManager;
	this->win = win;
	this->dialogSystem = dialogSystem;
	this->audio = audio;
	this->transitions = transitions;
	this->app = app;
	this->tex = tex;

	questManager = new QuestManager(input, render, tex, this, assetsManager);
	questManager->Start();

	map = (Map*)entityManager->CreateEntity(EntityType::MAP, "Map", EntitySubtype::UNKNOWN);

	// Load texture
	charactersSpritesheet = tex->Load("Textures/Characters/characters_spritesheet.png");
	titlesTex = tex->Load("Textures/Scenes/titles.png");
	entityManager->texture = charactersSpritesheet;

	// Load battle system textures
	backgroundTex = tex->Load("Textures/Scenes/battle_scene.jpg");

	guiAtlasTex = tex->Load("Textures/UI/ui_spritesheet.png");
	guiAtlasTex2 = tex->Load("Textures/UI/guiTextureSpritesheet.png");
	guiAtlasOut = tex->Load("Textures/UI/outsideGUI.png");

	cast1 = tex->Load("Textures/Effects/cast_001.png");
	enemyCast = tex->Load("Textures/Effects/cast_008.png");
	indicator = tex->Load("Textures/Effects/fire_003.png");
	signal = tex->Load("Textures/Effects/magic_004.png");

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

	// Signal
	for (int j = 0; j < 7; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			signalAnim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	for (int j = 6; j >= 0; j--)
	{
		for (int i = 4; i >= 0; i--)
		{
			signalAnim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	signalAnim.speed = 1.0f;
	signalAnim.loop = true;

	// Create fonts
	titleFont = new Font("Fonts/shojumaru.xml", tex, assetsManager);
	battleFont = new Font("Fonts/SHOWG.xml", tex, assetsManager);
	menuFont = new Font("Fonts/poppins.xml", tex, assetsManager);
	buttonFont = new Font("Fonts/comic_serif.xml", tex, assetsManager);
	dialogSystem->SetFont(buttonFont);

	// Load buttons Fx
	hoverFx = audio->LoadFx("Audio/Fx/bong.ogg");
	clickFx = audio->LoadFx("Audio/Fx/click.ogg");
	returnFx = audio->LoadFx("Audio/Fx/back.ogg");

	// Load other fx
	doorOpenFx = audio->LoadFx("Audio/Fx/door_open.ogg");
	bagOpenFx = audio->LoadFx("Audio/Fx/bag_open.ogg");

	screenRoaming = new ScreenRoaming();
	screenRoaming->Load(this, guiManager, entityManager, audio, easing, guiAtlasOut, guiAtlasTex2, buttonFont, clickFx);
	screenRoaming->isActive = true;

	// Gui id goes from 0 to 5
	screenPause = new ScreenPause();
	screenPause->Load(0, 5, this, win, guiManager, entityManager, audio, easing, guiAtlasTex2, buttonFont, hoverFx, clickFx);
	screenPause->SetQuestManager(questManager);
	screenPause->SetMap(currentMap);
	screenPause->Disable();

	// Gui id goes from 6 to 10
	screenSettings = new ScreenSettings();
	screenSettings->SetGameplaySettings(true);
	screenSettings->Load(6, 9, this, win, guiManager, guiAtlasTex2, buttonFont, hoverFx, clickFx, returnFx);
	screenSettings->Disable();

	// Gui id goes from 11 to 14
	screenBattle = new ScreenBattle();
	screenBattle->Load(10, 13, this, battleSystem, tex, win, audio, guiManager, entityManager, charactersSpritesheet, guiAtlasTex, guiAtlasTex2, titleFont, battleFont, menuFont, buttonFont, hoverFx, clickFx, returnFx);
	screenBattle->isActive = false;

	// Gui id goes from 15 to 16
	screenInventory = new ScreenInventory();
	screenInventory->Load(14, 15, this, battleSystem, win, guiManager, entityManager, audio, easing, guiAtlasTex2, buttonFont, hoverFx, clickFx);
	screenInventory->Disable();

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
		player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "Kenzo", EntitySubtype::PLAYER_KENZO, iPoint(4 * 32, 6 * 32));
		player->SetState(true);
		player = nullptr;
		currentPlayer = entityManager->playerList.At(0)->data;

		//// Create party member 2
		//player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "Maki", EntitySubtype::PLAYER_DOG, iPoint(4 * 32, 6 * 32));
		//player = nullptr;
		//// Create party member 3
		//player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "Rei", EntitySubtype::PLAYER_REI, iPoint(4 * 32, 6 * 32));
		//player = nullptr;
		//// Create party member 4
		//player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "Eiken", EntitySubtype::PLAYER_EIKEN, iPoint(4 * 32, 6 * 32));
		//player = nullptr;

		// LOAD FROM MAP_XML
		notifier->NotifyMapChange(MapType::HOUSE);
		//currentMap = MapType::HOUSE;
		SetUpTp();
		NPC* npc;
		npc = (NPC*)entityManager->CreateEntity(EntityType::NPC, "makiNPC", EntitySubtype::UNKNOWN, iPoint((2 * 32), (6 * 32) - 16));
		npc->name = "makiNPC";
		npc->width = 32;
		npc->height = 32;
		npc->spritePos = 15;
		npc->dialogIndex = 20;
		npc->stop = true;
		npc->SetTexture(npc->spritePos);
		if (npc->stop) npc->stopForever = true;
		npc = nullptr;
		Activator* activator;
		activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, "dialogTrigger", EntitySubtype::UNKNOWN, iPoint(8 * 32, 4 * 32));
		activator->width = 64;
		activator->height = 128;
		activator = nullptr;
	}

	guiManager->ToggleMouse();
	screenRoaming->SetCurrentPlayer(currentPlayer);

	return true;
}

bool SceneGameplay::Update(Input* input, float dt)
{
	controller = input->GetControllerState();

	/*	TO CHANGE PROGRESS BOLEANS 
	*	CHECK MAP TYPE
	*	CHECK ALL LEVERS/WALLS/ACTIVATORS IN THAT MAP BY NAME STRING
	*	IF THEY DON'T EXIST (DESTROYED) SET BOOLEAN TO TRUE
	*	AND FINALLY SAVE GAME PROGESS
	*/
	switch (currentMap)
	{
	case MapType::NONE:
		break;
	case MapType::CEMETERY:
		break;
	case MapType::HOUSE:
		break;
	case MapType::KANAGAWA:
		break;
	case MapType::RESTAURANT:
		break;
	case MapType::SECOND_RESTAURANT:
		break;
	case MapType::THIRD_RESTAURANT:
		break;
	case MapType::TOWN:
		break;
	case MapType::DOTONBORI:
		break;
	case MapType::SKYSCRAPER:
		if (gameProgress.hasFinishedPuzzle1 == false)
		{
			if (entityManager->SearchEntity("secretWallPuzzle1") == nullptr)
			{
				pugi::xml_document docData;
				pugi::xml_parse_result result = docData.load_file("save_game.xml");
				gameProgress.hasFinishedPuzzle1 = true;
				SaveGameProgress(docData.first_child().child("scenemanager").child("scenegameplay").child("gameProgress"));
				docData.save_file("save_game.xml");
			}
		}
		break;
	case MapType::SECRET_ROOM:
		break;
	case MapType::OSAKA:
		break;
	default:
		break;
	}

	if (input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		transitions->Transition(WhichAnimation::FADE_TO_BLACK, (Scene*)this, SceneType::ENDING_DEFEAT, 2);
	}
	if (input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		transitions->Transition(WhichAnimation::FADE_TO_BLACK, (Scene*)this, SceneType::ENDING_VICTORY, 2);
	}

	if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		pugi::xml_document docData;
		pugi::xml_node scenegameplayNode;

		pugi::xml_parse_result result = docData.load_file("save_game.xml");
		scenegameplayNode = docData.first_child().child("scenemanager").child("scenegameplay");

		SString tempName = scenegameplayNode.attribute("lastMap").name();
		if (tempName == "lastMap")
		{
			// Attribute currentMap exists
			scenegameplayNode.attribute("lastMap").set_value((int)this->currentMap);
		}
		else
		{
			// Attribute currentMap does not exist
			scenegameplayNode.append_attribute("lastMap").set_value((int)this->currentMap);
		}
		docData.save_file("save_game.xml");
	}
	// Player god mode
	if (input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN)
	{
		entityManager->TooglePlayerGodMode();
		map->drawColliders = !map->drawColliders;
		entityManager->entitiesBox = !entityManager->entitiesBox;
	}

	// Checking if we have to change the map
	if (notifier->OnMapChange() && notifier->GetNextMap() != currentMap)
	{
		app->SaveGameRequest();
		transitions->TransitionMap(WhichAnimation::FADE_TO_WHITE, this, 1.0f);
		notifier->ChangeMap();
	}
	if (readyToChangeMap)
	{
		readyToChangeMap = false;
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
			screenRoaming->SetCurrentPlayer(currentPlayer);
		}

		if (entityManager->playerList.At(entityManager->playerList.Find(currentPlayer))->next != nullptr)
		{
			currentPlayer->SetState(false);
			currentPlayer = entityManager->playerList.At(entityManager->playerList.Find(currentPlayer))->next->data;
			currentPlayer->SetState(true);
			screenRoaming->SetCurrentPlayer(currentPlayer);
		}
		else 
		{
			currentPlayer->SetState(false);
			currentPlayer = entityManager->playerList.At(0)->data;
			currentPlayer->SetState(true);
			screenRoaming->SetCurrentPlayer(currentPlayer);
		}
	}

	if (input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && screenBattle->isActive == false)
	{
		screenBattle->isActive = true; 
		battleSystem->SetupBattle(&entityManager->playerList, (Enemy*)entityManager->SearchEntity(notifier->GetEnemy()), &entityManager->itemList);

		screenBattle->EnableBattleButtons();

		currentState = GameState::BATTLE;
		screenBattle->isActive = true;

		focusedButtonId = 10;

		audio->StopMusic();
		audio->PlayMusic("Audio/Music/battle.ogg");
	}

	if (notifier->GetBattle())
	{
		for (int i = 0; i < entityManager->playerList.Count(); i++)
		{
			entityManager->playerList.At(i)->data->stopPlayer = true;
		}
		screenBattle->isActive = true;
		screenBattle->SetBackgroundMap(tex);
		battleSystem->SetupBattle(&entityManager->playerList, (Enemy*)entityManager->SearchEntity(notifier->GetEnemy()), &entityManager->itemList);

		screenBattle->EnableBattleButtons();

		// Reset battle bool to false in the notifier
		notifier->NotifyBattle();

		currentState = GameState::BATTLE;
		screenBattle->isActive = true;

		focusedButtonId = 10;

		audio->StopMusic();
		audio->PlayMusic("Audio/Music/battle.ogg");

		guiManager->ToggleMouse();
	}

	if (notifier->OnActivator())
	{
		notifier->SetActivatorMode(false);

		if (notifier->GetActivator()->name == "pc")
		{
			gameProgress.hasActivated = true;
		}
		else if (notifier->GetActivator()->name == "secretRoom")
		{
			notifier->GetActivator()->ChangeInteraction();
			/*if (gameProgress.hasPickedKey)
			{*/
			audio->PlayFx(doorOpenFx);
			notifier->NotifyMapChange(MapType::SECRET_ROOM);
			/*((Activator*)entityManager->SearchEntity("key"))->SetDrawState(DrawState::NONE);*/
			/*}
			else notifier->NotifyDialog(8);*/
		}
		else if (notifier->GetActivator()->name == "second_secretRoom")
		{
			notifier->GetActivator()->ChangeInteraction();

			if (gameProgress.hasKilledRestaurant2Enemies)
			{
				audio->PlayFx(doorOpenFx);
				notifier->NotifyMapChange(MapType::SECOND_SECRET_ROOM);
			}
			else
				notifier->NotifyDialog(8);
		}
		else if (notifier->GetActivator()->name == "third_secretRoom")
		{
			notifier->GetActivator()->ChangeInteraction();

			if (gameProgress.hasKilledRestaurant3Enemies)
			{
				audio->PlayFx(doorOpenFx);
				notifier->NotifyMapChange(MapType::THIRD_SECRET_ROOM);
			}
			else
				notifier->NotifyDialog(8);
		}
		else if (notifier->GetActivator()->name == "dialogTrigger")
		{
			notifier->NotifyDialog(15);
		}
		else if (notifier->GetActivator()->name == "secretRoomDialog")
		{
			notifier->NotifyDialog(16);
		}
		else if (notifier->GetActivator()->name == "dungeon1")
		{
			notifier->NotifyDialog(21);
		}
		else if (notifier->GetActivator()->name == "dungeon2")
		{
			notifier->NotifyDialog(22);
		}
		else if (notifier->GetActivator()->name == "dungeon3")
		{
			notifier->NotifyDialog(23);
		}
		else if (notifier->GetActivator()->name == "dungeon4")
		{
			notifier->NotifyDialog(25);
		}
		else if (notifier->GetActivator()->name == "dungeon5")
		{
			notifier->NotifyDialog(26);
		}
		else if (notifier->GetActivator()->name == "erikaTomb")
		{
			NPC* npc;
			npc = (NPC*)entityManager->CreateEntity(EntityType::NPC, "Erika", EntitySubtype::UNKNOWN, iPoint((22 * 32), (5 * 32)));
			npc->name = "Erika";
			npc->width = 32;
			npc->height = 32;
			npc->spritePos = 1;
			npc->dialogIndex = 17;
			npc->stop = true;
			npc->SetTexture(npc->spritePos);
			if (npc->stop) npc->stopForever = true;
			npc = nullptr;
		}
		else if (notifier->GetActivator()->GetDrawState() == ActivatorState::MAP)
		{
			/*if (notifier->GetActivator()->name == "key")
			{
				gameProgress.hasPickedKey = true;
			}*/
			if (notifier->GetActivator()->name == "Schichimi")
			{
				notifier->NotifyDialog(9);
			}
			else if (notifier->GetActivator()->name == "Karashi")
			{
				notifier->NotifyDialog(10);
			}
			else if (notifier->GetActivator()->name == "Rayu")
			{
				notifier->NotifyDialog(11);
			}
			else if (notifier->GetActivator()->name == "Sushi")
			{
				notifier->NotifyDialog(12);
			}
			else if (notifier->GetActivator()->name == "Oniguiri")
			{
				notifier->NotifyDialog(13);
			}
			else if (notifier->GetActivator()->name == "Sashimi")
			{
				notifier->NotifyDialog(14);
			}

			// Right now we don't have any activator that we want to have on the HUD
			notifier->GetActivator()->SetDrawState(ActivatorState::PICKED);
		}

		if (notifier->GetActivator()->name != "dialogTrigger" && notifier->GetActivator()->name != "secretRoomDialog" && notifier->GetActivator()->name != "dungeon1"
			&& notifier->GetActivator()->name != "dungeon2" && notifier->GetActivator()->name != "dungeon3" && notifier->GetActivator()->name != "dungeon4" && notifier->GetActivator()->name != "dungeon5")
			notifier->SetActivator(nullptr);
	}

	if (notifier->OnTeleport())
	{
		dialogSystem->NewDialog(29);
		notifier->SetTeleportMode(false);
		notifier->SetTeleport(nullptr);
	}

	if (!onDialog && notifier->OnDialog() && (input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN))
	{
		for (int i = 0; i < entityManager->playerList.Count(); i++)
		{
			entityManager->playerList.At(i)->data->stopPlayer = true;
		}
		dialogSystem->NewDialog(notifier->GetDialogIndex());

		onDialog = true;
	}
	else if (notifier->OnDialog() && notifier->GetActivator() != nullptr)
	{
		if (notifier->GetActivator()->name == "dialogTrigger")
		{
			for (int i = 0; i < entityManager->playerList.Count(); i++)
			{
				entityManager->playerList.At(i)->data->stopPlayer = true;
			}
			dialogSystem->NewDialog(notifier->GetDialogIndex());
			notifier->GetActivator()->destroy = true;
			notifier->SetActivator(nullptr);
		}
		else if (notifier->GetActivator()->name == "secretRoomDialog")
		{
			for (int i = 0; i < entityManager->playerList.Count(); i++)
			{
				entityManager->playerList.At(i)->data->stopPlayer = true;
			}
			dialogSystem->NewDialog(notifier->GetDialogIndex());
			notifier->GetActivator()->destroy = true;
			notifier->SetActivator(nullptr);
		}
		else if (notifier->GetActivator()->name == "dungeon1")
		{
			for (int i = 0; i < entityManager->playerList.Count(); i++)
			{
				entityManager->playerList.At(i)->data->stopPlayer = true;
			}
			dialogSystem->NewDialog(notifier->GetDialogIndex());
			notifier->GetActivator()->destroy = true;
			notifier->SetActivator(nullptr);
		}
		else if (notifier->GetActivator()->name == "dungeon2")
		{
			for (int i = 0; i < entityManager->playerList.Count(); i++)
			{
				entityManager->playerList.At(i)->data->stopPlayer = true;
			}
			dialogSystem->NewDialog(notifier->GetDialogIndex());
			notifier->GetActivator()->destroy = true;
			notifier->SetActivator(nullptr);
		}
		else if (notifier->GetActivator()->name == "dungeon3")
		{
			for (int i = 0; i < entityManager->playerList.Count(); i++)
			{
				entityManager->playerList.At(i)->data->stopPlayer = true;
			}
			dialogSystem->NewDialog(notifier->GetDialogIndex());
			notifier->GetActivator()->destroy = true;
			notifier->SetActivator(nullptr);
		}
		else if (notifier->GetActivator()->name == "dungeon4")
		{
			for (int i = 0; i < entityManager->playerList.Count(); i++)
			{
				entityManager->playerList.At(i)->data->stopPlayer = true;
			}
			dialogSystem->NewDialog(notifier->GetDialogIndex());
			notifier->GetActivator()->destroy = true;
			notifier->SetActivator(nullptr);
		}
		else if (notifier->GetActivator()->name == "dungeon5")
		{
			for (int i = 0; i < entityManager->playerList.Count(); i++)
			{
				entityManager->playerList.At(i)->data->stopPlayer = true;
			}
			dialogSystem->NewDialog(notifier->GetDialogIndex());
			notifier->GetActivator()->destroy = true;
			notifier->SetActivator(nullptr);
		}
	}
	if (dialogSystem->DialogHasFinished())
	{
		for (int i = 0; i < entityManager->playerList.Count(); i++)
		{
			entityManager->playerList.At(i)->data->stopPlayer = false;
		}
		notifier->SetDialogMode(false);
		dialogSystem->SetDialogFinished(false);

		if (notifier->GetDialogIndex() == 7)
		{
			gameProgress.hasSpoken = true;
		}
		else if (dialogSystem->GetDialogIndex() == 15)
		{
			entityManager->DestroyEntity(entityManager->SearchEntity("makiNPC"));
			// Create party member 2
			Player* player;
			player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "Maki", EntitySubtype::PLAYER_DOG, currentPlayer->position);
			player = nullptr;
			gameProgress.hasCalledCook = true;
		}
		else if (dialogSystem->GetDialogIndex() == 16)
		{
			gameProgress.hasVisitedRestaurantQuest = true;
		}
		else if (dialogSystem->GetDialogIndex() == 17)
		{
			gameProgress.hasVisitedErikaTombstone = true;
			entityManager->DestroyEntity(entityManager->SearchEntity("Erika"));
		}
		else if (dialogSystem->GetDialogIndex() == 18)
		{
			entityManager->DestroyEntity(entityManager->SearchEntity("reiNPC"));
			// Create party member 3
			Player* player;
			player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "Rei", EntitySubtype::PLAYER_REI, currentPlayer->position);
			player = nullptr;
			gameProgress.hasSavedFirstApprentice = true;
		}
		else if (dialogSystem->GetDialogIndex() == 19)
		{
			entityManager->DestroyEntity(entityManager->SearchEntity("eikenNPC"));
			// Create party member 4
			Player* player;
			player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "Eiken", EntitySubtype::PLAYER_EIKEN, currentPlayer->position);
			player = nullptr;
			gameProgress.hasSavedLastApprentice = true;
		}
		else if (dialogSystem->GetDialogIndex() == 21)
		{
			gameProgress.hasTriggeredDialogDungeon1 = true;

		}
		else if (dialogSystem->GetDialogIndex() == 22)
		{
			gameProgress.hasTriggeredDialogDungeon2 = true;

		}
		else if (dialogSystem->GetDialogIndex() == 23)
		{
			gameProgress.hasTriggeredDialogDungeon3 = true;

		}
		else if (dialogSystem->GetDialogIndex() == 25)
		{
			gameProgress.hasTriggeredDialogDungeon4 = true;

		}
		else if (dialogSystem->GetDialogIndex() == 26)
		{
			gameProgress.hasTriggeredDialogDungeon5 = true;

		}
		onDialog = false;
	}

	switch (currentState)
	{
	case GameState::ROAMING:
		screenRoaming->Update(input, dt);
		break;
	case GameState::PAUSE:
		screenPause->Update(input, dt, focusedButtonId);
		break;
	case GameState::SETTINGS:
		if (input->GetControllerState())
			screenSettings->Update(input, dt, focusedButtonId);
		break;
	case GameState::EXIT:
		break;
	case GameState::BATTLE:
		screenBattle->Update(input, dt, focusedButtonId);

		if (battleSystem->HasOpenedInventory() == false)
		{
			if (screenBattle->GetBattleSystem()->playerState == PlayerState::ITEM && !battleSystem->HasClosedInventory())
			{
				battleSystem->SetInventoryOpening(true);
				screenBattle->DisableBattleButtons();
				screenInventory->Enable();
			}
		}
		else
		{
			screenInventory->Update(input, dt, focusedButtonId);
		}
		break;
	case GameState::INVENTORY:
		screenInventory->Update(input, dt, focusedButtonId);
		break;
	default:
		break;
	}

	if (!screenPause->isActive &&
		!screenSettings->isActive &&
		!screenBattle->isActive &&
		!screenInventory->isActive)
	{
		questManager->Update(dt);
	}

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
		if (battleSystem->HasOpenedInventory())
			screenInventory->Draw(render);
		break;
	case GameState::INVENTORY:
		screenInventory->Draw(render);
		break;
	default:
		break;
	}

	if (!screenPause->isActive &&
		!screenSettings->isActive &&
		!screenBattle->isActive &&
		!screenInventory->isActive)
	{
		/*questManager->PostUpdate();*/

		if (currentMap == MapType::SKYSCRAPER && gameProgress.hasKilledOfficers && !gameProgress.hasActivated)
		{
			render->DrawTexture(signal, 4160, 120, &signalAnim.GetCurrentFrame());
		}

		if (currentMap == MapType::CEMETERY && gameProgress.hasVisitedRestaurantQuest && !gameProgress.hasVisitedErikaTombstone)
		{
			render->DrawTexture(signal, 1675, 325, &signalAnim.GetCurrentFrame());
		}
	}

	// Draw interaction notifier label
	if (notifier->GetInteractionNotifier() && notifier->GetInteractingEntity() != nullptr)
	{
		SDL_Rect rect = { 102,27,261,51 };
		render->DrawTexture(guiAtlasTex2, 640 - rect.w / 2, 500, &rect, 0);

		if (notifier->GetInteractingEntity()->type == EntityType::ENEMY)
		{
			if (!controller)
				render->DrawText(buttonFont, "Press F to fight", 640 - rect.w / 2 + 25, 500 + 7, 35, 1, { 128,128,128,255 });
			else
				render->DrawText(buttonFont, "Press A to fight", 640 - rect.w / 2 + 25, 500 + 7, 35, 1, { 128,128,128,255 });
		}
		else if (notifier->GetInteractingEntity()->type == EntityType::NPC)
		{
			if (!controller)
				render->DrawText(buttonFont, "Press F to talk", 640 - rect.w / 2 + 30, 500 + 7, 35, 1, { 128,128,128,255 });
			else
				render->DrawText(buttonFont, "Press A to talk", 640 - rect.w / 2 + 30, 500 + 7, 35, 1, { 128,128,128,255 });
		}
		else
		{
			if (!controller)
				render->DrawText(buttonFont, "Press F to interact", 640 - rect.w / 2 + 5, 500 + 7, 35, 1, { 128,128,128,255 });
			else
				render->DrawText(buttonFont, "Press A to interact", 640 - rect.w / 2 + 5, 500 + 7, 35, 1, { 128,128,128,255 });
		}

		notifier->NotifyInteraction();
	}

	return true;
}

bool SceneGameplay::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	// TODO: Unload all resources
	RELEASE(battleSystem);
	RELEASE(questManager);

	// Remove map
	map->CleanUp();

	// Stop music

	// Release fonts
	RELEASE(titleFont);
	RELEASE(battleFont);
	RELEASE(menuFont);
	RELEASE(buttonFont);

	// Unload textures
	tex->UnLoad(charactersSpritesheet);
	tex->UnLoad(backgroundTex);
	tex->UnLoad(guiAtlasTex);
	tex->UnLoad(guiAtlasTex2);
	tex->UnLoad(guiAtlasOut);
	tex->UnLoad(titlesTex);
	
	tex->UnLoad(cast1);
	tex->UnLoad(enemyCast);
	tex->UnLoad(indicator);
	tex->UnLoad(signal);

	// Unload Fx
	audio->UnloadFx(clickFx);
	audio->UnloadFx(hoverFx);
	audio->UnloadFx(returnFx);
	audio->UnloadFx(doorOpenFx);
	audio->UnloadFx(bagOpenFx);

	// Destory GUI Controls
	guiManager->DestroyGuiControl(btnAttack);
	guiManager->DestroyGuiControl(btnDefend);
	guiManager->DestroyGuiControl(btnItem);
	guiManager->DestroyGuiControl(btnRun);

	notifier = nullptr;

	// Unload screens
	screenRoaming->Unload(tex, audio, guiManager);
	screenPause->Unload(tex, audio, guiManager);
	screenSettings->Unload(tex, audio, guiManager);
	screenBattle->Unload(tex, audio, guiManager);
	screenInventory->Unload(tex, audio, guiManager);

	// Delete screens
	RELEASE(screenRoaming);
	RELEASE(screenPause);
	RELEASE(screenSettings);
	RELEASE(screenBattle);
	RELEASE(screenInventory);

	entityManager->CleanUp();

	// Removing used modules
	this->assetsManager = nullptr;
	this->entityManager = nullptr;
	this->guiManager = nullptr;
	this->win = nullptr;
	this->dialogSystem = nullptr;
	this->audio = nullptr;

	LOG("Unload gameplay finished.");
	return true;
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
			// Entering settings from pause
			currentState = GameState::SETTINGS;

			screenPause->Disable();
			screenSettings->Enable();

			// Vsync control is disabled if you acces from gameplay
			//this->guiManager->controls.At(7)->data->state = GuiControlState::DISABLED;

			audio->StopMusic();
			audio->PlayMusic("Audio/Music/menu_settings.ogg");

			focusedButtonId = 6;
		}
		else if (control->id == 1)
		{
			// Exiting to menu
			currentState = GameState::EXIT;

			screenRoaming->Disable();
			screenPause->Disable();

			focusedButtonId = 0;

			TransitionToScene(SceneType::TITLE);
		}
		else if (control->id == 2)
		{
			// From Main Screen of the mobile to Quest Screen
			screenPause->state = MobileState::QUEST;
			for (int i = 0; i <= 4; ++i)
				guiManager->controls.At(i)->data->state = GuiControlState::DISABLED;
		}
		else if (control->id == 3)
		{
			// From Main Screen of the mobile to Team Screen
			screenPause->state = MobileState::TEAM;
			for (int i = 0; i <= 4; ++i)
				guiManager->controls.At(i)->data->state = GuiControlState::DISABLED;
		}
		else if (control->id == 4)
		{
			// From Main Screen of the mobile to Map Screen
			screenPause->state = MobileState::MAP;
			for (int i = 0; i <= 4; ++i)
				guiManager->controls.At(i)->data->state = GuiControlState::DISABLED;
		}
		else if (control->id == 5)
		{
			if (screenPause->state != MobileState::MAIN)
			{
				if (screenPause->state == MobileState::QUEST)
					focusedButtonId = 2;
				else if (screenPause->state == MobileState::TEAM)
					focusedButtonId = 3;
				else if (screenPause->state == MobileState::MAP)
					focusedButtonId = 4;

				screenPause->state = MobileState::MAIN;
				for (int i = 0; i <= 4; ++i)
					guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
			}
			else if (pauseTimer.ReadSec() > 0.5f)
			{
				currentState = GameState::ROAMING;

				screenPause->Disable();
				screenRoaming->Enable();

				guiManager->ToggleMouse();

				for (int i = 0; i < entityManager->playerList.Count(); i++)
				{
					entityManager->playerList.At(i)->data->stopPlayer = false;
				}

				PlayMapMusic();

				focusedButtonId = 0;
			}
		}
		else if (control->id == 9)
		{
			// Returning from settings to pause
			currentState = GameState::PAUSE;

			screenSettings->Disable();
			screenPause->Enable(true);

			focusedButtonId = 0;
		}
		break;
	}
	case GuiControlType::BUTTON:
	{
		if (control->id == 10)
		{
			if (battleSystem->playerState != PlayerState::NONE || battleSystem->battleState != BattleState::PLAYER_TURN)
				break;

			battleSystem->playerState = PlayerState::ATTACK;
			screenBattle->HideButtons();
		}
		else if (control->id == 11)
		{
			if (battleSystem->playerState != PlayerState::NONE || battleSystem->battleState != BattleState::PLAYER_TURN)
				break;

			battleSystem->playerState = PlayerState::DEFENSE;
			screenBattle->HideButtons();
		}
		else if (control->id == 12)
		{
			if (battleSystem->playerState != PlayerState::NONE || battleSystem->battleState != BattleState::PLAYER_TURN)
				break;

			battleSystem->playerState = PlayerState::ITEM;
		}
		else if (control->id == 13)
		{
			if (battleSystem->playerState != PlayerState::NONE || battleSystem->battleState != BattleState::PLAYER_TURN)
				break;

			battleSystem->playerState = PlayerState::RUN;
			battleSystem->GetEnemy()->readyForCombat = true;
			ExitBattle();
		}
		else if (control->id == 14)
		{
			if (screenInventory->listInvItems.Count() != 0)
			{
				screenInventory->SetHasClickedConsume(true);

				if (currentState == GameState::BATTLE)
				{
					battleSystem->SetHasClickedConsume(true);
					screenInventory->ManageItemConsumption();
				}
			}

			if (currentState == GameState::BATTLE)
			{
				battleSystem->SetInventoryOpening(false);
				battleSystem->SetInventoryClosure(true);

				screenInventory->Disable();
				screenBattle->EnableBattleButtons();
			}
		}
		else if (control->id == 15)
		{
			if (currentState == GameState::BATTLE)
			{
				battleSystem->SetInventoryOpening(false);
				battleSystem->SetInventoryClosure(true);

				screenInventory->Disable();
				screenBattle->EnableBattleButtons();
			}
			else
			{
				currentState = GameState::ROAMING;

				screenInventory->Disable();
				screenRoaming->Enable();

				guiManager->ToggleMouse();

				for (int i = 0; i < entityManager->playerList.Count(); i++)
				{
					entityManager->playerList.At(i)->data->stopPlayer = false;
				}

				PlayMapMusic();

				focusedButtonId = 0;
			}
		}
		break;
	}
	case GuiControlType::CHECKBOX:
	{
		// Fullscreen
		if (control->id == 8)
		{
			if (screenSettings->checkFullScreen->GetCheck())
				SDL_SetWindowFullscreen(win->window, 1);
			else
				SDL_SetWindowFullscreen(win->window, 0);
		}
		pugi::xml_document docData;
		pugi::xml_node screenNode;

		pugi::xml_parse_result result = docData.load_file("save_game.xml");

		// Check result for loading errors
		if (result == NULL)
		{
			LOG("Could not load map info xml file map_info.xml. pugi error: %s", result.description());
		}
		else
		{
			screenNode = docData.child("game_state").child("screen");
			screenSettings->SaveState(screenNode);
			docData.save_file("save_game.xml");
		}
		break;
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 6)
		{
			GuiSlider* tempSlider = (GuiSlider*)this->guiManager->controls.At(8)->data;
			int value = tempSlider->GetValue();
			audio->ChangeMusicVolume(value);
		}
		else if (control->id == 7)
		{
			GuiSlider* tempSlider = (GuiSlider*)this->guiManager->controls.At(9)->data;
			int value = tempSlider->GetValue();
			audio->ChangeFxVolume(value);
		}
		break;
	}
	default:
		break;
	}
	return true;
}

void SceneGameplay::OpenPause()
{
	// Entering pause from roaming
	currentState = GameState::PAUSE;
	audio->ChangeMusicVolume(SDL_MIX_MAXVOLUME / 10);

	screenRoaming->Disable();
	screenPause->Enable();

	for (int i = 0; i < entityManager->playerList.Count(); i++)
	{
		entityManager->playerList.At(i)->data->stopPlayer = true;
	}

	focusedButtonId = 0;

	pauseTimer.Start();
}

void SceneGameplay::OpenInventory()
{
	// Entering inventory from roaming
	currentState = GameState::INVENTORY;

	screenRoaming->Disable();
	screenInventory->Enable();

	for (int i = 0; i < entityManager->playerList.Count(); i++)
	{
		entityManager->playerList.At(i)->data->stopPlayer = true;
	}

	audio->PlayFx(bagOpenFx);

	focusedButtonId = 14;
}

bool SceneGameplay::LoadState(pugi::xml_node& scenegameplay)
{
	/* If different map then change map*/
	if ((int)currentMap != scenegameplay.attribute("lastMap").as_int())
	{
		MapType nextMap = (MapType)scenegameplay.attribute("lastMap").as_int();
		this->notifier->NotifyMapChange(nextMap);
		SetMapTransitionState();
	}
	pugi::xml_node screenNode = scenegameplay.parent().parent().child("screen");
	screenSettings->LoadState(screenNode);

	/*Load inventory*/
	LoadInventory(screenNode.child("screeninventory"));

	/*Load game progress data */
	LOG("LOADING GAME PROGRESS");
	pugi::xml_node gameProgressNode = scenegameplay.child("gameProgress");
	LoadGameProgress(gameProgressNode);

	/*Restart quests*/
	questManager->CleanUp();
	questManager->Start();

	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& scenegameplay) const
{
	// Save each screen data
	pugi::xml_node screenNode = scenegameplay.parent().parent().child("screen");
	screenSettings->SaveState(screenNode);
	screenInventory->SaveState(screenNode);

	LOG("SAVING GAME PROGRESS");
	/* ---------- CHECKS IF THE Child WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	pugi::xml_node gameProgressNode;
	SString tempName = scenegameplay.child("gameProgress").name();

	if (tempName == "gameProgress")
	{
		// Attribute currentMap exists
		gameProgressNode = scenegameplay.child("gameProgress");
	}
	else
	{
		// Attribute currentMap does not exist
		gameProgressNode = scenegameplay.append_child("gameProgress");
	}

	SaveEntities(scenegameplay);
	SaveGameProgress(gameProgressNode);

	return true;
}

void SceneGameplay::SaveGameProgress(pugi::xml_node& data)const
{
	SString tempName = data.attribute("xp").name();

	if (tempName == "xp")
		data.attribute("xp").set_value(this->gameProgress.xp);
	else
		data.append_attribute("xp").set_value(this->gameProgress.xp);

	tempName = data.attribute("gold").name();
	if (tempName == "gold")
		data.attribute("gold").set_value(this->gameProgress.gold);
	else
		data.append_attribute("gold").set_value(this->gameProgress.gold);

	tempName = data.attribute("hasCalledCook").name();
	if (tempName == "hasCalledCook")
		data.attribute("hasCalledCook").set_value(this->gameProgress.hasCalledCook);
	else
		data.append_attribute("hasCalledCook").set_value(this->gameProgress.hasCalledCook);

	tempName = data.attribute("hasVisitedRestaurantQuest").name();
	if (tempName == "hasVisitedRestaurantQuest")
		data.attribute("hasVisitedRestaurantQuest").set_value(this->gameProgress.hasVisitedRestaurantQuest);
	else
		data.append_attribute("hasVisitedRestaurantQuest").set_value(this->gameProgress.hasVisitedRestaurantQuest);

	tempName = data.attribute("hasSavedFirstApprentice").name();
	if (tempName == "hasSavedFirstApprentice")
		data.attribute("hasSavedFirstApprentice").set_value(this->gameProgress.hasSavedFirstApprentice);
	else
		data.append_attribute("hasSavedFirstApprentice").set_value(this->gameProgress.hasSavedFirstApprentice);
		
	tempName = data.attribute("hasSavedLastApprentice").name();
	if (tempName == "hasSavedLastApprentice")
		data.attribute("hasSavedLastApprentice").set_value(this->gameProgress.hasSavedLastApprentice);
	else
		data.append_attribute("hasSavedLastApprentice").set_value(this->gameProgress.hasSavedLastApprentice);
	
	tempName = data.attribute("hasVisitedErikaTombstone").name();
	if (tempName == "hasVisitedErikaTombstone")
		data.attribute("hasVisitedErikaTombstone").set_value(this->gameProgress.hasVisitedErikaTombstone);
	else
		data.append_attribute("hasVisitedErikaTombstone").set_value(this->gameProgress.hasVisitedErikaTombstone);


	tempName = data.attribute("hasSpoken").name();
	if (tempName == "hasSpoken")
		data.attribute("hasSpoken").set_value(this->gameProgress.hasSpoken);
	else
		data.append_attribute("hasSpoken").set_value(this->gameProgress.hasSpoken);

	tempName = data.attribute("numKilledOfficers").name();
	if (tempName == "numKilledOfficers")
		data.attribute("numKilledOfficers").set_value(this->gameProgress.numKilledOfficers);
	else
		data.append_attribute("numKilledOfficers").set_value(this->gameProgress.numKilledOfficers);

	tempName = data.attribute("hasKilledOfficers").name();
	if (tempName == "hasKilledOfficers")
		data.attribute("hasKilledOfficers").set_value(this->gameProgress.hasKilledOfficers);
	else
		data.append_attribute("hasKilledOfficers").set_value(this->gameProgress.hasKilledOfficers);

	tempName = data.attribute("hasActivated").name();
	if (tempName == "hasActivated")
		data.attribute("hasActivated").set_value(this->gameProgress.hasActivated);
	else
		data.append_attribute("hasActivated").set_value(this->gameProgress.hasActivated);

	/*tempName = data.attribute("hasPickedKey").name();
	if (tempName == "hasPickedKey")
		data.attribute("hasPickedKey").set_value(this->gameProgress.hasPickedKey);
	else
		data.append_attribute("hasPickedKey").set_value(this->gameProgress.hasPickedKey);*/

	tempName = data.attribute("hasFinishedPuzzle1").name();
	if (tempName == "hasFinishedPuzzle1")
		data.attribute("hasFinishedPuzzle1").set_value(this->gameProgress.hasFinishedPuzzle1);
	else
		data.append_attribute("hasFinishedPuzzle1").set_value(this->gameProgress.hasFinishedPuzzle1);

	// Save map booleans
	data = data.next_sibling("map");

	tempName = data.attribute("hasVisitedCemetery").name();
	if (tempName == "hasVisitedCemetery")
		data.attribute("hasVisitedCemetery").set_value(this->gameProgress.hasVisitedCemetery);
	else
		data.append_attribute("hasVisitedCemetery").set_value(this->gameProgress.hasVisitedCemetery);

	tempName = data.attribute("hasVisitedHouse").name();
	if (tempName == "hasVisitedHouse")
		data.attribute("hasVisitedHouse").set_value(this->gameProgress.hasVisitedHouse);
	else
		data.append_attribute("hasVisitedHouse").set_value(this->gameProgress.hasVisitedHouse);

	tempName = data.attribute("hasVisitedMediumCity").name();
	if (tempName == "hasVisitedMediumCity")
		data.attribute("hasVisitedMediumCity").set_value(this->gameProgress.hasVisitedMediumCity);
	else
		data.append_attribute("hasVisitedMediumCity").set_value(this->gameProgress.hasVisitedMediumCity);

	tempName = data.attribute("hasVisitedRestaurant").name();
	if (tempName == "hasVisitedRestaurant")
		data.attribute("hasVisitedRestaurant").set_value(this->gameProgress.hasVisitedRestaurant);
	else
		data.append_attribute("hasVisitedRestaurant").set_value(this->gameProgress.hasVisitedRestaurant);

	tempName = data.attribute("hasVisitedSecondRestaurant").name();
	if (tempName == "hasVisitedSecondRestaurant")
		data.attribute("hasVisitedSecondRestaurant").set_value(this->gameProgress.hasVisitedSecondRestaurant);
	else
		data.append_attribute("hasVisitedThirdRestaurant").set_value(this->gameProgress.hasVisitedThirdRestaurant);
	tempName = data.attribute("hasVisitedThirdRestaurant").name();
	if (tempName == "hasVisitedThirdRestaurant")
		data.attribute("hasVisitedThirdRestaurant").set_value(this->gameProgress.hasVisitedThirdRestaurant);
	else
		data.append_attribute("hasVisitedThirdRestaurant").set_value(this->gameProgress.hasVisitedThirdRestaurant);

	tempName = data.attribute("hasVisitedTown").name();
	if (tempName == "hasVisitedTown")
		data.attribute("hasVisitedTown").set_value(this->gameProgress.hasVisitedTown);
	else
		data.append_attribute("hasVisitedTown").set_value(this->gameProgress.hasVisitedTown);

	tempName = data.attribute("hasVisitedBigCity").name();
	if (tempName == "hasVisitedBigCity")
		data.attribute("hasVisitedBigCity").set_value(this->gameProgress.hasVisitedBigCity);
	else
		data.append_attribute("hasVisitedBigCity").set_value(this->gameProgress.hasVisitedBigCity);
	
	tempName = data.attribute("hasVisitedSkyScraper").name();
	if (tempName == "hasVisitedSkyScraper")
		data.attribute("hasVisitedSkyScraper").set_value(this->gameProgress.hasVisitedSkyScraper);
	else
		data.append_attribute("hasVisitedSkyScraper").set_value(this->gameProgress.hasVisitedSkyScraper);

	tempName = data.attribute("hasVisitedSecretRoom").name();
	if (tempName == "hasVisitedSecretRoom")
		data.attribute("hasVisitedSecretRoom").set_value(this->gameProgress.hasVisitedSecretRoom);
	else
		data.append_attribute("hasVisitedSecretRoom").set_value(this->gameProgress.hasVisitedSecretRoom);

	tempName = data.attribute("hasVisitedOsaka").name();
	if (tempName == "hasVisitedOsaka")
		data.attribute("hasVisitedOsaka").set_value(this->gameProgress.hasVisitedOsaka);
	else
		data.append_attribute("hasVisitedOsaka").set_value(this->gameProgress.hasVisitedOsaka);

	tempName = data.attribute("hasTriggeredDialogDungeon1").name();
	if (tempName == "hasTriggeredDialogDungeon1")
		data.attribute("hasTriggeredDialogDungeon1").set_value(this->gameProgress.hasTriggeredDialogDungeon1);
	else
		data.append_attribute("hasTriggeredDialogDungeon1").set_value(this->gameProgress.hasTriggeredDialogDungeon1);

	tempName = data.attribute("hasTriggeredDialogDungeon2").name();
	if (tempName == "hasTriggeredDialogDungeon2")
		data.attribute("hasTriggeredDialogDungeon2").set_value(this->gameProgress.hasTriggeredDialogDungeon2);
	else
		data.append_attribute("hasTriggeredDialogDungeon2").set_value(this->gameProgress.hasTriggeredDialogDungeon2);

	tempName = data.attribute("hasTriggeredDialogDungeon3").name();
	if (tempName == "hasTriggeredDialogDungeon3")
		data.attribute("hasTriggeredDialogDungeon3").set_value(this->gameProgress.hasTriggeredDialogDungeon3);
	else
		data.append_attribute("hasTriggeredDialogDungeon3").set_value(this->gameProgress.hasTriggeredDialogDungeon3);

	tempName = data.attribute("hasTriggeredDialogDungeon4").name();
	if (tempName == "hasTriggeredDialogDungeon4")
		data.attribute("hasTriggeredDialogDungeon4").set_value(this->gameProgress.hasTriggeredDialogDungeon4);
	else
		data.append_attribute("hasTriggeredDialogDungeon4").set_value(this->gameProgress.hasTriggeredDialogDungeon4);

	tempName = data.attribute("hasTriggeredDialogDungeon5").name();
	if (tempName == "hasTriggeredDialogDungeon5")
		data.attribute("hasTriggeredDialogDungeon5").set_value(this->gameProgress.hasTriggeredDialogDungeon5);
	else
		data.append_attribute("hasTriggeredDialogDungeon5").set_value(this->gameProgress.hasTriggeredDialogDungeon5);
}

void SceneGameplay::LoadGameProgress(pugi::xml_node& data)
{
	this->gameProgress.xp = data.attribute("xp").as_int();
	this->gameProgress.gold = data.attribute("gold").as_int();
	this->gameProgress.hasCalledCook = data.attribute("hasCalledCook").as_bool();
	this->gameProgress.hasVisitedRestaurantQuest = data.attribute("hasVisitedRestaurantQuest").as_bool();
	this->gameProgress.hasSavedFirstApprentice = data.attribute("hasSavedFirstApprentice").as_bool();
	this->gameProgress.hasSavedLastApprentice = data.attribute("hasSavedLastApprentice").as_bool();
	this->gameProgress.hasVisitedErikaTombstone = data.attribute("hasVisitedErikaTombstone").as_bool();
	this->gameProgress.hasSpoken = data.attribute("hasSpoken").as_bool();
	this->gameProgress.numKilledOfficers = data.attribute("numKilledOfficers").as_int();
	this->gameProgress.hasKilledOfficers = data.attribute("hasKilledOfficers").as_bool();
	this->gameProgress.hasActivated = data.attribute("hasActivated").as_bool();
	/*this->gameProgress.hasPickedKey = data.attribute("hasPickedKey").as_bool();*/
	this->gameProgress.hasFinishedPuzzle1 = data.attribute("hasFinishedPuzzle1").as_bool();

	data = data.next_sibling("map");
	this->gameProgress.hasVisitedCemetery = data.attribute("hasVisitedCemetery").as_bool();
	this->gameProgress.hasVisitedHouse = data.attribute("hasVisitedHouse").as_bool();
	this->gameProgress.hasVisitedMediumCity = data.attribute("hasVisitedMediumCity").as_bool();
	this->gameProgress.hasVisitedRestaurant = data.attribute("hasVisitedRestaurant").as_bool();
	this->gameProgress.hasVisitedTown = data.attribute("hasVisitedTown").as_bool();
	this->gameProgress.hasVisitedBigCity = data.attribute("hasVisitedBigCity").as_bool();
	this->gameProgress.hasVisitedSkyScraper = data.attribute("hasVisitedSkyScraper").as_bool();
	this->gameProgress.hasVisitedSecretRoom = data.attribute("hasVisitedSecretRoom").as_bool();
	this->gameProgress.hasVisitedOsaka = data.attribute("hasVisitedOsaka").as_bool();

	this->gameProgress.hasTriggeredDialogDungeon1 = data.attribute("hasTriggeredDialogDungeon1").as_bool();
	this->gameProgress.hasTriggeredDialogDungeon2 = data.attribute("hasTriggeredDialogDungeon2").as_bool();
	this->gameProgress.hasTriggeredDialogDungeon3 = data.attribute("hasTriggeredDialogDungeon3").as_bool();
	this->gameProgress.hasTriggeredDialogDungeon4 = data.attribute("hasTriggeredDialogDungeon4").as_bool();
	this->gameProgress.hasTriggeredDialogDungeon5 = data.attribute("hasTriggeredDialogDungeon5").as_bool();
}

void SceneGameplay::SaveEntities(pugi::xml_node& scenegameplay) const
{
	entityManager->SaveStateInfo(scenegameplay, currentMap);
}

void SceneGameplay::LoadEntities(pugi::xml_node& scenegameplay)
{
}

void SceneGameplay::LoadInventory(pugi::xml_node& screeninventory)
{
	//---------------------------------//
	/*Delete inventory items*/
	LOG("LOADING INVENTORY");
	for (ListItem<InvItem*>* invItem = screenInventory->listInvItems.start; invItem; invItem = invItem->next)
	{
		screenInventory->listInvItems.Del(invItem);
	}
	screenInventory->listInvItems.Clear();

	pugi::xml_node screenInventoryNode;
	screenInventoryNode = screeninventory;
	int invCount = screenInventoryNode.attribute("inventoryCount").as_int();

	/*Load inventory items*/
	pugi::xml_node invSlotNode = screenInventoryNode.child("invSlot");
	Item* item = nullptr;
	for (int i = 0; i <= invCount; ++i)
	{
		EntitySubtype subtype = (EntitySubtype)invSlotNode.attribute("entitySubType").as_int();
		if ((int)subtype >= 5 && (int)subtype <= 10)
		{
			item = (Item*)entityManager->CreateEntity(EntityType::ITEM, invSlotNode.attribute("name").as_string(), subtype);
			item->onMap = false;
			item->name = invSlotNode.attribute("name").as_string();
			item->SetDescription(invSlotNode.attribute("description").as_string());
			if (item->collider != nullptr)
			{
				item->collider->pendingToDelete = true;
				item->collider = nullptr;
			}
			AddItemToInvItemsList(item);
			item = nullptr;
		}
		invSlotNode = invSlotNode.next_sibling("invSlot");
	}
	LOG("INVENTORY LOADED");
	//---------------------------------//
}

Player* SceneGameplay::GetCurrentPlayer()
{
	return currentPlayer;
}

MapType SceneGameplay::GetCurrentMap()
{
	return currentMap;
}

void SceneGameplay::ExitBattle()
{
	PlayMapMusic();

	for (int i = 10; i <= 13; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
	}

	screenBattle->isActive = false;
	battleSystem->ResetBattle();

	// Reset battle bool to false in the general battle state bool
	notifier->SetBattleState(false);

	// Change game state to roaming
	currentState = GameState::ROAMING;

	screenRoaming->Enable();

	for (int i = 0; i < entityManager->playerList.Count(); i++)
	{
		entityManager->playerList.At(i)->data->stopPlayer = false;
	}

	guiManager->ToggleMouse();
}

void SceneGameplay::SetUpTp()
{
	map->CleanUp();

	MapType previousMap = MapType::NONE;
	previousMap = currentMap;
	currentMap = notifier->GetNextMap();
	notifier->SetCurrentMap(currentMap);

	// Save player stats
	int size = entityManager->playerList.Count();

	Stats *playerStats = new Stats[size];
	ListItem<Player*>* list1;

	int i = 0;
	for (list1 = entityManager->playerList.start; list1 != NULL; list1 = list1->next)
	{
		playerStats[i] = list1->data->stats;
		++i;
	}
	RELEASE(list1);

	// Create map
	switch (currentMap)
	{
	case MapType::CEMETERY:
		if (map->Load("Cemetery", "cemetery.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			
			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
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
		}
		break;
	case MapType::KANAGAWA:
		if (map->Load("MediumCity", "mediumcity.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
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
		}
		break;
	case MapType::SECOND_RESTAURANT:
		if (map->Load("Restaurant", "restaurant.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
		}
		break;
	case MapType::THIRD_RESTAURANT:
		if (map->Load("Restaurant", "restaurant.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
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
		}
		break;
	case MapType::DOTONBORI:
		if (map->Load("bigCity", "bigCity.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);
			
			RELEASE_ARRAY(data);
			audio->StopMusic();
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
		}
		break;
	case MapType::SECRET_ROOM:
		if (map->Load("Restaurant", "secretRoom.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
		}
		break;
	case MapType::SECOND_SECRET_ROOM:
		if (map->Load("Restaurant", "secretRoom.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
		}
		break;
	case MapType::THIRD_SECRET_ROOM:
		if (map->Load("Restaurant", "secretRoom.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
		}
		break;
	case MapType::OSAKA:
		if (map->Load("Osaka", "osaka.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
			audio->StopMusic();
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
		bool hasVisitedLocation = false;
		mapNode = docData.child("map");
		// GET THE NODE TO THE NEW MAP
		switch (currentMap)
		{
		case MapType::NONE:
			break;
		case MapType::CEMETERY:
			mapNode = mapNode.child("cemetery");
			hasVisitedLocation = gameProgress.hasVisitedCemetery;
			break;
		case MapType::HOUSE:
			mapNode = mapNode.child("house");
			hasVisitedLocation = gameProgress.hasVisitedHouse;
			break;
		case MapType::KANAGAWA:
			mapNode = mapNode.child("mediumCity");
			hasVisitedLocation = gameProgress.hasVisitedMediumCity;
			break;
		case MapType::RESTAURANT:
			mapNode = mapNode.child("restaurant");

			hasVisitedLocation = gameProgress.hasVisitedRestaurant;
			break;
		case MapType::SECOND_RESTAURANT:
			mapNode = mapNode.child("second_restaurant");
			hasVisitedLocation = gameProgress.hasVisitedSecondRestaurant;
			break;
		case MapType::THIRD_RESTAURANT:
			mapNode = mapNode.child("third_restaurant");
			hasVisitedLocation = gameProgress.hasVisitedThirdRestaurant;
			break;
		case MapType::TOWN:
			mapNode = mapNode.child("town");
			hasVisitedLocation = gameProgress.hasVisitedTown;
			break;
		case MapType::DOTONBORI:
			mapNode = mapNode.child("bigCity");
			hasVisitedLocation = gameProgress.hasVisitedBigCity;
			break;
		case MapType::SKYSCRAPER:
			mapNode = mapNode.child("dungeon");
			hasVisitedLocation = gameProgress.hasVisitedSkyScraper;
			break;
		case MapType::SECRET_ROOM:
			mapNode = mapNode.child("secretRoom");
			hasVisitedLocation = gameProgress.hasVisitedSecretRoom;
			break;
		case MapType::SECOND_SECRET_ROOM:
			mapNode = mapNode.child("second_secretRoom");
			hasVisitedLocation = gameProgress.hasVisitedSecondSecretRoom;
			break;
		case MapType::THIRD_SECRET_ROOM:
			mapNode = mapNode.child("third_secretRoom");
			hasVisitedLocation = gameProgress.hasVisitedThirdSecretRoom;
			break;
		case MapType::OSAKA:
			mapNode = mapNode.child("osaka");
			hasVisitedLocation = gameProgress.hasVisitedOsaka;
			break;
		default:
			break;
		}
		// FIRST CHANGE PLAYERS POSITION TO NEW POSITION BASED ON THE PREVIOUS MAP 
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
		case MapType::KANAGAWA:
			previousMapNode = mapNode.child("prevMediumCity");
			break;
		case MapType::RESTAURANT:
			previousMapNode = mapNode.child("prevRestaurant");
			break;
		case MapType::SECOND_RESTAURANT:
			previousMapNode = mapNode.child("prevSecondRestaurant");
			break;
		case MapType::THIRD_RESTAURANT:
			previousMapNode = mapNode.child("prevThirdRestaurant");
			break;
		case MapType::TOWN:
			previousMapNode = mapNode.child("prevTown");
			break;
		case MapType::DOTONBORI:
			previousMapNode = mapNode.child("prevBigCity");
			break;
		case MapType::SKYSCRAPER:
			previousMapNode = mapNode.child("prevSkyscraper");
			break;
		case MapType::SECRET_ROOM:
			previousMapNode = mapNode.child("prevSecretRoom");
			break;
		case MapType::SECOND_SECRET_ROOM:
			previousMapNode = mapNode.child("prevSecondSecretRoom");
			break;
		case MapType::THIRD_SECRET_ROOM:
			previousMapNode = mapNode.child("prevThirdSecretRoom");
			break;
		case MapType::OSAKA:
			previousMapNode = mapNode.child("prevOsaka");
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

		entityManager->DeleteAllEntitiesExceptPlayer();

		// Check if we have visited that location already
		if (hasVisitedLocation == false)
		{
			/*LOAD FROM MAP INFO*/

			// DELETE ALL ENTITIES EXCEPT PLAYER
			/*entityManager->DeleteAllEntitiesExceptPlayer();*/

			// LOAD ENEMIES
			int enemyCount = mapNode.attribute("enemyCount").as_int();

			pugi::xml_node enemyNode = mapNode.child("enemy");
			Enemy* enemy = nullptr;
			for (int i = 0; i < enemyCount; ++i)
			{
				int subtype = enemyNode.attribute("subtype").as_int();
				iPoint position = { enemyNode.attribute("posX").as_int(),enemyNode.attribute("posY").as_int() };
				enemy = (Enemy*)entityManager->CreateEntity(EntityType::ENEMY, enemyNode.attribute("name").as_string(), (EntitySubtype)subtype, position);
				enemy->spritePos = enemyNode.attribute("spritePos").as_int();

				enemy = nullptr;
				enemyNode = enemyNode.next_sibling("enemy");
			}

			// LOAD NPCs
			int npcCount = mapNode.attribute("npcCount").as_int();

			pugi::xml_node npcNode = mapNode.child("npc");
			NPC* npc = nullptr;
			for (int i = 0; i < npcCount; ++i)
			{
				iPoint position = { npcNode.attribute("posX").as_int(), npcNode.attribute("posY").as_int() };
				npc = (NPC*)entityManager->CreateEntity(EntityType::NPC, npcNode.attribute("name").as_string(), EntitySubtype::UNKNOWN, position);

				npc->spritePos = npcNode.attribute("spritePos").as_int();
				npc->SetTexture(npc->spritePos);
				npc->position.x = npcNode.attribute("posX").as_int();
				npc->position.y = npcNode.attribute("posY").as_int();
				npc->stop = npcNode.attribute("stop").as_bool();
				if (npc->stop) npc->stopForever = true;
				npc->dialogIndex = npcNode.attribute("dialogIndex").as_int();
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
				iPoint position = { itemNode.attribute("posX").as_int() * 3,itemNode.attribute("posY").as_int() * 3 };

				item = (Item*)entityManager->CreateEntity(EntityType::ITEM, itemNode.attribute("name").as_string(), (EntitySubtype)subtype, position);

				item->spritePos = 0;

				item = nullptr;
				itemNode = itemNode.next_sibling("item");
			}

			// LOAD ACTIVATORS
			int activatorCount = mapNode.attribute("activatorCount").as_int();

			pugi::xml_node activatorNode = mapNode.child("activator");
			Activator* activator = nullptr;
			for (int i = 0; i < activatorCount; ++i)
			{
				iPoint position = { activatorNode.attribute("posX").as_int(), activatorNode.attribute("posY").as_int() };
				activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, activatorNode.attribute("name").as_string(), (EntitySubtype)activatorNode.attribute("entitySubtype").as_int(), position);
				ActivatorState drawState = (ActivatorState)activatorNode.attribute("drawState").as_int();
				activator->SetDrawState(drawState);
				activator = nullptr;
				activatorNode = activatorNode.next_sibling("activator");
			}

			// LOAD secretWall
			int secretWallCount = mapNode.attribute("secretWallCount").as_int();

			pugi::xml_node secretWallNode = mapNode.child("secretwall");
			SecretWall* secretWall = nullptr;

			for (int i = 0; i < secretWallCount; ++i)
			{
				iPoint position = { secretWallNode.attribute("posX").as_int(), secretWallNode.attribute("posY").as_int() };
				secretWall = (SecretWall*)entityManager->CreateEntity(EntityType::SECRET_WALL, secretWallNode.attribute("name").as_string(), EntitySubtype::UNKNOWN, position);
				secretWallNode = secretWallNode.next_sibling("secretWall");
			}
			// LOAD Levers
			int leverCount = mapNode.attribute("leverCount").as_int();

			pugi::xml_node leverNode = mapNode.child("lever");
			Lever* lever = nullptr;
			for (int i = 0; i < leverCount; ++i)
			{
				iPoint position = { leverNode.attribute("posX").as_int(), leverNode.attribute("posY").as_int() };
				lever = (Lever*)entityManager->CreateEntity(EntityType::LEVER, leverNode.attribute("name").as_string(), EntitySubtype::UNKNOWN, position);
				lever->SetSecretWall(secretWall);
				lever->SetNumber(i + 1);
				lever = nullptr;
				leverNode = leverNode.next_sibling("lever");
			}
			secretWall = nullptr;
			// Change the booleans of progress
			switch (currentMap)
			{
			case MapType::CEMETERY:
				gameProgress.hasVisitedCemetery = true;
				break;
			case MapType::HOUSE:
				gameProgress.hasVisitedHouse = true;
				break;
			case MapType::KANAGAWA:
				gameProgress.hasVisitedMediumCity = true;
				break;
			case MapType::RESTAURANT:
				gameProgress.hasVisitedRestaurant = true;
				break;
			case MapType::SECOND_RESTAURANT:
				gameProgress.hasVisitedSecondRestaurant = true;
				break;
			case MapType::THIRD_RESTAURANT:
				gameProgress.hasVisitedThirdRestaurant = true;
				break;
			case MapType::TOWN:
				gameProgress.hasVisitedTown = true;
				break;
			case MapType::DOTONBORI:
				gameProgress.hasVisitedBigCity = true;
				break;
			case MapType::SKYSCRAPER:
				gameProgress.hasVisitedSkyScraper = true;
				break;
			case MapType::SECRET_ROOM:
				gameProgress.hasVisitedSecretRoom = true;
				break;
			case MapType::SECOND_SECRET_ROOM:
				gameProgress.hasVisitedSecondSecretRoom = true;
				break;
			case MapType::THIRD_SECRET_ROOM:
				gameProgress.hasVisitedThirdSecretRoom = true;
				break;
			case MapType::OSAKA:
				gameProgress.hasVisitedOsaka = true;
				break;
			default:
				break;
			}
		}
		else 
		{
			/*LOAD FROM SAVE GAME*/
			pugi::xml_document docData;
			pugi::xml_node loadNode;

			pugi::xml_parse_result result = docData.load_file("save_game.xml");
			loadNode = docData.first_child().child("scenemanager").child("scenegameplay");

			if (hasStartedFromContinue)
			{
				entityManager->LoadStateInfo(loadNode, currentMap, false);
				hasStartedFromContinue = false;
			}
			else
				entityManager->LoadStateInfo(loadNode, currentMap, true);

			// Old player stats
			ListItem<Player*>* list1;
			int i = 0;
			for (list1 = entityManager->playerList.start; list1 != NULL; list1 = list1->next)
			{
				list1->data->stats = playerStats[i];
				++i;
			}
			RELEASE(list1);
			delete [] playerStats;
			playerStats = nullptr;
		}
		/*LOAD FROM SAVE GAME*/
		pugi::xml_document docSaveFileData;
		pugi::xml_parse_result result = docSaveFileData.load_file("save_game.xml");
		LoadInventory(docSaveFileData.first_child().child("screen").child("screeninventory"));
		
		CreateEntities();
	}
	if (currentMap == MapType::CEMETERY)
	{
		if (gameProgress.hasVisitedErikaTombstone == false && gameProgress.hasVisitedRestaurantQuest == true)
		{
			Activator* activator;
			activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, "erikaTomb", EntitySubtype::UNKNOWN, iPoint(18 * 32, 4 * 32));
			activator->width = 32;
			activator->height = 32;
		}
	}
	if (currentMap == MapType::SECOND_SECRET_ROOM && gameProgress.hasSavedFirstApprentice == false)
	{
		if (entityManager->SearchEntity("reiNPC") != nullptr)
			entityManager->DestroyEntity(entityManager->SearchEntity("reiNPC"));

		NPC* npc;
		npc = (NPC*)entityManager->CreateEntity(EntityType::NPC, "reiNPC", EntitySubtype::UNKNOWN, iPoint((2 * 32), (2 * 32)));
		npc->name = "reiNPC";
		npc->width = 32;
		npc->height = 32;
		npc->spritePos = 0;
		npc->dialogIndex = 18;
		npc->stop = true;
		npc->SetTexture(npc->spritePos);
		if (npc->stop) npc->stopForever = true;
		npc = nullptr;
	}
	if (currentMap == MapType::THIRD_SECRET_ROOM && gameProgress.hasSavedLastApprentice == false)
	{
		if (entityManager->SearchEntity("eikenNPC") != nullptr)
			entityManager->DestroyEntity(entityManager->SearchEntity("eikenNPC"));

		NPC* npc;
		npc = (NPC*)entityManager->CreateEntity(EntityType::NPC, "eikenNPC", EntitySubtype::UNKNOWN, iPoint((2 * 32), (2 * 32)));
		npc->name = "eikenNPC";
		npc->width = 32;
		npc->height = 32;
		npc->spritePos = 12;
		npc->dialogIndex = 19;
		npc->stop = true;
		npc->SetTexture(npc->spritePos);
		if (npc->stop) npc->stopForever = true;
		npc = nullptr;
	}
	if (currentMap == MapType::SECRET_ROOM && gameProgress.hasVisitedRestaurantQuest == false)
	{
		Activator* activator;
		activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, "secretRoomDialog", EntitySubtype::UNKNOWN, iPoint(1 * 32, 5 * 32));
		activator->width = 128;
		activator->height = 32;
		activator = nullptr;
	}
	if (currentMap == MapType::SKYSCRAPER && gameProgress.hasTriggeredDialogDungeon1 == false)
	{
		Activator* activator;
		activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, "dungeon1", EntitySubtype::UNKNOWN, iPoint(6 * 32, 46 * 32));
		activator->width = 160;
		activator->height = 64;
		activator = nullptr;
	}
	if (currentMap == MapType::SKYSCRAPER && gameProgress.hasTriggeredDialogDungeon2 == false)
	{
		Activator* activator;
		activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, "dungeon2", EntitySubtype::UNKNOWN, iPoint(9 * 32, 41 * 32));
		activator->width = 160;
		activator->height = 32;
		activator = nullptr;
	}
	if (currentMap == MapType::SKYSCRAPER && gameProgress.hasTriggeredDialogDungeon3 == false)
	{
		Activator* activator;
		activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, "dungeon3", EntitySubtype::UNKNOWN, iPoint(3 * 32, 2 * 32));
		activator->width = 128;
		activator->height = 32;
		activator = nullptr;
	}
	if (currentMap == MapType::SKYSCRAPER && gameProgress.hasTriggeredDialogDungeon4 == false)
	{
		Activator* activator;
		activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, "dungeon4", EntitySubtype::UNKNOWN, iPoint(36 * 32, 2 * 32));
		activator->width = 128;
		activator->height = 32;
		activator = nullptr;
	}
	if (currentMap == MapType::SKYSCRAPER && gameProgress.hasTriggeredDialogDungeon5 == false)
	{
		Activator* activator;
		activator = (Activator*)entityManager->CreateEntity(EntityType::ACTIVATOR, "dungeon5", EntitySubtype::UNKNOWN, iPoint(38 * 32, 35 * 32));
		activator->width = 224;
		activator->height = 32;
		activator = nullptr;
	}

	PlayMapMusic();
	screenPause->SetMap(currentMap);
}

void SceneGameplay::AddItemToInvItemsList(Item* item)
{
	notifier->GetInstance()->SetItemAddition(false);

	for (ListItem<InvItem*>* invItem = screenInventory->listInvItems.start; invItem; invItem = invItem->next)
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
	screenInventory->listInvItems.Add(invItem);
}

void SceneGameplay::PlayMapMusic()
{
	audio->StopMusic();
	switch (currentMap)
	{
	case MapType::CEMETERY:
		audio->PlayMusic("Audio/Music/cemetery.ogg");
		break;
	case MapType::HOUSE:
		audio->PlayMusic("Audio/Music/house.ogg");
		break;
	case MapType::KANAGAWA:
		audio->PlayMusic("Audio/Music/city_background.ogg");
		break;
	case MapType::RESTAURANT:
		audio->PlayMusic("Audio/Music/restaurant.ogg");
		break;
	case MapType::SECOND_RESTAURANT:
		audio->PlayMusic("Audio/Music/restaurant.ogg");
		break;
	case MapType::THIRD_RESTAURANT:
		audio->PlayMusic("Audio/Music/restaurant.ogg");
		break;
	case MapType::TOWN:
		audio->PlayMusic("Audio/Music/city_background.ogg");
		break;
	case MapType::DOTONBORI:
		audio->PlayMusic("Audio/Music/city_background.ogg");
		break;
	case MapType::SKYSCRAPER:
		audio->PlayMusic("Audio/Music/cemetry.ogg");
		break;
	case MapType::SECRET_ROOM:
		audio->PlayMusic("Audio/Music/restaurant.ogg");
		break;
	case MapType::SECOND_SECRET_ROOM:
		audio->PlayMusic("Audio/Music/restaurant.ogg");
		break;
	case MapType::THIRD_SECRET_ROOM:
		audio->PlayMusic("Audio/Music/restaurant.ogg");
		break;
	case MapType::OSAKA:
		audio->PlayMusic("Audio/Music/city_background.ogg");
		break;
	default:
		break;
	}
}

GameProgress* SceneGameplay::GetGameProgress()
{
	return &gameProgress;
}

void SceneGameplay::RewardXP(int xp)
{
	gameProgress.xp += xp;

	pugi::xml_document docData;
	pugi::xml_parse_result result = docData.load_file("save_game.xml");
	SaveGameProgress(docData.first_child().child("scenemanager").child("scenegameplay").child("gameProgress"));
	
	docData.save_file("save_game.xml");
}

void SceneGameplay::RewardGold(int gold)
{
	gameProgress.gold += gold;

	pugi::xml_document docData;
	pugi::xml_parse_result result = docData.load_file("save_game.xml");
	SaveGameProgress(docData.first_child().child("scenemanager").child("scenegameplay").child("gameProgress"));

	docData.save_file("save_game.xml");
}

void SceneGameplay::SetMapTransitionState()
{
	readyToChangeMap = true;
}

// Create entities from the map (For now ww are only creating static entities)
void SceneGameplay::CreateEntities()
{
	//Iterate all objects of the map made with Tiled to find entities
	for (std::list<ColliderObject*>::iterator position = map->data.colliders.begin(); position != map->data.colliders.end(); position++)
	{
		iPoint coll = { (*position)->collX, (*position)->collY };

		/*if ((*position)->name == "player")
		{
			app->entities->CreateEntity(Entity::Types::PLAYER, (*position)->collX, (*position)->collY, (*position)->name);
		}*/
		if ((*position)->entType == "static")
		{
			entityManager->CreateEntity(EntityType::STATIC, (*position)->name.c_str(), EntitySubtype::UNKNOWN, coll);
		}
		/*else if ((*position)->entType == "NPC")
		{
			app->entities->CreateEntity(Entity::Types::NPC, (*position)->collX, (*position)->collY, (*position)->name);
		}*/
		else
		{
			LOG("There isn't any entity with name %s and type %s", (*position)->name.data(), (*position)->entType.data());
		}
	}
}
