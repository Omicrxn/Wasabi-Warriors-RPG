#include "Render.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"
#include "Font.h"

#define VSYNC true

Render::Render(Window* win) : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;

	this->win = win;
	scale = 1;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = win->screenSurface->w;
		camera.h = win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// L02: DONE 6: Implement a method to load the state, for now load camera's x and y
// Load Game State
bool Render::LoadState(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// L02: DONE 8: Create a method to save the state of the renderer
// Save Game State
bool Render::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}
void Render::CameraFollow(int objectX, int objectY)
{

	camera.x = objectX * scale - win->GetWidth()/2;
	camera.y = objectY * scale - win->GetHeight()/4;
}
// Draw to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY, SDL_RendererFlip flip) const
{
	bool ret = true;

	SDL_Rect rect;

	rect.x = (x * scale) - (camera.x * speed);
	rect.y = (y * scale) - (camera.y * speed);

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, SDL_Color color, bool filled, bool useCamera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_Rect screen(rect);
	if (useCamera)
	{
		screen.x = (rect.x * scale) - (camera.x);
		screen.y = (rect.y * scale) - (camera.y);
		screen.w *= scale;
		screen.h *= scale;
	}
	int result = (filled) ? SDL_RenderFillRect(renderer, &screen) : SDL_RenderDrawRect(renderer, &screen);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, SDL_Color color) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int result = -1;

	result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, SDL_Color color) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint)
{
	bool ret = true;

	int length = strlen(text);
	int posX = x;

	float scale = (float)size / font->GetCharRec(32).h;

	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);

	for (int i = 0; i < length; i++)
	{
		SDL_Rect recGlyph = font->GetCharRec(text[i]);
		SDL_Rect recDest = { posX, y, (scale*recGlyph.w), size };

		SDL_RenderCopyEx(renderer, font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);

		posX += ((float)recGlyph.w*scale + spacing);
	}

	return ret;
}