#pragma once
#include "GameScene.h"
#include "ui/managers/OnlineGameUIManager.h"
#include "network/NetworkManager.h"
class OnlineGameScene : public GameScene {
public:

    void onEnter(CoreData* coreData) override;

    void renderWorld() override;

    void handleClick(int logicalX, int logicalY) override;

    void handleNetworkClick(int logicalX, int logicalY);

    void renderUI() override;
protected:
    std::unique_ptr<GameUIManager> createUIManager() override;
    bool preHandleClick(int logicalX, int logicalY) override;
    void postHandleClick() override;

    void handleBoardClick(int row, int col) override;
private:
    PlayerID m_myPlayerID = PlayerID::P1;
    bool m_isMyTurn = true;
    std::unique_ptr<NetworkManager> m_networkManager;
    GameState m_currentGameState = GameState::GAME_PREGAME;
};