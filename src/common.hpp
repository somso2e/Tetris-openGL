#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

constexpr unsigned int WINDOW_HEIGHT = 960;
constexpr unsigned int WINDOW_WIDTH = 480;

constexpr unsigned int NUM_OF_CELLS_W = 10;
constexpr unsigned int NUM_OF_CELLS_H = 20;


constexpr float CELL_WIDTH = (float)WINDOW_WIDTH / NUM_OF_CELLS_W;
constexpr float CELL_HEIGHT = (float)WINDOW_HEIGHT / NUM_OF_CELLS_H;



