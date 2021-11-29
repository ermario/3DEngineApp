#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleDummy : public Module
{
	bool Init()
	{
		EngineLOG("Dummy Init!");
		return true;
	}

	bool CleanUp()
	{
		EngineLOG("Dummy CleanUp!");
		return true;
	}
};