#include "Screen.h"

class ScreenPause : public Screen
{
public:
	ScreenPause();
	~ScreenPause();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	void Enable();
	void Disable();

	int position;

	GuiIcon* iconResume;
	GuiIcon* iconSettings;
	GuiIcon* iconExit;

	SDL_Rect pauseTitleRect;
	SDL_Rect mobileRect;
};
