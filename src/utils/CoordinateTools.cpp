#include "CoordinateTools.h"

std::pair<int, int> physicalToLogical(const float screenX, const float screenY, SDL_Renderer* renderer) {
    if (renderer == nullptr) {
        SDL_Log("Renderer is null in physicalToLogical\n");
        return {static_cast<int>(screenX), static_cast<int>(screenY)};
    }
    
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