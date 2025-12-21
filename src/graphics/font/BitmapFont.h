#pragma once

/*
    BitmapFont.h
    --------------------------------
    Bitmap Font（BMFont / AngelCode）渲染器
    - 支持 .fnt（文本格式）
    - 支持多 page（多张 .tga）
    - 支持 UTF-8 / 中文
    - SDL3 / 像素风（NEAREST）

    用法示例：
        BitmapFont font;
        font.load("assets/fonts/sanhan.fnt", renderer);
        font.drawText("你好，像素世界", 10, 10);
*/

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

////////////////////////////////////////////////////////////
// 单个字形（对应 .fnt 中的 char 行）
////////////////////////////////////////////////////////////
struct BitmapGlyph {
    int x = 0;        // 在贴图中的 x
    int y = 0;        // 在贴图中的 y
    int w = 0;        // 字形宽度
    int h = 0;        // 字形高度

    int xOffset = 0; // 绘制时 x 偏移
    int yOffset = 0; // 绘制时 y 偏移
    int xAdvance = 0;// 光标前进量

    int page = 0;    // 所属贴图页
};

////////////////////////////////////////////////////////////
// BitmapFont 类
////////////////////////////////////////////////////////////
class BitmapFont {
public:
    BitmapFont();
    ~BitmapFont();

    // 加载 .fnt 文件
    bool load(const std::string& fntPath, SDL_Renderer* renderer);

    // 绘制文本（UTF-8）
    void drawText(const std::string& text, int x, int y) const;

    // 获取行高（用于换行 / UI 布局）
    int getLineHeight() const;

private:
    // UTF-8 → Unicode codepoint
    static std::vector<uint32_t> utf8ToCodepoints(const std::string& text);

private:
    SDL_Renderer* m_renderer = nullptr;

    int m_lineHeight = 0;

    // Unicode → Glyph
    std::unordered_map<uint32_t, BitmapGlyph> m_glyphs;

    // 所有 page 对应的纹理
    std::vector<SDL_Texture*> m_pages;
};
