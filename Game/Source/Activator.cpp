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
    Notifier::GetInstance()->SetActivatorMode(true);
    Notifier::GetInstance()->SetActivator(this);
    if (name != "secretRoom" && name != "second_secretRoom" && name != "third_secretRoom") collider->pendingToDelete = true;
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

    if (drawState != ActivatorState::NONE)
    {
        if (drawState == ActivatorState::MAP)
        {
            render->DrawTexture(entityManager->itemsTexture, position.x * 3 + 32, position.y * 3 + 32, &rect);
        }
        else if (drawState == ActivatorState::PICKED)
        {
            /*iPoint hudPosition;
            if (name == "key")
            {
                hudPosition = { 20,200 };
            }
            else if (name == "Schichimi")
            {
                hudPosition = { 1140,50 };
            }
            else if (name == "Karashi")
            {
                hudPosition = { 1140,100 };
            }
            else if (name == "Rayu")
            {
                hudPosition = { 1140,150 };
            }
            else if (name == "Sushi")
            {
                hudPosition = { 1200,50 };
            }
            else if (name == "Oniguiri")
            {
                hudPosition = { 1200,100 };
            }
            else if (name == "Sashimi")
            {
                hudPosition = { 1200,150 };
            }

            if (name == "key")
            {
                render->scale = 3;
                render->DrawTexture(entityManager->itemsTexture, hudPosition.x, hudPosition.y, &rect, 0);
                render->scale = 1;
            }
            else
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

void Activator::SetDrawState(ActivatorState drawState)
{
    this->drawState = drawState;
    if (drawState == ActivatorState::PICKED)
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

ActivatorState Activator::GetDrawState()
{
    return drawState;
}

void Activator::OnCollision(Collider* collider)
{
    if (!hasInteracted)
    {
        if (collider->type == Collider::Type::PLAYER && (name == "dialogTrigger" || name == "erikaTomb" || name == "secretRoomDialog" || name == "dungeon1" || name == "dungeon2" || name == "dungeon3" || name == "dungeon4" || name == "dungeon5"))
        {
            Interact();
        }
        else 
        {
            if (input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN)
            {
                if (name == "key") audio->PlayFx(entityManager->keyFx);
                Interact();
            }
        }
    }

    if ((drawState == ActivatorState::MAP || drawState == ActivatorState::NONE) && !Notifier::GetInstance()->GetInteractionNotifier())
    {
        Notifier::GetInstance()->NotifyInteraction();
        Notifier::GetInstance()->SetInteractingEntity((Entity*)this);
    }
}