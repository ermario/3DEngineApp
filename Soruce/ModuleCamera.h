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
	float4x4 GetViewMatrix() const { return float4x4(frustum.ViewMatrix()); };
	float4x4 GetProjectionMatrix() const { return frustum.ProjectionMatrix(); };
	float4x4 GetProjectionMatrixTransposed() const { return frustum.ProjectionMatrix().Transposed(); };
private:
	Frustum frustum;
	double angle = 0;
};

