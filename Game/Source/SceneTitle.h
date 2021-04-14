#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "Animation.h"

class GuiButton;
class GuiSlider;
class GuiCheckBox;

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

    void EnableTitleButtons();
    void EnableSettingsButtons();

    void HideTitleButtons();
    void HideSettingsButtons();

private:

    GuiManager* guiManager;
    Window* win;

    SDL_Texture* backgroundTex;
    SDL_Rect backgroundRect;

    SDL_Texture* guiAtlasTex;
    SDL_Texture* titlesTex;

    SDL_Rect mainTitlesRect;
    SDL_Rect settingsTitleRect;
    SDL_Rect settingsBackgroundRect;

    Font* titleFont;
    Font* buttonFont;

    // Audio Fx for gui controls
    int hoverFx, clickFx;

    GuiButton* btnStart;
    GuiButton* btnContinue;
    GuiButton* btnOptions;
    GuiButton* btnCredits;
    GuiButton* btnExit;
    GuiButton* btnReturnTitle;

    GuiCheckBox* checkFullScreen;
    GuiCheckBox* checkVsync;

    GuiSlider* sliderMusicVolume;
    GuiSlider* sliderFXVolume;

    MenuSelection menuCurrentSelection;
    //SettingsSelection settingsCurrentSelection = SettingsSelection::NONE;

    bool settingsScene;

    // Gamepad's menu focused button ID
    uint focusedButtonId;
};

#endif // __SCENETITLE_H__