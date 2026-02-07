#pragma once
#include <utility>
#include <glm/glm.hpp>
struct InputState
{
    glm::vec2 mouseCilckOn;
    bool leftMouseDown = false;
    glm::vec2 mouseCurrentPosition;
    glm::ivec2 mouseCurrentLogicalPosition;
    bool isFullscreen = false;
};
