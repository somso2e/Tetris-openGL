#pragma once 
#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H
#include <stb/stb_image.h>

#include "shader.hpp"

class Texture {
public:
	GLuint ID;

	Texture();
	~Texture();
	void Generate(const char* imageFilePath);
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};
#endif