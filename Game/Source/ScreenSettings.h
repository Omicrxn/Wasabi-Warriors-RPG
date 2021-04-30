#include "Screen.h"

class ScreenSettings : public Screen
{
public:
	ScreenSettings();
	~ScreenSettings();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);
	
	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

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
