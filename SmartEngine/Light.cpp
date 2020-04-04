#include "Light.h"

Light::Light(char* path) : Model(path)
{
	
}

void Light::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}