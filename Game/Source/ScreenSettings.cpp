#include "ScreenSettings.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"

ScreenSettings::ScreenSettings()
{
	checkFullScreen = nullptr;
	checkVsync = nullptr;
	sliderMusicVolume = nullptr;
	sliderFXVolume = nullptr;
	iconReturnTitle = nullptr;

	mobileRect = { 375, 339, 392, 603 };
}

ScreenSettings::~ScreenSettings() {}

bool ScreenSettings::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* buttonFont, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->atlas[1] = atlas1;
	this->font = buttonFont;

	this->guiManager = guiManager;
	this->win = win;

	uint width, height;
	win->GetWindowSize(width, height);

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	checkFullScreen = (GuiCheckBox*)guiManager->CreateGuiControl(GuiControlType::CHECKBOX, counterId, { (int)width / 2 - (int)((float)width / 10), 150, 45, 49 }, "FULLSCREEN");
	checkFullScreen->SetCheckBoxProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	++counterId;

	checkVsync = (GuiCheckBox*)guiManager->CreateGuiControl(GuiControlType::CHECKBOX, counterId, { (int)width / 2 - (int)((float)width / 10), 250, 45, 49 }, "VSYNC");
	checkVsync->SetCheckBoxProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	++counterId;

	sliderMusicVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, counterId, { (int)width / 2 - (int)((float)width / 10), 350, 261, 51 }, "MUSIC VOLUME");
	sliderMusicVolume->SetSliderProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	sliderMusicVolume->minValue = 0;
	sliderMusicVolume->maxValue = SDL_MIX_MAXVOLUME;
	++counterId;

	sliderFXVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, counterId, { (int)width / 2 - (int)((float)width / 10), 450, 261, 51 }, "FX VOLUME");
	sliderFXVolume->SetSliderProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	sliderFXVolume->minValue = 0;
	sliderFXVolume->maxValue = SDL_MIX_MAXVOLUME;
	++counterId;

	iconReturnTitle = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 609, 580, 54, 54 });
	iconReturnTitle->SetIconProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx, IconType::ICON_RETURN);

	return true;
}

bool ScreenSettings::Update(Input* input, float dt, uint& focusedButtonId)
{
	if ((input->GetControllerButton(CONTROLLER_BUTTON_UP) == KeyState::KEY_DOWN || input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN) && focusedButtonId > minIndex)
		--focusedButtonId;
	else if ((input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KeyState::KEY_DOWN || input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN) && focusedButtonId < maxIndex)
		++focusedButtonId;

	UpdateControllerSelection(focusedButtonId);

	return true;
}

bool ScreenSettings::Draw(Render* render)
{
	render->DrawRectangle({ 0,0,1280,720 }, { 0, 0, 0, 70 }, true, false);

	render->DrawTexture(atlas[0], (1280 / 2 - mobileRect.w / 2) / render->scale, 50 / render->scale, &mobileRect, 0.0f);

	return true;
}

bool ScreenSettings::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	guiManager->DestroyGuiControl(checkFullScreen);
	guiManager->DestroyGuiControl(checkVsync);
	guiManager->DestroyGuiControl(sliderMusicVolume);
	guiManager->DestroyGuiControl(sliderFXVolume);

	guiManager->DestroyGuiControl(iconReturnTitle);

	return true;
}

bool ScreenSettings::LoadState(pugi::xml_node& screen)
{
	pugi::xml_node screenSettingsNode = screen.child("screensettings");

	this->sliderMusicVolume->SetSliderPosX(screenSettingsNode.attribute("sliderVolume").as_int());
	this->sliderFXVolume->SetSliderPosX(screenSettingsNode.attribute("sliderAudio").as_int());

	bool fullscreen = screenSettingsNode.attribute("fullScreen").as_bool();
	this->checkFullScreen->SetCheck(fullscreen);
	
	bool vsync = screenSettingsNode.attribute("vsync").as_bool();
	this->checkVsync->SetCheck(vsync);
	
	return false;
}

bool ScreenSettings::SaveState(pugi::xml_node& screen) const
{
	// Check screensettings node
	pugi::xml_node screenSettingsNode;
	SString tempName = screen.child("screensettings").name();
	if (tempName == "screensettings")
	{
		screenSettingsNode = screen.child("screensettings");
	}
	else
	{
		screenSettingsNode = screen.append_child("screensettings");
	}

	// Check sliderVolume attribute
	tempName = screenSettingsNode.attribute("sliderVolume").name();
	if (tempName == "sliderVolume")
	{
		screenSettingsNode.attribute("sliderVolume").set_value(this->sliderMusicVolume->GetSlider().x);
	}
	else
	{
		screenSettingsNode.append_attribute("sliderVolume").set_value(this->sliderMusicVolume->GetSlider().x);
	}

	// Check sliderAudio attribute
	tempName = screenSettingsNode.attribute("sliderAudio").name();
	if (tempName == "sliderAudio")
	{
		screenSettingsNode.attribute("sliderAudio").set_value(this->sliderFXVolume->GetSlider().x);
	}
	else
	{
		screenSettingsNode.append_attribute("sliderAudio").set_value(this->sliderFXVolume->GetSlider().x);
	}

	// Check fullscreen attribute
	tempName = screenSettingsNode.attribute("fullScreen").name();
	if (tempName == "fullScreen")
	{
		screenSettingsNode.attribute("fullScreen").set_value(this->checkFullScreen->GetCheck());
	}
	else
	{
		screenSettingsNode.append_attribute("fullScreen").set_value(this->checkFullScreen->GetCheck());
	}

	// Check vsync attribute
	tempName = screenSettingsNode.attribute("vsync").name();
	if (tempName == "vsync")
	{
		screenSettingsNode.attribute("vsync").set_value(this->checkVsync->GetCheck());
	}
	else
	{
		screenSettingsNode.append_attribute("vsync").set_value(this->checkVsync->GetCheck());
	}

	return false;
}