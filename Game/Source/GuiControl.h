#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Scene.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

enum class GuiControlType
{
    BUTTON,
    TOGGLE,
    CHECKBOX,
    SLIDER,
};

enum class GuiControlState
{
    DISABLED,
    NORMAL,
    FOCUSED,
    PRESSED,
};

class GuiControl
{
public:

    GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL) {}

    GuiControl(GuiControlType type, SDL_Rect bounds) :
        type(type),
        state(GuiControlState::NORMAL),
        bounds(bounds)
    {
        texture = NULL;
    }

    virtual bool Update(Input* input, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render) const
    {
        return true;
    }

    void SetTexture(SDL_Texture* tex)
    {
        texture = tex;
    }

    void SetObserver(Scene* module)
    {
        observer = module;
    }

    void NotifyObserver()
    {
        observer->OnGuiMouseClickEvent(this);
    }

public:

    uint32 id;
    GuiControlType type;
    GuiControlState state;

    SDL_Rect bounds;        // Position and size

    SDL_Texture* texture;   // Texture atlas reference

    Scene* observer;        // Observer module (it should probably be an array/list)
};

#endif // __GUICONTROL_H__