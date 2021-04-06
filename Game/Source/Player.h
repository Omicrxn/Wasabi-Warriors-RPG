#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Being.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

class SceneManager;



class Player: public Being
{
public:

    Player(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    void SetTexture(SDL_Texture *tex, int textureStartYPos);

    SDL_Rect GetBounds();

    // To set the player name
    void SetName(SString name);

private:

    void Walk(iPoint direction, float dt);

public:

    SDL_Texture* texture;   // Player spritesheet

    Animations currentAnim;

    int width, height;
};

#endif // __PLAYER_H__
