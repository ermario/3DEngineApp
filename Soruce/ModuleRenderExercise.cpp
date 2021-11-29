#include "Globals.h"
#include "ModuleRenderExercise.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"

float cube_vertices[] = {
    -1.0f, 1.0f, 0.0f, // triángulo 1 : comienza
     1.0f,-1.0f, 0.0f,
     1.0f,-1.0f, 0.0f, 
    -1.0f, 1.0f, 0.0f, // triángulo 2 : comienza
     1.0f,-1.0f, 0.0f,
    -1.0f,-1.0f, 0.0f 
};
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triángulo 1 : comienza
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triángulo 1 : termina
    1.0f, 1.0f,-1.0f, // triángulo 2 : comienza
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triángulo 2 : termina
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};
ModuleRenderExercise::ModuleRenderExercise()
{

}

bool ModuleRenderExercise::Init()
{


	return true;
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteBuffers(1, &vertex_buffer_object);

	return true;
}


update_status ModuleRenderExercise::Update()
{

	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	unsigned int program_id = App->shader_program->GetProgramId();
	glUseProgram(program_id);

	float4x4 model, view, projection;

	//MODEL
	//model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),float4x4::RotateZ(pi / 4.0f),float3(2.0f, 1.0f, 0.0f));
	model = float4x4::identity;
	glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "view"), 1, GL_TRUE, &App->camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "proj"), 1, GL_TRUE, &App->camera->GetProjectionMatrix()[0][0]);

	// TODO: bind buffer and vertex attributes
	glBindVertexArray(vertex_buffer_object);
	// 1 triangle to draw = 3 vertices 
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	
    return UPDATE_CONTINUE;
}

ModuleRenderExercise::~ModuleRenderExercise()
{
	
}

