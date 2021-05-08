#include "Screen.h"

class ScreenSettings : public Screen
{
public:
	ScreenSettings();
	~ScreenSettings();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);
	
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

private:
	SDL_Rect settingsTitleRect;
	SDL_Rect settingsBackgroundRect1;
	SDL_Rect settingsBackgroundRect2;
	SDL_Color settingsBackgroundColor1;
	SDL_Color settingsBackgroundColor2;

	SDL_Rect iconARect;
	SDL_Rect dpadRect;
	SDL_Rect iconSTARTRect;
};