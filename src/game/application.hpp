#pragma once
#include "engine/error.hpp"
#include "game/game.hpp"
#include <chrono>
#include <thread>
namespace Application {
	int Run();
	void Shutdown();
	void SetIcon(const char* iconFilePath);

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void window_size_callback(GLFWwindow* window, int width, int height);

	extern Game Tetris;
	extern GLFWwindow* window;
}
