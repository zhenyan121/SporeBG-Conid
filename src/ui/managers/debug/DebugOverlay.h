#pragma once

#include "ui/components/Label.h"
#include "DebugData.h"
#include <memory>
#include <unordered_map>

class DebugOverlay {
public:
    DebugOverlay(DebugData& debugData);
    ~DebugOverlay();

    void initialize();
    //void shutdown();


    void collectRenderData();

    const UIRenderData& getUIRenderData();
        
    void updateDebugInfo();


    // 应该添加：
    DebugOverlay(const DebugOverlay&) = delete;
    DebugOverlay& operator=(const DebugOverlay&) = delete;
    DebugOverlay(DebugOverlay&&) = delete;
    DebugOverlay& operator=(DebugOverlay&&) = delete;

private:
    DebugData& m_debugData;
    std::unordered_map<int, std::unique_ptr<Label>> m_labels;
    UIRenderData m_uiRenderData;

    size_t makeHash(const std::string& name) {
        return std::hash<std::string>{}(name);
    }
};