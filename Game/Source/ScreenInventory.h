#ifndef __SCREENINVENTORY_H__
#define __SCREENINVENTORY_H__

#include "Screen.h"
#include "Item.h"

class Player;

#define INVENTORY_ROWS 2
#define INVENTORY_COLUMNS 3

class ScreenInventory : public Screen
{
public:
	ScreenInventory();
	~ScreenInventory();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);

	bool Draw(Render* render);

	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	
	void SetHasClickedConsume(bool value);
	bool GetHasClickedConsume(bool value);
	void ManageItemConsumption();
	void ConsumeItem(Player* currentPlayer, InvItem* inventoryItem);

	void Enable(bool isFromBattle = false);
	void Disable();

	List<InvItem*> listInvItems;

private:

	iPoint itemHovering;
	iPoint itemSelected;

	GuiButton* btnConfirm;
	GuiButton* btnCancel;

	SDL_Rect LBButton;
	SDL_Rect RBButton;
	SDL_Rect playersIcons;
	SDL_Rect pinkBox;
	SDL_Rect grayBox;

	iPoint invMatrixPos;

	bool controller;
	bool hasClickedConsume;

	bool hasSelectedItem;

	SDL_Rect bagRect;

	int positionX;
	int animIncrementY;

	Timer selectionGamepadTimer;
};

#endif // __SCREENINVENTORY_H__