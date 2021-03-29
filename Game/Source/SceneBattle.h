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

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityman);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

private:

    SDL_Texture* texture;
};

#endif // __SCENEBATTLE_H__