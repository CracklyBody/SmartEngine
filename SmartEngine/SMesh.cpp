#include "SMesh.h"

Mesh::Mesh(std::vector<Vertex>& v, std::vector<GLuint>& i, std::vector<Texture>& t)
{
	vertices = v;
	indices = i;
	textures = t;

	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoords));
	glEnableVertexAttribArray(0);

	for (int i = 0; i < BONES_AMOUNT; i++)
	{
		glVertexAttribPointer(3 + i, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, boneIDs) + sizeof(float) * i));
		glEnableVertexAttribArray(3 + i);
	}

	for (int i = 0; i < BONES_AMOUNT; i++)
	{
		glVertexAttribPointer(9 + i, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, weights) + sizeof(float) * i));
		glEnableVertexAttribArray(9 + i);
	}
	glBindVertexArray(0);
}

void Mesh::draw(ShaderLoader* ShaderLoader)
{
	unsigned int diffuseNR = 1;
	unsigned int specularNR = 1;

	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;

		if (textures[i].type == "texture_diffuse")
		{
			number = std::to_string(diffuseNR);
			diffuseNR++;
		}
		else if (textures[i].type == "texture_specular")
		{
			number = std::to_string(specularNR);
			specularNR++;
		}
		glUniform1i(glGetUniformLocation(ShaderLoader->ID, ("material." + textures[i].type + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

Mesh::~Mesh(){}