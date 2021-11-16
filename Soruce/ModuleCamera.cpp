#include "ModuleCamera.h"
#include "Globals.h"
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
	frustum.SetPos(float3(0.0f, 1.0f, -2.0f));
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
	frustum.SetPos(float3(0.0f, 10.0f, 0.0f));
	float3x3 rotationMatrix = float3x3::identity; // = some rotation value (or LookAt matrix)
	frustum.SetFront(rotationMatrix.WorldX());
	frustum.SetUp(rotationMatrix.WorldY());
	//Send the frustum view matrix to OpenGL
	// direct mode would be:
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);

	if(App->input->)
	return UPDATE_CONTINUE;
}
