#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"

#include "SDL/include/SDL.h"

class SceneLogo : public Scene
{
public:

    SceneLogo();
    virtual ~SceneLogo();

    bool Load(Textures* tex, Window* win, GuiManager* guiman);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, GuiManager* guiManager);

private:

    SDL_Texture* logoTex;
    SDL_Rect logo;

    int state;
    float timeCounter;
    float logoAlpha;
};

#endif // __SCENELOGO_H__