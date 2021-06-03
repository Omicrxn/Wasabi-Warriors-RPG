#include "GuiIcon.h"

GuiIcon::GuiIcon(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::ICON, id)
{
    this->bounds = bounds;

    isHovering = false;
    gamepadFocus = false;

    controller = false;

    hoverFx = -1;
    clickFx = -1;

    iconSettings = { 102, 402, 40, 39 };
    iconExit = { 165, 402, 36, 39 };
    iconTeam = { 387, 234, 90, 88 };
    iconQuest = { 519, 234, 87, 88 };
    iconMap = { 651, 234, 72, 88 };

    grayBox = { 393, 108, 108, 108 };
    pinkBox = { 237, 315, 108, 108 };

    whiteCircle = { 36, 30, 48, 48 };
    grayCircle = { 399, 30, 48, 48 };
    pinkCircle = { 36, 87, 48, 48 };

    normalReturn = { 888, 972, 54, 54 };
    focusedReturn = { 825, 972, 54, 54 };
    pressedReturn = { 951, 972, 54, 54 };
}

GuiIcon::~GuiIcon()
{
}

void GuiIcon::SetIconProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx, IconType type)
{
    SetObserver(module);
    SetTexture(texture);
    SetFont(font);

    this->hoverFx = hoverFx;
    this->clickFx = clickFx;

    iconType = type;
}

bool GuiIcon::Update(Input* input, AudioManager* audio, float dt)
{
    controller = input->GetControllerState();

    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check if gamepad is focusing the icon
        if (gamepadFocus && input->GetControllerState())
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
                NotifyObserver();
                // Audio Fx when pressed
                audio->PlayFx(clickFx);
            }
        }
        // Check collision between mouse and icon bounds
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
                NotifyObserver();
                // Audio Fx when clicked
                audio->PlayFx(clickFx);
            }
        }
        else if (iconType == IconType::ICON_RETURN)
        {
            if (input->GetControllerButton(CONTROLLER_BUTTON_B) == KeyState::KEY_REPEAT || input->GetControllerButton(CONTROLLER_BUTTON_B) == KeyState::KEY_DOWN
                || input->GetControllerButton(CONTROLLER_BUTTON_START) == KeyState::KEY_REPEAT || input->GetControllerButton(CONTROLLER_BUTTON_START) == KeyState::KEY_DOWN
                || input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_REPEAT || input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
            }

            // If gamepad button pressed -> Generate event!
            if (input->GetControllerButton(CONTROLLER_BUTTON_B) == KeyState::KEY_UP
                || input->GetControllerButton(CONTROLLER_BUTTON_START) == KeyState::KEY_UP
                || input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_UP)
            {
                NotifyObserver();
                // Audio Fx when pressed
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

bool GuiIcon::Draw(Render* render, bool debugDraw)
{
    // Draw the right icon depending on state
    if (state == GuiControlState::NORMAL)
    {
        switch (iconType)
        {
        case IconType::ICON_RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &normalReturn, 0.0f);
            if (controller)
                render->DrawText(font, "B", bounds.x + bounds.w + 5, bounds.y + 12, 28, 3, { 0,0,0,255 });
            else
                render->DrawText(font, "ESC", bounds.x + bounds.w + 5, bounds.y + 12, 28, 3, { 0,0,0,255 });
            break;
        case IconType::ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &whiteCircle, 0.0f);
            break;
        case IconType::ICON_EXIT:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &whiteCircle, 0.0f);
            break;
        default:
            break;
        }
    }
    else if (state == GuiControlState::FOCUSED)
    {
        switch (iconType)
        {
        case IconType::ICON_RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &focusedReturn, 0.0f);
            break;
        case IconType::ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &grayCircle, 0.0f);
            break;
        case IconType::ICON_EXIT:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &grayCircle, 0.0f);
            break;
        case IconType::ICON_TEAM:
            render->DrawTexture(texture, bounds.x - 10, bounds.y - 10, &grayBox, 0.0f);
            break;
        case IconType::ICON_QUEST:
            render->DrawTexture(texture, bounds.x - 10, bounds.y - 10, &grayBox, 0.0f);
            break;
        case IconType::ICON_MAP:
            render->DrawTexture(texture, bounds.x - 17, bounds.y - 10, &grayBox, 0.0f);
            break;
        default:
            break;
        }
    }
    else if (state == GuiControlState::PRESSED)
    {
        switch (iconType)
        {
        case IconType::ICON_RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &pressedReturn, 0.0f);
            break;
        case IconType::ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &pinkCircle, 0.0f);
            break;
        case IconType::ICON_EXIT:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &pinkCircle, 0.0f);
            break;
        case IconType::ICON_TEAM:
            render->DrawTexture(texture, bounds.x - 10, bounds.y - 10, &pinkBox, 0.0f);
            break;
        case IconType::ICON_QUEST:
            render->DrawTexture(texture, bounds.x - 10, bounds.y - 10, &pinkBox, 0.0f);
            break;
        case IconType::ICON_MAP:
            render->DrawTexture(texture, bounds.x - 17, bounds.y - 10, &pinkBox, 0.0f);
            break;
        default:
            break;
        }
    }

    if (state == GuiControlState::NORMAL || state == GuiControlState::FOCUSED || state == GuiControlState::PRESSED)
    {
        switch (iconType)
        {
        case IconType::ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconSettings, 0.0f);
            break;
        case IconType::ICON_EXIT:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconExit, 0.0f);
            break;
        case IconType::ICON_TEAM:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconTeam, 0.0f);
            break;
        case IconType::ICON_QUEST:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconQuest, 0.0f);
            break;
        case IconType::ICON_MAP:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconMap, 0.0f);
            break;
        default:
            break;
        }
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