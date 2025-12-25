#include "Time.h"

Uint64 Time::s_lastCounter = 0;
Uint64 Time::s_freq = 0;
double Time::s_deltaTime = 0.0;

int Time::s_frames = 0;
double Time::s_fps = 0.0;
double Time::s_fpsTimer = 0.0;

void Time::init()
{
    s_freq = SDL_GetPerformanceFrequency();
    s_lastCounter = SDL_GetPerformanceCounter();
}

void Time::update()
{
    Uint64 now = SDL_GetPerformanceCounter();
    s_deltaTime = (double)(now - s_lastCounter) / s_freq;
    s_lastCounter = now;

    // 防止切后台
    if (s_deltaTime > 0.05)
        s_deltaTime = 0.05;

    // FPS
    s_frames++;
    s_fpsTimer += s_deltaTime;

    if (s_fpsTimer >= 1.0) {
        s_fps = s_frames / s_fpsTimer;
        s_frames = 0;
        s_fpsTimer = 0.0;
    }
}

double Time::deltaTime()
{
    return s_deltaTime;
}

double Time::fps()
{
    return s_fps;
}
