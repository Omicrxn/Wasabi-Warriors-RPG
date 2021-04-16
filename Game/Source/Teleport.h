#pragma once
#include "Interactive.h"
#include "EntityManager.h"
class Teleport : public Interactive
{
public:

	Teleport(Collisions* collisions, EntityManager* entityManager);
	~Teleport();

	bool Update(Input* input, float dt);
	void SetName(SString name){ this->name = name; }
	SDL_Rect GetBounds() { return { position.x,position.y,width,height }; }
	void SetUpDestination(MapType destination);
private:
	void Interact();
	void OnCollision(Collider* collider) override;

	MapType destination = MapType::NONE;
	SString name; // NPC name

	int width = 32, height = 32;

	uint stepsCounter;
};