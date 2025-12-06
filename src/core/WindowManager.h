#pragma once
#include<SDL3/SDL.h>
#include "utils/Config.h"
class WindowManager {
public:
    WindowManager();
    ~WindowManager();
    // 初始化
    bool Initialize(GameConfig& config);
    //关闭
    void Shutdown();
    //清理上一帧窗口

    void Clear();
    //呈现窗口
    void Present();

    
    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();


private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    int m_width;
    int m_height;

};