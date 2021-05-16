#include "Lever.h"
#include "SecretWall.h"

Lever::Lever(SString name, Textures* tex, EntityManager* entityManager, EntityType type, iPoint position)
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

Lever::~Lever()
{
}

bool Lever::Update(float dt)
{
	return true;
}

bool Lever::Draw(Render* render)
{
	render->DrawTexture(entityManager->itemsTexture, position.x, position.y, &rect);
	return true;
}

void Lever::SetName(SString name)
{
	this->name = name;
}

void Lever::SetSecretWall(SecretWall* secretWall)
{
	this->secretWall = secretWall;
}

void Lever::SetNumber(uint number)
{
	this->number = number;
}

void Lever::Interact()
{
	secretWall->SetLever(number);
}

void Lever::OnCollision(Collider* collider)
{
	Interact();
}

void Lever::Reset()
{
}
