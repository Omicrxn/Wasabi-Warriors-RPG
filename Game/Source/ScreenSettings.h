#include "Screen.h"

class ScreenSettings : public Screen
{
public:
	ScreenSettings();
	virtual ~ScreenSettings();

	bool Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);
	
	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	GuiCheckBox* checkFullScreen;
	GuiCheckBox* checkVsync;

	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFXVolume;

	GuiIcon* iconReturnTitle;

	SDL_Rect settingsTitleRect;
	SDL_Rect settingsBackgroundRect;

	SDL_Rect iconARect;
	SDL_Rect dpadRect;
	SDL_Rect iconSTARTRect;
};
