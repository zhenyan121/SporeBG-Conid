#include "SceneManager.h"

SceneManager::SceneManager(SDL_Renderer* renderer, UIRenderer* uiRenderer, SDL_Window* window) :
    m_renderer(renderer),
    m_uiRenderer(uiRenderer),
    m_window(window)

{
    
    
  
}


SceneManager::~SceneManager() {
    // 清理栈和缓存；shared_ptr 会自动管理生命周期
    while (!m_scenes.empty()) {
        m_scenes.pop();
    }
    m_sceneCache.clear();
    m_sceneFactories.clear();
}


bool SceneManager::initialize() {
    // 注册所有场景工厂
    registerAllScene();
    changeScene("MainMenuScene");
    return true;
}

void SceneManager::registerAllScene() {
    // 注册所有默认场景工厂（可以由外部覆盖/新增）
    
    registerSceneFactory("MainMenuScene", []() -> std::shared_ptr<Scene> {
        return std::make_shared<MainMenuScene>();
    });
    registerSceneFactory("GameScene", []() -> std::shared_ptr<Scene> {
        return std::make_shared<GameScene>();
    });
}

void SceneManager::createScene(const std::string& sceneName) {
    // 使用注册的工厂创建场景实例
    auto it = m_sceneFactories.find(sceneName);
    if (it != m_sceneFactories.end()) {
        auto scene = it->second();
        if (scene) {
            m_sceneCache[sceneName] = scene;
        } else {
            SDL_Log("SceneManager::createScene: factory for '%s' returned nullptr\n", sceneName.c_str());
        }
    } else {
        SDL_Log("SceneManager::createScene: no factory registered for '%s'\n", sceneName.c_str());
    }
}

void SceneManager::registerSceneFactory(const std::string& sceneName, std::function<std::shared_ptr<Scene>()> factory) {
    if (sceneName.empty()) return;
    m_sceneFactories[sceneName] = std::move(factory);
}

void SceneManager::unregisterSceneFactory(const std::string& sceneName) {
    m_sceneFactories.erase(sceneName);
}

void SceneManager::pushScene(const std::string& sceneName) {
    if (sceneName.empty()) {
        SDL_Log("SceneManager::pushScene: sceneName is empty!\n");
        return;
    }
    
    // 检查场景是否已在缓存中
    auto it = m_sceneCache.find(sceneName);
    if (it == m_sceneCache.end()) {
        // 场景未缓存，尝试创建
        createScene(sceneName);
        it = m_sceneCache.find(sceneName);
        if (it == m_sceneCache.end()) {
            SDL_Log("SceneManager::pushScene: Scene '%s' not found in cache after creation!\n", sceneName.c_str());
            return;
        }
    }
    
    // 保存当前场景并调用退出回调
    if (m_scene) {
        m_scene->onExit();
        m_scenes.push(m_scene);
    }

    // 设置新场景为当前场景
    m_scene = it->second;

    
    auto [w, h] = getWindowDimensions();
    m_scene->onEnter(m_renderer, w, h, m_uiRenderer);
}


void SceneManager::popScene() {
    if (!m_scene) return;

    // 调用当前场景的退出回调并释放引用
    m_scene->onExit();
    m_scene.reset();

    // 如果栈不为空，恢复前一个场景
    if (!m_scenes.empty()) {
        m_scene = m_scenes.top();
        m_scenes.pop();
        auto [w, h] = getWindowDimensions();
        if (m_scene) {
            m_scene->onEnter(m_renderer, w, h, m_uiRenderer);
        }
    }
}


void SceneManager::changeScene(const std::string& sceneName) {
    if (sceneName.empty()) return;
    
    // 检查场景是否已在缓存中
    if (m_sceneCache.find(sceneName) == m_sceneCache.end()) {
        // 场景未缓存，尝试创建
        createScene(sceneName);
    }
    
    // 从缓存中获取目标场景
    auto target = m_sceneCache[sceneName];
    if (!target) {
        SDL_Log("SceneManager::changeScene: Scene '%s' not found in cache!\n", sceneName.c_str());
        return;
    }
    target->setEventCallback([this](const SceneEvent& event) {
        handleSceneEvent(event);
    });
    // 退出当前场景
    if (m_scene) {
        m_scene->onExit();
    }
    // 切换到目标场景
    m_scene = target;

    auto [w, h] = getWindowDimensions();
    m_scene->onEnter(m_renderer, w, h, m_uiRenderer);
}



void SceneManager::handleClickCurrent(std::pair<float, float> clickOn) {
    auto [screenX, screenY] = clickOn;
    if (m_scene) m_scene->handleClick(screenX, screenY);
}

void SceneManager::handleMousePosition(std::pair<float, float> mousePosition) {
    auto [screenX, screenY] = mousePosition;
    if (m_scene) m_scene->renderMousePosition(screenX, screenY);
}

void SceneManager::updateCurrent() {
    if (m_scene) m_scene->update();
}


void SceneManager::renderWorld() {
    if (m_scene) m_scene->renderWorld();
}

void SceneManager::renderUI() {
    if (m_scene) m_scene->renderUI();
}

std::pair<int, int> SceneManager::getWindowDimensions() const {
    // 获取窗口尺寸的辅助方法
    int w = 0, h = 0;
    if (m_window) {
        SDL_GetRenderLogicalPresentation(m_renderer, &w, &h, nullptr);
    }
    return {w > 0 ? w : 1600, h > 0 ? h : 900};
}

void SceneManager::handleSceneEvent(const SceneEvent& event) {
    // 根据事件类型处理场景事件
    switch (event.type)
    {
    case SceneEventType::ChangeScene:
        changeScene(event.sceneName);
        break;
    case SceneEventType::PushScene:
        pushScene(event.sceneName);
        break;
    default:
        SDL_Log("SceneManager::handleSceneEvent: Unhandled event type!\n");
        break;
    }
}