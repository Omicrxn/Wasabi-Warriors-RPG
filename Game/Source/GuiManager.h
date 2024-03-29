#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class Input;
class Render;
class Textures;

class GuiManager : public Module
{
public:

	GuiManager(Input* input, Render* render, Textures* tex, AudioManager* audio);

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
	GuiControl* CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* text = "");
	void DestroyGuiControl(GuiControl* control);

	//void AddGuiControl(GuiControl* entity);
	
	void UpdateAll(float dt, bool doLogic);

	void DrawAll();

	void ExitGame();

	void ToggleMouse();

	GuiControl* FindById(int id);

public:

	List<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	bool debugDraw;

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audio;

	SDL_Texture* guiAtlasTex;

	// Mouse Cursor attributes
	SDL_Rect mouseRect[2];
	iPoint mousePos;
	bool clicking;

	bool activeMouse;

private:
	
	bool exitingGame;
};

#endif // __GUIMANAGER_H__