#include "Screen.h"

class Player;

class ScreenRoaming : public Screen
{
public:
	ScreenRoaming();
	~ScreenRoaming();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	void SetCurrentPlayer(Player* player);

	GuiIcon* iconPause;
	GuiIcon* iconInventory;
	GuiIcon* iconPhone;

	Player* currentPlayer;
};