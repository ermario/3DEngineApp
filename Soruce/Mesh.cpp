#include "Mesh.h"

#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"

#include "GL/glew.h" 

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::CleanUp()
{
	if(mesh_loaded == true)
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		mesh_loaded = false;
	}
	else
	{
		EngineLOG("CLEANUP ERR: Mesh not loaded");
	}
}

void Mesh::LoadMesh(const aiMesh* mesh)
{
	EngineLOG("------ MESH CREATION PROCESS ------")
	material_index = mesh->mMaterialIndex; //TODO: change into DRAW funct
	LoadVBO(mesh);
	LoadEBO(mesh);
	CreateVAO();
	mesh_loaded = true;
	EngineLOG("5- MESH LOADED");
}

void Mesh::Draw(const std::vector<Texture>& model_textures)
{
	if (mesh_loaded == true) 
	{
		unsigned program_id = App->shader_program->GetProgramId();

		const float4x4& view = App->camera->GetViewMatrix();
		const float4x4& proj = App->camera->GetProjectionMatrix();
		float4x4 model = float4x4::identity;

		glUseProgram(program_id);
		glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_TRUE, (const float*)&model);
		glUniformMatrix4fv(glGetUniformLocation(program_id, "view"), 1, GL_TRUE, (const float*)&view);
		glUniformMatrix4fv(glGetUniformLocation(program_id, "proj"), 1, GL_TRUE, (const float*)&proj);

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model_textures[material_index].id); // ERROR ON LOADING OTHER MODEL (INDEX OUT OF RANGE)

		glUniform1i(glGetUniformLocation(program_id, "diffuse"), 0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
		glDisable(GL_TEXTURE_2D);

	}
	else { EngineLOG("ERR: Mesh DRAW was called before initialization"); }

}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	//Generate and Bind buffer 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Calculate the size of the buffer and and update the buffer with null data
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2); // Each vertex has 3 coordinates for position and 2 for textures
	unsigned buffer_size = vertex_size * mesh->mNumVertices; 
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

	//Update the subset buffer object's data
	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices; //Size of all position vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);

	unsigned uv_offset = position_size;  // Calculate the offset for textures init
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices; // Size of all texture vertices

	// Map all or part of a buffer object's data store into the client's address space
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		// Fill the UV coordinates from the mesh into the map created
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}

	//Release the mapping
	glUnmapBuffer(GL_ARRAY_BUFFER); 

	num_vertices = mesh->mNumVertices;
	EngineLOG("1- VBO MESH LOADED");
}

void Mesh::LoadEBO(const aiMesh* mesh)
{
	//Generate and Bind buffer 
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Calculate the size of the buffer and and update the buffer with null data
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3; //All faces of a mesh * 3 vertices each
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);

	//Map and fill index buffer
	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	num_indices = mesh->mNumFaces * 3;
	EngineLOG("2- EBO MESH LOADED");
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * num_vertices));
	EngineLOG("3- VAO CREATED");
}
