#pragma once
#include <utility>
struct InputState
{
    std::pair<float, float> mouseCilckOn;
    bool leftMouseDown = false;
    std::pair<float, float> mouseCurrentPosition;
    std::pair<int, int> mouseCurrentLogicalPosition;
    bool isFullscreen = false;
};
