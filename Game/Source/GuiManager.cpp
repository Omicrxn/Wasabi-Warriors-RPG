#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "GuiIcon.h"

#include "Textures.h"

GuiManager::GuiManager(Input* input, Render* render, Textures* tex, AudioManager* audio)
{
	name.Create("guimanager");

	this->input = input;
	this->render = render;
	this->tex = tex;
	this->audio = audio;

	debugDraw = false;

	guiAtlasTex = nullptr;
	mousePos = { 0,0 };
	clicking = false;

	exitingGame = false;
}

GuiManager::~GuiManager()
{
}

bool GuiManager::Awake(pugi::xml_node&)
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* text)
{
	GuiControl* control = nullptr;

	switch (type)
	{
		// Create the corresponding GuiControl type
		//case GuiControlType::BUTTON: control = new GuiButton(1234);  break;
		case GuiControlType::BUTTON:
			control = new GuiButton(id, bounds, text);
			break;
		case GuiControlType::CHECKBOX:
			control = new GuiCheckBox(id, bounds, text);
			break;
		case GuiControlType::SLIDER:
			control = new GuiSlider(id, bounds, text);
			break;
		case GuiControlType::ICON:
			control = new GuiIcon(id, bounds);
			break;
		default: break;
	}

	// Created entities are added to the list
	if (control != nullptr) controls.Add(control);

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* control)
{
	int index = controls.Find(control);
	ListItem<GuiControl*>* List = controls.At(index);
	controls.Del(List);
}

bool GuiManager::Start()
{
	guiAtlasTex = tex->Load("Assets/Textures/UI/ui_spritesheet.png");

	mouseRect[0] = { 30,482,30,30 };
	mouseRect[1] = { 60,482,30,30 };

	return true;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debugDraw = !debugDraw;

	// Mouse Cursor Update
	input->GetMousePosition(mousePos.x, mousePos.y);

	if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
		clicking = true;
	else
		clicking = false;
	// ----------------------------

	UpdateAll(dt, doLogic);

	DrawAll();

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	// Mouse Cursor Draw
	if (clicking)
		render->DrawTexture(guiAtlasTex, mousePos.x, mousePos.y, &mouseRect[0], 0.0f);
	else
		render->DrawTexture(guiAtlasTex, mousePos.x, mousePos.y, &mouseRect[1], 0.0f);
	// ----------------------------

	if (exitingGame) return false;

	return true;
}

void GuiManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic == true)
	{
		for (int i = 0; i < controls.Count(); i++)
		{
			controls[i]->Update(this->input, this->audio, dt);
		}
	}
}

void GuiManager::DrawAll()
{
	for (int i = 0; i < controls.Count(); i++)
	{
		controls[i]->Draw(this->render, debugDraw);
	}
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* list;

	for(list = controls.start; list != NULL; list = list->next)
	{
		controls.Del(list);
		RELEASE(list->data);
	}

	controls.Clear();

	tex->UnLoad(guiAtlasTex);

	return true;
}

void GuiManager::ExitGame()
{
	exitingGame = true;
}