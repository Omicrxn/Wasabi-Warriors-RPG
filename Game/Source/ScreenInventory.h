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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void SetInventory(List<InvItem*> invItems);

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

	SDL_Rect inventoryBackgroundRect1;
	SDL_Rect inventoryBackgroundRect2;
	SDL_Color inventoryBackgroundColor1;
	SDL_Color inventoryBackgroundColor2;
};

#endif // __SCREENINVENTORY_H__