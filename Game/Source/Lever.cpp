#include "Lever.h"
#include "SecretWall.h"
#include "Input.h"

Lever::Lever(SString name, Collisions* collisions, Input* input, Textures* tex, EntityManager* entityManager, EntityType type, iPoint position)
{
	this->name = name;
	this->tex = tex;
	this->entityManager = entityManager;
	this->type = type;
	this->subtype = subtype;
	this->position = position;
	this->secretWall = secretWall;
	this->number = number;
	this->input = input;
	width = 32;
	height = 32;
	collider = collisions->AddCollider({ position.x,position.y , width, height }, Collider::Type::LEVER, (Module*)entityManager);

	// Lever animation
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			leverAnim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	for (int j = 2; j >= 0; j--)
	{
		for (int i = 4; i >= 0; i--)
		{
			leverAnim.PushBack({ i * 192, j * 192, 192, 192 });
		}
	}
	leverAnim.speed = 1.0f;
	leverAnim.loop = true;
}

Lever::~Lever()
{
}

bool Lever::Update(float dt)
{
	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y);
	}
	return true;
}

bool Lever::Draw(Render* render)
{
	if(!hasInteracted)
		render->DrawTexture(entityManager->leversTexture, position.x * 3 - 48, position.y * 3 - 64, &leverAnim.GetCurrentFrame());

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

uint Lever::GetNumber()
{
	return uint();
}

void Lever::Interact()
{
	secretWall->SetLever(number);
	hasInteracted = true;
}

void Lever::OnCollision(Collider* collider)
{
	if (!hasInteracted)
	{
		if (input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			Interact();
		}
	}
	
}

void Lever::Reset()
{
	hasInteracted = false;
}
