#include "GuiIcon.h"

GuiIcon::GuiIcon(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::ICON, id)
{
    this->bounds = bounds;

    iconType = IconType::NONE;

    isHovering = false;
    mouseFocus = false;

    hoverFx = -1;
    clickFx = -1;

    iconSettings = { 102, 402, 40, 39 };
    iconExit = { 165, 402, 36, 39 };
    iconTeam = { 387, 234, 90, 88 };
    iconQuest = { 519, 234, 87, 88 };
    iconMap = { 651, 234, 72, 88 };

    normalBox = { 237, 171, 108, 108 };
    focusedBox = { 393, 108, 108, 108 };
    pressedBox = { 237, 315, 108, 108 };

    normalCircle = { 36, 30, 48, 48 };
    focusedCircle = { 399, 30, 48, 48 };
    pressedCircle = { 36, 87, 48, 48 };

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
    if (state != GuiControlState::DISABLED && state != GuiControlState::HIDDEN)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;
            mouseFocus = true;

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
            mouseFocus = false;
        }
    }
    return true;
}

bool GuiIcon::Draw(Render* render, bool debugDraw)
{
    // Draw the right icon depending on state
    if (state == GuiControlState::NORMAL)
    {
        if (iconType == IconType::ICON_RETURN)
            render->DrawTexture(texture, bounds.x, bounds.y, &normalReturn, 0.0f);
    }
    else if (state == GuiControlState::FOCUSED)
    {
        switch (iconType)
        {
        case IconType::NONE:
            break;
        case IconType::ICON_RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &focusedReturn, 0.0f);
            break;
        case IconType::ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &focusedCircle, 0.0f);
            break;
        case IconType::ICON_EXIT:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &focusedCircle, 0.0f);
            break;
        case IconType::ICON_TEAM:
            render->DrawTexture(texture, bounds.x - 10, bounds.y - 10, &focusedBox, 0.0f);
            break;
        case IconType::ICON_QUEST:
            render->DrawTexture(texture, bounds.x - 10, bounds.y - 10, &focusedBox, 0.0f);
            break;
        case IconType::ICON_MAP:
            render->DrawTexture(texture, bounds.x - 17, bounds.y - 10, &focusedBox, 0.0f);
            break;
        default:
            break;
        }
    }
    else if (state == GuiControlState::PRESSED)
    {
        switch (iconType)
        {
        case IconType::NONE:
            break;
        case IconType::ICON_RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &pressedReturn, 0.0f);
            break;
        case IconType::ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &pressedCircle, 0.0f);
            break;
        case IconType::ICON_EXIT:
            render->DrawTexture(texture, bounds.x - 5, bounds.y - 7, &pressedCircle, 0.0f);
            break;
        case IconType::ICON_TEAM:
            render->DrawTexture(texture, bounds.x - 10, bounds.y - 10, &pressedBox, 0.0f);
            break;
        case IconType::ICON_QUEST:
            render->DrawTexture(texture, bounds.x - 10, bounds.y - 10, &pressedBox, 0.0f);
            break;
        case IconType::ICON_MAP:
            render->DrawTexture(texture, bounds.x - 17, bounds.y - 10, &pressedBox, 0.0f);
            break;
        default:
            break;
        }
    }

    if (state == GuiControlState::NORMAL || state == GuiControlState::FOCUSED || state == GuiControlState::PRESSED)
    {
        switch (iconType)
        {
        case IconType::NONE:
            break;
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