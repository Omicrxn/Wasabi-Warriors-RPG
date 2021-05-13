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

	settingsTitleRect = { 0, 149, 530, 81 };

	settingsBackgroundColor1 = { 74,79,95,255 };
	settingsBackgroundColor2 = { 128,137,154,255 };

	iconARect = { 684, 8, 57, 57 };
	dpadRect = { 665, 378, 96, 96 };
	iconSTARTRect = { 685, 291, 55, 54 };
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

	settingsBackgroundRect1 = { int(width) / 2 - 500, 75, 1000, 600 };
	settingsBackgroundRect2 = { settingsBackgroundRect1.x + 25, settingsBackgroundRect1.y + 25, settingsBackgroundRect1.w - 50, settingsBackgroundRect1.h - 50 };

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	checkFullScreen = (GuiCheckBox*)guiManager->CreateGuiControl(GuiControlType::CHECKBOX, counterId, { (int)width / 2 - (int)((float)width / 3.5f) - 20, 194, 45, 49 }, "FULLSCREEN");
	checkFullScreen->SetCheckBoxProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	++counterId;

	checkVsync = (GuiCheckBox*)guiManager->CreateGuiControl(GuiControlType::CHECKBOX, counterId, { (int)width / 2 - (int)((float)width / 3.5f) - 20, 294, 45, 49 }, "VSYNC");
	checkVsync->SetCheckBoxProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	++counterId;

	sliderMusicVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, counterId, { (int)width / 2 - (int)((float)width / 12) + 5, 200, 300, 30 }, "MUSIC VOLUME");
	sliderMusicVolume->SetSliderProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	sliderMusicVolume->minValue = 0;
	sliderMusicVolume->maxValue = SDL_MIX_MAXVOLUME;
	++counterId;

	sliderFXVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, counterId, { (int)width / 2 - (int)((float)width / 12) + 5, 300, 300, 30 }, "FX VOLUME");
	sliderFXVolume->SetSliderProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	sliderFXVolume->minValue = 0;
	sliderFXVolume->maxValue = SDL_MIX_MAXVOLUME;
	++counterId;

	iconReturnTitle = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { (int)width / 2 + (int)((float)width / 4), 570, 70, 55 });
	iconReturnTitle->SetIconProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx, CONTROLLER_BUTTON_B, IconType::ICON_RETURN);

	return true;
}

bool ScreenSettings::Update(Input* input, float dt, uint& focusedButtonId)
{
	if (isActive)
	{
		// Update
		// MinIndex is 5
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
	}

	return true;
}

bool ScreenSettings::Draw(Render* render)
{
	if (isActive)
	{
		uint width, height;
		win->GetWindowSize(width, height);

		//render->scale = 3;
		//render->DrawTexture(atlas[0], 60, 25, &settingsBackgroundRect, 0.0f);
		//render->scale = 1;
		render->DrawRectangle(settingsBackgroundRect1, settingsBackgroundColor1, true, false);
		render->DrawRectangle(settingsBackgroundRect2, settingsBackgroundColor2, true, false);

		render->DrawTexture(atlas[1], width / 2 - settingsTitleRect.w / 2, 40, &settingsTitleRect, 0.0f);

		render->DrawTexture(atlas[0], 339, 429, &dpadRect, 0.0f);
		render->DrawText(font, "Movement", 339 + dpadRect.w + 15, 465, 25, 3, { 255,255,255,255 });
		render->DrawTexture(atlas[0], 633, 400, &iconARect, 0.0f);
		render->DrawText(font, "Interact", 633 + iconARect.w + 15, 415, 25, 3, { 255,255,255,255 });
		render->DrawTexture(atlas[0], 633, 505, &iconSTARTRect, 0.0f);
		render->DrawText(font, "Pause", 633 + iconSTARTRect.w + 17, 520, 25, 3, { 255,255,255,255 });
	}
	
	return true;
}

bool ScreenSettings::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	guiManager->DestroyGuiControl(checkFullScreen);
	checkFullScreen = nullptr;
	guiManager->DestroyGuiControl(checkVsync);
	checkVsync = nullptr;
	guiManager->DestroyGuiControl(sliderMusicVolume);
	sliderMusicVolume = nullptr;
	guiManager->DestroyGuiControl(sliderFXVolume);
	sliderFXVolume = nullptr;

	guiManager->DestroyGuiControl(iconReturnTitle);
	iconReturnTitle = nullptr;

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

