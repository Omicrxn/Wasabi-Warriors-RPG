#include "Screen.h"

class ScreenMainMenu : public Screen
{
public:
	ScreenMainMenu();
	~ScreenMainMenu();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);

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