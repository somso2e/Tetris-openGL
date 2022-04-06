#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "renderer.hpp"


class Game {
public:
	Game(GLFWwindow* window);
	~Game();
	void Update();
	uint32_t Score = 0;

private:
	enum class Color { Gray, Yellow, Green, Orange, Cyan, Magenta, Pink, Red, Blue };
	enum class TetrominoType { I, O, J, L, T, S, Z };

	typedef glm::vec<2, int> point;
	struct Tetromino {
		std::array<point, 4> coords;
		Color color = Color::Gray;
		enum TetrominoType type;
		point origin = point(4, 1);
		int rotation = 0;
		bool placed = false;
	};
	const std::vector<Tetromino> tetrominos{
		Tetromino{
			{{point(-1,0),point(0,0),point(1,0),point(2,0)}},
			Color::Cyan,
			TetrominoType::I},
		Tetromino{
			{{point(-1,-1),point(-1,0),point(0,0),point(1,0)}},
			Color::Blue,
			TetrominoType::J},
		Tetromino{
			{{point(1,-1),point(-1,0),point(0,0),point(1,0)}},
			Color::Orange,
			TetrominoType::L},
		Tetromino{
			{{point(0,-1),point(1,-1),point(0,0),point(1,0)}},
			Color::Yellow,
			TetrominoType::O},
		Tetromino{
			{{point(0,-1),point(1,-1),point(-1,0),point(0,0)}},
			Color::Green,
			TetrominoType::S},
		Tetromino{
			{{point(0,-1),point(-1,0),point(0,0),point(1,0)}},
			Color::Magenta,
			TetrominoType::T},
		Tetromino{
			{{point(-1,-1),point(0,-1),point(0,0),point(1,0)}},
			Color::Red,
			TetrominoType::Z},
	};
	// Offsets applied when rotating tetrominoes based on the official "Super Rotation System"
	const std::vector<std::array<point, 5>> Offsets_JLSTZ{ {
	{point(0,0),point(0,0),point(0,0),point(0,0),point(0,0)},
	{point(0,0),point(-1,0),point(-1,1),point(0,-2),point(-1,-2)},
	{point(0,0),point(0,0),point(0,0),point(0,0),point(0,0)},
	{point(0,0),point(1,0),point(1,1),point(0,-2),point(1,-2)}
	} };
	const std::vector<std::array<point, 5>> Offsets_I{ {
	{point(0,0),point(-1,0),point(2,0),point(-1,0),point(2,0)},
	{point(-1,0),point(0,0),point(0,0),point(0,-1),point(0,+2)},
	{point(-1,-1),point(1,-1),point(-2,-1),point(1,0),point(-2,0)},
	{point(0,-1),point(0,-1),point(0,-1),point(0,-1),point(0,+2)}
	} };

	enum class Movement { Right, Left, Down, RotateR, RotateL };
	GLuint EmptyCellTextureID;

	uint8_t Keys[350] = { 0 };
	bool KeysProcessed[350]{ false };

	std::array<std::array<Color, NUM_OF_CELLS_H>, NUM_OF_CELLS_W> Map = { {Color::Gray} };
	Tetromino ActiveTetromino;
	float ts = 0.0f, inputTime = 0.0f, Delay = 1.0f;
	uint8_t NoMovement = 0;
	point PreviousState;

	bool MoveTetromino(Movement movement);
	bool HasCollied(Tetromino tetromino);
	void ProcessInput();
	Tetromino GenerateNewTetromino();
	glm::vec4 GetColor(Color color);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void s_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	// TESTING
	float x = 0.0f, y = 0.0f, sizeX = 0.5f, sizeY = 0.5f;
};
#endif
