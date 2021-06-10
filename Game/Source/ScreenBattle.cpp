#include "ScreenBattle.h"
#include "BattleSystem.h"
#include "SceneGameplay.h"

#include "EntityManager.h"
#include "GuiManager.h"
#include "Window.h"
#include "Textures.h"

#include "Player.h"
#include "Enemy.h"
#include "Activator.h"

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
	guiAtlasTex2 = nullptr;

	backgroundRect = { 0, 0, 1280, 720 };
	optionsBackgroundRect = { 0,0,1240,220 };
	backRamen1 = { 24,1323,363,210 };
	backRamen2 = { 408,1344,363,192 };
	frontRamen1 = { 1827,45,246,84 };
	frontRamen2 = { 60,1218,276,93 };
	leftSticks = { 1842,165,225,207 };
	rightSticks = { 1869,402,225,207 };
	crossRect = { 0,0,630,630 };

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

	// Variables to manage attack/defense menus
	hover = { 0,0 };
	pressed = false;

	lockedNotificationTimer = 0;
}

ScreenBattle::~ScreenBattle()
{
}

bool ScreenBattle::Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, SDL_Texture* charactersSpritesheet, SDL_Texture* guiAtlasTex, SDL_Texture* guiAtlasTex2, Font* titleFont, Font* buttonFont, Font* menuFont, Font* menuFont2, int hoverFx, int clickFx, int returnFx)
{
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
	this->guiAtlasTex2 = guiAtlasTex2;
	/*backgroundTex = tex->Load("Textures/Scenes/battle_scene.jpg");*/
	optionsBackgroundTex = tex->Load("Textures/Dialog/dialog_background.png");
	cast1 = tex->Load("Textures/Effects/cast_001.png");
	enemyCast = tex->Load("Textures/Effects/cast_008.png");
	indicator = tex->Load("Textures/Effects/fire_003.png");
	playerDefense = tex->Load("Textures/Effects/heal_001.png");
	enemyDefense = tex->Load("Textures/Effects/heal_002.png");
	crossTex = tex->Load("Textures/Items/cross.png");

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
	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
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

			UpdateControllerSelection(focusedButtonId);
		}

		battleSystem->Update(input, dt);

		if (battleSystem->battleState == BattleState::PLAYER_TURN)
		{
			if (battleSystem->playerState == PlayerState::ATTACK || battleSystem->playerState == PlayerState::DEFENSE)
			{
				if ((input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN) && hover.x > 0)
				{
					hover.x--;
				}
				if ((input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN) && hover.x < 3)
				{
					hover.x++;
				}
				if (input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
					input->GetControllerButton(CONTROLLER_BUTTON_A) == KEY_DOWN ||
					(HoverUpdate(input) && input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN))
					pressed = true;

				if (input->GetKey(SDL_SCANCODE_F) == KEY_UP || input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP ||
					input->GetControllerButton(CONTROLLER_BUTTON_A) == KEY_UP ||
					(HoverUpdate(input) && input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP))
				{
					EntitySubtype entitySubtype;
					if (battleSystem->playerState == PlayerState::ATTACK) entitySubtype = EntitySubtype::ATTACK;
					else if (battleSystem->playerState == PlayerState::DEFENSE) entitySubtype = EntitySubtype::DEFENSE;

					SString names[3] = { "null", "null", "null" };
					int index = 0;
					for (int i = 0; i < entityManager->activatorList.Count(); i++)
					{
						if ((Entity*)entityManager->activatorList.At(i) != nullptr && ((Entity*)entityManager->activatorList.At(i)->data)->subtype == entitySubtype)
						{
							names[index] = ((Activator*)entityManager->activatorList.At(i)->data)->name;
							index++;
						}
					}

					selectedItem.Create("null");

					switch (hover.x)
					{
					case 0:
						selectedItem = "default";
						break;
					case 1:
						if (names[0] != "null") selectedItem = names[0];
						break;
					case 2:
						if (names[1] != "null") selectedItem = names[1];
						break;
					case 3:
						if (names[2] != "null") selectedItem = names[2];
						break;
					}

					if (battleSystem->playerState == PlayerState::ATTACK)
					{
						if (selectedItem == "default") battleSystem->specialAttack = SpecialAttack::DEFAULT;
						else if (selectedItem == "Schichimi") battleSystem->specialAttack = SpecialAttack::ATTACK_1;
						else if (selectedItem == "Karashi") battleSystem->specialAttack = SpecialAttack::ATTACK_2;
						else if (selectedItem == "Rayu") battleSystem->specialAttack = SpecialAttack::ATTACK_3;
						else if (selectedItem == "null")
						{
							battleSystem->specialAttack = SpecialAttack::LOCKED;
							lockedNotificationTimer = 0;
						}
					}
					else if (battleSystem->playerState == PlayerState::DEFENSE)
					{
						if (selectedItem == "default") battleSystem->specialDefense = SpecialDefense::DEFAULT;
						else if (selectedItem == "Sushi") battleSystem->specialDefense = SpecialDefense::DEFENSE_1;
						else if (selectedItem == "Oniguiri") battleSystem->specialDefense = SpecialDefense::DEFENSE_2;
						else if (selectedItem == "Sashimi") battleSystem->specialDefense = SpecialDefense::DEFENSE_3;
						else if (selectedItem == "null")
						{
							battleSystem->specialDefense = SpecialDefense::LOCKED;
							lockedNotificationTimer = 0;
						}
					}

					if (((battleSystem->specialAttack == SpecialAttack::ATTACK_1 && battleSystem->GetSpecialAttack(0)) ||
						(battleSystem->specialAttack == SpecialAttack::ATTACK_2 && battleSystem->GetSpecialAttack(1)) ||
						(battleSystem->specialAttack == SpecialAttack::ATTACK_3 && battleSystem->GetSpecialAttack(2))) ||
					   ((battleSystem->specialDefense == SpecialDefense::DEFENSE_1 && battleSystem->GetSpecialDefense(0)) ||
						(battleSystem->specialDefense == SpecialDefense::DEFENSE_2 && battleSystem->GetSpecialDefense(1)) ||
						(battleSystem->specialDefense == SpecialDefense::DEFENSE_3 && battleSystem->GetSpecialDefense(2))))
					{
						used = true;
						usedNotificationTimer = 0;
						battleSystem->specialAttack = SpecialAttack::NONE;
						battleSystem->specialDefense = SpecialDefense::NONE;
					}

					pressed = false;

					switch (battleSystem->playerState)
					{
					case PlayerState::ATTACK:
						if (battleSystem->specialAttack != SpecialAttack::LOCKED &&
							battleSystem->specialAttack != SpecialAttack::NONE)
							ShowButtons();
						break;
					case PlayerState::DEFENSE:
						if (battleSystem->specialDefense != SpecialDefense::LOCKED &&
							battleSystem->specialDefense != SpecialDefense::NONE)
							ShowButtons();
						break;
					default:
						break;
					}
				}

				if (pressed && input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
					pressed = false;

				HoverUpdate(input);

				if (battleSystem->specialAttack == SpecialAttack::LOCKED ||
					battleSystem->specialDefense == SpecialDefense::LOCKED)
					lockedNotificationTimer++;

				if (lockedNotificationTimer > 100)
				{
					lockedNotificationTimer = 0;
					battleSystem->specialAttack = SpecialAttack::NONE;
					battleSystem->specialDefense = SpecialDefense::NONE;
				}

				if (used) usedNotificationTimer++;
				if (usedNotificationTimer > 100)
				{
					used = false;
					usedNotificationTimer = 0;
				}
			}
		}
		else if (battleSystem->battleState == BattleState::WON)
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

		battleSystem->specialAttack = SpecialAttack::NONE;
		battleSystem->specialDefense = SpecialDefense::NONE;

		hover.x = 0;
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

		render->scale = 1.5f;

		render->DrawTexture(guiAtlasTex2, 45, 183, &backRamen1, 0);
		render->DrawTexture(guiAtlasTex2, 445, 110, &backRamen2, 0);

		if (battleSystem->battleState == BattleState::PLAYER_TURN)
			render->DrawTexture(guiAtlasTex2, 0, 90, &leftSticks, 0);
		else if (battleSystem->battleState == BattleState::ENEMY_TURN)
			render->DrawTexture(guiAtlasTex2, 629, 0, &rightSticks, 0);

		render->scale = 1;

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
				if (battleSystem->specialAttack == SpecialAttack::DEFAULT)
				{
					if (timer.ReadSec() <= 0.25f)
					{
						sprintf_s(temp, 64, "%s attacks %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetEnemy()->name.GetString(), selectedItem.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
					}
					else if (timer.ReadSec() > 0.25f)
					{
						sprintf_s(temp, 64, "%s attacks %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetEnemy()->name.GetString(), selectedItem.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
					}
					if (timer.ReadSec() > 0.5f) timer.Start();
				}
				else if (battleSystem->specialAttack != SpecialAttack::NONE && battleSystem->specialAttack != SpecialAttack::LOCKED)
				{
					if (timer.ReadSec() <= 0.25f)
					{
						sprintf_s(temp, 64, "%s attacks %s with %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetEnemy()->name.GetString(), selectedItem.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
					}
					else if (timer.ReadSec() > 0.25f)
					{
						sprintf_s(temp, 64, "%s attacks %s with %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetEnemy()->name.GetString(), selectedItem.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
					}
					if (timer.ReadSec() > 0.5f) timer.Start();
				}
			}
			else if (battleSystem->playerState == PlayerState::DEFENSE)
			{
				if (battleSystem->specialDefense == SpecialDefense::DEFAULT)
				{
					if (timer.ReadSec() <= 0.25f)
					{
						sprintf_s(temp, 64, "%s defends", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetEnemy()->name.GetString(), selectedItem.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
					}
					else if (timer.ReadSec() > 0.25f)
					{
						sprintf_s(temp, 64, "%s defends", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetEnemy()->name.GetString(), selectedItem.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
					}
					if (timer.ReadSec() > 0.5f) timer.Start();
				}
				else if (battleSystem->specialDefense != SpecialDefense::NONE && battleSystem->specialDefense != SpecialDefense::LOCKED)
				{
					if (timer.ReadSec() <= 0.25f)
					{
						sprintf_s(temp, 64, "%s defends with %s", battleSystem->GetPlayer()->name.GetString(), selectedItem.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
					}
					else if (timer.ReadSec() > 0.25f)
					{
						sprintf_s(temp, 64, "%s defends with %s", battleSystem->GetPlayer()->name.GetString(), selectedItem.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
					}
					if (timer.ReadSec() > 0.5f) timer.Start();
				}

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
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
					}
					else if (timer.ReadSec() > 0.25f)
					{
						sprintf_s(temp, 64, "%s used the item %s", battleSystem->GetPlayer()->name.GetString(), battleSystem->GetItem().GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
					}
					if (timer.ReadSec() > 0.5f) timer.Start();
				}
				else if (battleSystem->HasClosedInventory() && !battleSystem->GetHasClickedConsume())
				{
					if (timer.ReadSec() <= 0.25f)
					{
						sprintf_s(temp, 64, "%s didn't use any item...", battleSystem->GetPlayer()->name.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
					}
					else if (timer.ReadSec() > 0.25f)
					{
						sprintf_s(temp, 64, "%s didn't use any item...", battleSystem->GetPlayer()->name.GetString());
						render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
						render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
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
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					sprintf_s(temp, 64, "%s attacks %s", battleSystem->GetEnemy()->name.GetString(), battleSystem->GetPlayer()->name.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();
			}
			else if (battleSystem->enemyState == EnemyState::DEFEND)
			{
				if (timer.ReadSec() <= 0.25f)
				{
					sprintf_s(temp, 64, "%s defends", battleSystem->GetEnemy()->name.GetString(), selectedItem.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					sprintf_s(temp, 64, "%s defends", battleSystem->GetEnemy()->name.GetString(), selectedItem.GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
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
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 105, 105, 105, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 255, 255, 255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					sprintf_s(temp, 64, "%s used the item %s", battleSystem->GetEnemy()->name.GetString(), battleSystem->GetItem().GetString());
					render->DrawText(menuFont2, temp, 75 + 3, 590 + 3, 40, 0, { 128, 113, 27, 255 });
					render->DrawText(menuFont2, temp, 75, 590, 40, 0, { 255, 225, 53, 255 });
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

		render->scale = 1.5;
		render->DrawTexture(guiAtlasTex2, 83, 263, &frontRamen1, 0);
		render->DrawTexture(guiAtlasTex2, 488, 164, &frontRamen2, 0);
		render->scale = 1;
	}

	// Draw attack and defense submenus over the rest of the battle screen
	if (battleSystem->battleState == BattleState::PLAYER_TURN &&
		(battleSystem->playerState == PlayerState::ATTACK ||
		battleSystem->playerState == PlayerState::DEFENSE) &&
		((battleSystem->specialAttack == SpecialAttack::NONE ||
		battleSystem->specialAttack == SpecialAttack::LOCKED) &&
		(battleSystem->specialDefense == SpecialDefense::NONE ||
		battleSystem->specialDefense == SpecialDefense::LOCKED)))
	{
		// Draw dark background
		render->DrawRectangle({ 0,0,1280,720 }, { 0,0,0,192 }, true, false);
		
		// Draw slots
		SDL_Rect rect = { 237,171,108,108 };
		SDL_Rect rect2 = { 393,108,108,108 };
		SDL_Rect rect3 = { 237,315,108,108 };
		SDL_Rect rect4 = { 642,99,87,27 };
		SDL_Rect rect5 = { 642,180,87,27 };
		SDL_Rect rect6 = { 642,138,87,27 };
		SDL_Rect rect7 = { 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 720 / 2 - rect.h / 2 - 10, rect.w * 7 + 20, rect.h + 20 + rect4.h + 10};

		render->DrawRectangle(rect7, { 255,255,255,128 }, true, false);
		render->DrawRectangle(rect7, { 255,255,255,255 }, false, false);

		for (int i = 0; i < 4; i++)
		{
			if (i == hover.x && pressed)
			{
				// Draw hover slot
				render->DrawTexture(guiAtlasTex2, 1280 / 2 - rect.w / 2 - rect.w * 3 + (rect.w * 2) * i, 720 / 2 - rect.h / 2, &rect3, 0);

				// Draw hover panel
				render->DrawTexture(guiAtlasTex2, 1280 / 2 - rect.w / 2 - rect.w * 3 + (rect.w * 2) * i + 10, 720 / 2 + rect.h / 2 + 10, &rect6, 0);
			}
			else if (i == hover.x)
			{
				// Draw hover slot
				render->DrawTexture(guiAtlasTex2, 1280 / 2 - rect.w / 2 - rect.w * 3 + (rect.w * 2) * i, 720 / 2 - rect.h / 2, &rect2, 0);

				// Draw hover panel
				render->DrawTexture(guiAtlasTex2, 1280 / 2 - rect.w / 2 - rect.w * 3 + (rect.w * 2) * i + 10, 720 / 2 + rect.h / 2 + 10, &rect5, 0);
			}
			else
			{
				// Draw slot
				render->DrawTexture(guiAtlasTex2, 1280 / 2 - rect.w / 2 - rect.w * 3 + (rect.w * 2) * i, 720 / 2 - rect.h / 2, &rect, 0);

				// Draw panel
				render->DrawTexture(guiAtlasTex2, 1280 / 2 - rect.w / 2 - rect.w * 3 + (rect.w * 2) * i + 10, 720 / 2 + rect.h / 2 + 10, &rect4, 0);
			}
		}

		if (battleSystem->playerState == PlayerState::ATTACK)
		{
			if (battleSystem->specialAttack == SpecialAttack::LOCKED)
			{
				char temp[64] = { 0 };
				if (timer.ReadSec() <= 0.25f)
				{
					render->DrawText(menuFont2, "This attack is locked!", 350 + 3, 100 + 3, 70, 0, { 64,0,0,255 });
					render->DrawText(menuFont2, "This attack is locked!", 350, 100, 70, 0, { 128,0,0,255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					render->DrawText(menuFont2, "This attack is locked!", 350 + 3, 100 + 3, 70, 0, { 128,0,0,255 });
					render->DrawText(menuFont2, "This attack is locked!", 350, 100, 70, 0, { 255,0,0,255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();
			}
			else if (used)
			{
				char temp[64] = { 0 };
				if (timer.ReadSec() <= 0.25f)
				{
					render->DrawText(menuFont2, "You already used this attack!", 250 + 3, 100 + 3, 70, 0, { 64,0,0,255 });
					render->DrawText(menuFont2, "You already used this attack!", 250, 100, 70, 0, { 128,0,0,255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					render->DrawText(menuFont2, "You already used this attack!", 250 + 3, 100 + 3, 70, 0, { 128,0,0,255 });
					render->DrawText(menuFont2, "You already used this attack!", 250, 100, 70, 0, { 255,0,0,255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();
			}

			// Draw text
			render->DrawText(menuFont2, "Choose an attack!", 400 + 3, 200 + 3, 70, 0, { 128,128,128,255 });
			render->DrawText(menuFont2, "Choose an attack!", 400, 200, 70, 0, { 255,255,255,255 });

			render->scale = 2;
			SDL_Rect basicAttackRect = { 125,486,44,27 };
			render->DrawTexture(entityManager->itemsTexture2, (1280 / 2 - rect.w * 2 - 10) / 3, (720 / 2 - rect.h / 2 + 25) / 2, &basicAttackRect, 0);
			render->scale = 1;

			render->DrawText(menuFont2, "Basic", 1280 / 2 - rect.w / 2 - rect.w * 3 + 18, 720 / 2 - rect.h / 2 + 117, 25, 0, { 128,128,128,255 });

			int attackPosition = 0;
			for (int i = 0; i < entityManager->activatorList.Count(); i++)
			{
				if ((Entity*)entityManager->activatorList.At(i) != nullptr && ((Entity*)entityManager->activatorList.At(i)->data)->subtype == EntitySubtype::ATTACK)
				{
					render->scale = 3;
					render->DrawTexture(entityManager->itemsTexture, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * attackPosition + 20) / 3, (720 / 2 - rect.h / 2 + 8) / 3, &(((Activator*)entityManager->activatorList.At(i)->data)->GetRect()), 0);
					render->scale = 1;

					render->DrawText(menuFont2, ((Activator*)entityManager->activatorList.At(i)->data)->name.GetString(), 1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * attackPosition + 18, 720 / 2 - rect.h / 2 + 117, 25, 0, { 128,128,128,255 });

					render->scale = 0.25;
					if (((Activator*)entityManager->activatorList.At(i)->data)->name == "Schichimi" && battleSystem->GetSpecialAttack(0))
						render->DrawTexture(crossTex, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * attackPosition + 20) * 4 - 175, (720 / 2 - rect.h / 2 + 8) * 4 - 50, &crossRect, 0);
					else if (((Activator*)entityManager->activatorList.At(i)->data)->name == "Karashi" && battleSystem->GetSpecialAttack(1))
						render->DrawTexture(crossTex, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * attackPosition + 20) * 4 - 175, (720 / 2 - rect.h / 2 + 8) * 4 - 50, &crossRect, 0);
					else if (((Activator*)entityManager->activatorList.At(i)->data)->name == "Rayu" && battleSystem->GetSpecialAttack(2))
						render->DrawTexture(crossTex, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * attackPosition + 20) * 4 - 175, (720 / 2 - rect.h / 2 + 8) * 4 - 50, &crossRect, 0);
					render->scale = 1;

					attackPosition++;
				}
			}

			if (attackPosition < 3)
			{
				SDL_Rect itemRect = { 12,344,22,30 };
				int lockedNum = 3 - attackPosition;
				for (int i = attackPosition; i < attackPosition + lockedNum; i++)
				{
					render->scale = 3;
					render->DrawTexture(entityManager->itemsTexture, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * i + 20) / 3, (720 / 2 - rect.h / 2 + 8) / 3, &itemRect, 0);
					render->scale = 1;

					render->DrawText(menuFont2, "Locked", 1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * i + 18, 720 / 2 - rect.h / 2 + 117, 25, 0, { 128,0,0,255 });
				}
			}

			SString attackNames[3] = { "null", "null", "null" };
			int index = 0;
			for (int i = 0; i < entityManager->activatorList.Count(); i++)
			{
				if ((Entity*)entityManager->activatorList.At(i) != nullptr && ((Entity*)entityManager->activatorList.At(i)->data)->subtype == EntitySubtype::ATTACK)
				{
					attackNames[index] = ((Activator*)entityManager->activatorList.At(i)->data)->name;
					index++;
				}
			}

			bool locked = false;
			SString itemToPrint;
			itemToPrint.Create("null");

			switch (hover.x)
			{
			case 0:
				render->DrawText(menuFont2, "Basic attack", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 475 + 3, 50, 3, { 128,128,128,255 });
				render->DrawText(menuFont2, "Basic attack", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 475, 50, 3, { 255,255,255,255 });

				render->DrawText(menuFont2, "Wasabi never stops itching.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 128,128,128,255 });
				render->DrawText(menuFont2, "Wasabi never stops itching.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 255,255,255,255 });
				break;
			case 1:
				if (attackNames[0] != "null") itemToPrint = attackNames[0];
				else locked = true;
				break;
			case 2:
				if (attackNames[1] != "null") itemToPrint = attackNames[1];
				else locked = true;
				break;
			case 3:
				if (attackNames[2] != "null") itemToPrint = attackNames[2];
				else locked = true;
				break;
			}

			if (locked)
			{
				render->DrawText(menuFont2, "Locked", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 475 + 3, 50, 3, { 64,0,0,255 });
				render->DrawText(menuFont2, "Locked", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 475, 50, 3, { 128,0,0,255 });

				render->DrawText(menuFont2, "This attack is still locked...", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 64,0,0,255 });
				render->DrawText(menuFont2, "This attack is still locked...", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 128,0,0,255 });
				render->DrawText(menuFont2, "You need to find it if you want to use it.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 560 + 3, 40, 3, { 64,0,0,255 });
				render->DrawText(menuFont2, "You need to find it if you want to use it.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 560, 40, 3, { 128,0,0,255 });
			}

			if (itemToPrint != "null")
			{
				render->DrawText(menuFont2, itemToPrint.GetString(), 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 475 + 3, 50, 3, { 128,128,128,255 });
				render->DrawText(menuFont2, itemToPrint.GetString(), 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 475, 50, 3, { 255,255,255,255 });

				if (itemToPrint == "Schichimi")
				{
					render->DrawText(menuFont2, "It substracts a big amount of your enemy", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "It substracts a big amount of your enemy", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 255,255,255,255 });
					render->DrawText(menuFont2, "life at once.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 560 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "life at once.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 560, 40, 3, { 255,255,255,255 });
				}
				else if (itemToPrint == "Karashi")
				{
					render->DrawText(menuFont2, "It substracts a constant small amount of", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "It substracts a constant small amount of", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 255,255,255,255 });
					render->DrawText(menuFont2, "your enemy life every turn.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 560 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "your enemy life every turn.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 560, 40, 3, { 255,255,255,255 });
				}
				else if (itemToPrint == "Rayu")
				{
					render->DrawText(menuFont2, "It increases by a 30 per cent your current", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "It increases by a 30 per cent your current", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 255,255,255,255 });
					render->DrawText(menuFont2, "attack damage.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 560 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "attack damage.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 560, 40, 3, { 255,255,255,255 });
				}
			}
		}
		else if (battleSystem->playerState == PlayerState::DEFENSE)
		{
			if (battleSystem->specialDefense == SpecialDefense::LOCKED)
			{
				char temp[64] = { 0 };
				if (timer.ReadSec() <= 0.25f)
				{
					render->DrawText(menuFont2, "This defense is locked!", 330 + 3, 100 + 3, 70, 0, { 64,0,0,255 });
					render->DrawText(menuFont2, "This defense is locked!", 330, 100, 70, 0, { 128,0,0,255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					render->DrawText(menuFont2, "This defense is locked!", 330 + 3, 100 + 3, 70, 0, { 128,0,0,255 });
					render->DrawText(menuFont2, "This defense is locked!", 330, 100, 70, 0, { 255,0,0,255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();
			}
			else if (used)
			{
				char temp[64] = { 0 };
				if (timer.ReadSec() <= 0.25f)
				{
					render->DrawText(menuFont2, "You already used this defense!", 250 + 3, 100 + 3, 70, 0, { 64,0,0,255 });
					render->DrawText(menuFont2, "You already used this defense!", 250, 100, 70, 0, { 128,0,0,255 });
				}
				else if (timer.ReadSec() > 0.25f)
				{
					render->DrawText(menuFont2, "You already used this defense!", 250 + 3, 100 + 3, 70, 0, { 128,0,0,255 });
					render->DrawText(menuFont2, "You already used this defense!", 250, 100, 70, 0, { 255,0,0,255 });
				}
				if (timer.ReadSec() > 0.5f) timer.Start();
			}

			// Draw text
			render->DrawText(menuFont2, "Choose a defense!", 400 + 3, 200 + 3, 70, 0, { 128,128,128,255 });
			render->DrawText(menuFont2, "Choose a defense!", 400, 200, 70, 0, { 255,255,255,255 });

			render->scale = 2;
			SDL_Rect basicDefenseRect = { 342,54,34,32 };
			render->DrawTexture(entityManager->itemsTexture, (1280 / 2 - rect.w * 2 - 0) / 3, (720 / 2 - rect.h / 2 + 25) / 2, &basicDefenseRect, 0);
			render->scale = 1;

			render->DrawText(menuFont2, "Basic", 1280 / 2 - rect.w / 2 - rect.w * 3 + 18, 720 / 2 - rect.h / 2 + 117, 25, 0, { 128,128,128,255 });

			int defensePosition = 0;
			for (int i = 0; i < entityManager->activatorList.Count(); i++)
			{
				if ((Entity*)entityManager->activatorList.At(i) != nullptr && ((Entity*)entityManager->activatorList.At(i)->data)->subtype == EntitySubtype::DEFENSE)
				{
					render->scale = 3;
					render->DrawTexture(entityManager->itemsTexture, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * defensePosition + 20) / 3, (720 / 2 - rect.h / 2 + 8) / 3, &(((Activator*)entityManager->activatorList.At(i)->data)->GetRect()), 0);
					render->scale = 1;

					render->DrawText(menuFont2, ((Activator*)entityManager->activatorList.At(i)->data)->name.GetString(), 1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * defensePosition + 18, 720 / 2 - rect.h / 2 + 117, 25, 0, { 128,128,128,255 });

					render->scale = 0.25;
					if (((Activator*)entityManager->activatorList.At(i)->data)->name == "Sushi" && battleSystem->GetSpecialDefense(0))
						render->DrawTexture(crossTex, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * defensePosition + 20) * 4 - 175, (720 / 2 - rect.h / 2 + 8) * 4 - 50, &crossRect, 0);
					else if (((Activator*)entityManager->activatorList.At(i)->data)->name == "Oniguiri" && battleSystem->GetSpecialDefense(1))
						render->DrawTexture(crossTex, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * defensePosition + 20) * 4 - 175, (720 / 2 - rect.h / 2 + 8) * 4 - 50, &crossRect, 0);
					else if (((Activator*)entityManager->activatorList.At(i)->data)->name == "Sashimi" && battleSystem->GetSpecialDefense(2))
						render->DrawTexture(crossTex, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * defensePosition + 20) * 4 - 175, (720 / 2 - rect.h / 2 + 8) * 4 - 50, &crossRect, 0);
					render->scale = 1;

					defensePosition++;
				}
			}

			if (defensePosition < 3)
			{
				SDL_Rect itemRect = { 12,344,22,30 };
				int lockedNum = 3 - defensePosition;
				for (int i = defensePosition; i < defensePosition + lockedNum; i++)
				{
					render->scale = 3;
					render->DrawTexture(entityManager->itemsTexture, (1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * i + 20) / 3, (720 / 2 - rect.h / 2 + 8) / 3, &itemRect, 0);
					render->scale = 1;

					render->DrawText(menuFont2, "Locked", 1280 / 2 - rect.w / 2 - rect.w + (rect.w * 2) * i + 18, 720 / 2 - rect.h / 2 + 117, 25, 0, { 128,0,0,255 });
				}
			}

			SString defensesNames[3] = {"null", "null", "null"};
			int index = 0;
			for (int i = 0; i < entityManager->activatorList.Count(); i++)
			{
				if ((Entity*)entityManager->activatorList.At(i) != nullptr && ((Entity*)entityManager->activatorList.At(i)->data)->subtype == EntitySubtype::DEFENSE)
				{
					defensesNames[index] = ((Activator*)entityManager->activatorList.At(i)->data)->name;
					index++;
				}
			}

			bool locked = false;
			SString itemToPrint;
			itemToPrint.Create("null");

			switch (hover.x)
			{
			case 0:
				render->DrawText(menuFont2, "Basic defense", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 475 + 3, 50, 3, { 128,128,128,255 });
				render->DrawText(menuFont2, "Basic defense", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 475, 50, 3, { 255,255,255,255 });

				render->DrawText(menuFont2, "Sweet never hurts to alleviate the spiciness.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 128,128,128,255 });
				render->DrawText(menuFont2, "Sweet never hurts to alleviate the spiciness.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 255,255,255,255 });
				break;
			case 1:
				if (defensesNames[0] != "null") itemToPrint = defensesNames[0];
				else locked = true;
				break;
			case 2:
				if (defensesNames[1] != "null") itemToPrint = defensesNames[1];
				else locked = true;
				break;
			case 3:
				if (defensesNames[2] != "null") itemToPrint = defensesNames[2];
				else locked = true;
				break;
			}

			if (locked)
			{
				render->DrawText(menuFont2, "Locked", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 475 + 3, 50, 3, { 64,0,0,255 });
				render->DrawText(menuFont2, "Locked", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 475, 50, 3, { 128,0,0,255 });

				render->DrawText(menuFont2, "This defense is still locked...", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 64,0,0,255 });
				render->DrawText(menuFont2, "This defense is still locked...", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 128,0,0,255 });
				render->DrawText(menuFont2, "You need to find it if you want to use it.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 560 + 3, 40, 3, { 64,0,0,255 });
				render->DrawText(menuFont2, "You need to find it if you want to use it.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 560, 40, 3, { 128,0,0,255 });
			}

			if (itemToPrint != "null")
			{
				render->DrawText(menuFont2, itemToPrint.GetString(), 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 475 + 3, 50, 3, { 128,128,128,255 });
				render->DrawText(menuFont2, itemToPrint.GetString(), 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 475, 50, 3, { 255,255,255,255 });

				if (itemToPrint == "Sushi")
				{
					render->DrawText(menuFont2, "It heals a big amount of your life at once.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "It heals a big amount of your life at once.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 255,255,255,255 });
				}
				else if (itemToPrint == "Oniguiri")
				{
					render->DrawText(menuFont2, "It heals a constant amount of your life", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "It heals a constant amount of your life", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 255,255,255,255 });
					render->DrawText(menuFont2, "every turn.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 560 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "every turn.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 560, 40, 3, { 255,255,255,255 });
				}
				else if (itemToPrint == "Sashimi")
				{
					render->DrawText(menuFont2, "It increases by a 30 per cent your current", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 525 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "It increases by a 30 per cent your current", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 525, 40, 3, { 255,255,255,255 });
					render->DrawText(menuFont2, "defense amount.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10 + 3, 560 + 3, 40, 3, { 128,128,128,255 });
					render->DrawText(menuFont2, "defense amount.", 1280 / 2 - rect.w / 2 - rect.w * 2 - rect.w - 10, 560, 40, 3, { 255,255,255,255 });
				}
			}
		}
	}

	return true;
}

bool ScreenBattle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	// Unload textures
	tex->UnLoad(backgroundTex);
	tex->UnLoad(optionsBackgroundTex);
	tex->UnLoad(cast1);
	tex->UnLoad(enemyCast);
	tex->UnLoad(indicator);
	tex->UnLoad(playerDefense);
	tex->UnLoad(enemyDefense);
	tex->UnLoad(crossTex);

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

	for (int i = 0; i <= (minIndex - 1); ++i)
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

bool ScreenBattle::HoverUpdate(Input* input)
{
	if (input->GetControllerState())
		return false;
	else
	{
		// Getting the mouse position
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);
		SDL_Rect rect = { 237,171,108,108 };
		for (int i = 0; i < 4; i++)
		{
			if (mouseX > (1280 / 2 - rect.w / 2 - rect.w * 3 + (rect.w * 2) * i) &&
				mouseX < (1280 / 2 - rect.w / 2 - rect.w * 3 + (rect.w * 2) * i + rect.w) &&
				mouseY >(720 / 2 - rect.h / 2) && mouseY < (720 / 2 - rect.h / 2 + rect.h))
			{
				hover.x = i;
				return true;
			}
		}
	}

	return false;
}

void ScreenBattle::SetBackgroundMap(Textures* tex)
{
	if (backgroundTex != nullptr) tex->UnLoad(backgroundTex);

	if (sceneGameplay->GetCurrentMap() == MapType::CEMETERY)
	{
		backgroundTex = tex->Load("Textures/Scenes/bg_cemetery.jpg");
	}
	else if (sceneGameplay->GetCurrentMap() == MapType::DOTONBORI)
	{
		backgroundTex = tex->Load("Textures/Scenes/bg_dotonbori.jpg");
	}
	else if (sceneGameplay->GetCurrentMap() == MapType::KANAGAWA)
	{
		backgroundTex = tex->Load("Textures/Scenes/bg_kanagawa.jpg");
	}
	else if (sceneGameplay->GetCurrentMap() == MapType::OSAKA)
	{
		backgroundTex = tex->Load("Textures/Scenes/bg_osaka.jpg");
	}
	else if (sceneGameplay->GetCurrentMap() == MapType::SKYSCRAPER)
	{
		backgroundTex = tex->Load("Textures/Scenes/bg_dungeon.jpg");
	}
	else if (sceneGameplay->GetCurrentMap() == MapType::TOWN)
	{
		backgroundTex = tex->Load("Textures/Scenes/bg_town.jpg");
	}
}