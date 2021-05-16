#include "SceneManager.h"

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGameplay.h"
#include "SceneEnding.h"

#include "Input.h"
#include "Render.h"
#include "AssetsManager.h"
#include "Textures.h"
#include "Window.h"
#include "Audio.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "DialogSystem.h"
#include "Easing.h"
#include "Transitions.h"
#include "QuestManager.h"
#include "App.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager(Input* input, Render* render, Textures* tex, Window* win, AudioManager* audio, EntityManager* entityman, GuiManager* guiManager, DialogSystem* dialogSystem, Easing* easing, Transitions* transitions, AssetsManager* assetsManager, App* app) : Module()
{
	name.Create("scenemanager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;

	this->input = input;
	this->render = render;
	this->assetsManager = assetsManager;
	this->tex = tex;
	this->win = win;
	this->audio = audio;
	this->entityman = entityman;
	this->guiManager = guiManager;
	this->dialogSystem = dialogSystem;
	this->easing = easing;
	this->transitions = transitions;
	this->app = app;
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	LOG("Loading Scene manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	current = new SceneLogo();
	if (current->type == SceneType::GAMEPLAY || current->type == SceneType::GAMEPLAY_LOAD)
	{
		current->Load(input, render, tex, win, audio, guiManager, entityman, dialogSystem, easing, transitions, assetsManager, app);
	}
	else if (current->type == SceneType::TITLE)
	{
		current->Load(tex, win, audio, guiManager, easing, render, transitions);
	}
	else
	{
		current->Load(tex, win, audio, guiManager, easing);
	}

	next = nullptr;

	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (!onTransition)
	{
		current->Update(input, dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.00f)
			{
				transitionAlpha = 1.0f;
				
				current->Unload(tex, audio, guiManager); // Unload current screen
				if (next->type == SceneType::GAMEPLAY || next->type == SceneType::GAMEPLAY_LOAD)
				{
					next->Load(input, render, tex, win, audio, guiManager, entityman, dialogSystem, easing, transitions, assetsManager, app);	// Load next screen
				}
				else if (next->type == SceneType::TITLE)
				{
					next->Load(tex, win, audio, guiManager, easing, render, transitions);
				}
				else
				{
					next->Load(tex, win, audio, guiManager, easing); // Load next screen
				}

				RELEASE(current); // Free current pointer
				current = next;	// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	// Draw current scene
	current->Draw(render);

	// Draw full screen rectangle in front of everything
	/*if (onTransition)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, (unsigned char)(255.0f * transitionAlpha) }, true, false);
	}*/

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
			case SceneType::LOGO: next = new SceneLogo(); break;
			case SceneType::TITLE: next = new SceneTitle(); break;
			case SceneType::GAMEPLAY: next = new SceneGameplay(false); break;
			case SceneType::GAMEPLAY_LOAD: next = new SceneGameplay(true); break;
			case SceneType::ENDING_DEFEAT: next = new SceneEnding(false); break;
			case SceneType::ENDING_VICTORY: next = new SceneEnding(true); break;
			default: break;
		}

		current->transitionRequired = false;
	}

	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	if (current != nullptr) current->Unload(tex, audio, guiManager);

	return true;
}

// Returns the current scene
Scene* SceneManager::PickScene()
{
	return current;
}

bool SceneManager::LoadState(pugi::xml_node& data)
{
	pugi::xml_node sceneManagerNode = data;
	pugi::xml_node currentSceneNode;

	currentSceneNode = sceneManagerNode.child(current->name.GetString());
	LOG("Saving to the current scene:", current->name.GetString());
	current->LoadState(currentSceneNode);

	return true;
}

bool SceneManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node sceneManagerNode = data;
	pugi::xml_node currentSceneNode;

	///* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	//SString tempName(sceneManagerNode.child("sceneManager").name());
	//if (tempName == "sceneManager")
	//{
	//	// Node sceneManager exists
	//	sceneManagerNode = sceneManagerNode.child("sceneManager");
	//}
	//else
	//{
	//	// Node sceneManager does not exist
	//	sceneManagerNode = sceneManagerNode.append_child("sceneManager");
	//}

	/* ---------- CHECKS IF THE NODE WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	SString tempName = sceneManagerNode.child(current->name.GetString()).name();
	if (tempName == current->name)
	{
		// Node cureentScene exists
		currentSceneNode = sceneManagerNode.child(current->name.GetString());
	}
	else
	{
		// Node cureentScene does not exist
		currentSceneNode = sceneManagerNode.append_child(current->name.GetString());
	}

	LOG("Saving to the current scene: %s", current->name.GetString());
	current->SaveState(currentSceneNode);

	//ListItem<Module*>* item;
	//item = modules.start;

	//while (item != NULL && ret == true)
	//{
	//	// Create a node for each module and send it to their Save function
	//	ret = item->data->SaveState(docNode.child(item->data->name.GetString()));
	//	item = item->next;
	//}

	return true;
}
