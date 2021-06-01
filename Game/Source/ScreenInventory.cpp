#include "ScreenInventory.h"

#include "EntityManager.h"
#include "Window.h"
#include "Player.h"
#include "BattleSystem.h"
#include "Log.h"
#include "Easing.h"

ScreenInventory::ScreenInventory()
{
	btnConfirm = nullptr;
	btnCancel = nullptr;

	itemHovering = { 0,0 };
	itemSelected = { -1,-1 };

	invMatrixPos = { 385, 110 };

	LBButton = { 434, 253, 90, 62 };
	RBButton = { 534, 253, 90, 62 };
	playersIcons = { 769,220, 81, 56 };
	pinkBox = { 237, 315, 108, 108 };
	grayBox = { 396, 108, 108, 108 };

	inventoryBackgroundColor1 = { 74,79,95,255 };
	inventoryBackgroundColor2 = { 128,137,154,255 };

	controller = false;
	hasClickedConsume = false;

	bagRect = { 1195, 320, 800, 603 };

	positionX = 2000;
	animIncrementY = 2000;
}

ScreenInventory::~ScreenInventory() {}

bool ScreenInventory::Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->atlas[1] = atlas1;

	this->font = font;

	this->guiManager = guiManager;
	this->entityManager = entityManager;
	this->win = win;
	this->easing = easing;
	this->audio = audio;
	this->battleSystem = battleSystem;

	controller = false;

	uint width, height;
	win->GetWindowSize(width, height);

	//inventoryBackgroundRect1 = { int(width) / 2 - 550, 40, 1100, 650 };
	//inventoryBackgroundRect2 = { inventoryBackgroundRect1.x + 25, inventoryBackgroundRect1.y + 25, inventoryBackgroundRect1.w - 50, inventoryBackgroundRect1.h - 50 };

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	btnConfirm = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { positionX, 480, 190, 49 }, "CONFIRM");
	btnConfirm->SetButtonProperties(currentScene, atlas[0], font, hoverFx, clickFx, Style::SUSHI_YELLOW);
	++counterId;

	btnCancel = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { positionX, 580, 190, 49 }, "CANCEL");
	btnCancel->SetButtonProperties(currentScene, atlas[0], font, hoverFx, clickFx, Style::SUSHI_YELLOW);

	return true;
}

bool ScreenInventory::Update(Input* input, float dt, uint& focusedButtonId)
{
	if ((input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_UP) == KeyState::KEY_DOWN) && itemHovering.y > 0)
		--itemHovering.y;
	else if ((input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KeyState::KEY_DOWN) && itemHovering.y < INVENTORY_ROWS - 1)
		++itemHovering.y;
	else if ((input->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KeyState::KEY_DOWN) && itemHovering.x > 0)
		--itemHovering.x;
	else if ((input->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KeyState::KEY_DOWN) && itemHovering.x < INVENTORY_COLUMNS - 1)
		++itemHovering.x;

	if (input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		itemSelected = { -1,-1 };

	int mouseX, mouseY;
	input->GetMousePosition(mouseX, mouseY);

	SDL_Rect slotRect = { invMatrixPos.x, invMatrixPos.y, 100, 100 };
	int spacing = slotRect.w / 2;
	// i is each row
	for (int i = 0; i < INVENTORY_ROWS; ++i)
	{
		slotRect.x = invMatrixPos.x;
		// j is each column
		for (int j = 0; j < INVENTORY_COLUMNS; ++j)
		{
			if (j == 0)
				slotRect.x = slotRect.x + slotRect.w - 5;
			else
				slotRect.x = slotRect.x + slotRect.w + spacing - 5;

			if ((mouseX > slotRect.x) && (mouseX < (slotRect.x + slotRect.w)) &&
				(mouseY > slotRect.y) && (mouseY < (slotRect.y + slotRect.h)))
			{
				itemHovering.x = j;
				itemHovering.y = i;
				if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
				{
					itemSelected = itemHovering;
				}
			}
		}
		slotRect.y = slotRect.y + 30 + slotRect.h;
	}

	// Accept button has been clicked
	if (hasClickedConsume)
	{
		ManageItemConsumption();
	}

	controller = input->GetControllerState();

	return true;
}

bool ScreenInventory::Draw(Render* render)
{
	//render->DrawRectangle({ 0,0,1280,720 }, { 0, 0, 0, 100 }, true, false);

	//render->DrawRectangle(inventoryBackgroundRect1, inventoryBackgroundColor1, true, false);
	//render->DrawRectangle(inventoryBackgroundRect2, inventoryBackgroundColor2, true, false);

	render->DrawTexture(atlas[0], (1480 / 2 - bagRect.w / 2) / render->scale, animIncrementY + 50 / render->scale, &bagRect, 0.0f);

	char pos[24] = { 0 };
	sprintf_s(pos, 24, "itemHovering %i,%i", itemHovering.x, itemHovering.y);
	render->DrawText(font, pos, 0, animIncrementY + 10, 22, 4, { 255,255,255,255 });

	sprintf_s(pos, 24, "itemSelected %i,%i", itemSelected.x, itemSelected.y);
	render->DrawText(font, pos, 0, animIncrementY + 30, 22, 4, { 255,255,255,255 });

	SDL_Rect slotRect = { invMatrixPos.x, invMatrixPos.y, 100, 100 };
	int spacing = slotRect.w / 2;
	// i is each row
	ListItem<InvItem*>* listItem = this->listInvItems.start;
	for (int i = 0; i < INVENTORY_ROWS; ++i)
	{
		slotRect.x = invMatrixPos.x;
		// j is each column
		for (int j = 0; j < INVENTORY_COLUMNS; ++j)
		{
			if (j == 0)
				slotRect.x = slotRect.x + slotRect.w - 5;
			else
				slotRect.x = slotRect.x + slotRect.w + spacing - 5;

			//if (itemHovering.x == j && itemHovering.y == i && itemHovering != itemSelected)
			//	//render->DrawRectangle(slotRect, { 191, 195,204,255 }, true, false);
			if (i == 0)
			{
				if (itemHovering.x == j && itemHovering.y == i && itemHovering != itemSelected)
					render->DrawTexture(this->atlas[0], slotRect.x - 3 - j, animIncrementY + slotRect.y + 1, &grayBox, 0.0f);
				else if (itemSelected.x == j && itemSelected.y == i)
					render->DrawTexture(this->atlas[0], slotRect.x - 8, animIncrementY + slotRect.y + 1, &pinkBox, 0.0f);
			}
			else
			{
				if (itemHovering.x == j && itemHovering.y == i && itemHovering != itemSelected)
					render->DrawTexture(this->atlas[0], slotRect.x - 3 - j, animIncrementY + slotRect.y - 3, &grayBox, 0.0f);
				if (itemSelected.x == j && itemSelected.y == i)
					render->DrawTexture(this->atlas[0], slotRect.x - 8, animIncrementY + slotRect.y - 3, &pinkBox, 0.0f);
			}
			

			// Draw item stats
			if (listInvItems.Count() > 0 && (listInvItems.Count() - 1) >= (itemSelected.y * INVENTORY_ROWS) + itemSelected.x)
			{
				InvItem* currentItem = listInvItems.At((itemSelected.y * INVENTORY_ROWS) + itemSelected.x)->data;

				char statsString[30] = { 0 };
				sprintf_s(statsString, 30, "%s", currentItem->item->name.GetString());
				render->DrawText(font, statsString, invMatrixPos.x + 250, animIncrementY + 60, 30, 2, { 255,255,255,255 });

				render->DrawText(font, currentItem->item->GetDescription(), invMatrixPos.x + 85, animIncrementY + 375, 22, 2, { 255,255,255,255 });

				int posIncreaseX = 620;
				// Items stats increase for player draw
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.level);
				render->DrawText(font, statsString, posIncreaseX, animIncrementY + 460, 18, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.damage);
				render->DrawText(font, statsString, posIncreaseX, animIncrementY + 490, 18, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.maxHP);
				render->DrawText(font, statsString, posIncreaseX, animIncrementY + 520, 18, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.currentHP);
				render->DrawText(font, statsString, posIncreaseX, animIncrementY + 550, 18, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.strength);
				render->DrawText(font, statsString, posIncreaseX + animIncrementY + 210, 460, 18, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.defense);
				render->DrawText(font, statsString, posIncreaseX + animIncrementY + 210, 490, 18, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.attackSpeed);
				render->DrawText(font, statsString, posIncreaseX + animIncrementY + 210, 520, 18, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%.1f  ", currentItem->item->stats.criticalRate);
				render->DrawText(font, statsString, posIncreaseX + animIncrementY + 210, 550, 18, 2, { 0,255,0,255 });
			}

			if (listItem != NULL)
			{
				render->scale = 2;
				render->DrawTexture(entityManager->itemsTexture, (slotRect.x + ((slotRect.w - listItem->data->item->GetSection().w)/2))/render->scale - 9, (animIncrementY + slotRect.y + ((slotRect.h - listItem->data->item->GetSection().h) / 2))/ render->scale - 9, &listItem->data->item->GetSection(), 0.0f);
				render->scale = 1;
				// Draw item
				char itemType[24] = { 0 };
				/*sprintf_s(itemType, 24, "item type is %i", (int)listItem->data->item->subtype);
				render->DrawText(font, itemType, slotRect.x, slotRect.y, 10, 2, { 255,255,255,255 });*/

				sprintf_s(itemType, 24, "%i", (int)listItem->data->count);
				render->DrawText(font, itemType, slotRect.x + slotRect.w - 20, animIncrementY + slotRect.y + slotRect.h - 30, 50, 2, { 255,255,255,255 });
				listItem = listItem->next;
			}
			else
				continue;

		}
		slotRect.y = slotRect.y + 30 + slotRect.h;
	}

	int posPlayerStatsX = 485;
	render->DrawTexture(this->atlas[0], 1000, animIncrementY + invMatrixPos.y, &playersIcons, 0.0f);
	if (controller)
	{
		render->DrawTexture(this->atlas[0], 780 - 40, animIncrementY + 70, &LBButton, 0.0f);
		render->DrawTexture(this->atlas[0], 880 + 40, animIncrementY + 70, &RBButton, 0.0f);
	}
	// Draw Player member and stats
	for (int i = 0; i < entityManager->playerList.Count(); ++i)
	{
		if (entityManager->playerList.At(i)->data->IsActive())
		{
			int y = entityManager->playerList.At(i)->data->spritePos * 32 * 5;
			SDL_Rect rect = { 0, y , 32, 32 };
			render->scale = 2;
			render->DrawRectangle({ positionX,  animIncrementY + invMatrixPos.y - 5, 70, 70 }, { 255,255,255,127 }, true, false);
			render->DrawRectangle({ positionX, animIncrementY + invMatrixPos.y - 5, 70, 70 }, { 255,255,255,255 }, false, false);
			render->DrawTexture(entityManager->texture, (positionX) / 2 + 2, (invMatrixPos.y - 5) / 2 + 2, &rect, 0.0f);
			render->scale = 1;

			// Draw Player Stats
			render->DrawText(font, "LVL:", posPlayerStatsX, animIncrementY + 460, 18, 2, { 19, 38, 57,255 });
			render->DrawText(font, "Damage:", posPlayerStatsX, animIncrementY + 490, 18, 2, { 19, 38, 57,255 });
			render->DrawText(font, "Max HP:", posPlayerStatsX, animIncrementY + 520, 18, 2, { 19, 38, 57,255 });
			render->DrawText(font, "HP:", posPlayerStatsX, animIncrementY + 550, 18, 2, { 19, 38, 57,255 });
			render->DrawText(font, "Strength:", posPlayerStatsX + 180, animIncrementY + 460, 18, 2, { 19, 38, 57,255 });
			render->DrawText(font, "Defense:", posPlayerStatsX + 180, animIncrementY + 490, 18, 2, { 19, 38, 57,255 });
			render->DrawText(font, "Atk Speed:", posPlayerStatsX + 180, animIncrementY + 520, 18, 2, { 19, 38, 57,255 });
			render->DrawText(font, "CritRate:", posPlayerStatsX + 180, animIncrementY + 550, 18, 2, { 19, 38, 57,255 });

			int posStatsNum = 580;
			char statsString[30] = { 0 };
			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.level);
			render->DrawText(font, statsString, posStatsNum, animIncrementY + 460, animIncrementY + 18, 2, { 255, 194, 102,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.damage);
			render->DrawText(font, statsString, posStatsNum, animIncrementY + 490, animIncrementY + 18, 2, { 255, 194, 102,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.maxHP);
			render->DrawText(font, statsString, posStatsNum, animIncrementY + 520, animIncrementY + 18, 2, { 255, 194, 102,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.currentHP);
			render->DrawText(font, statsString, posStatsNum, animIncrementY + 550, animIncrementY + 18, 2, { 255, 194, 102,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.strength);
			render->DrawText(font, statsString, posStatsNum + 208, animIncrementY + 460, animIncrementY + 18, 2, { 255, 194, 102,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.defense);
			render->DrawText(font, statsString, posStatsNum + 208, animIncrementY + 490, animIncrementY + 18, 2, { 255, 194, 102,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.attackSpeed);
			render->DrawText(font, statsString, posStatsNum + 208, animIncrementY + 520, animIncrementY + 18, 2, { 255, 194, 102,255 });

			sprintf_s(statsString, 30, "%.1f  ", entityManager->playerList.At(i)->data->stats.criticalRate);
			render->DrawText(font, statsString, posStatsNum + 208, animIncrementY + 550, animIncrementY + 18, 2, { 255, 194, 102,255 });
		}
	}
	return true;
}

bool ScreenInventory::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	guiManager->DestroyGuiControl(btnConfirm);
	btnConfirm = nullptr;
	guiManager->DestroyGuiControl(btnCancel);
	btnCancel = nullptr;

	for (int i = 0; i < listInvItems.Count(); ++i)
	{
		listInvItems.Del(listInvItems.At(0));
	}
	listInvItems.Clear();

	return true;
}

bool ScreenInventory::LoadState(pugi::xml_node& screenNode)
{
	return true;
}

bool ScreenInventory::SaveState(pugi::xml_node& screenNode) const
{
	LOG("SAVING INVENTORY");
	// Check screeninventory node
	pugi::xml_node screenInventoryNode;
	SString tempName = screenNode.child("screeninventory").name();
	if (tempName == "screeninventory")
	{
		screenInventoryNode = screenNode.child("screeninventory");
	}
	else
	{
		screenInventoryNode = screenNode.append_child("screeninventory");
	}

	for (int i = 0; i < screenInventoryNode.attribute("inventoryCount").as_int(); ++i)
	{
		bool remove = screenInventoryNode.remove_child("invSlot");
		if (remove == false)
			break;
	}

	// Check inventoryCount
	tempName = screenInventoryNode.attribute("inventoryCount").name();
	if (tempName == "inventoryCount")
	{
		screenInventoryNode.attribute("inventoryCount").set_value(listInvItems.Count());
	}
	else
	{
		screenInventoryNode.append_attribute("inventoryCount").set_value(listInvItems.Count());
	}

	// Save each individual inventory item
	ListItem<InvItem*>* list;
	for (list = listInvItems.start; list != NULL; list = list->next)
	{
		// Create Node to save invSlots
		pugi::xml_node itemNode = screenInventoryNode;
		itemNode = itemNode.append_child("invSlot");
		itemNode.append_attribute("entitySubType").set_value((int)list->data->item->subtype);
		itemNode.append_attribute("count").set_value(list->data->count);
		itemNode.append_attribute("name").set_value(list->data->item->name.GetString());
		itemNode.append_attribute("description").set_value(list->data->item->GetDescription());
	}

	LOG("INVENTORY SAVED");
	return true;
}

void ScreenInventory::SetHasClickedConsume(bool value)
{
	hasClickedConsume = value;
}

bool ScreenInventory::GetHasClickedConsume(bool value)
{
	return hasClickedConsume;
}

void ScreenInventory::ManageItemConsumption()
{
	for (int i = 0; i < entityManager->playerList.Count(); ++i)
	{
		if (entityManager->playerList.At(i)->data->IsActive())
		{
			// (y * width) + x
			int index = (itemSelected.y * INVENTORY_ROWS) + itemSelected.x;
			if (listInvItems.At(index) != nullptr)
			{
				battleSystem->SetItem(listInvItems.At(index)->data->item->name);

				// Play consume fx
				audio->PlayFx(listInvItems.At(index)->data->item->consumeFx);
				ConsumeItem(entityManager->playerList.At(i)->data, listInvItems.At(index)->data);
				break;
			}
		}
	}

	/*hasClickedConsume = false;*/
	/*battleSystem->SetHasClickedConsume(false);*/
}

void ScreenInventory::ConsumeItem(Player* currentPlayer, InvItem* inventoryItem)
{
	currentPlayer->stats = inventoryItem->item->Interact(currentPlayer->stats);
	inventoryItem->count -= 1;
	if (inventoryItem->count <= 0)
		listInvItems.Del(listInvItems.At(listInvItems.Find(inventoryItem)));
}

void ScreenInventory::Enable()
{
	easing->CreateSpline(&positionX, 1000, 2000, SplineType::QUINT);
	easing->CreateSpline(&animIncrementY, 0, 2000, SplineType::QUINT);
	easing->CreateSpline(&btnConfirm->bounds.x, 1000, 2000, SplineType::QUINT);
	easing->CreateSpline(&btnCancel->bounds.x, 1000, 2000, SplineType::QUINT);

	Screen::Enable();
}

void ScreenInventory::Disable()
{
	positionX = 2000;
	animIncrementY = 2000;
	btnConfirm->bounds.x = 2000;
	btnCancel->bounds.x = 2000;
	Screen::Disable();
}
