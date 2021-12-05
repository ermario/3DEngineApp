#include "Globals.h"

#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleInput.h"

#include "MathGeoLib.h"
#include "GL/glew.h"
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

	//INIT class variables for camera control and info
	camera_position = frustum.Pos();
	camera_pitch = frustum.Up();
	camera_yaw = frustum.Front();
	return true;
}

bool ModuleCamera::Clear()
{
	return true;
}

update_status ModuleCamera::Update()
{
	CameraMovement();
	SetCamera();

	//Send the frustum view matrix to OpenGL
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);

	return UPDATE_CONTINUE;
}

void ModuleCamera::CameraMovement()
{
	float movement_speed = 15.0f / 1000.f;
	float rotation_speed = 0.001f;
	double delta = App->GetDeltaTime();

	// PRESS SHIFT TO INCRESE SPEED OF MOVEMENT
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
	{
		movement_speed = movement_speed * 2;
		rotation_speed = rotation_speed * 2;
		EngineLOG("SHIFT %f and %f speed",movement_speed,rotation_speed);
	}

	float new_pos = movement_speed * delta;
	float new_angle = rotation_speed * delta;

	
	if (App->input->GetKey(SDL_SCANCODE_Q)) //ABSOLUTE UP -> Q
		camera_position += float3(0.0f, new_pos, 0.0f);
	if (App->input->GetKey(SDL_SCANCODE_E)) //ABSOLUTE DOWN -> E
		camera_position -= float3(0.0f, new_pos, 0.0f);
	if (App->input->GetKey(SDL_SCANCODE_W))	//ABSOLUTE FRONT -> W
		camera_position += frustum.Front() * new_pos;
	if (App->input->GetKey(SDL_SCANCODE_S)) //ABSOLUTE BACK -> S
		camera_position -= frustum.Front() * new_pos; 
	if (App->input->GetKey(SDL_SCANCODE_A))	//ABSOLUTE LEFT -> A
		camera_position -= frustum.WorldRight() * new_pos;
	if (App->input->GetKey(SDL_SCANCODE_D))	//ABSOLUTE RIGHT -> D
		camera_position += frustum.WorldRight() * new_pos;

	if (!camera_locked) {
		if (App->input->GetKey(SDL_SCANCODE_LEFT)) //ROTATE LEFT
			CameraRotation(new_angle, 0.0f, 0.0f);
		if (App->input->GetKey(SDL_SCANCODE_RIGHT)) //ROTATE RIGHT
			CameraRotation(-new_angle, 0.0f, 0.0f);
		if (App->input->GetKey(SDL_SCANCODE_UP)) //ROTATE UP
			CameraRotation(0.0f, new_angle, 0.0f);
		if (App->input->GetKey(SDL_SCANCODE_DOWN)) //ROTATE DOWN
			CameraRotation(0.0f, -new_angle, 0.0f);

		if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) 
		{
			frustum.SetPos(float3(0.0f, 10.0f, 0.0f));
			frustum.SetUp(float3(0.0f, 0.0f, 1.0f));
			frustum.SetFront(float3(0.0f, -1.0f, 0.0f));
		}
	}

}

void ModuleCamera::SetCamera()
{
	frustum.SetPos(camera_position);
	frustum.SetFront(camera_yaw);
	frustum.SetUp(camera_pitch);

}

void ModuleCamera::CameraRotation(float yaw, float pitch, float roll)
{
	if (yaw != 0.0f)
	{
		Quat rotation_vector = Quat::RotateY(yaw);
		camera_yaw = rotation_vector.Mul(frustum.Front()).Normalized();
		camera_pitch = rotation_vector.Mul(frustum.Up()).Normalized();
	}
	if(pitch != 0.0f)
	{
		Quat rotation_vector = Quat::RotateAxisAngle(frustum.WorldRight(), pitch);
		camera_yaw = rotation_vector.Mul(frustum.Front()).Normalized();
		camera_pitch = rotation_vector.Mul(frustum.Up()).Normalized();
	}

	if(roll != 0.0f)
	{
		//TODO
	}
}

void ModuleCamera::ImGuiCamera()
{
	ImGui::Separator;
	ImGui::Text("Camera Position");
	ImGui::SliderFloat3("pX, pY, pZ", &camera_position[0], -10.0f, 10.0f, "%.3f", 1.0f);
	ImGui::Separator;
	ImGui::Text("Camera Direction");
	ImGui::InputFloat3("dX, dY, dZ", &camera_pitch[0]);
	ImGui::Separator;
	ImGui::Text("Camera Inclination");
	ImGui::InputFloat3("iX, iY, iZ", &camera_yaw[0]);
	ImGui::Checkbox("Lock Model", &camera_locked);
	if (camera_locked) {
		ImGui::SameLine();
		ImGui::Text("(Rotation disabled)");
	}
}
