#pragma once
#ifndef __ACTIVATOR_H__
#define __ACTIVATOR_H__

#include "Interactive.h"
#include "Animation.h"
#include "Textures.h"
#include "EntityManager.h"

class Activator : public Interactive
{
public:

	Activator(SString name, Input* input, Textures* tex, Collisions* collisions, EntityManager* entityManager, EntityType type, EntitySubtype subtype, iPoint position);
	~Activator();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);

	void SetUpTexture(SString texPath);
	void SetName(SString name);

private:

	void Interact();
	void OnCollision(Collider* collider) override;

public:

	SString name; // Activator name

	int width, height;
};

#endif //__ACTIVATOR_H__