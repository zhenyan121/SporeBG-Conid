#pragma once

#include <SDL3/SDL.h>

class Renderer
{
private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr; 
    int m_Width;
    int m_Height;
public:
    Renderer(int WIDTH, int HEIGHT);  
    Renderer();
    ~Renderer();
    void render();
    bool initialize();
    void Renderhello();
};


