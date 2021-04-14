#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

enum ButtonColour
{
    NONE,
    WHITE,
    YELLOW,
    GREY
};

class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char* text);
    virtual ~GuiButton();

    bool Update(Input* input, AudioManager* audio, float dt);
    bool Draw(Render* render, bool debugDraw);

    void SetButtonProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, ButtonColour colour = ButtonColour::YELLOW);
    void SetButtonAudioFx(int hoverFx, int clickFx);

private:

    // Gui Button specific properties
    ButtonColour colour;

    bool isHovering;

    int hoverFx, clickFx;

    SDL_Rect greyButton;

    SDL_Rect yellowButton;
    SDL_Rect yellowButtonPressed;

    SDL_Rect arrowYellowLeft;
    SDL_Rect arrowYellowRight;

    SDL_Rect whiteButton;
    SDL_Rect whiteButtonPressed;

    SDL_Rect arrowWhiteLeft;
    SDL_Rect arrowWhiteRight;
};

#endif // __GUIBUTTON_H__