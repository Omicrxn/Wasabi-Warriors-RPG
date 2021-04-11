#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char* text);
    virtual ~GuiButton();

    bool Update(Input* input, AudioManager* audio, float dt);
    bool Draw(Render* render, bool debugDraw);

    void SetButtonAudioFx(int hoverFx, int clickFx);


private:

    // Gui Button specific properties
    // Maybe some animation properties for state change?

    SDL_Rect greyButton;
    SDL_Rect yellowButton;
    SDL_Rect yellowButtonPressed;

    SDL_Rect arrowLeft;
    SDL_Rect arrowRight;

    bool isHovering;

    int hoverFx, clickFx;
};

#endif // __GUIBUTTON_H__