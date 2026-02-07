#pragma once

#include <string>
#include <utility>
#include <glm/glm.hpp>
struct DebugData {
    bool showFPS = true;
    bool showMousePosition = true;
    glm::ivec2 mousePosition = {0, 0};
    glm::ivec2 mouseLogicalPostion = {0, 0};
    int currentFPS = 0;
    int round = 0;

};