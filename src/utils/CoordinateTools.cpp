#include "CoordinateTools.h"

std::pair<int, int> physicalToLogical(const float screenX, const float screenY, SDL_Renderer* renderer) {
    // 将窗口坐标转为逻辑坐标
    float logicalX, logicalY;
    SDL_RenderCoordinatesFromWindow(
        renderer,
        static_cast<float>(screenX),
        static_cast<float>(screenY),
        &logicalX,
        &logicalY
    );
    int mouseX = static_cast<int>(logicalX);
    int mouseY = static_cast<int>(logicalY);
    return {logicalX, logicalY};
}