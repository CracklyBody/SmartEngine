#pragma once
// GLEW GLFW
#define GLEW32_DLL
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>
#include <string>
// Freetype library
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

class Font
{
	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};
	// Here we store 128 character
	std::map<GLchar, Character> Characters;
	Shader* shader;
	GLuint VAO, VBO;
public:
	FT_Library ft;
	FT_Face face;
	Font();
	~Font();
	void render_text(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};