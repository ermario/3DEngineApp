#include "ModuleTimer.h"

#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


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
	UpdateRealDeltaTimer();
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

void ModuleTimer::UpdateRealDeltaTimer()
{
	uint32_t current_tick = SDL_GetTicks();
	real_delta_time = (current_tick - real_ticks_count);
	real_ticks_count = current_tick;
}

void ModuleTimer::ImguiTimer()
{
	ImGui::GetTime();
}

