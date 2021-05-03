#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"

class GuiButton;

class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;
class GuiManager;
class Window;
class DialogSystem;
class Easing;
class Transitions;
class App;

class SceneManager : public Module
{
public:

	SceneManager(Input* input, Render* render, Textures* tex, Window* win, AudioManager* audio, EntityManager* entityman, GuiManager* guiManager, DialogSystem* dialogSystem, Easing* easing, Transitions* transitions, App* app);

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// To take the current scene
	Scene* PickScene();

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

private:

	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audio;
	EntityManager* entityman;
	GuiManager* guiManager;
	Window* win;
	DialogSystem* dialogSystem;
	Easing* easing;
	Transitions* transitions;
	App* app;

	Scene* current;
	Scene* next;

	// Required variables to manage screen transitions (fade-in, fade-out)
	bool onTransition;
	bool fadeOutCompleted;
	float transitionAlpha;
};

#endif // __SCENEMANAGER_H__