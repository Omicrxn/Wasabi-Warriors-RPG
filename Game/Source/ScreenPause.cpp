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
	mobileRect = { 375, 339, 392, 603 };

	position = 700;
}

ScreenPause::~ScreenPause()
{
}

bool ScreenPause::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->atlas[1] = atlas1;
	this->font = font;
	this->easing = easing;

	this->guiManager = guiManager;
	this->win = win;

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	iconResume = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 609, position + 580, 65, 55 });
	iconResume->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_START, IconType::ICON_RESUME);
	iconResume->state = GuiControlState::HIDDEN;
	++counterId;

	iconSettings = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 490, position + 108, 58, 55 });
	iconSettings->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_X, IconType::ICON_SETTINGS);
	iconSettings->state = GuiControlState::HIDDEN;
	++counterId;

	iconExit = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 750, position + 108, 46, 55 });
	iconExit->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_B, IconType::ICON_EXIT);
	iconExit->state = GuiControlState::HIDDEN;

	//iconResume->bounds.x

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
	// title : render->DrawTexture(atlas[1], 1280 / 2 - pauseTitleRect.w / 2, 100, &pauseTitleRect, 0.0f);
	render->DrawTexture(atlas[0], (1280 / 2 - mobileRect.w / 2) / render->scale, position / render->scale , &mobileRect, 0.0f);
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

void ScreenPause::Enable()
{
	easing->CreateSpline(&position, 50, 1500, SplineType::QUINT);
	easing->CreateSpline(&iconSettings->bounds.y, 108, 1500, SplineType::QUINT);
	easing->CreateSpline(&iconExit->bounds.y, 108, 1500, SplineType::QUINT);
	easing->CreateSpline(&iconResume->bounds.y, 580, 1500, SplineType::QUINT);
	Screen::Enable();
}

void ScreenPause::Disable()
{
	position = 700;
	iconSettings->bounds.y = position + 108;
	iconExit->bounds.y = position + 108;
	iconResume->bounds.y = position + 580;
	Screen::Disable();
}
