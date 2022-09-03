#include "game.hpp"


void Game::SaveToMemoryReplay(bool_map state, bool_map previousState, uint32_t done) {
	uint32_t action = ActiveTetromino_.pos.x * ActiveTetromino_.rotation;
	Buffer_.push_back({
		previousState,
		state,
		done,
		action,
		CalculateReward(), }
	);

	// Only save to 
	if (Buffer_.size() >= MAX_BUFFER_SIZE || done) {
		std::ofstream file("memory_replay.somso", std::ios::app | std::ios::binary);
		for (auto& replay : Buffer_) {
			file.write(reinterpret_cast<char*>(&replay), sizeof(replay));
		}
		Buffer_.clear();
		file.close();
	}
}

Game::reward Game::CalculateReward() {
	reward reward{};
	for (int x = 0; x < NUM_OF_CELLS_W; x++) {
		for (int y = 0; y < NUM_OF_CELLS_H; y++) {
			if (BoolDisplayMap_.at(x).at(y)) {
				reward.heights.at(x) = NUM_OF_CELLS_H - 1 - y;
				int filledCells = 0;
				for (int y2 = y + 1; y2 < NUM_OF_CELLS_H - 1; y2++) {
					filledCells += BoolDisplayMap_.at(x).at(y + 1);
				}
				reward.bubbles += reward.heights.at(x) - filledCells;
				break;
			}
		}
	}

	reward.score = Score_;
	reward.lines = TotalLinesCleared_;
	return reward;
}