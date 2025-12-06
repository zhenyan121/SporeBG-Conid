#pragma once
#include "Scene.h"
#include "scenes/gameplay/GameScene.h"
#include <SDL3/SDL.h>
#include <string>
#include <stack>
#include <memory>
#include <unordered_map>
class SceneManager {
public:
    SceneManager(SDL_Renderer* renderer);
    ~SceneManager();

    void PushScene(const std::string& sceneName, std::unique_ptr<Scene> scene);
   // void PopScene();
    //void ChangeScene(const std::string& sceneName, std::unique_ptr<Scene> scene);
    void handleClickCurrent(std::pair<float, float> clickon);
    void updateCurrent();
    void renderCurrent();


private:
    std::unique_ptr<Scene> m_scene;
    // 用栈的形式来存储场景
    std::stack<std::unique_ptr<Scene>> m_scenes;
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_sceneCache;
};