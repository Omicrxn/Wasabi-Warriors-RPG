#include <math.h>
#include "App.h"
#include "Transitions.h"
#include "Render.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "Map.h"
#include "Textures.h"
#include "Audio.h"
#include "Map.h"
#include "Window.h"
#include "Scene.h"

// Include scenes
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGameplay.h"

Transitions::Transitions(Render* render, Window* win)
{
	screen = { 0, 0, 1280,720 };
	wipeTransRect = { -1280, 0, 1280,720 };

	// Saving needed modules
	this->render = render;
	this->win = win;
}

Transitions::~Transitions()
{}

// Load assets
bool Transitions::Start()
{
	SDL_SetRenderDrawBlendMode(render->renderer, SDL_BLENDMODE_BLEND);
	curtainTransRect = { -(int)win->GetWidth() / 2, 0, (int)win->GetWidth() / 2,720 };
	curtainTransRect2 = { (int)win->GetWidth(), 0, (int)win->GetWidth() / 2,720 };

	return true;
}

// Update: draw background
bool Transitions::PostUpdate()
{
	if (ongoingstep == FadeStep::NONE)
		return true;

	float normalized = MIN(1.0f, (float)timer.ReadSec() / (float)globaltime);

	switch (ongoingstep)
	{
	case FadeStep::ENTERING:
	{
		if (timer.ReadSec() >= globaltime)
		{
			/*moduleOff->Disable();
			moduleOn->Enable();*/

			if (sceneChange) moduleOff->TransitionToScene(nextSceneType);

			if (enemy != nullptr)
			{
				enemy->StartCombat();
				enemy = nullptr;
			}
			
			timer.Start();
			ongoingstep = FadeStep::EXITING;
		}
	} break;

	case FadeStep::EXITING:
	{
		normalized = 1.0f - normalized;

		if (timer.ReadSec() >= globaltime)
			ongoingstep = FadeStep::NONE;
	} break;
	}

	//the following switch decides which animation we're doing
	switch (wAnim)
	{
		//FADE TO BLACK TRANSITION
	case(WhichAnimation::FADE_TO_BLACK):
		// Finally render the black square with alpha on the screen
		SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
		SDL_RenderFillRect(render->renderer, &screen);
		break;

		//FADE TO WHITE TRANSITION
	case(WhichAnimation::FADE_TO_WHITE):
		// Finally render the white square with alpha on the screen
		SDL_SetRenderDrawColor(render->renderer, 255, 255, 255, (Uint8)(normalized * 255.0f));
		SDL_RenderFillRect(render->renderer, &screen);
		break;

		//WIPE TRANSITION
	case (WhichAnimation::WIPE):
		if (ongoingstep == FadeStep::ENTERING)
		{
			percentatge = timer.ReadSec() * (1 / (globaltime));
			float normalizedXPosition = LerpValue(percentatge, -(int)win->GetWidth(), 0);

			if (normalizedXPosition >= 0)
			{
				wipeTransRect.x = 0;
			}
			else wipeTransRect.x = normalizedXPosition;
		}
		else if (ongoingstep== FadeStep::EXITING)
		{
			percentatge = timer.ReadSec() * (1 / globaltime);
			float normalizedXPosition2 = LerpValue(percentatge, 0, -1280);

			if (normalizedXPosition2 <= -1280)
			{
				wipeTransRect.x = -1280;
			}else wipeTransRect.x = normalizedXPosition2;
		}
		//Finally we draw the two rect that will make the wipe
		SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(render->renderer, &wipeTransRect);
		break;

		//ZOOM TRANSITION
	case (WhichAnimation::ZOOM):

		if (ongoingstep == FadeStep::ENTERING) {
			percentatge2 = timer.ReadSec() * (1 / globaltime);
			float normalizedScale = LerpValue(percentatge2, normalScale, targetScale);

			float nextWidth = LerpValue(percentatge2, originalWidth, finalWidth);
			float nextHeight = LerpValue(percentatge2, originalHeight, finalHeight);

			float stepX = nextWidth - currentWidth;
			float stepY = nextHeight - currentHeight;

			render->camera.x += stepX;
			render->camera.y += stepY;

			currentScale = normalizedScale;
			SDL_RenderSetScale(render->renderer, normalizedScale, normalizedScale);
			currentHeight = nextHeight;
			currentWidth = nextWidth;
		}

		if (ongoingstep == FadeStep::EXITING)
		{
			// Once we have done the zoom, we set the original scale for the next scene
			SDL_RenderSetScale(render->renderer, 1, 1);
			
		}
		break;

		//CURTAIN TRANSITION
	case (WhichAnimation::CURTAIN):
		if (ongoingstep == FadeStep::ENTERING)
		{
			percentatge = timer.ReadSec() * (1 / (globaltime));
			float normalizedXPositionCurtain1 = LerpValue(percentatge, -(int)win->GetWidth() / 2, 0);

			if (normalizedXPositionCurtain1 >= 0) {
				curtainTransRect.x = 0;
			}
			else curtainTransRect.x = normalizedXPositionCurtain1;

			float normalizedXPositionCurtain2 = LerpValue(percentatge, (int)win->GetWidth(), (int)win->GetWidth() / 2);

			if (normalizedXPositionCurtain2 <= (int)win->GetWidth() / 2) {
				curtainTransRect2.x = (int)win->GetWidth() / 2;
			}
			else curtainTransRect2.x = normalizedXPositionCurtain2;
		}
		else if (ongoingstep == FadeStep::EXITING)
		{
			percentatge = timer.ReadSec() * (1 / (globaltime));
			float normalizedXPositionCurtain1 = LerpValue(percentatge, 0, -(int)win->GetWidth() / 2);

			if (normalizedXPositionCurtain1 <= -(int)win->GetWidth() / 2)
			{
				curtainTransRect.x = -(int)win->GetWidth() / 2;
			}
			else curtainTransRect.x = normalizedXPositionCurtain1;

			float normalizedXPositionCurtain2 = LerpValue(percentatge, (int)win->GetWidth() / 2, (int)win->GetWidth());

			if (normalizedXPositionCurtain2 >= (int)win->GetWidth())
			{
				curtainTransRect2.x = (int)win->GetWidth();
			}
			else curtainTransRect2.x = normalizedXPositionCurtain2;
		}

		// Finally we draw the two rects that will make the curtains
		SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(render->renderer, &curtainTransRect);

		SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(render->renderer, &curtainTransRect2);

		break;
	}

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool Transitions::Transition(WhichAnimation type, Scene* moduleOffP, SceneType sceneType, float time, float targetScaleP)
{
	bool ret = false;

	wAnim = type;

	if (ongoingstep == FadeStep::NONE)
	{
		moduleOff = moduleOffP;
		nextSceneType = sceneType;
		ongoingstep = FadeStep::ENTERING;
		// startTime = SDL_GetTicks();
		globaltime = time;
		timer.Start();
		
		// For zoom
		targetScale = targetScaleP;
		originalWidth = render->camera.w;
		originalHeight = render->camera.h;
		finalWidth = render->camera.w / targetScaleP;
		finalHeight = render->camera.h / targetScaleP;
		currentWidth = render->camera.w;
		currentHeight = render->camera.h;

		ret = true;
	}

	sceneChange = true;

	return ret;
}

bool Transitions::TransitionCombat(WhichAnimation type, Enemy* enemy, float time, float targetScaleP)
{
	bool ret = false;

	wAnim = type;
	this->enemy = enemy;

	if (ongoingstep == FadeStep::NONE)
	{
		ongoingstep = FadeStep::ENTERING;
		// startTime = SDL_GetTicks();
		globaltime = time;
		timer.Start();

		// For zoom
		targetScale = targetScaleP;
		originalWidth = render->camera.w;
		originalHeight = render->camera.h;
		finalWidth = render->camera.w / targetScaleP;
		finalHeight = render->camera.h / targetScaleP;
		currentWidth = render->camera.w;
		currentHeight = render->camera.h;

		ret = true;
	}

	sceneChange = false;

	return ret;
}

float Transitions::LerpValue(float percent, float start, float end)
{
	return start + percent * (end - start);
}

void Transitions::SetTargetScale(int targetScaleP)
{
	targetScale = targetScaleP;
}