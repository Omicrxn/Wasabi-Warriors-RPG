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
	optionsBackgroundRect = { 0,0,1240,220 };

	// Fonts
	titleFont = nullptr;
	buttonFont = nullptr;
	menuFont = nullptr;
	menuFont2 = nullptr;

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

bool ScreenBattle::Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, SDL_Texture* charactersSpritesheet, SDL_Texture* guiAtlasTex, Font* titleFont, Font* buttonFont, Font* menuFont, Font* menuFont2, int hoverFx, int clickFx, int returnFx)
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
	optionsBackgroundTex = tex->Load("Textures/Dialog/dialog_background.png");
	cast1 = tex->Load("Textures/Effects/cast_001.png");
	enemyCast = tex->Load("Textures/Effects/cast_008.png");
	indicator = tex->Load("Textures/Effects/fire_003.png");
	playerDefense = tex->Load("Textures/Effects/heal_001.png");
	enemyDefense = tex->Load("Textures/Effects/heal_002.png");

	// Set battle animations
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

	// Player defense
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			playerDefenseAnim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	playerDefenseAnim.speed = 0.5;

	// Enemy defense
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			enemyDefenseAnim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	enemyDefenseAnim.speed = 0.5;

	// Create fonts
	this->titleFont = titleFont;
	this->buttonFont = buttonFont;
	this->menuFont = menuFont;
	this->menuFont2 = menuFont2;

	// Load buttons Fx
	this->hoverFx = hoverFx;
	this->clickFx = clickFx;
	this->returnFx = returnFx;

	// Gui id goes from 13 to 16
	// Load buttons for the battle system
	int counterId = minIndex;

	btnAttack = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 800, 540, 190, 49 }, "Attack");
	btnAttack->SetButtonProperties(currentScene, guiAtlasTex, menuFont2, hoverFx, clickFx, Style::WHITE);
	btnAttack->state = GuiControlState::HIDDEN;
	++counterId;

	btnDefend = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 800, 595, 190, 49 }, "Defend");
	btnDefend->SetButtonProperties(currentScene, guiAtlasTex, menuFont2, hoverFx, clickFx, Style::WHITE);
	btnDefend->state = GuiControlState::HIDDEN;
	++counterId;

	btnItem = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 1010, 540, 190, 49 }, "Item");
	btnItem->SetButtonProperties(currentScene, guiAtlasTex, menuFont2, hoverFx, clickFx, Style::WHITE);
	btnItem->state = GuiControlState::HIDDEN;
	++counterId;

	btnRun = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 1010, 595, 190, 49 }, "Run");
	btnRun->SetButtonProperties(currentScene, guiAtlasTex, menuFont2, hoverFx, clickFx, Style::WHITE);
	btnRun->state = GuiControlState::HIDDEN;

	timer.Start();

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

		battleSystem->SetInventoryOpening(false);
		battleSystem->SetInventoryClosure(false);
		battleSystem->SetHasClickedConsume(false);
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
		render->DrawTexture(optionsBackgroundTex, 20, 480, &optionsBackgroundRect, 0);

		uint width, height;
		win->GetWindowSize(width, height);

		char temp[64] = { 0 };
		if (battleSystem->battleState == BattleState::PLAYER_TURN)
		{
			sprintf_s(temp, 64, "%s's turn", battleSystem->GetPlayer()->name.GetString());
			render->DrawText(menuFont2, temp, 75 + 3, 520 + 3, 70, 0, { 105, 105, 105, 255 });
			render->DrawText(menuFont2, temp, 75, 520, 70, 0, { 255, 255, 255, 255 });

			if (battleSystem->playerState == PlayerState::ATTACK)
			{
				if (timer.ReadSec() <= 0.25f)
				{
					sprintf_s(temp, 64, "%s attacks %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetEnemy()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 105, 105, 105, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 255, 255, 255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					sprintf_s(temp, 64, "%s attacks %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetEnemy()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 128, 113, 27, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 225, 53, 255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();
			}
			else if (battleSystem->playerState == PlayerState::DEFEND)
			{
				if (timer.ReadSec() <= 0.25f)
				{
					sprintf_s(temp, 64, "%s defends himself", battleSystem->GetPlayer()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 105, 105, 105, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 255, 255, 255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					sprintf_s(temp, 64, "%s defends himself", battleSystem->GetPlayer()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 128, 113, 27, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 225, 53, 255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();

				render->scale = 2;
				for (int i = 0; i < battleSystem->GetPlayersList()->Count(); i++)
				{
					if (battleSystem->GetPlayer()->name == battleSystem->GetPlayersList()->At(i)->data->name)
					{
						if (battleSystem->GetCounter() > 50 && !playerDefenseAnim.Finished())
							render->DrawTexture(playerDefense, 0 + 50 * i, 75, &playerDefenseAnim.GetCurrentFrame(), 0);
					}
				}
				render->scale = 1;
			}
			else if (battleSystem->playerState == PlayerState::ITEM)
			{
				if (battleSystem->GetHasClickedConsume())
				{
					if (timer.ReadSec() <= 0.25f)
					{
						sprintf_s(temp, 64, "%s used the item %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetItem().GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 105, 105, 105, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 255, 255, 255 });
					}
					else if (timer.ReadSec() > 0.25f)
					{
						sprintf_s(temp, 64, "%s used the item %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetItem().GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 128, 113, 27, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 225, 53, 255 });
					}
					if (timer.ReadSec() > 0.5f) timer.Start();
				}
				else if (battleSystem->HasClosedInventory() && !battleSystem->GetHasClickedConsume())
				{
					if (timer.ReadSec() <= 0.25f)
					{
						sprintf_s(temp, 64, "%s didn't use any item...", battleSystem->GetPlayer()->name.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 105, 105, 105, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 255, 255, 255 });
					}
					else if (timer.ReadSec() > 0.25f)
					{
						sprintf_s(temp, 64, "%s didn't use any item...", battleSystem->GetPlayer()->name.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 128, 113, 27, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 225, 53, 255 });
					}
					if (timer.ReadSec() > 0.5f) timer.Start();
				}
			}

			if (!cast1Anim.Finished())
			{
				render->DrawTexture(cast1, 100, 90, &cast1Anim.GetCurrentFrame(), 0);
			}

			for (int i = 0; i < battleSystem->GetPlayersList()->Count(); i++)
			{
				if (battleSystem->GetPlayer()->name == battleSystem->GetPlayersList()->At(i)->data->name)
				{
					render->DrawTexture(indicator, 95 + 100 * i, 315, &indicatorAnim.GetCurrentFrame(), 0);
				}
			}
		}
		else if (battleSystem->battleState == BattleState::ENEMY_TURN)
		{
			sprintf_s(temp, 64, "%s's turn", battleSystem->GetEnemy()->name.GetString());
			render->DrawText(menuFont2, temp, 75 + 3, 520 + 3, 70, 0, { 105, 105, 105, 255 });
			render->DrawText(menuFont2, temp, 75, 520, 70, 0, { 255, 255, 255, 255 });

			if (battleSystem->enemyState == EnemyState::ATTACK)
			{
				if (timer.ReadSec() <= 0.25f)
				{
					sprintf_s(temp, 64, "%s attacks %s", battleSystem->GetEnemy()->name.GetString(), battleSystem->GetPlayer()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 105, 105, 105, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 255, 255, 255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					sprintf_s(temp, 64, "%s attacks %s", battleSystem->GetEnemy()->name.GetString(), battleSystem->GetPlayer()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 128, 113, 27, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 225, 53, 255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();
			}
			else if (battleSystem->enemyState == EnemyState::DEFEND)
			{
				if (timer.ReadSec() <= 0.25f)
				{
					sprintf_s(temp, 64, "%s defends himself", battleSystem->GetEnemy()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 105, 105, 105, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 255, 255, 255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					sprintf_s(temp, 64, "%s defends himself", battleSystem->GetEnemy()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 128, 113, 27, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 225, 53, 255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();

				render->scale = 2;
				if (battleSystem->GetCounter() > 100 && !enemyDefenseAnim.Finished())
					render->DrawTexture(enemyDefense, 350, 25, &enemyDefenseAnim.GetCurrentFrame(), 0);
				render->scale = 1;
			}
			else if (battleSystem->enemyState == EnemyState::ITEM)
			{
				if (timer.ReadSec() <= 0.25f)
				{
					sprintf_s(temp, 64, "%s used the item %s", battleSystem->GetEnemy()->name.GetString(), battleSystem->GetItem().GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 105, 105, 105, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 255, 255, 255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					sprintf_s(temp, 64, "%s used the item %s", battleSystem->GetEnemy()->name.GetString(), battleSystem->GetItem().GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 50, 0, { 128, 113, 27, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 50, 0, { 255, 225, 53, 255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();
			}

			if (!enemyCastAnim.Finished())
				render->DrawTexture(enemyCast, 675, 0, &enemyCastAnim.GetCurrentFrame(), 0);

			render->DrawTexture(indicator, 835, 180, &indicatorAnim.GetCurrentFrame(), 0);
		}

		if (battleSystem->battleState != BattleState::WON &&
			battleSystem->battleState != BattleState::LOST &&
			battleSystem->battleState != BattleState::EXIT)
		{
			render->DrawRectangle({ 115,160,450,100 }, { 255,255,255,127 }, true, false);
			render->DrawRectangle({ 115,160,450,100 }, { 255,255,255,255 }, false, false);

			// Player name
			render->DrawText(menuFont2, battleSystem->GetPlayer()->name.GetString(), 125 + 3, 155 + 3, 50, 3, { 105, 105, 105, 255 });
			render->DrawText(menuFont2, battleSystem->GetPlayer()->name.GetString(), 125, 155, 50, 3, { 255, 255, 255, 255 });

			// Player level
			sprintf_s(temp, 64, "LVL: %03i", battleSystem->GetPlayer()->stats.level);
			render->DrawText(menuFont2, temp, 370 + 3, 155 + 3, 50, 3, { 105, 105, 105, 255 });
			render->DrawText(menuFont2, temp, 370, 155, 50, 3, { 255, 255, 255, 255 });

			// Player life
			sprintf_s(temp, 64, "HP: %03i", battleSystem->GetPlayer()->stats.currentHP);
			render->DrawText(menuFont2, temp, 125 + 3, 195 + 3, 35, 3, { 64, 128, 80, 255 });
			render->DrawText(menuFont2, temp, 125, 195, 35, 3, { 127, 255, 160, 255 });

			// Player strength
			sprintf_s(temp, 64, "Strength: %03i", battleSystem->GetPlayer()->stats.strength);
			render->DrawText(menuFont2, temp, 125 + 3, 220 + 3, 35, 3, { 128, 64, 128, 255 });
			render->DrawText(menuFont2, temp, 125, 220, 35, 3, { 255, 128, 0, 255 });

			// Player damage
			sprintf_s(temp, 64, "Damage: %03i", battleSystem->GetPlayer()->stats.strength);
			render->DrawText(menuFont2, temp, 370 + 3, 195 + 3, 35, 3, { 103, 46, 46, 255 });
			render->DrawText(menuFont2, temp, 370, 195, 35, 3, { 205, 92, 92, 255 });

			// Player defense
			sprintf_s(temp, 64, "Defense: %03i", battleSystem->GetPlayer()->stats.defense);
			render->DrawText(menuFont2, temp, 370 + 3, 220 + 3, 35, 3, { 0, 64, 128, 255 });
			render->DrawText(menuFont2, temp, 370, 220, 35, 3, { 0, 128, 255, 255 });

			render->DrawRectangle({ 720,20,450,100 }, { 255,255,255,127 }, true, false);
			render->DrawRectangle({ 720,20,450,100 }, { 255,255,255,255 }, false, false);

			// Enemy name
			render->DrawText(menuFont2, battleSystem->GetEnemy()->name.GetString(), 730 + 3, 15 + 3, 50, 3, { 105, 105, 105, 255 });
			render->DrawText(menuFont2, battleSystem->GetEnemy()->name.GetString(), 730, 15, 50, 3, { 255, 255, 255, 255 });

			// Enemy level
			sprintf_s(temp, 64, "LVL: %03i", battleSystem->GetEnemy()->stats.level);
			render->DrawText(menuFont2, temp, 975 + 3, 15 + 3, 50, 3, { 105, 105, 105, 255 });
			render->DrawText(menuFont2, temp, 975, 15, 50, 3, { 255, 255, 255, 255 });

			// Enemy life
			sprintf_s(temp, 64, "HP: %03i", battleSystem->GetEnemy()->stats.currentHP);
			render->DrawText(menuFont2, temp, 730 + 3, 55 + 3, 35, 3, { 64, 128, 80, 255 });
			render->DrawText(menuFont2, temp, 730, 55, 35, 3, { 127, 255, 160, 255 });

			// Enemy strength
			sprintf_s(temp, 64, "Strength: %03i", battleSystem->GetEnemy()->stats.strength);
			render->DrawText(menuFont2, temp, 730 + 3, 80 + 3, 35, 3, { 128, 64, 128, 255 });
			render->DrawText(menuFont2, temp, 730, 80, 35, 3, { 255, 128, 0, 255 });

			// Enemy damage
			sprintf_s(temp, 64, "Damage: %03i", battleSystem->GetEnemy()->stats.strength);
			render->DrawText(menuFont2, temp, 975 + 3, 55 + 3, 35, 3, { 103, 46, 46, 255 });
			render->DrawText(menuFont2, temp, 975, 55, 35, 3, { 205, 92, 92, 255 });

			// Enemy defense
			sprintf_s(temp, 64, "Defense: %03i", battleSystem->GetEnemy()->stats.defense);
			render->DrawText(menuFont2, temp, 975 + 3, 80 + 3, 35, 3, { 0, 64, 128, 255 });
			render->DrawText(menuFont2, temp, 975, 80, 35, 3, { 0, 128, 255, 255 });
		}
		else if (battleSystem->battleState == BattleState::WON)
		{
			// Display winner text
			render->DrawText(menuFont2, "You win!", 50 + 3, 30 + 3, 125, 0, { 105, 105, 105, 255 });
			render->DrawText(menuFont2, "You win!", 50, 30, 125, 0, { 255, 255, 255, 255 });
		}
		else if (battleSystem->battleState == BattleState::LOST)
		{
			// Revise
			SDL_Rect rect = { 0,0,1280,720 };
			render->DrawRectangle(rect, { 127,127,127,255 }, true, false);

			// Display loser text
			render->DrawText(menuFont2, "You lose...", 50 + 3, 30 + 3, 700, 0, { 105, 105, 105, 255 });
			render->DrawText(menuFont2, "You lose...", 50, 30, 700, 0, { 255, 255, 255, 255 });
		}

		// Draw party members textures
		render->scale = 5;
		SDL_Rect rect = { 0,481,32,32 };
		for (int i = 0; i < battleSystem->GetPlayersList()->Count(); i++)
		{
			rect = battleSystem->GetPlayersList()->At(i)->data->idleAnim.GetFrame(0);
			render->DrawTexture(charactersSpritesheet, 22.5 + 20 * i, 52.5, &rect, 0);
		}

		// Draw Enemy textures
		rect = battleSystem->GetEnemy()->animRec;
		render->DrawTexture(charactersSpritesheet, 170, 25, &rect, 0);
		render->scale = 1;
	}

	return true;
}

bool ScreenBattle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	// Unload textures
	tex->UnLoad(charactersSpritesheet);
	tex->UnLoad(backgroundTex);
	tex->UnLoad(optionsBackgroundTex);
	tex->UnLoad(cast1);
	tex->UnLoad(enemyCast);
	tex->UnLoad(indicator);

	// Destory GUI Controls
	guiManager->DestroyGuiControl(btnAttack);
	guiManager->DestroyGuiControl(btnDefend);
	guiManager->DestroyGuiControl(btnItem);
	guiManager->DestroyGuiControl(btnRun);

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
	playerDefenseAnim.Reset();
	enemyDefenseAnim.Reset();
}