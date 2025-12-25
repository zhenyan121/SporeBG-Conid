#include "MainMenuScene.h"

MainMenuScene::MainMenuScene() {
    // 构造函数实现
}

void MainMenuScene::onEnter(SDL_Renderer* renderer, int width, int height, UIRenderer* uiRenderer) {
    // 进入场景时的初始化逻辑
    m_renderer = renderer;
    m_uiRenderer = uiRenderer;
    m_mainMenuUIManager = std::make_unique<MainMenuUIManager>(
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

void MainMenuScene::renderWorld() {
    
    SDL_FRect test = {20, 20, 20, 20};
    // 渲染场景内容
    SDL_SetRenderDrawColor(m_renderer, 20, 20,20, 255);
    SDL_RenderFillRect(m_renderer, &test);
    
}

void MainMenuScene::renderUI() {
    const UIRenderData& uiRenderData = m_mainMenuUIManager->getUIRenderData();
    m_uiRenderer->renderUI(uiRenderData);
}

void MainMenuScene::handleClick(int logicalX, int logicalY) {
    // 处理点击事件
    m_mainMenuUIManager->handleClick(logicalX, logicalY);
}


