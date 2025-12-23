#include "TextRenderer.h"
#include <algorithm>
#include <vector>
#include <iostream>
TextRenderer::TextRenderer(SDL_Renderer* renderer, FontManager* fontManager, const Viewport& viewport) :
                            m_fontManager(fontManager),
                            m_renderer(renderer),
                            m_viewport(viewport)
{
    //m_bitmapFont = std::make_unique<BitmapFont>();
    //m_bitmapFont->load("assets/fonts/sanhan.fnt", renderer);
}

TextRenderer::~TextRenderer() {
    clearCache();
}

std::pair<int, int> TextRenderer::getLogicalTextSize(const std::string& text, TextStyle style) {
    auto key = makeHash(text, style);
    auto it = m_cache.find(key);
    
    // 查找缓存
    if (it != m_cache.end()) {
        // 更新最后访问时间
        it->second.lastAccessTime = std::time(nullptr);
        return {it->second.width, it->second.height};
    }

    // 创建并缓存纹理
    CachedText cached = createAndCacheTexture(text, style);
    if (!cached.texture) {
        return {0, 0};
    }

    return {cached.width / m_viewport.scale, cached.height / m_viewport.scale};
}

void TextRenderer::renderText(const std::string& text, TextStyle style, int logicalX, int logicalY) {
    
    CachedText& cached = createAndCacheTexture(text, style);
    if (!cached.texture) {
        return;
    }

    // ===============================
    // 1. 逻辑坐标 → 窗口坐标
    // ===============================
    SDL_FPoint winPos = {
        m_viewport.dst.x + logicalX * m_viewport.scale,
        m_viewport.dst.y + logicalY * m_viewport.scale
    };

    SDL_FRect dst {
        winPos.x,
        winPos.y,
        static_cast<float>(cached.width),
        static_cast<float>(cached.height)
    };
    //std::cout << "Rendering text at (" << dst.x << ", " << dst.y << ") with size (" << dst.w << ", " << dst.h << ")\n";
    
    // 渲染
    SDL_RenderTexture(
        m_renderer,
        cached.texture,
        nullptr,
        &dst
    );
    
   //m_bitmapFont->drawText(text, x, y);
}

TextRenderer::CachedText& TextRenderer::createAndCacheTexture(const std::string& text, TextStyle style) {
    auto key = makeHash(text, style);
    CachedText& slot = m_cache[key];
    // empty: 用于失败返回的只读哨兵对象（texture == nullptr）
    static CachedText empty{};
    // 如果已经存在，直接返回
    if (slot.texture) {
        slot.lastAccessTime = std::time(nullptr);
        return slot;
    }
    if (slot.texture) {
        SDL_DestroyTexture(slot.texture);
    }
    // 创建逻辑
    slot = {}; // reset
    slot.lastAccessTime = std::time(nullptr);
    /*
    // 获取字体 - 需要大号字体用于高清渲染
    const int TARGET_SCALE = 4; // 4倍超采样
    int largeFontSize = style.fontSize * TARGET_SCALE;
    TTF_Font* largeFont = m_fontManager->getFont(style.fontID, largeFontSize);

    if (!largeFont) {
        SDL_Log("错误：字体未找到 %s\n", style.fontID.c_str());
        return result;
    }

     // 1. 创建高清离屏纹理（大尺寸）
    int targetWidth = 0, targetHeight = 0;

    // SDL3 使用 TTF_GetTextSize 替代 TTF_SizeText
    
    if (TTF_GetTextSize(largeFont, text.c_str(), &targetWidth, &targetHeight) < 0) {
        SDL_Log("错误：无法测量文本尺寸: %s\n", text.c_str());
        return result;
    }
    
    // 计算高清纹理尺寸
    int hiResWidth = targetWidth * TARGET_SCALE;
    int hiResHeight = targetHeight * TARGET_SCALE;
    
    // 创建高清渲染目标纹理
    SDL_Texture* hiResTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        hiResWidth,
        hiResHeight
    );
    
    if (!hiResTexture) {
        SDL_Log("错误：无法创建高清纹理\n");
        return result;
    }
    
    // 设置高清纹理属性
    SDL_SetTextureScaleMode(hiResTexture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(hiResTexture, SDL_BLENDMODE_BLEND);
    
    // 2. 渲染到大尺寸高清纹理
    SDL_Texture* prevTarget = SDL_GetRenderTarget(m_renderer);
    SDL_SetRenderTarget(m_renderer, hiResTexture);
    
    // 用透明色清空高清纹理
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);
    

    // 创建文字表面
    SDL_Surface* surface = TTF_RenderText_Solid(largeFont, text.c_str(),text.length(), style.color);
    if (!surface) {
        SDL_Log("错误：无法创建文字表面 '%s'\n", text.c_str());
        return result;
    }
    */
    // 获取字体
    TTF_Font* font = m_fontManager->getFont(style.fontID, style.fontSize * m_viewport.scale);
    if (!font) {
        SDL_Log("错误：字体未找到 %s\n", style.fontID.c_str());
        m_cache.erase(key);
        return empty;
    }
    
    // 创建文字表面
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(),text.length(), style.color);
    if (!surface) {
        SDL_Log("错误：无法创建文字表面 '%s'\n", text.c_str());
        m_cache.erase(key);
        return empty;
    }
    
    // 创建纹理
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    int width = surface->w;
    int height = surface->h;
    SDL_DestroySurface(surface);  // 立即释放表面
    
    if (!texture) {
        SDL_Log("错误：无法创建纹理\n");
        m_cache.erase(key);
        return empty;
    }
    // 设置纹理缩放模式为最近邻
    //SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    // 保存结果
    slot.texture = texture;
    slot.width = width;
    slot.height = height;
    slot.lastAccessTime = std::time(nullptr);
    
   
    // 检查是否需要清理缓存
    if (m_cache.size() > MAX_CACHE_SIZE) {
        autoCleanCache(key);
    }

    return slot;
}


std::string makeCacheKey(const std::string& text, const std::string& fontID, SDL_Color color) {
        return text + "|" + fontID + "|" + 
               std::to_string(color.r) + "-" +
               std::to_string(color.g) + "-" +
               std::to_string(color.b) + "-" +
               std::to_string(color.a);
    }



size_t TextRenderer::makeHash(const std::string& text, TextStyle style) const {
        // 使用组合哈希技术
        
        size_t h1 = std::hash<std::string>{}(style.fontID);
        size_t h2 = std::hash<std::string>{}(text);
        size_t h3 = std::hash<int>{}(style.fontSize);
        size_t h4 = std::hash<Uint8>{}(style.color.r);
        size_t h5 = std::hash<Uint8>{}(style.color.g);
        size_t h6 = std::hash<Uint8>{}(style.color.b);
        size_t h7 = std::hash<Uint8>{}(style.color.a);
        
        // 组合哈希（使用黄金比例乘法混合）
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^ (h7 << 6);
    }


void TextRenderer::clearCache() {
    for (auto& pair : m_cache) {
        SDL_DestroyTexture(pair.second.texture);
    }
    m_cache.clear();
    SDL_Log("文本缓存已清空\n");
}


void TextRenderer::autoCleanCache(size_t keepKey) {
    // 如果缓存没有超过最大限制，则不进行清理
    if (m_cache.size() <= MAX_CACHE_SIZE) {
        return;
    }

    // 收集所有缓存项，按最后访问时间排序
    std::vector<std::pair<size_t, time_t>> cacheItems;
    cacheItems.reserve(m_cache.size());
    
    for (const auto& pair : m_cache) {
        if (pair.first == keepKey) continue;
        cacheItems.emplace_back(pair.first, pair.second.lastAccessTime);
    }
    
    // 按最后访问时间从早到晚排序（最旧的在前）
    std::sort(cacheItems.begin(), cacheItems.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
    
    // 计算需要删除的数量
    size_t itemsToRemove = m_cache.size() - MIN_CACHE_SIZE;
    
    SDL_Log("缓存清理：删除 %zu / %zu 条目(当前缓存大小：%zu)\n", 
            itemsToRemove, m_cache.size(), m_cache.size());
    
    // 删除最旧的缓存项
    for (size_t i = 0; i < itemsToRemove && i < cacheItems.size(); ++i) {

        auto it = m_cache.find(cacheItems[i].first);
        if (it != m_cache.end()) {
           
            SDL_DestroyTexture(it->second.texture);
            m_cache.erase(it);
        }
    }
    
    SDL_Log("缓存清理完成，当前缓存大小：%zu\n", m_cache.size());
}