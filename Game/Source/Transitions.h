#ifndef __TRANSITIONS_H__
#define __TRANSITIONS_H__

#include "Module.h"
#include "Timer.h"
#include "SDL\include\SDL_rect.h"

class Scene;
enum class SceneType;
class Enemy;
struct SDL_Texture;
class SceneGameplay;

//this enum will manage all the transitions, using it to indicate which type of transition we're using
enum WhichAnimation
{
	NONE,
	FADE_TO_BLACK,
	FADE_TO_WHITE,
	WIPE,
	ZOOM,
	CURTAIN
};

class Transitions : public Module
{
public:

	// Main functions
	Transitions(Render* render, Textures* tex, Window* win);
	~Transitions();

	bool Start();
	bool PostUpdate();

public:

	// This enum indicates the situation of the transition/effect we are in
	enum FadeStep
	{
		NONE,
		ENTERING,
		EXITING
	} ongoingstep = FadeStep::NONE;

	bool Transition(WhichAnimation type, Scene* moduleOffP, SceneType sceneType, float time = 2, float targetScaleP = 2);
	bool TransitionCombat(WhichAnimation type, Enemy* enemy, float time = 2, float targetScaleP = 2);
	bool TransitionMap(WhichAnimation type, SceneGameplay* sceneGameplay, float time = 2, float targetScaleP = 2);
	
	// The following calculates the percentatge of the transition that is done
	float LerpValue(float percent, float start, float end);

	// For the zoom camera transition, it sets the camera scale that we will get at the final of the zoom transition
	void SetTargetScale(int targetScaleP);
	
private:

	WhichAnimation wAnim = WhichAnimation::NONE;

	// Deactivate and activate the scenes
	SceneType nextSceneType;
	Scene* moduleOff = nullptr;

	// Used for calculating the percentage of screen doing the effect of the transition
	// We calculate it using the time of the transition and position of our rects that we create during it
	float percentatge = 0;
	float globaltime = 0;
	Timer timer;

	// Used for the renders and the transitions functions
	SDL_Rect screen;
	SDL_Rect wipeTransRect;
	SDL_Rect wipeTransTexRect;
	SDL_Rect curtainTransRect;
	SDL_Rect curtainTransRect2;
	
	// For the zoom transition
	int originalWidth;
	int originalHeight;
	int finalWidth;
	int finalHeight;
	int currentWidth;
	int currentHeight;
	float currentScale = 1;
	float targetScale;
	float normalScale = 1;
	float percentatge2 = 0;
	float percent3 = 0;

	// Needed modules
	Render* render = nullptr;
	Window* win = nullptr;
	Textures* tex;

	bool sceneChange = false;
	
	// Needed variables for specific transitions
	// Combat pending enemy
	Enemy* enemy = nullptr;
	// Scene gameplay instance to acces its info.
	SceneGameplay* sceneGameplay = nullptr;

	// Needed textures for transitions
	SDL_Texture* wipe = nullptr;
};

#endif // __TRANSITIONS_H__