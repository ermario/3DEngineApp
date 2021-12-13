#include "ModuleTimer.h"
#include "SDL.h"

bool ModuleTimer::Init()
{

    return true;
}

update_status ModuleTimer::Update()
{
	uint32_t current_tick = SDL_GetTicks();
	delta_time = (current_tick - ticks_count);
	ticks_count = current_tick;
	return update_status();
}
