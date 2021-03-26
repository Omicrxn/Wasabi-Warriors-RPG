#include "Player.h"

Player::Player(Textures* tex) : Being()
{
    texture = NULL;
    position = iPoint(12 * 16, 27 * 16);
    currentAnim = PlayerAnim::IDLE;
    velocity = { 200.0f, 200.0f };
    width = 32;
    height = 32;
    direction = { 0,0 };
    // Define Player animations
}

bool Player::Update(Input* input, float dt)
{
    Walk(direction, dt);
    if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) direction.y = -1; 
    if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) direction.y = 1;
    if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) direction.x = -1;
    if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) direction.x = 1;
    if (input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
        && input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
    {
        direction = { 0,0 };
    }



    return true;
}

bool Player::Draw(Render* render)
{
    SDL_Rect animRec;
    switch (currentAnim)
    {
    case PlayerAnim::IDLE:
        animRec = walkingAnim.GetCurrentFrame();
        break;
    case PlayerAnim::WALK:
        animRec = walkingAnim.GetCurrentFrame();
        break;
    default:
        break;
    }
    render->scale = 3;
    render->DrawTexture(texture, position.x, position.y, &animRec);
    render->CameraFollow(position.x,position.y);
    render->scale = 1;
    return false;
}

void Player::SetTexture(SDL_Texture *tex, int textureStartYPos)
{
    texture = tex;
    for (int y = textureStartYPos; y < 160; y+=32)
    {
        for (int x = 0; x < 8*32; x+=32)
        {
            walkingAnim.PushBack({ x,y,32,32 });
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
