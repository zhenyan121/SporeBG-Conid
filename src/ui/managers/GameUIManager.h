#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "ui/components/Button.h"
#include "ui/base/UIRenderData.h"
#include <memory>
#include <vector>
class GameUIManager {
private:
    std::vector<std::unique_ptr<Button>> m_buttons;
    UIRenderData m_uiRenderData;

public:
    GameUIManager();
    ~GameUIManager();

    void init();

    const UIRenderData& getUIRenderData();
    // 收集渲染数据
    void CollectRenderData();
};