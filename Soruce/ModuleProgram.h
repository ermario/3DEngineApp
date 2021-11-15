#pragma once
#include "Module.h"

class Application;

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	char* LoadShaderSource(const char* shader_path);
	unsigned CompileShader(const char* source, unsigned type); //unsigned == unsigned int
	unsigned CreateProgram(unsigned vertex_shader, unsigned fragment_shader);
	unsigned GetProgramId() { return program_id; };

	bool CleanUp();
private:
	unsigned program_id;
};
