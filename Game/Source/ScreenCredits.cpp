#include "ScreenCredits.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"

ScreenCredits::ScreenCredits()
{
	settingsBackgroundRect = { 1228, 295, 300, 200 };
	creditsTitleRect = { 0, 238, 511, 84 };
}

ScreenCredits::~ScreenCredits()
{
}

bool ScreenCredits::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
{
    this->currentScene = currentScene;
    this->atlas[0] = atlas0;
    this->atlas[1] = atlas1;
    this->font = font;

    this->guiManager = guiManager;
    this->win = win;

 
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

    render->scale = 3;
    render->DrawTexture(atlas[0], 60, 25, &settingsBackgroundRect, 0.0f);
    render->scale = 1;

    render->DrawTexture(atlas[1], width / 2 - creditsTitleRect.w / 2, 40, &creditsTitleRect, 0.0f);
    render->DrawText(font, "Authors", 250 + 3, 180, 40, 5, { 105,105,105,105 });
    render->DrawText(font, "Authors", 250, 180, 40, 5, { 255,255,255,255 });

    render->DrawText(font, "Alex Avila", 250 + 3, 250, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Alex Avila", 250, 250, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Ali Shahid", 250 + 3, 320, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Ali Shahid", 250, 320, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Yeray Tarifa", 250 + 3, 390, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Yeray Tarifa", 250, 390, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Carles Lopez", 250 + 3, 460, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Carles Lopez", 250, 460, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Maria Calle", 250 + 3, 530, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Maria Calle", 250, 530, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Bosco Barber", 250 + 3, 600, 35, 5, { 105,105,105,105 });
    render->DrawText(font, "Bosco Barber", 250, 600, 35, 5, { 0,0,0,0 });

    render->DrawText(font, "Project II", 650 + 3, 180, 40, 5, { 105,105,105,105 });
    render->DrawText(font, "Project II", 650, 180, 40, 5, { 255,255,255,255 });

    render->DrawText(font, "MIT License 2021", 650 + 3, 250, 40, 5, { 105,105,105,105 });
    render->DrawText(font, "MIT License 2021", 650, 250, 40, 5, { 255,255,255,255 });

    render->DrawText(font, "UPC - CITM", 650 + 3, 320, 40, 5, { 105,105,105,105 });
    render->DrawText(font, "UPC - CITM", 650, 320, 40, 5, { 0,0,255,255 });
	return true;
}

bool ScreenCredits::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	return true;
}
