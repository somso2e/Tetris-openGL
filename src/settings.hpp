#pragma once
#include "common.hpp"
#include "nlohmann/json.hpp"
#include <filesystem>

namespace Settings {
	using namespace nlohmann;
	void init();
	struct Module {
		const char* Name;
		int Value;
	};
	extern std::vector<Module> Hotkeys;
	extern std::vector<Module> Default;
	extern ordered_json Json;
};