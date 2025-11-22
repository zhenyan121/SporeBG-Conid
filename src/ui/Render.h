#pragma once

#include <SDL3/SDL.h>

class Renderer
{
private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr; 
    int Width;
    int Height;
public:
    Renderer(int WIDTH, int HEIGHT);  
    Renderer();
    ~Renderer();
    bool initialize();
    void Renderhello();
};


