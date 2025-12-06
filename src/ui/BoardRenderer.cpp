#include "Render.h"

Renderer::Renderer(int WIDTH, int HEIGHT) : m_Width(WIDTH), m_Height(HEIGHT) {
    m_cellSize = HEIGHT / m_boardRow;
}



Renderer::~Renderer() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window)   SDL_DestroyWindow(m_window);
}

bool Renderer::initialize() { 
    // 创建窗口（支持高DPI和横屏）[3,4](@ref)
    m_window = SDL_CreateWindow(
    "孢子棋",                          // 窗口标题，显示在标题栏上         
    m_Width,                    // 窗口的逻辑宽度（例如 800），用于统一布局，不受屏幕 DPI 影响
    m_Height,                   // 窗口的逻辑高度（例如 600）
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
                                    m_Width,
                                    m_Height,
                                    SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetWindowSize(m_window, m_Width, m_Height);
    return true;
}




void Renderer::render() {
    clear();
    
    drawBoard();

    present();


}

void Renderer::clear() {
    // 设置画笔颜色
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    // 使用画笔颜色填充整个屏幕
    SDL_RenderClear(m_renderer);
}

void Renderer::present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::drawBoard() {
    auto area = getBoardArea();
    

// 绘制棋盘格子（交替颜色）
for (int row = 0; row < area.rows; ++row) {
    for (int col = 0; col < area.cols; ++col) {
        // 使用 SDL_FRect（浮点数）
        SDL_FRect rect{
            static_cast<float>(area.x + col * area.cellSize),
            static_cast<float>(area.y + row * area.cellSize),
            static_cast<float>(area.cellSize),
            static_cast<float>(area.cellSize)
        };

        bool isLight = (row + col) % 2 == 0;
        SDL_SetRenderDrawColor(m_renderer, 
            isLight ? 240 : 180,
            isLight ? 220 : 160,
            isLight ? 180 : 120,
            255);

        // SDL3: RenderFillRect 接受 const SDL_FRect*
        SDL_RenderFillRect(m_renderer, &rect);
    }
}


}

ui::BoardArea Renderer::getBoardArea() const {
    return {
        (m_Width - m_cellSize * m_boardCOL) / 2,
        (m_Height - m_cellSize * m_boardRow) / 2,
        m_cellSize,
        m_boardRow,
        m_boardCOL
    };
}

SDL_Renderer* Renderer::getSDLRenderer() const {
    return m_renderer;
}