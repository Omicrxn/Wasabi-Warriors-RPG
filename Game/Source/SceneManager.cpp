#include "SceneManager.h"

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGameplay.h"
#include "SceneEnding.h"
#include "SceneBattle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"

#include "GuiButton.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager(Input* input, Render* render, Textures* tex, EntityManager* entityman, GuiManager* guiManager) : Module()
{
	name.Create("scenemanager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;

	this->input = input;
	this->render = render;
	this->tex = tex;
	this->entityman = entityman;
	this->guiManager = guiManager;
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
		current->Load(tex, entityman, guiManager);
	}
	else 
	{
		current->Load(tex, guiManager);
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

				current->Unload(tex, guiManager);	// Unload current screen
				if (next->type == SceneType::GAMEPLAY)
				{
					next->Load(tex, entityman, guiManager);	// Load next screen
				}
				else
				{
					next->Load(tex, guiManager);	// Load next screen
				}

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
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
			case SceneType::BATTLE: next = new SceneBattle(); break;
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

	if (current != nullptr) current->Unload(tex, guiManager);

	return true;
}