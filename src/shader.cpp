#include <shader.hpp>
void Shader::Compile(std::string vertexFile, std::string fragmentFile) {
	this->ID = glCreateProgram();

	std::string vertexCode = getShaderFileContent(vertexFile);
	std::string fragmentCode = getShaderFileContent(fragmentFile);
	unsigned int vertexShader = CompileSourceFile(GL_VERTEX_SHADER, vertexCode.c_str());
	unsigned int fragmentShader = CompileSourceFile(GL_FRAGMENT_SHADER, fragmentCode.c_str());
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);
	glLinkProgram(this->ID);
	glValidateProgram(this->ID);
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

void Shader::SetMatrix4(const char* name,glm::mat4 matrix) {
	this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));

}

void Shader::Use() {
	glUseProgram(this->ID);
}

void Shader::Delete() {
	glDeleteProgram(this->ID);
}


std::string getShaderFileContent(const std::string& filepath) {
	std::ifstream file(filepath);
	if (!file.good()) {
		std::cout << "[ERROR](Shader) Shader File " << filepath << " does not exist." << std::endl;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

//unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
//	unsigned int program = glCreateProgram();
//	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//	glAttachShader(program, vs);
//	glAttachShader(program, fs);
//	glLinkProgram(program);
//	glValidateProgram(program);
//	glDeleteShader(vs);
//	glDeleteShader(fs);
//
//	return program;
//}
//shaderProgramSource parseShader(const std::string& filepath) {
//	std::ifstream stream(filepath);
//	enum class shaderType {
//		NONE = -1,
//		VERTEX = 0,
//		FRAGMENT = 1
//	};
//
//	std::string line;
//	std::stringstream ss[2];
//	shaderType type = shaderType::NONE;
//
//	while (getline(stream, line)) {
//		if (line.find("shader") != std::string::npos) {
//			if (line.find("vertex") != std::string::npos) {
//				type = shaderType::VERTEX;
//			}
//			else if (line.find("fragment") != std::string::npos) {
//				type = shaderType::FRAGMENT;
//			}
//		}
//		else {
//			ss[(int)type] << line << '\n';
//		}
//	}
//	return { ss[0].str(),ss[1].str() };
//}
