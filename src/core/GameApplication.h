#pragma once

#include "game/GameSession.h"

#include "input/InputManager.h"
#include "utils/Config.h"


class GameApplication {
private:
    
    std::unique_ptr<GameSession> m_gameSession;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<InputManager> m_inputManager;
    GameConfig m_config;

public:
    GameApplication();
    ~GameApplication();
    bool initialize();
    SDL_AppResult handleInputEvent(SDL_Event* event);
    void run();

};