#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class FontManager {
public:
    FontManager();
    ~FontManager();

    // 加载字体（路径 + 字号）
    // ptSize 为字号
    TTF_Font* loadFont(const std::string& fontID, int ptSize);
    
    // 获取已加载的字体
    TTF_Font* getFont(const std::string& key, int ptSize);

   

private:
    // 用哈希表存储字体
    std::unordered_map<std::string, TTF_Font*> m_fonts;

    
};