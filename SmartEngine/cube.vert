#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 colour;
out vec3 normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

void main()
{
	colour = color;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	normal = mat3(transpose(inverse(view * model))) * aNormal;
	FragPos = vec3(view * model * vec4(aPos,1.0f));
}