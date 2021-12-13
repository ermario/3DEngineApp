#pragma once
#include "Globals.h"
#include "Module.h"

class ModuleTimer : public Module
{
public:
	ModuleTimer();
	~ModuleTimer();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	inline double GetDeltaTime() { return delta_time; }

private:
	double delta_time = 0.0f;
	double ticks_count = 0.0f;
};

