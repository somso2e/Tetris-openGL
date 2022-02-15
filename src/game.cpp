#include "game.hpp"

Game::Game(GLFWwindow* window) {
	this->window = window;
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, Game::s_key_callback);

	this->shader.Compile("res/shaders/EmptyCell.vertex", "res/shaders/EmptyCell.fragment");

	const glm::mat4 projection = glm::ortho(0.0f, static_cast<float> (WINDOW_WIDTH),
		static_cast<float> (WINDOW_HEIGHT), 0.0f, 0.0f, 1.0f);
	this->shader.SetMatrix4("projection", projection);

	//Texture EmptyCellTexture;
	this->EmptyCellTexture.Generate("res/textures/EmptyCell.png");
	EmptyCellTexture.texUnit(this->shader, "tex0", 0);

	m_cell.Init(this->shader, this->EmptyCellTexture);


	this->ActiveTetrimo = GenerateNewTetrimo();

}

Game::~Game() {
	this->EmptyCellTexture.Delete();
	this->shader.Delete();

}

void Game::Update() {
	// If previous tetrimo was placed generate a new one
	if (this->ActiveTetrimo.placed) {
		this->ActiveTetrimo = GenerateNewTetrimo();

		if (HasCollied(this->ActiveTetrimo)) {
			// GAME OVER
			std::cin;
		}
	}
	Game::ProcessInput();

	auto DisplayMap = this->Map;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (this->ActiveTetrimo.shape.at(i).at(j)) {
				DisplayMap.at(this->ActiveTetrimo.x + i).at(this->ActiveTetrimo.y + j) = this->ActiveTetrimo.color;
			}
		}
	}
	std::cout << ActiveTetrimo.x << std::endl;

	for (int j = 0; j < 20; j++) {
		for (int i = 0; i < 10; i++) {
			glm::vec2 pos(CELL_WIDTH * i, CELL_HEIGHT * j);;
			m_cell.Render(pos, DisplayMap.at(i).at(j), glm::vec2(CELL_WIDTH, CELL_HEIGHT));

		}
	}
}


Tetrimo Game::GenerateNewTetrimo() {
	std::default_random_engine generator(time(NULL));
	// TODO: check if this is correct????
	std::uniform_int_distribution<uint32_t> distribution(0, NUM_OF_TETRIMOS - 1);
	auto seed = distribution(generator);

	Tetrimo generatedTetrimo = tetrimos.at(seed);
	return generatedTetrimo;
}


void Game::MoveTetrimo(Tetrimo tetrimo, Movement movement) {
	Tetrimo originalTetrimo = tetrimo;
	switch (movement) {
		using enum TetrimoType;

		break;
	case Movement::Right:
		tetrimo.x++;
		break;

	case Movement::Left:
		tetrimo.x--;
		break;

	case Movement::Down:
		tetrimo.y++;
		break;
	case Movement::RotateL:
	case Movement::RotateR:
		if (movement == Movement::RotateR) {
			tetrimo.rotation = ++tetrimo.rotation % 4;
		}
		else if (movement == Movement::RotateL) {
			tetrimo.rotation = --tetrimo.rotation % 4;

		}

		switch (tetrimo.type) {
		case I:
			switch (tetrimo.rotation) {
			case 0:
				tetrimo.shape = { {
					{0, 0, 0, 0},
					{1, 1, 1, 1},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 1:
				tetrimo.shape = { {
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0} } };
				break;
			case 2:
				tetrimo.shape = { {
					{0, 0, 0, 0},
					{0, 0, 0, 0},
					{1, 1, 1, 1},
					{0, 0, 0, 0} } };
				break;
			case 3:
				tetrimo.shape = { {
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
			switch (tetrimo.rotation) {
			case 0:
				tetrimo.shape = { {
					{0, 0, 0, 0},
					{1, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				tetrimo.shape = { {
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				tetrimo.shape = { {
					{1, 0, 0, 0},
					{1, 1, 1, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				tetrimo.shape = { {
					{0, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
			/////////////
		case L:
			switch (tetrimo.rotation) {
			case 0:
				tetrimo.shape = { {
					{0, 0, 0, 0},
					{0, 0, 1, 0},
					{1, 1, 1, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				tetrimo.shape = { {
					{1, 0, 0, 0},
					{1, 0, 0, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				tetrimo.shape = { {
					{1, 1, 1, 0},
					{1, 0, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				tetrimo.shape = { {
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
			/////////////
		case T:
			switch (tetrimo.rotation) {
			case 0:
				tetrimo.shape = { {
					{0, 0, 0, 0},
					{0, 1, 0, 0},
					{1, 1, 1, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				tetrimo.shape = { {
					{1, 0, 0, 0},
					{1, 1, 0, 0},
					{1, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				tetrimo.shape = { {
					{1, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				tetrimo.shape = { {
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
			/////////////
		case S:
			switch (tetrimo.rotation) {
			case 0:
				tetrimo.shape = { {
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				tetrimo.shape = { {
					{1, 0, 0, 0},
					{1, 1, 0, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				tetrimo.shape = { {
					{0, 1, 1, 0},
					{1, 1, 0, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				tetrimo.shape = { {
					{0, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
			/////////////
		case Z:
			switch (tetrimo.rotation) {
			case 0:
				tetrimo.shape = { {
					{0, 0, 0, 0},
					{1, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0}} };
				break;
			case 1:
				tetrimo.shape = { {
					{0, 1, 0, 0},
					{1, 1, 0, 0},
					{1, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 2:
				tetrimo.shape = { {
					{1, 1, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0},
					{0, 0, 0, 0} } };
				break;
			case 3:
				tetrimo.shape = { {
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 0, 0, 0} } };
				break;
			}
			break;
		}
	}
	this->ActiveTetrimo = Game::HasCollied(tetrimo) ? originalTetrimo : tetrimo;
}

bool Game::HasCollied(Tetrimo tetrimo) {
	for (auto i = 0; i < 4; i++) {
		for (auto j = 0; j < 4; j++) {
			if (tetrimo.shape[i][j] == true) {
				// If x and y coords are out of bound 
				// or 
				if ((tetrimo.x + i) >= NUM_OF_CELLS_W || (tetrimo.y + j) >= NUM_OF_CELLS_H
					|| (tetrimo.x + i) < 0 || (tetrimo.y + j) < 0
					|| this->Map.at(tetrimo.x + i).at(tetrimo.y + j) != Color::Gray) {
					return true;
				}

			}

		}
	}
	return false;

}


void Game::ProcessInput() {
	Movement input;
	if (this->Keys[GLFW_KEY_RIGHT]) {
		Game::MoveTetrimo(this->ActiveTetrimo, Movement::Right);
	}
	else if (this->Keys[GLFW_KEY_LEFT]) {
		Game::MoveTetrimo(this->ActiveTetrimo, Movement::Left);
	}
	else if (this->Keys[GLFW_KEY_DOWN]) {
		Game::MoveTetrimo(this->ActiveTetrimo, Movement::Down);
	}
	else if (this->Keys[GLFW_KEY_UP]) {
		Game::MoveTetrimo(this->ActiveTetrimo, Movement::RotateR);
	}
	else if (this->Keys[GLFW_KEY_Z]) {
		Game::MoveTetrimo(this->ActiveTetrimo, Movement::RotateL);
	}

}


void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game* game = reinterpret_cast<Game*> (glfwGetWindowUserPointer(window));

	if (key > 0 && key < 350) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (action == GLFW_PRESS) {
			Keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			this->Keys[key] = false;

		}
	}
}
void Game::s_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game* that = static_cast<Game*>(glfwGetWindowUserPointer(window));
	that->key_callback(window, key, scancode, action, mods);
}
