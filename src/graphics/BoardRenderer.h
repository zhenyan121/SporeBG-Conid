#pragma once
#include "UIType.h"
#include <SDL3/SDL.h>

class Renderer
{
private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr; 
    int m_Width;
    int m_Height;
    int m_cellSize;
    int m_boardRow = 7;
    int m_boardCOL = 7;
public:
    Renderer(int WIDTH, int HEIGHT);  
    
    ~Renderer();
    // 渲染画面
    void render();
    bool initialize();
    // 清除上一帧的渲染
    void clear();
    // 展现画面
    void present();



    // 绘制棋盘
    void drawBoard();

    
    ui::BoardArea getBoardArea() const;

    SDL_Renderer* getSDLRenderer() const;
};


