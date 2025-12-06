#pragma once
#include "utils/Config.h"
#include <SDL3/SDL.h>

class GameRenderer
{
private:
    SDL_Renderer* m_renderer = nullptr; 
    int m_Width;
    int m_Height;
    int m_cellSize;
    int m_boardRow = 7;
    int m_boardCOL = 7;
public:
    GameRenderer(int WIDTH, int HEIGHT, SDL_Renderer* renderer);  
    
    ~GameRenderer();
    // 渲染画面
    // 初始化加载纹理
    bool initialize();

    // 帧控制
    void beginFrame();
    void endFrame();



    // 渲染函数
    // 绘制背景
    void drawBackground();
    //绘制棋盘
    void drawBoard();

    
    BoardArea getBoardArea() const;

  
};


