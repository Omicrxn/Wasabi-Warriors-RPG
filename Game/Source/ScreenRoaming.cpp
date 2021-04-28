#include "ScreenRoaming.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"

ScreenRoaming::ScreenRoaming()
{
	iconPause = nullptr;
	iconInventory = nullptr;
	iconPhone = nullptr;
}

ScreenRoaming::~ScreenRoaming()
{
}

bool ScreenRoaming::Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->font = font;

	this->guiManager = guiManager;
	this->win = win;

	iconPause = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, 0, { 50, 50, 52, 55 });
	iconPause->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_START, IconType::ICON_PAUSE);

	iconInventory = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, 1, { 170, 50, 56, 55 });
	iconInventory->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_X, IconType::ICON_INVENTORY);

	iconPhone = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, 2, { 290, 50, 41, 55 });
	iconPhone->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_B, IconType::ICON_PHONE);

	this->minIndex = 0;
	this->maxIndex = 2;

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
