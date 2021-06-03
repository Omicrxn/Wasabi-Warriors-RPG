#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;

    checked = false;

    isHovering = false;
    gamepadFocus = false;

    hoverFx = -1;
    clickFx = -1;

    whiteBox = { 36, 30, 49, 48 };
    grayBox = { 399, 30, 49, 48 };
    pinkBox = { 36, 87, 49, 48 };

    greyTick = { 42, 402, 36, 36 };

    //arrowWhiteRight = { 534, 112, 25, 25 };
}

GuiCheckBox::~GuiCheckBox()
{
}

void GuiCheckBox::SetCheckBoxProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx)
{
    SetObserver(module);
    SetTexture(texture);
    SetFont(font);

    this->hoverFx = hoverFx;
    this->clickFx = clickFx;
}

bool GuiCheckBox::GetCheck() const
{
    return checked;
}

void GuiCheckBox::SetCheck(bool boolean)
{
    this->checked = boolean;
}

bool GuiCheckBox::Update(Input* input, AudioManager* audio, float dt)
{
    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check if gamepad is focusing the checkbox
        if (gamepadFocus)
        {
            state = GuiControlState::FOCUSED;

            if (!isHovering)
            {
                isHovering = true;
                audio->PlayFx(hoverFx);
            }

            if (input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_REPEAT || input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN
                || input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_REPEAT || input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
            }

            // If gamepad button pressed -> Generate event!
            if (input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_UP || input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_UP)
            {
                checked = !checked;
                NotifyObserver();
                // Audio Fx when clicked
                audio->PlayFx(clickFx);
            }
        }
        // Check collision between mouse and checkbox bounds
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

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                NotifyObserver();
                // Audio Fx when clicked
                audio->PlayFx(clickFx);
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

bool GuiCheckBox::Draw(Render* render, bool debugDraw)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
        break;
    case GuiControlState::HIDDEN:
        break;
    case GuiControlState::NORMAL:
        render->DrawTexture(texture, bounds.x, bounds.y, &whiteBox, 0.0f);

        render->DrawText(font, text.GetString(), bounds.x, bounds.y - 30, 28, 3, { 105,105,105,255 });
        break;
    case GuiControlState::FOCUSED:
        render->DrawTexture(texture, bounds.x, bounds.y, &grayBox, 0.0f);

        //render->DrawTexture(texture, bounds.x - 30, bounds.y + bounds.h / 2 - arrowWhiteRight.h / 2, &arrowWhiteRight, 0.0f);

        //render->DrawText(font, text.GetString(), bounds.x + 2, bounds.y - 30, 28, 3, { 105,105,105,255 });
        render->DrawText(font, text.GetString(), bounds.x, bounds.y - 30, 28, 3, { 0,0,0,255 });
        break;
    case GuiControlState::PRESSED:
        render->DrawTexture(texture, bounds.x, bounds.y + 4, &pinkBox, 0.0f);

        //render->DrawTexture(texture, bounds.x - 30, bounds.y + bounds.h / 2 - arrowWhiteRight.h / 2 + 4, &arrowWhiteRight, 0.0f);

        //render->DrawText(font, text.GetString(), bounds.x + 2, bounds.y - 30 + 4, 28, 3, { 105,105,105,255 });
        render->DrawText(font, text.GetString(), bounds.x, bounds.y - 30 + 4, 28, 3, { 0,0,0,255 });
        break;
    default:
        break;
    }

    if (checked)
    {
        if (state == GuiControlState::PRESSED)
            render->DrawTexture(texture, bounds.x + bounds.w / 2 - greyTick.w / 2 + 2, bounds.y + bounds.h / 2 - greyTick.h / 2 + 4, &greyTick, 0.0f);
        else if (state == GuiControlState::NORMAL || state == GuiControlState::FOCUSED)
            render->DrawTexture(texture, bounds.x + bounds.w / 2 - greyTick.w / 2 + 2, bounds.y + bounds.h / 2 - greyTick.h / 2, &greyTick, 0.0f);
    }

    if (debugDraw)
    {
        switch (state)
        {
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 100, 100, 100, 255 }, true, false);
            break;
        case GuiControlState::NORMAL: render->DrawRectangle(bounds, { 0, 255, 0, 255 }, true, false);
            break;
        case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 255, 255, 0, 255 }, true, false);
            break;
        case GuiControlState::PRESSED: render->DrawRectangle(bounds, { 0, 255, 255, 255 }, true, false);
            break;
        default:
            break;
        }
    }

    return true;
}