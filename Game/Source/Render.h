#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"
#include "Font.h"

#include "SDL/include/SDL.h"

class Window;

class Render : public Module
{
public:

	Render(Window* win);

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;
	void CameraFollow(iPoint objectPosition);
	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	bool DrawRectangle(const SDL_Rect& rect, SDL_Color color, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, SDL_Color color) const;
	bool DrawCircle(int x1, int y1, int redius, SDL_Color color) const;

	bool DrawText(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint);

	// Set background color
	void SetBackgroundColor(SDL_Color color);

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;

	Window* win;
	uint scale;
};

#endif // __RENDER_H__