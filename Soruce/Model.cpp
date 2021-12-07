#include "Model.h"

#include "Application.h"
#include "ModuleTexture.h"

#include "GL/glew.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "IL/il.h"
#include "IL/ilu.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::Draw()
{
	if(model_loaded)
	{
		for (Mesh& mesh : meshes) { //FOREACH mesh inside meshes 
			mesh.Draw(textures);
		}
	}
}

void Model::LoadModel(const char* file_name)
{
	if (model_loaded) // Delete previous model
		CleanUp();

	EngineLOG("------ LOADING MODEL ------")
	const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate); //aiProcess_Triangulate fixes EBO with multiple meshes
	if (scene)
	{
		//TODO: ERROR CONTROL 
		LoadTextures(scene);
		LoadMeshes(scene);
		model_loaded = true;
	}
	else
	{
		EngineLOG("ERR: Failed loading model: %s --- %s", file_name, aiGetErrorString());
	}
}

void Model::CleanUp()
{
	//Clear Meshes
	for (Mesh& mesh : meshes) {
		mesh.CleanUp();
	}
	meshes.clear();

	//Clear Textures
	for (Texture texture : textures) {
		glDeleteTextures(1, &texture.id);
	}
	textures.clear();

	model_loaded = false;
	EngineLOG("------ MODEL UNLOADED ------");
}

const float3 Model::GetModelPos() const
{
	float3 test = model_matrix.TranslatePart();
	return test;
}

void Model::LoadMeshes(const aiScene* scene)
{
	textures.reserve(scene->mNumMeshes);
	EngineLOG("2- Loading Meshes: %d", scene->mNumMeshes)
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		EngineLOG("Mesh %d", i);
		Mesh mesh = Mesh();
		mesh.LoadMesh(scene->mMeshes[i]);
		if (mesh.IsLoaded()) {
			meshes.push_back(mesh);
			EngineLOG("Success");
		}
		else {
			EngineLOG("ERR: Failed loading mesh: %d --- %s", i, aiGetErrorString());
		}
	}
}

void Model::LoadTextures(const aiScene* scene)
{
	EngineLOG("1- Loading Textures: %d", scene->mNumMaterials);
	aiString file;
	textures.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			Texture aux_texture;
			aux_texture = App->textures->CompileTexture(file.data);

			if (!aux_texture.is_loaded)
			{
				EngineLOG("ERR: Failed loading texture: %s --- %s", file.data, aiGetErrorString());
				//TODO PATH HIERARCHY CONTROL
				// a) First check on the path described in the FBX
				// b) Then check on the same folder you loaded the FBX
				// c) Last, try in your own “Textures/” folder
			}
			else
			{
				textures.push_back(aux_texture);
			}
		}
	}
}

