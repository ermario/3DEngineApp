#pragma once
#include "assimp/scene.h"

class Model: public aiScene
{
public:
	Model();
	~Model();

	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);

};

