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

	float3x4 GetViewMatrix() const { return frustum.ViewMatrix(); };
	float4x4 GetProjectionMatrix() const { return frustum.ProjectionMatrix(); };
private:
	Frustum frustum;
};

