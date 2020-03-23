#include "Collisionplane.h"

Collisionplane::Collisionplane(float n1, float n2, float n3,
	float a, float b, float c,
	float d, float e, float f,
	float g, float h, float i,
	float j, float k, float l)
{
	normal = glm::vec3(n1, n2, n3);
	p[0] = glm::vec3(a, b, c);
	p[1] = glm::vec3(d, e, f);
	p[2] = glm::vec3(g, h, i);
	p[3] = glm::vec3(j, k, l);
}