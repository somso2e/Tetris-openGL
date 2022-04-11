#include "game.hpp"

Game::Game(GLFWwindow* window) {
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, s_key_callback);

	Renderer::Init2D();
	auto font = Renderer::InitText("res/fonts/arial.ttf");
	TextAtlas_.Create(font, 48);

	EmptyCellTextureID_ = Renderer::LoadTexture("res/textures/EmptyCell.png");
	GridTextureID_ = Renderer::LoadTexture("res/textures/Grid.png");

	Start();
}


void Game::Start() {
	// Initilize all class member variables
	// This function is again called when the game restarts

	memset(Keys_, GLFW_RELEASE, sizeof(Keys_));
	memset(KeysProcessed_, false, sizeof(KeysProcessed_));
	auto timeNow = glfwGetTime();

	Map_ = { {Color::Empty} };

	KeysRepeatTimer_.fill(timeNow);

	FallTimer_ = timeNow;
	LockDownTimer_ = timeNow;

	FallSpeed_ = 1.0;


	NextQueue_ = {};

	CanSwap_ = true;
	FirstSwap_ = true;
	HeldTetrominoType_ = {};


	GeneratedBag_ = { 0,1,2,3,4,5,6 };
	BagIndex_ = 7;

	Score = 0;
	Level = 1;
	TotalClearedLines_ = 0;

	for (auto& tetromino : NextQueue_) {
		tetromino = GenerateTetromino();
	}
	ActiveTetromino_ = GenerateTetromino();
	Previous_ = ActiveTetromino_;
}




void Game::Update() {
	auto timeNow = glfwGetTime();
	if (timeNow >= FallTimer_ + FallSpeed_) {
		if (!MoveTetromino(ActiveTetromino_, Movement::Down)) {
			if (timeNow >= LockDownTimer_ + LOCK_DOWN_TIMEOUT) {
				ActiveTetromino_.placed = true;
			}
		}
		FallTimer_ = timeNow;
	}
	if (Previous_.pos != ActiveTetromino_.pos ||
		Previous_.rotation != ActiveTetromino_.rotation) {
		LockDownTimer_ = timeNow;
	}
	Previous_ = ActiveTetromino_;


	// Check for full rows
	uint8_t clearedLines = 0;
	for (int j = 0; j < NUM_OF_CELLS_H; j++) {
		int occupiedCells = 0;
		for (int i = 0; i < NUM_OF_CELLS_W; i++) {
			if (Map_.at(i).at(j) != Color::Empty) {
				occupiedCells++;
			}
		}
		// shift rows down 
		if (occupiedCells == NUM_OF_CELLS_W) {
			clearedLines++;
			for (int j2 = j; 0 < j2; j2--) {
				for (int i = 0; i < NUM_OF_CELLS_W; i++) {
					Map_.at(i).at(j2) = Map_.at(i).at(j2 - 1);
				}
			}
		}
	}

	TotalClearedLines_ += clearedLines;
	Level = (uint32_t)TotalClearedLines_ / 10 + 1;
	if (Level > 25) {
		Level = 25;
	}
	FallSpeed_ = pow(0.8 - (Level - 1) * 0.007, Level - 1);
	static const uint32_t linesClearedScores[5] = { 0,40, 100, 300, 1200 };
	Score += Level * (linesClearedScores[clearedLines]);

	//if (timeNow >= ProcessInputTimer_ + 0.065) {
	ProcessInput();
	//	ProcessInputTimer_ = timeNow;
	//}



	// If previous tetromino was placed generate a new one
	if (ActiveTetromino_.placed) {
		CanSwap_ = true;
		// Permenantly add the Active Tetromino to the map
		for (auto const& piece : ActiveTetromino_.blocks) {
			point blocks = piece + ActiveTetromino_.pos;
			Map_.at(blocks.x).at(blocks.y) = ActiveTetromino_.color;
		}


		ActiveTetromino_ = NextQueue_.at(0);
		for (auto i = 0; i < 4; i++) {
			NextQueue_.at(i) = NextQueue_.at(i + 1);
		}
		NextQueue_.at(4) = GenerateTetromino();

		if (HasCollied(ActiveTetromino_)) {
			// GAME OVER
			std::cout << "GAME OVER" << std::endl;
			std::cin;
			Restart();
		}
	}

	auto displayMap = Map_;
	// Append the ghost of the active tetromino to a temporary map
	auto ghost = ActiveTetromino_;
	while (MoveTetromino(ghost, Movement::Down));

	for (const auto& block : ghost.blocks) {
		point pos = block + ghost.pos;
		displayMap.at(pos.x).at(pos.y) = Color::Gray;
	}
	// Append the active tetromino to a temporary map
	for (auto const& block : ActiveTetromino_.blocks) {
		point pos = block + ActiveTetromino_.pos;
		displayMap.at(pos.x).at(pos.y) = ActiveTetromino_.color;
	}

	// Generate the Map of Next queue
	std::array<std::array<Color, NEXT_TETROMINOES_COUNT * 3>, 4> nextPiecesMap{ {Color::Empty} };
	for (auto i = 0; i < NEXT_TETROMINOES_COUNT; i++) {
		for (const auto& block : NextQueue_.at(i).blocks) {
			point pos = block + point(1, 1 + 3 * i);
			nextPiecesMap.at(pos.x).at(pos.y) = NextQueue_.at(i).color;
		}
	}
	std::array<std::array<Color, 4>, 4> heldPiecesMap{ {Color::Empty} };
	Color heldColor = FirstSwap_ ? Color::Empty : TETROMINOES.at(HeldTetrominoType_).color;

	for (const auto& block : TETROMINOES.at(HeldTetrominoType_).blocks) {
		point pos = block + point(1, 1);
		heldPiecesMap.at(pos.x).at(pos.y) = heldColor;
	}

	// Render the screen
	Renderer::BeginBatch();

	RenderGrid(nextPiecesMap, NEXT_PIECE_POS);
	RenderGrid(displayMap, MAIN_MAP_POS);
	RenderGrid(heldPiecesMap, HELD_PIECE_POS);


	TextAtlas_.Draw("hi", glm::vec2(10.0f), glm::vec2(10.0f), GetColor(Color::Red));

	Renderer::EndBatch();
	Renderer::Flush();
}

template<std::size_t size_x, std::size_t size_y>
void Game::RenderGrid(std::array < std::array < Color, size_y>, size_x>& grid, const glm::vec2 offset) {
	for (int j = 0; j < grid.at(0).size(); j++) {
		for (int i = 0; i < grid.size(); i++) {
			if (grid.at(i).at(j) == Color::Empty) {
				Renderer::DrawQuad(
					glm::vec2(i * CELL_SIZE, j * CELL_SIZE) + offset,
					glm::vec2(CELL_SIZE),
					glm::vec4(1.0f),
					GridTextureID_);
			}
			else {
				Renderer::DrawQuad(
					glm::vec2(i * CELL_SIZE, j * CELL_SIZE) + offset,
					glm::vec2(CELL_SIZE),
					GetColor(grid.at(i).at(j)),
					EmptyCellTextureID_);
			}
		}
	}
}


Game::Tetromino Game::GenerateTetromino() {

	if (BagIndex_ == 7) {
		BagIndex_ = 0;
		std::mt19937  generator(static_cast<unsigned int>(time(NULL)));
		GeneratedBag_ = { 0,1,2,3,4,5,6 };
		std::shuffle(GeneratedBag_.begin(), GeneratedBag_.end(), generator);
	}
	return TETROMINOES.at(GeneratedBag_.at(BagIndex_++));
}


bool Game::MoveTetromino(Tetromino& tetromino, Movement movement) {
	// Offsets applied when rotating tetrominoes based on the official "Super Rotation System"
	// Offsets for J, L, S, T and Z Tetrominoes
	static const std::vector<std::array<point, 5>> OFFSETS_JLSTZ{ {
	{point(0,0),point(0,0),point(0,0),point(0,0),point(0,0)},			// Rotation state 0 
	{point(0,0),point(+1,0),point(+1,+1),point(0,-2),point(+1,-2)},		// Rotation state R 
	{point(0,0),point(0,0),point(0,0),point(0,0),point(0,0)},			// Rotation state 2 
	{point(0,0),point(-1,0),point(-1,1),point(0,-2),point(-1,-2)}		// Rotation state L 
	} };
	// Offsets for I Tetromino
	static const std::vector<std::array<point, 5>> OFFSETS_I{ {
   {point(0,0),point(-1,0),point(+2,0),point(-1,0),point(+2,0)},		// Rotation state 0 
   {point(-1,0),point(0,0),point(0,0),point(0,-1),point(0,+2)},			// Rotation state R 
   {point(-1,-1),point(+1,-1),point(-2,-1),point(+1,0),point(-2,0)},	// Rotation state 2 
   {point(0,-1),point(0,-1),point(0,-1),point(0,-1),point(0,+2)}		// Rotation state L 
   } };

	Tetromino original = tetromino;
	switch (movement) {
		using enum TetrominoType;
		using enum Movement;
	case Right:
		tetromino.pos.x++;
		break;

	case Left:
		tetromino.pos.x--;
		break;

	case Down:
		tetromino.pos.y++;
		break;
	default:
		if (tetromino.type != O) {
			switch (movement) {

			case RotateR:
				tetromino.rotation = (++tetromino.rotation % 4 + 4) % 4;
				for (auto& piece : tetromino.blocks) {
					piece = point(-piece.y, +piece.x);
				}
				break;
			case RotateL:
				tetromino.rotation = (--tetromino.rotation % 4 + 4) % 4;
				for (auto& piece : tetromino.blocks) {
					piece = point(+piece.y, -piece.x);
				}
				break;
			case Rotate2:
				tetromino.rotation = ((tetromino.rotation += 2) % 4 + 4) % 4;
				for (auto& piece : tetromino.blocks) {
					piece = point(-piece.x, -piece.y);
				}
				break;
			}
			auto& offsetArray = tetromino.type == I ? OFFSETS_I : OFFSETS_JLSTZ;
			for (int i = 0; i < 5; i++) {
				auto offset = offsetArray.at(original.rotation).at(i) - offsetArray.at(tetromino.rotation).at(i);
				tetromino.pos = original.pos + offset;
				//	Check for collision with each offset. 
				//	Return with the first offset that didn't collied
				auto collision = HasCollied(tetromino);
				if (!collision) {
					return true;
				}
			}
		}
	}

	auto collision = HasCollied(tetromino);
	if (collision) {
		tetromino = original;
	}

	return !collision;
}


bool Game::HasCollied(Tetromino tetromino) {
	for (auto const& piece : tetromino.blocks) {
		point blocks = piece + tetromino.pos;
		// If x and y coords are out of bound 
		// or if the cell in the map is a color other than gray AKA it's occupied 
		if (
			(blocks.x) >= NUM_OF_CELLS_W
			|| (blocks.y) >= NUM_OF_CELLS_H
			|| (blocks.x) < 0
			|| (blocks.y) < 0
			|| Map_.at(blocks.x).at(blocks.y) != Color::Empty) {
			return true;
		}
	}

	return false;

}

void Game::SwapTetromino() {
	if (CanSwap_) {
		if (FirstSwap_) {
			HeldTetrominoType_ = ActiveTetromino_.type;
			ActiveTetromino_ = GenerateTetromino();
			FirstSwap_ = false;
		}
		else {
			auto original = ActiveTetromino_.type;
			ActiveTetromino_ = TETROMINOES.at(HeldTetrominoType_);
			HeldTetrominoType_ = original;
			CanSwap_ = false;
		}
	}
}


void Game::Restart() {
	Start();
}

#define GET_ONCE(index) Keys_[index]==GLFW_PRESS && !KeysProcessed_[index]
#define GET_REPEAT(index) Keys_[index] != GLFW_RELEASE && KeysProcessed_[index] \
							&& timeNow >= KeysRepeatTimer_[index] + REPEAT_INTERVAL

void Game::ProcessInput() {
	static const double REPEAT_INTERVAL = 0.05;
	static const double REPEAT_DELAY = 0.15;
	auto timeNow = glfwGetTime();

	if (GET_ONCE(GLFW_KEY_DOWN)) {
		if (MoveTetromino(ActiveTetromino_, Movement::Down))
			Score += 1;
		KeysProcessed_[GLFW_KEY_DOWN] = true;
		KeysRepeatTimer_[GLFW_KEY_DOWN] = timeNow + REPEAT_DELAY;
	}
	else if (GET_REPEAT(GLFW_KEY_DOWN)) {
		if (MoveTetromino(ActiveTetromino_, Movement::Down))
			Score += 1;
		KeysRepeatTimer_[GLFW_KEY_DOWN] = timeNow;

	}

	if (GET_ONCE(GLFW_KEY_RIGHT)) {
		MoveTetromino(ActiveTetromino_, Movement::Right);
		KeysProcessed_[GLFW_KEY_RIGHT] = true;
		KeysRepeatTimer_[GLFW_KEY_RIGHT] = timeNow + REPEAT_DELAY;
	}
	else if (GET_REPEAT(GLFW_KEY_RIGHT)) {
		MoveTetromino(ActiveTetromino_, Movement::Right);
		KeysRepeatTimer_[GLFW_KEY_RIGHT] = timeNow;
	}


	if (GET_ONCE(GLFW_KEY_LEFT)) {
		MoveTetromino(ActiveTetromino_, Movement::Left);
		KeysProcessed_[GLFW_KEY_LEFT] = true;
		KeysRepeatTimer_[GLFW_KEY_LEFT] = timeNow + REPEAT_DELAY;
	}
	else if (GET_REPEAT(GLFW_KEY_LEFT)) {
		MoveTetromino(ActiveTetromino_, Movement::Left);
		KeysRepeatTimer_[GLFW_KEY_LEFT] = timeNow;
	}


	if (GET_ONCE(GLFW_KEY_X)) {
		MoveTetromino(ActiveTetromino_, Movement::RotateR);
		KeysProcessed_[GLFW_KEY_X] = true;
	}
	else if (GET_ONCE(GLFW_KEY_UP)) {
		MoveTetromino(ActiveTetromino_, Movement::RotateR);
		KeysProcessed_[GLFW_KEY_UP] = true;
	}
	else if (GET_ONCE(GLFW_KEY_Z)) {
		MoveTetromino(ActiveTetromino_, Movement::RotateL);
		KeysProcessed_[GLFW_KEY_Z] = true;
	}
	else if (GET_ONCE(GLFW_KEY_A)) {
		MoveTetromino(ActiveTetromino_, Movement::Rotate2);
		KeysProcessed_[GLFW_KEY_A] = true;
	}

	if (GET_ONCE(GLFW_KEY_SPACE)) {
		while (MoveTetromino(ActiveTetromino_, Movement::Down))
			Score += 2;
		ActiveTetromino_.placed = true;
		KeysProcessed_[GLFW_KEY_SPACE] = true;
	}

	if (GET_ONCE(GLFW_KEY_C)) {
		SwapTetromino();
		KeysProcessed_[GLFW_KEY_C] = true;
	}

	if (GET_ONCE(GLFW_KEY_R)) {
		Restart();
		KeysProcessed_[GLFW_KEY_R] = true;
	}

}


void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game* game = reinterpret_cast<Game*> (glfwGetWindowUserPointer(window));

	if (0 < key && key < 350) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		//std::cout << key << "," << action << std::endl;
		Keys_[key] = action;
		if (action == GLFW_RELEASE) {
			KeysProcessed_[key] = false;
		}
	}
}


void Game::s_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game* that = static_cast<Game*>(glfwGetWindowUserPointer(window));
	that->key_callback(window, key, scancode, action, mods);
}


glm::vec4 Game::GetColor(Color color) {
	using enum  Color;
	switch (color) {
	case Empty:
		return glm::vec4(0.0f);
	case Gray:
		return glm::vec4(0.65f, 0.65f, 0.65f, 1.0f);
	case Yellow:
		return glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	case Green:
		return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	case Orange:
		return glm::vec4(1.0f, 0.6f, 0.0f, 1.0f);
	case Cyan:
		return glm::vec4(0.4f, 1.0f, 0.8f, 1.0f);
	case Magenta:
		return glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	case Pink:
		return glm::vec4(1.0f, 0.4f, 0.7f, 1.0f);
	case Red:
		return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	case Blue:
		return glm::vec4(0.0f, 0.0f, 0.9f, 1.0f);
	case LightGray:
		return glm::vec4(0.3f, 0.35f, 0.35f, 1.0f);
	}
}