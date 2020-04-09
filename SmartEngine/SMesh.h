#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LoadShader.h"
#define BONES_AMOUNT 6

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	float boneIDs[BONES_AMOUNT] = { 0.0f };
	float weights[BONES_AMOUNT] = { 0.0f };
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
private:
	GLuint VAO;
	GLuint VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	void setupMesh();
public:
	Mesh(std::vector<Vertex>& v, std::vector<GLuint>& i, std::vector<Texture>& t);
	void draw(ShaderLoader* ShaderLoader);
	~Mesh();
};