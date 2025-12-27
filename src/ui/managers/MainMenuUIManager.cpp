#include "MainMenuUIManager.h"

#include "ui/base/UIWidgetFactory.h"
MainMenuUIManager::MainMenuUIManager(
    SceneEventCallback eventCallback) {
    
    m_eventCallback = eventCallback;
}

MainMenuUIManager::~MainMenuUIManager() {

}

void MainMenuUIManager::init() {
    auto startButton = UIWidgetFactory::createStandardButton(
        "StartButton", "开始游戏", 275, 180,
        [this]() { m_eventCallback("GameScene"); }
    );
    
    m_buttons.emplace(startButton->getNameHash(), std::move(startButton));

    auto onlineStartButton = UIWidgetFactory::createStandardButton(
        "OnlineStartButton", "联机对战", 275, 250,
        [this]() {m_eventCallback("OnlineGameScene"); }
    );

    m_buttons.emplace(onlineStartButton->getNameHash(), std::move(onlineStartButton));
    
}

void MainMenuUIManager::CollectRenderData() {

    // 清理上一帧的数据
    m_uiRenderData.buttons.clear();
    m_uiRenderData.labels.clear();

    for (auto& button : m_buttons) {
        if(!button.second->isVisible()) {
            continue;
    
        }
        m_uiRenderData.buttons.push_back(button.second->getButtonDate());
    }

    for (auto& label : m_labels) {
        if(!label.second->isVisible()) {
            continue;
    
        }
        m_uiRenderData.labels.push_back(label.second->getLabelDate());
    }
}


const UIRenderData& MainMenuUIManager::getUIRenderData() {
    CollectRenderData();
    return m_uiRenderData;
}

void MainMenuUIManager::handleClick(int logicalX, int logicalY) {
    
    for (auto& buttonPair : m_buttons) {
        SDL_Log("Handling click at logical position (%d, %d)\n", logicalX, logicalY);
        if (buttonPair.second->handleCilck(logicalX, logicalY)) {
            return;
        }
    }
}