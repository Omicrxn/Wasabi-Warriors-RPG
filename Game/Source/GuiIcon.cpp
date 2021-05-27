#include "GuiIcon.h"

GuiIcon::GuiIcon(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::ICON, id)
{
    this->bounds = bounds;

    iconType = IconType::NONE;

    isHovering = false;
    mouseFocus = false;

    hoverFx = -1;
    clickFx = -1;

    //arrowWhiteRight = { 160,50,18,18 };
    iconPhone = { 20, 14, 52, 66 };
    iconInventory = { 84, 16, 60, 62 };

    iconSettings = { 102, 402, 40, 39 };
    iconExit = { 165, 402, 36, 39 };
    iconReturn = { 540, 870, 54, 54 };
    
    iconTeam = { 387, 234, 90, 88 };
    iconQuest = { 519, 234, 87, 88 };
    iconMap = { 651, 234, 72, 88 };

    iconA = { 684, 8, 57, 57 };
    iconX = { 684, 196, 57, 57 };
    iconB = { 684, 101, 57, 57 };
    iconSTART = { 685, 291, 55, 54 };

    controller = false;
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
        controller = input->GetControllerState();

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
        else if ((iconType == IconType::ICON_PHONE || iconType == IconType::ICON_RESUME) && input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
        {
            NotifyObserver();
            // Audio Fx when clicked
            audio->PlayFx(clickFx);
        }
        else if (controller && input->GetControllerButton(assignedControl) == KeyState::KEY_DOWN)
        {
            NotifyObserver();
            // Audio Fx when pressed
            audio->PlayFx(clickFx);
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
    if (state == GuiControlState::NORMAL || state == GuiControlState::FOCUSED || state == GuiControlState::PRESSED)
    {
        switch (iconType)
        {
        case IconType::NONE:
            break;
        case IconType::ICON_RETURN:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconReturn, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconB, 0.0f);
            break;
        case IconType::ICON_PHONE:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconPhone, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconSTART, 0.0f);
            break;
        case IconType::ICON_INVENTORY:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconInventory, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconX, 0.0f);
            break;
        case IconType::ICON_RESUME:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconReturn, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconSTART, 0.0f);
            break;
        case IconType::ICON_SETTINGS:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconSettings, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconX, 0.0f);
            break;
        case IconType::ICON_EXIT:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconExit, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconB, 0.0f);
            break;
        case IconType::ICON_TEAM:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconTeam, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconB, 0.0f);
            break;
        case IconType::ICON_QUEST:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconQuest, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconB, 0.0f);
            break;
        case IconType::ICON_MAP:
            render->DrawTexture(texture, bounds.x, bounds.y, &iconMap, 0.0f);
            if (controller)
                render->DrawTexture(texture, bounds.x + bounds.w, bounds.y + bounds.h, &iconB, 0.0f);
            break;
        default:
            break;
        }
    }
    if (state == GuiControlState::FOCUSED)
        render->DrawTexture(texture, bounds.x - 45, bounds.y + 14, &arrowWhiteRight, 0.0f);
    else if (state == GuiControlState::PRESSED)
        render->DrawTexture(texture, bounds.x - 25, bounds.y + 14, &arrowWhiteRight, 0.0f);

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