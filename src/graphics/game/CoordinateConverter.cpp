#include "CoordinateConverter.h"

CoordinateConverter::CoordinateConverter(SDL_Renderer* renderer) : m_renderer(renderer) {

}

CoordinateConverter::~CoordinateConverter() {

}

std::optional<std::pair<int, int>> CoordinateConverter::ScreenToBoard(int mouseX, int mouseY, BoardArea area) {
    
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