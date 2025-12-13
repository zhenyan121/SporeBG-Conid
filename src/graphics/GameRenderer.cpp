#include "GameRenderer.h"
#include "game/Board.h"
#include <iostream>
GameRenderer::GameRenderer(int WIDTH, int HEIGHT, SDL_Renderer* renderer) : m_Width(WIDTH), m_Height(HEIGHT), m_renderer(renderer) {
    m_cellSize = HEIGHT / m_boardRow;
}



GameRenderer::~GameRenderer() {
    
}

bool GameRenderer::initialize() { 
    return true;
}



void GameRenderer::setBoard(const Board* board) {
    m_board = board;
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

        //bool isLight = (row + col) % 2 == 0;
        SDL_SetRenderDrawColor(m_renderer, 
            0,
            0,
            0,
            255);

        // SDL3: RenderFillRect 接受 const SDL_FRect*
        SDL_RenderRect(m_renderer, &rect);
    }
}


}

void GameRenderer::drawPiece(std::optional<std::pair<int, int>> selectedPiece) {
    if (!m_board || !m_renderer) return;

    auto area = getBoardArea();
    float pieceRadius = m_cellSize * m_pieceRadiusRatio / 2.0f;
    // 遍历所有格子
    for (int row = 0; row < m_boardRow; ++row) {
        for (int col = 0; col < m_boardCOL; ++col) {
            const Piece* piece = m_board->getPieceAt(row, col);
            if (!piece) continue; // 没有棋子则跳过
            
            // 计算棋子中心位置
            float centerX = area.x + col * area.cellSize + area.cellSize / 2.0f;
            float centerY = area.y + row * area.cellSize + area.cellSize / 2.0f;
            
            // 确定棋子颜色
            SDL_Color color;
            if (selectedPiece && selectedPiece->first == row && selectedPiece->second == col) {
                // 选中状态的棋子
                color = m_colors.selected;
            } else {
                // 根据玩家设置颜色
                color = (piece->getPieceOwner() == PlayerID::P1) ? 
                        m_colors.P1 : m_colors.P2;
            }
            
            // 绘制棋子（圆形）
            SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
            
            // 绘制实心圆（使用多个同心圆来近似）
            for (float r = pieceRadius; r > 0; r -= 1.0f) {
                SDL_FRect rect = {
                    centerX - r,
                    centerY - r,
                    r * 2.0f,
                    r * 2.0f
                };
                SDL_RenderFillRect(m_renderer, &rect);
            }
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

