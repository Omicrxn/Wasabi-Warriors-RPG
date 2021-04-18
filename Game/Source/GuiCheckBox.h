#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text);
    virtual ~GuiCheckBox();

    bool Update(Input* input, AudioManager* audio, float dt);
    bool Draw(Render* render, bool debugDraw);

    void SetCheckBoxProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx);

    bool GetCheck() const;

private:

    // Checkbox specific properties
    bool checked;

    // Rects for box
    SDL_Rect whiteBox;
    SDL_Rect whitePressedBox;

    // Rects for check
    SDL_Rect greyTick;
    SDL_Rect whiteTick;
    SDL_Rect yellowTick;

    SDL_Rect arrowWhiteRight;
};

#endif // __GUICHECKBOX_H__