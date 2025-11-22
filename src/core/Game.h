// src/core/Game.h
#pragma once
#include <SDL3/SDL.h>
#include <memory>
//#include "Board.h"
#include "ui/Render.h"



class Game {
private:
    
    std::unique_ptr<Renderer> m_render; 
    int Width;
    int Height;
    

public:
    Game(int g_width, int g_heith);
    ~Game();
    void cleanup();
    bool initialize();
    void render();
};