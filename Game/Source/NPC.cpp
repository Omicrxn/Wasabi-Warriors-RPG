#include "NPC.h"
#include "Render.h"

NPC::NPC() : Entity(EntityType::NPC)
{
    texture = NULL;
    position = iPoint(12 * 16, 27 * 16);
    currentAnim = NPCAnimations::IDLE;
    velocity = { 150.0f, 150.0f };
    width = 32;
    height = 32;
    direction = { 0,0 };
    active = true;
    stepsCounter = 0;
}

NPC::~NPC()
{

}

bool NPC::Update(Input* input, float dt)
{
    stepsCounter++;

    // Deciding a random direction if enough time has passed
    if (stepsCounter > 50)
    {
        stepsCounter = 0;

        // Assigning direction.x a random number between -1 and 1 (-1,0,1)
        int randNum = (rand() % 3) - 1; // Between -1 and 1
        direction.x = randNum;
        randNum = (rand() % 3) - 1; // Between -1 and 1
        direction.y = randNum;

        if (direction.x > 0 && direction.y == 0)
        {
            currentAnim = NPCAnimations::RIGHT;
        }
        else if (direction.x < 0 && direction.y == 0)
        {
            currentAnim = NPCAnimations::LEFT;
        }
        if (direction.y > 0 &&  direction.x == 0)
        {
            currentAnim = NPCAnimations::DOWN;
        }
        else if (direction.y < 0 && direction.x == 0)
        {
            currentAnim = NPCAnimations::UP;
        }

        if ((direction.x == 0 && direction.y == 0) || (direction.x != 0 && direction.y != 0))
        {
            currentAnim = NPCAnimations::IDLE;
        }
    }

    Walk(direction, dt);

    return true;
}

bool NPC::Draw(Render* render)
{
    animRec;
    switch (currentAnim)
    {
    case NPCAnimations::IDLE:
        animRec = idleAnim.GetFrame(0);
        break;
    case NPCAnimations::UP:
        animRec = walkUpAnim.GetCurrentFrame();
        break;
    case NPCAnimations::DOWN:
        animRec = walkDownAnim.GetCurrentFrame();
        break;
    case NPCAnimations::RIGHT:
        animRec = walkRightAnim.GetCurrentFrame();
        break;
    case NPCAnimations::LEFT:
        animRec = walkLeftAnim.GetCurrentFrame();
        break;
    default:
        break;
    }

    render->scale = 3;
    if (active) render->DrawTexture(texture, position.x, position.y, &animRec);
    render->scale = 1;

    return true;
}

void NPC::Walk(iPoint direction, float dt)
{
    if (direction.x != 0 && direction.y != 0)
    {
        // limit movement speed diagonally, so you move at 70% speed
        //velocity.x *= 0.7;
        //velocity.y *= 0.7;

        velocity = { 0,0 };
    }
    else
    {
        if (direction.x > 0) velocity.x = 75.0f;
        else if (direction.x < 0) velocity.x = 25.0f;
        else velocity.x = 0.0f;

        if (direction.y > 0) velocity.y = 75.0f;
        else if (direction.y < 0) velocity.y = 25.0f;
        else velocity.y = 0.0f;
    }

    position.x = position.x + direction.x * (velocity.x * dt);
    position.y = position.y + direction.y * (velocity.y * dt);
}

void NPC::SetUpTexture()
{
    // Define player textures / animations
    int textureStartYPos = spritePos * 32 * 5;
    for (int y = textureStartYPos; y < y + 160; y += 32)
    {
        for (int x = 0; x < 8 * 32; x += 32)
        {
            if (y == textureStartYPos && x == 128)
                break;
            if (y == textureStartYPos)
            {
                idleAnim.PushBack({ x,y,32,32 });
                idleAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32)
            {
                walkRightAnim.PushBack({ x,y,32,32 });
                walkRightAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32 * 2)
            {
                walkUpAnim.PushBack({ x,y,32,32 });
                walkUpAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32 * 3)
            {
                walkDownAnim.PushBack({ x,y,32,32 });
                walkDownAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32 * 4)
            {
                walkLeftAnim.PushBack({ x,y,32,32 });
                walkLeftAnim.speed = 0.2;
            }
        }
    }
}

void NPC::SetName(SString name)
{
    this->name = name;
}