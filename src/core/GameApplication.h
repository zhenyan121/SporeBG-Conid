#pragma once


#include "WindowManager.h"
#include "scenes/base/SceneManager.h"
#include "input/InputManager.h"
#include "utils/Config.h"
#include <SDL3_ttf/SDL_ttf.h>

class GameApplication {
private:
    
    
    std::unique_ptr<WindowManager> m_windowManager;
    std::unique_ptr<SceneManager> m_sceneManager;
    std::unique_ptr<InputManager> m_inputManager;
    GameConfig m_config;

public:
    GameApplication();
    ~GameApplication();
    bool initialize();
    SDL_AppResult handleInputEvent(SDL_Event* event);
    void run();

};