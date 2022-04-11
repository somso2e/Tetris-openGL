#pragma once

#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "renderer.hpp"


class Game {
public:
	Game(GLFWwindow* window);
	void Update();
private:
	enum class Color { Empty, Gray, Yellow, Green, Orange, Cyan, Magenta, Pink, Red, Blue, LightGray };
	enum TetrominoType { I, O, J, L, T, S, Z };

	typedef glm::vec<2, int> point;
	struct Tetromino {
		std::array<point, 4> blocks;
		Color color;
		enum TetrominoType type;
		point pos;
		int rotation = 0;
		bool placed = false;
	};
	const std::vector<Tetromino> TETROMINOES{
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


	static const unsigned int NUM_OF_CELLS_W = 10;
	static const unsigned int NUM_OF_CELLS_H = 20;

	const float CELL_SIZE = WINDOW_HEIGHT / NUM_OF_CELLS_H;

	static const unsigned int NEXT_TETROMINOES_COUNT = 5;

	const glm::vec2 MAIN_MAP_POS = glm::vec2((WINDOW_WIDTH - CELL_SIZE * NUM_OF_CELLS_W) / 2, 0.0f);
	const glm::vec2 NEXT_PIECE_POS = glm::vec2(WINDOW_WIDTH * 3 / 4 + CELL_SIZE * (NUM_OF_CELLS_W / 4 - 2), (WINDOW_HEIGHT - NEXT_TETROMINOES_COUNT * 3 * CELL_SIZE) / 2);
	const glm::vec2 HELD_PIECE_POS = glm::vec2((MAIN_MAP_POS.x - 4 * CELL_SIZE) / 2, (WINDOW_HEIGHT - NEXT_TETROMINOES_COUNT * 3 * CELL_SIZE) / 2);


	GLuint EmptyCellTextureID_, GridTextureID_;

	uint8_t Keys_[350]{ };
	bool KeysProcessed_[350]{}; 
	std::array<double, 350> KeysRepeatTimer_{};



	std::array<std::array<Color, NUM_OF_CELLS_H>, NUM_OF_CELLS_W> Map_{};
	Tetromino ActiveTetromino_{};
	double FallTimer_;
	double LockDownTimer_;
	double FallSpeed_{};
	const double LOCK_DOWN_TIMEOUT = 0.5;

	Tetromino Previous_{};

	std::array<Tetromino, NEXT_TETROMINOES_COUNT> NextQueue_{};

	bool CanSwap_, FirstSwap_;
	TetrominoType HeldTetrominoType_;

	std::array<int, 7> GeneratedBag_;
	unsigned int BagIndex_;

	uint32_t Score;
	uint32_t Level;
	uint32_t TotalClearedLines_;

	Renderer::Atlas TextAtlas_;

	void Start();

	enum class Movement { Right, Left, Down, RotateR, RotateL, Rotate2 };
	bool MoveTetromino(Tetromino& tetromino, Movement movement);

	Tetromino GenerateTetromino();
	bool HasCollied(Tetromino tetromino);
	template<std::size_t size_x, std::size_t size_y>
	void RenderGrid(std::array < std::array < Color, size_y>, size_x>& grid, const glm::vec2 offset);

	void SwapTetromino();
	void Restart();
	void ProcessInput();
	glm::vec4 GetColor(Color color);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void s_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

};
