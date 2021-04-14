#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;

    buttonStyle = Style::WHITE;

    isHovering = false;
    gamepadFocus = false;

    hoverFx = -1;
    clickFx = -1;

    greyButton = { 0,188,190,49 };

    yellowButton = { 0,282,190,49 };
    yellowButtonPressed = { 0,237,190,45 };

    arrowYellowLeft = { 303,486,22,21 };
    arrowYellowRight = { 171,486,22,21 };

    whiteButton = { 0,0,190,49 };
    whiteButtonPressed = { 0,331,190,45 };

    arrowWhiteLeft = { 281,486,22,21 };
    arrowWhiteRight = { 259,486,22,21 };

    iconReturn = { 1117, 29, 70, 55 };
    iconDoor = { 1000, 125, 46, 55 };
    iconSettings = { 870, 125, 58, 55 };
}

GuiButton::~GuiButton()
{
}

void GuiButton::SetButtonProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, Style style)
{
    SetObserver(module);
    SetTexture(texture);
    SetFont(font);

    this->hoverFx = hoverFx;
    this->clickFx = clickFx;

    this->buttonStyle = style;
}

bool GuiButton::Update(Input* input, AudioManager* audio, float dt)
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
            gamepadFocus = false;
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
        switch (buttonStyle)
        {
        case WHITE:
            render->DrawTexture(texture, bounds.x, bounds.y, &whiteButton, 0.0f);
            break;
        case GREY:
            break;
        case ICON_RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconReturn, 0.0f);
            break;
        case ICON_DOOR:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconDoor, 0.0f);
            break;
        case ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconSettings, 0.0f);
            break;
        default:
            break;
        }
        render->DrawText(font, text.GetString(), bounds.x + bounds.w / 5 - bounds.w / 6, bounds.y + bounds.h / 2 - bounds.h / 4, 22, 8, { 105,105,105,255 });
        break;
    case GuiControlState::FOCUSED:
        switch (buttonStyle)
        {
        case WHITE:
            render->DrawTexture(texture, bounds.x, bounds.y, &whiteButton, 0.0f);
            render->DrawTexture(texture, bounds.x - 30, bounds.y + 14, &arrowWhiteRight, 0.0f);
            render->DrawTexture(texture, bounds.x + bounds.w + 8, bounds.y + 14, &arrowWhiteLeft, 0.0f);
            break;
        case GREY:
            break;
        case ICON_RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconReturn, 0.0f);
            break;
        case ICON_DOOR:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconDoor, 0.0f);
            break;
        case ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconSettings, 0.0f);
            break;
        default:
            break;
        }
        render->DrawText(font, text.GetString(), bounds.x + bounds.w / 5 - bounds.w / 6 + 2, bounds.y + bounds.h / 2 - bounds.h / 4 + 2, 22, 8, { 105,105,105,255 });
        render->DrawText(font, text.GetString(), bounds.x + bounds.w / 5 - bounds.w / 6, bounds.y + bounds.h / 2 - bounds.h / 4, 22, 8, { 0,0,0,255 });
        break;
    case GuiControlState::PRESSED:
        switch (buttonStyle)
        {
        case WHITE:
            render->DrawTexture(texture, bounds.x, bounds.y + 4, &whiteButtonPressed, 0.0f);
            render->DrawTexture(texture, bounds.x - 30, bounds.y + 18, &arrowWhiteRight, 0.0f);
            render->DrawTexture(texture, bounds.x + bounds.w + 8, bounds.y + 18, &arrowWhiteLeft, 0.0f);
            break;
        case GREY:
            break;
        case ICON_RETURN:
            render->scale = 2;
            render->DrawTexture(texture, bounds.x, bounds.y, &iconReturn, 0.0f);
            render->scale = 1;
            break;
        case ICON_DOOR:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconDoor, 0.0f);
            break;
        case ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconSettings, 0.0f);
            break;
        default:
            break;
        }
        render->DrawText(font, text.GetString(), bounds.x + bounds.w / 5 - bounds.w / 6 + 2, bounds.y + bounds.h / 2 - bounds.h / 4 + 4 + 2, 22, 8, { 105,105,105,255 });
        render->DrawText(font, text.GetString(), bounds.x + bounds.w / 5 - bounds.w / 6, bounds.y + bounds.h / 2 - bounds.h / 4 + 4, 22, 8, { 0,0,0,255 });
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