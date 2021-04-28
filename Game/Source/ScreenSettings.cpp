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
	settingsBackgroundRect = { 1228, 295, 300, 200 };

	iconARect = { 684, 8, 57, 57 };
	dpadRect = { 665, 378, 96, 96 };
	iconSTARTRect = { 685, 291, 55, 54 };
}
ScreenSettings::~ScreenSettings() {}

bool ScreenSettings::Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* buttonFont, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->atlas[1] = atlas1;
	this->font = buttonFont;

	this->guiManager = guiManager;
	this->win = win;

	uint width, height;
	win->GetWindowSize(width, height);

	checkFullScreen = (GuiCheckBox*)guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 5, { (int)width / 2 - (int)((float)width / 3.5f) - 20, 194, 45, 49 }, "FULLSCREEN");
	checkFullScreen->SetCheckBoxProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);

	checkVsync = (GuiCheckBox*)guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 6, { (int)width / 2 - (int)((float)width / 3.5f) - 20, 294, 45, 49 }, "VSYNC");
	checkVsync->SetCheckBoxProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);

	sliderMusicVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, { (int)width / 2 - (int)((float)width / 12) + 5, 200, 300, 30 }, "MUSIC VOLUME");
	sliderMusicVolume->SetSliderProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	sliderMusicVolume->minValue = 0;
	sliderMusicVolume->maxValue = SDL_MIX_MAXVOLUME;

	sliderFXVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, 8, { (int)width / 2 - (int)((float)width / 12) + 5, 300, 300, 30 }, "FX VOLUME");
	sliderFXVolume->SetSliderProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx);
	sliderFXVolume->minValue = 0;
	sliderFXVolume->maxValue = SDL_MIX_MAXVOLUME;

	iconReturnTitle = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, 9, { (int)width / 2 + (int)((float)width / 4), 570, 70, 55 });
	iconReturnTitle->SetIconProperties(currentScene, atlas0, buttonFont, hoverFx, clickFx, CONTROLLER_BUTTON_B, IconType::ICON_RETURN);

	minIndex = 5;
	maxIndex = 9;

	return true;
}

bool ScreenSettings::Update(Input* input, float dt, uint& focusedButtonId)
{
	if (isActive)
	{
		//Update
		if (focusedButtonId == 5)
		{
			if (input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN)
				focusedButtonId = 6;
			else if (input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN)
				focusedButtonId = 7;
		}
		else if (focusedButtonId == 6)
		{
			if (input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN)
				focusedButtonId = 5;
			else if (input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN)
				focusedButtonId = 8;
		}
		else if (focusedButtonId == 7)
		{
			if (input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN)
				focusedButtonId = 8;
			else if (input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN)
				focusedButtonId = 5;
		}
		else if (focusedButtonId == 8)
		{
			if (input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN)
				focusedButtonId = 7;
			else if (input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN)
				focusedButtonId = 6;
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

		render->scale = 3;
		render->DrawTexture(atlas[0], 60, 25, &settingsBackgroundRect, 0.0f);
		render->scale = 1;

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

bool ScreenSettings::LoadState(pugi::xml_node& audio)
{
	this->sliderMusicVolume->SetSliderPosX(audio.attribute("sliderVolume").as_int());
	this->sliderFXVolume->SetSliderPosX(audio.attribute("sliderAudio").as_int());
	return false;
}

bool ScreenSettings::SaveState(pugi::xml_node& audio) const
{
	SString tempName = audio.attribute("sliderVolume").name();
	if (tempName == "sliderVolume")
	{
		// Attribute currentMap exists
		audio.attribute("sliderVolume").set_value(this->sliderMusicVolume->GetSlider().x);
	}
	else
	{
		// Attribute currentMap does not exist
		audio.append_attribute("sliderVolume").set_value(this->sliderMusicVolume->GetSlider().x);
	}
	tempName = audio.attribute("sliderAudio").name();
	if (tempName == "sliderAudio")
	{
		// Attribute currentMap exists
		audio.attribute("sliderAudio").set_value(this->sliderFXVolume->GetSlider().x);
	}
	else
	{
		// Attribute currentMap does not exist
		audio.append_attribute("sliderAudio").set_value(this->sliderFXVolume->GetSlider().x);
	}

	return false;
}

