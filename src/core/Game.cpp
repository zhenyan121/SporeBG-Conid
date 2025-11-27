// src/core/Game.cpp
#include "Game.h"


Game::Game()         
{
    m_board = std::make_unique<Board>();
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
    

   
    
    
    // ...
    return true;
}



