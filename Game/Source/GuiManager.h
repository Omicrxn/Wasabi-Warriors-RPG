#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class Input;
class Render;

class GuiManager : public Module
{
public:

	GuiManager(Input* input, Render* render);

	// Destructor
	virtual ~GuiManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, SDL_Rect bounds);
	void DestroyGuiControl(GuiControl* entity);

	//void AddGuiControl(GuiControl* entity);
	
	void UpdateAll(float dt, bool doLogic);

	void DrawAll();

public:

	List<GuiControl*> controls;

public:

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

public:
	Input* input;
	Render* render;

};

#endif // __GUIMANAGER_H__
