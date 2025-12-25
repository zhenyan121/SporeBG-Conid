#include "WindowManager.h"

WindowManager::WindowManager() : m_renderer(nullptr),
                                m_window(nullptr) 
{
    
}

WindowManager::~WindowManager() {
    Shutdown();
}

bool WindowManager::Initialize(Config& config) {
    m_viewport.logicalWidth = config.render.logicalWidth;
    m_viewport.logicalHeight = config.render.logicalHeight;
    // 创建窗口（支持高DPI和横屏）[3,4](@ref)
    m_window = SDL_CreateWindow(
    config.window.title.c_str(),                          // 窗口标题，显示在标题栏上         
    config.window.width,                    // 窗口的逻辑宽度（例如 800），用于统一布局，不受屏幕 DPI 影响
    config.window.height,                   // 窗口的逻辑高度（例如 600）
    SDL_WINDOW_HIGH_PIXEL_DENSITY |   // 启用高像素密度支持（HiDPI/Retina），确保在高分屏上画面清晰
    SDL_WINDOW_RESIZABLE              // 允许用户调整窗口大小（可拉伸）
);
    //SDL_Log("window width %d\nwindow height %d\ntitle %s\n", config.window.width, config.window.height, config.window.title);
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
    
    if (config.window.vsync) {
        SDL_SetRenderVSync(m_renderer, true);
    } else {
        SDL_SetRenderVSync(m_renderer, false);
    }
     

    
   /* // 设置逻辑呈现模式，实现分辨率自适应[3](@ref)
    SDL_SetRenderLogicalPresentation(m_renderer,
                                    m_width,
                                    m_height,
                                    SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    */
    //SDL_SetWindowSize(m_window, UI::StartWindowWidth, UI::StartWindowHeight);
    // 创建逻辑画布
    // RGBA8888: 32位色，8位红绿蓝和透明度
    // TARGET: 纹理可作为渲染目标
    m_logicalTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_viewport.logicalWidth,
        m_viewport.logicalHeight
    );
    // 设置纹理缩放模式为最近邻
    SDL_SetTextureScaleMode(m_logicalTexture, SDL_SCALEMODE_NEAREST);
    
    
    // 创建文本渲染层
    m_textTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_viewport.windowWidth,
        m_viewport.windowHeight
    );
    
    return true;
}

void WindowManager::Shutdown() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window)   SDL_DestroyWindow(m_window);
}

void WindowManager::Clear() {
    // 只清除窗口背景
    SDL_SetRenderTarget(m_renderer, nullptr);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
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

   
    
    //SDL_FRect dstRect;
    //calculateDstRect(dstRect);
    
    SDL_FRect textDis = {
        0.0f,
        0.0f,
        static_cast<float> (m_viewport.windowWidth),
        static_cast<float> (m_viewport.windowHeight)
    };

    SDL_RenderTexture (
        m_renderer,
        m_logicalTexture, // 源：你已经画好的逻辑画布
        nullptr,        // srcRect：源区域（nullptr = 整张）
        &m_viewport.dst        // dstRect：贴到哪里 & 贴多大
    );
    SDL_RenderTexture (
        m_renderer,
        m_textTexture,
        nullptr,
        &textDis
    );
    SDL_RenderPresent(m_renderer);
}


void WindowManager::beginWorld() {
    // 设置渲染目标为逻辑纹理
    // 清除逻辑纹理（世界层）
    SDL_SetRenderTarget(m_renderer, m_logicalTexture);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255); 
    SDL_RenderClear(m_renderer);
    
}

void WindowManager::endWorld() {
    SDL_SetRenderTarget(m_renderer, nullptr);
}

void WindowManager::beginUI() {
    // 设置渲染目标为窗口
    SDL_SetRenderTarget(m_renderer, m_textTexture);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 0);
    SDL_RenderClear(m_renderer);
}

void WindowManager::endUI() {
    // 恢复渲染目标为默认（窗口）
    SDL_SetRenderTarget(m_renderer, nullptr);
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
/*
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
    */
void WindowManager::onWindowResize() { 

    // =================================================
    // 1. 查询窗口的真实像素尺寸
    //    注意：这是“窗口坐标空间”的大小
    // =================================================
    SDL_GetWindowSize(
        m_window,
        &m_viewport.windowWidth,
        &m_viewport.windowHeight
    );

    // =================================================
    // 2. 计算 X / Y 方向最多能放大多少倍
    //    使用整数除法，保证整数缩放（像素不模糊）
    // =================================================
    int scaleX = m_viewport.windowWidth  / m_viewport.logicalWidth;
    int scaleY = m_viewport.windowHeight / m_viewport.logicalHeight;

    // =================================================
    // 3. 取较小值，保证逻辑画面完整显示
    //    同时确保最小为 1（窗口再小也能画）
    // =================================================
    m_viewport.scale = std::max(1, std::min(scaleX, scaleY));

    // =================================================
    // 4. 计算逻辑画面放大后的实际尺寸
    // =================================================
    m_viewport.dst.w =
        static_cast<float>(m_viewport.logicalWidth *
                           m_viewport.scale);

    m_viewport.dst.h =
        static_cast<float>(m_viewport.logicalHeight *
                           m_viewport.scale);

    // =================================================
    // 5. 计算居中偏移（letterbox / pillarbox）
    // =================================================
    m_viewport.dst.x =
        (m_viewport.windowWidth - m_viewport.dst.w) * 0.5f;

    m_viewport.dst.y =
        (m_viewport.windowHeight - m_viewport.dst.h) * 0.5f;

    // 到这里为止：
    // - scale       可供所有系统使用
    // - dst         是渲染 & 输入转换的唯一依据
    // - windowSize  不需要再到处 SDL_GetWindowSize

    // 创建文本渲染层
    SDL_DestroyTexture(m_textTexture);
    m_textTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_viewport.windowWidth,
        m_viewport.windowHeight
    );

}

Viewport const& WindowManager::getViewport() const {
    return m_viewport;
}
