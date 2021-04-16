#pragma once

#include "Module.h"
#include <list>
#include "SDL/include/SDL_timer.h"

struct SDL_Texture;
struct SDL_Rect;

enum class SplineType {

	EASE,
	EXPO,
	CIRC,
	QUINT,
	QUART,
	QUAD,
	BACK,
	ELASTIC,
	CUBIC,



	NONE
};

struct Esingfunctions {

	int Ease(float& timePassed, const int& origin, const int& finish, const float& time);
	int QuintEase(float& timePassed, const int& origin, const int& finish, const float& time);
	int CircEase(float& timePassed, const int& origin, const int& finish, const float& time);
	int BackEase(float& timePassed, const int& origin, const int& finish, const float& time);
	int QuartEase(float& timePassed, const int& origin, const int& finish, const float& time);
	int QuadEase(float& timePassed, const int& origin, const int& finish, const float& time);
	int ExpoEase(float& timePassed, const int& origin, const int& finish, const float& time);
	int CubicEase(float& timePassed, const int& origin, const int& finish, const float& time);

};

struct SplineInfo {

	int* position = nullptr;
	int initialPosition = 0;
	int finalPosition = 0;

	float totalTime = 0.0F;
	float timePassed = 0.0F;

	SplineType type;
	Esingfunctions functions;

	bool Update(float dt);

	SplineInfo(int* position, const int& finalPosition, const float& totalTime, const SplineType& type) {

		// TODO 1: Create the constructor saving all the values given. The timePassed is given by the internal SDL ticks.

		this->position = position;

		this->initialPosition = *position;

		this->finalPosition = finalPosition - *position;

		this->type = type;

		this->totalTime = totalTime;

		timePassed = SDL_GetTicks();
	}
};

class Easing : public Module
{
public:

	Easing();

	// Destructor
	virtual ~Easing();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void CreateSpline(int* position, const int& finalPos, const float& time, const SplineType& type);


private:

	std::list<SplineInfo*> splines;


};






