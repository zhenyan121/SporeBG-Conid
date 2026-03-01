#pragma once
#include <utility>
#include "Config.h"
#include <cmath>
#include <glm/glm.hpp>
static inline float easeInOutSine(float t) {
    return -(cos(M_PI * t) - 1) / 2.0f;
}
namespace Tools {
    inline glm::ivec2 physicalToLogical(glm::vec2 physicalPos, const Viewport& viewport) {
        glm::ivec2 logicalPoint = {0 , 0};
        
        // 计算相对于目标矩形的坐标
        float normalizedX = (physicalPos.x - viewport.dst.x) / viewport.dst.w;
        float normalizedY = (physicalPos.y - viewport.dst.y) / viewport.dst.h;
        
        // 转换为逻辑坐标
        logicalPoint.x = static_cast<int>(normalizedX * viewport.logicalWidth);
        logicalPoint.y = static_cast<int>(normalizedY * viewport.logicalHeight);

        return logicalPoint;
    }

    
    // t ∈ [0, 1]：0=开始，0.5=最远点，1=回到原点（带震荡）
    // b = 起始值（原位置）
    // c = 偏移量（最大偏移距离）
    inline float pingPongSpring(float t, float b, float c) {
        if (t <= 0.0f) return b;
        if (t >= 1.0f) return b; // 最终回到原点！
        float overshoot = 1.70158f;
        if (t < 0.5f) {
            // 前半段：从 0 → 1（冲到最远点）
            t *= 2.0f; // 映射到 [0,1]
            float val = t * t * ((overshoot + 1) * t - overshoot);
            return b + c * val;
        } else {
            // 后半段：从 1 → 0（从最远点弹回原点，带过冲）
            t = (t - 0.5f) * 2.0f; // 映射到 [0,1]
            t -= 1;
            float val = t * t * ((overshoot + 1) * t + overshoot) + 1;
            return b + c * (1.0f - val); // 从 c 往回减到 0
        }
    }

    // 通用插值函数
    inline float smoothMove(float t, float start, float end) {
        float progress = easeInOutSine(t); // t ∈ [0,1]
        return start + (end - start) * progress;
    }


    // 辅助函数：HSV -> RGB
    inline void hsvToRgb(float h, float s, float v, Uint8& r, Uint8& g, Uint8& b) {
        float c = v * s;
        float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
        float m = v - c;
        float r_, g_, b_;
        if (h < 60)      { r_ = c; g_ = x; b_ = 0; }
        else if (h < 120) { r_ = x; g_ = c; b_ = 0; }
        else if (h < 180) { r_ = 0; g_ = c; b_ = x; }
        else if (h < 240) { r_ = 0; g_ = x; b_ = c; }
        else if (h < 300) { r_ = x; g_ = 0; b_ = c; }
        else             { r_ = c; g_ = 0; b_ = x; }
        r = static_cast<Uint8>((r_ + m) * 255);
        g = static_cast<Uint8>((g_ + m) * 255);
        b = static_cast<Uint8>((b_ + m) * 255);
    }

}