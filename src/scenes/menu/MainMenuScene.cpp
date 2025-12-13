#include "MainMenuScene.h"

MainMenuScene::MainMenuScene() {
    // 构造函数实现
}

void MainMenuScene::onEnter(SDL_Renderer* renderer, int width, int height, UIRenderer* uiRenderer) {
    // 进入场景时的初始化逻辑
    m_renderer = renderer;
    m_uiRenderer = uiRenderer;
    m_mainMenuUIManager = std::make_unique<MainMenuUIManager>(
        renderer,
        uiRenderer->getTextRenderer(),
        [this](const std::string& sceneName) {
            if (m_eventCallback) {
                SceneEvent event{SceneEventType::ChangeScene, sceneName};
                m_eventCallback(event);
            }
        }
    );
    m_mainMenuUIManager->init();
}

void MainMenuScene::onExit() {
    // 退出场景时的清理逻辑
    m_mainMenuUIManager.reset();
}

void MainMenuScene::update() {
    // 更新场景逻辑
}

void MainMenuScene::render() {
    beginFrame();
    // 渲染场景内容
    const UIRenderData& uiRenderData = m_mainMenuUIManager->getUIRenderData();
    m_uiRenderer->renderUI(uiRenderData);
    endFrame();
}

void MainMenuScene::handleClick(float x, float y) {
    // 处理点击事件
    m_mainMenuUIManager->handleClick(x, y);
}

void MainMenuScene::beginFrame() {
    // 开始帧渲染的逻辑
    if (!m_renderer) {
        SDL_Log("ERROR: Renderer is NULL!\n");
        return;
    }
    // 清屏为白色色背景
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255); 
    SDL_RenderClear(m_renderer);
    //std::cout << "begin frame\n";
    

}

void MainMenuScene::endFrame() {
    // 结束帧渲染的逻辑
        // 提交到屏幕
    //std::cout << "end frame\n";
    SDL_RenderPresent(m_renderer);
}

void MainMenuScene::renderMousePosition(float x, float y) {
    m_mainMenuUIManager->UpdateMousePositon(x, y);
}