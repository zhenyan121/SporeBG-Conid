#include "GameUIManager.h"
#include "utils/CoordinateTools.h"
GameUIManager::GameUIManager(SDL_Renderer* renderer, TextRenderer* textRenderer)
{
    m_renderer = renderer;
    m_textRenderer = textRenderer;
}

GameUIManager::~GameUIManager() {

}

void GameUIManager::init() {
    auto button = std::make_unique<Button>(m_textRenderer);
    button->setBackgroundColor({255, 100, 0, 255});
    button->setBorder(2, {0, 0, 0, 255});
    button->setPosition(20, 20);
    button->setEnabled(true);
    button->setVisible(true);
    button->setText("Please Choose", {"SourceHanSansSC-Regular.otf", 48, {0, 0, 0, 255}});
    button->setName("ActionButton");
    m_buttons.emplace(button->getNameHash(), std::move(button));

    auto label = std::make_unique<Label>();
    label->setPosition(1200, 20);
    label->setText("0 0", {"SourceHanSansSC-Regular.otf", 48, {0, 0, 0, 255}});
    label->setName("MousePositionLabel");
    m_labels.emplace(label->getNameHash(), std::move(label));
}



const UIRenderData& GameUIManager::getUIRenderData() {
    CollectRenderData();
    return m_uiRenderData;
}

void GameUIManager::CollectRenderData() {

    //SDL_Log("CollectRenderData called. buttons count = %zu", m_buttons.size());
    // 清理上一帧的数据
    m_uiRenderData.buttons.clear();
    m_uiRenderData.labels.clear();
    //SDL_Log("collect data\n");
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

void GameUIManager::UpdateMousePositon(float x, float y) {
    
    auto logicalPos = physicalToLogical(static_cast<float>(x), static_cast<float>(y), m_renderer);
    int lx = logicalPos.first;
    int ly = logicalPos.second;
    std::string pos = std::to_string(lx) + " " + std::to_string(ly);
    
    m_labels[makeHash("MousePositionLabel")]->setText(pos);
}

void GameUIManager::updateActionType(ActionType type) {
    // 根据传入的 ActionType 更新 UI 组件状态
    auto buttonIt = m_buttons.find(makeHash("ActionButton"));
    if (buttonIt != m_buttons.end()) {
        auto& button = buttonIt->second;
        switch (type) {
            case ActionType::GROW:
                button->setBackgroundColor({255, 100, 0, 255}); // 橙色
                button->setText("GROW");
                break;
            case ActionType::MOVE:
                button->setBackgroundColor({0, 255, 0, 255}); // 绿色
                button->setText("MOVE");
                break;
            case ActionType::SPORE:
                button->setBackgroundColor({0, 0, 255, 255}); // 蓝色
                button->setText("SPORE");
                break;
            default:
                button->setBackgroundColor({255, 100, 0, 255}); // 默认橙色
                break;
        }
    }
}