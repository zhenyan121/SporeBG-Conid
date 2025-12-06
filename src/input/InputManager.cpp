#include "InputManager.h"

SDL_AppResult InputManager::handleInputEvent(const SDL_Event* event) {

    switch (event->type) {
        // 如果检测到退出
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event->button.button == SDL_BUTTON_LEFT) {
                // 更新状态
                m_currentInputState.mouseCilckOn = {event->button.x, event->button.y};
                
                
                return SDL_APP_CONTINUE;
            }
    }
    return SDL_APP_CONTINUE;
}

InputState InputManager::GetInputState() const {
    return m_currentInputState;
}
