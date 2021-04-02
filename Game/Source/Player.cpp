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

    // Define player parameters
    this->stats.name = "Player";
    this->stats.level = 1;
    this->stats.damage = 10;
    this->stats.maxHP = 120;
    this->stats.currentHP = 100;
    this->stats.strength = 10;
    this->stats.defense = 10;
    this->stats.attackSpeed = 5;
    this->stats.criticalRate = 10;
}

bool Player::Update(Input* input, float dt)
{
    Walk(direction, dt);
    direction.x = input->GetAxisRaw(AxisName::HORIZONTAL);
    direction.y = input->GetAxisRaw(AxisName::VERTICAL);
   
    if (direction.x > 0)
    {
        currentAnim = PlayerAnim::RIGHT;
    }else if(direction.x < 0)
    {
        currentAnim = PlayerAnim::LEFT;
    }
    if (direction.y > 0)
    {
        currentAnim = PlayerAnim::DOWN;
    }else if (direction.y < 0)
    {
        currentAnim = PlayerAnim::UP;
    }
    if ((direction.x == 0 && direction.y == 0) || (direction.x != 0 && direction.y != 0))
    {
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
    if (direction.x != 0 && direction.y != 0)
    {
        // limit movement speed diagonally, so you move at 70% speed
        //velocity.x *= 0.7;
        //velocity.y *= 0.7;
     
        velocity = { 0,0 };
    }
    else
    {
        velocity = { 150.0f, 150.0f };
    }

    position.x = position.x + direction.x * (velocity.x * dt);
    position.y = position.y + direction.y * (velocity.y * dt);
}

SDL_Rect Player::GetBounds()
{
    return { (int)position.x, (int)position.y, width, height };
}
