#version 400 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture_diffuse;

void main()
{
	FragColor = vec4(1.0,0.0,0.0,0.0);//texture(texture_diffuse,TexCoords);
}