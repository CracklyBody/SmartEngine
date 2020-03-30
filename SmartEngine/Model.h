#pragma once
#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <SOIL/stb_image_aug.h>

struct Plane {
	glm::vec3 xAxis;
	glm::vec3 yAxis;
	glm::vec3 zAxis;
};

class Model
{
public:
	Model(char* path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
	void update();

	std::vector<glm::vec2> getConvexHull();
	std::vector<glm::vec3> getNormals();
	std::vector<Plane> getPlanes(Model* secondPh);
	Plane setPlane(Plane plane, std::vector<glm::vec3>firstNormals, std::vector<glm::vec3>secondNormals,int num);
	Plane setFrom(glm::vec3 normal);
private:
	float radius;
	float maxX=0.0f, minX=0.0f, maxY=0.0f, minY=0.0f, maxZ=0.0f, minZ=0.0f;
	glm::vec3 moveVec = glm::vec3(0.0001f, 0.0001f, 0.0001f);
	glm::vec3 zeroPoint;
	glm::vec3 collcube[8];	// collision cube
	std::vector<glm::vec3> cubenormals;
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texture_loaded;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};




#endif