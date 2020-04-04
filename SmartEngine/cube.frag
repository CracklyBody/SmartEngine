#version 400 core
in vec3 colour;
in vec3 normal;
in vec3 FragPos;

out vec4 frag_colour;

uniform vec3 objectcolour;
uniform vec3 lightcolour;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(normal);
	vec3 lightdir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightdir),0.0);
	vec3 diffuse = diff * lightcolour;
	vec3 reflectDir = reflect(-lightdir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
	vec3 specular = specularStrength * spec * lightcolour;
	float ambientStrenght = 0.1f;
	vec3 ambient = ambientStrenght * lightcolour;
	vec3 result = (ambient + diffuse + specular) * objectcolour;
	frag_colour = vec4(result, 1.0);
};