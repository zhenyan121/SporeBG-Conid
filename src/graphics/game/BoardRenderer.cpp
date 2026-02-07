#include "BoardRenderer.h"
#include "game/Board.h"
#include "core/Time.h"
#include "utils/Tools.h"
#include <iostream>
BoardRenderer::BoardRenderer(int WIDTH, int HEIGHT, SDL_Renderer* renderer, TextureManager* textureManager) :
    m_Width(WIDTH),
    m_Height(HEIGHT),
    m_renderer(renderer), 
    m_textureManager(textureManager)

    {
    m_cellSize = HEIGHT / m_boardRow;
    
    m_area = getBoardArea();
}



BoardRenderer::~BoardRenderer() {
    m_pieceAnimation.grow.clear();
    m_pieceAnimation.move.clear();
    m_pieceAnimation.fight.clear();
    m_pieceAnimation.select.clear();
    std::cout << "BoardRenderer destory\n";
}

bool BoardRenderer::initialize() { 
    return true;
}



void BoardRenderer::setBoard(const Board* board) {
    m_board = board;
}


void BoardRenderer::update(float deltaTime) {

    updateGrowAnimation(deltaTime);
    updateMoveAnimation(deltaTime);
    updateFightAnimation(deltaTime);
    updateSelectedAnimation(deltaTime);

}

void BoardRenderer::updateGrowAnimation(float deltaTime) {

    for (auto growAnimation = m_pieceAnimation.grow.begin(); growAnimation != m_pieceAnimation.grow.end(); ) {
        
        if (!growAnimation->isAnimating) {

            std::cout << "BoardRenderer: grow animation play finished\n";
            growAnimation = m_pieceAnimation.grow.erase(growAnimation);
            
            continue;
        }

        growAnimation->currentTime += deltaTime;
        if (growAnimation->currentTime > growAnimation->animationDuration) {
            growAnimation->currentTime = growAnimation->animationDuration;
            growAnimation->isAnimating = false;
        }
        growAnimation->renderColor = growAnimation->baseColor;
        growAnimation->progress = growAnimation->currentTime / growAnimation->animationDuration;
        growAnimation->renderColor.a = Tools::smoothMove(growAnimation->progress, 0, 255);
        ++growAnimation;
    }

    
}

void BoardRenderer::updateMoveAnimation(float deltaTime) {

    for (auto moveAnimation = m_pieceAnimation.move.begin(); moveAnimation != m_pieceAnimation.move.end(); ) {
        
        if (!moveAnimation->isAnimating) {

            std::cout << "BoardRenderer: move animation play finished\n";
            moveAnimation = m_pieceAnimation.move.erase(moveAnimation);
            
            continue;
        }
        
        moveAnimation->currentTime += deltaTime;
        if (moveAnimation->currentTime > moveAnimation->animationDuration) {
            moveAnimation->currentTime = moveAnimation->animationDuration;
            moveAnimation->isAnimating = false;
        }
        moveAnimation->progress = moveAnimation->currentTime / moveAnimation->animationDuration;

        auto renderRect = moveAnimation->fromPieceRect;
        renderRect.x += Tools::smoothMove(moveAnimation->progress, 0, moveAnimation->distanceCol);
        renderRect.y += Tools::smoothMove(moveAnimation->progress, 0, moveAnimation->distanceRow);
        moveAnimation->renderRect = renderRect;
        ++moveAnimation;
    }

    
}

void BoardRenderer::updateFightAnimation(float deltaTime) {
    for (auto fightAnimation = m_pieceAnimation.fight.begin(); fightAnimation != m_pieceAnimation.fight.end(); ) {
        if (!fightAnimation->isAnimating) {

            std::cout << "BoardRenderer: fight animation play finished\n";
            fightAnimation = m_pieceAnimation.fight.erase(fightAnimation);
            
            continue;
        }
        
        fightAnimation->currentTime += deltaTime;
        if (fightAnimation->currentTime > fightAnimation->animationDuration) {
            fightAnimation->currentTime = fightAnimation->animationDuration;
            fightAnimation->isAnimating = false;
        }
        fightAnimation->progress = fightAnimation->currentTime / fightAnimation->animationDuration;
        auto renderRect = fightAnimation->fromPieceRect;
        float cX = fightAnimation->distanceCol * 0.5f;
        float cY = fightAnimation->distanceRow * 0.5f;
        renderRect.x = Tools::pingPongSpring(fightAnimation->progress, renderRect.x, cX);
        renderRect.y = Tools::pingPongSpring(fightAnimation->progress, renderRect.y, cY);
        fightAnimation->renderRect = renderRect;
        ++fightAnimation;
    }
    
}

void BoardRenderer::updateSelectedAnimation(float deltaTime) {
    // 累加时间，限制不超过总时长

    if (m_pieceAnimation.select.empty()) {
        return; // 没有选中动画，直接退出
    }

    auto selectAnimation = m_pieceAnimation.select.begin();
    selectAnimation->currentTime += deltaTime;
    if (selectAnimation->currentTime > selectAnimation->duration) {
        selectAnimation->currentTime = 0;
        selectAnimation->isBigger = !selectAnimation->isBigger;
        
    }

    SDL_FRect renderRect = {
        selectAnimation->baseRect.x,
        selectAnimation->baseRect.y,
        selectAnimation->baseRect.w,
        selectAnimation->baseRect.h
    };

    float progess = selectAnimation->currentTime / selectAnimation->duration;
    selectAnimation->rotatedAngel = 360 * static_cast<double>(progess);
    float scale = 0.2 * progess;
    
    float baseW = selectAnimation->baseRect.w;
    float baseH = selectAnimation->baseRect.h;
    if (selectAnimation->isBigger) {
        renderRect.w = baseW * (1.0f + scale);
        renderRect.h = baseH * (1.0f + scale);
    } else {
        renderRect.w = baseW * (1.2f - scale); // 从 1.2 缩回到 1.0
        renderRect.h = baseH * (1.2f - scale);
    }
    // 居中缩放：重新计算 x, y 使中心点不变
    renderRect.x = selectAnimation->baseRect.x + (baseW - renderRect.w) / 2.0f;
    renderRect.y = selectAnimation->baseRect.y + (baseH - renderRect.h) / 2.0f;
    
    selectAnimation->renderRect = renderRect;
}

void BoardRenderer::drawBackground() {


    
}


void BoardRenderer::drawBoard() {
    // 绘制棋盘格子
    for (int row = 0; row < m_area.rows; ++row) {
        for (int col = 0; col < m_area.cols; ++col) {
            // 使用 SDL_FRect（浮点数）
            SDL_FRect rect{
                static_cast<float>(m_area.x + col * m_area.cellSize),
                static_cast<float>(m_area.y + row * m_area.cellSize),
                static_cast<float>(m_area.cellSize),
                static_cast<float>(m_area.cellSize)
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
    if (!m_board || !m_renderer) {
        return;
    }

    // 遍历所有格子
    for (int row = 0; row < m_boardRow; ++row) {
        for (int col = 0; col < m_boardCOL; ++col) {
            drawPieceAt(row, col, selectedPiece);

        }
    }
}

void BoardRenderer::drawPieceAt(int row, int col, std::optional<std::pair<int, int>> selectedPiece) {
    float pieceRadius = m_cellSize * m_pieceRadiusRatio / 2.0f;
    const Piece* piece = m_board->getPieceAt(row, col);
        if (!piece) {
            return; // 没有棋子则跳过
        } 
        // 计算棋子中心位置
        float x = m_area.x + col * m_area.cellSize;
        float y = m_area.y + row * m_area.cellSize;
        bool isSelected = false;
        // 确定棋子颜色
        SDL_Color color;
        if (selectedPiece && selectedPiece->first == row && selectedPiece->second == col) {
            // 选中状态的棋子，暂时不渲染黄色的选中状态
            color = m_colors.selected;
            isSelected = true;
        } else {
            // 根据玩家设置颜色
            color = (piece->getPieceOwner() == PlayerID::P1) ? 
                    m_colors.P1 : m_colors.P2;
        }
        //SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        
        SDL_FRect rect{
            x + (m_area.cellSize - pieceRadius * 2) / 2.0f,
            y + (m_area.cellSize - pieceRadius * 2) / 2.0f,
            pieceRadius * 2,
            pieceRadius * 2
        };
        
        //SDL_RenderFillRect(m_renderer, &rect);
        
        auto texture = m_textureManager->createTextureFromRect(rect, color);
        //SDL_FRect srect = {0, 0, rect.w, rect.h};

        for (const auto& growAnimation : m_pieceAnimation.grow) {
            if (growAnimation.isAnimating && growAnimation.row == row && growAnimation.col == col) {
            
                m_textureManager->destoryTexture(rect, color);
                
                auto texture = m_textureManager->createTextureFromRect(rect, growAnimation.renderColor);
                SDL_RenderTexture(m_renderer, texture, nullptr, &rect);
                return;
            
            }
        }
        

        if (isSelected) {
            auto selectAnimation = m_pieceAnimation.select.begin();
            SDL_RenderTextureRotated(m_renderer, texture, nullptr, &selectAnimation->renderRect, selectAnimation->rotatedAngel, nullptr, SDL_FLIP_NONE);
            
            return;
        }
        
        for (const auto& moveAnimation : m_pieceAnimation.move) {
            if (moveAnimation.isAnimating && col == moveAnimation.toCol && row == moveAnimation.toRow) {
                //SDL_Log("rendering..\n");
                
                SDL_RenderTexture(m_renderer, texture, nullptr, &moveAnimation.renderRect);
                return;
            }
        }
        
        for (const auto& fightAnimation : m_pieceAnimation.fight) {
            if (fightAnimation.isAnimating && col == fightAnimation.fromCol && row == fightAnimation.fromRow) {

                SDL_RenderTexture(m_renderer, texture, nullptr, &fightAnimation.renderRect);
                return;

            } 
        }
        
        
        SDL_RenderTexture(m_renderer, texture, nullptr, &rect);   
}

void BoardRenderer::updateSelectedPiece(std::optional<std::pair<int, int>> selectedPiece) {
    if (!selectedPiece) {
        return;
    }

    float pieceRadius = m_cellSize * m_pieceRadiusRatio / 2.0f;
    // 计算棋子中心位置
    float x = m_area.x + selectedPiece->second * m_area.cellSize;
    float y = m_area.y + selectedPiece->first * m_area.cellSize;
    SDL_FRect baseRect{
            x + (m_area.cellSize - pieceRadius * 2) / 2.0f,
            y + (m_area.cellSize - pieceRadius * 2) / 2.0f,
            pieceRadius * 2,
            pieceRadius * 2
        };
    SelectAnimation selectAnimation = {
        true,
        1.0f,
        0.0f,
        true,
        baseRect,
        selectedPiece,
        baseRect,
        0.0f
    };
    // 选择棋子的动画只需要一个
    m_pieceAnimation.select.clear();

    m_pieceAnimation.select.push_back(selectAnimation);

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

    
    float pieceRadius = m_cellSize * m_pieceRadiusRatio / 4.0f;
    for (auto PieceID : m_currentDrawRange) {
        auto [row, col] = m_board->getCoordFromID(PieceID);

        // 计算棋子中心位置
        float x = m_area.x + col * m_area.cellSize;
        float y = m_area.y + row * m_area.cellSize;

        SDL_FRect rect{
                x + (m_area.cellSize - pieceRadius * 2) / 2.0f,
                y + (m_area.cellSize - pieceRadius * 2) / 2.0f,
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
    //SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

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
    //SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
}

void BoardRenderer::handleGamePieceEvent(GamePieceEvent event, int fromRow, int fromCol, int toRow, int toCol) {

    // 计算棋子中心位置
    float fromX = m_area.x + fromCol * m_area.cellSize;
    float fromY = m_area.y + fromRow * m_area.cellSize;
    float toX = -1;
    float toY = -1;
    if (toRow != -1 && toCol != -1) {
        toX = m_area.x + toCol * m_area.cellSize;
        toY = m_area.y + toRow * m_area.cellSize;
    }
    float pieceRadius = m_cellSize * m_pieceRadiusRatio / 2.0f;

    SDL_FRect rect{
                fromX + (m_area.cellSize - pieceRadius * 2) / 2.0f,
                fromY + (m_area.cellSize - pieceRadius * 2) / 2.0f,
                pieceRadius * 2,
                pieceRadius * 2
            };

    const Piece* piece = m_board->getPieceAt(fromRow, fromCol);
    // 这里直接return导致的
    if (!piece && event != GamePieceEvent::MOVE_PIECE) {
        SDL_Log("BoardRenderer: piece is null\n");
        return;
    }
    
    switch (event) {
        case (GamePieceEvent::REMOVE_PIECE):
        {
            SDL_Color color = (piece->getPieceOwner() == PlayerID::P1) ? 
                        m_colors.P1 : m_colors.P2;
            //SDL_Log("BoardRenderer: try to destory texture\n");
            m_textureManager->destoryTexture(rect, color);
            break;
        }
        case (GamePieceEvent::PLACE_PIECE):
            break;
        case (GamePieceEvent::MOVE_PIECE):
        {
            //SDL_Log("MovePPPPPP\n");
            MoveAnimation moveAnimation = {
                fromRow, fromCol,
                toRow, toCol,
                toY - fromY,
                toX - fromX,
                0.0f,
                true,
                rect,
                1.0f
            };
            
            m_pieceAnimation.move.push_back(moveAnimation);

            break;
        }
        case (GamePieceEvent::GROW_PIECE):
        {
            GrowAnimation growAnimation = {
                toRow,
                toCol,
                0.0f,
                true,
                1.0f
            };

            SDL_Color color = (piece->getPieceOwner() == PlayerID::P1) ? 
                        m_colors.P1 : m_colors.P2;
            growAnimation.baseColor = color;

            m_pieceAnimation.grow.push_back(growAnimation);

            break;
        }
        case (GamePieceEvent::FIGHT_PIECE):
        {
            FightAnimation fightAnimation = {
                fromRow, fromCol,
                toRow, toCol,
                toY - fromY,
                toX - fromX,
                0.0f,
                true,
                rect,
                1.0f
            };

            m_pieceAnimation.fight.push_back(fightAnimation);

            break;
        }
        default:
            break;
    }

}


