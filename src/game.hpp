#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "renderer.hpp"
enum class TetrominoType {
	I, O, J, L, T, S, Z
};

struct Tetromino {
	std::array <std::array<bool, 4>, 4>  shape = { {0} };
	Color color = Color::Gray;
	enum TetrominoType type;
	int x = 0;
	int y = 0;
	int rotation = 0;
	bool placed = false;
};

const int NUM_OF_TETRIMOS = 7;


enum class Movement {
	Right,
	Left,
	Down,
	RotateR,
	RotateL
};

class Game {
public:
	Game(GLFWwindow* window);
	~Game();
	void Update();

private:
	GLuint EmptyCellTextureID, tex2, tex3;

	uint8_t Keys[350] = { 0 };

	bool KeysPressed[350]{ false };
	bool KeysProcessed[350]{ false };
	bool KeysHeld[350]{ false };
	//bool KeysProcessed[350]{ false };

	std::array<std::array<Color, NUM_OF_CELLS_H>, NUM_OF_CELLS_W> Map = { {Color::Gray} };
	Tetromino ActiveTetromino;
	float ts = 0.0f, inputTime = 0.0f, Delay = 1.0f;

	void MoveTetromino(Movement movement);
	bool HasCollied(Tetromino tetromino);
	void ProcessInput();
	Tetromino GenerateNewTetromino();
	glm::vec4 GetColor(Color color);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void s_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	// TESTING
	float x = 0.0f, y = 0.0f, sizeX = 0.5f, sizeY = 0.5f;
};


const std::vector<Tetromino> tetrominos{
	//I
	Tetromino{{{
		{0, 0, 0, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}}}

		,Color::Cyan, TetrominoType::I},
	// O
	Tetromino{{{
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Yellow, TetrominoType::O},
	// J
	Tetromino{{{
		{0, 0, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Magenta, TetrominoType::J},
	// L
	Tetromino{{{
		{0, 0, 0, 0},
		{0, 0, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Orange, TetrominoType::L},
	// T
	Tetromino{{{
		{0, 0, 0, 0},
		{0, 1, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Green, TetrominoType::T},
	// S
	Tetromino{{{
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}}}

		,Color::Pink, TetrominoType::S},
	// Z
	Tetromino{{{
		{0, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Red, TetrominoType::Z},
};
#endif
