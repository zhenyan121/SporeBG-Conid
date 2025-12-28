#pragma once

#include "Config.h"
#include <nlohmann/json.hpp>

class ConfigLoader {
public:
    static bool load(const std::string& path, Config& config);
    static Config load(const std::string&path);

};
