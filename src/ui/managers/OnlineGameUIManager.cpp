#include "OnlineGameUIManager.h"
#include "ui/base/UIWidgetFactory.h"
OnlineGameUIManager::OnlineGameUIManager(SceneEventCallback eventCallback)
    : GameUIManager(eventCallback) {
}

OnlineGameUIManager::~OnlineGameUIManager() {
}

void OnlineGameUIManager::init() {
    GameUIManager::init();
    // 在这里可以添加在线游戏特有的UI组件

    auto it = m_buttons.find(makeHash("ActionButton"));
    if (it != m_buttons.end()) {
        auto& actionButton = it->second;
        actionButton->setEnabled(false); // 在线游戏中初始禁用
    }
    
    auto onlineHostButton = UIWidgetFactory::createStandardButton(
        "OnlineHostButton",
        "Host Game",
        200,
        100,
        [this]() {
            // host 模式
            if(m_onlineTypeEvent) {
                m_onlineTypeEvent(NetType::HOST);
            }
        }
    );
    m_buttons.emplace(onlineHostButton->getNameHash(), std::move(onlineHostButton));
    auto onlineJoinButton = UIWidgetFactory::createStandardButton(
        "OnlineJoinButton",
        "Join Game",
        400,
        100,
        [this]() {
            
            if(m_onlineTypeEvent) {
                m_onlineTypeEvent(NetType::CLIENT);
            }
        }
    );
    m_buttons.emplace(onlineJoinButton->getNameHash(), std::move(onlineJoinButton));
}

void OnlineGameUIManager::setOnlineTypeCallback(OnlineTypeEvent onlineTypeEvent) {
    m_onlineTypeEvent = onlineTypeEvent;
}

void OnlineGameUIManager::hideOnlineButtons() {
    auto hostIt = m_buttons.find(makeHash("OnlineHostButton"));
    if (hostIt != m_buttons.end()) {
        hostIt->second->setVisible(false);
        hostIt->second->setEnabled(false);
    }
    auto joinIt = m_buttons.find(makeHash("OnlineJoinButton"));
    if (joinIt != m_buttons.end()) {
        joinIt->second->setVisible(false);
        joinIt->second->setEnabled(false);
    }
}
