#include "BitmapFont.h"

#include <SDL3_image/SDL_image.h>

#include <fstream>
#include <filesystem>
#include <cstdio>
#include <cstring>
static bool startsWith(const std::string& s, const char* prefix) {
    return s.size() >= std::strlen(prefix) &&
           s.compare(0, std::strlen(prefix), prefix) == 0;
}

////////////////////////////////////////////////////////////
// 构造 / 析构
////////////////////////////////////////////////////////////
BitmapFont::BitmapFont() = default;

BitmapFont::~BitmapFont() {
    for (SDL_Texture* tex : m_pages) {
        SDL_DestroyTexture(tex);
    }
    m_pages.clear();
}

////////////////////////////////////////////////////////////
// 加载 .fnt
////////////////////////////////////////////////////////////
bool BitmapFont::load(const std::string& fntPath, SDL_Renderer* renderer) {
    m_renderer = renderer;

    std::ifstream file(fntPath);
    if (!file.is_open()) {
        SDL_Log("BitmapFont: 无法打开 %s", fntPath.c_str());
        return false;
    }

    // .fnt 所在目录（用于加载 .tga）
    std::string baseDir =
        std::filesystem::path(fntPath).parent_path().string();

    std::string line;
    while (std::getline(file, line)) {

        // common 行：行高
        if (startsWith(line, "common ")) {
            sscanf(line.c_str(),
                "common lineHeight=%d",
                &m_lineHeight
            );
        }

        // page 行：加载贴图
        else if (startsWith(line, "page ")) {
            int id = 0;
            char filename[256]{};

            sscanf(line.c_str(),
                "page id=%d file=\"%255[^\"]\"",
                &id, filename
            );

            if ((int)m_pages.size() <= id) {
                m_pages.resize(id + 1, nullptr);
            }

            std::string texPath = baseDir + "/" + filename;

            SDL_Surface* surf = IMG_Load(texPath.c_str());
            if (!surf) {
                SDL_Log("BitmapFont: 无法加载贴图 %s", texPath.c_str());
                return false;
            }

            SDL_Texture* tex =
                SDL_CreateTextureFromSurface(renderer, surf);
            SDL_DestroySurface(surf);

            // 像素风关键：最近邻
            SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

            m_pages[id] = tex;
        }

        // char 行：字形数据
        else if (startsWith(line, "char ")) {
            uint32_t id = 0;
            BitmapGlyph g{};

            sscanf(line.c_str(),
                "char id=%u x=%d y=%d width=%d height=%d "
                "xoffset=%d yoffset=%d xadvance=%d page=%d",
                &id,
                &g.x, &g.y,
                &g.w, &g.h,
                &g.xOffset, &g.yOffset,
                &g.xAdvance,
                &g.page
            );

            m_glyphs[id] = g;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////
// 绘制文本
////////////////////////////////////////////////////////////
void BitmapFont::drawText(const std::string& text, int x, int y) const {
    int cursorX = x;
    int cursorY = y;

    for (uint32_t cp : utf8ToCodepoints(text)) {

        // 换行
        if (cp == '\n') {
            cursorX = x;
            cursorY += m_lineHeight;
            continue;
        }

        auto it = m_glyphs.find(cp);
        if (it == m_glyphs.end()) {
            continue;
        }

        const BitmapGlyph& g = it->second;

        SDL_FRect src{
            (float)g.x,
            (float)g.y,
            (float)g.w,
            (float)g.h
        };

        SDL_FRect dst{
            (float)(cursorX + g.xOffset),
            (float)(cursorY + g.yOffset),
            (float)g.w,
            (float)g.h
        };

        SDL_RenderTexture(
            m_renderer,
            m_pages[g.page],
            &src,
            &dst
        );

        cursorX += g.xAdvance;
    }
}

////////////////////////////////////////////////////////////
// 行高
////////////////////////////////////////////////////////////
int BitmapFont::getLineHeight() const {
    return m_lineHeight;
}

////////////////////////////////////////////////////////////
// UTF-8 → Unicode codepoint（最小但够用）
////////////////////////////////////////////////////////////
std::vector<uint32_t>
BitmapFont::utf8ToCodepoints(const std::string& text) {
    std::vector<uint32_t> result;

    for (size_t i = 0; i < text.size();) {
        uint8_t c = (uint8_t)text[i];

        if (c < 0x80) {
            result.push_back(c);
            i += 1;
        }
        else if ((c >> 5) == 0x6) {
            uint32_t cp =
                ((text[i] & 0x1F) << 6) |
                (text[i + 1] & 0x3F);
            result.push_back(cp);
            i += 2;
        }
        else if ((c >> 4) == 0xE) {
            uint32_t cp =
                ((text[i] & 0x0F) << 12) |
                ((text[i + 1] & 0x3F) << 6) |
                (text[i + 2] & 0x3F);
            result.push_back(cp);
            i += 3;
        }
        else if ((c >> 3) == 0x1E) {
            uint32_t cp =
                ((text[i] & 0x07) << 18) |
                ((text[i + 1] & 0x3F) << 12) |
                ((text[i + 2] & 0x3F) << 6) |
                (text[i + 3] & 0x3F);
            result.push_back(cp);
            i += 4;
        }
        else {
            // 非法 UTF-8，跳过
            i += 1;
        }
    }

    return result;
}
