#include "Globals.h"
#include "ModuleCamera.h"
#include "MathGeoLib.h"
#include "GL/glew.h"
#include "Application.h"
#include "ModuleInput.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, float(SCREEN_WIDTH)/float(SCREEN_HEIGHT));
	frustum.SetPos(float3(0.0f, 5.0f, -5.0f));
	float3x3 world_rotation = float3x3::identity;
	frustum.SetFront(world_rotation.WorldZ());
	frustum.SetUp(world_rotation.WorldY());
	camera_position = frustum.Pos();
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
		EngineLOG("SHIFT %f and %f speed",movement_speed,rotation_speed);
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
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(frustum.Front()*new_pos));
	}
	//ABSOLUTE BACK -> S
	if (App->input->GetKey(SDL_SCANCODE_S))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Sub(frustum.Front() * new_pos));
	}

	//ABSOLUTE LEFT -> A
	if (App->input->GetKey(SDL_SCANCODE_A))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Sub(frustum.WorldRight() * new_pos));
	}

	//ABSOLUTE RIGHT -> D
	if (App->input->GetKey(SDL_SCANCODE_D))
	{
		vec oldPos = frustum.Pos();
		frustum.SetPos(oldPos.Add(frustum.WorldRight() * new_pos));
	}
	if (!camera_locked) {
		//ROTATE LEFT
		if (App->input->GetKey(SDL_SCANCODE_LEFT))
		{

			Quat rot = Quat::RotateY(new_angle);
			frustum.SetFront(rot.Mul(frustum.Front()).Normalized());
			frustum.SetUp(rot.Mul(frustum.Up()).Normalized());
		}

		//ROTATE RIGHT
		if (App->input->GetKey(SDL_SCANCODE_RIGHT))
		{
			Quat rotation_vector = Quat::RotateY(-new_angle);
			frustum.SetFront(rotation_vector.Mul(frustum.Front()).Normalized());
			frustum.SetUp(rotation_vector.Mul(frustum.Up()).Normalized());
		}

		//ROTATE UP
		if (App->input->GetKey(SDL_SCANCODE_UP))
		{
			Quat rotation_vector = Quat::RotateAxisAngle(frustum.WorldRight(), new_angle);
			frustum.SetUp(rotation_vector.Mul(frustum.Up()).Normalized());
			frustum.SetFront(rotation_vector.Mul(frustum.Front()).Normalized());
		}

		//ROTATE DOWN
		if (App->input->GetKey(SDL_SCANCODE_DOWN))
		{
			float3x3 rotation_vector = float3x3::RotateAxisAngle(frustum.WorldRight(), -new_angle);
			vec oldUp = frustum.Up().Normalized();
			vec oldFront = frustum.Front().Normalized();
			frustum.SetUp(rotation_vector.MulDir(oldUp));
			frustum.SetFront(rotation_vector.MulDir(oldFront));

		}
		if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) 
		{
			frustum.SetPos(float3(0.0f, 10.0f, 0.0f));
			frustum.SetUp(float3(0.0f, 0.0f, 1.0f));
			frustum.SetFront(float3(0.0f, -1.0f, 0.0f));
		}
	}
	
}

void ModuleCamera::ImGuiCamera()
{
	camera_position = frustum.Pos();
	float3 camera_direction = frustum.Front();
	float3 camera_inclination = frustum.Up();
	ImGui::Separator;
	ImGui::Text("Camera Position");
	ImGui::SliderFloat3("pX, pY, pZ", &camera_position[0], -10.0f, 10.0f, "%.3f", 1.0f);
	ImGui::Separator;
	ImGui::Text("Camera Direction");
	ImGui::SliderFloat3("dX, dY, dZ", &camera_direction[0], -10.0f, 10.0f, "%.3f", 1.0f);
	ImGui::Separator;
	ImGui::Text("Camera Inclination");
	ImGui::SliderFloat3("iX, iY, iZ", &camera_inclination[0], -10.0f, 10.0f, "%.3f", 1.0f);


	ImGui::Checkbox("Lock Model", &camera_locked);
	if (camera_locked) {
		ImGui::SameLine();
		ImGui::Text("(Rotation disabled)");
	}
}
