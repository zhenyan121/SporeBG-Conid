#include "DebugOverlay.h"
#include "ui/base/UIWidgetFactory.h"
#include <iostream>
DebugOverlay::DebugOverlay(DebugData& debugData)
    : m_debugData(debugData) {

}

DebugOverlay::~DebugOverlay() {

}

void DebugOverlay::initialize() {
    auto fpsLabel = UIWidgetFactory::createStandardLabel(
        "FPS",
        "FPS: 0",
        0,
        0
    );

    m_labels.emplace(fpsLabel->getNameHash(), std::move(fpsLabel));
    auto mousePhyicalPos = UIWidgetFactory::createStandardLabel(
        "MousePhyscialPosition",
        "PhyscialPos: 0, 0",
        0,
        40
    );
    m_labels.emplace(mousePhyicalPos->getNameHash(), std::move(mousePhyicalPos));

    auto mouseLogicalPos = UIWidgetFactory::createStandardLabel(
        "MouseLogicalPosition",
        "LogicalPos: 0, 0",
        0,
        80
    );
    m_labels.emplace(mouseLogicalPos->getNameHash(), std::move(mouseLogicalPos));
}


const UIRenderData& DebugOverlay::getUIRenderData() {
    collectRenderData();
    return m_uiRenderData;
}

void DebugOverlay::collectRenderData() {
    // 清理上一帧的数据
    m_uiRenderData.buttons.clear();
    m_uiRenderData.labels.clear();

    for (auto& label : m_labels) {
        if(!label.second->isVisible()) {
            continue;
    
        }
        m_uiRenderData.labels.push_back(label.second->getLabelDate());
        //std::cout << "Collect label: " << label.second->getNameHash() << "\n";
    }
}

void DebugOverlay::updateDebugInfo() {
    const auto key = makeHash("FPS");
    auto fpsLabelIt = m_labels.find(key);

    if (fpsLabelIt != m_labels.end()) {
        auto fpsLabel = fpsLabelIt->second.get();
    
        if (m_debugData.showFPS) {
            fpsLabel->setText("FPS: " + std::to_string(m_debugData.currentFPS));
            fpsLabel->setVisible(true);
        } else {
            
            fpsLabel->setVisible(false);
        }
    } else {
        std::cerr << "FPS Label not found!\n";
    }
    const auto mouseKey = makeHash("MousePhyscialPosition");
    auto mousePosLabelIt = m_labels.find(mouseKey);

    if (mousePosLabelIt != m_labels.end()) {
        auto mousePosLabel = mousePosLabelIt->second.get();

        if (m_debugData.showMousePosition) {
            
            if (!mousePosLabel) {
                std::cerr << "Mouse PhyscialPosition Label not found!\n";
                return;
            }
            mousePosLabel->setText(
                "PhyscialPos: " + 
                std::to_string(m_debugData.mousePosition.first) + ", " +
                std::to_string(m_debugData.mousePosition.second)
            );
            
            mousePosLabel->setVisible(true);
        } else {
            
            mousePosLabel->setVisible(false);
        }
    } else {
        std::cerr << "Mouse PhyscialPosition Label not found!\n";
    }
    const auto mouseLogicalKey = makeHash("MouseLogicalPosition");
    auto mouseLogicalPosLabelIt = m_labels.find(mouseLogicalKey);
    if (mouseLogicalPosLabelIt != m_labels.end()) {
        auto mouseLogicalPosLabel = mouseLogicalPosLabelIt->second.get();

        if (m_debugData.showMousePosition) {
            
            if (!mouseLogicalPosLabel) {
                std::cerr << "Mouse LogicalPosition Label not found!\n";
                return;
            }
            mouseLogicalPosLabel->setText(
                "LogicalPos: " + 
                std::to_string(m_debugData.mouseLogicalPostion.first) + ", " +
                std::to_string(m_debugData.mouseLogicalPostion.second)
            );
            
            mouseLogicalPosLabel->setVisible(true);
        } else {
            
            mouseLogicalPosLabel->setVisible(false);
        }
    } else {
        std::cerr << "Mouse LogicalPosition Label not found!\n";
    }
}