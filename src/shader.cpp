#include "shader.hpp"

void Shader::Compile(std::string vertexFile, std::string fragmentFile) {
	ID = glCreateProgram();
	//glObjectLabel(GL_SHADER, ID, -1, "shader");
	std::string vertexCode = GetShaderFileContent(vertexFile);
	std::string fragmentCode = GetShaderFileContent(fragmentFile);
	unsigned int vertexShader = CompileSourceFile(GL_VERTEX_SHADER, vertexCode.c_str());
	unsigned int fragmentShader = CompileSourceFile(GL_FRAGMENT_SHADER, fragmentCode.c_str());
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glValidateProgram(ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
unsigned int Shader::CompileSourceFile(unsigned int type, const char* source) {
	unsigned int ID = glCreateShader(type);
	glShaderSource(ID, 1, &source, nullptr);
	glCompileShader(ID);
	int result;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(ID, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(ID);
		return 0;
	}
	return ID;
}

void Shader::SetMatrix4(const char* name, glm::mat4 matrix) {
	Use();
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::SetUniform1iv(const char* name, GLsizei count, const GLint* value) {
	Use();
	auto loc = glGetUniformLocation(ID, name);
	glUniform1iv(loc, count, value);

}

void Shader::Use() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}


std::string Shader::GetShaderFileContent(const std::string& filepath) {
	std::ifstream file(filepath);
	if (!file.good()) {
		std::cout << "[ERROR](Shader) Shader File " << filepath << " does not exist." << std::endl;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
