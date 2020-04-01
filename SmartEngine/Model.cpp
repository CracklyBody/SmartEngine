#include "Model.h"

std::vector<Plane> Model::getPlanes(Model* secondPh)
{
	std::vector<Plane> planes;
	std::vector<glm::vec3> firstNormals = cubenormals;
	std::vector<glm::vec3> secondNormals = secondPh->cubenormals;
	Plane plane;
	int size = firstNormals.size() + secondNormals.size();

	for (int i = 0; i < size; i++)
	{
		//plane = setPlane(plane, firstNormals, secondNormals,i);
	}
	return planes;
}

Plane swapXZ(Plane pl)
{
	glm::vec3 temp = pl.xAxis;
	pl.xAxis = pl.zAxis;
	pl.zAxis = temp;
	return pl;
}

Plane Model::setPlane(Plane plane, std::vector<glm::vec3>firstNormals, std::vector<glm::vec3>secondNormals,int num)
{
	if (num < firstNormals.size())
		plane = setFrom(firstNormals[num]);
	else
	{
		num -= firstNormals.size();
		plane = setFrom(secondNormals[num]);
	}
	plane = swapXZ(plane);	// XZ OR ZY?
	return plane;
}

float getLenght(glm::vec3 vec)
{
	double powX = pow(vec.x, 2);
	double powY = pow(vec.y, 2);
	double powZ = pow(vec.z, 2);

	return (float)sqrt(powX + powY + powZ);
}

glm::vec3 normalize(glm::vec3 vec)
{
	float lenght = getLenght(vec);

	if (lenght == 0.0f)
		return vec;
	vec.x /= lenght;
	vec.y /= lenght;
	vec.z /= lenght;
	return vec;
}

glm::vec3 cross(glm::vec3 value, glm::vec3 other)
{
	float result1 = value.y * other.z;
	float result2 = -1 * value.z * other.y;
	float result3 = -1 * value.x * other.z;
	float result4 = value.z * other.x;
	float result5 = value.x * other.y;
	float result6 = -1 * value.y * other.x;
	return glm::vec3(result1 + result2, result3 + result4, result5 + result6);
}

Plane Model::setFrom(glm::vec3 normal)
{
	Plane plane;

	plane.zAxis = normal;
	plane.xAxis = normal;
	plane.xAxis = glm::vec3(-plane.xAxis.y,plane.xAxis.x,0);

	plane.yAxis = normal;
	plane.yAxis = cross(plane.yAxis, plane.xAxis);

	plane.xAxis = normalize(plane.xAxis);
	plane.yAxis = normalize(plane.yAxis);
	plane.zAxis = normalize(plane.zAxis);

	return plane;
}


void Model::update()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		for (unsigned int j = 0; j < meshes[i].vertices.size(); j++)
		{
			meshes[i].vertices[j].Position += moveVec;
		}
		meshes[i].setupMesh();
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	zeroPoint = glm::vec3((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);
	radius = sqrtf((maxX - minX) * (maxX - minX) + (maxY - minY) * (maxY - minY) + (maxZ - minZ) * (maxZ - minZ))/2;
	collcube[0] = glm::vec3(zeroPoint.x - radius, zeroPoint.y - radius, zeroPoint.z + radius);
	collcube[1] = glm::vec3(zeroPoint.x + radius, zeroPoint.y - radius, zeroPoint.z + radius);
	collcube[2] = glm::vec3(zeroPoint.x + radius, zeroPoint.y - radius, zeroPoint.z - radius);
	collcube[3] = glm::vec3(zeroPoint.x - radius, zeroPoint.y - radius, zeroPoint.z - radius);
	
	collcube[4] = glm::vec3(zeroPoint.x - radius, zeroPoint.y + radius, zeroPoint.z + radius);
	collcube[5] = glm::vec3(zeroPoint.x + radius, zeroPoint.y + radius, zeroPoint.z + radius);
	collcube[6] = glm::vec3(zeroPoint.x + radius, zeroPoint.y + radius, zeroPoint.z - radius);
	collcube[7] = glm::vec3(zeroPoint.x - radius, zeroPoint.y + radius, zeroPoint.z - radius);

	cubenormals.push_back (glm::vec3(0.0f, 0.0f, 1.0f));
	cubenormals.push_back ( glm::vec3(-1.0f, 0.0f, 0.0f));
	cubenormals.push_back ( glm::vec3(0.0f, -1.0f, 0.0f));
	cubenormals.push_back ( glm::vec3(1.0f, 0.0f, 0.0f));
	cubenormals.push_back ( glm::vec3(0.0f, -1.0f, 0.0f));
	cubenormals.push_back ( glm::vec3(0.0f, 0.0f, 1.0f));
			   
	cubenormals.push_back (glm::vec3(1.0f, 0.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, -1.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 0.0f, -1.0f)); 
	cubenormals.push_back (glm::vec3(-1.0f, 0.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, -1.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 0.0f, -1.0f));	
			   
	cubenormals.push_back (glm::vec3(-1.0f, 0.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 1.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 0.0f, 1.0f));
	cubenormals.push_back (glm::vec3(1.0f, 0.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 1.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 0.0f, 1.0f));
			   
	cubenormals.push_back (glm::vec3(1.0f, 0.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 1.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 0.0f, -1.0f));
	cubenormals.push_back (glm::vec3(-1.0f, 0.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 1.0f, 0.0f));
	cubenormals.push_back (glm::vec3(0.0f, 0.0f, -1.0f));

}
void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}
unsigned int TextureFromFile1(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		if (mesh->mVertices[i].x > maxX)
			maxX = mesh->mVertices[i].x;
		if (mesh->mVertices[i].x < minX)
			minX = mesh->mVertices[i].x;
		if (mesh->mVertices[i].y > maxY)
			maxY = mesh->mVertices[i].y;
		if (mesh->mVertices[i].y < minY)
			minY = mesh->mVertices[i].y;
		if (mesh->mVertices[i].z > maxZ)
			maxZ = mesh->mVertices[i].z;
		if (mesh->mVertices[i].z < minZ)
			minZ = mesh->mVertices[i].z;

		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < texture_loaded.size(); j++)
		{
			if (std::strcmp(texture_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(texture_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile1(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			texture_loaded.push_back(texture);
		}
	}
	return textures;
}

void Model::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

