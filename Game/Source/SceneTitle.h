#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "Animation.h"

#include "SDL/include/SDL.h"
#include "Point.h"
#include "Timer.h"

class GuiButton;
class GuiSlider;
class GuiCheckBox;
class GuiIcon;

class Font;
class Window;
class Screen;

enum class MenuSelection
{
    NONE,
    CONTINUE,
    START,
    SETTINGS,
    CREDITS,
    EXIT
};

class SceneTitle : public Scene
{
public:

    SceneTitle();
    virtual ~SceneTitle();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing, Render* render, Transitions* transitions, AssetsManager* assetsManager);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    // Load / Save
    bool LoadState(pugi::xml_node&);
    bool SaveState(pugi::xml_node&) const;

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    // Needed modules
    AssetsManager* assetsManager;
    GuiManager* guiManager;
    Window* win;
    Render* render;
    Easing* easing;
    AudioManager* audio;
    Transitions* transitions;

    Screen* screenSettings;
    Screen* screenMainMenu;
    Screen* screenCredits;

    SDL_Texture* backgroundTex;
    SDL_Rect backgroundRect;

    SDL_Texture* guiAtlasTex;
    SDL_Texture* guiAtlasTex2;
    SDL_Texture* titlesTex;

    SDL_Rect settingsTitleRect;
    SDL_Rect settingsBackgroundRect;
    SDL_Rect creditsTitleRect;

    Font* titleFont;
    Font* buttonFont;

    // Audio Fx for gui controls
    int hoverFx, clickFx, returnFx;

    Timer titleFxTimer;
    int titleFx;

    MenuSelection menuCurrentSelection;

    // Gamepad's menu focused button ID
    uint focusedButtonId;
    uint prevFocusedButtonId;
};

#endif // __SCENETITLE_H__