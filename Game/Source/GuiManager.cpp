#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

GuiManager::GuiManager(Input* input, Render* render)
{
	name.Create("guimanager");

	this->input = input;
	this->render = render;
}

GuiManager::~GuiManager()
{
}

bool GuiManager::Awake(pugi::xml_node&)
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, SDL_Rect bounds)
{
	GuiControl* control = nullptr;

	switch (type)
	{
		// Create the corresponding GuiControl type
		//case GuiControlType::BUTTON: control = new GuiButton(1234);  break;
		case GuiControlType::BUTTON:
			control = new GuiButton(id, bounds, "NOTEXT");
			break;
		case GuiControlType::CHECKBOX:
			control = new GuiCheckBox(id, bounds, "NOTEXT");
			break;
		case GuiControlType::SLIDER:
			control = new GuiSlider(id, bounds, "NOTEXT");
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
	return true;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	DrawAll();

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

void GuiManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic == true)
	{
		for (int i = 0; i < controls.Count(); i++)
		{
			controls[i]->Update(this->input, dt);
		}
	}
}

void GuiManager::DrawAll()
{
	for (int i = 0; i < controls.Count(); i++)
	{
		controls[i]->Draw(this->render);
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

	return true;
}

