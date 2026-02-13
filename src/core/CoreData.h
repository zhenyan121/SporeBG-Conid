#pragma once
 
#include "ui/managers/debug/DebugData.h"
#include "input/InputState.h"
#include "scenes/base/SceneEvent.h"
#include "graphics/ui/UIRenderer.h"
#include "graphics/texture/TextureManager.h"

struct CoreData {
    DebugData debugData;
    InputState inputState;
    SceneType sceneType = SceneType::MainMenuScene;
    SDL_Renderer* renderer = nullptr;
    UIRenderer* uiRenderer = nullptr;
    SDL_Window* window = nullptr;
    TextureManager* textureManager = nullptr;
};