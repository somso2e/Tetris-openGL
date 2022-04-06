#include "game.hpp"

void Game::Init(GLFWwindow* window) {
	this->window = window;
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, s_key_callback);

	Renderer::Init();

	EmptyCellTextureID = Renderer::LoadTexture("res/textures/EmptyCell.png");

	ActiveTetromino = GenerateNewTetromino();

	Score = 0;
	Level = 1;

	memset(Keys, GLFW_RELEASE, sizeof(Keys));
	memset(KeysProcessed, false, sizeof(Keys));

	Map = { {Color::Gray} };
	ts = glfwGetTime();
	Speed = 1.0f;
	NoMovement = 0;
	PreviousState = point(0, 0);

	CanSwap = true, FirstSwap = true;
}

Game::~Game() {
	Renderer::Shutdown();
}

void Game::Update() {

	//auto timeNow = glfwGetTime();

	if (glfwGetTime() >= ts + Speed) {
		MoveTetromino(ActiveTetromino, Movement::Down);
		ts += Speed;
		if (ActiveTetromino.origin == PreviousState) {
			NoMovement++;
		}
		else {
			NoMovement = 0;
		}
		std::cout << "Score:" << Score << std::endl;
	}

	PreviousState = ActiveTetromino.origin;

	if (NoMovement == 3) {
		ActiveTetromino.placed = true;
		NoMovement = 0;
	}


	// Clear Lines
	uint8_t clearedLines = 0;
	for (int j = 0; j < NUM_OF_CELLS_H; j++) {
		int occupiedCells = 0;
		for (int i = 0; i < NUM_OF_CELLS_W; i++) {
			if (Map.at(i).at(j) != Color::Gray) {
				occupiedCells++;
			}
		}
		// Row is filled
		if (occupiedCells == NUM_OF_CELLS_W) {
			clearedLines++;
			for (int j2 = j; 0 < j2; j2--) {
				for (int i = 0; i < NUM_OF_CELLS_W; i++) {
					Map.at(i).at(j2) = Map.at(i).at(j2 - 1);
				}
			}

		}
	}
	uint32_t linesClearedScores[5] = { 0,40, 100, 300, 1200 };
	Score += Level * (linesClearedScores[clearedLines]);

	ProcessInput();
	// Append the active tetromino to a temporary map
	auto DisplayMap = Map;
	for (auto const& piece : ActiveTetromino.coords) {
		point coords = piece + ActiveTetromino.origin;
		DisplayMap.at(coords.x).at(coords.y) = ActiveTetromino.color;
	}

	// If previous tetromino was placed generate a new one
	if (ActiveTetromino.placed) {
		CanSwap = true;
		// Permenantly add the Active Tetromino to the map
		for (auto const& piece : ActiveTetromino.coords) {
			point coords = piece + ActiveTetromino.origin;
			Map.at(coords.x).at(coords.y) = ActiveTetromino.color;
		}

		ActiveTetromino = GenerateNewTetromino();

		if (HasCollied(ActiveTetromino)) {
			// GAME OVER
			std::cout << "GAME OVER" << std::endl;
			std::cin;
			Restart();
		}
	}
	// Render the screen
	Renderer::BeginBatch();
	for (int j = 0; j < NUM_OF_CELLS_H; j++) {
		for (int i = 0; i < NUM_OF_CELLS_W; i++) {
			Renderer::DrawQuad(
				glm::vec2((float)i, (float)j),
				glm::vec2(1.0f, 1.0f),
				Game::GetColor(DisplayMap.at(i).at(j)),
				EmptyCellTextureID);
		}
	}
	RenderGhost();
	Renderer::EndBatch();
	Renderer::Flush();
}


Game::Tetromino Game::GenerateNewTetromino() {
	std::default_random_engine generator(static_cast<unsigned int>(time(NULL)));
	// TODO: check if this is correct????
	std::uniform_int_distribution<uint32_t> distribution(0, 7 - 1);
	auto seed = distribution(generator);

	Tetromino generatedTetromino = tetrominoes.at(seed);
	return generatedTetromino;
}


bool Game::MoveTetromino(Tetromino& tetromino, Movement movement) {
	Tetromino original = tetromino;
	switch (movement) {
		using enum TetrominoType;
		using enum Movement;
	case Right:
		tetromino.origin.x++;
		break;

	case Left:
		tetromino.origin.x--;
		break;

	case Down:
		tetromino.origin.y++;
		break;

	case RotateR:
	case RotateL:
		if (tetromino.type != O) {

			if (movement == RotateR) {
				tetromino.rotation = (++tetromino.rotation % 4 + 4) % 4;

				for (auto& piece : tetromino.coords) {
					piece = point(-piece.y, +piece.x);
				}
			}
			else if (movement == RotateL) {
				tetromino.rotation = (--tetromino.rotation % 4 + 4) % 4;

				for (auto& piece : tetromino.coords) {
					piece = point(+piece.y, -piece.x);
				}
			}

			auto& offsetArray = tetromino.type == I ? Offsets_I : Offsets_JLSTZ;
			for (int i = 0; i < 5; i++) {
				auto offset = offsetArray.at(original.rotation).at(i) - offsetArray.at(tetromino.rotation).at(i);
				tetromino.origin = original.origin + offset;
				//	Check for collision with each offset. 
				//	Return with the first offset that didn't collied
				auto collision = HasCollied(tetromino);
				if (!collision) {
					return false;
				}
			}
		}
		break;
	}

	auto collision = HasCollied(tetromino);
	if (collision) {
		tetromino = original;
	}

	return collision;
}


bool Game::HasCollied(Tetromino tetromino) {
	for (auto const& piece : tetromino.coords) {
		point coords = piece + tetromino.origin;
		// If x and y coords are out of bound 
		// or if the cell in the map is a color other than gray AKA it's occupied 
		if (
			(coords.x) >= NUM_OF_CELLS_W
			|| (coords.y) >= NUM_OF_CELLS_H
			|| (coords.x) < 0
			|| (coords.y) < 0
			|| Map.at(coords.x).at(coords.y) != Color::Gray) {
			return true;
		}
	}

	return false;

}

void Game::SwapTetromino() {
	if (CanSwap) {
		if (FirstSwap) {
			HeldTetrominoType = ActiveTetromino.type;
			ActiveTetromino = GenerateNewTetromino();
			FirstSwap = false;
		}
		else {
			auto original = ActiveTetromino.type;
			ActiveTetromino = tetrominoes.at(HeldTetrominoType);
			HeldTetrominoType = original;
			CanSwap = false;
		}
	}
}

void Game::RenderGhost() {
	auto ghost = ActiveTetromino;
	while (!MoveTetromino(ghost, Movement::Down));
	for (const auto& piece : ghost.coords) {
		Renderer::DrawQuad(
			glm::vec2((float)(piece.x + ghost.origin.x), (float)(piece.y + ghost.origin.y)),
			glm::vec2(1.0f, 1.0f),
			GetColor(ghost.color),
			EmptyCellTextureID);

	}
}

void Game::Restart() {
	Init(this->window);
}



#define CHECK_KEYS(index) ((Keys[index] == GLFW_PRESS || Keys[index] == GLFW_REPEAT) && !KeysProcessed[index]) 
void Game::ProcessInput() {

	if CHECK_KEYS(GLFW_KEY_RIGHT) {
		MoveTetromino(ActiveTetromino, Movement::Right);
		KeysProcessed[GLFW_KEY_RIGHT] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_LEFT) {
		MoveTetromino(ActiveTetromino, Movement::Left);
		KeysProcessed[GLFW_KEY_LEFT] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_DOWN) {
		MoveTetromino(ActiveTetromino, Movement::Down);
		Score += 1;
		KeysProcessed[GLFW_KEY_DOWN] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_UP) {
		MoveTetromino(ActiveTetromino, Movement::RotateR);
		KeysProcessed[GLFW_KEY_UP] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_W) {
		MoveTetromino(ActiveTetromino, Movement::RotateL);
		KeysProcessed[GLFW_KEY_W] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_SPACE) {
		while (!MoveTetromino(ActiveTetromino, Movement::Down)) {
			Score += 2;
		}
		ActiveTetromino.placed = true;
		KeysProcessed[GLFW_KEY_SPACE] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_C) {
		SwapTetromino();
		KeysProcessed[GLFW_KEY_C] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_R) {
		Restart();
		KeysProcessed[GLFW_KEY_R] = true;
	}


}


void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game* game = reinterpret_cast<Game*> (glfwGetWindowUserPointer(window));

	if (0 < key && key < 350) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		Keys[key] = action;
		if (action == GLFW_RELEASE || action == GLFW_REPEAT) {
			KeysProcessed[key] = false;
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
	}
}