#pragma once
#include "utils/Config.h"
#include <SDL3/SDL.h>
#include <utility>
#include <optional>
class CoordinateConverter {
public:
    CoordinateConverter(SDL_Renderer* renderer);
    ~CoordinateConverter();
    // 将物理坐标转化成逻辑坐标
    std::optional<std::pair<int, int>> ScreenToBoard(int mouseX, int mouseY, BoardArea aera);

private:
    SDL_Renderer* m_renderer;
};