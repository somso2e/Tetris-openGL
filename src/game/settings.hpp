#pragma once
#include "common.hpp"
#include "nlohmann/json.hpp"
#include <filesystem>

namespace Settings {
	using namespace nlohmann;
	void init();
	void save();
	struct Module {
		const char* Name;
		int Value;
	};
	extern std::vector<Module> Hotkeys;
	extern std::vector<Module> DefaultHotkeys;
	extern ordered_json Json;
	const int NUM_OF_TOP_SCORES = 10;
};