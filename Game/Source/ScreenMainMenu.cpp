#include "ScreenMainMenu.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"
ScreenMainMenu::ScreenMainMenu()
{
    btnStart = nullptr;
    btnContinue = nullptr;
    btnOptions = nullptr;
    btnCredits = nullptr;
    btnExit = nullptr;

    mainTitlesRect = { 0, 0, 1073, 73 };
    titlePosition = { 0,0 };
}

ScreenMainMenu::~ScreenMainMenu()
{
}

bool ScreenMainMenu::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
{
    this->currentScene = currentScene;
    this->atlas[0] = atlas0;
    this->atlas[1] = atlas1;
    this->font = font;

    this->guiManager = guiManager;
    this->win = win;

    uint width, height;
    win->GetWindowSize(width, height);

    this->minIndex = minIndex;
    this->maxIndex = maxIndex;
    int counterId = minIndex;

    btnContinue = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { -350, 200, 190, 49 }, "CONTINUE");
    btnContinue->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);
    ++counterId;

    btnStart = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { -350, 300, 190, 49 }, "START");
    btnStart->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);
    ++counterId;

    btnOptions = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { -350, 400, 190, 49 }, "OPTIONS");
    btnOptions->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);
    ++counterId;

    btnCredits = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { -350, 500, 190, 49 }, "CREDITS");
    btnCredits->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);
    ++counterId;

    btnExit = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, counterId, { -350, 600, 190, 49 }, "EXIT");
    btnExit->SetButtonProperties(currentScene, atlas0, font, hoverFx, clickFx, Style::WHITE);

    
	return true;
}

bool ScreenMainMenu::Update(Input* input, float dt, uint& focusedButtonId)
{
    if ((input->GetControllerButton(CONTROLLER_BUTTON_UP) == KeyState::KEY_DOWN) && focusedButtonId > minIndex)
        --focusedButtonId;
    else if ((input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KeyState::KEY_DOWN) && focusedButtonId < maxIndex)
        ++focusedButtonId;

	return true;
}

bool ScreenMainMenu::Draw(Render* render)
{
    if (isActive)
    {
        render->DrawTexture(atlas[1], titlePosition.x, titlePosition.y, &mainTitlesRect, 0.0f);
    }
	return true;
}

bool ScreenMainMenu::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
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

