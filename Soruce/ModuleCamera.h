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
private:
	Frustum frustum;
};

