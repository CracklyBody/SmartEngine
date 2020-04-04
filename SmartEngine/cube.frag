#version 400 core
in vec3 colour;
out vec4 frag_colour;

uniform vec3 objectcolour;
uniform vec3 lightcolour;

void main()
{
	frag_colour = vec4(lightcolour * objectcolour, 1.0);
};