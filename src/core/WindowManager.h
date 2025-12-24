#pragma once
#include<SDL3/SDL.h>
#include "utils/Config.h"
class WindowManager {
public:
    WindowManager();
    ~WindowManager();
    // 初始化
    bool Initialize(Config& config);
    //关闭
    void Shutdown();
    //清理上一帧窗口

    void beginWorld();

    void endWorld();

    void beginUI();

    void endUI();

    void Clear();
    //呈现窗口
    void Present();
    bool setFullscreen(bool isFullscreen);

    const Viewport& getViewport() const;

    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();


private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    Viewport m_viewport;
    //int m_logicalWidth;
    //int m_logicalHeight;
    //int m_windowWidth;
    //int m_windowHeight;
    bool m_isFullscreen = false;
    SDL_Texture* m_logicalTexture = nullptr;
    // 计算缩放后的目标矩形
    //void calculateDstRect(SDL_FRect& dstRect);
    void onWindowResize();

};