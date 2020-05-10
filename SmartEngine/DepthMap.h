#pragma once

#define GLEW32_DLL
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class DepthMap {
public:
	unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
	unsigned int depthMapFbo;
	unsigned int depthMap;
	glm::vec3 lightPos;
	// Prepare depthMap befor draw shadow
	void bind();

	DepthMap();
	~DepthMap();
};