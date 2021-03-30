#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "Animation.h"

class GuiButton;
class Font;
class Window;

#include "SDL/include/SDL.h"

enum class MenuSelection
{
    NONE,
    START,
    CONTINUE,
    SETTINGS,
    CREDITS,
    EXIT
};

//enum class SettingsSelection
//{
//    NONE,
//    MUSICVOLUME,
//    FXVOLUME,
//    FULLSCREEN,
//    VSYNC
//};

class SceneTitle : public Scene
{
public:

    SceneTitle();
    virtual ~SceneTitle();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    GuiManager* guiManager;

    GuiButton* btnStart;
    GuiButton* btnContinue;
    GuiButton* btnOptions;
    GuiButton* btnCredits;
    GuiButton* btnExit;

    SDL_Texture* backgroundTex;
    SDL_Rect backgroundRect;

    SDL_Texture* guiAtlasTex;

    //Animation backgroundAnim;

    Font* titleFont;
    Font* buttonFont;

    Window* win;

    MenuSelection menuCurrentSelection = MenuSelection::NONE;
    //SettingsSelection settingsCurrentSelection = SettingsSelection::NONE;

    // Audio Fx for buttons
    int hoverFx, clickFx;

    // Gamepad's menu focused button
    uint controllerFocus;
};

#endif // __SCENETITLE_H__