#pragma once
#ifndef __SCENEBATTLE_H__
#define __SCENEBATTLE_H__

#include "Scene.h"

struct SDL_Texture;

class SceneBattle : public Scene
{
public:

    SceneBattle();
    virtual ~SceneBattle();

    bool Load(Textures* tex);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload();

private:

    SDL_Texture* texture;
};

#endif // __SCENEBATTLE_H__