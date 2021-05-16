#include "SecretWall.h"

SecretWall::SecretWall(SString name, Textures* tex, EntityManager* entityManager, EntityType type, iPoint position) : Interactive()
{
	this->name = name;
	this->tex = tex;
	this->entityManager = entityManager;
	this->type = type;
	this->subtype = subtype;
	this->position = position;
	
}

SecretWall::~SecretWall()
{}

bool SecretWall::Draw(Render * render)
{
	render->scale = 3;
	render->DrawTexture(entityManager->secretWallTexture, position.x, position.y, &rect);
	render->scale = 1;

	return true;
}

void SecretWall::SetName(SString name)
{
	this->name = name;
}

void SecretWall::SetLever(int leverNumber)
{
	switch (leverNumber)
	{
	case 1:

		lever1 = true;
		sequence.Add(leverNumber);
		break;


	case2:

		lever2 = true;
		sequence.Add(leverNumber);

		break;
	case3:

		lever3 = true;
		sequence.Add(leverNumber);
		break;
	default:
		break;
	}
}

void SecretWall::OnCollision(Collider* collider)
{
}

bool SecretWall::Update(float dt)
{
	if (lever1 == true && lever2 == true && lever3 == true)
	{
		if (sequence[0] == 2 && sequence[1] == 3 && sequence[2] == 1)
		{
			this->destroy = true;
		}
		else
		{
			reset = true;
		}
	}

	if (reset == true)
	{
		for (size_t i = 0; i < entityManager->leverList.Count(); i++)
		{
			entityManager->leverList.At(i)->data->Reset();
		}
		lever1 = false;
		lever2 = false;
		lever3 = false;
		this->reset == false;
		
	}

	return true;
}