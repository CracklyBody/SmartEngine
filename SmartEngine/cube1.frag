#version 400 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 1
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform DirLight dirlight;
uniform Material material;
uniform vec3 viewPos;

out vec4 frag_colour;

vec3 CalcDirLight(DirLight light, vec3 Normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 Normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcDirLight(dirlight,norm, viewDir);

	for(int i =0;i<NR_POINT_LIGHTS;i++)
		result+=CalcPointLight(pointLight[i],norm,FragPos,viewDir);

	frag_colour = vec4(result, 1.0);
};

vec3 CalcDirLight(DirLight light, vec3 Normal,vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(Normal, lightDir),0.0);
	vec3 reflectDir = reflect(-lightDir,Normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light,vec3 Normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(Normal,lightDir),0.0);

	vec3 reflectDir = reflect(-lightDir,Normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

	float distance1 = length(light.position  - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance1 + light.quadratic * (distance1 * distance1));
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
