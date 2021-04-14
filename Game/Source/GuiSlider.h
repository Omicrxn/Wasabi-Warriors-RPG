#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(uint32 id, SDL_Rect bounds, const char *text);
    virtual ~GuiSlider();

    bool Update(Input* input, AudioManager* audio, float dt);
    bool Draw(Render* render, bool debugDraw);

    void SetSliderProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, ButtonColour colour = ButtonColour::YELLOW);

private:

    // GuiSlider specific properties
    // Maybe some animation properties for state change?
    
    int value;

    int minValue;
    int maxValue;

    SDL_Rect bar;
    SDL_Rect slider;

    SDL_Rect arrowWhiteLeft;
    SDL_Rect arrowWhiteRight;

    SDL_Rect whiteCircle;
    SDL_Rect blueCircle;
    SDL_Rect yellowCircle;
};

#endif // __GUISLIDER_H__