#include "ConfigLoader.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;


static void loadWindow(const json& j, WindowConfig& window) {
    auto get = [&](const char* key, auto& out) {
        if (j.contains(key))
            j.at(key).get_to(out);
        else {
            std::cout << "Unkonw key " << key << "\n";
        }
    };
    get("width", window.width);
    get("height", window.height);
    get("title", window.title);
}

static void loadRender(const json& j, RenderConfig& render) {
    auto get = [&](const char* key, auto& out) {
        if (j.contains(key))
            j.at(key).get_to(out);
        else {
            std::cout << "Unkonw key " << key << "\n";
        }
    };
    get("logical_width", render.logicalWidth);
    get("logical_height", render.logicalHeight);
}

bool ConfigLoader::load(const std::string& path, Config& config) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "[Config] Cannot open " << path << "\n";
        return false;
    }

    json j;

    try {
    file >> j;
    }
    catch (const json::parse_error& e) {
        std::cerr << "[Config] JSON parse error: " << e.what() << std::endl;
        return false;
    }

    if (j.contains("window")) {
        loadWindow(j["window"], config.window);
    }
    if (j.contains("render")) {
        loadRender(j["render"], config.render);
    }
    std::cout << "load json success!\n";
    return true;

}