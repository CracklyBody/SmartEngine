#pragma once
#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <SOIL/stb_image_aug.h>

class Model
{
public:
	Model(char* path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
	void update();
private:
	float radius;
	float maxX=0.0f, minX=0.0f, maxY=0.0f, minY=0.0f, maxZ=0.0f, minZ=0.0f;
	glm::vec3 moveVec = glm::vec3(0.0001f, 0.0001f, 0.0001f);
	glm::vec3 zeroPoint;
	std::vector<glm::vec3> collcube;	// collision cube
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texture_loaded;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};




#endif