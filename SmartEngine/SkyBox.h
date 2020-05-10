#pragma once
#define GLEW32_DLL
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

#include <SOIL/stb_image_aug.h>
#include <SOIL/SOIL.h>

#include "Shader.h"

class SkyBox {
public:
	float * skyboxVertices;
	std::vector<std::string> textures_faces;
	GLuint textureID;
	void draw();
	SkyBox(std::vector<std::string> textures);
	~SkyBox();
private:
	unsigned int skyboxVAO, skyboxVBO;
};