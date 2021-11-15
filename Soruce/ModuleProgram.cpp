#include <algorithm>
#include "ModuleProgram.h"
#include "Globals.h"
#include "GL/glew.h"
#include "Application.h"

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init()
{
	bool return_flag = true;

	// Load shaders from files and compile them:
	unsigned int vertex_shader_id = CompileShader(LoadShaderSource(SHADER_VERTEX_FILEPATH), GL_VERTEX_SHADER);
	unsigned int fragment_shader_id = CompileShader(LoadShaderSource(SHADER_FRAGMENT_FILEPATH), GL_FRAGMENT_SHADER);

	program_id = CreateProgram(vertex_shader_id, fragment_shader_id);

	// Use the created program:
	glUseProgram(program_id);

	// Delete shaders since they are linked into the program and they are not needed anymore:
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);


	return return_flag;

}

char* ModuleProgram::LoadShaderSource(const char* shader_path)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_path, "rb"); //open a file obj from a file path, mode "read binary"

	if (file)
	{
		fseek(file, 0, SEEK_END); //point to the end of the file with offset 0
		int size = ftell(file);
		data = (char*)malloc(size + 1); //allocate memory for file
		fseek(file, 0, SEEK_SET); // point to the start of the file with ofsset 0
		fread(data, 1, size, file); // read the file and write it on data variable
		data[size] = 0; //set the end of the data to 0 so we know where it ends
		fclose(file);
		LOG("\n---- Loaded Shader ----\n");
	}
	else {
		LOG("\n---- Shader NOT Loaded ----\n");
	}
	return data;
	
}

unsigned ModuleProgram::CompileShader(const char* source, unsigned type)
{
	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}
	return shader_id;
}

unsigned ModuleProgram::CreateProgram(unsigned vertex_shader, unsigned fragment_shader)
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return program_id;
}

bool ModuleProgram::CleanUp()
{
	// Delete shader program:
	glDeleteProgram(program_id);

	return true;
}
