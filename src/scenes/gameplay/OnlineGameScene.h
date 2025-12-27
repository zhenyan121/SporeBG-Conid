#pragma once
#include "GameScene.h"
#include "ui/managers/OnlineGameUIManager.h"
#include "network/NetworkManager.h"
class OnlineGameScene : public GameScene {
public:

    void onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT, UIRenderer* uiRenderer) override;

    void renderWorld() override;

    void handleClick(int logicalX, int logicalY) override;

    void handleNetworkClick(int logicalX, int logicalY);

    void renderUI() override;
private:
    PlayerID m_myPlayerID;
    bool m_isMyTurn;
    std::unique_ptr<NetworkManager> m_networkManager;
    std::unique_ptr<OnlineGameUIManager> m_gameUIManager;
};