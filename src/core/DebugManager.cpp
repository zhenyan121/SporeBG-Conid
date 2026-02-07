#include "DebugManager.h"
#include "Time.h"
#include <iostream>
DebugManager::DebugManager(
    const SDL_Renderer* renderer,
    const SDL_Window* window,
    //const InputState& inputState,
    UIRenderer* uiRenderer,
    DebugData& debugData
    ):
    m_renderer(renderer),
    m_window(window),
    //m_inputState(inputState),
    m_uiRenderer(uiRenderer),
    m_debugData(debugData)
{

}

DebugManager::~DebugManager() {

}

void DebugManager::initialize() {
    m_debugOverlay = std::make_unique<DebugOverlay>(m_debugData);
    m_debugOverlay->initialize();
    m_isDebugInfoVisible = true;
}

void DebugManager::updateMousePos(glm::ivec2 logicalPos, const InputState& inputState) {
    m_debugData.mousePosition = {
        static_cast<int>(inputState.mouseCurrentPosition.x),
        static_cast<int>(inputState.mouseCurrentPosition.y)
    };
    m_debugData.mouseLogicalPostion = logicalPos;
    
}

void DebugManager::updateDebugInfo() {
    m_debugData.currentFPS = static_cast<int>(Time::fps());
    //std::cout << "FPS: " << m_debugData.currentFPS << std::endl;

    m_debugOverlay->updateDebugInfo();
    
}

void DebugManager::showDebugInfo() {
    //m_isDebugInfoVisible = !m_isDebugInfoVisible;
    if (m_isDebugInfoVisible) {
        
        m_uiRenderer->renderUI(m_debugOverlay->getUIRenderData());
    }
    
}