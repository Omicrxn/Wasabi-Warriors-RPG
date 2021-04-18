#ifndef __SCENE_H__
#define __SCENE_H__

#include "SString.h"
#include "PugiXml/src/pugixml.hpp"

class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;
class GuiManager;
class Window;
class DialogSystem;
class Easing;
class App;

class GuiControl;

enum class SceneType
{
    LOGO,
    TITLE,
    GAMEPLAY_LOAD,
    GAMEPLAY,
    ENDING,
    BATTLE
};

class Scene
{
public:

    Scene() : active(true), loaded(false), transitionRequired(false) {}

    virtual bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing)
    {
        return true;
    }
    virtual bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing, Render* render)
    {
        return true;
    }
    virtual bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityman, DialogSystem* dialogSystem, Easing* easing, App* app)
    {
        return true;
    }

    virtual bool Update(Input* input, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render)
    {
        return true;
    }

    virtual bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
    {
        return true;
    }

    void TransitionToScene(SceneType scene)
    {
        transitionRequired = true;
        nextScene = scene;
    }

    // Define multiple Gui Event methods
    virtual bool OnGuiMouseClickEvent(GuiControl* control)
    {
        return true;
    }

    virtual bool LoadState(pugi::xml_node&)
    {
        return true;
    }

    virtual bool SaveState(pugi::xml_node&) const
    {
        return true;
    }

public:

    bool active = true;
    SString name; // Scene name identifier

    // Possible properties
    bool loaded = false;
    // TODO: Transition animation properties
    SceneType type;
    bool transitionRequired;
    SceneType nextScene = SceneType::LOGO;
};

#endif // __SCENE_H__