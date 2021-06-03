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
    whiteButton = { 0,0,190,49 };
    whiteButtonPressed = { 0,331,190,45 };

    backgroundButtonAnim = { 0,0,0,0 };
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

    if (style != Style::WHITE)
    {
        /*
        * [0] is the full sushi
        * [1] is the bitten sushi
        * [2] is the almost fully eaten sushi
        */
        switch (style)
        {
        case Style::SUSHI_ORANGE:
            buttonAnim.PushBack({ 768, 30, 204, 66 });
            buttonAnim.PushBack({ 996, 30, 150, 66 });
            buttonAnim.PushBack({ 1172, 30, 99, 66 });
            backgroundButtonAnim = {1302, 30, 204, 66};
            break;
        case Style::SUSHI_RED:
            buttonAnim.PushBack({ 768, 105, 204, 66 });
            buttonAnim.PushBack({ 996, 105, 150, 66 });
            buttonAnim.PushBack({ 1172, 105, 99, 66 });
            backgroundButtonAnim = { 1302, 105, 204, 66 };
            break;
        case Style::SUSHI_YELLOW:
            buttonAnim.PushBack({ 768, 186, 204, 66 });
            buttonAnim.PushBack({ 996, 186, 150, 66 });
            buttonAnim.PushBack({ 1172, 186, 99, 66 });
            backgroundButtonAnim = { 1302, 186, 204, 66 };
            break;
        default:
            break;
        }
    }
}

bool GuiButton::Update(Input* input, AudioManager* audio, float dt)
{
    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check if gamepad is focusing the button
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
        // Check collision between mouse and button bounds
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
        break;
    case GuiControlState::HIDDEN:
        break;
    case GuiControlState::NORMAL:
        switch (buttonStyle)
        {
        case Style::WHITE:
            render->DrawTexture(texture, bounds.x, bounds.y, &greyButton, 0.0f);
            render->DrawText(font, text.GetString(), bounds.x + 10 + 3, bounds.y + 3 + 3, 40, 8, { 128,128,128,255 });
            render->DrawText(font, text.GetString(), bounds.x + 10, bounds.y + 3, 40, 8, { 255,255,255,255 });
            break;
        default:
            // Sushi styles
            render->DrawTexture(texture, bounds.x, bounds.y, &backgroundButtonAnim, 0.0f);
            render->DrawTexture(texture, bounds.x, bounds.y, &buttonAnim.frames[0], 0.0f);
            if (buttonStyle == Style::SUSHI_RED)
                render->DrawText(font, text.GetString(), bounds.x + 50, bounds.y + 25, 30, 8, { 0,0,0,255 });
            else
                render->DrawText(font, text.GetString(), bounds.x + 20, bounds.y + 25, 30, 8, { 0,0,0,255 });
            break;
        }
        break;
    case GuiControlState::FOCUSED:
        switch (buttonStyle)
        {
        case Style::WHITE:
            render->DrawTexture(texture, bounds.x, bounds.y, &whiteButton, 0.0f);
            render->DrawText(font, text.GetString(), bounds.x + 7 + 3, bounds.y + 0 + 3, 40, 8, { 64,64,64,255 });
            render->DrawText(font, text.GetString(), bounds.x + 7, bounds.y + 0, 40, 8, { 128,128,128,255 });
            break;
        default:
            // Sushi styles
            render->DrawTexture(texture, bounds.x, bounds.y, &backgroundButtonAnim, 0.0f);
            render->DrawTexture(texture, bounds.x, bounds.y, &buttonAnim.frames[1], 0.0f);
            if (buttonStyle == Style::SUSHI_RED)
                render->DrawText(font, text.GetString(), bounds.x + 50, bounds.y + 25, 30, 8, { 0,0,0,255 });
            else
                render->DrawText(font, text.GetString(), bounds.x + 20, bounds.y + 25, 30, 8, { 0,0,0,255 });
            break;
        }
        break;
    case GuiControlState::PRESSED:
        switch (buttonStyle)
        {
        case Style::WHITE:
            render->DrawTexture(texture, bounds.x, bounds.y + 4, &whiteButtonPressed, 0.0f);
            render->DrawText(font, text.GetString(), bounds.x + 10 + 3, bounds.y + 3 + 3, 40, 8, { 64,64,64,255 });
            render->DrawText(font, text.GetString(), bounds.x + 10, bounds.y + 3, 40, 8, { 128,128,128,255 });
            break;
        default:
            // Sushi styles
            render->DrawTexture(texture, bounds.x, bounds.y, &backgroundButtonAnim, 0.0f);
            render->DrawTexture(texture, bounds.x, bounds.y, &buttonAnim.frames[2], 0.0f);
            if (buttonStyle == Style::SUSHI_RED)
                render->DrawText(font, text.GetString(), bounds.x + 50, bounds.y + 25, 30, 8, { 0,0,0,255 });
            else
                render->DrawText(font, text.GetString(), bounds.x + 20, bounds.y + 25, 30, 8, { 0,0,0,255 });
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