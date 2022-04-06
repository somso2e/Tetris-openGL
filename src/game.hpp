#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "renderer.hpp"


class Game {
public:
	void Init(GLFWwindow* window);
	~Game();
	void Update();
	uint32_t Score;
	uint32_t Level;
	GLFWwindow* window;
private:
	enum class Color { Gray, Yellow, Green, Orange, Cyan, Magenta, Pink, Red, Blue };
	enum TetrominoType { I, O, J, L, T, S, Z };

	typedef glm::vec<2, int> point;
	struct Tetromino {
		std::array<point, 4> coords;
		Color color;
		enum TetrominoType type;
		point origin;
		int rotation = 0;
		bool placed = false;
	};
	//
	const std::vector<Tetromino> tetrominoes{
		Tetromino{
			{{point(-1,0),point(0,0),point(1,0),point(2,0)}},
			Color::Cyan,
			TetrominoType::I,
			point(4, 1)},
		Tetromino{
			{{point(0,-1),point(1,-1),point(0,0),point(1,0)}},
			Color::Yellow,
			TetrominoType::O,
			point(4, 1)},
		Tetromino{
			{{point(-1,-1),point(-1,0),point(0,0),point(1,0)}},
			Color::Blue,
			TetrominoType::J,
			point(4, 1)},
		Tetromino{
			{{point(1,-1),point(-1,0),point(0,0),point(1,0)}},
			Color::Orange,
			TetrominoType::L,
			point(4, 1)},
		Tetromino{
			{{point(0,-1),point(-1,0),point(0,0),point(1,0)}},
			Color::Magenta,
			TetrominoType::T,
			point(4, 1)},
		Tetromino{
			{{point(0,-1),point(1,-1),point(-1,0),point(0,0)}},
			Color::Green,
			TetrominoType::S,
			point(4, 1)},
		Tetromino{
			{{point(-1,-1),point(0,-1),point(0,0),point(1,0)}},
			Color::Red,
			TetrominoType::Z,
			point(4, 1)},
	};

	// Offsets applied when rotating tetrominoes based on the official "Super Rotation System"
	// Offsets for J, L, S, T and Z Tetrominoes
	const std::vector<std::array<point, 5>> Offsets_JLSTZ{ {
	{point(0,0),point(0,0),point(0,0),point(0,0),point(0,0)},		// Rotation state 0 
	{point(0,0),point(-1,0),point(-1,1),point(0,-2),point(-1,-2)},	// Rotation state R 
	{point(0,0),point(0,0),point(0,0),point(0,0),point(0,0)},		// Rotation state 2 
	{point(0,0),point(1,0),point(1,1),point(0,-2),point(1,-2)}		// Rotation state L 
	} };
	// Offsets for I Tetromino
	const std::vector<std::array<point, 5>> Offsets_I{ {
	{point(0,0),point(-1,0),point(2,0),point(-1,0),point(2,0)},		// Rotation state 0 
	{point(-1,0),point(0,0),point(0,0),point(0,-1),point(0,+2)},	// Rotation state R 
	{point(-1,-1),point(1,-1),point(-2,-1),point(1,0),point(-2,0)},	// Rotation state 2 
	{point(0,-1),point(0,-1),point(0,-1),point(0,-1),point(0,+2)}	// Rotation state L 
	} };
	enum class Movement { Right, Left, Down, RotateR, RotateL };


	GLuint EmptyCellTextureID;

	uint8_t Keys[350];
	bool KeysProcessed[350];

	std::array<std::array<Color, NUM_OF_CELLS_H>, NUM_OF_CELLS_W> Map;
	Tetromino ActiveTetromino;
	float ts, Speed;
	uint8_t NoMovement;
	point PreviousState;

	bool CanSwap, FirstSwap;
	TetrominoType HeldTetrominoType;

	bool MoveTetromino(Tetromino& tetromino, Movement movement);
	bool HasCollied(Tetromino tetromino);
	void SwapTetromino();
	void RenderGhost();
	void Restart();
	void ProcessInput();
	Tetromino GenerateNewTetromino();
	glm::vec4 GetColor(Color color);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void s_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

};
#endif
