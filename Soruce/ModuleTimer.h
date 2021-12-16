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

	void UpdateRealDeltaTimer();
	const double GetDeltaTime() const { return real_delta_time; }

	void ImguiTimer();

private:
	double real_delta_time = 0.0f;
	double real_ticks_count = 0.0f;
};

