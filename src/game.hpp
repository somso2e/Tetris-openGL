#ifndef GAME_HPP
#define GAME_HPP

#include <random>
#include "cell.hpp"
#include <chrono>
#include <thread>
enum class TetrimoType {
	I, O, J, L, T, S, Z
};

struct Tetrimo {
	std::array <std::array<bool, 4>, 4>  shape = { {0} };
	Color color = Color::Gray;
	enum TetrimoType type;
	int x = 0;
	int y = 0;
	int rotation = 0;
	bool placed = 0;
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
	GLFWwindow* window;


	// ?????
	bool KeysProcessed[350] = { true };

	Game(GLFWwindow* window);
	~Game();
	void Update();
private:
	Shader shader;
	Texture EmptyCellTexture;
	Cell m_cell;
	bool Keys[350]{ false };
	
	std::array<std::array<Color, NUM_OF_CELLS_H>, NUM_OF_CELLS_W> Map = { {Color::Gray} };
	
	Tetrimo ActiveTetrimo;
	
	void MoveTetrimo(Tetrimo tetrimo, Movement movement);
	bool HasCollied(Tetrimo tetrimo);

	Tetrimo GenerateNewTetrimo();
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void s_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void ProcessInput();

};

const std::vector<Tetrimo> tetrimos{
	//I
	Tetrimo{{{
		{0, 0, 0, 0},
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}}}

		,Color::Cyan, TetrimoType::I},
	// O
	Tetrimo{{{
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Yellow, TetrimoType::O},
	// J
	Tetrimo{{{
		{0, 0, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Magenta, TetrimoType::J},
	// L
	Tetrimo{{{
		{0, 0, 0, 0},
		{0, 0, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Orange, TetrimoType::L},
	// T
	Tetrimo{{{
		{0, 0, 0, 0},
		{0, 1, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Green, TetrimoType::T},
	// S
	Tetrimo{{{
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0}}}

		,Color::Pink, TetrimoType::S},

	// Z
	Tetrimo{{{
		{0, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0}}}

		,Color::Red, TetrimoType::Z},
};

#endif
