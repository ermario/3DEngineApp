#include "Globals.h"
#include "ModuleRenderExercise.h"
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
	unsigned int program_id = App->shader_program->GetProgramId();
	glUseProgram(program_id);

	float4x4 model, view, projection;

	//MODEL
	model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),float4x4::RotateZ(pi / 4.0f),float3(2.0f, 1.0f, 0.0f));


	//view = LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);

	//PROJECTION
	Frustum frustum;
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3::zero);
	frustum.SetFront(-float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetHorizontalFovAndAspectRatio(90.0f, 1.0f);

	float4x4 proj = frustum.ProjectionMatrix();

	//VIEW
	view = float4x4(frustum.ViewMatrix()).Transposed();

	glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "proj"), 1, GL_TRUE, &projection[0][0]);
	
	// TODO: bind buffer and vertex attributes

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

