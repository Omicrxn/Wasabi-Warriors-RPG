#include "ScreenRoaming.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"
#include "EntityManager.h"
#include "Player.h"

ScreenRoaming::ScreenRoaming()
{
	iconPause = nullptr;
	iconInventory = nullptr;
	iconPhone = nullptr;
	currentPlayer = nullptr;

	posRight = { 1096 + 50, 78 + 22 };
	posLeft = { 1000 + 50, 78 + 22 };

	LBButton = { 434, 253, 90, 62 };
	RBButton = { 534, 253, 90, 62 };
	playersIcons = { 769,220, 81, 56};

	for (int i = 0; i < 4; ++i)
	{
		playerMiniRectPos[i] = posRight;
	}

	// Important for these to b1 -1
	previousSelected = -1;
	currentSelected = -1;
}

ScreenRoaming::~ScreenRoaming()
{
}

bool ScreenRoaming::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->font = font;

	this->guiManager = guiManager;
	this->entityManager = entityManager;
	this->win = win;
	this->easing = easing;

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex; 

	iconPause = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 50, 50, 52, 55 });
	iconPause->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_START, IconType::ICON_PAUSE);
	++counterId;

	iconInventory = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 170, 50, 56, 55 });
	iconInventory->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_X, IconType::ICON_INVENTORY);
	++counterId;

	iconPhone = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 290, 50, 41, 55 });
	iconPhone->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_B, IconType::ICON_PHONE);

	return true;
}

bool ScreenRoaming::Update(Input* input, float dt, uint& focusedButtonId)
{
	// Update anything extra in the hud like the party member change
	controller = input->GetControllerState();
	return true;
}

bool ScreenRoaming::Draw(Render* render)
{
	posRight = { 1096 + 50, 78 + 22 };
	posLeft = { 1000 + 50, 78 + 22 };

	// Draw anything extra needed in the hud
	render->DrawTexture(this->atlas[0], posLeft.x + 50, posLeft.y - 70, &playersIcons, 0.0f);
	// LB & RB buttons sprite draw
	if (1)
	{
		render->DrawTexture(this->atlas[0], posLeft.x - 40, posLeft.y - 70, &LBButton, 0.0f);
		render->DrawTexture(this->atlas[0], posRight.x + 40, posRight.y - 70, &RBButton, 0.0f);
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
			if (entityManager->playerList.At(i)->data == currentPlayer) {

				// Draw current player with higlight
				render->DrawRectangle({ playerMiniRectPos[i].x, playerMiniRectPos[i].y, 70, 70 }, { 255,255,255,127 }, true, false);

				render->DrawRectangle({ playerMiniRectPos[i].x, playerMiniRectPos[i].y, 70, 70 }, { 255,255,255,255 }, false, false);
				render->DrawTexture(entityManager->texture, playerMiniRectPos[i].x / 2 + 2, playerMiniRectPos[i].y / 2 + 2, &rect, 0.0f);
				
			}
			else {
				// Draw other players without higlight
				render->DrawRectangle({ playerMiniRectPos[i].x + spacing, playerMiniRectPos[i].y, 70, 70 }, { 255,255,255,255 }, false, false);
				render->DrawTexture(entityManager->texture, playerMiniRectPos[i].x / 2 + 2 + spacing, playerMiniRectPos[i].y / 2 + 2, &rect, 0.0f);
				spacing += 4;
			}
			render->scale = 1;
		}
	}

	return true;
}

bool ScreenRoaming::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	guiManager->DestroyGuiControl(iconPause);
	iconPause = nullptr;
	guiManager->DestroyGuiControl(iconInventory);
	iconInventory = nullptr;
	guiManager->DestroyGuiControl(iconPhone);
	iconPhone = nullptr;
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
