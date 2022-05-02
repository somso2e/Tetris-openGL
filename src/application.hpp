#pragma once
#include "error.hpp"
#include "game.hpp"

namespace Application {
	int Run();
	void Shutdown();

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void window_size_callback(GLFWwindow* window, int width, int height);

	extern Game Tetris;
	extern GLFWwindow* window;
}
