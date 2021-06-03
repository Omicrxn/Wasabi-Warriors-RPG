#include "ScreenCredits.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"

ScreenCredits::ScreenCredits()
{
    iconReturnTitle = nullptr;

    mobileRect = { 375, 339, 392, 603 };
}

ScreenCredits::~ScreenCredits()
{
}

bool ScreenCredits::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, SDL_Texture* atlas0, Font* font, int hoverFx, int returnFx)
{
    this->currentScene = currentScene;
    this->atlas[0] = atlas0;
    this->font = font;

    this->guiManager = guiManager;
    this->win = win;

    uint width, height;
    win->GetWindowSize(width, height);

    this->minIndex = minIndex;
    this->maxIndex = maxIndex;
    int counterId = minIndex;

    iconReturnTitle = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 609, 580, 54, 54 });
    iconReturnTitle->SetIconProperties(currentScene, atlas0, font, hoverFx, returnFx, IconType::ICON_RETURN);

	return true;
}

bool ScreenCredits::Update(Input* input, float dt, uint& focusedButtonId)
{

    return true;
}

bool ScreenCredits::Draw(Render* render)
{
    render->DrawRectangle({ 0,0,1280,720 }, { 0, 0, 0, 70 }, true, false);

    render->DrawTexture(atlas[0], (1280 / 2 - mobileRect.w / 2) / render->scale, 50 / render->scale, &mobileRect, 0.0f);
    
    uint width, height;
    win->GetWindowSize(width, height);

    int positionX = (int)width / 2 - (int)((float)width / 10);

    render->DrawText(font, "Alex Avila", positionX, 120, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Ali Shahid", positionX, 160, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Yeray Tarifa", positionX, 200, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Carles Lopez", positionX, 240, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Maria Calle", positionX, 280, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Bosco Barber", positionX, 320, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Project II", positionX, 400, 35, 5, { 0,0,255,255 });
    render->DrawText(font, "UPC - CITM", positionX, 440, 35, 5, { 0,0,255,255 });
    render->DrawText(font, "MIT License 2021", positionX, 480, 35, 5, { 0,0,255,255 });

	return true;
}

bool ScreenCredits::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    guiManager->DestroyGuiControl(iconReturnTitle);

	return true;
}