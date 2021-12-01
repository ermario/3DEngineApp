#include "ModuleTexture.h"

#include "GL/glew.h"
#include "IL/il.h"
#include "IL/ilu.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	ilInit();
	return true;
}

bool ModuleTexture::CleanUp()
{
	ilShutDown();
	return true;
}

Texture ModuleTexture::CompileTexture(const char* path)
{
	Texture texture;
	unsigned texture_id = LoadTextureSoruce(path);

	if (texture_id != 0) {
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width_px = ilGetInteger(IL_IMAGE_WIDTH), texture.height_px = ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_BGR, GL_UNSIGNED_BYTE, ilGetData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), texture.width_px = ilGetInteger(IL_IMAGE_WIDTH),
		//	texture.height_px = ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		//	ilGetData());

		ilDeleteImages(1, &texture_id);
		texture.is_loaded = true;
	}
	texture.texutre_path = path;
	return texture;
}



unsigned ModuleTexture::LoadTextureSoruce(const char* path)
{
	ILuint texture_id; 
	ilGenImages(1, &texture_id); 
	ilBindImage(texture_id);
	if (!ilLoadImage(path))
	{
		EngineLOG("---- TEXTURE NOT LOADED -----");
		return 0;
	}
	else { EngineLOG("--- LOADED TEXTURE ----"); }
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	iluFlipImage();

	return texture_id;
}

void ModuleTexture::DeleteTexture(unsigned& texture_id)
{
	ilDeleteImages(1, &texture_id);
}
