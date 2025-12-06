#pragma once
#include <SDL3/SDL.h>


#include "InputState.h"
class InputManager {
public:

    SDL_AppResult handleInputEvent(const SDL_Event* event);
    
    InputState GetInputState() const;
private:
    InputState m_currentInputState;
};