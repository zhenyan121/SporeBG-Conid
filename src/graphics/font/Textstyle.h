#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

/**
 * @brief 文本样式定义结构体
 * 
 * 封装字体、大小、颜色等文本渲染属性
 * 用于统一管理文本外观
 */
struct TextStyle {
    std::string fontID = "SourceHanSansSC-Regular.otf";           ///< 字体标识符（在FontManager中注册的ID）
    int fontSize = 24;            ///< 字体大小（像素）
    SDL_Color color = {0, 0, 0, 255};  ///< 文本颜色（RGBA）
    
    /**
     * @brief 比较两个样式是否相等（用于缓存查找）
     * @param other 另一个TextStyle对象
     * @return true 如果所有属性都相等
     */
    bool operator==(const TextStyle& other) const {
        return fontID == other.fontID &&
               fontSize == other.fontSize &&
               color.r == other.color.r &&
               color.g == other.color.g &&
               color.b == other.color.b &&
               color.a == other.color.a;
    }
};