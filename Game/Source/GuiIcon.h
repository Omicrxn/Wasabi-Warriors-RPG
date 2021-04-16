#pragma once
#ifndef __GUIICON_H__
#define __GUIICON_H__

#include "GuiControl.h"

enum IconType
{
    NONE,
    ICON_PAUSE,
    ICON_INVENTORY,
    ICON_PHONE,
    ICON_RETURN,
    ICON_SETTINGS,
    ICON_EXIT
};

class GuiIcon : public GuiControl
{
public:

    GuiIcon(uint32 id, SDL_Rect bounds);
    virtual ~GuiIcon();

    bool Update(Input* input, AudioManager* audio, float dt);
    bool Draw(Render* render, bool debugDraw);

    void SetIconProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, ControllerButton control, IconType type = IconType::NONE);

private:

    IconType iconType;

    ControllerButton assignedControl;

    SDL_Rect arrowWhiteRight;

    SDL_Rect iconReturn;
    SDL_Rect iconSettings;
    SDL_Rect iconExit;
    SDL_Rect iconPause;
    SDL_Rect iconInventory;
    SDL_Rect iconPhone;
};

#endif // __GUIICON_H__