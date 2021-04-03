#pragma once
#ifndef __SCENEBATTLE_H__
#define __SCENEBATTLE_H__

#include "Scene.h"

struct SDL_Texture;

class GuiButton;
class Font;
class Window;

#include "SDL/include/SDL.h"

enum class ActionSelection
{
    ATTACK,
    DEFEND,
    ITEM,
    RUN,
    NONE
};

class SceneBattle : public Scene
{
public:

    SceneBattle();
    virtual ~SceneBattle();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityman);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    SDL_Texture* texture;

    GuiManager* guiManager;

    GuiButton* btnAttack;
    GuiButton* btnDefend;
    GuiButton* btnItem;
    GuiButton* btnRun;
    GuiButton* btnNone;

    SDL_Texture* backgroundTex;
    SDL_Rect backgroundRect;

    SDL_Texture* guiAtlasTex;

    //Animation backgroundAnim;

    Font* titleFont;
    Font* buttonFont;

    Window* win;

    ActionSelection menuCurrentSelection = ActionSelection::NONE;
    //SettingsSelection settingsCurrentSelection = SettingsSelection::NONE;

    // Audio Fx for buttons
    int hoverFx, clickFx;

    // Gamepad's menu focused button
    uint controllerFocus;
};

#endif // __SCENEBATTLE_H__