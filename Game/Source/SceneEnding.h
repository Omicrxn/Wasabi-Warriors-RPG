#ifndef __SCENEENDING_H__
#define __SCENEENDING_H__

#include "Scene.h"
#include "Animation.h"

#include "SDL/include/SDL.h"
#include "Point.h"
#include "Timer.h"

class Font;
class GuiIcon;
class SceneEnding : public Scene
{
public:

    SceneEnding(bool isVictory = false);
    virtual ~SceneEnding();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing, AssetsManager* assetsManager);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    bool isVictory = false;

    // Needed modules
    AssetsManager* assetsManager;
    GuiManager* guiManager;
    Window* win;
    Easing* easing;
    AudioManager* audio;
    Transitions* transitions;

    Font* textFont;
    Font* buttonFont;

    SDL_Texture* guiAtlasTex;
    SDL_Texture* backgroundTex;

    // Audio Fx for gui controls
    int hoverFx, clickFx;

    int positionX;

    GuiIcon* iconReturnTitle;

    bool controllerState = false;
};

#endif // __SCENEENDING_H__
