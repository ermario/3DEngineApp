#pragma once
#include <windows.h>

#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FULLSCREEN false
#define RESIZABLE true
#define VSYNC true
#define TITLE "Super Awesome Engine"

// Files -----------
#define SHADER_VERTEX_FILEPATH "test_vertex.glsl"
#define SHADER_FRAGMENT_FILEPATH "test_fragment.glsl"