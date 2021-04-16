#include "SceneManager.h"

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGameplay.h"
#include "SceneEnding.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "Audio.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "DialogSystem.h"
#include "Easing.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager(Input* input, Render* render, Textures* tex, Window* win, AudioManager* audio, EntityManager* entityman, GuiManager* guiManager, DialogSystem* dialogSystem, Easing* easing) : Module()
{
	name.Create("scenemanager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;

	this->input = input;
	this->render = render;
	this->tex = tex;
	this->win = win;
	this->audio = audio;
	this->entityman = entityman;
	this->guiManager = guiManager;
	this->dialogSystem = dialogSystem;
	this->easing = easing;
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
	if (current->type == SceneType::GAMEPLAY)
	{
		current->Load(tex, win, audio, guiManager, entityman, dialogSystem, easing);
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
				if (next->type == SceneType::GAMEPLAY)
				{
					next->Load(tex, win, audio, guiManager, entityman, dialogSystem, easing);	// Load next screen
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
	if (onTransition)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, (unsigned char)(255.0f * transitionAlpha) }, true, false);
	}

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
			case SceneType::LOGO: next = new SceneLogo(); break;
			case SceneType::TITLE: next = new SceneTitle(); break;
			case SceneType::GAMEPLAY: next = new SceneGameplay(); break;
			case SceneType::ENDING: next = new SceneEnding(); break;
			default: break;
		}

		current->transitionRequired = false;
	}

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;
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
	pugi::xml_node docNode;
	docNode = data.child(current->name.GetString());
	LOG("Saving to the current scene:", current->name.GetString());
	current->LoadState(docNode);

	return true;
}

bool SceneManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node docNode;
	docNode = data.child(current->name.GetString());
	LOG("Saving to the current scene: %s", current->name.GetString());
	current->SaveState(docNode);

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
