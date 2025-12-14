#include "Button.h"
#include "graphics/font/TextRenderer.h"

Button::Button()
{

}

Button::Button(TextRenderer* textRenderer) : m_textRenderer(textRenderer) {
}

Button::Button(
    const std::string& text,
    TextStyle style,
    int x,
    int y,
    TextRenderer* textRenderer,
    SDL_Color backgroundColor,
    int borderThickness,
    SDL_Color borderColor
) : m_textRenderer(textRenderer)
{
    m_rect.x = static_cast<float>(x);
    m_rect.y = static_cast<float>(y);
    m_buttonData.text = text;
    m_buttonData.textstytle = style;
    m_buttonData.backgroundColor = backgroundColor;
    m_buttonData.borderThickness = borderThickness;
    m_buttonData.borderColor = borderColor;

    // 如果提供了 TextRenderer，则立即测量文本并更新控件尺寸
    if (m_textRenderer) {
        auto [w, h] = m_textRenderer->getTextSize(text, style);
        m_rect.w = static_cast<float>(w);
        m_rect.h = static_cast<float>(h);
        m_buttonData.rect = m_rect;
    }
}


void Button::setText(const std::string& text, TextStyle style) {
    m_buttonData.text = text;
    m_buttonData.textstytle = style;

    // 如果提供了 TextRenderer，则立即测量文本并更新控件尺寸
    if (m_textRenderer) {
        auto [w, h] = m_textRenderer->getTextSize(text, style);
        m_rect.w = static_cast<float>(w);
        m_rect.h = static_cast<float>(h);
        m_buttonData.rect = m_rect;
    }
}

void Button::setText(const std::string& text) {
    m_buttonData.text = text;

    // 如果提供了 TextRenderer，则立即测量文本并更新控件尺寸
    if (m_textRenderer) {
        auto [w, h] = m_textRenderer->getTextSize(text, m_buttonData.textstytle);
        m_rect.w = static_cast<float>(w);
        m_rect.h = static_cast<float>(h);
        m_buttonData.rect = m_rect;
    }
}

void Button::setBackgroundColor(SDL_Color normal) {
    m_buttonData.backgroundColor = normal;
}

void Button::setBorder(int thickness, SDL_Color color) {
    m_buttonData.borderThickness = thickness;
    m_buttonData.borderColor = color;
}


void Button::setCallback(std::function<void()> callback) {
    m_callback = callback;
}

void Button::update(float deltaTime) {

}
bool Button::handleCilck(int x, int y) {

    if (m_callback && m_isEnabled && m_isVisible) {
        //SDL_Log("rect x: %f, y: %f, w: %f, h: %f", m_rect.x, m_rect.y, m_rect.w, m_rect.h);
        if (x >= m_rect.x && x <= m_rect.x + m_rect.w &&
            y >= m_rect.y && y <= m_rect.y + m_rect.h) {
            m_callback();
            return true;
        }
    }
    return false;
}






