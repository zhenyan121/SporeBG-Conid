#pragma once
#include "ui/managers/debug/DebugOverlay.h"
#include <SDL3/SDL.h>
#include "input/InputState.h"
#include "graphics/ui/UIRenderer.h"

class DebugManager {
public:
    DebugManager(
        const SDL_Renderer* renderer,
        const SDL_Window* window,
        //const InputState& inputState,
        UIRenderer* uiRenderer
    );
    ~DebugManager();

    void initialize();
    //void shutdown();

    void showDebugInfo();

    void updateMousePos(int logicalX, int logicalY, const InputState& inputState);
    void updateDebugInfo();
    
private:
    std::unique_ptr<DebugOverlay> m_debugOverlay;
    DebugData m_debugData;
    const SDL_Renderer* m_renderer = nullptr;
    const SDL_Window* m_window = nullptr;
    //const InputState& m_inputState;
    bool m_isDebugInfoVisible = false;

    UIRenderer* m_uiRenderer = nullptr;

};