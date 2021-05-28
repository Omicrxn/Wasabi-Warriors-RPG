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

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);

	bool Draw(Render* render);

	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	
	void SetHasClickedConsume(bool value);
	void ConsumeItem(Player* currentPlayer, InvItem* inventoryItem);

	List<InvItem*> listInvItems;

private:
	//InventorySlot inventory[INVENTORY_ROWS][INVENTORY_COLUMNS];

	iPoint itemHovering;
	iPoint itemSelected;

	GuiButton* btnConfirm;
	GuiButton* btnCancel;

	SDL_Rect LBButton;
	SDL_Rect RBButton;
	SDL_Rect playersIcons;

	iPoint invMatrixPos;

	bool controller;
	bool hasClickedConsume;

	SDL_Rect inventoryBackgroundRect1;
	SDL_Rect inventoryBackgroundRect2;
	SDL_Color inventoryBackgroundColor1;
	SDL_Color inventoryBackgroundColor2;

	SDL_Rect bagRect;
};

#endif // __SCREENINVENTORY_H__