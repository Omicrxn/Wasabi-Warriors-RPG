#include "Screen.h"

class Player;

class ScreenRoaming : public Screen
{
public:
	ScreenRoaming();
	~ScreenRoaming();

	bool Load(SceneGameplay* gameplayScene, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, Font* font, int clickFx);

	bool Update(Input* input, float dt);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	void SetCurrentPlayer(Player* player);

	void Enable();
	void Disable();

	SceneGameplay* gameplayScene;

	Player* currentPlayer;

	iPoint playerMiniRectPos[4];
	iPoint posRight;
	iPoint posLeft;

	SDL_Rect iconPhone;
	SDL_Rect iconInventory;

	SDL_Rect LBButton;
	SDL_Rect RBButton;
	SDL_Rect playersIcons;

	int previousSelected;
	int currentSelected;

	int positionX;

private:

	bool controller;

	int clickFx;
};