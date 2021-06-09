#include "Activator.h"

#include "Render.h"
#include "Input.h"
#include "EntityManager.h"
#include "Audio.h"

Activator::Activator(SString name, Input* input, Textures* tex, AudioManager* audio, Collisions* collisions, EntityManager* entityManager, EntityType type, EntitySubtype subtype, iPoint position) : Interactive()
{
    this->name = name;
    this->input = input;
    this->tex = tex;
    texture = nullptr;
    this->audio = audio;
    this->entityManager = entityManager;
    this->position = position;
    this->type = type;
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
    hasInteracted = true;
}

bool Activator::Update(Input* input, float dt)
{
    if (collider != NULL)
    {
        collider->rect.w = width;
        collider->rect.h = height;
    }
    return true;
}

bool Activator::Draw(Render* render)
{
    /*render->scale = 3;
    if (active) render->DrawTexture(texture, position.x, position.y, &animRec);
    render->scale = 1;*/

    if (drawState != DrawState::NONE)
    {
        if (drawState == DrawState::MAP)
        {
            render->DrawTexture(entityManager->itemsTexture, position.x * 3 + 32, position.y * 3 + 32, &rect);
        }
        else if (drawState == DrawState::HUD)
        {
            iPoint hudPosition;
            if (name == "key")
            {
                hudPosition = { 20,200 };
            }
            else if (name == "attack_1")
            {
                hudPosition = { 1140,50 };
            }
            else if (name == "attack_2")
            {
                hudPosition = { 1140,100 };
            }
            else if (name == "attack_3")
            {
                hudPosition = { 1140,150 };
            }
            else if (name == "defense_1")
            {
                hudPosition = { 1200,50 };
            }
            else if (name == "defense_2")
            {
                hudPosition = { 1200,100 };
            }
            else if (name == "defense_3")
            {
                hudPosition = { 1200,150 };
            }

            if (name == "key")
            {
                render->scale = 3;
                render->DrawTexture(entityManager->itemsTexture, hudPosition.x, hudPosition.y, &rect, 0);
                render->scale = 1;
            }
            /*else
            {
                render->DrawTexture(entityManager->itemsTexture, hudPosition.x, hudPosition.y, &rect, 0);
            }*/
        }
    }

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

void Activator::SetDrawState(DrawState drawState)
{
    this->drawState = drawState;
    if (drawState == DrawState::HUD)
    {
        if (collider != nullptr) 
            collider->pendingToDelete = true;
    }
       
    if (name == "key")
    {
        rect = { 296,346,30,28 };
    }
    else if (name == "Schichimi")
    {
        rect = { 446,680,22,32 };
    }
    else if (name == "Karashi")
    {
        rect = { 494,680,22,32 };
    }
    else if (name == "Rayu")
    {
        rect = { 542,680,22,32 };
    }
    else if (name == "Sushi")
    {
        rect = { 590,680,22,32 };
    }
    else if (name == "Oniguiri")
    {
        rect = { 638,680,22,32 };
    }
    else if (name == "Sashimi")
    {
        rect = { 686,680,22,32 };
    }
}

DrawState Activator::GetDrawState()
{
    return drawState;
}

void Activator::OnCollision(Collider* collider)
{
    if (!hasInteracted)
    {
        if (collider->type == Collider::Type::PLAYER && name != "dialogTrigger")
        {
            if (input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
            {
                if (name == "key") audio->PlayFx(entityManager->keyFx);
                Interact();
            }
        }
        else {
            Interact();
        }
    }

    if (drawState == DrawState::MAP && !Notifier::GetInstance()->GetInteractionNotifier())
    {
        Notifier::GetInstance()->NotifyInteraction();
        Notifier::GetInstance()->SetInteractingEntity((Entity*)this);
    }
}