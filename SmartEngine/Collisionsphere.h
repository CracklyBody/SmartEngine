#pragma once
#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Collisionsphere 
{
public:
	float r; // radius
	glm::vec3 center;
	Collisionsphere();
	Collisionsphere(const glm::vec3 vec, float rad);
	~Collisionsphere();
};

#endif