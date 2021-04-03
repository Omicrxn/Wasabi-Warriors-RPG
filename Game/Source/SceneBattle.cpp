#include "SceneBattle.h"

#include "Textures.h"
#include "Render.h"
#include "Font.h"
#include "Window.h"

#include "BattleSystem.h"

#include "GuiManager.h"
#include "GuiButton.h"

#include <string.h>

#define TITLE_FADE_SPEED 0.05f

SceneBattle::SceneBattle()
{
	type = SceneType::BATTLE;

    backgroundTex = nullptr;
    backgroundRect = { 0, 0, 0, 0 };

    guiAtlasTex = nullptr;

    btnAttack = nullptr;
    btnDefend = nullptr;
    btnItem = nullptr;
    btnRun = nullptr;
    btnNone = nullptr;

    titleFont = nullptr;
    buttonFont = nullptr;

    win = nullptr;

    menuCurrentSelection = ActionSelection::NONE;

    hoverFx = -1;
    clickFx = -1;

    controllerFocus = 0;
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityman)
{
    this->win = win;

    audio->PlayMusic("Assets/Audio/Music/menu.ogg");

    titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
    buttonFont = new Font("Assets/Fonts/showg.xml", tex);

    //backgroundTex = tex->Load("Assets/Textures/Scenes/main_menu.png");
    backgroundRect = { 0, 0, 1280, 720 };

    guiAtlasTex = tex->Load("Assets/Textures/UI/Elements/ui_spritesheet.png");

    hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

    uint width, height;
    win->GetWindowSize(width, height);

    this->guiManager = guiManager;

    btnAttack = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 100, 500, 190, 49 }, "ATTACK");
    btnAttack->SetObserver(this);
    btnAttack->SetTexture(guiAtlasTex);
    btnAttack->SetFont(buttonFont);
    btnAttack->SetButtonAudioFx(hoverFx, clickFx);

    btnDefend = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { 100, 600, 190, 49 }, "DEFEND");
    btnDefend->SetObserver(this);
    btnDefend->SetTexture(guiAtlasTex);
    btnDefend->SetFont(buttonFont);
    btnDefend->SetButtonAudioFx(hoverFx, clickFx);

    btnItem = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { 400, 500, 190, 49 }, "ITEM");
    btnItem->SetObserver(this);
    btnItem->SetTexture(guiAtlasTex);
    btnItem->SetFont(buttonFont);
    btnItem->SetButtonAudioFx(hoverFx, clickFx);

    btnRun = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { 400, 600, 190, 49 }, "RUN");
    btnRun->SetObserver(this);
    btnRun->SetTexture(guiAtlasTex);
    btnRun->SetFont(buttonFont);
    btnRun->SetButtonAudioFx(hoverFx, clickFx);

    btnNone = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { 700, 500, 190, 49 }, "NONE");
    btnNone->SetObserver(this);
    btnNone->SetTexture(guiAtlasTex);
    btnNone->SetFont(buttonFont);
    btnNone->SetButtonAudioFx(hoverFx, clickFx);

	return true;
}

bool SceneBattle::Update(Input* input, float dt)
{
    if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

    if (input->pads[0].enabled)
    {
            /* Input */
        if (((input->pads[0].up || input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)) && controllerFocus >= 1)
            --controllerFocus;
        else if (((input->pads[0].down || input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)) && controllerFocus <= 3)
            ++controllerFocus;

        for (int i = 0; i < 5; ++i)
        {
            if (i != controllerFocus)
            {
                // SET GAMEPAD FOCUS TO FALSE
                guiManager->controls.At(i)->data->gamepadFocus = false;
            }
            else
            {
                // SET GAMEPAD FOCUS TO TRUE
                guiManager->controls.At(i)->data->gamepadFocus = true;
            }
        }
    }

    if (menuCurrentSelection == ActionSelection::NONE)
        BattleSystem::GetInstance()->battleGUIState = BattleGUIState::NONE;
    else if (menuCurrentSelection == ActionSelection::ATTACK)
    {
        BattleSystem::GetInstance()->battleGUIState = BattleGUIState::ATTACK;
        menuCurrentSelection = ActionSelection::NONE;
    } 
    else if (menuCurrentSelection == ActionSelection::DEFEND)
    {
        BattleSystem::GetInstance()->battleGUIState = BattleGUIState::DEFEND;
        menuCurrentSelection = ActionSelection::NONE;
    }
    
    BattleSystem::GetInstance()->Update(input);

	return true;
}

bool SceneBattle::Draw(Render* render)
{
	render->DrawRectangle({ 0, 0, 1280, 720 }, { 255, 255, 255, (unsigned char)255.0f }, true, false);

    /*render->DrawTexture(backgroundTex, 0, 0, &backgroundRect);*/

    uint width, height;
    win->GetWindowSize(width, height);

    if (BattleSystem::GetInstance()->battleState == BattleState::PLAYER_TURN)
    {
        render->DrawText(titleFont, "Your turn", 100 + 3, 100 + 3, 125, 0, { 105, 105, 105, 255 });
        render->DrawText(titleFont, "Your turn", 100, 100, 125, 0, { 255, 255, 255, 255 });
    }
    else if (BattleSystem::GetInstance()->battleState == BattleState::ENEMY_TURN)
    {
        render->DrawText(titleFont, "Enemy turn", 100 + 3, 100 + 3, 125, 0, { 105, 105, 105, 255 });
        render->DrawText(titleFont, "Enemy turn", 100, 100, 125, 0, { 255, 255, 255, 255 });

        if (BattleSystem::GetInstance()->IsEnemyAttacking())
        {
            render->DrawText(titleFont, "Enemy is attacking!", 100 + 3, 200 + 3, 75, 0, { 105, 105, 105, 255 });
            render->DrawText(titleFont, "Enemy is attacking!", 100, 200, 75, 0, { 255, 255, 255, 255 });
        }
        else if (BattleSystem::GetInstance()->IsEnemyDefending())
        {
            render->DrawText(titleFont, "Enemy is defending!", 100 + 3, 200 + 3, 75, 0, { 105, 105, 105, 255 });
            render->DrawText(titleFont, "Enemy is defending!", 100, 200, 75, 0, { 255, 255, 255, 255 });
        }
    }

    // Player name
    render->DrawText(titleFont, BattleSystem::GetInstance()->GetPlayer()->stats.name.GetString(), 100 + 3, 300 + 3, 75, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, BattleSystem::GetInstance()->GetPlayer()->stats.name.GetString(), 100, 300, 75, 0, { 255, 255, 255, 255 });

    // Player life
    char HP[8] = { 0 };
    sprintf_s(HP, 8, "HP: %03i", BattleSystem::GetInstance()->GetPlayer()->stats.currentHP);
    render->DrawText(titleFont, HP, 100 + 3, 375 + 3, 75, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, HP, 100, 375, 75, 0, { 255, 255, 255, 255 });

    // Enemy name
    render->DrawText(titleFont, BattleSystem::GetInstance()->GetEnemy()->stats.name.GetString(), 800 + 3, 100 + 3, 75, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, BattleSystem::GetInstance()->GetEnemy()->stats.name.GetString(), 800, 100, 75, 0, { 255, 255, 255, 255 });

    sprintf_s(HP, 8, "HP: %03i", BattleSystem::GetInstance()->GetEnemy()->stats.currentHP);
    render->DrawText(titleFont, HP, 800 + 3, 175 + 3, 75, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, HP, 800, 175, 75, 0, { 255, 255, 255, 255 });

	return true;
}

bool SceneBattle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    tex->UnLoad(backgroundTex);
    tex->UnLoad(guiAtlasTex);

    guiManager->DestroyGuiControl(btnAttack);
    guiManager->DestroyGuiControl(btnDefend);
    guiManager->DestroyGuiControl(btnItem);
    guiManager->DestroyGuiControl(btnRun);
    guiManager->DestroyGuiControl(btnNone);

    RELEASE(titleFont);
    RELEASE(buttonFont);

	return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 1) menuCurrentSelection = ActionSelection::ATTACK;
        else if (control->id == 2) menuCurrentSelection = ActionSelection::DEFEND;
        else if (control->id == 3) menuCurrentSelection = ActionSelection::ITEM;
        else if (control->id == 4) menuCurrentSelection = ActionSelection::RUN;
        else if (control->id == 5) menuCurrentSelection = ActionSelection::NONE;
        break;
    }
    default: break;
    }

    return true;
}
