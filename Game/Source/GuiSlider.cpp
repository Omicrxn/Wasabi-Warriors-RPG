#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;

    isHovering = false;
    gamepadFocus = false;

    hoverFx = -1;
    clickFx = -1;

    whiteBar = { 102, 27, 261, 51 };
    grayBar = { 465, 27, 261, 51 };
    pinkBar = { 102, 87, 261, 51 };

    slider.x = bounds.x;
    slider.y = bounds.y;
    slider.w = 36;
    slider.h = 36;

    grayCircle = { 42, 402, 36, 36 };

    //arrowWhiteRight = { 534, 112, 25, 25 };
}

GuiSlider::~GuiSlider()
{
}

void GuiSlider::SetSliderProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx)
{
    SetObserver(module);
    SetTexture(texture);
    SetFont(font);

    this->hoverFx = hoverFx;
    this->clickFx = clickFx;
}

int GuiSlider::GetValue() const
{
    return this->value;
}

SDL_Rect GuiSlider::GetSlider() const
{
    return this->slider;
}

void GuiSlider::SetSliderPosX(int posX)
{
    this->slider.x = posX;
}

bool GuiSlider::Update(Input* input, AudioManager* audio, float dt)
{
    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check if gamepad is focusing the slider
        if (gamepadFocus)
        {
            state = GuiControlState::FOCUSED;

            if (!isHovering)
            {
                isHovering = true;
                audio->PlayFx(hoverFx);
            }

            // If gamepad button pressed -> Generate event!
            if (input->GetControllerButton(CONTROLLER_BUTTON_RB) == KeyState::KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
                slider.x += 10;
                audio->PlayFx(clickFx);
            }
            else if (input->GetControllerButton(CONTROLLER_BUTTON_LB) == KeyState::KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
                slider.x -= 10;
                audio->PlayFx(clickFx);
            }

            if (input->GetControllerButton(CONTROLLER_BUTTON_RB) == KeyState::KEY_UP || input->GetControllerButton(CONTROLLER_BUTTON_LB) == KeyState::KEY_UP)
                NotifyObserver();

            if (slider.x < bounds.x)
                slider.x = bounds.x;

            if (slider.x + slider.w >= bounds.x + bounds.w)
                slider.x = bounds.x + bounds.w - slider.w;

            float percentage = (100.0f / bounds.w) * (slider.x - bounds.x);
            percentage = percentage / 100.0f;
            value = maxValue * percentage;
        }
        // Check collision between mouse and slider bounds
        else if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w))
            && (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h))
            && !input->GetControllerState())
        {
            state = GuiControlState::FOCUSED;

            if (!isHovering)
            {
                isHovering = true;
                audio->PlayFx(hoverFx);
            }

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
                audio->PlayFx(clickFx);

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                int posX, posY;
                input->GetMousePosition(posX, posY);
                slider.x = posX - slider.w;
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
        else
        {
            state = GuiControlState::NORMAL;
            isHovering = false;
            gamepadFocus = false;
        }
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
        render->DrawText(font, text.GetString(), bounds.x, bounds.y - bounds.h / 2 - bounds.h / 4, 28, 3, { 105,105,105,255 });

        render->DrawTexture(texture, bounds.x, bounds.y, &whiteBar, 0.0f);
        break;
    case GuiControlState::FOCUSED:
        render->DrawText(font, text.GetString(), bounds.x, bounds.y - bounds.h / 2 - bounds.h / 4, 28, 3, { 0,0,0,255 });

        render->DrawTexture(texture, bounds.x, bounds.y, &grayBar, 0.0f);

        //render->DrawTexture(texture, bounds.x - 30, bounds.y + 7, &arrowWhiteRight, 0.0f);
        break;
    case GuiControlState::PRESSED:
        render->DrawText(font, text.GetString(), bounds.x, bounds.y - bounds.h / 2 - bounds.h / 4, 28, 3, { 0,0,0,255 });

        render->DrawTexture(texture, bounds.x, bounds.y, &pinkBar, 0.0f);

        //render->DrawTexture(texture, bounds.x - 30, bounds.y + 7, &arrowWhiteRight, 0.0f);
        break;
    default:
        break;
    }

    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
        render->DrawTexture(texture, slider.x, slider.y + 8, &grayCircle, 0.0f);

    if (debugDraw)
    {
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