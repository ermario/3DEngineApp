#pragma once

#include "ModuleTexture.h"

#include "assimp/mesh.h"
#include "MathGeoLib.h"

#include <vector>


class Mesh: public aiMesh
{
public:

	Mesh();
	~Mesh();
	bool IsLoaded() { return mesh_loaded; }
	void CleanUp();

	void LoadMesh(const aiMesh* mesh);
	void Draw(const std::vector<Texture>& model_textures);

private:
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	unsigned VBO, EBO, VAO;
	bool mesh_loaded = false;
	unsigned num_vertices = 0;
	unsigned num_indices = 0;
	unsigned material_index = 0; //texture
};

