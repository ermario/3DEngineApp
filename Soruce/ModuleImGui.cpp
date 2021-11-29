#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "SDL.h"


#include "GL/glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"


ModuleImGui::ModuleImGui()
{
}

ModuleImGui::~ModuleImGui()
{
}

// Called before render is available
bool ModuleImGui::Init()
{
	EngineLOG("Created context for ImGui")

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();

	about.system = (unsigned char*)SDL_GetPlatform();

	about.cpu = SDL_GetCPUCount();
	about.ram = SDL_GetSystemRAM() / 1024.0f;

	about.gpu = (unsigned char*)glGetString(GL_RENDERER);
	about.gpu_vendor = (unsigned char*)glGetString(GL_VENDOR);
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &about.vram_capacity);
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &about.vram_free);
	SDL_GetVersion(&about.sdl_version);
	//demo_tab = true;
	return true;
}

update_status ModuleImGui::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	Menu();

	//if (console_tab) 
		//Logger->Draw(&show_console);

	if (demo_tab)
		ImGui::ShowDemoWindow(&demo_tab);

	if (about_tab)
		ImGui::ShowAboutWindow(&about_tab);

	if (inspector_tab)
		InspectorSidebar();


	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleImGui::Update()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleImGui::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleImGui::Menu() {


	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Load");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Do")) {}
			if (ImGui::MenuItem("Undo")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) {
			ImGui::MenuItem("Inspector", NULL, &inspector_tab);
			ImGui::MenuItem("Console", NULL, &console_tab);
			ImGui::MenuItem("Imgui Demo", NULL, &demo_tab);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Build")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {

			if (ImGui::MenuItem("GitHub")) {
				BROWSER("https://github.com/ermario");
			}
	
			ImGui::MenuItem("About ImGui", NULL, &about_tab);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ModuleImGui::InspectorSidebar() {

	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Inspector", NULL, window_flags);

	if (ImGui::CollapsingHeader("Camera"))
		App->camera->ImGuiCamera();

	if (ImGui::CollapsingHeader("About system")) 
		About();

	ImGui::End();

}

void ModuleImGui::Performance() {

}

void ModuleImGui::About() {
	static SDL_version version;
	static const float vram_total = about.vram_capacity / 1024.0f;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &about.vram_free);
	float vram_free = about.vram_free / 1024.0f;
	float vram_usage = vram_total - vram_free;

	ImGui::Separator();
	ImGui::Text("System: %s", about.system);
	ImGui::Text("SDL Version: %d.%d.%d", about.sdl_version.major,
		about.sdl_version.minor, about.sdl_version.patch);
	ImGui::Separator();
	ImGui::Text("CPUs: %d", about.cpu);
	ImGui::Text("System RAM: %.1f Gb", about.ram);
	ImGui::Separator();
	ImGui::Text("GPU: %s", about.gpu);
	ImGui::Text("Vendor: %s", about.gpu_vendor);
	ImGui::Text("VRAM: %.1f Mb", vram_total);
	ImGui::Text("Vram Usage:  %.1f Mb", vram_usage);
	ImGui::Text("Vram Available:  %.1f Mb", vram_free);
}
