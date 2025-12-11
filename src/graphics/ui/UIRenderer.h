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
     * @brief 渲染按钮背景
     * @param renderer SDL渲染器
     */
    void renderButtonBackground(const ButtonData& buttonData);
    
    /**
     * @brief 渲染按钮边框
     * @param renderer SDL渲染器
     */
    void renderButtonBorder(const ButtonData& buttonData);
    
    /**
     * @brief 渲染按钮文本
     * @param renderer SDL渲染器
     */
    void renderButtonText(const ButtonData& buttonData);
public:

    UIRenderer(SDL_Renderer* m_renderer, TextRenderer* textRenderer);
    ~UIRenderer();
    void renderUI(const UIRenderData& uiRenderData);
    
    void renderButton(const ButtonData& buttonData);

};