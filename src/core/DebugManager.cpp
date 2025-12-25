#include "DebugManager.h"
#include "Time.h"
#include <iostream>
DebugManager::DebugManager(
    const SDL_Renderer* renderer,
    const SDL_Window* window,
    //const InputState& inputState,
    UIRenderer* uiRenderer
    ):
    m_renderer(renderer),
    m_window(window),
    //m_inputState(inputState),
    m_uiRenderer(uiRenderer)
{

}

DebugManager::~DebugManager() {

}

void DebugManager::initialize() {
    m_debugOverlay = std::make_unique<DebugOverlay>(m_debugData);
    m_debugOverlay->initialize();
    m_isDebugInfoVisible = true;
}

void DebugManager::updateMousePos(int logicalX, int logicalY, const InputState& inputState) {
    m_debugData.mousePosition = {
        static_cast<int>(inputState.mouseCurrentPosition.first),
        static_cast<int>(inputState.mouseCurrentPosition.second)
    };
    m_debugData.mouseLogicalPostion = {logicalX, logicalY};
    
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