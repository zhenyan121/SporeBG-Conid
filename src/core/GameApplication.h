#pragma once


#include "WindowManager.h"
#include "scenes/base/SceneManager.h"
#include "input/InputManager.h"
#include "utils/Config.h"
#include "utils/ConfigLoader.h"
#include "graphics/font/TextRenderer.h"
#include "graphics/font/FontManager.h"
#include "core/DebugManager.h"
#include "graphics/texture/TextureManager.h"
#include "CoreData.h"

class GameApplication {
private:
    
    
    std::unique_ptr<WindowManager> m_windowManager;
    std::unique_ptr<SceneManager> m_sceneManager;
    std::unique_ptr<InputManager> m_inputManager;
    std::unique_ptr<FontManager> m_fontManager;
    std::unique_ptr<TextRenderer> m_textRenderer;
    std::unique_ptr<UIRenderer> m_uiRenderer;
    std::unique_ptr<DebugManager> m_debugManager;
    std::unique_ptr<TextureManager> m_textureManager;
    
    Config m_config;
    void cleanup();

    CoreData m_coreData;

public:
    GameApplication();
    ~GameApplication();
    bool initialize();
    SDL_AppResult handleInputEvent(SDL_Event* event);
    void run();
};