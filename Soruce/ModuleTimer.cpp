#include "ModuleTimer.h"
#include "SDL.h"

ModuleTimer::ModuleTimer()
{
}

ModuleTimer::~ModuleTimer()
{
}

bool ModuleTimer::Init()
{

    return true;
}

update_status ModuleTimer::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimer::Update()
{
	UpdateDeltaTimer();
	return UPDATE_CONTINUE;
}

update_status ModuleTimer::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTimer::CleanUp()
{
	return false;
}

void ModuleTimer::UpdateDeltaTimer()
{
	uint32_t current_tick = SDL_GetTicks();
	delta_time = (current_tick - ticks_count);
	ticks_count = current_tick;
}
