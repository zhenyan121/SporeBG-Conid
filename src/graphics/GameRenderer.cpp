#include "GameRenderer.h"
#include <iostream>
GameRenderer::GameRenderer(int WIDTH, int HEIGHT, SDL_Renderer* renderer) : m_Width(WIDTH), m_Height(HEIGHT), m_renderer(renderer) {
    m_cellSize = HEIGHT / m_boardRow;
}



GameRenderer::~GameRenderer() {
    
}

bool GameRenderer::initialize() { 
    return true;
}

void GameRenderer::beginFrame() {
    if (!m_renderer) {
        std::cout << "ERROR: Renderer is NULL!\n";
        return;
    }
    // 清屏为白色色背景
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255); 
    SDL_RenderClear(m_renderer);
    //std::cout << "begin frame\n";
    // 临时测试：画个红框
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    SDL_FRect test{100, 100, 200, 200};
    SDL_RenderFillRect(m_renderer, &test);
}

void GameRenderer::endFrame() {
    // 提交到屏幕
    //std::cout << "end frame\n";
    SDL_RenderPresent(m_renderer);
}


void GameRenderer::drawBackground() {

}


void GameRenderer::drawBoard() {
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

BoardArea GameRenderer::getBoardArea() const {
    return {
        (m_Width - m_cellSize * m_boardCOL) / 2,
        (m_Height - m_cellSize * m_boardRow) / 2,
        m_cellSize,
        m_boardRow,
        m_boardCOL
    };
}

