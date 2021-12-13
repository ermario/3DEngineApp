#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleInput;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleCamera;
class ModuleDebugDraw;
class ModuleImGui;
class ModuleTexture;

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
	ModuleTexture* textures = nullptr;

	//inline double GetDeltaTime() { return delta_time; }
private:

	std::list<Module*> modules;
	//double delta_time = 0;
	//double ticks_count = 0;

};

extern Application* App;
