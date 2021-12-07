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

	const float3 GetModelPos() const;

private:
	void LoadMeshes(const aiScene* scene);
	void LoadTextures(const aiScene* scene);

	std::vector<Texture> textures;
	std::vector<Mesh> meshes;
	float4x4 model_matrix = float4x4::identity;

	bool model_loaded = false;

};

