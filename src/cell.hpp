#pragma once
#ifndef CELL_HPP
#define CELL_HPP

#include <common.hpp>
#include "VAO.hpp"
#include "texture.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Cell {
private:
	glm::vec4 getColor(Color color);
public:
	Shader shader;
	Texture texture;
	VAO VAO;

	void Init(Shader& shaderProgram, Texture& texture);
	Cell() {};
	~Cell();
	void Render(glm::vec2 position, Color color, glm::vec2 size);

};
#endif