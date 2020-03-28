#pragma once
#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#define NUM_BONES_PER_VERTEX 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct VertexBoneData
{
	unsigned int IDs[NUM_BONES_PER_VERTEX];
	float Weights[NUM_BONES_PER_VERTEX];
};

struct Texture
{
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<VertexBoneData> Bones;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, const aiScene* pscene);
	void Draw(Shader shader);
private:
	unsigned int VAO, VBO, EBO, BBO;	// BBO - Bone Buffer Object
	void setupMesh(const aiScene* pscene);
};
#endif