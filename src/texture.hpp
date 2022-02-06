#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shader.hpp"

class Texture {
public:
	GLuint ID;
	GLuint InternalFormat;
	GLuint ImageFormat;
	GLuint WrapS;
	GLuint WrapT;
	GLuint FilterMin;
	GLuint FilterMax;

	Texture();
	void Generate(const char* imageFilePath);
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};
#endif