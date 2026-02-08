#pragma once
#include <SDL3/SDL.h>

#include "core/CoreData.h"
#include "InputState.h"
class InputManager {
public:
    InputManager(CoreData& coreData);
    SDL_AppResult handleInputEvent(const SDL_Event* event);
    
    InputState GetInputState() const;
private:
    InputState& m_currentInputState;
    CoreData& m_coreData;
    bool m_isStartBadApple = false;

    std::string m_badapple;   // 彩蛋
};