#include "TextRenderer.h"

TextRenderer::TextRenderer(FontManager& fontManager,SDL_Renderer* renderer) :
                            m_fontManager(fontManager),
                            m_renderer(renderer)
{

}

TextRenderer::~TextRenderer() {
     for (auto& pair : m_cache) {
        
            SDL_DestroyTexture(pair.second.texture);
        
    }
    m_cache.clear();
}



void TextRenderer::renderText(const std::string& text, const std::string& fontID, int x, int y, SDL_Color color) {
    std::string key = makeCacheKey(text, fontID, color);
    auto it = m_cache.find(key);
    
    // 查找缓存
    if (it != m_cache.end()) {
        // 使用缓存的纹理,  SDL_FRect为浮点数矩形
            SDL_FRect dest = { static_cast<float>(x), static_cast<float>(y), 
                               static_cast<float>(it->second.width), 
                               static_cast<float>(it->second.height) };

            // 绘制材质 NULL 的含义：绘制整个纹理（从 (0,0) 到纹理的完整宽高） &dest 目标区域
            SDL_RenderTexture(m_renderer, it->second.texture, NULL, &dest);
            return;
    }

    // 创建新的纹理
    TTF_Font* font = m_fontManager.getFont(fontID);
    if (!font) {
        SDL_Log("错误：字体未找到 %s\n", fontID.c_str());
        return;
    }
    // 创建文字表面
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(),text.length(), color);
    if (!surface) {
        printf("错误：无法创建文字表面\n");
        return;
    }
    // 创建纹理
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!texture) {
        SDL_Log("错误：无法创建纹理\n");
        SDL_DestroySurface(surface);
        return;
    }
    // 保存到缓存
    CachedText cached;
    cached.texture = texture;
    cached.width = surface->w;
    cached.height = surface->h;
    m_cache[key] = cached;


    // 渲染
    SDL_FRect dest = { static_cast<float>(x), static_cast<float>(y), 
                        static_cast<float>(surface->w), 
                        static_cast<float>(surface->h) };
    SDL_RenderTexture(m_renderer, texture, NULL, &dest);
    
    // 清理表面
    SDL_DestroySurface(surface);


}

SDL_Texture* TextRenderer::createTextTexture(const std::string& text, const std::string& fontID, SDL_Color color) {

}


std::string makeCacheKey(const std::string& text, const std::string& fontID, SDL_Color color) {
        return text + "|" + fontID + "|" + 
               std::to_string(color.r) + "-" +
               std::to_string(color.g) + "-" +
               std::to_string(color.b) + "-" +
               std::to_string(color.a);
    }
