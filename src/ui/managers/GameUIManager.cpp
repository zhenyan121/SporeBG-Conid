#include "GameUIManager.h"

GameUIManager::GameUIManager()
{

}

GameUIManager::~GameUIManager() {

}

void GameUIManager::init() {
    auto button = std::make_unique<Button>();
    button->setBackgroundColor({255, 100, 0, 255});
    button->setBorder(2, {0, 0, 0, 255});
    button->setPosition(20, 20);
    button->setEnabled(true);
    button->setVisible(true);
    button->setText("hello,world!!", {"SourceHanSansSC-Regular.otf", 48, {0, 0, 0, 255}});
    m_buttons.push_back(std::move(button));
}



const UIRenderData& GameUIManager::getUIRenderData() {
    CollectRenderData();
    return m_uiRenderData;
}

void GameUIManager::CollectRenderData() {

    //SDL_Log("CollectRenderData called. buttons count = %zu", m_buttons.size());
    // 清理上一帧的数据
    m_uiRenderData.buttons.clear();
    //SDL_Log("collect data\n");
    for (auto& button : m_buttons) {
        if(!button->isVisible()) {
            continue;
    
        }
        m_uiRenderData.buttons.push_back(button->getButtonDate());
    }
}
