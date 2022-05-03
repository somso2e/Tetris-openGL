#include "game.hpp"

void Game::Init() {
	SmallText_.Init("res/fonts/SourceCodePro-Regular.ttf", 128);
	BigText_.Init("res/fonts/SourceCodePro-Regular.ttf", 256);
	Renderer::Init2D();

	EmptyCellTextureID_ = Renderer::LoadTexture("res/textures/EmptyCell.png");
	GridTextureID_ = Renderer::LoadTexture("res/textures/Grid.png");
	PauseButtonTextureID_ = Renderer::LoadTexture("res/textures/PauseButton.png");
	ArrowUpTextureID_ = Renderer::LoadTexture("res/textures/ArrowUp.png");
	ArrowDownTextureID_ = Renderer::LoadTexture("res/textures/ArrowDown.png");
	Settings::init();

	InitMembers();

}


void Game::InitMembers() {
	// Initilize all class member variables
	// This function is again called when the game restarts

	memset(Keys_, GLFW_RELEASE, sizeof(Keys_));
	memset(KeysProcessed_, false, sizeof(KeysProcessed_));
	auto timeNow = glfwGetTime();

	Map_ = { {Color::Empty} };

	KeysRepeatTimer_.fill(timeNow);

	FallTimer_ = timeNow;
	LockDownTimer_ = timeNow;

	// Initial piece falling speed 
	FallSpeed_ = 1.0; // sec


	NextQueue_ = {};


	CanSwap_ = true;
	FirstSwap_ = true;

	GeneratedBag_ = { 0,1,2,3,4,5,6 };
	BagIndex_ = 7;

	Score_ = 0;
	Level_ = 1;
	TotalLinesCleared_ = 0;
	State_ = GameState::MainMenu;
	PreviousState_ = GameState::MainMenu;

	CountDownInd_ = 0;
	CountDownTimer_ = timeNow;


	HighlightedSettingsInd_ = 0;
	TopModuleind_ = 0;
	InputCustomHotkey_ = GLFW_KEY_UNKNOWN;

	InvalidCustomHotkey_ = false;
	DisplayHotkeys_ = Settings::Hotkeys;
	MenuHighlighted_ = false;

	// Generate 5 Tetrominos and put them in the next queue
	for (auto& tetromino : NextQueue_) {
		tetromino = GenerateTetromino();
	}
	ActiveTetromino_ = GenerateTetromino();
	PreviousTetromino_ = ActiveTetromino_;
}


void Game::Update() {
	const auto timeNow = glfwGetTime();

	ProcessInput();

	if (State_ == GameState::Active) {

		// Move the active tetromino down if piece hasnt moved for "FallSpeed_" seconds.
		if (timeNow >= FallTimer_ + FallSpeed_) {
			if (!MoveTetromino(ActiveTetromino_, Movement::Down)) {
				if (timeNow >= LockDownTimer_ + LOCK_DOWN_TIMEOUT) {
					ActiveTetromino_.placed = true;
				}
			}
			FallTimer_ = timeNow;
		}
		// Check if the tetromino has changed
		if (PreviousTetromino_.pos != ActiveTetromino_.pos ||
			PreviousTetromino_.rotation != ActiveTetromino_.rotation) {
			LockDownTimer_ = timeNow;
		}
		PreviousTetromino_ = ActiveTetromino_;

		// Check for full rows
		uint8_t linesCleared = 0;
		for (int j = 0; j < NUM_OF_CELLS_H; j++) {
			int occupiedCells = 0;
			for (int i = 0; i < NUM_OF_CELLS_W; i++) {
				if (Map_.at(i).at(j) != Color::Empty) {
					occupiedCells++;
				}
			}
			// shift rows down 
			if (occupiedCells == NUM_OF_CELLS_W) {
				linesCleared++;
				for (int j2 = j; 0 < j2; j2--) {
					for (int i = 0; i < NUM_OF_CELLS_W; i++) {
						Map_.at(i).at(j2) = Map_.at(i).at(j2 - 1);
					}
				}
			}
		}

		// Calculate score and level
		TotalLinesCleared_ += linesCleared;
		Level_ = (uint32_t)TotalLinesCleared_ / 10 + 1;
		if (Level_ > 25) {
			Level_ = 25;
		}
		// Change falling speed based on the level
		FallSpeed_ = pow(0.8 - (Level_ - 1) * 0.007, Level_ - 1);
		const uint32_t linesClearedScores[5] = { 0,40, 100, 300, 1200 };
		Score_ += Level_ * (linesClearedScores[linesCleared]);

		// If previous tetromino was placed generate a new one
		if (ActiveTetromino_.placed) {
			CanSwap_ = true;
			// Permenantly append the active Tetromino to the map
			for (auto const& piece : ActiveTetromino_.blocks) {
				point blocks = piece + ActiveTetromino_.pos;
				Map_.at(blocks.x).at(blocks.y) = ActiveTetromino_.color;
			}

			// shift next queue by one
			ActiveTetromino_ = NextQueue_.at(0);
			for (auto i = 0; i < 4; i++) {
				NextQueue_.at(i) = NextQueue_.at(i + 1);
			}
			NextQueue_.at(4) = GenerateTetromino();

			// If the new tetromino has collied, game is over
			if (HasCollied(ActiveTetromino_)) {
				State_ = GameState::GameOver;
			}
		}

		// Append the ghost of the active tetromino to display map
		DisplayMap_ = Map_;
		auto ghost = ActiveTetromino_;
		while (MoveTetromino(ghost, Movement::Down));
		for (const auto& block : ghost.blocks) {
			point pos = block + ghost.pos;
			DisplayMap_.at(pos.x).at(pos.y) = Color::Gray;
		}

		// Append the active tetromino to the display map
		for (auto const& block : ActiveTetromino_.blocks) {
			point pos = block + ActiveTetromino_.pos;
			DisplayMap_.at(pos.x).at(pos.y) = ActiveTetromino_.color;
		}

		// Generate map of next queue
		NextPiecesMap_ = { {Color::Empty} };
		for (auto i = 0; i < NEXT_TETROMINOES_COUNT; i++) {
			for (const auto& block : NextQueue_.at(i).blocks) {
				point pos = block + point(1, 1 + 3 * i);
				NextPiecesMap_.at(pos.x).at(pos.y) = NextQueue_.at(i).color;
			}
		}
		// Generate map of held pieces
		HeldPieceMap_ = { {Color::Empty} };
		Color heldColor = FirstSwap_ ? Color::Empty : TETROMINOES.at(HeldTetrominoType_).color;
		for (const auto& block : TETROMINOES.at(HeldTetrominoType_).blocks) {
			point pos = block + point(1, 1);
			HeldPieceMap_.at(pos.x).at(pos.y) = heldColor;
		}

	}
	else if (State_ == GameState::CountDown) {
		if (timeNow >= CountDownTimer_ + 1.0) {
			if (CountDownInd_ == 2) {
				CountDownInd_ = 0;
				State_ = GameState::Active;
			}
			else {
				CountDownInd_++;
			}
			CountDownTimer_ = timeNow;
		}
	}
	else if (State_ == GameState::CatchedHotkey) {
		// check if the input hotkey hasnt been used before 
		InvalidCustomHotkey_ = false;
		if (InputCustomHotkey_ != DisplayHotkeys_.at(HighlightedSettingsInd_).Value) {
			for (const auto& hotkey : Settings::Hotkeys) {
				if (hotkey.Value == InputCustomHotkey_) {
					InvalidCustomHotkey_ = true;
					State_ = GameState::EditingHotkey;
					break;
				}
			}
			if (InputCustomHotkey_ > 350 || InputCustomHotkey_ < 0) {
				InvalidCustomHotkey_ = true;
				State_ = GameState::EditingHotkey;
			}
		}

		if (InvalidCustomHotkey_ == false) {
			DisplayHotkeys_[HighlightedSettingsInd_].Value = InputCustomHotkey_;
			State_ = GameState::HotkeyMenu;
		}
	}
	Render();
}


bool Game::IsKeyPressed(int key) {
	static const double REPEAT_INTERVAL = 0.05;
	static const double REPEAT_DELAY = 0.15;
	const auto timeNow = glfwGetTime();

	if (Keys_[key] == GLFW_PRESS && !KeysProcessed_[key]) {
		KeysProcessed_[key] = true;
		return true;
	}
	return false;
}


bool Game::IsKeyHeld(int key) {
	static const double REPEAT_INTERVAL = 0.05;
	static const double REPEAT_DELAY = 0.15;
	const auto timeNow = glfwGetTime();

	if (Keys_[key] == GLFW_PRESS && !KeysProcessed_[key]) {
		KeysProcessed_[key] = true;
		KeysRepeatTimer_[key] = timeNow + REPEAT_DELAY;
		return true;
	}
	else if (Keys_[key] != GLFW_RELEASE) {
		if (KeysProcessed_[key] && timeNow >= KeysRepeatTimer_[key] + REPEAT_INTERVAL) {
			KeysRepeatTimer_[key] = timeNow;
			return true;
		}
	}
	return false;

}


void Game::ProcessInput() {
	auto& hotkeys = Settings::Json.at("settings").at("hotkeys");


	switch (State_) {
	case GameState::MainMenu:
		if (IsKeyPressed(GLFW_KEY_UP)) {
			if (HighlightedSettingsInd_ > 0) {
				HighlightedSettingsInd_--;
			}
		}
		else if (IsKeyPressed(GLFW_KEY_DOWN)) {
			// total of 3 buttons on the main menuappl
			if (HighlightedSettingsInd_ < 2) {
				HighlightedSettingsInd_++;
			}
		}
		else if (IsKeyPressed(GLFW_KEY_ENTER)) {
			// PLAY button
			if (HighlightedSettingsInd_ == 0) {
				State_ = GameState::CountDown;
			}// SETTINGS buton
			else if (HighlightedSettingsInd_ == 1) {
				HighlightedSettingsInd_ = 0;
				PreviousState_ = GameState::MainMenu;
				State_ = GameState::HotkeyMenu;
			}// ABOUT button
			else if (HighlightedSettingsInd_ == 2) {
				State_ = GameState::About;
			}
		}
		break;
	case GameState::Active:
		if (IsKeyHeld(hotkeys.at("SOFT DROP"))) {
			if (MoveTetromino(ActiveTetromino_, Movement::Down)) { Score_ += 1; }
		}
		else if (IsKeyHeld(hotkeys["MOVE RIGHT"])) {
			MoveTetromino(ActiveTetromino_, Movement::Right);
		}
		else if (IsKeyHeld(hotkeys["MOVE LEFT"])) {
			MoveTetromino(ActiveTetromino_, Movement::Left);
		}


		if (IsKeyPressed(hotkeys["ROTATE R"])) {
			MoveTetromino(ActiveTetromino_, Movement::RotateR);
		}
		else if (IsKeyPressed(hotkeys["ROTATE L"])) {
			MoveTetromino(ActiveTetromino_, Movement::RotateL);
		}
		else if (IsKeyPressed(hotkeys["ROTATE 180"])) {
			MoveTetromino(ActiveTetromino_, Movement::Rotate2);
		}

		if (IsKeyPressed(hotkeys["HARD DROP"])) {
			while (MoveTetromino(ActiveTetromino_, Movement::Down)) { Score_ += 2; }
			ActiveTetromino_.placed = true;
		}

		if (IsKeyPressed(hotkeys["SWAP"])) {
			SwapTetromino();
		}

		if (IsKeyPressed(hotkeys["RESTART"])) {
			Restart();
		}

		if (IsKeyPressed(hotkeys["PAUSE"])) {
			State_ = GameState::Pause;
		}

		if (IsKeyPressed(hotkeys.at("MAIN MENU"))) {
			State_ = GameState::MainMenu;
			HighlightedSettingsInd_ = 0;
		}

		if (IsKeyPressed(hotkeys["OPEN SETTINGS"])) {
			State_ = GameState::HotkeyMenu;
		}
		break;

	case GameState::Pause:
		if (IsKeyPressed(hotkeys["PAUSE"])) {
			State_ = GameState::CountDown;
		}
		else if (IsKeyPressed(hotkeys["OPEN SETTINGS"])) {
			State_ = GameState::HotkeyMenu;
		}
		else if (IsKeyPressed(hotkeys.at("MAIN MENU"))) {
			State_ = GameState::MainMenu;
			HighlightedSettingsInd_ = 0;
		}
		break;

	case GameState::GameOver:
		if (IsKeyPressed(hotkeys["RESTART"])) {
			Restart();
		}
		else if (IsKeyPressed(hotkeys["OPEN SETTINGS"])) {
			State_ = GameState::HotkeyMenu;
		}
		else if (IsKeyPressed(hotkeys.at("MAIN MENU"))) {
			State_ = GameState::MainMenu;
			HighlightedSettingsInd_ = 0;
		}
		break;
	case GameState::HotkeyMenu:
		/*	if the last input key wasn't released, don't check for any more key presses
			as it may cause problems
		*/
		if (Keys_[InputCustomHotkey_] == GLFW_RELEASE) {
			//reset to a unachievable index
			InputCustomHotkey_ = -1;

			if (MenuHighlighted_) {
				//Back to hotkeys menu
				if (IsKeyPressed(GLFW_KEY_RIGHT)) {
					MenuHighlighted_ = false;
				}
				//Button selected, back to menu
				if (IsKeyPressed(GLFW_KEY_ENTER)) {
					HighlightedSettingsInd_ = 0;
					TopModuleind_ = 0;
					MenuHighlighted_ = false;
					State_ = GameState::MainMenu;
				}
			}
			else {
				if (IsKeyPressed(hotkeys.at("MAIN MENU"))) {
					State_ = GameState::MainMenu;
					HighlightedSettingsInd_ = 0;
				}//to "menu" button
				else if (IsKeyPressed(GLFW_KEY_LEFT)) {
					MenuHighlighted_ = true;
				}
				else if (IsKeyPressed(hotkeys["OPEN SETTINGS"])) {
					//Back to game
					HighlightedSettingsInd_ = 0;
					TopModuleind_ = 0;
					DisplayHotkeys_ = Settings::Hotkeys;
					State_ = GameState::CountDown;
				}
				else if (IsKeyHeld(GLFW_KEY_UP)) {
					if (HighlightedSettingsInd_ > 0) {
						HighlightedSettingsInd_--;
					}
				}
				else if (IsKeyHeld(GLFW_KEY_DOWN)) {
					/* There are total of "number of hotkeys"+"RESET TO DEFAULT button"+"APPLY button"

					*/
					if (HighlightedSettingsInd_ < (int)Settings::Hotkeys.size() + 1) {
						HighlightedSettingsInd_++;
					}
				}
				else if (IsKeyPressed(GLFW_KEY_ENTER)) {
					//RESET TO DEFAULT was chosen
					if (HighlightedSettingsInd_ == Settings::Hotkeys.size()) {
						DisplayHotkeys_ = Settings::DefaultHotkeys;
						State_ = GameState::HotkeyMenu;
					} // APPLY was chosen
					else if (HighlightedSettingsInd_ == Settings::Hotkeys.size() + 1) {
						// Save the new settings to the json file
						Settings::Hotkeys = DisplayHotkeys_;
						for (const auto& hotkey : Settings::Hotkeys) {
							Settings::Json.at("settings").at("hotkeys").at(hotkey.Name) = hotkey.Value;
						}
						std::ofstream file("settings.json");
						file << Settings::Json.dump(1, '\t');

						HighlightedSettingsInd_ = 0;
						TopModuleind_ = 0;

						if (PreviousState_ == GameState::MainMenu) {
							State_ = GameState::MainMenu;
						}
						else {
							State_ = GameState::Active;
							PreviousState_ = GameState::Active;
						}

					}
					else {
						State_ = GameState::EditingHotkey;
					}
				}

			}
		}
		break;
	case GameState::EditingHotkey:
		DisplayHotkeys_.at(HighlightedSettingsInd_).Value = 349;
		for (int i = 0; i < 350; i++) {
			if (Keys_[i] == GLFW_PRESS && !KeysProcessed_[i]) {
				InputCustomHotkey_ = i;
				KeysProcessed_[i] = true;
				State_ = GameState::CatchedHotkey;
				break;
			}
		}
		break;
	case GameState::About:
		if (IsKeyPressed(GLFW_KEY_ENTER)) {
			State_ = GameState::MainMenu;
		}
		break;
	}

}


bool Game::HasCollied(Tetromino tetromino) {
	for (auto const& piece : tetromino.blocks) {
		point blocks = piece + tetromino.pos;
		// If x and y coords are out of bound 
		// or if the cell in the map is a highlightColor other than gray AKA it's occupied 
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


void Game::SwapTetromino() {
	if (CanSwap_) {
		if (FirstSwap_) {
			HeldTetrominoType_ = ActiveTetromino_.type;
			ActiveTetromino_ = NextQueue_.at(0);
			for (auto i = 0; i < 4; i++) {
				NextQueue_.at(i) = NextQueue_.at(i + 1);
			}
			NextQueue_.at(4) = GenerateTetromino();

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


template<std::size_t size_x, std::size_t size_y>
void Game::DrawGrid(std::array < std::array < Color, size_y>, size_x>& grid, const glm::vec2 offset) {
	for (int j = 0; j < (int)grid.at(0).size(); j++) {
		for (int i = 0; i < (int)grid.size(); i++) {
			if (grid.at(i).at(j) == Color::Empty) {
				Renderer::DrawQuad(
					glm::vec2(i * CellSize_, j * CellSize_) + offset,
					glm::vec2(CellSize_),
					GetColor(Color::White),
					GridTextureID_);
			}
			else {
				Renderer::DrawQuad(
					glm::vec2(i * CellSize_, j * CellSize_) + offset,
					glm::vec2(CellSize_),
					GetColor(grid.at(i).at(j)),
					EmptyCellTextureID_);
			}
		}
	}
}


void Game::Render() {
	Renderer::BeginBatch();

	if (State_ == GameState::HotkeyMenu || State_ == GameState::EditingHotkey) {
		RenderHotkeysMenu();
	}
	else if (State_ == GameState::MainMenu) {
		RenderMainMenu();
	}
	else if (State_ == GameState::About) {
		RenderAbout();
	}
	else {
		// Render main game
		DrawGrid(NextPiecesMap_, NextPieceCoords_);
		DrawGrid(DisplayMap_, MainMapCoords_);
		DrawGrid(HeldPieceMap_, HeldPieceCoords_);

		RenderScoreBox();
	}

	if (State_ == GameState::Pause ||
		State_ == GameState::GameOver ||
		State_ == GameState::CountDown) {
		Renderer::DrawQuad(
			glm::vec2(0.0f),
			glm::vec2(windowWidth, windowHeight),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.8f),
			Renderer::Data.WhiteTextureID);

		if (State_ == GameState::Pause) {
			RenderPause();
		}
		else if (State_ == GameState::GameOver) {
			RenderGameOver();
		}
		else if (State_ == GameState::CountDown) {
			RenderCountDown();
		}
	}


	Renderer::EndBatch();
	Renderer::Flush();
}


void Game::RenderScoreBox() {
	// Render score display
	Renderer::DrawQuad(
		ScoreWindowCoords_,
		glm::vec2(4 * CellSize_),
		GetColor(Color::White),
		GridTextureID_);

	SmallText_.Write(
		"SCORE:\n" + std::to_string(Score_) + "\nLEVEL:\n" + std::to_string(Level_),
		ScoreWindowCoords_ + glm::vec2(CellSize_ * 2),
		glm::vec2(DefaultFontSize_),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Center);
}


void Game::RenderPause() {
	Renderer::DrawQuad(
		PauseButtonCoords_,
		glm::vec2(PauseButtonSize_),
		GetColor(Color::White),
		PauseButtonTextureID_);

	SmallText_.Write(
		std::string("PRESS ") + GetKeyName(Settings::Json["settings"]["hotkeys"]["PAUSE"]) + " TO RESUME",
		glm::vec2(windowWidth / 2, windowHeight / 2 + PauseButtonSize_ * 0.4f),
		glm::vec2(PauseButtonSize_ / 2.5f),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Center);
}


void Game::RenderGameOver() {
	BigText_.Write(
		"GAME OVER",
		glm::vec2(windowWidth / 2, windowHeight / 2) - glm::vec2(0.0f, PauseButtonSize_ * 0.5f),
		glm::vec2(PauseButtonSize_ * 1.5f),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center);
	SmallText_.Write(
		std::string("PRESS ") + GetKeyName(Settings::Json["settings"]["hotkeys"]["RESTART"]) + " TO RESTART",
		glm::vec2(windowWidth / 2, windowHeight / 2 + PauseButtonSize_ * 0.2f),
		glm::vec2(PauseButtonSize_ / 2.5f),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Center);
}


void Game::RenderCountDown() {
	static const char* numbers[] = { "3","2","1" };
	static float size = (float)windowHeight / 4;
	BigText_.Write(
		numbers[CountDownInd_],
		glm::vec2(windowWidth / 2, windowHeight / 2),
		glm::vec2(size),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Center);
}


void Game::RenderHotkeysMenu() {
	if (HighlightedSettingsInd_ > TopModuleind_ + MAX_VISIBLE_SETTINGS_ - 1) {
		TopModuleind_++;
	}
	else if (HighlightedSettingsInd_ < TopModuleind_ && TopModuleind_ > 0) {
		TopModuleind_--;
	}

	auto position = SettingsBoxCoords_;
	static const float offset = 2.5f;
	static const auto fontSize = glm::vec2(DefaultFontSize_ * 0.9f);

	auto max = std::min((int)Settings::Hotkeys.size(), TopModuleind_ + MAX_VISIBLE_SETTINGS_ + 1);

	for (auto i = TopModuleind_; i < max; i++) {
		auto& [name, key] = DisplayHotkeys_.at(i);
		//auto& key = DisplayHotkeys_.at(i);

		Renderer::DrawQuad(
			position + glm::vec2(offset),
			SettingsBoxSize_ - glm::vec2(offset) * 2.0f,
			glm::vec4(0.16f, 0.16f, 0.16f, 1.0f),
			Renderer::Data.WhiteTextureID
		);
		SmallText_.Write(
			name,
			position + SettingsBoxSize_ / 2.0f,
			fontSize,
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center
		);

		Renderer::DrawQuad(
			position + glm::vec2(SettingsBoxSize_.x, 0.0f) + glm::vec2(offset),
			SettingsBoxSize_ - glm::vec2(offset) * 2.0f,
			GetColor(Color::Gray1),
			Renderer::Data.WhiteTextureID
		);
		SmallText_.Write(
			GetKeyName(key),
			position + glm::vec2(SettingsBoxSize_.x, 0.0f) + SettingsBoxSize_ / 2.0f,
			fontSize,
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center
		);

		position.y += SettingsBoxSize_.y;
	}
	if (TopModuleind_ >= (int)Settings::Hotkeys.size() - MAX_VISIBLE_SETTINGS_) {
		Renderer::DrawQuad(
			position + glm::vec2(offset),
			glm::vec2(2 * SettingsBoxSize_.x, SettingsBoxSize_.y) - glm::vec2(offset) * 2.0f,
			GetColor(Color::Gray2),
			Renderer::Data.WhiteTextureID
		);
		SmallText_.Write(
			"RESET TO DEFAULT",
			position + glm::vec2(SettingsBoxSize_.x / 2.0f, 0.0f) + SettingsBoxSize_ / 2.0f,
			fontSize,
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center
		);
	}
	position.y += SettingsBoxSize_.y;
	if (TopModuleind_ == Settings::Hotkeys.size() - MAX_VISIBLE_SETTINGS_ + 2) {
		Renderer::DrawQuad(
			position + glm::vec2(offset),
			glm::vec2(2 * SettingsBoxSize_.x, SettingsBoxSize_.y) - glm::vec2(offset) * 2.0f,
			GetColor(Color::Gray2),
			Renderer::Data.WhiteTextureID
		);
		SmallText_.Write(
			"APPLY",
			position + glm::vec2(SettingsBoxSize_.x / 2.0f, 0.0f) + SettingsBoxSize_ / 2.0f,
			fontSize,
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center
		);
	}
	Renderer::DrawQuad(
		glm::vec2(offset) * 4.0f,
		glm::vec2(SettingsBoxSize_.x / 3, SettingsBoxSize_.y),
		GetColor(Color::Gray1),
		Renderer::Data.WhiteTextureID
	);
	SmallText_.Write(
		"MENU",
		glm::vec2(SettingsBoxSize_.x / 3, SettingsBoxSize_.y) / 2.0f + glm::vec2(offset) * 4.0f,
		fontSize,
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Center
	);

	glm::vec4 highlightColor = GetColor(Color::Gray3);
	if (MenuHighlighted_) {
		DrawHighlightSelection(
			glm::vec2(offset) * 3.0f,
			glm::vec2(SettingsBoxSize_.x / 3, SettingsBoxSize_.y),
			offset * 2,
			highlightColor
		);

	}
	else {
		float thickness = offset;
		if (State_ == GameState::EditingHotkey) {
			highlightColor = GetColor(Color::White);
			thickness = offset * 1.5f;
		}

		if (InvalidCustomHotkey_) {
			SmallText_.Write(
				"INVALID!",
				SettingsBoxCoords_ + glm::vec2(SettingsBoxSize_.x * 2.0f, SettingsBoxSize_.y * (HighlightedSettingsInd_ - TopModuleind_ + 0.5f)) + glm::vec2(offset) * 4.0f,
				fontSize,
				GetColor(Color::Red),
				Renderer::Text::HorizontalAlignment::Left,
				Renderer::Text::VerticalAlignment::Center
			);
			highlightColor = GetColor(Color::Red);
		}

		DrawHighlightSelection(
			SettingsBoxCoords_ + glm::vec2(0.0f, SettingsBoxSize_.y * (HighlightedSettingsInd_ - TopModuleind_)),
			glm::vec2(2 * SettingsBoxSize_.x, SettingsBoxSize_.y),
			thickness * 2,
			highlightColor
		);
	}
	if (TopModuleind_ != 0) {
		Renderer::DrawQuad(
			glm::vec2(windowWidth / 2 - SettingsBoxSize_.x - SettingsBoxSize_.y * 2.0f, windowHeight / 2 - SettingsBoxSize_.y),
			glm::vec2(SettingsBoxSize_.y),
			GetColor(Color::Gray),
			ArrowUpTextureID_
		);
	}
	if (TopModuleind_ != Settings::Hotkeys.size() - MAX_VISIBLE_SETTINGS_ + 2) {
		Renderer::DrawQuad(
			glm::vec2(windowWidth / 2 - SettingsBoxSize_.x - SettingsBoxSize_.y * 2.0f, windowHeight / 2),
			glm::vec2(SettingsBoxSize_.y),
			GetColor(Color::Gray),
			ArrowDownTextureID_
		);
	}
}


void Game::DrawHighlightSelection(const glm::vec2& position, const glm::vec2& size, const float& thickness, const glm::vec4& color) {
	//Upper line
	Renderer::DrawQuad(
		position,
		glm::vec2(size.x, thickness),
		color,
		Renderer::Data.WhiteTextureID
	);
	// Lower line
	Renderer::DrawQuad(
		position + glm::vec2(0.0f, size.y - thickness),
		glm::vec2(size.x, thickness),
		color,
		Renderer::Data.WhiteTextureID
	);
	// Left line
	Renderer::DrawQuad(
		position,
		glm::vec2(thickness, size.y),
		color,
		Renderer::Data.WhiteTextureID
	);
	// Right line 
	Renderer::DrawQuad(
		position + glm::vec2(size.x - thickness, 0.0f),
		glm::vec2(thickness, size.y),
		color,
		Renderer::Data.WhiteTextureID
	);
}


void Game::RenderMainMenu() {
	BigText_.Write(
		"TETRIS",
		glm::vec2(windowWidth / 2.0f, windowHeight / 4.0f),
		glm::vec2(windowHeight / 4.0f),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Center
	);
	static const float offset = 5.0f;

	static const auto size = glm::vec2(windowWidth / 3, windowHeight / 9);
	auto pos = glm::vec2(windowWidth / 2, windowHeight / 2);

	DrawHighlightSelection(
		pos - size / 2.0f + glm::vec2(0.0f, size.y * HighlightedSettingsInd_),
		size,
		offset,
		GetColor(Color::Gray3)
	);

	const char* buttons[] = { "PLAY","SETTINGS","ABOUT" };
	for (const auto& button : buttons) {
		Renderer::DrawQuad(
			pos - size / 2.0f + glm::vec2(offset),
			size - glm::vec2(offset) * 2.0f,
			GetColor(Color::Gray2),
			Renderer::Data.WhiteTextureID
		);
		SmallText_.Write(
			button,
			pos,
			glm::vec2(size.y / 2),
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center
		);
		pos.y += size.y;
	}
}


void Game::RenderAbout() {
	SmallText_.Write(
		"Tetris  clone  made  using  OpenGL  in  C++\n"
		"Developed  by  Somso2e  in  the  spring  of  2022  as  a  passion  project.\n"
		"The  full  source  code  is  available  at\n"
		"https://github.com/somso2e/Tetris-openGL",
		glm::vec2(windowWidth / 2, windowHeight / 4),
		glm::vec2(DefaultFontSize_ / 1.2f),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Top
	);

	static const auto pos = glm::vec2(windowWidth / 2, windowHeight * 3 / 4);
	static const auto size = glm::vec2(windowWidth / 3, windowHeight / 9);
	static const float offset = 5.0f;

	Renderer::DrawQuad(
		glm::vec2(windowWidth / 2, windowHeight * 3 / 4) - size / 2.0f + glm::vec2(offset),
		size - glm::vec2(offset) * 2.0f,
		GetColor(Color::Gray1),
		Renderer::Data.WhiteTextureID
	);
	DrawHighlightSelection(
		pos - size / 2.0f,
		size,
		offset,
		GetColor(Color::Gray3)
	);

	SmallText_.Write(
		"BACK",
		glm::vec2(windowWidth / 2, windowHeight * 3 / 4),
		glm::vec2(DefaultFontSize_),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Center
	);

}

void Game::Restart() {
	InitMembers();
	State_ = GameState::CountDown;
}


const char* Game::GetKeyName(int key) {
	static std::array<const char*, 350> names;
	names.fill("UNKNOWN");

	names[GLFW_KEY_SPACE] = "SPACE";
	names[GLFW_KEY_APOSTROPHE] = "'";
	names[GLFW_KEY_COMMA] = ",";
	names[GLFW_KEY_MINUS] = "-";
	names[GLFW_KEY_PERIOD] = ".";
	names[GLFW_KEY_SLASH] = "/";
	names[GLFW_KEY_0] = "0";
	names[GLFW_KEY_1] = "1";
	names[GLFW_KEY_2] = "2";
	names[GLFW_KEY_3] = "3";
	names[GLFW_KEY_4] = "4";
	names[GLFW_KEY_5] = "5";
	names[GLFW_KEY_6] = "6";
	names[GLFW_KEY_7] = "7";
	names[GLFW_KEY_8] = "8";
	names[GLFW_KEY_9] = "9";
	names[GLFW_KEY_SEMICOLON] = ";";
	names[GLFW_KEY_EQUAL] = "=";
	names[GLFW_KEY_A] = "A";
	names[GLFW_KEY_B] = "B";
	names[GLFW_KEY_C] = "C";
	names[GLFW_KEY_D] = "D";
	names[GLFW_KEY_E] = "E";
	names[GLFW_KEY_F] = "F";
	names[GLFW_KEY_G] = "G";
	names[GLFW_KEY_H] = "H";
	names[GLFW_KEY_I] = "I";
	names[GLFW_KEY_J] = "J";
	names[GLFW_KEY_K] = "K";
	names[GLFW_KEY_L] = "L";
	names[GLFW_KEY_M] = "M";
	names[GLFW_KEY_N] = "N";
	names[GLFW_KEY_O] = "O";
	names[GLFW_KEY_P] = "P";
	names[GLFW_KEY_Q] = "Q";
	names[GLFW_KEY_R] = "R";
	names[GLFW_KEY_S] = "S";
	names[GLFW_KEY_T] = "T";
	names[GLFW_KEY_U] = "U";
	names[GLFW_KEY_V] = "V";
	names[GLFW_KEY_W] = "W";
	names[GLFW_KEY_X] = "X";
	names[GLFW_KEY_Y] = "Y";
	names[GLFW_KEY_Z] = "Z";
	names[GLFW_KEY_LEFT_BRACKET] = "[";
	names[GLFW_KEY_BACKSLASH] = "\\";
	names[GLFW_KEY_RIGHT_BRACKET] = "]";
	names[GLFW_KEY_GRAVE_ACCENT] = "`";
	names[GLFW_KEY_WORLD_1] = "NON-US #1";
	names[GLFW_KEY_WORLD_2] = "NON-US #2";
	names[GLFW_KEY_ESCAPE] = "ESC";
	names[GLFW_KEY_ENTER] = "ENTER";
	names[GLFW_KEY_TAB] = "TAB";
	names[GLFW_KEY_BACKSPACE] = "BACKSPACE";
	names[GLFW_KEY_INSERT] = "INSERT";
	names[GLFW_KEY_DELETE] = "DELETE";
	names[GLFW_KEY_RIGHT] = "RIGHT ARROW";
	names[GLFW_KEY_LEFT] = "LEFT ARROW";
	names[GLFW_KEY_DOWN] = "DOWN ARROW";
	names[GLFW_KEY_UP] = "UP ARROW";
	names[GLFW_KEY_PAGE_UP] = "PAGE UP";
	names[GLFW_KEY_PAGE_DOWN] = "PAGE DOWN";
	names[GLFW_KEY_HOME] = "HOME";
	names[GLFW_KEY_END] = "END";
	names[GLFW_KEY_CAPS_LOCK] = "CAPS LOCK";
	names[GLFW_KEY_SCROLL_LOCK] = "SCROLL LOCK";
	names[GLFW_KEY_NUM_LOCK] = "NUM LUCK";
	names[GLFW_KEY_PRINT_SCREEN] = "PRINT SCREEN";
	names[GLFW_KEY_PAUSE] = "PAUSE";
	names[GLFW_KEY_F1] = "F1";
	names[GLFW_KEY_F2] = "F2";
	names[GLFW_KEY_F3] = "F3";
	names[GLFW_KEY_F4] = "F4";
	names[GLFW_KEY_F5] = "F5";
	names[GLFW_KEY_F6] = "F6";
	names[GLFW_KEY_F7] = "F7";
	names[GLFW_KEY_F8] = "F8";
	names[GLFW_KEY_F9] = "F9";
	names[GLFW_KEY_F10] = "F10";
	names[GLFW_KEY_F11] = "F11";
	names[GLFW_KEY_F12] = "F12";
	names[GLFW_KEY_F13] = "F13";
	names[GLFW_KEY_F14] = "F14";
	names[GLFW_KEY_F15] = "F15";
	names[GLFW_KEY_F16] = "F16";
	names[GLFW_KEY_F17] = "F17";
	names[GLFW_KEY_F18] = "F18";
	names[GLFW_KEY_F19] = "F19";
	names[GLFW_KEY_F20] = "F20";
	names[GLFW_KEY_F21] = "F21";
	names[GLFW_KEY_F22] = "F22";
	names[GLFW_KEY_F23] = "F23";
	names[GLFW_KEY_F24] = "F24";
	names[GLFW_KEY_F25] = "F25";
	names[GLFW_KEY_KP_0] = "KEYPAD 0";
	names[GLFW_KEY_KP_1] = "KEYPAD 1";
	names[GLFW_KEY_KP_2] = "KEYPAD 2";
	names[GLFW_KEY_KP_3] = "KEYPAD 3";
	names[GLFW_KEY_KP_4] = "KEYPAD 4";
	names[GLFW_KEY_KP_5] = "KEYPAD 5";
	names[GLFW_KEY_KP_6] = "KEYPAD 6";
	names[GLFW_KEY_KP_7] = "KEYPAD 7";
	names[GLFW_KEY_KP_8] = "KEYPAD 8";
	names[GLFW_KEY_KP_9] = "KEYPAD 9";
	names[GLFW_KEY_KP_DECIMAL] = "KEYPAD DECIMAL";
	names[GLFW_KEY_KP_DIVIDE] = "KEYPAD /";
	names[GLFW_KEY_KP_MULTIPLY] = "KEYPAD *";
	names[GLFW_KEY_KP_SUBTRACT] = "KEYPAD -";
	names[GLFW_KEY_KP_ADD] = "KEYPAD +";
	names[GLFW_KEY_KP_ENTER] = "KEYPAD ENTER";
	names[GLFW_KEY_KP_EQUAL] = "KEYPAD =";
	names[GLFW_KEY_LEFT_SHIFT] = "LEFT SHIFT";
	names[GLFW_KEY_LEFT_CONTROL] = "LEFT CTRL";
	names[GLFW_KEY_LEFT_ALT] = "LEFT ALT";
	names[GLFW_KEY_LEFT_SUPER] = "LEFT SUPER";
	names[GLFW_KEY_RIGHT_SHIFT] = "RIGHT SHIFT";
	names[GLFW_KEY_RIGHT_CONTROL] = "RIGHT CONTROL";
	names[GLFW_KEY_RIGHT_ALT] = "RIGHT ALT";
	names[GLFW_KEY_RIGHT_SUPER] = "RIGHT SUPER";
	names[GLFW_KEY_MENU] = "MENU";
	names[349] = "PRESS NEW KEY";
	if (0 < key && key < 350) {
		return names[key];
	}
	else {
		return "UNKOWN";
	}
}


glm::vec4 Game::GetColor(Color color) {
	using enum  Color;
	switch (color) {
	case Empty:
		return glm::vec4(0.0f);
	case White:
		return glm::vec4(1.0f);
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
		return glm::vec4(0.0f, 0.3f, 1.0f, 1.0f);
	case Gray1:
		return glm::vec4(0.129f, 0.129f, 0.129f, 1.0f);
	case Gray2:
		return glm::vec4(0.16f, 0.16f, 0.16f, 1.0f);
	case Gray3:
		return glm::vec4(0.3f, 0.35f, 0.35f, 1.0f);
	}
}
