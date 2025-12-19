#include "GameApplication.h"

GameApplication::GameApplication() {

}

GameApplication::~GameApplication() {

}

bool GameApplication::initialize() {
    // 初始化SDL视频子系统[6,7](@ref)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL初始化失败: %s", SDL_GetError());
        return false;
    }
    if (!TTF_Init()) {
        SDL_Log("无法初始化 SDL_ttf: %s", SDL_GetError());
        return false;
    }
    // 输入管理
    m_inputManager = std::make_unique<InputManager>();
    // 窗口管理
    m_windowManager = std::make_unique<WindowManager>();
    
    m_windowManager->Initialize(m_config);
    
    
    
    // 字体管理
    m_fontManager = std::make_unique<FontManager>();
    // 文字渲染
    m_textRenderer = std::make_unique<TextRenderer>(m_windowManager->GetRenderer(), m_fontManager.get());

    m_uiRenderer = std:: make_unique<UIRenderer>(m_windowManager->GetRenderer(), m_textRenderer.get());

    // 场景管理，传入窗口句柄以便 SceneManager 能获取窗口尺寸
    m_sceneManager = std::make_unique<SceneManager>(m_windowManager->GetRenderer(), m_uiRenderer.get(), m_windowManager->GetWindow());
    if (!m_sceneManager->initialize()) {
        SDL_Log("SceneManager 初始化失败！");
        return false;
    }
    return true;
}

SDL_AppResult GameApplication::handleInputEvent(SDL_Event* event) {
    auto result = m_inputManager->handleInputEvent(event);
     // 只有当事件是鼠标左键按下时，才触发点击逻辑
    InputState input = m_inputManager->GetInputState();
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && 
        event->button.button == SDL_BUTTON_LEFT) {
        
        m_sceneManager->handleClickCurrent(input.mouseCilckOn);
    }
    m_sceneManager->handleMousePosition(input.mouseCurrentPosition);
    m_windowManager->setFullscreen(input.isFullscreen);
    return result;
}

void GameApplication::run() {
    m_windowManager->Clear();
    m_sceneManager->updateCurrent();
    m_sceneManager->renderCurrent();
    m_windowManager->Present();
}