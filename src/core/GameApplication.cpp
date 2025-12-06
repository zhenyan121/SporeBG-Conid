#include "GameApplication.h"

GameApplication::GameApplication() {

}

GameApplication::~GameApplication() {

}

bool GameApplication::initialize() {
    m_inputManager = std::make_unique<InputManager>();
    m_windowManager = std::make_unique<WindowManager>();
    
    m_windowManager->Initialize(m_config);
    m_sceneManager = std::make_unique<SceneManager>(m_windowManager->GetRenderer());
    
    return true;
}

SDL_AppResult GameApplication::handleInputEvent(SDL_Event* event) {
    auto result = m_inputManager->handleInputEvent(event);
     // 只有当事件是鼠标左键按下时，才触发点击逻辑
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && 
        event->button.button == SDL_BUTTON_LEFT) {
        InputState input = m_inputManager->GetInputState();
        m_sceneManager->handleClickCurrent(input.mouseCilckOn);
    }
    return result;
}

void GameApplication::run() {
    m_sceneManager->updateCurrent();
    m_sceneManager->renderCurrent();
}