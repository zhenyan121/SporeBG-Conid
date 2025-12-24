#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "ui/components/Button.h"
#include "ui/components/Label.h"
#include "ui/base/UIRenderData.h"

#include <memory>
#include <vector>
#include <unordered_map>

class IUIManager {
public:
    virtual ~IUIManager() = default;

    virtual void init() = 0;

    virtual const UIRenderData& getUIRenderData() = 0;
    // 收集渲染数据
    virtual void CollectRenderData() = 0;

    virtual void updateMousePositon(int logicalX, int logicalY) = 0;

protected:
    std::unordered_map<int, std::unique_ptr<Button>> m_buttons;
    std::unordered_map<int, std::unique_ptr<Label>> m_labels;
    UIRenderData m_uiRenderData;
    SDL_Renderer* m_renderer = nullptr;
    TextRenderer* m_textRenderer = nullptr;

    size_t makeHash(const std::string& name) {
        return std::hash<std::string>{}(name);
    }
};