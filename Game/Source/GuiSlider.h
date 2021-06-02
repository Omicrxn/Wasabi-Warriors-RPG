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

    void SetSliderProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx);

    int GetValue() const;
    SDL_Rect GetSlider() const;
    void SetSliderPosX(int posX);

    int minValue;
    int maxValue;

private:

    // GuiSlider specific properties
    int value;

    SDL_Rect whiteBar;
    SDL_Rect grayBar;
    SDL_Rect pinkBar;

    SDL_Rect slider;

    SDL_Rect arrowWhiteRight;

    SDL_Rect grayCircle;
    SDL_Rect blueCircle;
    SDL_Rect yellowCircle;
};
#endif // __GUISLIDER_H__