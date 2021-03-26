#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Being.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

enum class PlayerAnim
{
    IDLE,
    WALK,
    JUMP,
    CLIMB
};

class Player: public Being
{
public:

    Player(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    void SetTexture(SDL_Texture *tex);

    SDL_Rect GetBounds();
private:
    void Walk(iPoint direction, float dt);
public:

    SDL_Texture* texture;   // Player spritesheet

    // TODO: Define all animation properties
    PlayerAnim currentAnim;

    int width, height;
};

#endif // __PLAYER_H__
