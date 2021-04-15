#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

enum Style 
{
    WHITE,
    GREY, 
    ICON_RETURN, 
    ICON_SETTINGS,
    ICON_EXIT,
    ICON_PAUSE,
    ICON_INVENTORY,
    ICON_PHONE
};

class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char* text);
    virtual ~GuiButton();

    bool Update(Input* input, AudioManager* audio, float dt);
    bool Draw(Render* render, bool debugDraw);

    void SetButtonProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, Style style = Style::WHITE);

private:

    Style buttonStyle;

    SDL_Rect greyButton;

    SDL_Rect yellowButton;
    SDL_Rect yellowButtonPressed;

    SDL_Rect arrowYellowLeft;
    SDL_Rect arrowYellowRight;

    SDL_Rect whiteButton;
    SDL_Rect whiteButtonPressed;

    SDL_Rect arrowWhiteLeft;
    SDL_Rect arrowWhiteRight;

    SDL_Rect iconReturn;
    SDL_Rect iconSettings;
    SDL_Rect iconExit;
    SDL_Rect iconPause;
    SDL_Rect iconInventory;
    SDL_Rect iconPhone;

};

#endif // __GUIBUTTON_H__