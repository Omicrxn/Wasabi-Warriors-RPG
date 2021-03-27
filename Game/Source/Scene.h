#ifndef __SCENE_H__
#define __SCENE_H__

#include "SString.h"

class Input;
class Render;
class Textures;
class EntityManager;
class GuiManager;

class GuiControl;

enum class SceneType
{
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING,
    BATTLE
};

class Scene
{
public:

    Scene() : active(true), loaded(false), transitionRequired(false) {}

    virtual bool Load(Textures* tex, GuiManager* guiman)
    {
        return true;
    }
    virtual bool Load(Textures* tex, EntityManager* entityman, GuiManager* guiman)
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

    virtual bool Unload(Textures* tex)
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

public:

    bool active = true;
    SString name; // Scene name identifier

    // Possible properties
    bool loaded = false;
    // TODO: Transition animation properties
    SceneType type;
    bool transitionRequired;
    SceneType nextScene;
};

#endif // __SCENE_H__