#include "Player.h"

Player::Player(Textures* tex) : Being()
{
    texture = NULL;
    position = iPoint(12 * 16, 27 * 16);
    jumpSpeed = 200.0f;
    velocity = fPoint(1, 1);
    width = 32;
    height = 32;
    direction = iPoint(0, 0);
    texture = tex->Load("Assets/Textures/Characters/Main/player.png");
    // Define Player animations
}

bool Player::Update(Input* input, float dt)
{


    if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) direction.y = -1;
    if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) direction.y = 1;
    if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) direction.x = -1;
    if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) direction.x = 1;
    if (input->GetKey(SDL_SCANCODE_W) == KEY_UP || input->GetKey(SDL_SCANCODE_S) == KEY_UP
        || input->GetKey(SDL_SCANCODE_A) == KEY_UP || input->GetKey(SDL_SCANCODE_D) == KEY_UP)
    {
        direction = iPoint(0, 0);
    }

    Walk(direction, dt);

    return true;
}

bool Player::Draw(Render* render)
{
    SDL_Rect rect = { 0,0,width,height };
    render->DrawTexture(texture, position.x, position.y, &rect);
    render->CameraFollow(position);
    return false;
}

void Player::SetTexture(SDL_Texture *tex)
{
    texture = tex;
}
void Player::Walk(iPoint direction, float dt)
{
    position.x += direction.x * velocity.x;
    position.y += direction.y * velocity.y;
}
SDL_Rect Player::GetBounds()
{
    return { position.x, position.y, width, height };
}
