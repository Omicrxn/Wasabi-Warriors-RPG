#include "ScreenBattle.h"
#include "BattleSystem.h"
#include "SceneGameplay.h"

#include "EntityManager.h"
#include "GuiManager.h"
#include "Window.h"
#include "Textures.h"

#include "Player.h"
#include "Enemy.h"

#include "GuiButton.h"
#include "GuiIcon.h"
#include "App.h"
#include "Log.h"

ScreenBattle::ScreenBattle()
{
	// Needed modules
	entityManager = nullptr;
	guiManager = nullptr;
	win = nullptr;
	audio = nullptr;

	charactersSpritesheet = nullptr;

	// Battle system instance
	battleSystem = nullptr;

	// Battle system textures
	backgroundTex = nullptr;
	guiAtlasTex = nullptr;

	backgroundRect = { 0, 0, 1280, 720 };

	// Fonts
	titleFont = nullptr;
	buttonFont = nullptr;
	menuFont = nullptr;

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
}

ScreenBattle::~ScreenBattle()
{
}

bool ScreenBattle::Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, SDL_Texture* charactersSpritesheet, SDL_Texture* guiAtlasTex, Font* titleFont, Font* buttonFont, Font* menuFont, int hoverFx, int clickFx, int returnFx)
{
	this->minIndex = minIndex;
	this->maxIndex = maxIndex;

	sceneGameplay = (SceneGameplay*)currentScene;
	this->battleSystem = battleSystem;

	// Needed modules
	this->entityManager = entityManager;
	this->guiManager = guiManager;
	this->win = win;
	this->audio = audio;

	// Load battle system textures
	this->charactersSpritesheet = charactersSpritesheet;
	this->guiAtlasTex = guiAtlasTex;
	backgroundTex = tex->Load("Textures/Scenes/battle_scene.jpg");
	aura = tex->Load("Textures/Scenes/aura.png");
	cast1 = tex->Load("Textures/Effects/cast_001.png");
	enemyCast = tex->Load("Textures/Effects/cast_008.png");
	indicator = tex->Load("Textures/Effects/fire_003.png");

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
	this->titleFont = titleFont;
	this->menuFont = menuFont;

	// Load buttons Fx
	this->hoverFx = hoverFx;
	this->clickFx = clickFx;
	this->returnFx = returnFx;

	// Gui id goes from 13 to 16
	// Load buttons for the battle system
	int counterId = minIndex;

	btnAttack = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 730, 500, 190, 49 }, "ATTACK");
	btnAttack->SetButtonProperties(currentScene, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnAttack->state = GuiControlState::HIDDEN;
	++counterId;

	btnDefend = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 730, 600, 190, 49 }, "DEFEND");
	btnDefend->SetButtonProperties(currentScene, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnDefend->state = GuiControlState::HIDDEN;
	++counterId;

	btnItem = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 1030, 500, 190, 49 }, "ITEM");
	btnItem->SetButtonProperties(currentScene, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnItem->state = GuiControlState::HIDDEN;
	++counterId;

	btnRun = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 1030, 600, 190, 49 }, "RUN");
	btnRun->SetButtonProperties(currentScene, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
	btnRun->state = GuiControlState::HIDDEN;

	return true;
}

inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}

bool ScreenBattle::Update(Input* input, float dt, uint& focusedButtonId)
{
	if (this->isActive)
	{
		if (input->GetControllerState())
		{
			// minIndex = 11
			// GAMEPAD INPUT
			if (focusedButtonId == minIndex)
			{
				if (input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN)
					focusedButtonId = minIndex + 1;
				else if (input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN)
					focusedButtonId = minIndex + 2;
			}
			else if (focusedButtonId == minIndex + 1)
			{
				if (input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN)
					focusedButtonId = minIndex;
				else if (input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN)
					focusedButtonId = minIndex + 3;
			}
			else if (focusedButtonId == minIndex + 2)
			{
				if (input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN)
					focusedButtonId = minIndex + 3;
				else if (input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN)
					focusedButtonId = minIndex;
			}
			else if (focusedButtonId == minIndex + 3)
			{
				if (input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN)
					focusedButtonId = minIndex + 2;
				else if (input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN)
					focusedButtonId = minIndex + 1;
			}

			bool isHovering = false;
			for (int i = minIndex; i <= maxIndex; ++i)
			{
				if (guiManager->controls.At(i)->data->mouseFocus)
					isHovering = true;
			}
			for (int i = minIndex; i <= maxIndex; ++i)
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
				audio->PlayMusic("Audio/Music/victory.ogg");
			}
		}
		else if (battleSystem->battleState == BattleState::LOST)
		{
			if (battleSystem->currentMusic == BattleMusic::LOST)
			{
				battleSystem->currentMusic = BattleMusic::NONE;
				audio->StopMusic();
				audio->PlayMusic("Audio/Music/defeat.ogg");
			}
		}
		else if (battleSystem->battleState == BattleState::EXIT)
		{
			// Get out of the battle screen and return to the gameplay screen
			sceneGameplay->ExitBattle();
		}
	}

	if (battleSystem->IsTurnChanging()) ResetOneTimeAnimations();

	if (battleSystem->IsTurnChanging())
	{
		if (battleSystem->battleState == BattleState::WON)
		{
			if (!sceneGameplay->GetGameProgress()->hasKilledOfficers)
			{
				if (battleSystem->GetEnemy()->name == "Ben" ||
					battleSystem->GetEnemy()->name == "John" ||
					battleSystem->GetEnemy()->name == "Robert")
				{
					sceneGameplay->GetGameProgress()->numKilledOfficers++;
					if (sceneGameplay->GetGameProgress()->numKilledOfficers == 3) sceneGameplay->GetGameProgress()->hasKilledOfficers = true;
				}
			}
			else if (!sceneGameplay->GetGameProgress()->hasKilledBoss)
			{
				if (battleSystem->GetEnemy()->name == "Takuda")
				{
					sceneGameplay->GetGameProgress()->hasKilledBoss = true;
				}
			}
		}
	}

	if (sceneGameplay->GetGameProgress()->hasKilledBoss)
		sceneGameplay->TransitionToScene(SceneType::ENDING_VICTORY);

	return true;
}

bool ScreenBattle::Draw(Render* render)
{
	if (this->isActive)
	{
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

			if (!cast1Anim.Finished())
			{
				render->DrawTexture(cast1, 100, 200, &cast1Anim.GetCurrentFrame(), 0);
			}

			for (int i = 0; i < battleSystem->GetPlayersList()->Count(); i++)
			{
				if (battleSystem->GetPlayer()->name == battleSystem->GetPlayersList()->At(i)->data->name)
				{
					render->DrawTexture(indicator, 95 + 100 * i, 430, &indicatorAnim.GetCurrentFrame(), 0);
				}
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

			if (!enemyCastAnim.Finished())
			{
				render->DrawTexture(enemyCast, 675, 0, &enemyCastAnim.GetCurrentFrame(), 0);
			}

			render->DrawTexture(indicator, 835, 180, &indicatorAnim.GetCurrentFrame(), 0);
		}

		if (battleSystem->battleState != BattleState::WON &&
			battleSystem->battleState != BattleState::LOST &&
			battleSystem->battleState != BattleState::EXIT)
		{
			render->DrawRectangle({ 115,270,450,100 }, { 255,255,255,127 }, true, false);
			render->DrawRectangle({ 115,270,450,100 }, { 255,255,255,255 }, false, false);

			// Player name
			render->DrawText(menuFont, battleSystem->GetPlayer()->name.GetString(), 125 + 3, 265 + 3, 50, 3, { 105, 105, 105, 255 });
			render->DrawText(menuFont, battleSystem->GetPlayer()->name.GetString(), 125, 265, 50, 3, { 255, 255, 255, 255 });

			// Player level
			char temp[16] = { 0 };
			sprintf_s(temp, 16, "LVL: %03i", battleSystem->GetPlayer()->stats.level);
			render->DrawText(menuFont, temp, 420 + 3, 265 + 3, 50, 3, { 105, 105, 105, 255 });
			render->DrawText(menuFont, temp, 420, 265, 50, 3, { 255, 255, 255, 255 });

			// Player life
			sprintf_s(temp, 16, "HP: %03i", battleSystem->GetPlayer()->stats.currentHP);
			render->DrawText(menuFont, temp, 125 + 3, 305 + 3, 35, 3, { 64, 128, 80, 255 });
			render->DrawText(menuFont, temp, 125, 305, 35, 3, { 127, 255, 160, 255 });

			render->DrawRectangle({ 720,20,450,100 }, { 255,255,255,127 }, true, false);
			render->DrawRectangle({ 720,20,450,100 }, { 255,255,255,255 }, false, false);

			// Enemy name
			render->DrawText(menuFont, battleSystem->GetEnemy()->name.GetString(), 730 + 3, 15 + 3, 50, 3, { 105, 105, 105, 255 });
			render->DrawText(menuFont, battleSystem->GetEnemy()->name.GetString(), 730, 15, 50, 3, { 255, 255, 255, 255 });

			// Enemy level
			sprintf_s(temp, 16, "LVL: %03i", battleSystem->GetEnemy()->stats.level);
			render->DrawText(menuFont, temp, 1020 + 3, 15 + 3, 50, 3, { 105, 105, 105, 255 });
			render->DrawText(menuFont, temp, 1020, 15, 50, 3, { 255, 255, 255, 255 });

			// Enemy life
			sprintf_s(temp, 16, "HP: %03i", battleSystem->GetEnemy()->stats.currentHP);
			render->DrawText(menuFont, temp, 730 + 3, 55 + 3, 35, 3, { 64, 128, 80, 255 });
			render->DrawText(menuFont, temp, 730, 55, 35, 3, { 127, 255, 160, 255 });

			// Draw party members textures
			SDL_Rect rect = { 0,481,32,32 };
			render->scale = 5;
			for (int i = 0; i < battleSystem->GetPlayersList()->Count(); i++)
			{
				rect = battleSystem->GetPlayersList()->At(i)->data->idleAnim.GetFrame(0);
				render->DrawTexture(charactersSpritesheet, 22.5 + 20 * i, 75, &rect, 0);
			}
			render->scale = 1;

			// Draw Enemy textures
			rect = battleSystem->GetEnemy()->animRec;
			render->scale = 5;
			render->DrawTexture(charactersSpritesheet, 170, 25, &rect, 0);
			render->scale = 1;

			render->scale = 2;
			for (int i = 0; i < battleSystem->GetPlayersList()->Count(); i++)
			{
				if (battleSystem->GetPlayer()->name == battleSystem->GetPlayersList()->At(i)->data->name)
				{
					render->DrawTexture(aura, 29 + 50 * i, 165, &auraAnim.GetCurrentFrame(), 0);
				}
			}
			render->scale = 1;
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

	return true;
}

bool ScreenBattle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	// Unload textures
	tex->UnLoad(charactersSpritesheet);
	charactersSpritesheet = nullptr;
	tex->UnLoad(backgroundTex);
	backgroundTex = nullptr;
	tex->UnLoad(cast1);
	cast1 = nullptr;
	tex->UnLoad(enemyCast);
	enemyCast = nullptr;
	tex->UnLoad(indicator);
	indicator = nullptr;

	// Destory GUI Controls
	guiManager->DestroyGuiControl(btnAttack);
	btnAttack = nullptr;
	guiManager->DestroyGuiControl(btnDefend);
	btnDefend = nullptr;
	guiManager->DestroyGuiControl(btnItem);
	btnItem = nullptr;
	guiManager->DestroyGuiControl(btnRun);
	btnRun = nullptr;

	// Removing used modules
	this->entityManager = nullptr;
	this->guiManager = nullptr;
	this->win = nullptr;
	this->audio = nullptr;

	return true;
}

void ScreenBattle::EnableBattleButtons()
{
	for (int i = minIndex; i <= maxIndex; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
	}

	for (int i = 0; i <= minIndex - 1; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
	}
}

void ScreenBattle::DisableBattleButtons()
{
	for (int i = minIndex; i <= maxIndex; ++i)
	{
		guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
	}
}

BattleSystem* ScreenBattle::GetBattleSystem()
{
	return battleSystem;
}

void ScreenBattle::ResetOneTimeAnimations()
{
	cast1Anim.Reset();
	enemyCastAnim.Reset();
}