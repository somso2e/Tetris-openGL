#include "game.hpp"
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

	if (State_ == GameState::HotkeyMenu ||
		State_ == GameState::EditingHotkey) {
		RenderHotkeysMenu();
	}
	else if (State_ == GameState::MainMenu) {
		RenderMainMenu();
	}
	else if (State_ == GameState::About) {
		RenderAbout();
	}
	else if (State_ == GameState::GameModeMenu) {
		RenderGameModeSelect();
	}
	else if (State_ == GameState::HighScoreMenu) {
		RenderRecordsMenu();
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
		State_ == GameState::CountDown ||
		State_ == GameState::YouWon) {
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
		else if (State_ == GameState::YouWon) {
			RenderYouWon();
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

	if (GameMode_ == GameMode::Classic) {
		SmallText_.Write(
			"SCORE:\n" + std::to_string(Score_) + "\nLEVEL:\n" + std::to_string(Level_),
			ScoreWindowCoords_ + glm::vec2(CellSize_ * 2),
			glm::vec2(DefaultFontSize_),
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center);
	}
	else if (GameMode_ == GameMode::FourtyLines) {

		SmallText_.Write(
			"Lines:\n" + std::to_string(TotalLinesCleared_) + " /40\nTime:",
			ScoreWindowCoords_ + CellSize_ * glm::vec2(2, 1.5),
			glm::vec2(DefaultFontSize_ * 0.9f),
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center);
		SmallText_.Write(
			GetTime(ElapsedTime_),
			ScoreWindowCoords_ + glm::vec2(CellSize_ * 2) + glm::vec2(0, DefaultFontSize_ * 0.9 * 1.5),
			glm::vec2(DefaultFontSize_ * 0.7f),
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center);
	}
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


void Game::RenderYouWon() {
	BigText_.Write(
		"YOU WON",
		glm::vec2(windowWidth / 2, windowHeight / 2) - glm::vec2(0.0f, PauseButtonSize_ * 0.5f),
		glm::vec2(PauseButtonSize_ * 1.5f),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center);
	SmallText_.Write(
		GetTime(ElapsedTime_) + "\n"
		"PRESS " + GetKeyName(Settings::Json["settings"]["hotkeys"]["RESTART"]) + " TO RESTART",
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
			GetColor(Color::Gray2),
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


void Game::RenderGameModeSelect() {

	static const float offset = 5.0f;
	const std::vector<const char*> buttons = { "CLASSIC","40 LINES","BACK" };

	static const auto size = glm::vec2(windowWidth / 3, windowHeight / 9);
	auto pos = glm::vec2(windowWidth / 2, windowHeight / 2) - (buttons.size() / 2.0f) * glm::vec2(0.0f, size.y) - glm::vec2(offset);

	DrawHighlightSelection(
		pos - size / 2.0f + glm::vec2(0.0f, size.y * (HighlightedSettingsInd_ + 0.5f)),
		size,
		offset,
		GetColor(Color::Gray3)
	);

	for (const auto& button : buttons) {
		Renderer::DrawQuad(
			pos - glm::vec2(size.x, 0.0f) / 2.0f + glm::vec2(offset),
			size - glm::vec2(offset) * 2.0f,
			GetColor(Color::Gray2),
			Renderer::Data.WhiteTextureID
		);
		SmallText_.Write(
			button,
			pos + glm::vec2(0.0f, size.y) / 2.0f,
			glm::vec2(size.y / 2),
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center
		);
		pos.y += size.y;
	}
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

	const char* buttons[] = { "PLAY","SETTINGS","HIGH SCORES","ABOUT" };
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


void Game::RenderRecordsMenu() {
	auto& recordsList = Settings::Json.at("high scores");

	static const float offset = 2.5f;
	static const auto size = glm::vec2(SettingsBoxSize_.x, windowHeight / 12);
	auto position = glm::vec2(windowWidth / 2 - size.x, 0.0f);
	static const  char* gamemodes[] = { "CLASSIC","40 LINES" };
	auto pos = position;
	for (int g = 0; g < 2; g++) {
		Renderer::DrawQuad(
			pos + glm::vec2(offset),
			size - glm::vec2(offset) * 2.0f,
			GetColor(Color::Gray3),
			Renderer::Data.WhiteTextureID
		);
		SmallText_.Write(
			gamemodes[g],
			pos + size / 2.0f,
			glm::vec2(DefaultFontSize_),
			GetColor(Color::White),
			Renderer::Text::HorizontalAlignment::Center,
			Renderer::Text::VerticalAlignment::Center
		);
		pos.y += size.y;
		std::array<std::string, 10> scores;
		if (g == 0) {
			auto recordList = recordsList.at("classic").get<std::vector<int>>();
			scores.fill("-");
			for (int s = 0; s < std::min((int)recordList.size(), 10); s++) {
				scores.at(s) = std::to_string(recordList.at(s));
			}

		}
		else {
			auto recordList = recordsList.at("fourty lines").get<std::vector<double>>();
			scores.fill("--:--:---");
			for (int s = 0; s < std::min((int)recordList.size(), 10); s++) {
				scores.at(s) = GetTime(recordList.at(s));
			}

		}
		for (int i = 0; i < 10; i++) {
			Renderer::DrawQuad(
				pos + glm::vec2(offset),
				size - glm::vec2(offset) * 2.0f,
				GetColor(Color::Gray1),
				Renderer::Data.WhiteTextureID
			);
			SmallText_.Write(
				scores[i],
				pos + 2.0f * glm::vec2(offset) + size / 2.0f,
				glm::vec2(DefaultFontSize_),
				GetColor(Color::White),
				Renderer::Text::HorizontalAlignment::Center,
				Renderer::Text::VerticalAlignment::Center
			);
			pos.y += size.y;
		}
		pos.x += size.x;
		pos.y = position.y;
	}
	auto p = position + glm::vec2(0, 11 * size.y) + glm::vec2(offset);
	auto s = size * glm::vec2(2.0f, 1.0f) - glm::vec2(offset) * 2.0f;
	Renderer::DrawQuad(
		p,
		s,
		GetColor(Color::Gray2),
		Renderer::Data.WhiteTextureID
	);
	DrawHighlightSelection(
		position + glm::vec2(0, 11 * size.y),
		size * glm::vec2(2.0f, 1.0f),
		offset,
		GetColor(Color::Gray3)
	);
	SmallText_.Write(
		"BACK",
		p + s / 2.0f,
		glm::vec2(DefaultFontSize_),
		GetColor(Color::White),
		Renderer::Text::HorizontalAlignment::Center,
		Renderer::Text::VerticalAlignment::Center
	);

}

