#include "Globals.h"
#include "ModuleCamera.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleInput.h"


ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(90.0f, 1.0f);
	frustum.SetPos(float3(0.0f, 1.0f, 0.0f));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);


	return true;
}

bool ModuleCamera::Clear()
{
	return true;
}

update_status ModuleCamera::Update()
{


	//ABSOLUTE UP
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(float3(0.0f, 1.0f, 0.0f)));
		LOG("\n Q PRESSED\n");
	}
	//ABSOLUTE DOWN
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(float3(0.0f, -1.0f, 0.0f)));
		LOG("\n E PRESSED\n");
	}

	//Send the frustum view matrix to OpenGL
	// direct mode would be:
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);


	return UPDATE_CONTINUE;
}
