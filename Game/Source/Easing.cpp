#include "Log.h"
#include "Easing.h"

Easing::Easing() : Module()
{
	name.Create("easing");

}

Easing::~Easing()
{

}

bool Easing::Update(float dt)
{
	std::list<SplineInfo*>::iterator item = splines.begin();

	for (; item != splines.end(); ++item) {

		if (*item != nullptr) {

			if (!(*item)->Update(dt)) {

				delete(*item);
				(*item) = nullptr;

			}

		}

	}

	splines.remove(nullptr);

	return true;
}

bool Easing::CleanUp()
{
	std::list<SplineInfo*>::iterator item = splines.begin();

	for (; item != splines.end(); ++item) {

		if (*item != nullptr) {

			delete(*item);
			(*item) = nullptr;

		}

	}

	splines.clear();

	return true;
}

void Easing::CreateSpline(int* position, const int& finaPos, const float& time, const SplineType& type)
{

	SplineInfo* info = new SplineInfo(position, finaPos, time, type);

	if (info != nullptr)
		splines.push_back(info);
	else
		LOG("Error when creating the spline");
}




bool SplineInfo::Update(float dt)
{
	bool ret = true;

	// TODO 2: Calculate the time that has passed since the function has started, so we can track the spline. It uses also the SDL ticks
	float timeCounter = SDL_GetTicks() - timePassed;

	// TODO 3: We need a way to know when the whole movement has finished, so we shoud get a conditional
	// With the variables that we have and the ones that we already implemented
	if (timeCounter < totalTime) {
		// TODO 4:: inside the previous Todo's conditional we should create a switch that selects the funcion for the respective spline type.
		// The funcions are defiuned below and the types are in the .h
		switch (type) {
		case SplineType::EASE: {
			*position = functions.Ease(timeCounter, initialPosition, finalPosition, totalTime);
		} break;
		case SplineType::QUINT: {
			*position = functions.QuintEase(timeCounter, initialPosition, finalPosition, totalTime);
		} break;
		case SplineType::BACK: {
			*position = functions.BackEase(timeCounter, initialPosition, finalPosition, totalTime);
		} break;
		case SplineType::QUART: {
			*position = functions.QuartEase(timeCounter, initialPosition, finalPosition, totalTime);
		} break;
		case SplineType::QUAD: {
			*position = functions.QuadEase(timeCounter, initialPosition, finalPosition, totalTime);
		} break;
		case SplineType::EXPO: {
			*position = functions.ExpoEase(timeCounter, initialPosition, finalPosition, totalTime);
		} break;
		case SplineType::CUBIC: {
			*position = functions.CubicEase(timeCounter, initialPosition, finalPosition, totalTime);
		} break;
		case SplineType::CIRC: {
			*position = functions.CircEase(timeCounter, initialPosition, finalPosition, totalTime);
		} break;
		default:
			LOG("No valid SplineType");
			break;
		}
	}
	else
		ret = false;

	return ret;
}

int Esingfunctions::Ease(float& timePassed, const int& origin, const int& finish, const float& time)
{
	//We use this variable to save the value of the time in the formula. I use this for all the next functions to keep the information separated.
	timePassed /= time;

	return finish * timePassed + origin;
}

int Esingfunctions::QuintEase(float& timePassed, const int& origin, const int& finish, const float& time)
{
	//Quint function with an easy out implemented
	timePassed /= time;
	timePassed--;
	return finish * (timePassed * timePassed * timePassed * timePassed * timePassed + 1) + origin;;
}

int Esingfunctions::CircEase(float& timePassed, const int& origin, const int& finish, const float& time)
{
	return -finish * (sqrt(1 - (timePassed /= time) * timePassed) - 1) + origin;
}

int Esingfunctions::BackEase(float& timePassed, const int& origin, const int& finish, const float& time)
{
	//Cubic function with an extra movement to the back before starting the forward movement. Its like anticipation in animation.

	int pos = 0;
	float s = 1.0f;
	if ((timePassed /= time / 2) < 1) {
		pos = finish / 2 * (timePassed * timePassed * (((s *= (0.9f)) + 1) * timePassed - s)) + origin;
	}
	else {
		float postFix = timePassed -= 2;
		pos = finish / 2 * ((postFix)*timePassed * (((s *= (0.9f)) + 1) * timePassed + s) + 2) + origin;
	}
	return pos;
}

int Esingfunctions::QuartEase(float& timePassed, const int& origin, const int& finish, const float& time)
{
	timePassed /= time;
	timePassed--;

	return -finish * (timePassed * timePassed * timePassed * timePassed - 1) + origin;
}

int Esingfunctions::QuadEase(float& timePassed, const int& origin, const int& finish, const float& time)
{
	timePassed /= time;
	return finish * timePassed * timePassed + origin;
}

int Esingfunctions::ExpoEase(float& timePassed, const int& origin, const int& finish, const float& time)
{

	//This function has the same principle as the BackEase, but without the previous movement. It accelerates until the middle point and then slows down.
	int pos = 0;

	if ((timePassed /= time / 2) < 1) {
		pos = finish / 2 * pow(2, 10 * (timePassed - 1)) + origin;
	}
	else {
		pos = finish / 2 * (-pow(2, -10 * --timePassed) + 2) + origin;
	}
	return pos;
}


int Esingfunctions::CubicEase(float& timePassed, const int& origin, const int& finish, const float& time)
{
	//deacceleration on the end
	timePassed /= time;
	timePassed--;
	return finish * (timePassed* timePassed * timePassed + 1) + origin;
}


