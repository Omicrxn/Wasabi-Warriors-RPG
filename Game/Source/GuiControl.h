#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Audio.h"

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
    HIDDEN,
    NORMAL,
    FOCUSED,
    PRESSED,
};

class GuiControl
{
public:

    GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL) {}

    GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) :
        type(type),
        state(GuiControlState::NORMAL),
        bounds(bounds),
        text(text)
    {
        texture = NULL;
    }

    virtual bool Update(Input* input, AudioManager* audio, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render, bool debugDraw)
    {
        return true;
    }

    void SetTexture(SDL_Texture* tex)
    {
        texture = tex;
    }

    void SetFont(Font* font)
    {
        this->font = font;
    }

    void SetObserver(Scene* module)
    {
        observer = module;
    }

    void NotifyObserver()
    {
        observer->OnGuiMouseClickEvent(this);
    }

protected:

    int hoverFx, clickFx;
    bool isHovering;

public:

    bool gamepadFocus;

    uint32 id;
    GuiControlType type;
    GuiControlState state;

    SString text;
    SDL_Rect bounds;        // Position and size

    SDL_Texture* texture;   // Texture atlas reference

    Font* font;

    Scene* observer;        // Observer module (it should probably be an array/list)
};

#endif // __GUICONTROL_H__