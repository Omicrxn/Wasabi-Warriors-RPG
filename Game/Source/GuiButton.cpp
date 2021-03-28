#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;

    whiteButton = { 0,0,190,49 };
    brownButton = { 0,49,190,49 };
    greyButton = { 0,188,190,49 };
    yellowButton = { 0,282,190,49 };
    yellowButtonPressed = { 0, 237, 190, 45 };
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
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
            }

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
                // Audio Fx when clicked
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return true;
}

bool GuiButton::Draw(Render* render, bool debugDraw)
{
    // Draw the right button depending on state
   
    switch (state)
    {
    case GuiControlState::DISABLED: 
        render->DrawTexture(texture, bounds.x, bounds.y, &greyButton, 0.0f);
        break;
    case GuiControlState::NORMAL: 
        render->DrawTexture(texture, bounds.x, bounds.y, &yellowButton, 0.0f);
        render->DrawText(font, text.GetString(), bounds.x + bounds.w/5 - bounds.w/6, bounds.y + bounds.h/2 - bounds.h/4, 22, 8, { 89,73,34,255 });
        break;
    case GuiControlState::FOCUSED:
        render->DrawTexture(texture, bounds.x, bounds.y, &yellowButton, 0.0f);
        render->DrawText(font, text.GetString(), bounds.x + bounds.w/5 - bounds.w/6, bounds.y + bounds.h/2 - bounds.h/4, 22, 8, { 0,0,0,255 });
        break;
    case GuiControlState::PRESSED: 
        render->DrawTexture(texture, bounds.x, bounds.y + 4, &yellowButtonPressed, 0.0f);
        render->DrawText(font, text.GetString(), bounds.x + bounds.w/5 - bounds.w/6, bounds.y + bounds.h/2 - bounds.h/4 + 4, 22, 8, { 0,0,0,255 });
        break;
    default:
        break;
    }
     
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