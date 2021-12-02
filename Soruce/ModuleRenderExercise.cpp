#include "Globals.h"
#include "ModuleRenderExercise.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"


float cube_vertices[] = {
     1.0f, 1.0f, 0.0f, // Triangle 1
     1.0f,-1.0f, 0.0f,
    -1.0f,-1.0f, 0.0f,
    -1.0f,-1.0f, 0.0f, // Triangle 2
    -1.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 0.0f,
     1.0f, 1.0f,       // Texture T1
     1.0f, 0.0f,
     0.0f, 0.0f,
     0.0f, 0.0f,       // Texture T2
     0.0f, 1.0f,
     1.0f, 1.0f
};

float vertices[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f
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

    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    // size = 3 float per vertex
    // stride = 0 is equivalent to stride = sizeof(float)*3
    glEnableVertexAttribArray(0); // Shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1); // Textures
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 6)); //3 * 6 is the size of the vertices, buffer offset 

    glActiveTexture(GL_TEXTURE0);


	return true;
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteBuffers(1, &vertex_buffer_object);

	return true;
}


update_status ModuleRenderExercise::Update()
{
    float4x4 model(float4(5.0f, 0.0f, 0.0f, 0.0f), // MODEL SIZED x5
                   float4(0.0f, 5.0f, 0.0f, 0.0f), 
                   float4(0.0f, 0.0f, 5.0f, 0.0f), 
                   float4(0.0f, 0.0f, 0.0f, 1.0f));
    unsigned int program_id = App->shader_program->GetProgramId();
    glUseProgram(program_id);

    //MODEL
    //float4x4 model = float4x4::identity;
	glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "view"), 1, GL_TRUE, &App->camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "proj"), 1, GL_TRUE, &App->camera->GetProjectionMatrix()[0][0]);

    
    //TEXTURE
    Texture txtur = App->textures->CompileTexture("Textures/lena.jpg");
    glBindTexture(GL_TEXTURE_2D, App->textures->GetTexture(&txtur));
    glUniform1i(glGetUniformLocation(App->shader_program->GetProgramId(), "mytexture"), 0);

    
	// TODO: bind buffer and vertex attributes
	glBindVertexArray(vertex_buffer_object);

	// 1 triangle to draw = 3 vertices 
	glDrawArrays(GL_TRIANGLES, 0, 6*3*2);
	
    return UPDATE_CONTINUE;
}

ModuleRenderExercise::~ModuleRenderExercise()
{
	
}

