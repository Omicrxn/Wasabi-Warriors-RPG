#include "ScreenInventory.h"

#include "EntityManager.h"
#include "Player.h"

ScreenInventory::ScreenInventory()
{
	btnConfirm = nullptr;
	btnCancel = nullptr;

	itemHovering = { 0,0 };
	itemSelected = { -1,-1 };

	LBButton = { 434, 253, 90, 62 };
	RBButton = { 534, 253, 90, 62 };
	playersIcons = { 769,220, 81, 56 };
}

ScreenInventory::~ScreenInventory() {}

bool ScreenInventory::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->font = font;

	this->guiManager = guiManager;
	this->entityManager = entityManager;
	this->win = win;
	this->easing = easing;

	controller = false;

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	btnConfirm = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 800, 600, 190, 49 }, "CONFIRM");
	btnConfirm->SetButtonProperties(currentScene, atlas[0], font, hoverFx, clickFx, Style::WHITE);
	++counterId;

	btnCancel = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 1050, 600, 190, 49 }, "CANCEL");
	btnCancel->SetButtonProperties(currentScene, atlas[0], font, hoverFx, clickFx, Style::WHITE);

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

	SDL_Rect slotRect = { 0, 100, 100, 100 };
	int spacing = slotRect.w / 2;
	// i is each row
	for (int i = 0; i < INVENTORY_ROWS; ++i)
	{
		// j is each column
		slotRect.x = 0;
		for (int j = 0; j < INVENTORY_COLUMNS; ++j)
		{
			if (j == 0)
				slotRect.x = slotRect.x + slotRect.w;
			else
				slotRect.x = slotRect.x + slotRect.w + spacing;

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
		slotRect.y = slotRect.y + spacing + slotRect.h;
	}


	//if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
	//	(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
	//{
	//	if(input->GetMouseButtonDown(SDL_SCANCODE_) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_) == KeyState::KEY_DOWN)
	controller = input->GetControllerState();

	return true;
}

bool ScreenInventory::Draw(Render* render)
{
	render->DrawRectangle({ 0,0,1280,720 }, { 0, 0, 0, 100 }, true, false);

	char pos[24] = { 0 };
	sprintf_s(pos, 24, "itemHovering %i,%i", itemHovering.x, itemHovering.y);
	render->DrawText(font, pos, 0, 10, 22, 4, { 255,255,255,255 });

	sprintf_s(pos, 24, "itemSelected %i,%i", itemSelected.x, itemSelected.y);
	render->DrawText(font, pos, 0, 30, 22, 4, { 255,255,255,255 });

	// Background
	SDL_Rect slotRect = { 0, 100, 100, 100 };
	int spacing = slotRect.w / 2;
	// i is each row
	ListItem<InvItem*>* listItem = this->listInvItems.start;
	for (int i = 0; i < INVENTORY_ROWS; ++i)
	{
		// j is each column
		slotRect.x = 0;
		for (int j = 0; j < INVENTORY_COLUMNS; ++j)
		{
			if (j == 0)
				slotRect.x = slotRect.x + slotRect.w;
			else
				slotRect.x = slotRect.x + slotRect.w + spacing;

			if (itemHovering.x == j && itemHovering.y == i && itemHovering != itemSelected)
				render->DrawRectangle(slotRect, { 0,255,0,255 }, true, false);
			else if (itemSelected.x == j && itemSelected.y == i)
				render->DrawRectangle(slotRect, { 255,255,0,255 }, true, false);
			else
				render->DrawRectangle(slotRect, { 255,0,0,255 }, true, false);

			// Draw item stats
			if (listInvItems.Count() > 0 && (listInvItems.Count() - 1) >= (itemSelected.x * INVENTORY_COLUMNS + itemSelected.y))
			{
				InvItem* currentItem = listInvItems.At(itemSelected.x * INVENTORY_COLUMNS + itemSelected.y)->data;

				render->DrawText(font, "Description", 100, 450, 22, 2, { 255,255,255,255 });

				char statsString[30] = { 0 };
				sprintf_s(statsString, 30, "%s", currentItem->item->name.GetString());
				render->DrawText(font, statsString, 100, 400, 30, 2, { 255,255,255,255 });

				// Items stats increase for player draw
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.level);
				render->DrawText(font, statsString, 1150, 260, 30, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.damage);
				render->DrawText(font, statsString, 1150, 300, 30, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.maxHP);
				render->DrawText(font, statsString, 1150, 340, 30, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.currentHP);
				render->DrawText(font, statsString, 1150, 380, 30, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.strength);
				render->DrawText(font, statsString, 1150, 420, 30, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.defense);
				render->DrawText(font, statsString, 1150, 460, 30, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%i  ", currentItem->item->stats.attackSpeed);
				render->DrawText(font, statsString, 1150, 500, 30, 2, { 0,255,0,255 });
				sprintf_s(statsString, 30, "+%.1f  ", currentItem->item->stats.criticalRate);
				render->DrawText(font, statsString, 1150, 540, 30, 2, { 0,255,0,255 });
			}

			if (listItem != NULL)
			{
				// Draw item
				char itemType[24] = { 0 };
				sprintf_s(itemType, 24, "item type is %i", (int)listItem->data->item->subtype);
				render->DrawText(font, itemType, slotRect.x, slotRect.y, 10, 2, { 255,255,255,255 });

				sprintf_s(itemType, 24, "item count is %i", (int)listItem->data->count);
				render->DrawText(font, itemType, slotRect.x, slotRect.y + 10, 10, 2, { 255,255,255,255 });
				listItem = listItem->next;
			}
			else
				continue;

		}
		slotRect.y = slotRect.y + spacing + slotRect.h;
	}

	render->DrawTexture(this->atlas[0], 930, 70, &playersIcons, 0.0f);
	if (0)
	{
		render->DrawTexture(this->atlas[0], 880 - 40, 70, &LBButton, 0.0f);
		render->DrawTexture(this->atlas[0], 980 + 40, 70, &RBButton, 0.0f);
	}
	// Draw miniplayer
	for (int i = 0; i < entityManager->playerList.Count(); ++i)
	{
		if (entityManager->playerList.At(i)->data->IsActive())
		{
			int y = entityManager->playerList.At(i)->data->spritePos * 32 * 5;
			SDL_Rect rect = { 0, y , 32, 32 };
			render->scale = 2;
			render->DrawRectangle({ 940,  150, 70, 70 }, { 255,255,255,127 }, true, false);
			render->DrawRectangle({ 940,  150, 70, 70 }, { 255,255,255,255 }, false, false);
			render->DrawTexture(entityManager->texture, 940 / 2 + 2, 150 / 2 + 2, &rect, 0.0f);
			render->scale = 1;

			// Draw Player Stats
			render->DrawText(font, "LVL:", 825, 260, 30, 2, { 255,255,255,255 });
			render->DrawText(font, "Damage:", 825, 300, 30, 2, { 255,255,255,255 });
			render->DrawText(font, "Max HP:", 825, 340, 30, 2, { 255,255,255,255 });
			render->DrawText(font, "HP:", 825, 380, 30, 2, { 255,255,255,255 });
			render->DrawText(font, "Strength:", 825, 420, 30, 2, { 255,255,255,255 });
			render->DrawText(font, "Defense:", 825, 460, 30, 2, { 255,255,255,255 });
			render->DrawText(font, "Atk Speed:", 825, 500, 30, 2, { 255,255,255,255 });
			render->DrawText(font, "CritRate:", 825, 540, 30, 2, { 255,255,255,255 });

			char statsString[30] = { 0 };
			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.level);
			render->DrawText(font, statsString, 1050, 260, 30, 2, { 255,255,255,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.damage);
			render->DrawText(font, statsString, 1050, 300, 30, 2, { 255,255,255,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.maxHP);
			render->DrawText(font, statsString, 1050, 340, 30, 2, { 255,255,255,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.currentHP);
			render->DrawText(font, statsString, 1050, 380, 30, 2, { 255,255,255,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.strength);
			render->DrawText(font, statsString, 1050, 420, 30, 2, { 255,255,255,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.defense);
			render->DrawText(font, statsString, 1050, 460, 30, 2, { 255,255,255,255 });

			sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.attackSpeed);
			render->DrawText(font, statsString, 1050, 500, 30, 2, { 255,255,255,255 });

			sprintf_s(statsString, 30, "%.1f  ", entityManager->playerList.At(i)->data->stats.criticalRate);
			render->DrawText(font, statsString, 1050, 540, 30, 2, { 255,255,255,255 });
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

void ScreenInventory::SetInventory(List<InvItem*> invItems)
{
	listInvItems.Clear();
	for (ListItem<InvItem*>* invItem = invItems.start; invItem; invItem = invItem->next)
	{
		listInvItems.Add(invItem->data);
	}
}
