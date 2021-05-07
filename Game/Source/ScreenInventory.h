#ifndef __SCREENINVENTORY_H__
#define __SCREENINVENTORY_H__

#include "Screen.h"
#include "Item.h"

#define INVENTORY_ROWS 2
#define INVENTORY_COLUMNS 3

//struct InventorySlot
//{
//	Item* item = nullptr;
//	uint quantity = 0;
//};

class ScreenInventory : public Screen
{
public:
	ScreenInventory();
	~ScreenInventory();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);

	bool Draw(Render* render);

	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	void SetInventory(List<InvItem*> invItems);
private:
	//InventorySlot inventory[INVENTORY_ROWS][INVENTORY_COLUMNS];

	iPoint itemHovering;
	iPoint itemSelected;

	GuiButton* btnConfirm;
	GuiButton* btnCancel;

	SDL_Rect LBButton;
	SDL_Rect RBButton;
	SDL_Rect playersIcons;

	List<InvItem*> listInvItems;

	bool controller;
};

#endif // __SCREENINVENTORY_H__