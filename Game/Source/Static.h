#ifndef __STATIC_H__
#define __STATIC_H__

#include "Entity.h"

struct SDL_Texture;

class Static : public Entity
{

public:

	enum class Type
	{
		TREE,
		ROOF,
		CUP,
		SIGNE,
		MACHINE,
		WINDOW,
		LIGHT,
		BIN,
		STATUE,
		STONE,
		ANTENNA,
		SIGNAL,
		TABLE,
		DOOR,
		PLATE,
		CASH,
		FOOD,
		LAMP,
		FURNITURE,
		FENCE,
		FLOATING,
		ROAD,
		BUSH,
		FLOOR,
		UNKNOWN
	};

public:

	Static(std::string name, Textures* tex, EntityManager* entityManager, iPoint position);

	~Static();

	//void OnCollision(Collider*, Collider*, float dt);

private:

	bool Draw(Render* render);
	void SetRect(int x, int y, int w, int h);

private:
	SDL_Rect frame;
	Type type;
};

#endif