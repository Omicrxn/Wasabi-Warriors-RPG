#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;

    slider.x = bounds.x;
    slider.y = bounds.y;
    slider.w = 10;
    slider.h = bounds.h;

    arrowWhiteLeft = { 281,486,22,21 };
    arrowWhiteRight = { 259,486,22,21 };

    whiteCircle = { 335, 0, 35, 38 };
    bar = { 794, 486, 300, 5 };
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, AudioManager* audio, float dt)
{
    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
         
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                int posX, posY;
                input->GetMousePosition(posX, posY);
                slider.x = posX;
            }

            if (slider.x < bounds.x)
                slider.x = bounds.x;

            if (slider.x + slider.w >= bounds.x + bounds.w)
                slider.x = bounds.x + bounds.w - slider.w;

            float percentage = (100.0f / bounds.w) * (slider.x - bounds.x);
            percentage = percentage / 100.0f;
            value = maxValue * percentage;

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                audio->PlayFx(clickFx);
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return true;
}

bool GuiSlider::Draw(Render* render, bool debugDraw)
{
    switch (state)
    {
    case GuiControlState::DISABLED:
        break;
    case GuiControlState::HIDDEN:
        break;
    case GuiControlState::NORMAL:
        render->DrawText(font, text.GetString(), bounds.x + bounds.w, bounds.y + bounds.h / 2 - bounds.h / 4, 22, 3, { 255,255,255,255 });
        render->DrawTexture(this->texture, this->bounds.x, this->bounds.y + 15, &bar, 0.0f);
        render->DrawTexture(this->texture, this->slider.x, this->slider.y, &whiteCircle, 0.0f);
        break;
    case GuiControlState::FOCUSED:
        render->DrawText(font, text.GetString(), bounds.x + bounds.w, bounds.y + bounds.h / 2 - bounds.h / 4, 22, 3, { 0,0,0,255 });
        render->DrawTexture(texture, bounds.x - 30, bounds.y + 18, &arrowWhiteRight, 0.0f);
        render->DrawTexture(this->texture, this->bounds.x, this->bounds.y + 15, &bar, 0.0f);
        render->DrawTexture(this->texture, this->slider.x, this->slider.y, &whiteCircle, 0.0f);
        break;
    case GuiControlState::PRESSED:
        render->DrawText(font, text.GetString(), bounds.x + bounds.w, bounds.y + bounds.h / 2 - bounds.h / 4, 22, 3, { 0,0,0,255 });
        render->DrawTexture(texture, bounds.x - 30, bounds.y + 18, &arrowWhiteRight, 0.0f);
        render->DrawTexture(this->texture, this->bounds.x, this->bounds.y + 15, &bar, 0.0f);
        render->DrawTexture(this->texture, this->slider.x, this->slider.y, &whiteCircle, 0.0f);
        break;
    default:
        break;
    }

    if (debugDraw)
    {
        // Draw the right button depending on state
        switch (state)
        {
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 100, 100, 100, 255 });
            break;
        case GuiControlState::NORMAL: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
            break;
        case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 255, 255, 0, 255 });
            break;
        case GuiControlState::PRESSED: render->DrawRectangle(bounds, { 0, 255, 255, 255 });
            break;
        default:
            break;
        }
    }
   

    return true;
}

void GuiSlider::SetSliderProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx)
{
    SetObserver(module);
    SetTexture(texture);
    SetFont(font);

    this->hoverFx = hoverFx;
    this->clickFx = clickFx;
}