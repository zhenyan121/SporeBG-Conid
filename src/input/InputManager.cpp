#include "InputManager.h"

SDL_AppResult InputManager::handleInputEvent(const SDL_Event* event, const Renderer* g_renderer, Game* g_game) {

    switch (event->type) {
        // 如果检测到退出
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event->button.button == SDL_BUTTON_LEFT) {
                // 将窗口坐标转为逻辑坐标
                float logicalX, logicalY;
                SDL_RenderCoordinatesFromWindow(
                    g_renderer->getSDLRenderer(),
                    static_cast<float>(event->button.x),
                    static_cast<float>(event->button.y),
                    &logicalX,
                    &logicalY
                );
                auto boardArea = g_renderer->getBoardArea();
                auto click = handleMouseClick(static_cast<int>(logicalX), static_cast<int>(logicalY), boardArea);
                if (click) {
                    auto [row, col] = click.value();
                    SDL_Log("click on (%d, %d)", row, col);
                    g_game->handleCoordinateInput(row, col);
                    g_game->printBoard();
                } else {
                    SDL_Log("invail cilck aera!");
                }
                return SDL_APP_CONTINUE;
            }
    }
    return SDL_APP_CONTINUE;
}

std::optional<std::pair<int, int>> 
InputManager::handleMouseClick(int mouseX, int mouseY, const ui::BoardArea& area) {
    // 判断是否点击在棋盘区域内
    if (mouseX < area.x || mouseX >= area.x + area.cellSize * area.cols ||
        mouseY < area.y || mouseY >= area.y + area.cellSize * area.rows) {
        return std::nullopt; // 点击在棋盘外
    }

    // 转换为逻辑坐标
    int col = (mouseX - area.x) / area.cellSize;
    int row = (mouseY - area.y) / area.cellSize;

    // 安全检查（通常不需要，但保险）
    if (row >= 0 && row < area.rows && col >= 0 && col < area.cols) {
        return std::pair<int, int>{row, col};
    }
    return std::nullopt;
}