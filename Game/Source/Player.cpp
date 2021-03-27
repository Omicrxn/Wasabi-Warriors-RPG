#include "Player.h"

Player::Player(Textures* tex) : Being()
{
    texture = NULL;
    position = iPoint(12 * 16, 27 * 16);
    currentAnim = PlayerAnim::IDLE;
    velocity = { 150.0f, 150.0f };
    width = 32;
    height = 32;
    direction = { 0,0 };
    // Define Player animations
}

bool Player::Update(Input* input, float dt)
{
    Walk(direction, dt);
    if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
    {
        direction.y = -1;
        currentAnim = PlayerAnim::UP;
    }else if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
    {
        direction.y = 1;
        currentAnim = PlayerAnim::DOWN;
    }else if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    {
        direction.x = -1;
        currentAnim = PlayerAnim::LEFT;
    }else if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    {
        direction.x = 1;
        currentAnim = PlayerAnim::RIGHT;
    }else if (input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
        && input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
    {
        direction = { 0,0 };
        currentAnim = PlayerAnim::IDLE;
    }

    return true;
}

bool Player::Draw(Render* render)
{
    SDL_Rect animRec;
    switch (currentAnim)
    {
    case PlayerAnim::IDLE:
        animRec = idleAnim.GetFrame(0);
        break;
    case PlayerAnim::UP:
        animRec = walkUpAnim.GetCurrentFrame();
        break;
    case PlayerAnim::DOWN:
        animRec = walkDownAnim.GetCurrentFrame();
        break;
    case PlayerAnim::RIGHT:
        animRec = walkRightAnim.GetCurrentFrame();
        break;
    case PlayerAnim::LEFT:
        animRec = walkLeftAnim.GetCurrentFrame();
        break;
    default:
        break;
    }
    render->scale = 3;
    render->CameraFollow(position.x, position.y);
    render->DrawTexture(texture, position.x, position.y, &animRec);

    render->scale = 1;
    return false;
}

void Player::SetTexture(SDL_Texture *tex, int spritePos)
{
    int textureStartYPos = spritePos * 32 * 5;
    texture = tex;
    for (int y = textureStartYPos; y < y+160; y+=32)
    {
        
        for (int x = 0; x < 8*32; x+=32)
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
            else if (y == textureStartYPos + 32*2)
            {
                walkUpAnim.PushBack({ x,y,32,32 });
                walkUpAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32*3)
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
void Player::Walk(iPoint direction, float dt)
{
    position.x = position.x + direction.x * (velocity.x*dt);
    position.y = position.y + direction.y * (velocity.y*dt);
}
SDL_Rect Player::GetBounds()
{
    return { (int)position.x, (int)position.y, width, height };
}
