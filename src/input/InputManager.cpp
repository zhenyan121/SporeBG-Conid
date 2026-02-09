#include "InputManager.h"

InputManager::InputManager(CoreData& coreData) :
    m_currentInputState(coreData.inputState),
    m_coreData(coreData) 
{

}

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
            switch(event->key.key) {
                case SDLK_F11:
                    m_currentInputState.isFullscreen = !m_currentInputState.isFullscreen;
                    break;
                case SDLK_B:
                    if (m_coreData.sceneType == SceneType::MainMenuScene) {
                        m_isStartBadApple = true;
                        m_badapple.push_back('b');
                    }
                    
                    break;
                case SDLK_A:
                    if (m_isStartBadApple) {
                        m_badapple.push_back('a');
                    }
                    break;
                case SDLK_D:
                    if (m_isStartBadApple) {
                        m_badapple.push_back('d');
                    }
                    break;
                case SDLK_P:
                    if (m_isStartBadApple) {
                        m_badapple.push_back('p');
                    }
                    break;
                case SDLK_L:
                    if (m_isStartBadApple) {
                        m_badapple.push_back('l');
                    }
                    break;
                case SDLK_E:
                    if (m_isStartBadApple) {
                        m_badapple.push_back('e');
                        if (m_badapple == "badapple") {
                            m_currentInputState.isBadApplePress = true;
                        }
                        m_badapple.clear();
                    }
                    break;
                case SDLK_Q:
                    m_currentInputState.isQ = true;
                    break;
                case SDLK_R:
                    m_currentInputState.isR = true;
            }
            break;
        
    }
    return SDL_APP_CONTINUE;
}

InputState InputManager::GetInputState() const {
    return m_currentInputState;
}

void InputManager::clearInput() {
    m_currentInputState.isQ = false;
    m_currentInputState.isR = false;
}