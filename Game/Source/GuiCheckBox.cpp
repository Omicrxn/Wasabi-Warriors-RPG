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

    whiteBox = { 293,437,45,49 };
    whitePressedBox = { 290,49,45,45 };

    greyTick = { 370,30,16,15 };
    whiteTick = { 370,75,16,15 };
    yellowTick = { 369,184,16,15 };

    arrowWhiteRight = { 259,486,22,21 };
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

bool GuiCheckBox::Update(Input* input, AudioManager* audio, float dt)
{
    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        if (!input->GetControllerState())
            gamepadFocus = false;

        // Check if gamepad is focusing the button
        if (gamepadFocus && input->GetControllerState())
        {
            state = GuiControlState::FOCUSED;

            if (!isHovering)
            {
                isHovering = true;
                audio->PlayFx(hoverFx);
            }

            if (input->GetControllerButton(CONTROLLER_BUTTON_A) == KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If gamepad button pressed -> Generate event!
            if (input->GetControllerButton(CONTROLLER_BUTTON_A) == KEY_DOWN)
            {
                checked = !checked;
                NotifyObserver();
                // Audio Fx when clicked
                audio->PlayFx(clickFx);
            }
        }
        // Check collision between mouse and button bounds
        else if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (!isHovering)
            {
                isHovering = true;
                audio->PlayFx(hoverFx);
            }

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
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
        if (checked)
        {
            render->DrawTexture(texture, bounds.x + bounds.w / 2 - greyTick.w / 2, bounds.y + bounds.h / 2 - greyTick.h / 2, &greyTick, 0.0f);
        }

        render->DrawText(font, text.GetString(), bounds.x + bounds.h + 20, bounds.y + 14, 25, 3, { 255,255,255,255 });
        break;
    case GuiControlState::FOCUSED:
        render->DrawTexture(texture, bounds.x, bounds.y, &whiteBox, 0.0f);
        if (checked)
        {
            render->DrawTexture(texture, bounds.x + bounds.w / 2 - greyTick.w / 2, bounds.y + bounds.h / 2 - greyTick.h / 2, &greyTick, 0.0f);
        }

        render->DrawTexture(texture, bounds.x - 30, bounds.y + bounds.h / 2 - arrowWhiteRight.h / 2, &arrowWhiteRight, 0.0f);

        render->DrawText(font, text.GetString(), bounds.x + bounds.h + 22, bounds.y + 14, 25, 3, { 105,105,105,255 });
        render->DrawText(font, text.GetString(), bounds.x + bounds.h + 20, bounds.y + 14, 25, 3, { 0,0,0,255 });
        break;
    case GuiControlState::PRESSED:
        render->DrawTexture(texture, bounds.x, bounds.y + 4, &whitePressedBox, 0.0f);
        if (checked)
        {
            render->DrawTexture(texture, bounds.x + bounds.w / 2 - greyTick.w / 2, bounds.y + bounds.h / 2 - greyTick.h / 2 + 4, &greyTick, 0.0f);
        }

        render->DrawTexture(texture, bounds.x - 30, bounds.y + bounds.h / 2 - arrowWhiteRight.h / 2 + 4, &arrowWhiteRight, 0.0f);

        render->DrawText(font, text.GetString(), bounds.x + bounds.h + 22, bounds.y + 18, 25, 3, { 105,105,105,255 });
        render->DrawText(font, text.GetString(), bounds.x + bounds.h + 20, bounds.y + 18, 25, 3, { 0,0,0,255 });
        break;
    default:
        break;
    }

    return true;
}