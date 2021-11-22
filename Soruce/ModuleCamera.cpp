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
	CameraMovement();

	//Send the frustum view matrix to OpenGL
	// direct mode would be:
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);


	return UPDATE_CONTINUE;
}

void ModuleCamera::CameraMovement()
{
	static const float speed = 0.01f;
	uint32_t delta = App->GetDeltaTime();
	float new_pos = speed * delta;

	//ABSOLUTE UP -> Q
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(float3(0.0f, new_pos, 0.0f)));
	}
	//ABSOLUTE DOWN -> E
	if (App->input->GetKey(SDL_SCANCODE_E))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(float3(0.0f, -new_pos, 0.0f)));
	}
	//ABSOLUTE FRONT -> W
	if (App->input->GetKey(SDL_SCANCODE_W))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(float3(0.0f, 0.0f, new_pos)));
	}
	//ABSOLUTE BACK -> E
	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(float3(0.0f, 0.0f, -new_pos)));
	}

	//ABSOLUTE LEFT -> A
	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(float3(new_pos, 0.0f, 0.0f)));
	}

	//ABSOLUTE RIGHT -> D
	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(float3(-new_pos, 0.0f, 0.0f)));
	}
	//ROTATE LEFT
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		float3x3 rotationDeltaMatrix = float3x3(1.0f, 0.0f, 0.0f, // I HAVE NO IDEA HOW ROTATION MATRIX WORK
												0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 1.0f);

		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotationDeltaMatrix.MulDir(oldFront));
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotationDeltaMatrix.MulDir(oldUp));
		LOG("LEFT ARROW")
	}
}
