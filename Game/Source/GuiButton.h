#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

    GuiButton(uint32 id, SDL_Rect bounds, const char* text);
    virtual ~GuiButton();

    bool Update(Input* input, float dt);
    bool Draw(Render* render, bool debugDraw);

private:

    // Gui Button specific properties
    // Maybe some animation properties for state change?

    SDL_Rect whiteButton;
    SDL_Rect brownButton;
    SDL_Rect greyButton;
    SDL_Rect yellowButton;
    SDL_Rect yellowButtonPressed;
};

#endif // __GUIBUTTON_H__