#include "ScreenTitle.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"
ScreenTitle::ScreenTitle()
{
    btnStart = nullptr;
    btnContinue = nullptr;
    btnOptions = nullptr;
    btnCredits = nullptr;
    btnExit = nullptr;

    mainTitlesRect = { 0, 0, 1073, 73 };
    titlePosition = { 0,0 };
}

ScreenTitle::~ScreenTitle()
{
}

bool ScreenTitle::Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
{
    this->currentScene = currentScene;
    this->atlas[0] = atlas0;
    this->atlas[1] = atlas1;
    this->font = font;

    this->guiManager = guiManager;
    this->win = win;

    uint width, height;
    win->GetWindowSize(width, height);

    btnContinue = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, { -350, 200, 190, 49 }, "CONTINUE");
    btnContinue->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);

    btnStart = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { -350, 300, 190, 49 }, "START");
    btnStart->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);

    btnOptions = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { -350, 400, 190, 49 }, "OPTIONS");
    btnOptions->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);

    btnCredits = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { -350, 500, 190, 49 }, "CREDITS");
    btnCredits->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);

    btnExit = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { -350, 600, 190, 49 }, "EXIT");
    btnExit->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);

    this->minIndex = 0;
    this->maxIndex = 4;
	return true;
}

bool ScreenTitle::Update(Input* input, float dt, uint& focusedButtonId)
{
    if ((input->GetControllerButton(CONTROLLER_BUTTON_UP) == KeyState::KEY_DOWN) && focusedButtonId > 0)
        --focusedButtonId;
    else if ((input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KeyState::KEY_DOWN) && focusedButtonId < 4)
        ++focusedButtonId;

	return true;
}

bool ScreenTitle::Draw(Render* render)
{
    if (isActive)
    {
        render->DrawTexture(atlas[1], titlePosition.x, titlePosition.y, &mainTitlesRect, 0.0f);
    }
	return true;
}

bool ScreenTitle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    guiManager->DestroyGuiControl(btnStart);
    btnStart = nullptr;
    guiManager->DestroyGuiControl(btnContinue);
    btnContinue = nullptr;
    guiManager->DestroyGuiControl(btnOptions);
    btnOptions = nullptr;
    guiManager->DestroyGuiControl(btnCredits);
    btnCredits = nullptr;
    guiManager->DestroyGuiControl(btnExit);
    btnExit = nullptr;

	return true;
}

