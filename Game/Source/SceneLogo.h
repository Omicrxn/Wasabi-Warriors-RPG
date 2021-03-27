#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"

#include "SDL/include/SDL.h"

class SceneLogo : public Scene
{
public:

    SceneLogo();
    virtual ~SceneLogo();

    bool Load(Textures* tex, GuiManager* guiman);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload();

private:

    SDL_Rect logo;
    SDL_Texture* logoTex;

    int state;
    float timeCounter;
    float logoAlpha;
};

#endif // __SCENELOGO_H__
