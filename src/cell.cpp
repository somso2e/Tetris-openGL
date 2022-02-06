#include "cell.hpp"


Cell::Cell(Shader& shaderProgram, Texture& texture) {
	this->shader = shaderProgram;
	this->texture = texture;
	this->initRender();
}
Cell::~Cell() {
	glDeleteVertexArrays(1, &this->VAO.ID);
}
void Cell::initRender() {
	std::vector<Vertex> vertices = {
		Vertex{glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		Vertex{glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},

		Vertex{glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
	};

	this->VAO.Bind();
	VBO VBO(vertices);
	this->VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, sizeof(Vertex), (void*)0);
	this->VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(2 * sizeof(float)));
	VBO.Unbind();
	this->VAO.Unbind();
}
void Cell::Draw(glm::vec2 position, Color color, glm::vec2 size) {

	this->shader.Use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));
	this->shader.SetMatrix4("model", model);

	glm::vec4 colorCode = getColor(color);
	glUniform4f(glGetUniformLocation(this->shader.ID, "aColor"), colorCode.x, colorCode.y, colorCode.z, colorCode.w);

	this->texture.texUnit(shader, "tex0", 0);
	this->texture.Bind();
	this->VAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	this->VAO.Unbind();
}

glm::vec4 Cell::getColor(Color color) {
	using enum  Color;
	switch (color) {
	case Gray:
		return glm::vec4(0.55f, 0.55f, 0.55f, 1.0f);

	case Yellow:
		return glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

	case Green:
		return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	case Orange:
		return glm::vec4(1.0f, 0.6f, 0.0f, 1.0f);

	case Cyan:
		return glm::vec4(0.4f, 1.0f, 0.8f, 1.0f);

	case Magenta:
		return glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

	case Pink:
		return glm::vec4(1.0f, 0.4f, 0.7f, 1.0f);

	case Red:
		return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
}
