#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"

j1Input::j1Input() : j1Module()
{
	name.create("input");

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		mouse_buttons[i] = KS_IDLE;
	}
}

// Destructor
j1Input::~j1Input()
{}

// Called before render is available
bool j1Input::Awake()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	CleanKeys();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;
	CleanKeys();

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
			switch(event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
				case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				case SDL_WINDOWEVENT_MAXIMIZED:
				case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}

			break;

			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				int code = event.key.keysym.sym;
				j1KeyState state = KS_IDLE;

				if(event.key.repeat != 0)
				{
					state = KS_REPEAT;
				}
				else if(event.key.state == SDL_PRESSED)
				{
					state = KS_DOWN;
				}
				else
				{
					state = KS_UP;
				}

				if(code > 127)
				{
					code -= (127 + 1073741881); // https://wiki.libsdl.org/SDLKeycodeLookup
				}

				keyState[code] = state;
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KS_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KS_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
			int scale = App->win->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
void j1Input::CleanKeys()
{
	// memset would be faster!
	for(int i = 0; i < WE_COUNT; ++i)
	{
		windowEvents[i] = false;
	}

	for(int i = 0; i < NUM_KEYS; ++i)
	{
		keyState[i] = KS_IDLE;
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		(mouse_buttons[i] == KS_DOWN || mouse_buttons[i] == KS_REPEAT) ? mouse_buttons[i] = KS_REPEAT : mouse_buttons[i] = KS_IDLE;
	}

	mouse_motion_x = mouse_motion_y = 0;
}

bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

bool j1Input::GetKeyDown(int code)
{
	return keyState[code] == KS_DOWN;
}

bool j1Input::GetKeyRepeat(int code)
{
	return keyState[code] == KS_REPEAT;
}

bool j1Input::GetKeyUp(int code)
{
	return keyState[code] == KS_UP;
}

bool j1Input::GetMouseButtonDown(int code)
{
	return mouse_buttons[code - 1] == KS_DOWN;
}

bool j1Input::GetMouseButtonRepeat(int code)
{
	return mouse_buttons[code - 1] == KS_REPEAT;
}

bool j1Input::GetMouseButtonUp(int code)
{
	return mouse_buttons[code - 1] == KS_UP;
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}