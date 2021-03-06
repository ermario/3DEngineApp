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
			mesh.Draw(texture);
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
	for (Texture texture : texture) {
		glDeleteTextures(1, &texture.id);
	}
	texture.clear();

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
	texture.reserve(scene->mNumMeshes);
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

	texture.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			std::string texture_path = file.data;
			std::string folder_path = texture_path.substr(0, texture_path.find_last_of("/\\")+1);
			std::string default_path = "Resources/Textures/\\";
			Texture aux_texture;
			aux_texture = App->textures->CompileTexture(file.data); // a) First check on the path described in the FBX
			if (!aux_texture.is_loaded)
			{
				EngineLOG("ERR: Failed loading texture: %s from model", texture_path.c_str());
				aux_texture = App->textures->CompileTexture((folder_path + texture_path).c_str()); // b) Then check on the same folder you loaded the FBX
				
				if (!aux_texture.is_loaded)
				{
					EngineLOG("ERR: Failed loading texture: %s from folder: %s ", texture_path.c_str(), folder_path.c_str());
					aux_texture = App->textures->CompileTexture((default_path + texture_path).c_str()); // c) Last, try in your own ?Textures/? folder
					
					if (!aux_texture.is_loaded)
						EngineLOG("ERR: Failed loading texture: %s from default folder: %s", texture_path.c_str(), default_path.c_str());
				}
				
				
			}

			if (aux_texture.is_loaded)
				texture.push_back(aux_texture);
			else
				EngineLOG("ERR: Texture: %s can not be found", texture_path.c_str())
		}
		else
		{
			Texture aux_texture = App->textures->CompileTexture(DEFAULT_SOLID_TEXTURE); // a) First check on the path described in the FBX
			if (!aux_texture.is_loaded)
			{
				EngineLOG("ERR: Failed loading solid texture");
			}
			if (aux_texture.is_loaded)
				texture.push_back(aux_texture);
		}
	}
}

