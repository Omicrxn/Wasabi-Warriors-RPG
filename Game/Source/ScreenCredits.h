#include "Screen.h"

class ScreenCredits : public Screen
{
public:
	ScreenCredits();
	~ScreenCredits();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

private:
	SDL_Rect creditsTitleRect;
	SDL_Rect creditsBackgroundRect1;
	SDL_Rect creditsBackgroundRect2;

	SDL_Color creditsBackgroundColor1;
	SDL_Color creditsBackgroundColor2;
};