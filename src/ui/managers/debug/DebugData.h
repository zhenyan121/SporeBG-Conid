#pragma once

#include <string>
#include <utility>

struct DebugData {
    bool showFPS = true;
    bool showMousePosition = true;
    std::pair<int, int> mousePosition = {0, 0};
    std::pair<int, int> mouseLogicalPostion = {0, 0};
    int currentFPS = 0;
    int round = 0;

};