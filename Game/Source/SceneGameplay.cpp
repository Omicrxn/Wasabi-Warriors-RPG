#include "SceneGameplay.h"

#include "EntityManager.h"
#include "BattleSystem.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "Window.h"
#include "DialogSystem.h"

#include "Notifier.h"
#include "Easing.h"


SceneGameplay::SceneGameplay()
{
	type = SceneType::GAMEPLAY;
	this->name = "scenegameplay";

	// Needed modules
	entityManager = nullptr;
	guiManager = nullptr;
	win = nullptr;
	dialogSystem = nullptr;

	spritesheet = nullptr;

	camera = { 0,0,1280,720 };

	map = nullptr;
	currentPlayer = nullptr;

	battleSystem = new BattleSystem();
	// Battle system bool
	battle = false;

	currentState = ROAMING;

	// Battle system textures
	backgroundTex = nullptr;
	backgroundRect = { 0, 0, 0, 0 };
	guiAtlasTex = nullptr;
	pauseBackgroundRect = { 1544, 217, 113, 295 };

	// Rects for titles
	settingsTitleRect = { 0, 149, 530, 81 };
	pauseTitleRect = { 0, 73, 440, 75 };

	// Fonts
	titleFont = nullptr;
	buttonFont = nullptr;

	// Audio Fx for buttons
	hoverFx = -1;
	clickFx = -1;

	// Buttons
	btnAttack = nullptr;
	btnDefend = nullptr;
	btnItem = nullptr;
	btnRun = nullptr;

	btnPause = nullptr;
	btnInventory = nullptr;
	btnPhone = nullptr;

	btnReturn = nullptr;
	btnSettings = nullptr;
	btnExit = nullptr;
	//btnNone = nullptr;

	// Gamepad's menu focused button
	focusedButtonId = 0;

	notifier = nullptr;
	currentMap = MapType::CEMETERY;
}

SceneGameplay::~SceneGameplay()
{
}

bool SceneGameplay::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem, Easing* easing)
{
	notifier = Notifier::GetInstance();

	// Needed modules
	this->entityManager = entityManager;
	this->guiManager = guiManager;
	this->win = win;
	this->dialogSystem = dialogSystem;

	map = (Map*)entityManager->CreateEntity(EntityType::MAP, "Map");

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
			}
			break;
		case MapType::HOUSE:
			if (map->Load("House", "house.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			break;
		case MapType::MEDIUM_CITY:
			if (map->Load("MediumCity", "mediumcity.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			break;
		case MapType::RESTAURANT:
			if (map->Load("Restaurant", "restaurant.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			break;
		case MapType::TOWN:
			if (map->Load("Town", "townMap.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			break;
		default:
			break;
		}

	

	// Load texture
	spritesheet = tex->Load("Assets/Textures/Characters/characters_spritesheet.png");
	titlesTex = tex->Load("Assets/Textures/Scenes/titles.png");

	// Create party member 1
	Entity* entity;
	entity = entityManager->CreateEntity(EntityType::PLAYER, "DaBaby");
	entity->position = iPoint(12 * 32, 6 * 32);
	entity->SetTexture(spritesheet, 3);
	entity->SetState(true);
	entity = nullptr;
	currentPlayer = entityManager->playerList.At(0)->data;
	// Create party member 2
	entity = entityManager->CreateEntity(EntityType::PLAYER, "DaCrack");
	entity->position = iPoint(12 * 32, 6 * 32);
	entity->SetTexture(spritesheet, 6);
	entity = nullptr;
	// Create enemy
	entity = entityManager->CreateEntity(EntityType::ENEMY, "DaBoss");
	entity->position = iPoint(10 * 32, 6 * 32);
	entity->SetTexture(spritesheet, 5);
	entity = nullptr;
	// Create NPC
	entity = entityManager->CreateEntity(EntityType::NPC, "DaBot");
	entity->position = iPoint(8 * 32, 8 * 32);
	entity->SetTexture(spritesheet, 4);
	entity = nullptr;
	// Create NPC 2
	entity = entityManager->CreateEntity(EntityType::NPC, "DaBot2");
	entity->position = iPoint(10 * 32, 6 * 32);
	entity->SetTexture(spritesheet, 8);
	entity = nullptr;
	// Create NPC 3
	entity = entityManager->CreateEntity(EntityType::NPC, "DaBot3");
	entity->position = iPoint(15 * 32, 7 * 32);
	entity->SetTexture(spritesheet, 7);
	entity = nullptr;

	// Create Teleport
	entity = entityManager->CreateEntity(EntityType::TELEPORT, "DaTransfer");
	entity->position = iPoint(12 * 32, 10 * 32);
	entityManager->teleportList.At(entityManager->teleportList.Find((Teleport*)entity))->data->SetUpDestination(MapType::TOWN);
	entity = nullptr;

	// Load battle system textures
	backgroundTex = tex->Load("Assets/Textures/Scenes/battle_scene.jpg");
	backgroundRect = { 0, 0, 1280, 720 };
	guiAtlasTex = tex->Load("Assets/Textures/UI/Elements/ui_spritesheet.png");

	// Create fonts
	titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
	buttonFont = new Font("Assets/Fonts/showg.xml", tex);

	// Load buttons Fx
	hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
	clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

	// Get window info
	/*uint width, height;
	win->GetWindowSize(width, height);*/

	// Load buttons for HUD
	btnPause = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, { 50, 50, 70, 55 }, "");
	btnPause->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::ICON_PAUSE);

	btnInventory = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 170, 50, 70, 55 }, "");
	btnInventory->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::ICON_INVENTORY);

	btnPhone = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { 290, 50, 70, 55 }, "");
	btnPhone->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::ICON_PHONE);
	
	// Load buttons for pause
	btnReturn = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { 610, 280, 70, 55 }, "");
	btnReturn->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::ICON_RETURN);
	btnReturn->state = GuiControlState::HIDDEN;

	btnSettings = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { 610, 350, 70, 55 }, "");
	btnSettings->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::ICON_SETTINGS);
	btnSettings->state = GuiControlState::HIDDEN;

	btnExit = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { 615, 430, 70, 55 }, "");
	btnExit->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::ICON_EXIT);
	btnExit->state = GuiControlState::HIDDEN;

	// Load buttons for the battle system
	btnAttack = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, { 730, 500, 190, 49 }, "ATTACK");
	btnAttack->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnAttack->state = GuiControlState::HIDDEN;

	btnDefend = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, { 730, 600, 190, 49 }, "DEFEND");
	btnDefend->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnDefend->state = GuiControlState::HIDDEN;

	btnItem = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, { 1030, 500, 190, 49 }, "ITEM");
	btnItem->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnItem->state = GuiControlState::HIDDEN;

	btnRun = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, { 1030, 600, 190, 49 }, "RUN");
	btnRun->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnRun->state = GuiControlState::HIDDEN;

	//btnNone = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { 700, 500, 190, 49 }, "NONE");
	//btnNone->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);
	//btnNone->state = GuiControlState::HIDDEN;

	audio->PlayMusic("Assets/Audio/Music/menu.ogg");

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

	if (notifier->OnMapChange() && notifier->GetNextMap() != currentMap)
	{
		map->CleanUp();

		currentMap = notifier->ChangeMap();
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
			}
			break;
		case MapType::HOUSE:
			if (map->Load("House", "house.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			break;
		case MapType::MEDIUM_CITY:
			if (map->Load("MediumCity", "mediumcity.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			break;
		case MapType::RESTAURANT:
			if (map->Load("Restaurant", "restaurant.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			break;
		case MapType::TOWN:
			if (map->Load("Town", "townMap.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			break;
		default:
			break;
		}
	}

	if (input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && battle == false)
	{
		if (entityManager->playerList.At(entityManager->playerList.Find(currentPlayer))->next != nullptr)
		{
			currentPlayer->SetState(false);
			currentPlayer = entityManager->playerList.At(entityManager->playerList.Find(currentPlayer))->next->data;
			currentPlayer->SetState(true);
		}
		else {
			currentPlayer->SetState(false);
			currentPlayer = entityManager->playerList.At(0)->data;
			currentPlayer->SetState(true);
		}
	}

	if (input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && battle == false)
	{
		battle = true;
		battleSystem->SetupBattle(&entityManager->playerList, entityManager->enemyList.start->data);

		EnableBattleButtons();
		
		currentState = GameState::BATTLE;

		focusedButtonId = 6;
	}

	if (notifier->GetBattle())
	{
		battle = true;
		battleSystem->SetupBattle(&entityManager->playerList, entityManager->enemyList.start->data);

		EnableBattleButtons();

		notifier->NotifyBattle();

		currentState = GameState::BATTLE;

		focusedButtonId = 6;
	}

	if (input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		dialogSystem->NewDialog();
	}

	if (battle == true)
	{
		if (input->GetControllerState())
		{
			/* Input */
			if ((input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN) && focusedButtonId >= 7)
				--focusedButtonId;
			else if ((input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN) && focusedButtonId <= 8)
				++focusedButtonId;

			for (int i = 6; i < 10; ++i)
			{
				if (i != focusedButtonId)
				{
					// SET GAMEPAD FOCUS TO FALSE
					guiManager->controls.At(i)->data->gamepadFocus = false;
				}
				else
				{
					// SET GAMEPAD FOCUS TO TRUE
					guiManager->controls.At(i)->data->gamepadFocus = true;
				}
			}
		}

		battleSystem->Update(input, dt);
	}

	switch (currentState)
	{
	case GameState::ROAMING:
		UpdateHud(input);
		break;
	case GameState::PAUSE:
		UpdatePause(input);
		break;
	}

	return true;
}

bool SceneGameplay::Draw(Render* render)
{
	if (battle == true)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 }, { 255, 255, 255, (unsigned char)255.0f }, true, false);

		render->DrawTexture(backgroundTex, 0, 0, &backgroundRect, 0);

		uint width, height;
		win->GetWindowSize(width, height);

		if (battleSystem->battleState == BattleState::PLAYER_TURN)
		{
			render->DrawText(titleFont, "Your turn", 50 + 3, 30 + 3, 100, 0, { 105, 105, 105, 255 });
			render->DrawText(titleFont, "Your turn", 50, 30, 100, 0, { 255, 255, 255, 255 });

			if (battleSystem->playerState == PlayerState::ATTACK)
			{
				render->DrawText(titleFont, "You're attacking!", 50 + 3, 130 + 3, 50, 0, { 105, 105, 105, 255 });
				render->DrawText(titleFont, "You're attacking!", 50, 130, 50, 0, { 255, 255, 255, 255 });
			}
			else if (battleSystem->playerState == PlayerState::DEFEND)
			{
				render->DrawText(titleFont, "You're defending!", 50 + 3, 130 + 3, 50, 0, { 105, 105, 105, 255 });
				render->DrawText(titleFont, "You're defending!", 50, 130, 50, 0, { 255, 255, 255, 255 });
			}
		}
		else if (battleSystem->battleState == BattleState::ENEMY_TURN)
		{
			render->DrawText(titleFont, "Enemy turn", 50 + 3, 30 + 3, 100, 0, { 105, 105, 105, 255 });
			render->DrawText(titleFont, "Enemy turn", 50, 30, 100, 0, { 255, 255, 255, 255 });

			if (battleSystem->enemyState == EnemyState::ATTACK)
			{
				render->DrawText(titleFont, "Enemy is attacking!", 50 + 3, 130 + 3, 50, 0, { 105, 105, 105, 255 });
				render->DrawText(titleFont, "Enemy is attacking!", 50, 130, 50, 0, { 255, 255, 255, 255 });
			}
			else if (battleSystem->enemyState == EnemyState::DEFEND)
			{
				render->DrawText(titleFont, "Enemy is defending!", 50 + 3, 130 + 3, 50, 0, { 105, 105, 105, 255 });
				render->DrawText(titleFont, "Enemy is defending!", 50, 130, 50, 0, { 255, 255, 255, 255 });
			}
		}

		if (battleSystem->battleState != BattleState::WON &&
			battleSystem->battleState != BattleState::LOST &&
			battleSystem->battleState != BattleState::EXIT)
		{
			// Player name
			render->DrawText(buttonFont, battleSystem->GetPlayer()->name.GetString(), 125 + 3, 275 + 3, 40, 0, { 105, 105, 105, 255 });
			render->DrawText(buttonFont, battleSystem->GetPlayer()->name.GetString(), 125, 275, 40, 0, { 255, 255, 255, 255 });

			// Player life
			char HP[8] = { 0 };
			sprintf_s(HP, 8, "HP: %03i", battleSystem->GetPlayer()->stats.currentHP);
			render->DrawText(buttonFont, HP, 325 + 3, 275 + 3, 40, 0, { 105, 105, 105, 255 });
			render->DrawText(buttonFont, HP, 325, 275, 40, 0, { 255, 255, 255, 255 });

			// Enemy name
			render->DrawText(buttonFont, battleSystem->GetEnemy()->name.GetString(), 750 + 3, 75 + 3, 40, 0, { 105, 105, 105, 255 });
			render->DrawText(buttonFont, battleSystem->GetEnemy()->name.GetString(), 750, 75, 40, 0, { 255, 255, 255, 255 });

			// Enemy life
			sprintf_s(HP, 8, "HP: %03i", battleSystem->GetEnemy()->stats.currentHP);
			render->DrawText(buttonFont, HP, 950 + 3, 75 + 3, 40, 0, { 105, 105, 105, 255 });
			render->DrawText(buttonFont, HP, 950, 75, 40, 0, { 255, 255, 255, 255 });

			// Draw party members textures
			SDL_Rect rect = { 0,481,32,32 };
			render->scale = 5;
			for (int i = 0; i < battleSystem->GetPlayersList()->Count(); i++)
			{
				rect = battleSystem->GetPlayersList()->At(i)->data->idleAnim.GetFrame(0);
				render->DrawTexture(spritesheet, 22.5 + 20 * i, 75, &rect, 0);
			}
			render->scale = 1;

			// Draw Enemy textures
			rect = battleSystem->GetEnemy()->animRec;
			render->scale = 5;
			render->DrawTexture(spritesheet, 170, 25, &rect, 0);
			render->scale = 1;

			rect = { 171,486,22,21 };
			for (int i = 0; i < battleSystem->GetPlayersList()->Count(); i++)
			{
				if (battleSystem->GetPlayer()->name == battleSystem->GetPlayersList()->At(i)->data->name)
				{
					render->DrawTexture(guiAtlasTex, 175 + 100*i, 350, &rect, 0, 90.0);
				}
			}
		}
		else if (battleSystem->battleState == BattleState::WON)
		{
			// Display winner text
			render->DrawText(titleFont, "You win!", 50 + 3, 30 + 3, 125, 0, { 105, 105, 105, 255 });
			render->DrawText(titleFont, "You win!", 50, 30, 125, 0, { 255, 255, 255, 255 });
		}
		else if (battleSystem->battleState == BattleState::LOST)
		{
			// Display loser text
			render->DrawText(titleFont, "You lose!", 50 + 3, 30 + 3, 125, 0, { 105, 105, 105, 255 });
			render->DrawText(titleFont, "You lose!", 50, 30, 125, 0, { 255, 255, 255, 255 });
		}
		else if (battleSystem->battleState == BattleState::EXIT)
		{
			// Get out of the battle screen and return to the gameplay screen
			ExitBattle();
		}
	}

	switch (currentState)
	{
	case GameState::ROAMING:
		DrawHud(render);
		break;
	case GameState::PAUSE:
		DrawPause(render);
		break;
	}

    return true;
}

bool SceneGameplay::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	// TODO: Unload all resources
	
	// Removing used modules
	this->entityManager = nullptr;
	this->guiManager = nullptr;
	this->win = nullptr;
	RELEASE(battleSystem);

	// Remove map
	map->CleanUp();

	// Stop music (GitHub)

	// Release fonts
	RELEASE(titleFont);
	RELEASE(buttonFont);

	// Unload textures
	tex->UnLoad(spritesheet);
	tex->UnLoad(backgroundTex);
	tex->UnLoad(guiAtlasTex);
	tex->UnLoad(spritesheet);
	tex->UnLoad(titlesTex);

	// Unload Fx
	/*audio.Unload(hoverFx);
	audio.Unload(clickFx);*/

	// Destory GUI Controls
	guiManager->DestroyGuiControl(btnAttack);
	guiManager->DestroyGuiControl(btnDefend);
	guiManager->DestroyGuiControl(btnItem);
	guiManager->DestroyGuiControl(btnRun);
	guiManager->DestroyGuiControl(btnNone);

	guiManager->DestroyGuiControl(btnPause);
	guiManager->DestroyGuiControl(btnInventory);
	guiManager->DestroyGuiControl(btnPhone);

	guiManager->DestroyGuiControl(btnReturn);
	guiManager->DestroyGuiControl(btnSettings);
	guiManager->DestroyGuiControl(btnExit);

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

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//if (control->id == 1) battleSystem->playerState = PlayerState::ATTACK;
		//else if (control->id == 2) battleSystem->playerState = PlayerState::DEFEND;
		//else if (control->id == 3) battleSystem->playerState = PlayerState::ITEM;
		//else if (control->id == 4)
		//{
		//	battleSystem->playerState = PlayerState::RUN;
		//	ExitBattle();
		//}
		//else if (control->id == 5)
		//{
		//	currentState = GameState::PAUSE;
		//	ToggleHUDButtons();
		//	TogglePauseButtons();
		//}
		//else if (control->id == 6) currentState = GameState::INVENTORY;
		//else if (control->id == 7) currentState = GameState::MAP;
		//else if (control->id == 8)
		//{
		//	currentState = GameState::ROAMING;
		//	TogglePauseButtons();
		//	ToggleHUDButtons();
		//}
		//else if (control->id == 9) currentState = GameState::SETTINGS;
		//else if (control->id == 10) {} // RETURN TO MAIN MENU
		//break;
		if (control->id == 0)
		{
			currentState = GameState::PAUSE;
			ToggleHUDButtons();
			TogglePauseButtons();
		}
		else if (control->id == 1) currentState = GameState::INVENTORY;
		else if (control->id == 2) currentState = GameState::MAP;
		else if (control->id == 3)
		{
			currentState = GameState::ROAMING;
			TogglePauseButtons();
			ToggleHUDButtons();
		}
		else if (control->id == 4) currentState = GameState::SETTINGS;
		else if (control->id == 5) {} // RETURN TO MAIN MENU
		else if (control->id == 6) battleSystem->playerState = PlayerState::ATTACK;
		else if (control->id == 7) battleSystem->playerState = PlayerState::DEFEND;
		else if (control->id == 8) battleSystem->playerState = PlayerState::ITEM;
		else if (control->id == 9)
		{
			battleSystem->playerState = PlayerState::RUN;
			ExitBattle();
		}
		break;
	}
	default: break;
	}

	return true;
}

void SceneGameplay::ExitBattle()
{
	for (int i = 6; i < 10; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
	}

	battle = false;
	battleSystem->ResetBattle();

	// Change game state to roaming
	currentState = GameState::ROAMING;
	ToggleHUDButtons();
}

void SceneGameplay::EnableBattleButtons()
{
	for (int i = 6; i < 10; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
	}

	for (int i = 0; i < 6; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
	}
}

void SceneGameplay::ToggleHUDButtons()
{
	for (int i = 0; i < 3; ++i)
	{
		if (currentState == GameState::ROAMING)
		{
			guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
		}
		else
		{
			guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
		}
	}
}

void SceneGameplay::TogglePauseButtons()
{
	for (int i = 3; i < 6; ++i)
	{
		if (currentState == GameState::PAUSE)
		{
			guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
		}
		else
		{
			guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
		}
	}
}

void SceneGameplay::UpdateHud(Input* input)
{
	// Upadate anything extra in the hud like the party member change
	if (input->GetControllerState())
	{
		/*
		if (input->GetControllerButton(CONTROLLER_BUTTON_START) == KEY_DOWN)
			btnPause->gamepadFocus = true;
		else if (input->GetControllerButton(CONTROLLER_BUTTON_X) == KEY_DOWN)
			btnInventory->gamepadFocus = true;
		else if (input->GetControllerButton(CONTROLLER_BUTTON_B) == KEY_DOWN)
			btnPhone->gamepadFocus = true;
		*/
	}
}

void SceneGameplay::UpdatePause(Input* input)
{
	// Lower volume in the pause menu
	if (input->GetControllerState())
	{
		if ((input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN) && focusedButtonId > 3)
			--focusedButtonId;
		else if ((input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN) && focusedButtonId < 5)
			++focusedButtonId;

		for (int i = 3; i <= 5; ++i)
		{
			if (i != focusedButtonId)
			{
				// SET GAMEPAD FOCUS TO FALSE
				guiManager->controls.At(i)->data->gamepadFocus = false;
			}
			else
			{
				// SET GAMEPAD FOCUS TO TRUE
				guiManager->controls.At(i)->data->gamepadFocus = true;
			}
		}
	}
}

void SceneGameplay::DrawHud(Render* render)
{
	// Draw anything extra needed in the hud
}

void SceneGameplay::DrawPause(Render* render)
{
	// Draw pause background & title
	render->DrawRectangle({ 0,0,1280,720 }, { 0, 0, 0, 100 }, true, false);
	render->DrawTexture(guiAtlasTex, 1280 / 2 - pauseBackgroundRect.w/2, 720 / 2 - pauseBackgroundRect.h / 2, &pauseBackgroundRect, 0.0f);
	render->DrawTexture(titlesTex, 1280 / 2 - pauseTitleRect.w / 2, 720 / 2 - pauseTitleRect.h * 1.5f - pauseTitleRect.h /2 - 10,  &pauseTitleRect, 0.0f);
}
