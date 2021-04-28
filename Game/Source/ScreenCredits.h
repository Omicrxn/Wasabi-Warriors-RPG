#include "Screen.h"

class ScreenCredits : public Screen
{
public:
	ScreenCredits();
	~ScreenCredits();

	bool Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	SDL_Rect settingsBackgroundRect;
	SDL_Rect creditsTitleRect;
};