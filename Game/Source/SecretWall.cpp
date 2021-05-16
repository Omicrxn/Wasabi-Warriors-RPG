#include "SecretWall.h"
#include "Lever.h"
#include "Collisions.h"
SecretWall::SecretWall(SString name,Collisions* collisions, Textures* tex, EntityManager* entityManager, EntityType type, iPoint position) : Interactive()
{
	this->name = name;
	this->tex = tex;
	this->entityManager = entityManager;
	this->type = type;
	this->subtype = subtype;
	this->position = position;
	width = 32;
	height = 68;
	this->rect = { 0,0,width,height };
	collider = collisions->AddCollider({ position.x,position.y , width, height }, Collider::Type::SECRET_WALL, (Module*)entityManager);

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


	case 2:

		lever2 = true;
		sequence.Add(leverNumber);

		break;
	case 3:

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
	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y);
	}
	if (lever1 == true && lever2 == true && lever3 == true)
	{
		if (sequence.At(0)->data == 2 && sequence.At(1)->data == 3 && sequence.At(2)->data == 1)
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
		sequence.Clear();
		reset = false;
		
	}

	return true;
}