#include "settings.hpp"

//using namespace nlohmann;
std::vector<Settings::Module> Settings::Hotkeys;
std::vector<Settings::Module> Settings::Default;
nlohmann::ordered_json Settings::Json;

void Settings::init() {
	Default.insert(Default.end(), {
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
	Hotkeys = Default;

	std::ifstream file("settings.json");
	std::filesystem::path path("settings.json");
	bool validJSON = true;

	if (std::filesystem::exists(path)) {
		Json = json::parse(file);
		file.close();

		auto& j = Json["settings"]["hotkeys"];
		for (const auto& module : Hotkeys) {
			if (j.find(module.Name) == j.end()) {
				validJSON = false;
				std::remove("settings.json");
				break;
			}
		}
	}
	if (!validJSON || !std::filesystem::exists(path)) {
		auto& j = Json["settings"]["hotkeys"];
		for (const auto& module : Hotkeys) {
			j[module.Name] = module.Value;
		}
		std::ofstream file("settings.json");
		file << Json.dump(1, '\t');
	}
}
