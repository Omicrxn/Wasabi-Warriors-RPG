#include "Input.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

Input::Input(Window* win) : Module()
{
	name.Create("input");

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	controllerButtons = new KeyState[NUM_CONTROLLER_BUTTONS];
	memset(controllerButtons, KEY_IDLE, sizeof(KeyState) * NUM_CONTROLLER_BUTTONS);

	controller.haptic = nullptr;
	controller.sdlController = nullptr;
	controller.enabled = false;
	controller.index = 0;

	this->win = win;
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
	delete[] controllerButtons;
	delete[] mouseButtons;
}

// Called before render is available
bool Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
		LOG("SDL_INIT_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0) {
		LOG("SDL_INIT_HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	SDL_ShowCursor(0);

	return ret;
}

// Called before the first frame
bool Input::Start()
{
	SDL_StopTextInput();

	return true;
}

// Called each loop iteration
bool Input::PreUpdate()
{
	static SDL_Event event;

	// Keyboard input
	const Uint8* prevFrameKeys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (prevFrameKeys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	// Controller input
	const uint8_t* prevFrameButtons = UpdateControllerInput();

	for (int i = 0; i < NUM_CONTROLLER_BUTTONS; ++i)
	{
		if (prevFrameButtons[i] == 1)
		{
			if (controllerButtons[i] == KEY_IDLE)
				controllerButtons[i] = KEY_DOWN;
			else
				controllerButtons[i] = KEY_REPEAT;
		}
		else
		{
			if (controllerButtons[i] == KEY_REPEAT || controllerButtons[i] == KEY_DOWN)
				controllerButtons[i] = KEY_UP;
			else
				controllerButtons[i] = KEY_IDLE;
		}
	}

	// Mouse buttons input
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if (mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
				//SDL_MinimizeWindow(win->window);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;
				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
				//SDL_MaximizeWindow(win->window);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				//SDL_RestoreWindow(win->window);
				break;
			}
			break;

		case SDL_CONTROLLERDEVICEADDED:
			HandleDeviceConnection(event.cdevice.which);
			break;

		case SDL_CONTROLLERDEVICEREMOVED:
			HandleDeviceRemoval(event.cdevice.which);
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_MOUSEMOTION:
			int scale = win->GetScale();
			mouseMotionX = event.motion.xrel / scale;
			mouseMotionY = event.motion.yrel / scale;
			mouseX = event.motion.x / scale;
			mouseY = event.motion.y / scale;
			//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}

	return true;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");

	// Stop rumble from the gamepad and deactivate SDL functionallity
	if (controller.haptic != nullptr)
	{
		SDL_HapticStopAll(controller.haptic);
		SDL_HapticClose(controller.haptic);
	}
	if (controller.sdlController != nullptr && controller.enabled == true)
		SDL_GameControllerClose(controller.sdlController);

	SDL_QuitSubSystem(SDL_INIT_HAPTIC);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

bool Input::ShakeController(int id, int duration, float dt, float strength)
{
	bool ret = false;

	if (!controller.enabled || controller.haptic == nullptr || SDL_HapticRumbleSupported(controller.haptic) != SDL_TRUE) return ret;

	// If the pad is already in rumble state and the new strength is below the current value, ignore this call
	if (duration < controller.rumbleCountdown && strength < controller.rumbleStrength)
		return ret;

	if (SDL_HapticRumbleInit(controller.haptic) == -1)
	{
		LOG("Cannot init rumble for controller number %d", id);
	}
	else
	{
		SDL_HapticRumbleStop(controller.haptic);
		SDL_HapticRumblePlay(controller.haptic, strength, duration / dt);

		controller.rumbleCountdown = duration;
		controller.rumbleStrength = strength;

		ret = true;
	}

	return ret;
}

const char* Input::GetControllerName(int id) const
{
	// Check if the given id has a valid controller
	if (controller.enabled == true && controller.sdlController != nullptr)
		return SDL_GameControllerName(controller.sdlController);

	return "unplugged";
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}

int Input::GetAxisRaw(AxisName axisName)
{
	int axis = 0;

	switch (axisName)
	{
	case VERTICAL:
		if (keyboard[SDL_SCANCODE_W] == KEY_REPEAT || keyboard[SDL_SCANCODE_UP] == KEY_REPEAT || controllerButtons[CONTROLLER_BUTTON_UP] == KEY_REPEAT)
			axis = -1;
		else if (keyboard[SDL_SCANCODE_S] == KEY_REPEAT || keyboard[SDL_SCANCODE_DOWN] == KEY_REPEAT || controllerButtons[CONTROLLER_BUTTON_DOWN] == KEY_REPEAT)
			axis = 1;
		break;
	case HORIZONTAL:
		if (keyboard[SDL_SCANCODE_A] == KEY_REPEAT || keyboard[SDL_SCANCODE_LEFT] == KEY_REPEAT || controllerButtons[CONTROLLER_BUTTON_LEFT] == KEY_REPEAT)
			axis = -1;
		else if (keyboard[SDL_SCANCODE_D] == KEY_REPEAT || keyboard[SDL_SCANCODE_RIGHT] == KEY_REPEAT || controllerButtons[CONTROLLER_BUTTON_RIGHT] == KEY_REPEAT)
			axis = 1;
		break;
	default:
		axis = 0;
		break;
	}

	return axis;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::HandleDeviceConnection(int index) 
{
	if (SDL_IsGameController(index)) 
	{
		if (controller.enabled == false)
		{
			if (controller.sdlController = SDL_GameControllerOpen(index))
			{
				LOG("Found a gamepad with named %s", SDL_GameControllerName(controller.sdlController));
				controller.enabled = true;
				controller.leftDeadZone = controller.rightDeadZone = 0.1f;
				controller.haptic = SDL_HapticOpen(index);

				if (controller.haptic != nullptr)
					LOG("... gamepad has force feedback capabilities");

				controller.index = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller.sdlController));
			}
		}
	}
}

void Input::HandleDeviceRemoval(int index) 
{
	// Deactivate all SDL device functionallity
	if (controller.enabled && controller.index == index)
	{
		controller.enabled = false;
		SDL_HapticClose(controller.haptic);
		SDL_GameControllerClose(controller.sdlController);
		memset(&controller, 0, sizeof(Controller));
	}
}

uint8_t* Input::UpdateControllerInput() 
{
	static uint8_t buttons[NUM_CONTROLLER_BUTTONS] = {};

	// Update all input data
	if (controller.enabled == true)
	{
		buttons[CONTROLLER_BUTTON_A] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_A) == 1;
		buttons[CONTROLLER_BUTTON_B] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_B) == 1;
		buttons[CONTROLLER_BUTTON_X] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_X) == 1;
		buttons[CONTROLLER_BUTTON_Y] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_Y) == 1;
		buttons[CONTROLLER_BUTTON_UP] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1;
		buttons[CONTROLLER_BUTTON_DOWN] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1;
		buttons[CONTROLLER_BUTTON_LEFT] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1;
		buttons[CONTROLLER_BUTTON_RIGHT] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1;
		buttons[CONTROLLER_BUTTON_LB] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1;
		buttons[CONTROLLER_BUTTON_RB] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1;
		buttons[CONTROLLER_BUTTON_L3] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1;
		buttons[CONTROLLER_BUTTON_R3] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1;
		
		buttons[CONTROLLER_BUTTON_START] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_START) == 1;
		buttons[CONTROLLER_BUTTON_GUIDE] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_GUIDE) == 1;
		buttons[CONTROLLER_BUTTON_BACK] = SDL_GameControllerGetButton(controller.sdlController, SDL_CONTROLLER_BUTTON_BACK) == 1;

		if (controller.rumbleCountdown > 0)
			controller.rumbleCountdown--;

		// --- Other buttons that are not in used for the game (provisional) ---
		controller.LT = float(SDL_GameControllerGetAxis(controller.sdlController, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) / 32767.0f;
		controller.RT = float(SDL_GameControllerGetAxis(controller.sdlController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / 32767.0f;

		controller.leftX = float(SDL_GameControllerGetAxis(controller.sdlController, SDL_CONTROLLER_AXIS_LEFTX)) / 32767.0f;
		controller.leftY = float(SDL_GameControllerGetAxis(controller.sdlController, SDL_CONTROLLER_AXIS_LEFTY)) / 32767.0f;
		controller.rightX = float(SDL_GameControllerGetAxis(controller.sdlController, SDL_CONTROLLER_AXIS_RIGHTX)) / 32767.0f;
		controller.rightY = float(SDL_GameControllerGetAxis(controller.sdlController, SDL_CONTROLLER_AXIS_RIGHTY)) / 32767.0f;

		// Apply deadzone. All values below the deadzone will be discarded
		controller.leftX = (fabsf(controller.leftX) > controller.leftDeadZone) ? controller.leftX : 0.0f;
		controller.leftY = (fabsf(controller.leftY) > controller.leftDeadZone) ? controller.leftY : 0.0f;
		controller.rightX = (fabsf(controller.rightX) > controller.rightDeadZone) ? controller.rightX : 0.0f;
		controller.rightY = (fabsf(controller.rightY) > controller.rightDeadZone) ? controller.rightY : 0.0f;
	}

	return buttons;
}