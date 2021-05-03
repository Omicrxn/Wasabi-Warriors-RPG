#include "ScreenPause.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"

ScreenPause::ScreenPause()
{
	iconResume = nullptr;
	iconSettings = nullptr;
	iconExit = nullptr;

	pauseTitleRect = { 0, 73, 440, 75 };
}

ScreenPause::~ScreenPause()
{
}

bool ScreenPause::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->atlas[1] = atlas1;
	this->font = font;

	this->guiManager = guiManager;
	this->win = win;

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	iconResume = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 440 - 65 / 2, 300, 65, 55 });
	iconResume->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_START, IconType::ICON_RESUME);
	iconResume->state = GuiControlState::HIDDEN;
	++counterId;

	iconSettings = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 640 - 58 / 2, 300, 58, 55 });
	iconSettings->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_X, IconType::ICON_SETTINGS);
	iconSettings->state = GuiControlState::HIDDEN;
	++counterId;

	iconExit = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 840 - 46 / 2, 300, 46, 55 });
	iconExit->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_B, IconType::ICON_EXIT);
	iconExit->state = GuiControlState::HIDDEN;

	return true;
}

bool ScreenPause::Update(Input* input, float dt, uint& focusedButtonId)
{
	return true;
}

bool ScreenPause::Draw(Render* render)
{
	// Draw pause background & title
	render->DrawRectangle({ 0,0,1280,720 }, { 0, 0, 0, 100 }, true, false);
	//render->DrawTexture(guiAtlasTex, 1280 / 2 - pauseBackgroundRect.w / 2, 720 / 2 - pauseBackgroundRect.h / 2, &pauseBackgroundRect, 0.0f);
	render->DrawTexture(atlas[1], 1280 / 2 - pauseTitleRect.w / 2, 100, &pauseTitleRect, 0.0f);
	return true;
}

bool ScreenPause::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	guiManager->DestroyGuiControl(iconResume);
	iconResume = nullptr;
	guiManager->DestroyGuiControl(iconSettings);
	iconSettings = nullptr;
	guiManager->DestroyGuiControl(iconExit);
	iconExit = nullptr;

	return true;
}