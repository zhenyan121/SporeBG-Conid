#include "SceneManager.h"
#include "utils/Config.h"
SceneManager::SceneManager(SDL_Renderer* renderer, UIRenderer* uiRenderer, SDL_Window* window, TextureManager* textureManager, CoreData& coreData) :
    m_renderer(renderer),
    m_uiRenderer(uiRenderer),
    m_window(window),
    m_textureManager(textureManager),
    m_coreData(coreData)

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
    registerSceneFactory("OnlineGameScene", []() -> std::shared_ptr<Scene> {
        return std::make_shared<OnlineGameScene>();
    });
}

std::shared_ptr<Scene> SceneManager::createScene(const std::string& sceneName) {
    // 使用注册的工厂创建场景实例
    auto it = m_sceneFactories.find(sceneName);
    if (it != m_sceneFactories.end()) {
        auto scene = it->second();
        if (scene) {
            // 并不缓存实例，而是返回给调用者，由调用者决定缓存与否
            return scene;
        } else {
            SDL_Log("SceneManager::createScene: factory for '%s' returned nullptr\n", sceneName.c_str());
        }
    } else {
        SDL_Log("SceneManager::createScene: no factory registered for '%s'\n", sceneName.c_str());
    }
    return nullptr;
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

    
    
    m_scene->onEnter(m_renderer, UI::LogicalWidth, UI::LogicalHeight, m_uiRenderer, m_textureManager, &m_coreData);
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
        
        if (m_scene) {
            m_scene->onEnter(m_renderer, UI::LogicalWidth, UI::LogicalHeight, m_uiRenderer, m_textureManager, &m_coreData);
        }
    }
}


void SceneManager::changeScene(const std::string& sceneName) {
    if (sceneName.empty()) return;
    /*
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
    */
   
    // 不缓存场景，每次都创建新实例，以避免状态残留问题
    auto target = createScene(sceneName);
    if (!target) {
        SDL_Log("SceneManager::changeScene: Scene '%s' could not be created!\n", sceneName.c_str());
        return;
    }
    target->setEventCallback([this](const SceneEvent& event) {
        handleSceneEvent(event);
    });
    // 退出当前场景
    if (m_scene) {
        m_scene->onExit();
        m_quitedScene = m_scene;
    }
    // 切换到目标场景
    m_scene = target;

    
    m_scene->onEnter(m_renderer, UI::LogicalWidth, UI::LogicalHeight, m_uiRenderer, m_textureManager, &m_coreData);
}



void SceneManager::handleClickCurrent(glm::ivec2 clickOn) {
    int logicalX = clickOn.x;
    int logicalY = clickOn.y;
    if (m_scene) m_scene->handleClick(logicalX, logicalY);
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
/*
std::pair<int, int> SceneManager::getWindowDimensions() const {
    // 获取窗口尺寸的辅助方法
    int w = 0, h = 0;
    if (m_window) {
        SDL_GetRenderLogicalPresentation(m_renderer, &w, &h, nullptr);
    }
    return {w > 0 ? w : 1600, h > 0 ? h : 900};
}
*/
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
    case SceneEventType::PopScene:
        popScene();
    default:
        SDL_Log("SceneManager::handleSceneEvent: Unhandled event type!\n");
        break;
    }
}

void SceneManager::destoryQuitedScene() {
    if (!m_quitedScene) {
        return;
    }
    const auto info = typeid(*m_quitedScene.get()).name();
    m_quitedScene.reset();
    std::cout << "SceneManager: " << info << " destroyed\n";
    /*
        对于onlinegamescene，在点击quit之后，因为对象没销毁，把点击操作传到了对方，一起退出了
    */
}