#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"
#include "Animation.h"

enum class Style 
{
    WHITE,
    GREY,
    SUSHI_ORANGE,
    SUSHI_RED, // We may use this red one for short strings, because it is centered
    SUSHI_YELLOW
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
    SDL_Rect whiteButton;
    SDL_Rect whiteButtonPressed;

    SDL_Rect arrowWhiteLeft;
    SDL_Rect arrowWhiteRight;

    Animation buttonAnim;
    SDL_Rect backgroundButtonAnim;
};

#endif // __GUIBUTTON_H__