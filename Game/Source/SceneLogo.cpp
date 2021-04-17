#include "SceneLogo.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "GuiManager.h"
#include "Easing.h"

#define LOGO_FADE_SPEED 1.0f

SceneLogo::SceneLogo()
{
    type = SceneType::LOGO;

    this->name = "scenelogo";

    logoTex = nullptr;
    logo = { 0, 0, 1280, 720 };

    logoFx = -1;
    state = 0;
    timeCounter = 0.0f;
    logoAlpha = 0.0f;

    logoFxTimer.Start();
}

SceneLogo::~SceneLogo()
{
}

bool SceneLogo::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing)
{
    this->audio = audio;

    logoTex = tex->Load("Assets/Textures/Scenes/logo_scene.png");
    logoFx = audio->LoadFx("Assets/Audio/Fx/logo.wav");

    logoFxTimer.Start();
    return true;
}

bool SceneLogo::Update(Input* input, float dt)
{
    if (state == 0)
    {
        state = 1;
    }
    else if (state == 1)
    {
        logoAlpha += (LOGO_FADE_SPEED * dt);

        if (logoAlpha > 1.0f)
        {
            logoAlpha = 1.0f;
            state = 2;
        }
    }
    else if (state == 2)
    {
        // Waiting for 3 seconds
        timeCounter += dt;
        if (timeCounter >= 3.0f) state = 3;
    }
    else if (state == 3)
    {
        logoAlpha -= (LOGO_FADE_SPEED * dt);

        if (logoAlpha < 0.0f)
        {
            logoAlpha = 0.0f;
            TransitionToScene(SceneType::TITLE);
        }
    }

    // For debugging purposes
    if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_A) == KEY_DOWN) 
        TransitionToScene(SceneType::TITLE);

    // Main title FX sounds just at title appearing
    if (logoFxTimer.ReadSec() >= 0.0f && logoFxTimer.ReadSec() < 0.1f)
    {
        audio->PlayFx(logoFx);
    }

    return true;
}

bool SceneLogo::Draw(Render* render)
{
    // Set texture alpha with the updated logoAlpha to accomplish fade in / fade out
    SDL_SetTextureAlphaMod(logoTex, logoAlpha);
    render->DrawTexture(logoTex, 0, 0, &logo);

    return true;
}

bool SceneLogo::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    tex->UnLoad(logoTex);
    audio->UnloadFx(logoFx);
    return true;
}