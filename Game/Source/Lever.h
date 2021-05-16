#pragma once
#include "Interactive.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Render.h"

class Input;
class SecretWall;

class Lever : public Interactive
{
public:

	Lever(SString name,Collisions* collisions, Input* input, Textures* tex, EntityManager* entityManager, EntityType type, iPoint position);
	~Lever();

	bool Update(float dt);
	bool Draw(Render* render);
	void SetName(SString name);
	void SetSecretWall(SecretWall* secretWall);
	void SetNumber(uint number);
	uint GetNumber();
	void Reset();

private:

	void Interact();
	void OnCollision(Collider* collider) override;


private:

	SecretWall* secretWall;
	uint number = 0;
	Input* input = nullptr;
	Animation leverAnim;

public:

	SDL_Rect rect;
};