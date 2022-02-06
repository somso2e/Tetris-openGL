#include "texture.hpp"

Texture::Texture() :
	InternalFormat(GL_RGB),ImageFormat(GL_RGB),WrapS(GL_REPEAT),WrapT(GL_REPEAT),FilterMin(GL_LINEAR),FilterMax(GL_LINEAR) {
	glGenTextures(1, &this->ID);
}
Texture::~Texture() {

}
void Texture::Generate(const char* imageFilePath) {

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);

	// is numColCh needed????
	unsigned char* bytes = stbi_load(imageFilePath, &widthImg, &heightImg, &numColCh, 0);
	if (!bytes) {

		std::cout << "[ERROR](Texture) Image File " << imageFilePath << " does not exist." << std::endl;
		return;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, widthImg, heightImg, 0, this->InternalFormat, GL_UNSIGNED_BYTE, bytes);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Use();
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &this->ID);
}
