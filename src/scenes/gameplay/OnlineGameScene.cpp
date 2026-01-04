#include "OnlineGameScene.h"
#include "core/GameApplication.h"
#include <iostream>

std::unique_ptr<GameUIManager> OnlineGameScene::createUIManager() {
    auto onlineUIManager = std::make_unique<OnlineGameUIManager>(
        [this](const std::string& sceneName) {
            if (m_eventCallback) {
                SceneEvent event{SceneEventType::ChangeScene, sceneName};
                m_eventCallback(event);
            }
        }
    );
    // 设置网络类型回调
    onlineUIManager->setOnlineTypeCallback(
        [this](NetType type) {
            if (m_networkManager) {
                m_networkManager->init(type);

                m_gameUIManager->setButton("OnlineHostButton", false);
                m_gameUIManager->setButton("OnlineJoinButton", false);
                if (type == NetType::CLIENT) {
                    m_isMyTurn = false;
                    m_myPlayerID = PlayerID::P2;
                    m_gameUIManager->setLabel("OnlineJoinConnectLabel", true);
                } else if (type == NetType::HOST) {
                    m_isMyTurn = true;
                    m_myPlayerID = PlayerID::P1;
                    m_gameUIManager->setLabel("OnlineHostConnectLabel", true);
                }
                m_networkManager->setIsMyTurn(m_isMyTurn);
                
            }
        }
    );
    
    return onlineUIManager;
}

void OnlineGameScene::onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT, UIRenderer* uiRenderer) {
    // 先创建网络管理器
    m_networkManager = std::make_unique<NetworkManager>();
    m_networkManager->setClickEventCallback(
        [this](int logicalX, int logicalY) {
            this->handleNetworkClick(logicalX, logicalY);
        }
    );
    m_networkManager->setStartGameCallback(
        [this]() {
            if (!m_gameUIManager) {
                std::cerr << "gameUIManager is null!\n";
            }
            m_gameUIManager->setLabel("OnlineHostConnectLabel", false);
            m_gameUIManager->setLabel("OnlineJoinConnectLabel", false);
            m_gameUIManager->setButton("ActionButton", true); 
            m_gameUIManager->setLabel("PlayerLabel", true); 
            m_currentGameState = GameState::GAME_RUNING;
        }
    );
    // 调用父类的onEnter（会调用我们重写的createUIManager）
    GameScene::onEnter(renderer, WIDTH, HEIGHT, uiRenderer);

    m_gameUIManager->setButton("ActionButton", false); // 初始禁用行动按钮，等待游戏开始
    m_gameUIManager->setLabel("PlayerLabel", false); // 初始隐藏玩家回合标签
}

bool OnlineGameScene::preHandleClick(int logicalX, int logicalY) {
    
    // 1. 先调用父类的UI处理
    if (GameScene::preHandleClick(logicalX, logicalY)) {
        return true;
    }
    // 2. 检查是否是自己的回合
    // 网络处理会使用这个函数来决定是否处理点击，但是因为不是自己的回合所以不会调用handleBoardClick，导致bug
    /*if (!m_isMyTurn) {
        std::cout << "It is not your turn\n";
        return true; // 不是自己的回合，阻止处理
    }*/
    
    return false; // 继续处理点击
}

void OnlineGameScene::postHandleClick() {
    // 调用父类的后处理
    GameScene::postHandleClick();
    
    // 在线游戏特有的后处理
    if (m_gameSession->getCurrentPlayer() != m_myPlayerID) {
        m_isMyTurn = false;
        // 可以在这里发送网络消息给对手
        m_networkManager->setIsMyTurn(false);
    }
    
}

void OnlineGameScene::handleBoardClick(int row, int col) {
    if (m_currentGameState != GameState::GAME_RUNING) {
        std::cout << "Game is not running, board click ignored.\n";
        return;
    }
    // 调用父类的处理逻辑
    GameScene::handleBoardClick(row, col);
}

void OnlineGameScene::handleClick(int logicalX, int logicalY) {
    
    // 要阻止的是鼠标点击事件，但是网络事件可以处理
    if (m_currentGameState == GameState::GAME_RUNING && !m_isMyTurn) {
        std::cout << "It is not your turn, click ignored.\n";
        return; // 不是自己的回合，忽略点击
    }
    
    // 重用父类的逻辑
    GameScene::handleClick(logicalX, logicalY);
    // 此时调用changgescene时，没有缓存导致类已经析构为空指针，我服了，使用考虑要不要缓存一下在清理
    if (!m_gameSession || !m_networkManager) {
        return;
    }
    if (m_currentGameState != GameState::GAME_RUNING) {
        std::cout << "Game has not started yet, not sending click\n";
        return; // 游戏未开始不发送点击
    }
    if (m_gameSession->getCurrentPlayer() != m_myPlayerID) {
        m_isMyTurn = false;
        m_networkManager->setIsMyTurn(false);
        m_networkManager->postClickPosition(logicalX, logicalY, true);
    } else {
        m_networkManager->postClickPosition(logicalX, logicalY, false);
    }

}

void OnlineGameScene::renderWorld() {
    if (m_currentGameState != GameState::GAME_RUNING) {
        // 渲染遮罩或提示
        m_boardRenderer->renderBlackOverlay();
        return;
    }
    GameScene::renderWorld();
}

void OnlineGameScene::renderUI() {
    GameScene::renderUI();
}
/*
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
*/
void OnlineGameScene::handleNetworkClick(int logicalX, int logicalY) {
    std::cout << "Handling network click at (" << logicalX << ", " << logicalY << ")\n";
     // 处理来自网络的点击
    if (m_isMyTurn) {
        return; // 如果是自己的回合，忽略网络点击
    }
    // 调用父类的基础点击处理
    GameScene::handleClick(logicalX, logicalY);
    std::cout << "After handling network click, current player: "
              << (m_gameSession->getCurrentPlayer() == PlayerID::P1 ? "P1" : "P2") << "\n";
     // 更新回合状态
    if (m_gameSession->getCurrentPlayer() == m_myPlayerID) {
        std::cout << "OnlineGameScene: It is now my turn.\n";
        m_isMyTurn = true;
        m_networkManager->setIsMyTurn(true);
    }
}