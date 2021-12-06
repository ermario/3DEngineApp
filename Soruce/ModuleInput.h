#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"

#include "SDL/include/SDL_scancode.h"

#define NUM_MOUSE_BUTTONS 5

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

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	void GetMouseMovement(int& mov_x, int& mov_y) const;
	int GetWheelMovement() const { return scroll_mov; }
	void ResetMouseValues() { mouse_mov_x = mouse_mov_y = scroll_mov = 0.0f; }

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;

	bool is_escape = false;
	bool drag_model = false; // TODO: POP UP FOR


private:
	bool windowEvents[WE_COUNT];
	KeyState* keyboard;
	KeyState mouse_buttons[NUM_MOUSE_BUTTONS];
	int mouse_mov_x = 0;
	int mouse_mov_y = 0;
	int scroll_mov = 0;
};

#endif // __MODULEINPUT_H__