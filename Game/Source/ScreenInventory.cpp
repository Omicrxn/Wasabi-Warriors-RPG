#include "ScreenInventory.h"

ScreenInventory::ScreenInventory()
{
	btnConfirm = nullptr;
	btnCancel = nullptr;

	itemHovering = { 0,0 };
	itemSelected = { -1,-1 };
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

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	btnConfirm = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 900, 600, 190, 49 }, "");
	btnConfirm->SetButtonProperties(currentScene, atlas[0], font, hoverFx, clickFx, Style::WHITE);
	++counterId;

	btnCancel = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { 950, 600, 190, 49 }, "");
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

	return true;
}

bool ScreenInventory::Draw(Render* render)
{
	render->DrawRectangle({ 0,0,1280,720 }, { 0,0,0,255 }, true, false);

	//char pos[24] = { 0 };
	//sprintf_s(pos, 24, "itemHovering %i,%i", itemHovering.x, itemHovering.y);
	//render->DrawText(font, pos, 0, 10, 22, 4, { 255,255,255,255 });

	//sprintf_s(pos, 24, "itemSelected %i,%i", itemSelected.x, itemSelected.y);
	//render->DrawText(font, pos, 0, 30, 22, 4, { 255,255,255,255 });

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

			if (itemHovering.x == j && itemHovering.y == i && itemHovering != itemSelected)
				render->DrawRectangle(slotRect, { 0,255,0,255 }, true, false);
			else if (itemSelected.x == j && itemSelected.y == i)
				render->DrawRectangle(slotRect, { 255,255,0,255 }, true, false);
			else
				render->DrawRectangle(slotRect, { 255,0,0,255 }, true, false);
		}
		slotRect.y = slotRect.y + spacing + slotRect.h;
	}

	return true;
}

bool ScreenInventory::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	guiManager->DestroyGuiControl(btnConfirm);
	btnConfirm = nullptr;
	guiManager->DestroyGuiControl(btnCancel);
	btnCancel = nullptr;

	return true;
}