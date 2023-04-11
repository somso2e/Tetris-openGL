#include "settings.hpp"

//using namespace nlohmann;
std::vector<Settings::Module> Settings::Hotkeys;
std::vector<Settings::Module> Settings::DefaultHotkeys;
nlohmann::ordered_json Settings::Json;
nlohmann::ordered_json Settings::DefaultJson;

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

	DefaultJson["high scores"]["classic"] = json::array();
	DefaultJson["high scores"]["fourty lines"] = json::array();

	DefaultJson["settings"]["record data"] = true;

	DefaultJson["settings"]["Auto Repeat time"] = 0.033;
	DefaultJson["settings"]["Delayed Auto Shift"] = 0.167;
	//DefaultJson["settings"]["DAS Cut Delay"] = 0.033; 
	DefaultJson["settings"]["Soft Drop Speed"] = 0.033;


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
			else {
				Hotkeys.at(i).Value = DefaultHotkeys.at(i).Value;
			}
		}
		for (const auto& [key1, val1] : DefaultJson.items()) {
			for (const auto& [key2, val2] : DefaultJson.at(key1).items()) {
				if (Json[key1].find(key2) == Json[key1].end()) {
					Json[key1][key2] = DefaultJson[key1].at(key2);
				}
			}
		}
	}
	else {
		Json = DefaultJson;
		auto &j = Json["settings"]["hotkeys"];
        for (const auto &[key, val] : DefaultHotkeys) {
            j[key] = val;
        }
	}
	save();
}
void Settings::save() {
	std::ofstream file("settings.json");
	file << Json.dump(1, '\t');
	file.close();
}

