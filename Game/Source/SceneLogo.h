#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"
#include "Timer.h"

#include "SDL/include/SDL.h"
class AudioManager;

class SceneLogo : public Scene
{
public:

    SceneLogo();
    virtual ~SceneLogo();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiman, Easing* easing);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

private:
    AudioManager* audio;

    SDL_Texture* logoTex;
    SDL_Rect logo;

    Timer logoFxTimer;

    int logoFx;
    int state;


    float timeCounter;
    float logoAlpha;
};

#endif // __SCENELOGO_H__