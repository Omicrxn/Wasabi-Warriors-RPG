#include "Screen.h"
#include "Animation.h"
#include "Font.h"
#include "Notifier.h"

class QuestManager;

enum MobileState
{
	MAIN,
	QUEST,
	TEAM,
	MAP
};

class ScreenPause : public Screen
{
public:
	ScreenPause();
	~ScreenPause();

	bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx);

	bool Update(Input* input, float dt, uint& focusedButtonId);
	bool Draw(Render* render);
	bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

	void Enable(bool isFromSettings = false);
	void Disable();

	void SetQuestManager(QuestManager*);

	void SetMap(MapType map);

	bool PointCircleCollCheck(float px, float py, float cx, float cy, float r);

	int position;
	int mobRelativePos;

	GuiIcon* iconReturn;
	GuiIcon* iconSettings;
	GuiIcon* iconExit;

	GuiIcon* iconQuest;
	GuiIcon* iconTeam;
	GuiIcon* iconMap;

	SDL_Rect mobileRect;
	SDL_Rect redDot;

	MapType currentMap;
	SDL_Rect mapRect;
	iPoint mapHoverPos[5] = {};
	bool isMapHover[5];
	int hoverFx = -1;
	int hovering = 0;

	Animation* currentAnimation;
	Animation kenzoAnim;
	Animation reiAnim;
	Animation eikenAnim;
	Animation makiAnim;

	MobileState state = MobileState::MAIN;

	QuestManager* questManager;
};