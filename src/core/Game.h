// src/core/Game.h
#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include "Board.h"




class Game {
private:
    std::unique_ptr<Board> m_board;
    
    
    

public:
    Game();
    ~Game();
    void cleanup();
    bool initialize();
    
};