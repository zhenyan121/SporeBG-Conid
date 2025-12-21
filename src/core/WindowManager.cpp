#include "WindowManager.h"

WindowManager::WindowManager() : m_renderer(nullptr),
                                m_window(nullptr) 
{
    
}

WindowManager::~WindowManager() {
    Shutdown();
}

bool WindowManager::Initialize(GameConfig& config) {
    m_logicalWidth = config.logicalWidth;
    m_logicalHeight = config.logicalHeight;
    // 创建窗口（支持高DPI和横屏）[3,4](@ref)
    m_window = SDL_CreateWindow(
    "孢子棋",                          // 窗口标题，显示在标题栏上         
    m_logicalWidth,                    // 窗口的逻辑宽度（例如 800），用于统一布局，不受屏幕 DPI 影响
    m_logicalHeight,                   // 窗口的逻辑高度（例如 600）
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
    
   /* // 设置逻辑呈现模式，实现分辨率自适应[3](@ref)
    SDL_SetRenderLogicalPresentation(m_renderer,
                                    m_width,
                                    m_height,
                                    SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    */
    SDL_SetWindowSize(m_window, UI::StartWindowWidth, UI::StartWindowHeight);
    // 创建逻辑画布
    // RGBA8888: 32位色，8位红绿蓝和透明度
    // TARGET: 纹理可作为渲染目标
    m_logicalTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_logicalWidth,
        m_logicalHeight
    );
    // 设置纹理缩放模式为最近邻
    SDL_SetTextureScaleMode(m_logicalTexture, SDL_SCALEMODE_NEAREST);

    return true;
}

void WindowManager::Shutdown() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window)   SDL_DestroyWindow(m_window);
}

void WindowManager::Clear() {
    // 1. 设置渲染目标为逻辑纹理
    SDL_SetRenderTarget(m_renderer, m_logicalTexture);
    // 设置画笔颜色
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    // 使用画笔颜色填充整个逻辑画布
    SDL_RenderClear(m_renderer);
    
}

/*
void WindowManager::Clear() {
    
    // 设置画笔颜色
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255); // 白色背景
    // 使用画笔颜色填充整个逻辑画布
    SDL_RenderClear(m_renderer);
    
}
*/

void WindowManager::Present() {

    // 4. 切回默认渲染目标（窗口）
    SDL_SetRenderTarget(m_renderer, nullptr);
    SDL_RenderClear(m_renderer);  // 清的是窗口（黑边
    SDL_FRect dstRect;
    calculateDstRect(dstRect);
    SDL_RenderTexture (
        m_renderer,
        m_logicalTexture, // 源：你已经画好的逻辑画布
        nullptr,        // srcRect：源区域（nullptr = 整张）
        &dstRect        // dstRect：贴到哪里 & 贴多大
    );
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

void WindowManager::calculateDstRect(SDL_FRect& dstRect) {
    // 获取窗口的实际尺寸（像素）
    SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
    // 计算缩放比例,整数缩放
    int scaleX = m_windowWidth / m_logicalWidth;
    int scaleY = m_windowHeight / m_logicalHeight;
    int scale = (scaleX < scaleY) ? scaleX : scaleY;

    // 计算目标矩形的尺寸
    dstRect.w = static_cast<float>(m_logicalWidth * scale);
    dstRect.h = static_cast<float>(m_logicalHeight * scale);
    // 居中显示
    dstRect.x = static_cast<float>((m_windowWidth - dstRect.w) / 2);
    dstRect.y = static_cast<float>((m_windowHeight - dstRect.h) / 2);
}