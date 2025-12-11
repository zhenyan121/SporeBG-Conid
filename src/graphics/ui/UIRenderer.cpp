#include "UIRenderer.h"

UIRenderer::UIRenderer(SDL_Renderer* renderer, TextRenderer* textRenderer) :
    m_renderer(renderer), 
    //m_fontManager(fontManager),
    m_textRenderer(textRenderer)
{

}
UIRenderer::~UIRenderer() {

}

void UIRenderer::renderUI(const UIRenderData& uiRenderData) {
    if (!m_renderer) return;
    // 控制颜色混合方式 用 NONE 可避免所有混合
    //SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);

    // 保存当前渲染状态
    SDL_Color previousDrawColor;
    SDL_GetRenderDrawColor(m_renderer, 
                          &previousDrawColor.r, 
                          &previousDrawColor.g, 
                          &previousDrawColor.b, 
                          &previousDrawColor.a);
    
    // 设置UI渲染状态
    // 例如：SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    for (auto& button : uiRenderData.buttons) {
        renderButton(button);
    }



    // 恢复之前的渲染状态
    SDL_SetRenderDrawColor(m_renderer, 
                          previousDrawColor.r, 
                          previousDrawColor.g, 
                          previousDrawColor.b, 
                          previousDrawColor.a);
}

void UIRenderer::renderButton(const ButtonData& buttonData) {
    //SDL_Log("render button\n");
    renderButtonBackground(buttonData);
    
    renderButtonBorder(buttonData);
    renderButtonText(buttonData);
}

void UIRenderer::renderButtonBackground(const ButtonData& buttonData) {
    // 设置绘制颜色
    if (!m_renderer) {
        SDL_Log("renderer is null\n");
    }
   // SDL_Log("renderButtonBackground called for rect (%d,%d,%d,%d)",
   //         buttonData.rect.x, buttonData.rect.y, buttonData.rect.w, buttonData.rect.h);
    //SDL_Log("start render background\n");
    auto m_backgroundColor = buttonData.backgroundColor;
    auto m_rect = buttonData.rect;

    SDL_SetRenderDrawColor(m_renderer, m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);

    // 绘制普通矩形
    SDL_FRect rect = { static_cast<float>(m_rect.x), 
                        static_cast<float>(m_rect.y), 
                        static_cast<float>(m_rect.w), 
                        static_cast<float>(m_rect.h) };
    SDL_RenderRect(m_renderer, &rect);
}

void UIRenderer::renderButtonBorder(const ButtonData& buttonData) {
     
}

void UIRenderer::renderButtonText(const ButtonData& buttonData) {
    auto m_text = buttonData.text;
    auto m_rect = buttonData.rect;
    auto m_textStyle = buttonData.textstytle;
    if (m_text.empty()) return;
    
    // 计算文本位置（居中）
    // 这里需要TextRenderer的实际实现
    // 假设TextRenderer有一个renderText方法：
    // void renderText(SDL_Renderer* renderer, const std::string& text, 
    //                const TextStyle& style, int x, int y);
    
    // 计算居中位置
    // 注意：这里需要获取文本的实际大小来计算居中
    // 由于不知道TextRenderer的具体实现，这里提供基本思路
    
    // 假设可以获取文本尺寸
    // SDL_Point textSize = m_textRenderer.getTextSize(m_text, m_textStyle);
    
    // 计算居中位置
    // int textX = static_cast<int>(m_rect.x + (m_rect.w - textSize.x) / 2);
    // int textY = static_cast<int>(m_rect.y + (m_rect.h - textSize.y) / 2);
    
    // 临时实现：直接在中心位置渲染
    int centerX = static_cast<int>(m_rect.x + m_rect.w / 2);
    int centerY = static_cast<int>(m_rect.y + m_rect.h / 2);
    
    // 根据你的TextRenderer实际API调整
    m_textRenderer->renderText(m_text, m_textStyle, 
                              centerX, centerY); // true表示居中
}
