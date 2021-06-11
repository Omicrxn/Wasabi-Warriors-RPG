#pragma once
#ifndef __ACTIVATOR_H__
#define __ACTIVATOR_H__

#include "Interactive.h"
#include "Animation.h"
#include "Textures.h"
#include "EntityManager.h"

enum class ActivatorState
{
	NONE = -1,
	MAP = 0,
	PICKED
};

class Activator : public Interactive
{
public:

	Activator(SString name, Input* input, Textures* tex, AudioManager* audio, Collisions* collisions, EntityManager* entityManager, EntityType type, EntitySubtype subtype, iPoint position);
	~Activator();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);

	void SetUpTexture(SString texPath);
	void SetName(SString name);

	void SetDrawState(ActivatorState drawState);
	ActivatorState GetDrawState();

	virtual SDL_Rect GetRect() const { return rect; };

private:

	void Interact();
	void OnCollision(Collider* collider) override;

private:

	SDL_Rect rect;
	ActivatorState drawState = ActivatorState::NONE;
};

#endif //__ACTIVATOR_H__