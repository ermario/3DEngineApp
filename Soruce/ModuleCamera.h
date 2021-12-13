#pragma once
#include "Module.h"
#include "Geometry/Frustum.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	bool Clear();
	update_status Update();

	void CameraMovement();
	void CameraRotation(float yaw, float pitch, float roll);
	void SetCameraPosition();
	void LookAtTarget(const float3 target);
	void Orbit(float direction, float direction_y);

	void ComputeAspectRatio(float width, float height);
	void ComputeHorizontalFov(float fov);
	void UpdateFovAndAspectRatio(){ frustum.SetHorizontalFovAndAspectRatio(horizontal_fov, aspect_ratio); }
	void UpdatePlaneDistances(){ frustum.SetViewPlaneDistances(near_plane_distance, far_plane_distance); }
	float4x4 GetViewMatrix() const { return float4x4(frustum.ViewMatrix()); };
	float4x4 GetViewMatrixTransposed() const { return float4x4(frustum.ViewMatrix()).Transposed(); };
	float4x4 GetProjectionMatrix() const { return frustum.ProjectionMatrix(); };
	float4x4 GetProjectionMatrixTransposed() const { return frustum.ProjectionMatrix().Transposed(); };

	void ImGuiCamera();
private:
	Frustum frustum;
	double angle = 0;
	float3 camera_position; 
	float3 camera_pitch;
	float3 camera_yaw;
	bool camera_locked = false;
	bool on_orbit = false;
	float aspect_ratio = 0.0f;
	float horizontal_fov = 0.0f;
	float near_plane_distance = 0.0f;
	float far_plane_distance = 0.0f;
};

