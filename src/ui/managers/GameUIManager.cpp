#include "GameUIManager.h"
#include "ui/base/UIWidgetFactory.h"
GameUIManager::GameUIManager(SceneEventCallback eventCallback) 
{
    m_eventCallback = eventCallback;
}

GameUIManager::~GameUIManager() {

}

void GameUIManager::init() {
    //m_restartCallback = restartCallback;
    setupUIComponents();
}

void GameUIManager::setCallback(std::function<void()> restartCallback) {
    m_restartCallback = restartCallback;
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

void GameUIManager::updateGameState(GameState state) {
    if (m_currentGameState == state) {
        SDL_Log("State unchanged, returning early\n");
        return;
    }
    // 根据传入的 GameState 更新 UI 组件状态
    {
        auto buttonIt = m_buttons.find(makeHash("RestartButton"));
        if (buttonIt != m_buttons.end()) {
            auto& button = buttonIt->second;
            if (state != GameState::GAME_RUNING) {
                //button->setBackgroundColor({255, 215, 0, 255}); // 金色
                //button->setText("GAME OVER");
                button->setVisible(true);
                button->setEnabled(true);
            } else {
                button->setVisible(false);
                button->setEnabled(false);
            }
        }
    }
    {
        auto actionButtonIt = m_buttons.find(makeHash("ActionButton"));
        if (actionButtonIt != m_buttons.end()) {
            auto& actionButton = actionButtonIt->second;
            if (state != GameState::GAME_RUNING) {
                actionButton->setEnabled(false);
                actionButton->setVisible(false);
            } else {
                actionButton->setEnabled(true);
                actionButton->setVisible(true);
            }
        }
    }
    // 保存当前游戏状态，避免后续相同状态更新被误判为无变化
    m_currentGameState = state;
}

void GameUIManager::updatePlayerTurn(PlayerID playerID) {
    auto labelIt = m_labels.find(makeHash("PlayerLabel"));
    if (labelIt != m_labels.end()) {
        auto& label = labelIt->second;
        if (playerID == PlayerID::P1) {
            label->setText("Player 1's Turn");
        } else if (playerID == PlayerID::P2) {
            label->setText("Player 2's Turn");
        }
    }
}

void GameUIManager::setupUIComponents() {
    // 这里可以添加更多的UI组件初始化逻辑

    auto button = UIWidgetFactory::createStandardButton(
        "ActionButton",
        "Please Choose",
        20,
        60,
        [](){

        }
    );
    m_buttons.emplace(button->getNameHash(), std::move(button));

    
    auto restartButton = UIWidgetFactory::createStandardButton(
        "RestartButton",
        "Restart",
        320,
        160,
        [this](){
            if (m_restartCallback) {
                m_restartCallback();
            }
        }

    );
    
    restartButton->setVisible(false); // 初始时隐藏
    restartButton->setEnabled(false);
    m_buttons.emplace(restartButton->getNameHash(), std::move(restartButton));

    auto QuitButton = UIWidgetFactory::createStandardButton(
        "QuitButton",
        "Quit",
        550,
        20,
        [this](){
            m_eventCallback(SceneType::MainMenuScene);
        }

    );
    m_buttons.emplace(QuitButton->getNameHash(), std::move(QuitButton));

    auto playerLabel = UIWidgetFactory::createStandardLabel(
        "PlayerLabel",
        "Player 1's Turn",
        20,
        20
    );
    m_labels.emplace(playerLabel->getNameHash(), std::move(playerLabel));

    auto pieceInfoLabel = UIWidgetFactory::createStandardLabel(
        "PieceInfoLabel",
        "HP: 0\nATK: 0",
        120,
        0
    );
    pieceInfoLabel->setVisible(false);
    pieceInfoLabel->setBackgroundColor({255, 180, 190, 230});
    pieceInfoLabel->setRect(120, 0, UI::ButtonSize * 4, UI::ButtonSize * 3);
    m_labels.emplace(pieceInfoLabel->getNameHash(), std::move(pieceInfoLabel));


}


bool GameUIManager::handleClick(int lx, int ly) {
    
    

    // 遍历所有按钮，检查点击位置是否在按钮范围内
    for (auto& [id, button] : m_buttons) {
        if (button->isEnabled() && button->isVisible()) {
                if(button->handleCilck(lx, ly)) {
                    return true;
                }    
        }
    }
    return false;
}

