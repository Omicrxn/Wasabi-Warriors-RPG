#pragma once
#include "Interactive.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Render.h"
#include "SecretWall.h"
class Lever : public Interactive
{
public:

	Lever(SString name, Textures* tex, EntityManager* entityManager, EntityType type, iPoint position)
	{
		this->name = name;
		this->tex = tex;
		this->entityManager = entityManager;
		this->type = type;
		this->subtype = subtype;
		this->position = position;
		this->secretWall = secretWall;
		this->number = number;
	}
	~Lever() {}

	bool Update(float dt) 
	{
		return true;
	}
	bool Draw(Render* render)
	{
		render->DrawTexture(entityManager->itemsTexture, position.x, position.y, &rect);
		return true;
	}
	void SetName(SString name);
	void SetSecretWall(SecretWall* secretWall) { this->secretWall = secretWall; }
	void SetNumber(uint number) { this->number = number; }

private:

	void Interact() 
	{
		secretWall->SetLever(number);
	};
	void OnCollision(Collider* collider) override { Interact(); }
	void Reset()
	{

	}

private:

	SDL_Rect rect;
	SecretWall* secretWall;
	uint number = 0;
};