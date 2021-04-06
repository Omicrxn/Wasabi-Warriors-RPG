#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Being.h"
#include "Render.h"
#include "Textures.h"

#include "Point.h"
#include "SString.h"

class Enemy : public Being
{
public:

    Enemy();
    virtual ~Enemy();

public:

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    void SetTexture(SDL_Texture* tex, int spritePos);

    void Walk(iPoint direction, float dt);

    void SetName(SString name);


public:
    SDL_Texture* texture;   // Player spritesheet

    Animations currentAnim;

    int width, height;

};

#endif // __ENEMY_H__
