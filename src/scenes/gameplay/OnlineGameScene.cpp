#include "OnlineGameScene.h"
#include "core/GameApplication.h"
#include <iostream>
void OnlineGameScene::onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT, UIRenderer* uiRenderer) {
    m_renderer = renderer;
    m_uiRenderer = uiRenderer;
    m_gameUIManager = std::make_unique<OnlineGameUIManager>(
        [this](const std::string& sceneName) {
            if (m_eventCallback) {
                SceneEvent event{SceneEventType::ChangeScene, sceneName};
                m_eventCallback(event);
            }
        }
    );
    m_networkManager = std::make_unique<NetworkManager>();
    
    m_gameUIManager->init();
    m_gameUIManager->setCallback([this]() {
        this->restartGame();
    });

    m_gameUIManager->setOnlineTypeCallback(
        [this](NetType type){
            //std::cout << "try to init networkmanager\n";
            if (!m_networkManager) {
                std::cerr << "networkmanager is null\n";
            }
            m_networkManager->init(type);
            if (type == NetType::CLIENT) {
                m_isMyTurn = false;
                m_myPlayerID = PlayerID::P2;
            }
            if (type == NetType::HOST) {
                m_isMyTurn = true;
                m_myPlayerID = PlayerID::P1;
            }
        }
    );
    m_boardRenderer = std::make_unique<BoardRenderer>(WIDTH, HEIGHT, renderer);
    m_gameSession = std::make_unique<GameSession>();
    m_CoordinateConverter = std::make_unique<CoordinateConverter>(renderer);
    m_gameSession->initialize();

    m_boardRenderer->setBoard(m_gameSession->getBoard());

    
    
}

void OnlineGameScene::renderWorld() {
    GameScene::renderWorld();
}

void OnlineGameScene::renderUI() {
    m_uiRenderer->renderUI(m_gameUIManager->getUIRenderData());
}

void OnlineGameScene::handleClick(int logicalX, int logicalY) {
    if (m_gameUIManager && m_gameUIManager->handleClick(logicalX, logicalY)) {
        return;
    }
    // 如果当前是自己的回合就处理点击
    // 否则忽略
    if (!m_isMyTurn) {\
        std::cout << "it is not your turn\n";
        return;
    }
    GameScene::handleClick(logicalX, logicalY);
    if (m_gameSession->getCurrentPlayer() != m_myPlayerID) {
        m_isMyTurn = false;
    }
}

void OnlineGameScene::handleNetworkClick(int logicalX, int logicalY) {
    if (m_isMyTurn) {
        return;
    }
    GameScene::handleClick(logicalX, logicalY);
    if (m_gameSession->getCurrentPlayer() == m_myPlayerID) {
        m_isMyTurn = true;
    }
}