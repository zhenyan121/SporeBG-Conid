
#include "FontManager.h"
#include <stdexcept>

FontManager::FontManager() {
    // 构造函数：初始化字体管理器
    // m_fonts 映射表会在默认构造时初始化为空
}

FontManager::~FontManager() {
    // 析构函数：清理所有加载的字体资源
    for (auto& pair : m_fonts) {
        // 关闭并释放每个字体资源
        TTF_CloseFont(pair.second);
    }
    // 清空字体缓存表
    m_fonts.clear();
}

TTF_Font* FontManager::loadFont(const std::string& fontID, int ptSize) {
    // 构造字体文件的完整路径
    std::string path = "assets/fonts/" + fontID;
    // 计算该字体在缓存中的唯一哈希键
    size_t key = makeHash(fontID, ptSize);
    
    // 检查字体是否已经被加载过
    auto it = m_fonts.find(key);
    if (it != m_fonts.end()) {
        // 字体已缓存，直接返回缓存的字体指针
        return it->second;
    }
    
    // 从文件中加载字体
    TTF_Font* font = TTF_OpenFont(path.c_str(), ptSize);
    if (!font) {
        // 字体加载失败，抛出异常
        throw std::runtime_error("无法加载字体: " + fontID);
    }
    TTF_SetFontHinting(font, TTF_HINTING_MONO);  // 单色渲染，不抗锯齿    
    // 将新加载的字体存入缓存
    m_fonts[key] = font;
    return font;
}

TTF_Font* FontManager::getFont(const std::string& fontID, int ptSize) {
    // 计算该字体的哈希键
    size_t key = makeHash(fontID, ptSize);
    
    // 从字体缓存中查找该键对应的字体
    auto it = m_fonts.find(key);
    if (it != m_fonts.end()) {
        // 字体已缓存，直接返回缓存的字体指针
        return it->second;
    }
    
    // 如果缓存中未找到，自动调用loadFont加载字体
    return loadFont(fontID, ptSize);
}

size_t FontManager::makeHash(const std::string& fontID, int ptSize) {
    // 分别计算字体名称和大小的哈希值
    size_t h1 = std::hash<std::string>{}(fontID);
    size_t h2 = std::hash<int>{}(ptSize);

    // 组合两个哈希值：使用XOR运算和位左移组合两个独立的哈希值
    // 这样可以确保不同的fontID或ptSize组合都会产生不同的哈希值
    return h1 ^ (h2 << 1);
}
