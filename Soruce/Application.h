#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleCamera;
class ModuleDebugDraw;
class ModuleImGui;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* shader_program = nullptr;
	ModuleRenderExercise* render_exercise = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleDebugDraw* grid = nullptr;
	ModuleImGui* imgui = nullptr;
	inline uint32_t GetDeltaTime() { return delta_time; }
private:

	std::list<Module*> modules;
	uint32_t delta_time = 0;
	uint32_t ticks_count = 0;

};

extern Application* App;
