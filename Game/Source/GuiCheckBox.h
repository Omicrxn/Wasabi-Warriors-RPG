#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiCheckBox();

    bool Update(Input* input, AudioManager* audio, float dt);
    bool Draw(Render* render, bool debugDraw);

    void SetBoxProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, ButtonColour colour = ButtonColour::YELLOW);

private:

    // Checkbox specific properties
    bool checked;

    // Rects for boxes
    SDL_Rect greyBox = { 290,0,45,49 };
    SDL_Rect greyPressedBox = { 290,139,45,45 };

    SDL_Rect yellowBox = { 293,294,45,49 };
    SDL_Rect yellowPressedBox = { 290,94,45,45 };

    SDL_Rect whiteBox = { 293,437,45,49 };
    SDL_Rect whitePressedBox = { 290,49,45,45 };

    // Rects for checks
    SDL_Rect greyTick = { 370,30,16,15 };
    SDL_Rect whiteTick = { 370,75,16,15 };
    SDL_Rect yellowTick = { 369,184,16,15 };

};

#endif // __GUICHECKBOX_H__