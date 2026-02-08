#pragma once
 
#include "ui/managers/debug/DebugData.h"
#include "input/InputState.h"
#include "scenes/base/SceneEvent.h"
struct CoreData {
    DebugData debugData;
    InputState inputState;
    SceneType sceneType = SceneType::MainMenuScene;
};