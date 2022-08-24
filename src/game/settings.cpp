#include "settings.hpp"

//using namespace nlohmann;
std::vector<Settings::Module> Settings::Hotkeys;
std::vector<Settings::Module> Settings::DefaultHotkeys;
nlohmann::ordered_json Settings::Json;

void Settings::init() {
	DefaultHotkeys.insert(DefaultHotkeys.end(), {
		{"MOVE RIGHT", GLFW_KEY_RIGHT},
		{"MOVE LEFT", GLFW_KEY_LEFT},
		{"SOFT DROP", GLFW_KEY_DOWN},
		{"HARD DROP", GLFW_KEY_SPACE},
		{"ROTATE R", GLFW_KEY_UP},
		{"ROTATE L", GLFW_KEY_Z},
		{"ROTATE 180", GLFW_KEY_A},
		{"SWAP", GLFW_KEY_C},
		{"PAUSE", GLFW_KEY_P},
		{"RESTART", GLFW_KEY_R},
		{"OPEN SETTINGS", GLFW_KEY_GRAVE_ACCENT},
		{"MAIN MENU",GLFW_KEY_M}
		});
	Hotkeys = DefaultHotkeys;

	std::filesystem::path path("settings.json");
	bool validJSON = true;

	if (std::filesystem::exists(path)) {
		// Read json file from disk
		std::ifstream file("settings.json");
		Json = json::parse(file);
		file.close();

		auto& j = Json["settings"]["hotkeys"];
		for (int i = 0; i < DefaultHotkeys.size(); i++) {
			const auto& name = DefaultHotkeys.at(i).Name;
			// Load from json
			if (j.find(name) != j.end()) {
				Hotkeys.at(i).Value = j.at(name);
			}
		}

	}
	else {
		Hotkeys = DefaultHotkeys;

		auto& j = Json["settings"]["hotkeys"];
		for (const auto& h : Hotkeys) {
			j[h.Name] = h.Value;
		}
		auto& r = Json["high scores"];
		r["classic"] = json::array();
		r["fourty lines"] = json::array();
		save();
	}
}
void Settings::save() {
	std::ofstream file("settings.json");
	file << Json.dump(1, '\t');
	file.close();
}
