#pragma once
#include "ui/base/IUIManager.h"
#include "game/GameTypes.h"
class GameUIManager : public IUIManager {
public:
    GameUIManager(SDL_Renderer* renderer, TextRenderer* textRenderer);
    
    ~GameUIManager();

    void init();

    const UIRenderData& getUIRenderData();
    // 收集渲染数据
    void CollectRenderData();

    void UpdateMousePositon(float x, float y);

    void updateActionType(ActionType type);
private:

    
};