#pragma once
#include "Model.h"
// Light class created for lignt model
class Light :public Model
{
public:
	Light(char* path);
	~Light() {};
	void Draw(Shader shader);
	glm::vec3 lightspos;
	glm::vec3 lightcolour = glm::vec3(1.0,1.0,1.0);

private:

};
