#pragma once
//#include <gl/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

#include "error.hpp"
constexpr auto WINDOW_HEIGHT = 960;
constexpr auto WINDOW_WIDTH = 480;

constexpr auto CELLS_NUM_W = 10;
constexpr auto CELLS_NUM_H = 20;


constexpr float CELL_WIDTH = (float)WINDOW_WIDTH / CELLS_NUM_W;
constexpr float CELL_HEIGHT = (float)WINDOW_HEIGHT / CELLS_NUM_H;

//typedef  std::array<std::array<Color, CELLS_WIDTH>, CELLS_HEIGHT> TetrisCells;
//TetrisCells TetrisMap;
struct Vertex {
	glm::vec2 vertices;
	glm::vec2 texCoords;
};
