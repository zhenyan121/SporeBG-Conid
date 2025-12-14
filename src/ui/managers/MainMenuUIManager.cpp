#include "MainMenuUIManager.h"
#include "utils/CoordinateTools.h"
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
    auto button = std::make_unique<Button>(m_textRenderer);
    button->setBackgroundColor({0, 150, 255, 255});
    button->setBorder(2, {0, 0, 0, 255});
    button->setPosition(650, 430);
    button->setName("StartButton");
    button->setText("Start Game", {"SourceHanSansSC-Regular.otf", 64, {255, 255, 255, 255}});
    button->setCallback([this](){
        SDL_Log("Start Game button clicked!");
        m_eventCallback("GameScene");
    });
    m_buttons.emplace(button->getNameHash(), std::move(button));
    auto label = std::make_unique<Label>();
    label->setPosition(1200, 20);
    label->setText("0 0", {"SourceHanSansSC-Regular.otf", 48, {0, 0, 0, 255}});
    label->setName("MousePositionLabel");
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

void MainMenuUIManager::UpdateMousePositon(float x, float y) {
    auto logicalPos = physicalToLogical(static_cast<float>(x), static_cast<float>(y), m_renderer);
    int lx = logicalPos.first;
    int ly = logicalPos.second;
    std::string pos = std::to_string(lx) + " " + std::to_string(ly);
    
    m_labels[makeHash("MousePositionLabel")]->setText(pos);
}

const UIRenderData& MainMenuUIManager::getUIRenderData() {
    CollectRenderData();
    return m_uiRenderData;
}

void MainMenuUIManager::handleClick(float x, float y) {
    auto logicPos = physicalToLogical(x, y, m_renderer);
    for (auto& buttonPair : m_buttons) {
        SDL_Log("Handling click at logical position (%d, %d)\n", logicPos.first, logicPos.second);
        if (buttonPair.second->handleCilck(logicPos.first, logicPos.second)) {
            return;
        }
    }
}