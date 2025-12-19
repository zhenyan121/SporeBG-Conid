#pragma once
#include <utility>
struct InputState
{
    std::pair<float, float> mouseCilckOn;
    bool leftMouseDown = false;
    std::pair<float, float> mouseCurrentPosition;
    bool isFullscreen = false;
};
