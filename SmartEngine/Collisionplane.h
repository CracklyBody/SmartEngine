#pragma once
#ifndef COLLISIONPLANE_H
#define COLLISIONPLANE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Collisionplane
{
public:
	glm::vec3 p[4];
	glm::vec3 normal;
	Collisionplane(float n1, float n2, float n3,
		float a, float b, float c,
		float d, float e, float f,
		float g, float h, float i,
		float j, float k, float l);

	~Collisionplane();
};

#endif