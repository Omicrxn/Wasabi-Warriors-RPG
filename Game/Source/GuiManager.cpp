//#include "GuiManager.h"
//
//#include "GuiButton.h"
//
//GuiManager::GuiManager(Input* input, Render* render)
//{
//	name.Create("guimanager");
//
//	this->input = input;
//	this->render = render;
//	this->tex = tex;
//}
//
//GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, SDL_Rect bounds)
//{
//	GuiControl* control = nullptr;
//
//	switch (type)
//	{
//		// Create the corresponding GuiControl type
//		//case GuiControlType::BUTTON: control = new GuiButton(1234);  break;
//		case GuiControlType::BUTTON:
//			control = new GuiButton(id, bounds, "NOTEXT");
//			break;
//		case GuiControlType::CHECKBOX:
//			control = new GuiCheckBox(id, bounds, "NOTEXT");
//			break;
//		case GuiControlType::SLIDER:
//			control = new GuiSlider(id, bounds, "NOTEXT");
//			break;
//		default: break;
//	}
//
//	// Created entities are added to the list
//	if (control != nullptr) controls.Add(control);
//
//	return control;
//}
//
//void GuiManager::DestroyGuiControl(GuiControl* control)
//{
//	int index = controls.Find(control);
//	ListItem<GuiControl*>* List = controls.At(index);
//	controls.Del(List);
//}
//
//bool GuiManager::Update(float dt)
//{
//	accumulatedTime += dt;
//	if (accumulatedTime >= updateMsCycle) doLogic = true;
//
//	UpdateAll(dt, doLogic);
//
//	DrawALL();
//
//	if (doLogic == true)
//	{
//		accumulatedTime = 0.0f;
//		doLogic = false;
//	}
//
//	return true;
//}
//
//void GuiManager::UpdateAll(float dt, bool doLogic)
//{
//	if (doLogic == true)
//	{
//		for (int i = 0; i < controls.Count(); i++)
//		{
//			controls[i]->Update(this->input, dt);
//		}
//	}
//}
//
//void GuiManager::DrawAll()
//{
//	for (int i = 0; i < controls.Count(); i++)
//	{
//		controls[i]->Draw(this->render);
//	}
//}
//
//bool GuiManager::CleanUp()
//{
//	ListItem<GuiControl*>* list;
//
//	for(list = controls.start; list != NULL; list = list->next)
//	{
//		controls.Del(list);
//		RELEASE(list->data);
//	}
//
//	controls.Clear();
//
//	return false;
//}
//
