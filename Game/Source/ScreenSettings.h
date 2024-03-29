#include "Screen.h"

class ScreenSettings : public Screen
{
public:
	ScreenSettings();
	~ScreenSettings();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx, int returnFx);
	
	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	void SetGameplaySettings(bool isGameplay);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	GuiCheckBox* checkFullScreen;
	GuiCheckBox* checkVsync;

	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFXVolume;

	GuiIcon* iconReturnTitle;

private:
	SDL_Rect mobileRect;

	bool isGameplaySettings;
};