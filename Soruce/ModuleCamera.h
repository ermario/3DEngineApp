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
	void SetCamera();
	void CameraRotation(float yaw, float pitch, float roll);
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
};

