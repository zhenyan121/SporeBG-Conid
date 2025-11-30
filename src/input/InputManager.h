#pragma once
#include <SDL3/SDL.h>
#include <optional>
#include <utility>
#include "ui/UIType.h"
class Renderer;
class Game;
class InputManager {
public:

    SDL_AppResult handleInputEvent(const SDL_Event* event, const Renderer* g_renderer, Game* g_game);
    // 调用前需传入当前棋盘渲染区域（来自 Renderer）
    std::optional<std::pair<int, int>> handleMouseClick(int mouseX, int mouseY, const ui::BoardArea& boardArea);
};