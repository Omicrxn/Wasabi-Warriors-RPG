#include "GuiIcon.h"

GuiIcon::GuiIcon(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::ICON, id)
{
    this->bounds = bounds;

    iconType = IconType::NONE;

    assignedControl = ControllerButton::CONTROLLER_BUTTON_A;

    isHovering = false;
    gamepadFocus = false;

    hoverFx = -1;
    clickFx = -1;

    arrowWhiteRight = { 259,486,22,21 };

    iconReturn = { 1117, 29, 65, 55 };
    iconExit = { 1000, 125, 46, 55 };
    iconSettings = { 870, 125, 58, 55 };
    iconPause = { 870, 220, 52, 55 };
    iconInventory = { 1000, 220, 56, 55 };
    iconPhone = { 1117, 220, 41, 55 };
}

GuiIcon::~GuiIcon()
{
}

void GuiIcon::SetIconProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, ControllerButton control, IconType type)
{
    SetObserver(module);
    SetTexture(texture);
    SetFont(font);

    this->hoverFx = hoverFx;
    this->clickFx = clickFx;

    assignedControl = control;
    iconType = type;
}

bool GuiIcon::Update(Input* input, AudioManager* audio, float dt)
{
    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check if gamepad is focusing the button
        if (input->GetControllerState())
        {
            if (input->GetControllerButton(assignedControl) == KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If gamepad button pressed -> Generate event!
            if (input->GetControllerButton(assignedControl) == KEY_DOWN)
            {
                NotifyObserver();
                // Audio Fx when pressed
                audio->PlayFx(clickFx);
            }
        }
        // Check collision between mouse and button bounds
        else if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)) && !input->GetControllerState())
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
                NotifyObserver();
                // Audio Fx when clicked
                audio->PlayFx(clickFx);
            }
        }
        else
        {
            state = GuiControlState::NORMAL;
            isHovering = false;
        }
    }

    return true;
}

bool GuiIcon::Draw(Render* render, bool debugDraw)
{
    // Draw the right icon depending on state
    switch (state)
    {
    case GuiControlState::HIDDEN:
        break;

    case GuiControlState::NORMAL:
        switch (iconType)
        {
        case RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconReturn, 0.0f);
            break;
        case EXIT:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconExit, 0.0f);
            break;
        case I_SETTINGS:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconSettings, 0.0f);
            break;
        case I_PAUSE:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconPause, 0.0f);
            break;
        case I_INVENTORY:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconInventory, 0.0f);
            break;
        case PHONE:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconPhone, 0.0f);
            break;
        default:
            break;
        }
        break;

    case GuiControlState::FOCUSED:
        render->DrawTexture(texture, bounds.x - 30, bounds.y + 14, &arrowWhiteRight, 0.0f);
        switch (iconType)
        {
        case RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconReturn, 0.0f);
            break;
        case EXIT:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconExit, 0.0f);
            break;
        case I_SETTINGS:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconSettings, 0.0f);
            break;
        case I_PAUSE:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconPause, 0.0f);
            break;
        case I_INVENTORY:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconInventory, 0.0f);
            break;
        case PHONE:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconPhone, 0.0f);
            break;
        default:
            break;
        }
        break;

    case GuiControlState::PRESSED:
        render->DrawTexture(texture, bounds.x - 30, bounds.y + 18, &arrowWhiteRight, 0.0f);
        switch (iconType)
        {
        case RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconReturn, 0.0f);
            break;
        case EXIT:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconExit, 0.0f);
            break;
        case I_SETTINGS:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconSettings, 0.0f);
            break;
        case I_PAUSE:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconPause, 0.0f);
            break;
        case I_INVENTORY:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconInventory, 0.0f);
            break;
        case PHONE:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconPhone, 0.0f);
            break;
        default:
            break;
        }
        break;

    default:
        break;
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