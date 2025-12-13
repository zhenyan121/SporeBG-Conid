#pragma once
#include "ui/base/IUIManager.h"

class GameUIManager : public IUIManager {
private:
    
public:
    GameUIManager(SDL_Renderer* renderer, TextRenderer* textRenderer);
    
    ~GameUIManager();

    void init();

    const UIRenderData& getUIRenderData();
    // 收集渲染数据
    void CollectRenderData();

    void UpdateMousePositon(float x, float y);
};