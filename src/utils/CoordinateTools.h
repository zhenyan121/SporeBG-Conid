#pragma once
#include <SDL3/SDL.h>
#include <utility>

std::pair<int, int> physicalToLogical(const float screenX, const float screenY, SDL_Renderer* renderer);
    
