#pragma once
#include <windows.h>

#include <stdio.h>

#define EngineLOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define BROWSER(url) OpenBrowser(url);


void log(const char file[], int line, const char* format, ...);

void OpenBrowser(const char* url);


enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Deletes a buffer
#define RELEASE( x ) \
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

// Configuration -----------
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 720
#define SCREEN_SIZE 2
#define DEFAULT_SCREEN_RATIO 0.95f
#define DEFAULT_FOV 90.0f
#define DEFAULT_NEAR_DIST 0.1f
#define DEFAULT_FAR_DIST 100.0f
#define FULLSCREEN false
#define RESIZABLE true
#define VSYNC true
#define TITLE "Super Awesome Engine"
#define GLSL_VERSION "#version 330"


//CONSTANTS
#define MILI_TO_SECONDS 1000.0f
#define DEGTORAD .0174533f


//SDL EVENTS
#define SDL_MOUSE_BUTTON_RIGHT 3

// Files -----------
#define SHADER_VERTEX_FILEPATH "Resources/vertex.glsl"
#define SHADER_FRAGMENT_FILEPATH "Resources/fragment.glsl"
#define DEFAULT_SOLID_TEXTURE "Resources/Textures/solid_texture.png"

