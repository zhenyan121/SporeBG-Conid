#include "TextureManager.h"

TextureManager::TextureManager(SDL_Renderer* renderer) :
    m_renderer(renderer)
{


}

TextureManager::~TextureManager() {
    cleanupAllTexture();
}


void TextureManager::cleanupAllTexture() {

    for (auto [id, texture] : m_cacheTexture) {
        SDL_DestroyTexture(texture);
    }

    SDL_Log("TextureManager: cleanup all texture successful\n");

}



SDL_Texture* TextureManager::createTextureFromRect(SDL_FRect& rect, SDL_Color& color) {
    
    // 先在缓存中查找是否存在
    auto it = getTexture(rect, color);
    if(it) {
        return it;
    }
    if (!m_renderer) {
        SDL_Log("TextureManager renderer is null\n");
        return nullptr;
    }
    
    auto newTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        rect.w,
        rect.h
    );
    // 开启纹理混合
    SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
    // 保存当前的渲染目标
    auto currentTexture = SDL_GetRenderTarget(m_renderer);
    
    SDL_SetRenderTarget(m_renderer, newTexture);
    SDL_SetTextureScaleMode(currentTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    // 因为修改了渲染目标，所以坐标系不一样了
    SDL_FRect renderRect = {0, 0, rect.w, rect.h};
    SDL_RenderFillRect(m_renderer, &renderRect);
    // 缓存
    m_cacheTexture.emplace(makeHash(rect, color), newTexture);

    // 恢复渲染目标
    SDL_SetRenderTarget(m_renderer, currentTexture);
    return newTexture;
}

size_t TextureManager::makeHash(SDL_FRect& rect, SDL_Color& color) {
    // 分别计算字体名称和大小的哈希值
    size_t h1 = std::hash<float>{}(rect.x);
    size_t h2 = std::hash<float>{}(rect.y);
    size_t h3 = std::hash<float>{}(rect.w);
    size_t h4 = std::hash<float>{}(rect.h);

    size_t h5 = std::hash<Uint8>{}(color.r);
    size_t h6 = std::hash<Uint8>{}(color.g);
    size_t h7 = std::hash<Uint8>{}(color.b);
    size_t h8 = std::hash<Uint8>{}(color.a);

    // 组合两个哈希值：使用XOR运算和位左移组合两个独立的哈希值
    // 这样可以确保不同的fontID或ptSize组合都会产生不同的哈希值
    return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^ (h7 << 6) ^ (h8 << 7);
}

SDL_Texture* TextureManager::getTexture(SDL_FRect& rect, SDL_Color& color) {
    auto key = makeHash(rect, color);
    auto it = m_cacheTexture.find(key);

    if (it != m_cacheTexture.end()) {
        return it->second;
    } else {
        SDL_Log("TextureManager: texture not exist!");
        return nullptr;
    }
}

bool TextureManager::destoryTexture(SDL_FRect& rect, SDL_Color& color) {
    int key = makeHash(rect, color);
    auto it = m_cacheTexture.find(key);
    if (it == m_cacheTexture.end()) {
        SDL_Log("can't find the texture\n");
        return false;
        
    }
    SDL_DestroyTexture(it->second);
    m_cacheTexture.erase(key);
    SDL_Log("TextureManager: destory texture sucessfully\n");
    return true;
}
