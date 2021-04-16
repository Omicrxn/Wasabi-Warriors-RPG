#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50
#define NUM_CONTROLLER_BUTTONS 15

class Window;

struct SDL_Rect;
struct _SDL_GameController;
struct _SDL_Haptic;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum ControllerButton
{
	CONTROLLER_BUTTON_A,
	CONTROLLER_BUTTON_B,
	CONTROLLER_BUTTON_X,
	CONTROLLER_BUTTON_Y,
	CONTROLLER_BUTTON_UP,
	CONTROLLER_BUTTON_DOWN,
	CONTROLLER_BUTTON_LEFT,
	CONTROLLER_BUTTON_RIGHT,
	CONTROLLER_BUTTON_LB,
	CONTROLLER_BUTTON_RB,
	CONTROLLER_BUTTON_L3,
	CONTROLLER_BUTTON_R3,
	CONTROLLER_BUTTON_START,
	CONTROLLER_BUTTON_GUIDE,
	CONTROLLER_BUTTON_BACK
};

struct Controller
{
	// Input data
	float LT, RT;
	float leftX, leftY, rightX, rightY, leftDeadZone, rightDeadZone;

	// Controller data
	bool enabled;
	int index;
	_SDL_GameController* sdlController;
	_SDL_Haptic* haptic;

	// Rumble controller
	int rumbleCountdown;
	float rumbleStrength;
};

enum AxisName
{
	VERTICAL,
	HORIZONTAL
};

class Input : public Module
{
public:
	
	Input(Window* win);

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// --- KEYBOARD METHODS ---
	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	// --- CONTROLLER METHODS ---
	// Check controller button states
	KeyState GetControllerButton(int id) const
	{
		return controllerButtons[id];
	}

	bool GetControllerState() const
	{
		return controller.enabled;
	}

	bool GetMouseMotion() const
	{
		return mouseMotion;
	}

	bool ShakeController(int id, int duration, float dt, float strength = 0.5f);
	const char* GetControllerName(int id) const;

	// --- MOUSE METHODS ---
	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Get mouse / axis position
	void GetMousePosition(int& x, int& y);
	void GetMouseMotion(int& x, int& y);

	int GetAxisRaw(AxisName axisName);

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

private:

	// --- GAMEPAD ---
	// Activates SDL device funcionallity when a gamepad has been connected
	void HandleDeviceConnection(int index);

	// Deactivates SDL device funcionallity when a gamepad has been disconnected
	void HandleDeviceRemoval(int index);

	// Called at PreUpdate
	// Iterates through all active gamepads and update all input data
	bool* UpdateControllerInput();

private:

	Window* win;

	bool windowEvents[WE_COUNT];

	KeyState* keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];

	Controller controller;
	KeyState* controllerButtons;

	bool mouseMotion;
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
};

class GameControl
{
public:
	bool toChange = false;
	KeyState currentState = KeyState::KEY_IDLE;

public:
	void Update()
	{
		// if (CheckAssignedKey() || CheckAssignedButton())
		// change state
	}
	void ChangeAssignation();
	KeyState CheckAssignedKey();
	KeyState CheckAssignedButton()
	{
		// input->pads[0].up?
	}
};

//enum state {
//	UP, DOWN, REPEAT
//};
//enum assignedKey {
//	A,B,C,D,E,F,G
//};
//enum assignedButton {
//	RB,LB,A,X,Y,B
//};
//class GameControl
//{
//public:
//	bool toChange = false;
//	state currentState = UP;
//
//public:
//	void Update()
//	{
//		// if (CheckAssignedKey() || CheckAssignedButton())
//		// change state
//	}
//	void ChangeAssignation();
//	bool CheckAssignedKey()
//	{
//		// input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN
//	}
//	bool CheckAssignedButton()
//	{
//		// input->pads[0].up?
//	}
//};
//
//int main()
//{
//	GameControl* interact;
//
//	interact->Update();
//	//if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || ) return false;
//	if (interact->currentState == state::DOWN)
//		return false;
//}

#endif // __INPUT_H__