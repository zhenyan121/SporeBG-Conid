// src/core/Game.cpp
#include "Game.h"


Game::Game()         
{
    m_board = std::make_unique<Board>(7, 7);
}


Game::~Game() {
    cleanup();
}
//清理资源
void Game::cleanup() {
    
     
}


bool Game::initialize() {
    // 初始化游戏特定资源（棋盘、棋子等）
    if (!m_board->initialize()) {
        return false;
    }
    m_board->printBoard();
    

   
    
    
    // ...
    return true;
}

void Game::resetActionableComponents() {
    
    m_actionableComponents = m_board->getAllPlayerComponent(m_currentPlayer);
}


bool Game::playerSelectPiece(int row, int col) {
    if (Rule::canbeSelect(m_board->getPieceAt(row, col), m_currentPlayer)) {
        /*if (m_seletedPiece == std::nullopt) {
            m_seletedPiece = {row, col};
        }*/
       m_seletedPiece = {row, col};
    }
    return true;
}

void Game::setPlayerAction(ActionType type) {
    m_currentActionType = type;
}


void Game::executeAction(int toRow, int toCol) {
    auto [fromRow, fromCol] = *m_seletedPiece;
    if (m_currentActionType == ActionType::GROW) {
        if (Rule::canGrow(m_board.get(), fromRow, fromCol, toRow, toCol, m_currentPlayer)) {
            m_board->placePieceAt(toRow, toCol, m_currentPlayer);
            // 如果执行了操作就擦除
            markComponentAsUsed(m_board->getComponentID(fromRow, fromCol));
            return;
        }
    }
    if (m_currentActionType == ActionType::MOVE) {
        if (Rule::canMove(m_board.get(), fromRow, fromCol, toRow, toCol, m_currentPlayer)) {
            m_board->removePieceAt(fromRow, fromCol);
            m_board->removePieceAt(toRow, toCol);
            m_board->placePieceAt(toRow, toCol, m_currentPlayer);

            markComponentAsUsed(m_board->getComponentID(fromRow, fromCol));
            return;
        }
    }
    if (m_currentActionType == ActionType::SPORE) {
        if (Rule::canSpore(m_board.get(), fromRow, fromCol, toRow, toCol, m_currentPlayer)) {
            m_board->removePieceAt(fromRow, fromCol);
            m_board->placePieceAt(toRow, toCol, m_currentPlayer);

            markComponentAsUsed(m_board->getComponentID(fromRow, fromCol));
            return;
        }
    }

}

void Game::markComponentAsUsed(int componentID) {
    m_actionableComponents.erase(componentID);
}

PlayerID Game::getCurrentPlayer() const {
    return m_currentPlayer;
}

void Game::printBoard() const {
    m_board->printBoard();
}
