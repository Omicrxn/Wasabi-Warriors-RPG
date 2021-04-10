#include "SceneGameplay.h"

#include "EntityManager.h"
#include "BattleSystem.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "Window.h"
#include "DialogSystem.h"

SceneGameplay::SceneGameplay()
{
	type = SceneType::GAMEPLAY;
	this->name = "scenegameplay";

	// Needed modules
	entityManager = nullptr;
	guiManager = nullptr;
	win = nullptr;
	battleSystem = new BattleSystem();

	map = nullptr;
	currentPlayer = nullptr;

	camera = { 0,0,1280,720 };

	spritesheet = nullptr;

	// Battle system bool
	battle = false;

	// Buttons
	btnAttack = nullptr;
	btnDefend = nullptr;
	btnItem = nullptr;
	btnRun = nullptr;
	btnNone = nullptr;

	// Fonts
	titleFont = nullptr;
	buttonFont = nullptr;

	// Battle system textures
	backgroundTex = nullptr;
	backgroundRect = { 0, 0, 0, 0 };
	guiAtlasTex = nullptr;
	spritesheet = nullptr;

	// Buttons management
	hoverFx = -1;
	clickFx = -1;

	// Gamepad's menu focused button
	controllerFocus = 0;
}

SceneGameplay::~SceneGameplay()
{
}

bool SceneGameplay::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem)
{
	// Needed modules
	this->entityManager = entityManager;
	this->guiManager = guiManager;
	this->win = win;
	this->dialogSystem = dialogSystem;

	// Create map
	map = (Map*)entityManager->CreateEntity(EntityType::MAP, "Map");

	// L03: DONE: Load map
	// L12b: Create walkability map on map loading
	if (map->Load("Cemetery", "Cemetery.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	// Load textures
	spritesheet = tex->Load("Assets/Textures/Characters/characters_spritesheet.png");
	backgroundTex = tex->Load("Assets/Textures/Scenes/battle_scene.jpg"); backgroundRect = { 0, 0, 1280, 720 };
	guiAtlasTex = tex->Load("Assets/Textures/UI/Elements/ui_spritesheet.png");

	// Create fonts
	titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
	buttonFont = new Font("Assets/Fonts/showg.xml", tex);

	// Load music and Fx
	audio->PlayMusic("Assets/Audio/Music/menu.ogg");
	// AudioManager::PlayMusic("Assets/Audio/Music/music_spy.ogg");
	hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
	clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

	// Get window info
	/*uint width, height;
	win->GetWindowSize(width, height);*/
	
	// Create party member 1
	Entity* entity;
	entity = entityManager->CreateEntity(EntityType::PLAYER,"DaBaby");
	entity->position = iPoint(12 * 32, 6 * 32);
	entity->SetTexture(spritesheet, 3);
	entity->SetState(true);
	entity = nullptr;
	currentPlayer = entityManager->playerList.At(0)->data;
	//// Create party member 2
	//entity = entityManager->CreateEntity(EntityType::PLAYER, "DaCrack");
	//entity->position = iPoint(10 * 32, 6 * 32);
	//entity->SetTexture(spritesheet, 6);
	//entity = nullptr;
	// Create enemy
	entity = entityManager->CreateEntity(EntityType::ENEMY, "DaBoss");
	entity->position = iPoint(10 * 32, 6 * 32);
	entity->SetTexture(spritesheet, 5);
	entity = nullptr;
	// Create NPC
	entity = entityManager->CreateEntity(EntityType::NPC, "DaBot");
	entity->position = iPoint(10 * 32, 6 * 32);
	entity->SetTexture(spritesheet, 4);
	entity = nullptr;

	// Load buttons for the battle system
	btnAttack = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 730, 500, 190, 49 }, "ATTACK");
	btnAttack->SetObserver(this);
	btnAttack->SetTexture(guiAtlasTex);
	btnAttack->SetFont(buttonFont);
	btnAttack->SetButtonAudioFx(hoverFx, clickFx);
	btnAttack->state = GuiControlState::DISABLED;

	btnDefend = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { 730, 600, 190, 49 }, "DEFEND");
	btnDefend->SetObserver(this);
	btnDefend->SetTexture(guiAtlasTex);
	btnDefend->SetFont(buttonFont);
	btnDefend->SetButtonAudioFx(hoverFx, clickFx);
	btnDefend->state = GuiControlState::DISABLED;

	btnItem = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { 1030, 500, 190, 49 }, "ITEM");
	btnItem->SetObserver(this);
	btnItem->SetTexture(guiAtlasTex);
	btnItem->SetFont(buttonFont);
	btnItem->SetButtonAudioFx(hoverFx, clickFx);
	btnItem->state = GuiControlState::DISABLED;

	btnRun = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { 1030, 600, 190, 49 }, "RUN");
	btnRun->SetObserver(this);
	btnRun->SetTexture(guiAtlasTex);
	btnRun->SetFont(buttonFont);
	btnRun->SetButtonAudioFx(hoverFx, clickFx);
	btnRun->state = GuiControlState::DISABLED;

	//btnNone = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { 700, 500, 190, 49 }, "NONE");
	//btnNone->SetObserver(this);
	//btnNone->SetTexture(guiAtlasTex);
	//btnNone->SetFont(buttonFont);
	//btnNone->SetButtonAudioFx(hoverFx, clickFx);
	//btnNone->state = GuiControlState::DISABLED;

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

	if (input->GetKey(SDL_SCANCODE_B) == KEY_DOWN && battle == false)
	{
		battle = true;
		battleSystem->SetupBattle(&entityManager->playerList, entityManager->enemyList.start->data);

		btnAttack->state = GuiControlState::NORMAL;
		btnDefend->state = GuiControlState::NORMAL;
		btnItem->state = GuiControlState::NORMAL;
		btnRun->state = GuiControlState::NORMAL;
		/*btnNone->state = GuiControlState::NORMAL;*/
	}
	for (int i = 0; i < entityManager->enemyList.Count(); i++)
	{
		if (entityManager->enemyList.At(i)->data->inCombat)
		{
			battle = true;
			battleSystem->SetupBattle(&entityManager->playerList, entityManager->enemyList.start->data);

			btnAttack->state = GuiControlState::NORMAL;
			btnDefend->state = GuiControlState::NORMAL;
			btnItem->state = GuiControlState::NORMAL;
			btnRun->state = GuiControlState::NORMAL;
			/*btnNone->state = GuiControlState::NORMAL;*/
			entityManager->enemyList.At(i)->data->inCombat = false;
		}
	}

	if (input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		dialogSystem->NewDialog();
	}

	if (battle == true)
	{
		if (input->pads[0].enabled)
		{
			/* Input */
			if (((input->pads[0].up || input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)) && controllerFocus >= 1)
				--controllerFocus;
			else if (((input->pads[0].down || input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)) && controllerFocus <= 3)
				++controllerFocus;

			for (int i = 0; i < 5; ++i)
			{
				if (i != controllerFocus)
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

	// Unload Fx
	/*audio.Unload(hoverFx);
	audio.Unload(clickFx);*/

	// Destory GUI Controls
	guiManager->DestroyGuiControl(btnAttack);
	guiManager->DestroyGuiControl(btnDefend);
	guiManager->DestroyGuiControl(btnItem);
	guiManager->DestroyGuiControl(btnRun);
	guiManager->DestroyGuiControl(btnNone);

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
		if (control->id == 1) battleSystem->playerState = PlayerState::ATTACK;
		else if (control->id == 2) battleSystem->playerState = PlayerState::DEFEND;
		else if (control->id == 3) battleSystem->playerState = PlayerState::ITEM;
		else if (control->id == 4)
		{
			battleSystem->playerState = PlayerState::RUN;
			ExitBattle();
		}
		else if (control->id == 5) battleSystem->playerState = PlayerState::NONE;
		break;
	}
	default: break;
	}

	return true;
}

void SceneGameplay::ExitBattle()
{
	btnAttack->state = GuiControlState::DISABLED;
	btnDefend->state = GuiControlState::DISABLED;
	btnItem->state = GuiControlState::DISABLED;
	btnRun->state = GuiControlState::DISABLED;
	/*btnNone->state = GuiControlState::DISABLED;*/
	battle = false;
	battleSystem->ResetBattle();
}
