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
                m_currentInputState.leftMouseDown = true;
                
                break;
            }
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event->button.button == SDL_BUTTON_LEFT) {
                m_currentInputState.leftMouseDown = false;

                break;
            }
        case SDL_EVENT_MOUSE_MOTION:
            m_currentInputState.mouseCurrentPosition = {static_cast<float>(event->motion.x), 
                                                        static_cast<float>(event->motion.y)};
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.key == SDLK_F11) {
                m_currentInputState.isFullscreen = !m_currentInputState.isFullscreen;
            }
            break;
    }
    return SDL_APP_CONTINUE;
}

InputState InputManager::GetInputState() const {
    return m_currentInputState;
}
