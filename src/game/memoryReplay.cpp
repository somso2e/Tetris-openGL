#include "game.hpp"


void Game::SaveToMemoryReplay(bool_map state, bool_map previousState, uint32_t done, uint32_t action) {
	Buffer_.push_back({
		previousState,
		state,
		done,
		action,
		Score_,
		TotalLinesCleared_, }
	);
	if (Buffer_.size() >= MAX_BUFFER_SIZE || done) {
		FlushMemoryBuffer();
	}
}

void Game::FlushMemoryBuffer() {
	std::ofstream file("memory_replay.somso", std::ios::app | std::ios::binary);
	for (auto& replay : Buffer_) {
		file.write(reinterpret_cast<char*>(&replay), sizeof(replay));
	}
	Buffer_.clear();
	file.close();
}