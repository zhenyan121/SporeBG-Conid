// src/core/Game.cpp
#include "Game.h"


Game::Game(int g_width, int g_heith)
        : Width(g_width),
          Height(g_heith),
          m_render(std::make_unique<Renderer>()) 
{

}


Game::~Game() {
    cleanup();
}
//清理资源
void Game::cleanup() {
    
     
}


bool Game::initialize() {
    if (!m_render->initialize()) {
        return false;
    }
    

   
    
    // 初始化游戏特定资源（棋盘、棋子等）
    // ...
    return true;
}


void Game::render() {
    m_render->Renderhello();
}
