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
	DisplayMap_ = { {Color::Empty} };

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

	ElapsedTime_ = 0.0;

	CountDownInd_ = 0;
	CountDownTimer_ = timeNow;

	HighlightedSettingsInd_ = 0;
	TopModuleind_ = 0;
	InputCustomHotkey_ = GLFW_KEY_UNKNOWN;

	InvalidCustomHotkey_ = false;
	DisplayHotkeys_ = Settings::Hotkeys;
	MenuHighlighted_ = false;

	PreviousBoolDisplayMap_ = { {false} };
	BoolDisplayMap_ = { {false} };

	// Generate 5 Tetrominos and put them in the next queue
	for (auto& tetromino : NextQueue_) {
		tetromino = GenerateTetromino();
	}
	ActiveTetromino_ = GenerateTetromino();
	PreviousTetromino_ = ActiveTetromino_;

}


void Game::Update() {
	const auto timeNow = glfwGetTime();
	static auto timeThen = glfwGetTime();
	ProcessInput();

	if (State_ == GameState::Active) {
		ElapsedTime_ += timeNow - timeThen;
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
				for (int i = 0; i < NUM_OF_CELLS_W; i++) {
					Map_.at(i).at(0) = Color::Empty;
				}

			}
		}
		//Map_.at(0).at(1) = Color::Blue;
		// Calculate score and level
		TotalLinesCleared_ += linesCleared;
		Level_ = (uint32_t)TotalLinesCleared_ / 10 + 1;
		if (Level_ > 25) {
			Level_ = 25;
		}
		// Change falling speed based on the level
		FallSpeed_ = pow(0.8 - (Level_ - 1) * 0.007, Level_ - 1);
		const uint32_t linesClearedScores[5] = { 0, 40, 100, 300, 1200 };
		Score_ += Level_ * (linesClearedScores[linesCleared]);

		// 

		if (GameMode_ == GameMode::FourtyLines) {
			if (TotalLinesCleared_ >= 40) {
				State_ = GameState::YouWon;
				UpdateHighScore();
			}
			// Game over if reached 100 minutes
			if (ElapsedTime_ >= 5999.0) {
				State_ = GameState::GameOver;
			}
		}
		bool shouldSave = false;
		bool done = false;
		// If previous tetromino was placed generate a new one
		if (ActiveTetromino_.placed) {
			shouldSave = true;
			FallTimer_ = timeNow;
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
				UpdateHighScore();
				done = true;
			}
		}
		for (auto y = 0; y < NUM_OF_CELLS_H; y++) {
			for (auto x = 0; x < NUM_OF_CELLS_W; x++) {
				BoolDisplayMap_.at(x).at(y) = (Map_.at(x).at(y) != Color::Empty);
			}
		}

		// save to buffer replay if the active tetromino was placed
		if (shouldSave && Settings::Json.at("settings").at("record data")) {
			SaveToMemoryReplay(BoolDisplayMap_, PreviousBoolDisplayMap_, done);
		}

		PreviousBoolDisplayMap_ = BoolDisplayMap_;


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
				FallTimer_ = timeNow;
				LockDownTimer_ = timeNow;
			}
			else {
				CountDownInd_++;
			}
			CountDownTimer_ = timeNow;
		}
	}
	else if (State_ == GameState::EditingHotkey) {
		DisplayHotkeys_.at(HighlightedSettingsInd_).Value = 349;
		for (int i = 0; i < 350; i++) {
			if (Keys_[i] == GLFW_PRESS && !KeysProcessed_[i]) {
				InputCustomHotkey_ = i;
				KeysProcessed_[i] = true;
				State_ = GameState::CatchedHotkey;
				break;
			}
		}
	}

	if (State_ == GameState::CatchedHotkey) {
		// check if the input hotkey hasnt been used before 
		InvalidCustomHotkey_ = false;
		if (InputCustomHotkey_ != DisplayHotkeys_.at(HighlightedSettingsInd_).Value) {
			for (const auto& hotkey : DisplayHotkeys_) {
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
	timeThen = timeNow;
	Render();
}


void Game::Restart() {
	InitMembers();
	State_ = GameState::CountDown;
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


std::string Game::GetTime(double elapsedTime) {
	auto t = elapsedTime;
	int min = (int)t / 60;
	t -= 60 * min;
	int sec = (int)t;
	t -= sec;
	int millis = t * 1000;
	char str[10]{};
	sprintf_s(str, "%02d:%02d:%03d", min, sec, millis);
	return std::string(str);
}


void Game::UpdateHighScore() {
	auto& j = Settings::Json.at("high scores");
	if (GameMode_ == GameMode::Classic) {
		auto records = j.at("classic").get<std::vector<int>>();
		auto it = std::upper_bound(records.begin(), records.end(), Score_,
			[](auto& lhs, auto& rhs) { return lhs > rhs; });
		records.insert(it, Score_);
		j["classic"] = records;
	}
	else if (GameMode_ == GameMode::FourtyLines) {
		auto records = j.at("fourty lines").get<std::vector<float>>();
		auto it = std::upper_bound(records.begin(), records.end(), ElapsedTime_,
			[](auto& lhs, auto& rhs) { return lhs < rhs; });
		records.insert(it, ElapsedTime_);
		j["fourty lines"] = records;
	}
	Settings::save();
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

