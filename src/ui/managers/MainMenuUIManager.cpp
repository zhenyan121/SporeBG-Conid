#include "MainMenuUIManager.h"

#include "ui/base/UIWidgetFactory.h"
MainMenuUIManager::MainMenuUIManager(SDL_Renderer* renderer,
    TextRenderer* textRenderer,
    SceneEventCallback eventCallback) {
    m_renderer = renderer; 
    m_textRenderer = textRenderer;
    m_eventCallback = eventCallback;
}

MainMenuUIManager::~MainMenuUIManager() {

}

void MainMenuUIManager::init() {
    auto startButton = UIWidgetFactory::createStandardButton(
        "StartButton", "开始游戏", 0, 0,
        [this]() { m_eventCallback("GameScene"); }
    );
    
    m_buttons.emplace(startButton->getNameHash(), std::move(startButton));
    auto label = UIWidgetFactory::createStandardLabel("MousePositionLabel", "0 0", 240, 0);
    m_labels.emplace(label->getNameHash(), std::move(label));
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

void MainMenuUIManager::updateMousePositon(int logicalX, int logicalY) {
    
    
    std::string pos = std::to_string(logicalX) + " " + std::to_string(logicalY);
    
    m_labels[makeHash("MousePositionLabel")]->setText(pos);
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