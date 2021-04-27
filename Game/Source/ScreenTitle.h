#include "Screen.h"

class ScreenTitle : public Screen
{
public:
	ScreenTitle();
	virtual ~ScreenTitle();

	bool Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	GuiButton* btnStart;
	GuiButton* btnContinue;
	GuiButton* btnOptions;
	GuiButton* btnCredits;
	GuiButton* btnExit;

	SDL_Rect mainTitlesRect;
	iPoint titlePosition;
};