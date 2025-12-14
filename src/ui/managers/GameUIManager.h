#pragma once
#include "ui/base/IUIManager.h"
#include "game/GameTypes.h"
#include <functional>
class GameUIManager : public IUIManager {
public:
    GameUIManager(SDL_Renderer* renderer, TextRenderer* textRenderer);
    
    ~GameUIManager();

    void init();

    void setCallback(std::function<void()> restartCallback);

    const UIRenderData& getUIRenderData();
    // 收集渲染数据
    void CollectRenderData();

    void UpdateMousePositon(float x, float y);

    void updateActionType(ActionType type);

    void updateGameState(GameState state);

    bool handleClick(float x, float y);
private:
    std::function<void()> m_restartCallback;
    void setupUIComponents();

    GameState m_currentGameState = GameState::GAME_RUNING;
};

