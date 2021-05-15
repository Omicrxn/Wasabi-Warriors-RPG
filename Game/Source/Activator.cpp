#include "Activator.h"

#include "Render.h"
#include "Input.h"

Activator::Activator(SString name, Input* input, Textures* tex, Collisions* collisions, EntityManager* entityManager, EntityType type, EntitySubtype subtype, iPoint position) : Interactive()
{
    this->tex = tex;
    texture = NULL;
    this->input = input;
    this->position = position;
    this->type = type;
    this->name = name;
    this->subtype = subtype;
    width = 32;
    height = 32;
    active = true;
    collider = collisions->AddCollider({ position.x,position.y,width,height }, Collider::Type::ACTIVATOR, (Module*)entityManager);
}

Activator::~Activator()
{
}

void Activator::Interact()
{
    Notifier::GetInstance()->NotifyActivator();
    Notifier::GetInstance()->SetActivator(this);
    if (name != "secretRoom") collider->pendingToDelete = true;
}

bool Activator::Update(Input* input, float dt)
{
    return true;
}

bool Activator::Draw(Render* render)
{
    render->scale = 3;
    if (active) render->DrawTexture(texture, position.x, position.y, &animRec);
    render->scale = 1;

    return true;
}

void Activator::SetUpTexture(SString texPath)
{
    texture = tex->Load(texPath.GetString());
    // Define player textures / animations
    int textureStartYPos = spritePos * 32 * 5;
}

void Activator::SetName(SString name)
{
    this->name = name;
}

void Activator::OnCollision(Collider* collider)
{
    if (collider->type == Collider::Type::PLAYER)
    {
        if (input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) Interact();
    }
}