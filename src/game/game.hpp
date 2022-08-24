#pragma once

#include <random>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/renderer.hpp"
#include "settings.hpp"

class Game {
public:
	void Init();
	void Update();

	uint8_t Keys_[350]{};
	bool KeysProcessed_[350]{};

	//private:
	Renderer::Text SmallText_, BigText_;

	enum class GameState {
		MainMenu,
		GameModeMenu,
		Active,
		Pause,
		GameOver,
		YouWon,
		CountDown,
		HotkeyMenu,
		EditingHotkey,
		CatchedHotkey,
		About,
		HighScoreMenu,
	};
	GameState State_, PreviousState_;
	enum class GameMode { Classic, FourtyLines };
	GameMode GameMode_;
	// Sizes and compenents locations
	static const unsigned int NUM_OF_CELLS_W = 10;
	static const unsigned int NUM_OF_CELLS_H = 20;

	static const unsigned int NEXT_TETROMINOES_COUNT = 5;

	float CellSize_ = (float)windowHeight / NUM_OF_CELLS_H;

	const glm::vec2 MainMapCoords_ = glm::vec2((windowWidth - CellSize_ * NUM_OF_CELLS_W) / 2, 0.0f);
	const glm::vec2 HeldPieceCoords_ = glm::vec2((MainMapCoords_.x - 4 * CellSize_) / 2, (windowHeight - NEXT_TETROMINOES_COUNT * 3 * CellSize_) / 2);
	const glm::vec2 ScoreWindowCoords_ = glm::vec2(HeldPieceCoords_.x, windowHeight - HeldPieceCoords_.y - 4 * CellSize_);
	const glm::vec2 NextPieceCoords_ = glm::vec2(HeldPieceCoords_.x + MainMapCoords_.x + NUM_OF_CELLS_W * CellSize_, (windowHeight - NEXT_TETROMINOES_COUNT * 3 * CellSize_) / 2);

	const float PauseButtonSize_ = (float)windowHeight / 8;
	const glm::vec2 PauseButtonCoords_ = glm::vec2(windowWidth / 2, windowHeight / 2) - glm::vec2(PauseButtonSize_ / 2, PauseButtonSize_);

	static const auto MAX_VISIBLE_SETTINGS_ = 12;
	const glm::vec2 SettingsBoxSize_ = glm::vec2(windowHeight / MAX_VISIBLE_SETTINGS_ * 7, windowHeight / MAX_VISIBLE_SETTINGS_);
	const glm::vec2 SettingsBoxCoords_ = glm::vec2(windowWidth / 2 - SettingsBoxSize_.x, 0.0f);

	const float DefaultFontSize_ = windowHeight / 20.0f;
	// Textures
	GLuint EmptyCellTextureID_, GridTextureID_, PauseButtonTextureID_, ArrowUpTextureID_, ArrowDownTextureID_;


	enum class Color {
		Empty, White, Gray, Yellow, Green, Orange, Cyan, Magenta, Pink, Red, Blue, Gray1, Gray2, Gray3
	};
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

	//Maps
	std::array<std::array<Color, NUM_OF_CELLS_H>, NUM_OF_CELLS_W> Map_{};
	std::array<std::array<Color, NUM_OF_CELLS_H>, NUM_OF_CELLS_W> DisplayMap_;
	std::array<std::array<Color, NEXT_TETROMINOES_COUNT * 3>, 4> NextPiecesMap_;
	std::array<std::array<Color, 4>, 4> HeldPieceMap_;


	Tetromino ActiveTetromino_{};
	Tetromino PreviousTetromino_{};
	std::array<Tetromino, NEXT_TETROMINOES_COUNT> NextQueue_{};

	std::array<int, 7> GeneratedBag_;
	unsigned int BagIndex_;


	bool CanSwap_, FirstSwap_;
	TetrominoType HeldTetrominoType_;

	// Timers
	double FallTimer_;
	double LockDownTimer_;
	double FallSpeed_{};
	const double LOCK_DOWN_TIMEOUT = 0.5;

	double ElapsedTime_;

	uint32_t Score_;
	uint32_t Level_;
	uint32_t TotalLinesCleared_;


	unsigned int CountDownInd_;
	double CountDownTimer_;

	int HighlightedSettingsInd_;
	int InputCustomHotkey_;
	int TopModuleind_;
	bool InvalidCustomHotkey_;
	bool MenuHighlighted_;
	std::vector<Settings::Module> DisplayHotkeys_;

	void InitMembers();

	std::array<double, 350> KeysRepeatTimer_{};
public:
	bool IsKeyPressed(int key);
	bool IsKeyHeld(int key);
	void ProcessInput();
	std::string GetTime(double elapsedTime);

	template<std::size_t size_x, std::size_t size_y>
	void DrawGrid(std::array < std::array < Color, size_y>, size_x>& grid, const glm::vec2 offset);
	void Render();
	void RenderScoreBox();
	void RenderPause();
	void RenderYouWon();
	void RenderGameOver();
	void RenderCountDown();

	void RenderHotkeysMenu();
	void DrawHighlightSelection(const glm::vec2& position, const glm::vec2& size, const float& thickness, const glm::vec4& color);

	void RenderGameModeSelect();
	void RenderMainMenu();
	void RenderAbout();
	void RenderRecordsMenu();

	bool HasCollied(Tetromino tetromino);
	Tetromino GenerateTetromino();
	enum class Movement { Right, Left, Down, RotateR, RotateL, Rotate2 };
	bool MoveTetromino(Tetromino& tetromino, Movement movement);
	void SwapTetromino();

	void Restart();

	void UpdateHighScore();
	glm::vec4 GetColor(Color color);
	const char* GetKeyName(int key);;
};
/*
	Menu MainMenu_ = Menu(&MainMenu_, std::bind(&Game::Render, this), []() {});
	MainMenu_.AddSubMenu("PLAY", std::bind(&Game::RenderGameModeSelect, this));
	MainMenu_.AddSubMenu("SETTINGS", std::bind(&Game::RenderHotkeysMenu, this), [&]() {
		SelectedMenu_ = &MainMenu_.Children_.at("SETTINGS");
		});
	MainMenu_.AddSubMenu("ABOUT", std::bind(&Game::RenderAbout, this));
	auto& settings_M = MainMenu_.Children_.at("SETTINGS");
	for (const auto& hotkey : Settings::Hotkeys) {
		settings_M->AddItem(hotkey.Name, [&]() {State_ == GameState::EditingHotkey; });
	}
	settings_M->AddItem("RESET TO DEFAULT", [&]() {DisplayHotkeys_ = Settings::DefaultHotkeys; });
	auto& about_M = MainMenu_.Children_.at("ABOUT");
	about_M->AddItem("BACK", [&]() {SelectedMenu_ = &MainMenu_; })
	
class Menu : public Game {
public:
	Menu(Menu* parent, std::function<void()>(render), std::function<void()>(action)) :
		Render(render), Action(action) {};

	std::function<void()>(Render);
	std::function<void()>(Action);
	std::map<std::string, Menu*> Children_;

	void AddSubMenu(const char* item, std::function<void()>(render), std::function<void()>(action) = []() {}) {
		Menu* child = new Menu(this, action, render);
		Children_[item] = child;
	}
	void AddItem(const char* item, std::function<void()>(action)) {
		Menu* child = new Menu(this, action, []() {});
		Children_[item] = child;
	}
}; 
*/