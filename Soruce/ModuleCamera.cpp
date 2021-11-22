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
	frustum.SetPos(float3(0.0f, 5.0f, -5.0f));
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
	float movement_speed = 0.01f;
	float rotation_speed = 0.001f;
	uint32_t delta = App->GetDeltaTime();

	// PRESS SHIFT TO INCRESE SPEED OF MOVEMENT
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
	{
		movement_speed = movement_speed * 2;
		rotation_speed = rotation_speed * 2;
		LOG("SHIFT %f and %f speed",movement_speed,rotation_speed);
	}

	float new_pos = movement_speed * delta;
	float new_angle = rotation_speed * delta;

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
		vec oldPos = frustum.Pos().Normalized();
		float3x3 moving_vector = float3x3::LookAt(frustum.Front(), frustum.Pos(), frustum.Up(), float3::unitY);
		//frustum.SetPos(oldPos.Add(float3(0.0f, 0.0f, new_pos)));
		frustum.SetPos();
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
	if (App->input->GetKey(SDL_SCANCODE_LEFT))
	{

		float3x3 rotation_vector = float3x3::RotateAxisAngle(float3::unitY, new_angle);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotation_vector.MulDir(oldFront));
	}

	//ROTATE RIGHT
	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
	{
		float3x3 rotation_vector = float3x3::RotateAxisAngle(float3::unitY, -new_angle);
		vec oldFront = frustum.Front().Normalized();
		frustum.SetFront(rotation_vector.MulDir(oldFront));
	}

	//ROTATE UP
	if (App->input->GetKey(SDL_SCANCODE_UP))
	{

		float3x3 rotation_vector = float3x3::RotateAxisAngle(float3::unitX, -new_angle);
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotation_vector.MulDir(oldUp));

	}

	//ROTATE DOWN
	if (App->input->GetKey(SDL_SCANCODE_DOWN))
	{

		float3x3 rotation_vector = float3x3::RotateAxisAngle(float3::unitX, new_angle);
		vec oldUp = frustum.Up().Normalized();
		frustum.SetUp(rotation_vector.MulDir(oldUp));

	}
}
