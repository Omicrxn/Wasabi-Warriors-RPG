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

	spritesheet = nullptr;

	camera = { 0,0,1280,720 };

	map = nullptr;
	currentPlayer = nullptr;

	battleSystem = new BattleSystem();
	// Battle system bool
	battle = false;

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

	map = (Map*)entityManager->CreateEntity(EntityType::MAP, "Map");

	// Load texture
	spritesheet = tex->Load("Assets/Textures/Characters/characters_spritesheet.png");
	titlesTex = tex->Load("Assets/Textures/Scenes/titles.png");
	entityManager->texture = spritesheet;

	// Load battle system textures
	backgroundTex = tex->Load("Assets/Textures/Scenes/battle_scene.jpg");
	guiAtlasTex = tex->Load("Assets/Textures/UI/ui_spritesheet.png");

	// Create fonts
	titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
	buttonFont = new Font("Assets/Fonts/showg.xml", tex);

	// Load buttons Fx
	hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
	clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");
	returnFx = audio->LoadFx("Assets/Audio/Fx/back.ogg");

	// Gui id goes from 0 to 2
	screenRoaming = new ScreenRoaming();
	screenRoaming->Load(0, 2, this, win, guiManager, easing, guiAtlasTex, buttonFont, hoverFx, clickFx);
	screenRoaming->isActive = true;
	screenRoaming->ShowButtons();

	// Gui id goes from 3 to 5
	screenPause = new ScreenPause();
	screenPause->Load(3, 5, this, win, guiManager, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);
	screenPause->isActive = false;
	screenPause->HideButtons();

	// Gui id goes from 6 to 10
	screenSettings = new ScreenSettings();
	screenSettings->Load(6, 10, this, win, guiManager, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);
	screenSettings->isActive = false;
	screenSettings->HideButtons();
	
	// Gui id goes from 11 to 14
	// Load buttons for the battle system
	btnAttack = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, { 730, 500, 190, 49 }, "ATTACK");
	btnAttack->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnAttack->state = GuiControlState::HIDDEN;

	btnDefend = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, { 730, 600, 190, 49 }, "DEFEND");
	btnDefend->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnDefend->state = GuiControlState::HIDDEN;

	btnItem = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, { 1030, 500, 190, 49 }, "ITEM");
	btnItem->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnItem->state = GuiControlState::HIDDEN;

	btnRun = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, { 1030, 600, 190, 49 }, "RUN");
	btnRun->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnRun->state = GuiControlState::HIDDEN;

	// Create party member 1
	Player* player;
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "DaBaby");
	player->position = iPoint(19 * 32, 1 * 32);
	player->SetTexture(spritesheet, 3);
	player->SetState(true);
	player->SetUpClass("hunter");
	player = nullptr;
	currentPlayer = entityManager->playerList.At(0)->data;

	// Create party member 2
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, "DaCrack");
	player->position = iPoint(19 * 32, 1 * 32);
	player->SetTexture(spritesheet, 6);
	player->SetUpClass("wizard");
	player = nullptr;
	RELEASE(player);

	if (hasStartedFromContinue)
	{
		// LOAD FROM THE SAVE FILE
		app->LoadGameRequest();
	}
	else
	{
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

	if (notifier->OnMapChange() && notifier->GetNextMap() != currentMap)
	{
		SetUpTp();
	}

	if ((input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_Y) == KeyState::KEY_DOWN) && battle == false)
	{
		if (entityManager->playerList.At(entityManager->playerList.Find(currentPlayer))->next != nullptr)
		{
			currentPlayer->SetState(false);
			currentPlayer = entityManager->playerList.At(entityManager->playerList.Find(currentPlayer))->next->data;
			currentPlayer->SetState(true);
		}
		else 
		{
			currentPlayer->SetState(false);
			currentPlayer = entityManager->playerList.At(0)->data;
			currentPlayer->SetState(true);
		}
	}

	if (input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && battle == false)
	{
		battle = true; 
		battleSystem->SetupBattle(&entityManager->playerList, (Enemy*)entityManager->SearchEntity(notifier->GetEnemy()));

		EnableBattleButtons();

		currentState = GameState::BATTLE;

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
		battle = true;
		battleSystem->SetupBattle(&entityManager->playerList, (Enemy*)entityManager->SearchEntity(notifier->GetEnemy()));

		EnableBattleButtons();

		notifier->NotifyBattle();

		currentState = GameState::BATTLE;

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

	if (battle == true)
	{
		if (input->GetControllerState())
		{
			//11 = 6
			// GAMEPAD INPUT
			if (focusedButtonId == 11)
			{
				if (input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN)
					focusedButtonId = 12;
				else if (input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN)
					focusedButtonId = 13;
			}
			else if (focusedButtonId == 12)
			{
				if (input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN)
					focusedButtonId = 11;
				else if (input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN)
					focusedButtonId = 14;
			}
			else if (focusedButtonId == 13)
			{
				if (input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN)
					focusedButtonId = 14;
				else if (input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN)
					focusedButtonId = 11;
			}
			else if (focusedButtonId == 14)
			{
				if (input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN)
					focusedButtonId = 13;
				else if (input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN)
					focusedButtonId = 12;
			}

			bool isHovering = false;
			for (int i = 11; i <= 14; ++i)
			{
				if (guiManager->controls.At(i)->data->mouseFocus)
					isHovering = true;
			}
			for (int i = 11; i <= 14; ++i)
			{
				if (i != focusedButtonId || isHovering)
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

		if (battleSystem->battleState == BattleState::WON)
		{
			if (battleSystem->currentMusic == BattleMusic::WON)
			{
				battleSystem->currentMusic = BattleMusic::NONE;
				audio->StopMusic();
				audio->PlayMusic("Assets/Audio/Music/victory.ogg");
			}
		}
		else if (battleSystem->battleState == BattleState::LOST)
		{
			if (battleSystem->currentMusic == BattleMusic::LOST)
			{
				battleSystem->currentMusic = BattleMusic::NONE;
				audio->StopMusic();
				audio->PlayMusic("Assets/Audio/Music/defeat.ogg");
			}
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
	default:
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
					render->DrawTexture(guiAtlasTex, 175 + 100 * i, 350, &rect, 0, 90.0);
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
	}

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

	// Remove map
	map->CleanUp();

	// Stop music (GitHub)

	// Release fonts
	RELEASE(titleFont);
	titleFont = nullptr;
	RELEASE(buttonFont);
	buttonFont = nullptr;

	// Unload textures
	tex->UnLoad(spritesheet);
	spritesheet = nullptr;
	tex->UnLoad(backgroundTex);
	backgroundTex = nullptr;
	tex->UnLoad(guiAtlasTex);
	guiAtlasTex = nullptr;
	tex->UnLoad(titlesTex);
	titlesTex = nullptr;

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

	// Delete screens
	RELEASE(screenRoaming);
	RELEASE(screenPause);
	RELEASE(screenSettings);

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
			
			pauseTimer.Start();
		}
		else if (control->id == 1) {} //currentState = GameState::INVENTORY;
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
	}

	for (int i = 11; i <= 14; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
	}

	battle = false;
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
	MapType previousMap = MapType::NONE;

	previousMap = currentMap;

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
	default:
		break;
	}

	if (hasStartedFromContinue == false)
	{
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
				previousMapNode = mapNode.child("prevMediumcity");
				break;
			case MapType::RESTAURANT:
				previousMapNode = mapNode.child("prevRestaurant");
				break;
			case MapType::TOWN:
				previousMapNode = mapNode.child("prevTown");
				break;
			default:
				break;
			}

			// DELETE ALL ENTITIES EXCEPT PLAYER
			entityManager->DeleteAllEntitiesExceptPlayer();

			
			if (previousMap != MapType::NONE)
			{
				int newPosX = 0; int newPosY = 0;
				newPosX = previousMapNode.child("player").attribute("posX").as_int();
				newPosY = previousMapNode.child("player").attribute("posY").as_int();

				//	// Iterate all players and change position
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

			// LOAD ENEMIES
			int enemyCount = mapNode.attribute("enemyCount").as_int();

			pugi::xml_node enemyNode = mapNode.child("enemy");
			Enemy* enemy = nullptr;
			for (int i = 0; i < enemyCount; ++i)
			{
				enemy = (Enemy*)entityManager->CreateEntity(EntityType::ENEMY, enemyNode.attribute("name").as_string());

				enemy->id = enemyNode.attribute("id").as_uint();
				enemy->spritePos = enemyNode.attribute("spritePos").as_int();
				enemy->position.x = enemyNode.attribute("posX").as_int();
				enemy->position.y = enemyNode.attribute("posY").as_int();

				enemy->classType = enemyNode.attribute("class").as_string();
				enemy->SetUpClass(enemy->classType.GetString());

				enemy->SetUpTexture();
				enemy = nullptr;
				enemyNode = enemyNode.next_sibling("enemy");
			}

			// LOAD NPCs
			int npcCount = mapNode.attribute("npcCount").as_int();

			pugi::xml_node npcNode = mapNode.child("npc");
			NPC* npc = nullptr;
			for (int i = 0; i < npcCount; ++i)
			{
				npc = (NPC*)entityManager->CreateEntity(EntityType::NPC, npcNode.attribute("name").as_string());

				npc->id = npcNode.attribute("id").as_uint();
				npc->spritePos = npcNode.attribute("spritePos").as_int();
				npc->position.x = npcNode.attribute("posX").as_int();
				npc->position.y = npcNode.attribute("posY").as_int();

				npc->SetUpTexture();
				npc = nullptr;
				npcNode = npcNode.next_sibling("npc");
			}

			// LOAD TELEPORTS
			int teleportCount = mapNode.attribute("teleportCount").as_int();

			pugi::xml_node teleportNode = mapNode.child("teleport");
			Teleport* teleport = nullptr;
			for (int i = 0; i < teleportCount; ++i)
			{
				teleport = (Teleport*)entityManager->CreateEntity(EntityType::TELEPORT, teleportNode.attribute("name").as_string());

				teleport->id = teleportNode.attribute("id").as_uint();
				teleport->spritePos = teleportNode.attribute("spritePos").as_int();
				teleport->position.x = teleportNode.attribute("posX").as_int();
				teleport->position.y = teleportNode.attribute("posY").as_int();

				/*newTeleportNode.append_attribute("nameTeleport").set_value(list4->data->GetName().GetString());
				newTeleportNode.append_attribute("name").set_value(list4->data->name.GetString());
				newTeleportNode.append_attribute("destination").set_value((int)list4->data->GetDestination());*/

				//teleport->SetName((SString)teleportNode.attribute("nameTeleport").as_string());
				teleport->name = teleportNode.attribute("name").as_string();
				teleport->SetUpDestination((MapType)teleportNode.attribute("destination").as_int());
				teleport = nullptr;
				teleportNode = teleportNode.next_sibling("teleport");
			}
		}
			
	}
	else
	{
		hasStartedFromContinue = false;
	}
		
}

void SceneGameplay::EnableBattleButtons()
{
	for (int i = 11; i <= 14; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
	}

	for (int i = 0; i <= 10; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
	}
}