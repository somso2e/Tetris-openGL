#pragma once
#ifndef SHADER_HPP
#define SHADER_HPP
#include <common.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int ID;

	void Compile(std::string vertexFile, std::string fragmentFile);
	void SetMatrix4(const char* name, glm::mat4 matrix);
	void SetUniform1iv(const char* name, GLsizei count, const GLint* value);
	void Use();
	void Delete();
private:
	unsigned int CompileSourceFile(unsigned int type, const char* source);
	std::string GetShaderFileContent(const std::string& filepath);

};
#endif