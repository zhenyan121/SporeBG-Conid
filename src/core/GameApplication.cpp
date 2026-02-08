#include "GameApplication.h"
#include "utils/Tools.h"
#include "Time.h"


GameApplication::GameApplication() 
    
{

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
    if (!ConfigLoader::load("assets/config.json", m_config)) {
        SDL_Log("无法加载json");
    }
    Time::init();
    
   
    
    // 输入管理
    m_inputManager = std::make_unique<InputManager>(m_coreData);
    // 窗口管理
    m_windowManager = std::make_unique<WindowManager>();
    
    m_windowManager->Initialize(m_config);
    
    
    
    // 字体管理
    m_fontManager = std::make_unique<FontManager>();
    // 文字渲染
    m_textRenderer = std::make_unique<TextRenderer>(m_windowManager->GetRenderer(), m_fontManager.get(), m_windowManager->getViewport());

    m_uiRenderer = std:: make_unique<UIRenderer>(m_windowManager->GetRenderer(), m_textRenderer.get());
    // 调试管理
    m_debugManager = std::make_unique<DebugManager>(
        m_windowManager->GetRenderer(),
        m_windowManager->GetWindow(),
        //m_inputManager->GetInputState(),
        m_uiRenderer.get(),
        m_coreData.debugData
    );
    m_debugManager->initialize();
    // 材质管理
    m_textureManager = std::make_unique<TextureManager>(m_windowManager->GetRenderer());

    // 场景管理，传入窗口句柄以便 SceneManager 能获取窗口尺寸
    m_sceneManager = std::make_unique<SceneManager>(m_windowManager->GetRenderer(), m_uiRenderer.get(), m_windowManager->GetWindow(), m_textureManager.get(), m_coreData);
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
        auto pos = Tools::physicalToLogical(input.mouseCilckOn, m_windowManager->getViewport());
        m_sceneManager->handleClickCurrent(pos);
    }
    auto pos = Tools::physicalToLogical(input.mouseCurrentPosition, m_windowManager->getViewport());
    
    m_coreData.inputState.mouseCurrentLogicalPosition = pos;

    m_debugManager->updateMousePos(pos, input);
    
    m_windowManager->setFullscreen(input.isFullscreen);
    
    // 改变窗口时清理旧的缓存
    if (event->type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
        m_windowManager->onWindowResize();
        m_textRenderer->clearCache();
    }

    return result;
    
}

void GameApplication::run() {
    Time::update();
    m_sceneManager->updateCurrent();
    m_debugManager->updateDebugInfo();
    m_windowManager->Clear();
    m_windowManager->beginWorld();
    m_sceneManager->renderWorld();
    m_windowManager->endWorld();
    
    m_windowManager->beginUI();
    m_sceneManager->renderUI();
    m_debugManager->showDebugInfo();
    m_windowManager->endUI();

    m_windowManager->Present();
    cleanup();
}


void GameApplication::cleanup() {
    m_sceneManager->destoryQuitedScene();
}
