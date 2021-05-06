#pragma once
#ifndef __NPC_H__
#define __NPC_H__

#include "Interactive.h"
#include "Animation.h"
#include "Textures.h"
#include "EntityManager.h"

enum class NPCAnimations
{
	IDLE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class NPC : public Interactive
{
public:

	NPC(Textures* tex, Collisions* collisions, EntityManager* entityManager, EntitySubtype subtype);
	~NPC();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);

	void SetUpTexture(SString texPath);
	void SetName(SString name);

private:

	void Walk(iPoint direction, float dt);
	void Interact();
	void OnCollision(Collider* collider) override;

public:

	SString name; // NPC name
	fPoint velocity;
	iPoint direction;
	Animation idleAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation walkUpAnim;
	Animation walkDownAnim;
	NPCAnimations currentAnim;
	SDL_Rect animRec;

	int width, height;

	uint stepsCounter;
	bool stop = false;
};

#endif //__NPC_H__