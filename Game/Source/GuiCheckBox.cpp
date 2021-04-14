#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, AudioManager* audio, float dt)
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

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
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
        if (checked) render->DrawTexture(texture, bounds.x, bounds.y, &greyTick, 0.0f);
        
        render->DrawText(font, text.GetString(), bounds.x + bounds.h, bounds.y, 22, 3, { 255,255,255,255 });
        break;
    case GuiControlState::FOCUSED:
        render->DrawTexture(texture, bounds.x, bounds.y, &whiteBox, 0.0f);
        if (checked) render->DrawTexture(texture, bounds.x, bounds.y, &greyTick, 0.0f);
        
        // Arrow

        render->DrawText(font, text.GetString(), bounds.x + bounds.h, bounds.y, 22, 3, { 0,0,0,255 });
        break;
    case GuiControlState::PRESSED:
        render->DrawTexture(texture, bounds.x, bounds.y + 4, &whiteBox, 0.0f);
        if (checked) render->DrawTexture(texture, bounds.x, bounds.y + 4, &greyTick, 0.0f);

        // Arrow

        render->DrawText(font, text.GetString(), bounds.x + bounds.h, bounds.y + 4, 22, 3, { 0,0,0,255 });
        break;
    default:
        break;
    }
    return true;
}

void GuiCheckBox::SetCheckBoxProperties(Scene* module, SDL_Texture* texture, Font* font, int hoverFx, int clickFx)
{
    SetObserver(module);
    SetTexture(texture);
    SetFont(font);

    this->hoverFx = hoverFx;
    this->clickFx = clickFx;
}