#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"

#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum j1KeyState
{
	KS_IDLE = 0,
	KS_DOWN,
	KS_REPEAT,
	KS_UP
};

class j1Input : public j1Module
{

public:

	j1Input();

	// Destructor
	virtual ~j1Input();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(j1EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	bool GetKeyDown(int code);
	bool GetKeyRepeat(int code);
	bool GetKeyUp(int code);

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	bool GetMouseButtonDown(int code);
	bool GetMouseButtonRepeat(int code);
	bool GetMouseButtonUp(int code);

private:
	void CleanKeys();

private:
	bool		windowEvents[WE_COUNT];
	j1KeyState	keyState[NUM_KEYS];
	j1KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;
};

#endif // __j1INPUT_H__