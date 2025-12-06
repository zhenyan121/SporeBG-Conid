#include "GameApplication.h"

GameApplication::GameApplication() {
    m_gameSession = std::make_unique<GameSession>();
    m_inputManager = std::make_unique<InputManager>();

}

void GameApplication::initialize() {
    m_gameSession->initialize();
    
}

SDL_AppResult GameApplication::handleInputEvent(SDL_Event* event) {
    
}