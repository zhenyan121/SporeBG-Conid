#include "WindowManager.h"

WindowManager::WindowManager() : m_renderer(nullptr),
                                m_window(nullptr) 
{
    
}

WindowManager::~WindowManager() {
    Shutdown();
}

bool WindowManager::Initialize(GameConfig& config) {
    m_width = config.windowWidth;
    m_height = config.windowHeight;
    // 创建窗口（支持高DPI和横屏）[3,4](@ref)
    m_window = SDL_CreateWindow(
    "孢子棋",                          // 窗口标题，显示在标题栏上         
    m_width,                    // 窗口的逻辑宽度（例如 800），用于统一布局，不受屏幕 DPI 影响
    m_height,                   // 窗口的逻辑高度（例如 600）
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
    // 关键设置：启用像素模式
    
    // 设置逻辑呈现模式，实现分辨率自适应[3](@ref)
    SDL_SetRenderLogicalPresentation(m_renderer,
                                    m_width,
                                    m_height,
                                    SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetWindowSize(m_window, m_width, m_height);
    return true;
}

void WindowManager::Shutdown() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window)   SDL_DestroyWindow(m_window);
}

void WindowManager::Clear() {
    // 设置画笔颜色
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    // 使用画笔颜色填充整个屏幕
    SDL_RenderClear(m_renderer);
}

void WindowManager::Present() {
    SDL_RenderPresent(m_renderer);
}

SDL_Renderer* WindowManager::GetRenderer() {
    return m_renderer;
}

SDL_Window* WindowManager::GetWindow() {
    return m_window;
}

bool WindowManager::setFullscreen(bool isFullscreen) {
    if (isFullscreen == m_isFullscreen) {
        return true; // 状态未改变，直接返回
    }
    SDL_SetWindowFullscreen(m_window, isFullscreen);
    m_isFullscreen = isFullscreen;
    
    return true;
}