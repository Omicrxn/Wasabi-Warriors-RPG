#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiManager.h"

#include "GuiButton.h"
#include "Font.h"

#include "SDL/include/SDL.h"

SceneTitle::SceneTitle()
{
    type = SceneType::TITLE;

    backgroundTex = nullptr;
    backgroundRect = { 0, 0, 0, 0 };

    guiAtlasTex = nullptr;

    btnStart = nullptr;
    btnCredits = nullptr;
    btnExit = nullptr;

    font = nullptr;

    mousePos = { 0,0 };
    clicking = false;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex, GuiManager* guiManager)
{
    font = new Font("Assets/Fonts/SHOWG.xml", tex);

    backgroundTex = tex->Load("Assets/Textures/Scenes/main_menu.png");
    backgroundRect = { 0, 0, 1280, 720 };

    guiAtlasTex = tex->Load("Assets/Textures/UI/Elements/ui_spritesheet.png");

    mouseRect[0] = { 30,482,30,30 };
    mouseRect[1] = { 60,482,30,30 };

    btnStart = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 1280 / 2 - 300 / 2, 200, 190, 49 }, "START");
    btnStart->SetObserver(this);
    btnStart->SetTexture(guiAtlasTex);
    btnStart->SetFont(font);

    btnContinue = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 1280 / 2 - 300 / 2, 300, 190, 49 }, "CONTINUE");
    btnContinue->SetObserver(this);
    btnContinue->SetTexture(guiAtlasTex);
    btnContinue->SetFont(font);

    btnOptions = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 1280 / 2 - 300 / 2, 400, 190, 49 }, "OPTIONS");
    btnOptions->SetObserver(this);
    btnOptions->SetTexture(guiAtlasTex);
    btnOptions->SetFont(font);

    btnExit = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 1280 / 2 - 300 / 2, 500, 190, 49 }, "EXIT");
    btnExit->SetObserver(this);
    btnExit->SetTexture(guiAtlasTex);
    btnExit->SetFont(font);

    return true;
}

bool SceneTitle::Update(Input* input, float dt)
{
    //if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

    backgroundAnim.Update();

    input->GetMousePosition(mousePos.x, mousePos.y);

    if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
        clicking = true;
    else
        clicking = false;

    return true;
}

bool SceneTitle::Draw(Render* render)
{
    render->DrawTexture(backgroundTex, 0, 0, &backgroundRect);

    if (clicking)
        render->DrawTexture(guiAtlasTex, mousePos.x, mousePos.y, &mouseRect[0], 0.0f);
    else
        render->DrawTexture(guiAtlasTex, mousePos.x, mousePos.y, &mouseRect[1], 0.0f);

    /*char score[64] = { 0 };
    sprintf_s(score, 64, "SCORE: %03i", 56);

    render->DrawText(font, score, 10, 10, 200, 0, { 255, 0, 255, 255 });*/

    return true;
}

bool SceneTitle::Unload(Textures* tex, GuiManager* guiManager)
{
    tex->UnLoad(backgroundTex);
    tex->UnLoad(guiAtlasTex);

    guiManager->DestroyGuiControl(btnStart);
    guiManager->DestroyGuiControl(btnContinue);
    guiManager->DestroyGuiControl(btnOptions);
    guiManager->DestroyGuiControl(btnExit);

    return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 1) TransitionToScene(SceneType::GAMEPLAY);
        else if (control->id == 2) return false; // TODO: Exit request
    }
    default: break;
    }

    return true;
}