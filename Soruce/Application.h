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
class ModuleTimer;

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
	ModuleTimer* timer = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
