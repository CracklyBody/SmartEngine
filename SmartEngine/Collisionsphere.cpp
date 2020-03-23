#include "Collisionsphere.h"

Collisionsphere::Collisionsphere()
{
	r = 0;
}
Collisionsphere::Collisionsphere(const glm::vec3 vec, float rad)
{
	center = vec;
	r = rad;
}