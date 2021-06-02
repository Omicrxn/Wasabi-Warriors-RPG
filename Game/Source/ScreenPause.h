#include "Screen.h"
#include "Animation.h"
#include "Font.h"

class QuestManager;
enum MobileState
{
	MAIN, QUEST, TEAM, MAP
};

class ScreenPause : public Screen
{
public:
	ScreenPause();
	~ScreenPause();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	void Enable(bool isFromSettings = false);
	void Disable();

	void SetMenuFont(Font*);
	void SetQuestManager(QuestManager*);

	int position;

	GuiIcon* iconResume;
	GuiIcon* iconSettings;
	GuiIcon* iconExit;

	GuiIcon* iconQuest;
	GuiIcon* iconTeam;
	GuiIcon* iconMap;

	SDL_Rect pauseTitleRect;
	SDL_Rect mobileRect;

	SDL_Rect pinkBox;
	SDL_Rect pinkCircle;
	SDL_Rect grayCircle;

	Animation* currentAnimation;
	Animation kenzoAnim;
	Animation reiAnim;
	Animation eikenAnim;

	Font* menuFont;

	MobileState state = MobileState::MAIN;

	QuestManager* questManager;
};