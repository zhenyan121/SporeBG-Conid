#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "ui/components/Button.h"
#include "ui/components/Label.h"
#include "ui/base/UIRenderData.h"
#include "scenes/base/SceneEvent.h"
#include <memory>
#include <vector>
#include <unordered_map>

class IUIManager {
public:
    using SceneEventCallback = std::function<void(SceneType sceneType)>;
    virtual ~IUIManager() = default;

    virtual void init() = 0;

    virtual const UIRenderData& getUIRenderData() = 0;
    // 收集渲染数据
    virtual void CollectRenderData() = 0;
    // 提供接口给外部设置ui的各种设置
    void setButton(const std::string& name, int x, int y) {
        size_t hash = makeHash(name);
        auto it = m_buttons.find(hash);
        if (it != m_buttons.end()) {
            it->second->setPosition(x, y);
        }
    }
    void setButton(const std::string&name, bool enabled) {
        size_t hash = makeHash(name);
        auto it = m_buttons.find(hash);
        if (it != m_buttons.end()) {
            it->second->setEnabled(enabled);
            it->second->setVisible(enabled);
        }
    }
    void setButton(const std::string& name, const std::string& text) {
        size_t hash = makeHash(name);
        auto it = m_buttons.find(hash);
        if (it != m_buttons.end()) {
            it->second->setText(text);
        }
    }
    void setLabel(const std::string& name, int x, int y) {
        size_t hash = makeHash(name);
        auto it = m_labels.find(hash);
        if (it != m_labels.end()) {
            it->second->setPosition(x, y);
        }
    }
    void setLabel(const std::string& name, bool visible) {
        size_t hash = makeHash(name);
        auto it = m_labels.find(hash);
        if (it != m_labels.end()) {
            it->second->setVisible(visible);
        }
    }
    void setLabel(const std::string& name, const std::string& text) {
        size_t hash = makeHash(name);
        auto it = m_labels.find(hash);
        if (it != m_labels.end()) {
            it->second->setText(text);
        }
    }
protected:
    std::unordered_map<int, std::unique_ptr<Button>> m_buttons;
    std::unordered_map<int, std::unique_ptr<Label>> m_labels;
    UIRenderData m_uiRenderData;
   
    SceneEventCallback m_eventCallback;
    size_t makeHash(const std::string& name) {
        return std::hash<std::string>{}(name);
    }
};