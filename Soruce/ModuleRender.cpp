#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"


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
	LOG("Creating Renderer context");

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
		LOG("GLEW FAILED INITIALIZING: %s", glewGetErrorString(err));
	}
	else{ LOG("Using Glew %s", glewGetString(GLEW_VERSION)); }

	//Detect current hardwar and dirver capabilities
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	return true;
}

update_status ModuleRender::PreUpdate()
{
	int width = NULL;
	int height = NULL;
	unsigned program_id = 0;
	SDL_GetWindowSize(App->window->window, &width, &height); //update de width and height with the window size
	glViewport(0, 0, width, height); // set the view port

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // specify clear values for the color buffers - RGBAlpha
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers to preset values (from above)
	//program_id = App->shader_program->GetProgramId();
	//glUseProgram(program_id);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	/* REMOVE FROM HERE*/

	// GROUND GRID
	

	/*
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
	*/
	// 3D AXES PRINTING


	/* TO HERE*/

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	App->grid->Draw(App->camera->GetViewMatrix(), App->camera->GetProjectionMatrix(),10,10);
	SDL_GL_SwapWindow(App->window->window); // update a window with OpenGL rendering.
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	SDL_GL_DeleteContext(context); //Destroy window
	
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}

