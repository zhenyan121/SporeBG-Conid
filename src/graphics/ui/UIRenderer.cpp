#include "UIRenderer.h"
#include <iostream>
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

    for (auto& label : uiRenderData.labels) {
        renderLabel(label);
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
    renderText<ButtonData>(buttonData);
}

void UIRenderer::renderLabel(const LabelData& labelData) {
    //SDL_Log("start render label\n");
    renderText<LabelData>(labelData);
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
    auto [width, height] = m_textRenderer->getTextSize(buttonData.text, buttonData.textstytle);
    // 绘制普通矩形
    SDL_FRect rect = { static_cast<float>(m_rect.x), 
                        static_cast<float>(m_rect.y), 
                        static_cast<float>(width), 
                        static_cast<float>(height) };
    SDL_RenderFillRect(m_renderer, &rect);
}

void UIRenderer::renderButtonBorder(const ButtonData& buttonData) {
     
}
template <typename Type>
void UIRenderer::renderText(const Type& data) {
    auto m_text = data.text;
   // std::cout <<  data.text << "\n";
    auto m_rect = data.rect;
    auto m_textStyle = data.textstytle;
    if (m_text.empty()) return;
    
    // 计算文本位置（居中）
    // 这里需要TextRenderer的实际实现
    // 假设TextRenderer有一个renderText方法：
    // void renderText(SDL_Renderer* renderer, const std::string& text, 
    //                const TextStyle& style, int x, int y);
    
    // 获取文本实际尺寸以便正确居中渲染
    auto [textW, textH] = m_textRenderer->getTextSize(m_text, m_textStyle);

    // 如果组件的宽高为 0，则使用文本尺寸填充（相当于自动调整控件大小）
    float boxW = m_rect.w;
    float boxH = m_rect.h;
    if (boxW <= 0.0f) boxW = static_cast<float>(textW);
    if (boxH <= 0.0f) boxH = static_cast<float>(textH);

    // 计算文本左上角坐标以实现居中
    int textX = static_cast<int>(m_rect.x + (boxW - textW) / 2.0f);
    int textY = static_cast<int>(m_rect.y + (boxH - textH) / 2.0f);

    // 渲染文本（TextRenderer 的 x,y 为左上角）
    m_textRenderer->renderText(m_text, m_textStyle, textX, textY);
}
