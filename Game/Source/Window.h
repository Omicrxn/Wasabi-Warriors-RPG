#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"
#include "AssetsManager.h"
struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window(AssetsManager* assetsManager);

	// Destructor
	virtual ~Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;
	// Retrieve window width
	uint GetWidth() const;
	// Retrieve window height
	uint GetHeight() const;
public:
	// The window we'll be rendering to
	SDL_Window* window;

	// The surface contained by the window
	SDL_Surface* screenSurface;

private:
	SString title;
	uint width;
	uint height;
	uint scale;
	AssetsManager* assetsManager;
	SDL_Surface* windowIcon;
};

#endif // __WINDOW_H__