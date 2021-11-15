#include "ModuleCamera.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "GL/glew.h"

bool ModuleCamera::Init()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(90.0f, 1.3f);
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

	return UPDATE_CONTINUE;
}
