#pragma once

#include "Mesh.h"
#include "ModuleTexture.h"

#include "assimp/scene.h"

#include <vector>

class Model
{
public:
	Model();
	~Model();

	void Draw();
	void LoadModel(const char* file_name);
	void CleanUp();

private:
	void LoadMeshes(const aiScene* scene);
	void LoadTextures(const aiScene* scene);

	std::vector<Texture> textures;
	std::vector<Mesh> meshes;

	bool model_loaded = false;

};

