#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "graphics/font/TextRenderer.h"
#include "graphics/font/FontManager.h"
#include "ui/base/UIRenderData.h"
class UIRenderer {
private:
    TextRenderer* m_textRenderer;
    

    SDL_Renderer* m_renderer;


    /**
     * @brief 渲染背景
     * @param renderer SDL渲染器
     */
    template <typename Type>
    void renderBackground(const Type& data);
    
    /**
     * @brief 渲染按钮边框
     * @param renderer SDL渲染器
     */
    void renderButtonBorder(const ButtonData& buttonData);
    
    /**
     * @brief 渲染文本
     * @param renderer SDL渲染器
     */
    template <typename Type>
    void renderText(const Type& data);
public:

    UIRenderer(SDL_Renderer* m_renderer, TextRenderer* textRenderer);
    ~UIRenderer();
    void renderUI(const UIRenderData& uiRenderData);
    
    void renderButton(const ButtonData& buttonData);

    void renderLabel(const LabelData& buttonData);

    // 允许外部获取内部的 TextRenderer 指针（非拥有）以便在需要时测量文本尺寸
    TextRenderer* getTextRenderer() const { return m_textRenderer; }

};