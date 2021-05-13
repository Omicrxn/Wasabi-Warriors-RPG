#include "ScreenCredits.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"

ScreenCredits::ScreenCredits()
{
	creditsTitleRect = { 0, 238, 511, 84 };

    creditsBackgroundColor1 = { 74,79,95,255 };
    creditsBackgroundColor2 = { 128,137,154,255 };
}

ScreenCredits::~ScreenCredits()
{
}

bool ScreenCredits::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
{
    this->currentScene = currentScene;
    this->atlas[0] = atlas0;
    this->atlas[1] = atlas1;
    this->font = font;

    this->guiManager = guiManager;
    this->win = win;

    uint width, height;
    win->GetWindowSize(width, height);

    creditsBackgroundRect1 = { int(width) / 2 - 500, 75, 1000, 600 };
    creditsBackgroundRect2 = { creditsBackgroundRect1.x + 25, creditsBackgroundRect1.y + 25, creditsBackgroundRect1.w - 50, creditsBackgroundRect1.h - 50 };

	return true;
}

bool ScreenCredits::Update(Input* input, float dt, uint& focusedButtonId)
{
	return true;
}

bool ScreenCredits::Draw(Render* render)
{
    uint width, height;
    win->GetWindowSize(width, height);

    //render->scale = 3;
    //render->DrawTexture(atlas[0], 60, 25, &settingsBackgroundRect, 0.0f);
    //render->scale = 1;
    render->DrawRectangle(creditsBackgroundRect1, creditsBackgroundColor1, true, false);
    render->DrawRectangle(creditsBackgroundRect2, creditsBackgroundColor2, true, false);

    render->DrawTexture(atlas[1], width / 2 - creditsTitleRect.w / 2, 40, &creditsTitleRect, 0.0f);
    render->DrawText(font, "Authors", 250 + 3, 170, 40, 5, { 105,105,105,105 });
    render->DrawText(font, "Authors", 250, 170, 40, 5, { 255,255,255,255 });

    render->DrawText(font, "Alex Avila", 250 + 3, 240, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Alex Avila", 250, 240, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Ali Shahid", 250 + 3, 310, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Ali Shahid", 250, 310, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Yeray Tarifa", 250 + 3, 380, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Yeray Tarifa", 250, 380, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Carles Lopez", 250 + 3, 450, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Carles Lopez", 250, 450, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Maria Calle", 250 + 3, 520, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Maria Calle", 250, 520, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Bosco Barber", 250 + 3, 590, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Bosco Barber", 250, 590, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Project II", 650 + 3, 170, 40, 5, { 105,105,105,105 });
    render->DrawText(font, "Project II", 650, 170, 40, 5, { 255,255,255,255 });

    render->DrawText(font, "MIT License 2021", 650 + 3, 240, 40, 5, { 105,105,105,105 });
    render->DrawText(font, "MIT License 2021", 650, 240, 40, 5, { 255,255,255,255 });

    render->DrawText(font, "UPC - CITM", 650 + 3, 310, 40, 5, { 105,105,105,105 });
    render->DrawText(font, "UPC - CITM", 650, 310, 40, 5, { 0,0,255,255 });

	return true;
}

bool ScreenCredits::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	return true;
}