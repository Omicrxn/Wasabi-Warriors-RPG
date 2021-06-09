#include "ScreenRoaming.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"
#include "EntityManager.h"
#include "Player.h"
#include "SceneGameplay.h"

ScreenRoaming::ScreenRoaming()
{
	currentPlayer = nullptr;

	gameplayScene = nullptr;

	controller = false;

	iconPhone = { 20, 14, 52, 66 };
	iconInventory = { 84, 16, 60, 62 };
	notificationRect = { 278, 12, 198, 34 };

	/*LBButton = { 434, 253, 90, 62 };
	RBButton = { 534, 253, 90, 62 };*/
	playersIcons = { 387, 234, 90, 88 };
	
	posRight = { 1100, 40 };
	posLeft = { 920, 40 };

	for (int i = 0; i < 4; ++i)
	{
		playerMiniRectPos[i] = posRight;
	}

	// Important for these to be -1
	previousSelected = -1;
	currentSelected = -1;

	positionX = 50;

	clickFx = -1;
	mobileNotFx = -1;
}

ScreenRoaming::~ScreenRoaming()
{
}

bool ScreenRoaming::Load(SceneGameplay* gameplayScene, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int clickFx)
{
	this->atlas[0] = atlas0;
	this->atlas[1] = atlas1;

	this->font = font;

	this->gameplayScene = gameplayScene;
	this->guiManager = guiManager;
	this->audio = audio;
	this->entityManager = entityManager;
	this->easing = easing;

	this->clickFx = clickFx;

	this->mobileNotFx = audio->LoadFx("Audio/Fx/mobile_notification.ogg");

	return true;
}

bool ScreenRoaming::Update(Input* input, float dt)
{
	// Update anything extra in the hud like the party member change
	controller = input->GetControllerState();

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_START) == KeyState::KEY_DOWN)
	{
		guiManager->ToggleMouse();
		gameplayScene->OpenPause();
	}

	if (input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_X) == KeyState::KEY_DOWN)
	{
		audio->PlayFx(clickFx);

		guiManager->ToggleMouse();
		gameplayScene->OpenInventory();
	}

	if (input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN)
	{
		Notifier::GetInstance()->SendMobileNotification("New party member added!");
	}
	if (input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_DOWN)
	{
		Notifier::GetInstance()->SendMobileNotification("New quest and party member added!");
	}

	return true;
}

bool ScreenRoaming::Draw(Render* render)
{
	// HUD Draw
	render->DrawTexture(atlas[0], positionX, 50, &iconPhone, 0.0f);
	render->DrawTexture(atlas[0], positionX + 120, 50, &iconInventory, 0.0f);

	if (!controller)
	{
		render->DrawText(font, "ESC", positionX + 3, 20, 30, 3, { 255,255,255,255 });
		render->DrawText(font, "Q", positionX + 120 + 55, 20, 35, 3, { 255,255,255,255 });
	}
	else
	{
		render->DrawText(font, "START", positionX - 8, 20, 28, 2, { 255,255,255,255 });
		render->DrawText(font, "X", positionX + 120 + 55, 20, 35, 3, { 0,0,255,255 });
	}

	if (Notifier::GetInstance()->GetMobileNotification() != nullptr)
	{
		if (Notifier::GetInstance()->GetNotificationTimer().ReadSec() <= 0.5f)
			audio->PlayFx(mobileNotFx);

		if (Notifier::GetInstance()->GetNotificationTimer().ReadSec() <= 5.0f)
		{
			SString not = Notifier::GetInstance()->GetMobileNotification();
			render->scale = 2;
			render->DrawTexture(atlas[0], (positionX + 20) / render->scale, (50 + 80) / render->scale, &notificationRect, 0.0f);
			render->scale = 1;
			render->DrawText(font, not.GetString(), positionX + 30, 50 + 110, 20, 2, { 255,0,0,255 });
		}
	}

	posRight = { 1100, 40 };
	posLeft = { 920, 40 };

	// Draw anything extra needed in the hud
	render->DrawTexture(this->atlas[1], 1000, 40, &playersIcons, 0.0f);

	// LB & RB buttons sprite draw
	if (controller)
	{
		/*render->DrawTexture(this->atlas[0], posLeft.x - 40, posLeft.y, &LBButton, 0.0f);
		render->DrawTexture(this->atlas[0], posRight.x + 40, posRight.y, &RBButton, 0.0f);*/
	}

	// Player Selection
	if (currentPlayer != nullptr)
	{
		int spacing = 0;
		for (int i = 0; i < entityManager->playerList.Count(); ++i)
		{
			int y = entityManager->playerList.At(i)->data->spritePos * 32 * 5;
			SDL_Rect rect = { 0, y , 32, 32 };
			render->scale = 2;
			if (entityManager->playerList.At(i)->data == currentPlayer)
			{
				// Draw current player with higlight
				render->DrawRectangle({ playerMiniRectPos[i].x, playerMiniRectPos[i].y, 70, 70 }, { 255,255,255,127 }, true, false);

				render->DrawRectangle({ playerMiniRectPos[i].x, playerMiniRectPos[i].y, 70, 70 }, { 255,255,255,255 }, false, false);
				render->DrawTexture(entityManager->texture, playerMiniRectPos[i].x / 2 + 2, (playerMiniRectPos[i].y) / 2 + 2, &rect, 0.0f);

			}
			else
			{
				// Draw other players without higlight
				render->DrawRectangle({ playerMiniRectPos[i].x + spacing, playerMiniRectPos[i].y, 70, 70 }, { 255,255,255,255 }, false, false);
				render->DrawTexture(entityManager->texture, playerMiniRectPos[i].x / 2 + 2 + spacing, (playerMiniRectPos[i].y) / 2 + 2, &rect, 0.0f);
				spacing += 4;
			}
			render->scale = 1;
		}
	}

	return true;
}

bool ScreenRoaming::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{

	return true;
}

void ScreenRoaming::SetCurrentPlayer(Player* player)
{
	// Set the new currentPlayer
	currentPlayer = player;
	// Only the first time do this spline
	if (currentSelected == -1)
	{
		currentSelected = entityManager->playerList.Find(player);
		easing->CreateSpline(&playerMiniRectPos[currentSelected].x, posLeft.x, 1000, SplineType::BACK);
		previousSelected = currentSelected;
	}
	else // Every else do these splines
	{
		currentSelected = entityManager->playerList.Find(player);
		easing->CreateSpline(&playerMiniRectPos[currentSelected].x, posLeft.x, 1000, SplineType::BACK);
		easing->CreateSpline(&playerMiniRectPos[previousSelected].x, posRight.x, 1000, SplineType::BACK);
		previousSelected = currentSelected;
	}
}

void ScreenRoaming::Enable()
{
	isActive = true;
	easing->CreateSpline(&positionX, 50, 1500, SplineType::BACK);
}

void ScreenRoaming::Disable()
{
	isActive = false;
	positionX = 2000;
}