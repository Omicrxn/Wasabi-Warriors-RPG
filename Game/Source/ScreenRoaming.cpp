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
	return true;
	
}

bool ScreenRoaming::Draw(Render* render)
{
	// Draw anything extra needed in the hud
	if (currentPlayer != nullptr)
	{
		//int y = currentPlayer->spritePos * 32 * 5;
		//SDL_Rect rect = { 0, y , 32, 32 };
		//// Draw current player
		//render->scale = 2;
		//render->DrawRectangle({ 1096, 78, 70, 70 }, { 255,255,255,127 }, true, false);
		//render->DrawRectangle({ 1096, 78, 70, 70 }, { 255,255,255,255 }, false, false);
		//render->DrawTexture(entityManager->texture, 550, 40, &rect, 0.0f);
		//render->scale = 1;
		for (int i = 0; i < entityManager->playerList.Count(); ++i)
		{
			int y = entityManager->playerList.At(i)->data->spritePos * 32 * 5;
			SDL_Rect rect = { 0, y , 32, 32 };
			// Draw current player
			render->scale = 2;

			if (entityManager->playerList.At(i)->data == currentPlayer)
				render->DrawRectangle({ 1096 + i * 80, 78, 70, 70 }, { 255,255,255,127 }, true, false);

			render->DrawRectangle({ 1096 + i * 80, 78, 70, 70 }, { 255,255,255,255 }, false, false);
			render->DrawTexture(entityManager->texture, 550 + i * 40, 40, &rect, 0.0f);
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
	currentPlayer = player;
}
