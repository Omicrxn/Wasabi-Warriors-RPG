#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;

    greyButton = { 0,188,190,49 };
    yellowButton = { 0,282,190,49 };
    yellowButtonPressed = { 0,237,190,45 };

    arrowLeft = { 303,486,22,21 };
    arrowRight = { 171,486,22,21 };

    isFocusing = false;

    hoverFx = -1;
    clickFx = -1;
}

GuiButton::~GuiButton()
{
}

void GuiButton::SetButtonAudioFx(int hoverFx, int clickFx)
{
    this->hoverFx = hoverFx;
    this->clickFx = clickFx;
}

bool GuiButton::Update(Input* input, AudioManager* audio, float dt)
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
            
            if (!isFocusing)
            {
                isFocusing = true;
                audio->PlayFx(hoverFx);
            }

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) // && gamepad.A
            {
                state = GuiControlState::PRESSED;
            }

            //if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) // && gamepad.A (?)
            //{
            //    state = GuiControlState::PRESSED;
            //}

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                NotifyObserver();
                // Audio Fx when clicked
                audio->PlayFx(clickFx);
            }
        }
        else
        {
            state = GuiControlState::NORMAL;
            isFocusing = false;
        }
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
    case GuiControlState::HIDDEN:
        break;
    case GuiControlState::NORMAL: 
        render->DrawTexture(texture, bounds.x, bounds.y, &yellowButton, 0.0f);

        render->DrawText(font, text.GetString(), bounds.x + bounds.w/5 - bounds.w/6, bounds.y + bounds.h/2 - bounds.h/4, 22, 8, { 89,73,34,255 });
        break;
    case GuiControlState::FOCUSED:
        render->DrawTexture(texture, bounds.x, bounds.y, &yellowButton, 0.0f);

        render->DrawText(font, text.GetString(), bounds.x + bounds.w / 5 - bounds.w / 6 + 2, bounds.y + bounds.h / 2 - bounds.h / 4 + 2, 22, 8, { 105,105,105,255 });
        render->DrawText(font, text.GetString(), bounds.x + bounds.w/5 - bounds.w/6, bounds.y + bounds.h/2 - bounds.h/4, 22, 8, { 0,0,0,255 });

        render->DrawTexture(texture, bounds.x - 30, bounds.y + 14, &arrowRight, 0.0f);
        render->DrawTexture(texture, bounds.x + bounds.w + 8, bounds.y + 14, &arrowLeft, 0.0f);
        break;
    case GuiControlState::PRESSED: 
        render->DrawTexture(texture, bounds.x, bounds.y + 4, &yellowButtonPressed, 0.0f);

        render->DrawText(font, text.GetString(), bounds.x + bounds.w / 5 - bounds.w / 6 + 2, bounds.y + bounds.h / 2 - bounds.h / 4 + 4 + 2, 22, 8, { 105,105,105,255 });
        render->DrawText(font, text.GetString(), bounds.x + bounds.w/5 - bounds.w/6, bounds.y + bounds.h/2 - bounds.h/4 + 4, 22, 8, { 0,0,0,255 });

        render->DrawTexture(texture, bounds.x - 30, bounds.y + 18, &arrowRight, 0.0f);
        render->DrawTexture(texture, bounds.x + bounds.w + 8, bounds.y + 18, &arrowLeft, 0.0f);
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