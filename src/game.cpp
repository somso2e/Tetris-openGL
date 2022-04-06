#include "game.hpp"

Game::Game(GLFWwindow* window) {
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, s_key_callback);

	Renderer::Init();

	EmptyCellTextureID = Renderer::LoadTexture("res/textures/EmptyCell.png");

	ActiveTetromino = GenerateNewTetromino();
}

Game::~Game() {
	Renderer::Shutdown();
}

void Game::Update() {

	//auto timeNow = glfwGetTime();

	if (glfwGetTime() >= ts + Delay) {
		MoveTetromino(Movement::Down);
		ts += Delay;
		if (ActiveTetromino.origin == PreviousState) {
			NoMovement;
		}
		else {
			NoMovement = 0;
		}
		PreviousState = ActiveTetromino.origin;
	}

	for (int j = 0; j < NUM_OF_CELLS_H; j++) {
		int occupiedCells = 0;
		for (int i = 0; i < NUM_OF_CELLS_W; i++) {
			if (Map.at(i).at(j) != Color::Gray) {
				occupiedCells++;
			}
		}
		// Row is filled
		if (occupiedCells == NUM_OF_CELLS_W) {
			for (int j2 = j; 0 < j2; j2--) {
				for (int i = 0; i < NUM_OF_CELLS_W; i++) {
					Map.at(i).at(j2) = Map.at(i).at(j2 - 1);
				}
			}

		}
	}

	ProcessInput();

	auto DisplayMap = Map;
	for (auto const& piece : ActiveTetromino.coords) {
		point coords = piece + ActiveTetromino.origin;
		DisplayMap.at(coords.x).at(coords.y) = ActiveTetromino.color;
	}

	if (NoMovement == 3) {
		ActiveTetromino.placed = true;
		NoMovement = 0;
	}

	// If previous tetromino was placed generate a new one
	if (ActiveTetromino.placed) {
		for (auto const& piece : ActiveTetromino.coords) {
			point coords = piece + ActiveTetromino.origin;
			Map.at(coords.x).at(coords.y) = ActiveTetromino.color;
		}

		ActiveTetromino = GenerateNewTetromino();

		if (HasCollied(ActiveTetromino)) {
			// GAME OVER
			std::cout << "GAME OVER" << std::endl;
			while (1);
		}
	}




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

	Renderer::EndBatch();
	Renderer::Flush();
}


Game::Tetromino Game::GenerateNewTetromino() {
	std::default_random_engine generator(static_cast<unsigned int>(time(NULL)));
	// TODO: check if this is correct????
	std::uniform_int_distribution<uint32_t> distribution(0, 7 - 1);
	auto seed = distribution(generator);

	Tetromino generatedTetromino = tetrominos.at(seed);
	return generatedTetromino;
}


bool Game::MoveTetromino(Movement movement) {
	Tetromino originalTetromino = ActiveTetromino;
	switch (movement) {
		using enum TetrominoType;
		using enum Movement;
	case Right:
		ActiveTetromino.origin.x++;
		break;

	case Left:
		ActiveTetromino.origin.x--;
		break;

	case Down:
		ActiveTetromino.origin.y++;
		break;

	case RotateR:
	case RotateL:
		if (ActiveTetromino.type != O) {
			auto tempTetromino = ActiveTetromino;
			if (movement == RotateR) {
				tempTetromino.rotation = (++tempTetromino.rotation % 4 + 4) % 4;

				for (auto& piece : tempTetromino.coords) {
					piece = point(-piece.y, +piece.x);
				}
			}
			else if (movement == RotateL) {
				tempTetromino.rotation = (--tempTetromino.rotation % 4 + 4) % 4;

				for (auto& piece : tempTetromino.coords) {
					piece = point(+piece.y, -piece.x);
				}
			}

			auto& offsetArray = tempTetromino.type == I ? Offsets_I : Offsets_JLSTZ;
			for (int i = 0; i < 5; i++) {
				auto offset = offsetArray.at(ActiveTetromino.rotation).at(i) - offsetArray.at(tempTetromino.rotation).at(i);
				tempTetromino.origin = ActiveTetromino.origin + offset;
				if (!HasCollied(tempTetromino)) {
					ActiveTetromino = tempTetromino;
					break;
				}
			}
		}
		break;
	}
	auto collition = HasCollied(ActiveTetromino);
	if (collition) {
		ActiveTetromino = originalTetromino;
	}
	return collition;
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

#define CHECK_KEYS(index) ((Keys[index] == GLFW_PRESS || Keys[index] == GLFW_REPEAT) && !KeysProcessed[index]) 
void Game::ProcessInput() {

	if CHECK_KEYS(GLFW_KEY_RIGHT) {
		MoveTetromino(Movement::Right);
		KeysProcessed[GLFW_KEY_RIGHT] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_LEFT) {
		MoveTetromino(Movement::Left);
		KeysProcessed[GLFW_KEY_LEFT] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_DOWN) {
		MoveTetromino(Movement::Down);
		KeysProcessed[GLFW_KEY_DOWN] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_UP) {
		MoveTetromino(Movement::RotateR);
		KeysProcessed[GLFW_KEY_UP] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_W) {
		MoveTetromino(Movement::RotateL);
		KeysProcessed[GLFW_KEY_W] = true;
	}
	else if CHECK_KEYS(GLFW_KEY_SPACE) {
		while (!MoveTetromino(Movement::Down)) {
		}
		ActiveTetromino.placed = true;
		KeysProcessed[GLFW_KEY_SPACE] = true;

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