#pragma once

#include "Module.h"
#include "SDL.h"


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

struct AboutInfo {

	unsigned char* system;
	int cpu;
	float ram;
	unsigned char* gpu;
	unsigned char* gpu_vendor;
	int vram_capacity;
	int vram_free;
	SDL_version sdl_version;
};

class ModuleImGui : public Module
{
public: 
	ModuleImGui();
	~ModuleImGui();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	bool CleanUp() override;
	//Console* debugger = NULL;

	void Menu();
	void InspectorSidebar();
	void Performance();
	void About();
	bool ExitPopup();

private:
	bool demo_tab = true;
	bool console_tab = false;
	bool about_tab = false;
	bool inspector_tab = true;
	bool exit_popup = false;
	AboutInfo about;
};

