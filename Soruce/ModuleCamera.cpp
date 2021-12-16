#include "Globals.h"

#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTimer.h"

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
	ComputeAspectRatio(float(App->window->screen_surface->w) , float(App->window->screen_surface->h));
	ComputeHorizontalFov(DEFAULT_FOV);
	near_plane_distance = DEFAULT_NEAR_DIST; // TODO: Make it funciton?
	far_plane_distance = DEFAULT_FAR_DIST;
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	UpdatePlaneDistances();
	UpdateFovAndAspectRatio();
	frustum.SetPos(float3(0.0f, 5.0f, -10.0f));
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
	SetCameraPosition();
	UpdatePlaneDistances();

	//Send the frustum view matrix to OpenGL
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);

	return UPDATE_CONTINUE;
}

void ModuleCamera::CameraMovement()
{
	float movement_speed = 15.0f / MILI_TO_SECONDS;
	float rotation_speed = 1.5f / MILI_TO_SECONDS;
	float zoom_speed = 5.0f;
	double delta = App->timer->GetDeltaTime();

	const float zoom_min = 90.0f * DEGTORAD;
	const float zoom_max = 45.0f * DEGTORAD;

	// PRESS SHIFT TO INCRESE SPEED OF MOVEMENT x2
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT))
	{
		movement_speed = movement_speed * 2;
		rotation_speed = rotation_speed * 2;
		zoom_speed = zoom_speed * 2;
	}

	float new_pos = movement_speed * delta;
	float new_angle = rotation_speed * delta;

	if(App->input->GetMouseButtonDown(SDL_MOUSE_BUTTON_RIGHT))
	{
		int mouse_x = 0;
		int mouse_y = 0;
		App->input->GetMouseMovement(mouse_x, mouse_y);
		
		//FIRST PERSON CAMERA MOVEMENT
		
		if (App->input->GetKey(SDL_SCANCODE_W))	//ABSOLUTE FRONT -> W
			camera_position += frustum.Front() * new_pos;
		if (App->input->GetKey(SDL_SCANCODE_S)) //ABSOLUTE BACK -> S
			camera_position -= frustum.Front() * new_pos;
		if (App->input->GetKey(SDL_SCANCODE_A))	//ABSOLUTE LEFT -> A
			camera_position -= frustum.WorldRight() * new_pos;
		if (App->input->GetKey(SDL_SCANCODE_D))	//ABSOLUTE RIGHT -> D
			camera_position += frustum.WorldRight() * new_pos;


		if (App->input->GetKey(SDL_SCANCODE_LALT))
		{
			Orbit(mouse_x, mouse_y);
		}
		else
		{
			// FREE LOOK AROUND ROTATIONS USING MOUSE 
			if (!camera_locked) {
				if (mouse_x != 0 || mouse_y != 0)
					CameraRotation(-mouse_x * new_angle, -mouse_y * new_angle, 0.0f);
			}
		}
		
	}
	
	// ABSOLUTE UP AND DOWN CAMERA MOVEMENT

	if (App->input->GetKey(SDL_SCANCODE_Q)) //ABSOLUTE UP -> Q
		camera_position += float3(0.0f, new_pos, 0.0f);
	if (App->input->GetKey(SDL_SCANCODE_E)) //ABSOLUTE DOWN -> E
		camera_position -= float3(0.0f, new_pos, 0.0f);

	// CAMERA ZOOM 
	int mouse_scroll = App->input->GetWheelMovement();
	if (mouse_scroll != 0)
	{
		horizontal_fov -= mouse_scroll * DEGTORAD * zoom_speed;
		if (horizontal_fov < zoom_max)
			horizontal_fov = zoom_max;
		if (horizontal_fov > zoom_min)
			horizontal_fov = zoom_min;
		frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio);
	}


	// CAMERA ROTATION USING ARROWS

	if (!camera_locked) {
		if (App->input->GetKey(SDL_SCANCODE_LEFT)) //ROTATE LEFT
			CameraRotation(new_angle, 0.0f, 0.0f);
		if (App->input->GetKey(SDL_SCANCODE_RIGHT)) //ROTATE RIGHT
			CameraRotation(-new_angle, 0.0f, 0.0f);
		if (App->input->GetKey(SDL_SCANCODE_UP)) //ROTATE UP
			CameraRotation(0.0f, new_angle, 0.0f);
		if (App->input->GetKey(SDL_SCANCODE_DOWN)) //ROTATE DOWN
			CameraRotation(0.0f, -new_angle, 0.0f);

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) 
		{
			LookAtTarget(App->renderer->model->GetModelPos());
		}
	}

}

void ModuleCamera::SetCameraPosition()
{
	frustum.SetPos(camera_position);
	if (on_orbit)
	{
		LookAtTarget(App->renderer->model->GetModelPos());
		on_orbit = false;
	}

}

void ModuleCamera::CameraRotation(float yaw, float pitch, float roll)
{
	if (yaw != 0.0f)
	{
		Quat rotation_vector = Quat::RotateY(yaw);
		camera_yaw = rotation_vector.Mul(frustum.Front()).Normalized();
		camera_pitch = rotation_vector.Mul(frustum.Up()).Normalized();
		frustum.SetFront(camera_yaw);
		frustum.SetUp(camera_pitch);
	}
	if(pitch != 0.0f)
	{
		Quat rotation_vector = Quat::RotateAxisAngle(frustum.WorldRight(), pitch);
		camera_yaw = rotation_vector.Mul(frustum.Front()).Normalized();
		camera_pitch = rotation_vector.Mul(frustum.Up()).Normalized();
		frustum.SetFront(camera_yaw);
		frustum.SetUp(camera_pitch);
	}

	if(roll != 0.0f)
	{
		//TODO
	}
}

void ModuleCamera::LookAtTarget(const float3 target)
{
	float3 test(0.0f, 0.0f, 0.0f);
	float3 dir = test - frustum.Pos();

	float3x3 m = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(m.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(m.MulDir(frustum.Up()).Normalized());
}

void ModuleCamera::Orbit(float direction_x, float direction_y)
{
	if (direction_x != 0.0f)
	{
		camera_position += frustum.WorldRight() * direction_x;
	}
	if (direction_y != 0.0f)
	{
		camera_position += float3(0.0f, direction_y, 0.0f);
	}
	on_orbit = true;
}

void ModuleCamera::ComputeAspectRatio(float width, float height)
{
	aspect_ratio = width / height;
}

void ModuleCamera::ComputeHorizontalFov(float fov)
{
	horizontal_fov = fov * DEGTORAD;
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
	ImGui::Separator;
	ImGui::Text("Camera Distance");
	ImGui::SliderFloat("pixels", &far_plane_distance, 50.0f, 10000.0f, "%.3f", 1.0f);

}
