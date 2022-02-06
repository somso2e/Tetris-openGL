#include <common.hpp>
#include "VAO.hpp"
#include "IBO.hpp"
#include "texture.hpp"
enum class Color {
	Gray,
	Yellow,
	Green,
	Orange,
	Cyan,
	Magenta,
	Pink,
	Red
};

class Cell {
private:
	void initRender();
	glm::vec4 getColor(Color color);
public:
	Shader shader;
	Texture texture;
	VAO VAO;

	Cell(Shader& shaderProgram, Texture& texture);
	void Draw(glm::vec2 position, Color color, glm::vec2 size = glm::vec2(CELL_WIDTH, CELL_HEIGHT));

};
