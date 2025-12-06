#pragma once
#include <string>

//  统一管理配置
struct GameConfig {
    int windowWidth = 1600;
    int windowHeight = 900;
    std::string windowTitle = "孢子棋";
    bool vsync = true;
} ;