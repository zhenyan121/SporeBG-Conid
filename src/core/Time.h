#pragma once
#include <SDL3/SDL.h>

class Time {
public:
    static void init();
    static void update();

    static double deltaTime();
    static double fps();

private:
    static Uint64 s_lastCounter;
    static Uint64 s_freq;
    static double s_deltaTime;

    static int s_frames;
    static double s_fps;
    static double s_fpsTimer;
};
