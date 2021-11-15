#include "ModuleRenderExercise.h"
#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"


float vertices[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f
};

ModuleRenderExercise::ModuleRenderExercise()
{

}

bool ModuleRenderExercise::Init()
{
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glUseProgram(App->shader_program->GetProgramId());



	return true;
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteBuffers(1, &vertex_buffer_object);

	return true;
}


update_status ModuleRenderExercise::Update()
{
	// 1 triangle to draw = 3 vertices 
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
    return UPDATE_CONTINUE;
}

ModuleRenderExercise::~ModuleRenderExercise()
{
	
}

