#pragma once
#ifndef __GUIICON_H__
#define __GUIICON_H__

#include "GuiControl.h"

enum class IconType
{
    NONE,
    ICON_RETURN,
    ICON_SETTINGS,
    ICON_EXIT,
    ICON_TEAM,
    ICON_QUEST,
    ICON_MAP
};

class GuiIcon : public GuiControl
{
public:

    GuiIcon(uint32 id, SDL_Rect bounds);
    virtual ~GuiIcon();

    bool Update(Input* input, AudioManager* audio, float dt);
    bool Draw(Render* render, bool debugDraw);

    void SetIconProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, IconType type = IconType::NONE);

private:

    IconType iconType;

    SDL_Rect iconSettings;
    SDL_Rect iconExit;
    SDL_Rect iconTeam;
    SDL_Rect iconQuest;
    SDL_Rect iconMap;

    SDL_Rect normalBox;
    SDL_Rect focusedBox;
    SDL_Rect pressedBox;

    SDL_Rect normalCircle;
    SDL_Rect focusedCircle;
    SDL_Rect pressedCircle;

    SDL_Rect normalReturn;
    SDL_Rect focusedReturn;
    SDL_Rect pressedReturn;
};

#endif // __GUIICON_H__