#include "game.hpp"


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
			// total of 3 buttons on the main menu
			if (HighlightedSettingsInd_ < 3) {
				HighlightedSettingsInd_++;
			}
		}
		else if (IsKeyPressed(GLFW_KEY_ENTER)) {
			switch (HighlightedSettingsInd_) {
			case 0:
				// PLAY button
				State_ = GameState::GameModeMenu;
				break;
			case 1:
				// SETTINGS button
				HighlightedSettingsInd_ = 0;
				PreviousState_ = GameState::MainMenu;
				State_ = GameState::HotkeyMenu;
				break;
			case 2:
				// TOP RECORDS button
				State_ = GameState::HighScoreMenu;
				break;
			case 3:
				// ABOUT button
				State_ = GameState::About;
				break;
			}
			HighlightedSettingsInd_ = 0;
		}
		break;
	case GameState::HighScoreMenu:
		if (IsKeyPressed(GLFW_KEY_ENTER)) {
			State_ = GameState::MainMenu;
		}
		break;
	case GameState::GameModeMenu:
		if (IsKeyPressed(GLFW_KEY_UP)) {
			if (HighlightedSettingsInd_ > 0) {
				HighlightedSettingsInd_--;
			}
		}
		else if (IsKeyPressed(GLFW_KEY_DOWN)) {
			// total of 3 buttons on the main menu
			if (HighlightedSettingsInd_ < 2) {
				HighlightedSettingsInd_++;
			}
		}
		else if (IsKeyPressed(GLFW_KEY_ENTER)) {
			switch (HighlightedSettingsInd_) {
			case 0:
				// Classic button
				GameMode_ = GameMode::Classic;
				State_ = GameState::CountDown;
				break;
			case 1:
				// 40 Lines buton
				GameMode_ = GameMode::FourtyLines;
				State_ = GameState::CountDown;
				break;
			case 2:
				// BACK buton
				State_ = GameState::MainMenu;
				break;
			}
			HighlightedSettingsInd_ = 0;
		}
		break;
	case GameState::Active:
		if (IsKeyHeld(hotkeys.at("SOFT DROP"))) {
			if (MoveTetromino(ActiveTetromino_, Movement::Down)) { Score_ += 1; }
		}
		else if (IsKeyHeld(hotkeys.at("MOVE RIGHT"))) {
			MoveTetromino(ActiveTetromino_, Movement::Right);
		}
		else if (IsKeyHeld(hotkeys.at("MOVE LEFT"))) {
			MoveTetromino(ActiveTetromino_, Movement::Left);
		}


		if (IsKeyPressed(hotkeys.at("ROTATE R"))) {
			MoveTetromino(ActiveTetromino_, Movement::RotateR);
		}
		else if (IsKeyPressed(hotkeys.at("ROTATE L"))) {
			MoveTetromino(ActiveTetromino_, Movement::RotateL);
		}
		else if (IsKeyPressed(hotkeys.at("ROTATE 180"))) {
			MoveTetromino(ActiveTetromino_, Movement::Rotate2);
		}

		if (IsKeyPressed(hotkeys.at("HARD DROP"))) {
			while (MoveTetromino(ActiveTetromino_, Movement::Down)) { Score_ += 2; }
			ActiveTetromino_.placed = true;
		}

		if (IsKeyPressed(hotkeys.at("SWAP"))) {
			SwapTetromino();
		}

		if (IsKeyPressed(hotkeys.at("RESTART"))) {
			Restart();
		}

		if (IsKeyPressed(hotkeys.at("PAUSE"))) {
			State_ = GameState::Pause;
		}

		if (IsKeyPressed(hotkeys.at("MAIN MENU"))) {
			State_ = GameState::MainMenu;
			HighlightedSettingsInd_ = 0;
		}

		if (IsKeyPressed(hotkeys.at("OPEN SETTINGS"))) {
			State_ = GameState::HotkeyMenu;
		}
		break;

	case GameState::Pause:
		if (IsKeyPressed(hotkeys.at("PAUSE"))) {
			State_ = GameState::CountDown;
		}
		else if (IsKeyPressed(hotkeys.at("OPEN SETTINGS"))) {
			State_ = GameState::HotkeyMenu;
		}
		else if (IsKeyPressed(hotkeys.at("MAIN MENU"))) {
			Restart();
			State_ = GameState::MainMenu;
			HighlightedSettingsInd_ = 0;
		}
		break;

	case GameState::GameOver:
	case GameState::YouWon:
		if (IsKeyPressed(hotkeys.at("RESTART"))) {
			Restart();
		}
		else if (IsKeyPressed(hotkeys.at("OPEN SETTINGS"))) {
			State_ = GameState::HotkeyMenu;
		}
		else if (IsKeyPressed(hotkeys.at("MAIN MENU"))) {
			Restart();
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
				else if (IsKeyPressed(hotkeys.at("OPEN SETTINGS"))) {
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
							hotkeys.at(hotkey.Name) = hotkey.Value;
						}
						Settings::save();

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
	case GameState::About:
		if (IsKeyPressed(GLFW_KEY_ENTER)) {
			State_ = GameState::MainMenu;
		}
		break;
	}
}