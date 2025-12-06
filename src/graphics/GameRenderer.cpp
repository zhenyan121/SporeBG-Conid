#include "BoardRenderer.h"

Renderer::Renderer(int WIDTH, int HEIGHT) : m_Width(WIDTH), m_Height(HEIGHT) {
    m_cellSize = HEIGHT / m_boardRow;
}



Renderer::~Renderer() {
    
}

bool Renderer::initialize() { 
    
}




void Renderer::render() {
  
    
    drawBoard();

   


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