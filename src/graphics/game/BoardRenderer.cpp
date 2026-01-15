#include "BoardRenderer.h"
#include "game/Board.h"
#include <iostream>
BoardRenderer::BoardRenderer(int WIDTH, int HEIGHT, SDL_Renderer* renderer, TextureManager* textureManager) :
    m_Width(WIDTH),
    m_Height(HEIGHT),
    m_renderer(renderer), 
    m_textureManager(textureManager)

    {
    m_cellSize = HEIGHT / m_boardRow;
}



BoardRenderer::~BoardRenderer() {
    
}

bool BoardRenderer::initialize() { 
    return true;
}



void BoardRenderer::setBoard(const Board* board) {
    m_board = board;
}

void BoardRenderer::drawBackground() {

}


void BoardRenderer::drawBoard() {
    auto area = getBoardArea();
    

// 绘制棋盘格子
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

void BoardRenderer::drawPiece(std::optional<std::pair<int, int>> selectedPiece) {
    if (!m_board || !m_renderer) return;

    auto area = getBoardArea();
    float pieceRadius = m_cellSize * m_pieceRadiusRatio / 2.0f;
    // 遍历所有格子
    for (int row = 0; row < m_boardRow; ++row) {
        for (int col = 0; col < m_boardCOL; ++col) {
            const Piece* piece = m_board->getPieceAt(row, col);
            if (!piece) continue; // 没有棋子则跳过
            
            // 计算棋子中心位置
            float x = area.x + col * area.cellSize;
            float y = area.y + row * area.cellSize;
            
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
            
            
            //SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
            
            SDL_FRect rect{
                x + (area.cellSize - pieceRadius * 2) / 2.0f,
                y + (area.cellSize - pieceRadius * 2) / 2.0f,
                pieceRadius * 2,
                pieceRadius * 2
            };
            
            //SDL_RenderFillRect(m_renderer, &rect);
            
            auto texture = m_textureManager->createTextureFromRect(rect.x, rect.y, rect, color);
            SDL_FRect srect = {0, 0, rect.w, rect.h};
            SDL_RenderTexture(m_renderer, texture, &srect, &rect);

        }
    }
}

void BoardRenderer::updateMovementRange(std::optional<std::pair<int, int>> selectedPiece, ActionType tpye) {
    // 清空当前范围以便下次更新
    m_currentDrawRange.clear();
    if (!m_board || !m_renderer || !selectedPiece) return;
    if (selectedPiece == std::nullopt) return;
    auto selectedComponent = m_board->getComponentByPieceID(m_board->getPieceID(selectedPiece->first, selectedPiece->second));
    
    auto currentPlayer = m_board->getPieceAt(selectedPiece->first, selectedPiece->second)->getPieceOwner();
    switch (tpye) {
        case ActionType::GROW:
            // 绘制生长范围的逻辑
            for (auto PieceID : selectedComponent) {
                auto Neighbors = m_board->getOrthogonalNeighbors(PieceID);
                for (auto neighborID : Neighbors) {
                    auto pieceCoord = m_board->getCoordFromID(neighborID);
                    const Piece* piece = m_board->getPieceAt(pieceCoord.first, pieceCoord.second);
                    if (piece == nullptr) {
                        m_currentDrawRange.insert(neighborID);
                    }
                }          
            }
            break;
        case ActionType::MOVE:
            // 绘制移动范围的逻辑 
            for (auto PieceID : selectedComponent) {
                auto Neighbors = m_board->getOrthogonalNeighbors(PieceID);
                for (auto neighborID : Neighbors) {
                    auto pieceCoord = m_board->getCoordFromID(neighborID);
                    const Piece* piece = m_board->getPieceAt(pieceCoord.first, pieceCoord.second);
                    if (piece == nullptr) {
                        m_currentDrawRange.insert(neighborID);
                    } else if (piece->getPieceOwner() != currentPlayer) {
                        m_currentDrawRange.insert(neighborID);
                    }
                }          
            }
            break;
        case ActionType::SPORE: {
            // 绘制孢子范围的逻辑
            auto SporeRange = m_board->getSporeRange(m_board->getPieceID(selectedPiece->first, selectedPiece->second));
            for (auto neighborID : SporeRange) {
                auto pieceCoord = m_board->getCoordFromID(neighborID);
                const Piece* piece = m_board->getPieceAt(pieceCoord.first, pieceCoord.second);
                if (piece == nullptr) {
                    m_currentDrawRange.insert(neighborID);
                }
            }          
        }
            break;
        default:
            break;
    }
    
}

void BoardRenderer::drawMovementRange() {

    if (!m_board || !m_renderer) return;

    // 开启混合模式（重要！）
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    auto area = getBoardArea();
    float pieceRadius = m_cellSize * m_pieceRadiusRatio / 4.0f;
    for (auto PieceID : m_currentDrawRange) {
        auto [row, col] = m_board->getCoordFromID(PieceID);

        // 计算棋子中心位置
        float x = area.x + col * area.cellSize;
        float y = area.y + row * area.cellSize;

        SDL_FRect rect{
                x + (area.cellSize - pieceRadius * 2) / 2.0f,
                y + (area.cellSize - pieceRadius * 2) / 2.0f,
                pieceRadius * 2,
                pieceRadius * 2
            };

        SDL_SetRenderDrawColor(m_renderer, 
            0,
            255,
            0,
            100); // 半透明绿色

        SDL_RenderFillRect(m_renderer, &rect);
    }
    // 如果需要，绘制完可以恢复原来的混合模式
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
}

BoardArea BoardRenderer::getBoardArea() const {
    return {
        (m_Width - m_cellSize * m_boardCOL) / 2,
        (m_Height - m_cellSize * m_boardRow) / 2,
        m_cellSize,
        m_boardRow,
        m_boardCOL
    };
}

void BoardRenderer::setGameState(GameState state) {
    m_gameState = state;
}

void BoardRenderer::renderBlackOverlay() {
    if (!m_renderer) return;
    if (m_gameState == GameState::GAME_RUNING) return;
    // 开启混合模式（重要！）
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    // 设置黑色半透明颜色
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150); // 半透明黑色

    // 绘制覆盖整个屏幕的矩形
    SDL_FRect rect{
        0.0f,
        0.0f,
        static_cast<float>(m_Width),
        static_cast<float>(m_Height)
    };

    SDL_RenderFillRect(m_renderer, &rect);

    // 恢复原来的混合模式
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
}