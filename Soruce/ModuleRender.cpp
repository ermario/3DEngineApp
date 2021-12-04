#include "Globals.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"

#include "SDL.h"


ModuleRender::ModuleRender()
{
	
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	EngineLOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits

	context = SDL_GL_CreateContext(App->window->window); // create an OpenGL context for an OpenGL window, and make it current

	GLenum err = glewInit();
	if (GLEW_OK != err) //Checking for errors in glew init
	{
		EngineLOG("GLEW FAILED INITIALIZING: %s", glewGetErrorString(err));
	}
	else{ EngineLOG("Using Glew %s", glewGetString(GLEW_VERSION)); }

	//Detect current hardwar and dirver capabilities
	EngineLOG("Vendor: %s", glGetString(GL_VENDOR));
	EngineLOG("Renderer: %s", glGetString(GL_RENDERER));
	EngineLOG("OpenGL version supported %s", glGetString(GL_VERSION));
	EngineLOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	//INIT TEXTURES

	model = new Model();
	model->LoadModel("BakerHouse.fbx");

	return true;
}

update_status ModuleRender::PreUpdate()
{
	int width = NULL;
	int height = NULL;
	SDL_GetWindowSize(App->window->window, &width, &height); //update de width and height with the window size
	glViewport(0, 0, width, height); // set the view port

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // specify clear values for the color buffers - RGBAlpha
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers to preset values (from above)

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	glUseProgram(App->shader_program->GetProgramId());
	auto& size = App->window->screen_surface;
	App->grid->Draw(App->camera->GetViewMatrix(), App->camera->GetProjectionMatrix(), size->w, size->h);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model->Draw();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{

	SDL_GL_SwapWindow(App->window->window); // update a window with OpenGL rendering.
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	EngineLOG("Destroying renderer");
	SDL_GL_DeleteContext(context); //Destroy window
	
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}

