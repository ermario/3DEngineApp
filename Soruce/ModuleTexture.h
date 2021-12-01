#pragma once
#include "Module.h"

struct Texture {
	unsigned id;
	bool is_loaded = false;
	const char* texutre_path;
	unsigned width_px;
	unsigned height_px;
};

class ModuleTexture : public Module
{
public:

	ModuleTexture();
	~ModuleTexture();

	bool Init();
	bool CleanUp();

	unsigned GetTexture(Texture* texture) { return texture->id; }

	Texture CompileTexture(const char* path);

private:

	Texture texture;
	unsigned LoadTextureSoruce(const char* path);
	void DeleteTexture(unsigned& texture_id);
};

