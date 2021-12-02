#pragma once
#include "assimp/mesh.h"

class Mesh: public aiMesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	//void Draw(const std::vector<unsigned>& model_textures);
private:
	
};

