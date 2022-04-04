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


	if (glfwGetTime() >= ts + Delay) {
		MoveTetromino(Movement::Down);
		ts += Delay;
	}
	//if (glfwGetTime() >= inputTime + 0.1f) {
	//	ProcessInput();
	//	inputTime += 0.1f;
	//}

	ProcessInput();

	//std::cout << CurrentFrame << ' ' << LastFrame << std::endl;
	// If previous tetromino was placed generate a new one
	if (ActiveTetromino.placed) {
		ActiveTetromino = GenerateNewTetromino();

		if (HasCollied(ActiveTetromino)) {
			// GAME OVER
			std::cin;
		}
	}


	auto DisplayMap = Map;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (ActiveTetromino.shape.at(i).at(j)) {
				DisplayMap.at(ActiveTetromino.x + i).at(ActiveTetromino.y + j) = ActiveTetromino.color;
			}
		}
	}



	Renderer::BeginBatch();
	for (int j = 0; j < NUM_OF_CELLS_H; j++) {
		for (int i = 0; i < NUM_OF_CELLS_W; i++) {
			//std::cout << pos[0] << " " << pos[1] << std::endl;
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


Tetromino Game::GenerateNewTetromino() {
	std::default_random_engine generator(static_cast<unsigned int>(time(NULL)));
	// TODO: check if this is correct????
	std::uniform_int_distribution<uint32_t> distribution(0, NUM_OF_TETRIMOS - 1);
	auto seed = distribution(generator);

	Tetromino generatedTetromino = tetrominos.at(seed);
	return generatedTetromino;
}


void Game::MoveTetromino(Movement movement) {
	Tetromino originalTetromino = ActiveTetromino;
	switch (movement) {
		using enum TetrominoType;

	case Movement::Right:
		ActiveTetromino.x++;
		break;

	case Movement::Left:
		ActiveTetromino.x--;
		break;

	case Movement::Down:
		ActiveTetromino.y++;
		break;
	case Movement::RotateR:
	case Movement::RotateL:
		if (movement == Movement::RotateR) {
			ActiveTetromino.rotation = ++ActiveTetromino.rotation % 4;
		}
		else if (movement == Movement::RotateL) {
			ActiveTetromino.rotation = --ActiveTetromino.rotation % 4;

		}

		switch (ActiveTetromino.type) {
		case I:
			switch (ActiveTetromino.rotation) {
			case 0:
				ActiveTetromino.shape = { {
					{0, 0, 0, 0},
					{1, 1, 1, 1},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 1:
				ActiveTetromino.shape = { {
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0} } };
				break;
			case 2:
				ActiveTetromino.shape = { {
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{1, 1, 1, 1},
					{0, 0, 0, 0} } };
				break;
			case 3:
				ActiveTetromino.shape = { {
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0} } };
				break;
			}
			break;
			/////////////
		case O:
			// Do nothing
			break;
			/////////////
		case J:
			switch (ActiveTetromino.rotation) {
			case 0:
				ActiveTetromino.shape = { {
					{0, 0, 0, 0},
					{1, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				ActiveTetromino.shape = { {
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				ActiveTetromino.shape = { {
					{1, 0, 0, 0},
					{1, 1, 1, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				ActiveTetromino.shape = { {
					{0, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
			/////////////
		case L:
			switch (ActiveTetromino.rotation) {
			case 0:
				ActiveTetromino.shape = { {
					{0, 0, 0, 0},
					{0, 0, 1, 0},
					{1, 1, 1, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				ActiveTetromino.shape = { {
					{1, 0, 0, 0},
					{1, 0, 0, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				ActiveTetromino.shape = { {
					{1, 1, 1, 0},
					{1, 0, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				ActiveTetromino.shape = { {
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
			/////////////
		case T:
			switch (ActiveTetromino.rotation) {
			case 0:
				ActiveTetromino.shape = { {
					{0, 0, 0, 0},
					{0, 1, 0, 0},
					{1, 1, 1, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				ActiveTetromino.shape = { {
					{1, 0, 0, 0},
					{1, 1, 0, 0},
					{1, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				ActiveTetromino.shape = { {
					{1, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				ActiveTetromino.shape = { {
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
			/////////////
		case S:
			switch (ActiveTetromino.rotation) {
			case 0:
				ActiveTetromino.shape = { {
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				ActiveTetromino.shape = { {
					{1, 0, 0, 0},
					{1, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				ActiveTetromino.shape = { {
					{0, 1, 1, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				ActiveTetromino.shape = { {
					{0, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
			/////////////
		case Z:
			switch (ActiveTetromino.rotation) {
			case 0:
				ActiveTetromino.shape = { {
					{0, 0, 0, 0},
					{1, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				ActiveTetromino.shape = { {
					{0, 1, 0, 0},
					{1, 1, 0, 0},
					{1, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				ActiveTetromino.shape = { {
					{1, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				ActiveTetromino.shape = { {
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
		}
	}
	if (HasCollied(ActiveTetromino)) {
		ActiveTetromino = originalTetromino;
	}
}


bool Game::HasCollied(Tetromino tetromino) {
	for (auto i = 0; i < 4; i++) {
		for (auto j = 0; j < 4; j++) {
			if (tetromino.shape[i][j] == true) {
				// If x and y coords are out of bound 
				// or if the cell in the map is a color other than gray AKA it's occupied 
				if (
					(tetromino.x + i) >= NUM_OF_CELLS_W
					|| (tetromino.y + j) >= NUM_OF_CELLS_H
					|| (tetromino.x + i) < 0
					|| (tetromino.y + j) < 0
					|| Map.at(tetromino.x + i).at(tetromino.y + j) != Color::Gray) {
					return true;
				}

			}

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
	else if (KeysPressed[GLFW_KEY_W]) {
		MoveTetromino(Movement::RotateL);
	}
	else if (KeysPressed[GLFW_KEY_S]) {
		MoveTetromino(Movement::RotateL);
	}
	else if (KeysPressed[GLFW_KEY_D]) {
		MoveTetromino(Movement::RotateL);
	}
	else if (KeysPressed[GLFW_KEY_A]) {
		MoveTetromino(Movement::RotateL);
	}

}



void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game* game = reinterpret_cast<Game*> (glfwGetWindowUserPointer(window));

	if (0 < key && key < 350) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		Keys[key] = action;

		if (action == GLFW_PRESS) {

			KeysPressed[key] = true;
			KeysHeld[key] = false;
			KeysProcessed[key] = true;
			std::cout << "press" << std::endl;
		}
		else if (action == GLFW_RELEASE) {
			KeysPressed[key] = false;
			KeysProcessed[key] = false;
			KeysHeld[key] = false;
			std::cout << "release" << std::endl;
		}
		else if (action == GLFW_REPEAT) {
			KeysProcessed[key] = false;
			KeysPressed[key] = false;
			KeysHeld[key] = true;
			std::cout << "repeat" << std::endl;
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
		return glm::vec4(0.55f, 0.55f, 0.55f, 1.0f);
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
	}
}