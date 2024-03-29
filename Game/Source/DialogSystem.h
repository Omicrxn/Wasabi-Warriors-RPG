#pragma once

#include <map>
#include <string>
#include <functional>

#include "Module.h"
#include "SDL/include/SDL_rect.h"
#include "Animation.h"

class DialogNode;
class Font;

class Input;
class Render;
class AssetsManager;
class Fonts;
class Textures;

struct SDL_Texture;

class DialogSystem : public Module
{
public:

	DialogSystem(Input* input, Render* render, Textures* tex, Fonts* fonts, AssetsManager* assetsManager);

	// Destructor
	virtual ~DialogSystem();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	void DrawDialog();

	// Called before quitting
	bool CleanUp();

	bool LoadDialog(const char* filename);

	void StartDialog(const char* id);

	void NextDialog();

	bool DialogHasFinished();

	int GetDialogIndex();

	void SetDialogFinished(bool dialogFinished);

	void NewDialog(int dialogIndex);

	const char* GetRightSpeaker();
	const char* GetLeftSpeaker();

	// Draws faces on dialogs
	void DrawDialogSpeaker();

	void SetFont(Font* font);

private:

	DialogNode* ParseDialogXML(pugi::xml_node dialogRoot);

	DialogNode* currentDialog = nullptr;

	std::map<std::string, DialogNode*> dialogues;

	std::map<std::string, DialogNode*> nodeRoutes;

	std::map<std::string, std::function<void()>> callbacks;

	int selectedOption = 0;

	std::string folder;
	pugi::xml_document dialogFile;

	// Needed modules
	Input* input;
	Render* render;
	AssetsManager* assetsManager;
	Fonts* fonts;
	Textures* tex;

	// Bool to search a random dialog
	bool newRandomDialog = false;
	bool newDialog = false;
	int dialogIndex;
	bool dialogFinished = false;

	SDL_Texture* dialogBackground;
	SDL_Rect backgroundRect;

private:

	// Texture for speakers
	SDL_Texture* speakerTexture;
	SDL_Texture* speakerTexture2;

	// Dialog speaker animations
	Animation* rightDialogAnim;
	Animation* leftDialogAnim;

	Animation erikaDialogAnim;
	Animation takadaDialogAnim;
	Animation mobileDialogAnim;

	Animation kenzoDialogAnim;
	Animation reiDialogAnim;
	Animation eikenDialogAnim;
	Animation makiDialogAnim;

	Animation oscarDialogAnim;
	Animation shopKeeperDialogAnim;
	Animation pedestrianrDialogAnim;
	Animation receptionistDialogAnim;

	Font* font;
};