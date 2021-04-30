#include "Screen.h"

class ScreenPause : public Screen
{
public:
	ScreenPause();
	~ScreenPause();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	GuiIcon* iconResume;
	GuiIcon* iconSettings;
	GuiIcon* iconExit;

	SDL_Rect pauseTitleRect;
};
