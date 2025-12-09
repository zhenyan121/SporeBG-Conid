#include "FontManager.h"
#include <stdexcept>
FontManager::FontManager() {

}

FontManager::~FontManager() {
    for (auto& pair : m_fonts) {
        TTF_CloseFont(pair.second);
    }

    m_fonts.clear();

   
}

TTF_Font* FontManager::loadFont(const std::string& fontID, int ptSize) {
    std::string path = "assets/fonts/" + fontID;
    std::string key = fontID + std::to_string(ptSize);
    auto it = m_fonts.find(key);
    // 检查字体是否已经加载
    if (it != m_fonts.end()) {
        return it->second;
    }

    TTF_Font* font =  TTF_OpenFont(path.c_str(), ptSize);
    if (!font) {
        throw std::runtime_error("无法加载字体: " + key);
    }
    m_fonts[key] = font;
    return font;
}

TTF_Font* FontManager::getFont(const std::string& key) {
    auto it = m_fonts.find(key);
    return (it != m_fonts.end()) ? it->second : nullptr;
}



