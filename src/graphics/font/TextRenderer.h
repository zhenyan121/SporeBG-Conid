#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "FontManager.h"

class TextRenderer {
private:
    SDL_Renderer* m_renderer;
    FontManager& m_fontManager;

    // 缓存文字纹理

    
    struct CachedText {
        SDL_Texture* texture;
        int width;
        int height;
    };

    std::unordered_map<std::string, CachedText> m_cache;
    // 创建材质
    SDL_Texture* createTextTexture(const std::string& text, const std::string& fontID, SDL_Color color);

    // 创建一个唯一的缓存键：文本 + 字体 + 颜色
    std::string makeCacheKey(const std::string& text, const std::string& fontID, SDL_Color color);

    


public:
    TextRenderer(FontManager& fontManager, SDL_Renderer* renderer);
    ~TextRenderer();

    
    //渲染文本

    void renderText(const std::string& text, const std::string& fontID, int x, int y, SDL_Color color);

};