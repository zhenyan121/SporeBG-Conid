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
    std::string fontID;           ///< 字体标识符（在FontManager中注册的ID）
    int fontSize = 16;            ///< 字体大小（像素）
    SDL_Color color = {255, 255, 255, 255};  ///< 文本颜色（RGBA）
    
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
    
    /**
     * @brief 计算样式的哈希值（用于unordered_map）
     * @return size_t 哈希值
     */
    size_t hash() const {
        // 使用组合哈希技术
        size_t h1 = std::hash<std::string>{}(fontID);
        size_t h2 = std::hash<int>{}(fontSize);
        size_t h3 = std::hash<Uint8>{}(color.r);
        size_t h4 = std::hash<Uint8>{}(color.g);
        size_t h5 = std::hash<Uint8>{}(color.b);
        size_t h6 = std::hash<Uint8>{}(color.a);
        
        // 组合哈希（使用黄金比例乘法混合）
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5);
    }
};