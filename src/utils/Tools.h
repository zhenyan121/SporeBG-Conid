#pragma once
#include <utility>
#include "Config.h"

namespace Tools {
    inline std::pair<int, int> physicalToLogical(float physicalX, float physicalY, const Viewport& viewport) {
        std::pair<int, int> logicalPoint = {0 , 0};
        
        // 计算相对于目标矩形的坐标
        float normalizedX = (physicalX - viewport.dst.x) / viewport.dst.w;
        float normalizedY = (physicalY - viewport.dst.y) / viewport.dst.h;
        
        // 转换为逻辑坐标
        logicalPoint.first = static_cast<int>(normalizedX * viewport.logicalWidth);
        logicalPoint.second = static_cast<int>(normalizedY * viewport.logicalHeight);

        return logicalPoint;
    }
}