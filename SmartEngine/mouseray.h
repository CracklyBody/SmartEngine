#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

bool mouse_in_shape(int b, int s, int mouse_x, int mouse_y)
{
	glm::mat4 projection;
	float x = (2.0f * mouse_x) / b;
	float y = 1.0f - (2.0f * mouse_y) / s;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
}