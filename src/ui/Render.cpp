#include "Render.h"

Renderer::Renderer(int WIDTH, int HEIGHT) : Width(WIDTH), Height(HEIGHT) {
    
}

Renderer::Renderer() : Width(1600), Height(900) {

}

Renderer::~Renderer() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window)   SDL_DestroyWindow(m_window);
}

bool Renderer::initialize() { 
    // 创建窗口（支持高DPI和横屏）[3,4](@ref)
    m_window = SDL_CreateWindow(
    "孢子棋",                          // 窗口标题，显示在标题栏上         
    Width,                    // 窗口的逻辑宽度（例如 800），用于统一布局，不受屏幕 DPI 影响
    Height,                   // 窗口的逻辑高度（例如 600）
    SDL_WINDOW_HIGH_PIXEL_DENSITY |   // 启用高像素密度支持（HiDPI/Retina），确保在高分屏上画面清晰
    SDL_WINDOW_RESIZABLE              // 允许用户调整窗口大小（可拉伸）
);
    if (!m_window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "创建窗口失败: %s", SDL_GetError());
                 return false;
    }
    
    // 创建渲染器
    m_renderer = SDL_CreateRenderer(m_window, nullptr); //自动选择合适渲染器，开启硬件加速和垂直同步
    if (!m_renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "创建渲染器失败: %s", SDL_GetError());
                 return false;
    }
    
    // 设置逻辑呈现模式，实现分辨率自适应[3](@ref)
    SDL_SetRenderLogicalPresentation(m_renderer,
                                    Width,
                                    Height,
                                    SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetWindowSize(m_window, Width, Height);
    return true;
}

void Renderer::Renderhello() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderPresent(m_renderer);
}