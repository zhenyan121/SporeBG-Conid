#pragma once
#include "Scene.h"
#include <string>
#include <stack>
#include <memory>
#include <unordered_map>
class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    void PushScene(const std::string& sceneName, std::unique_ptr<Scene> scene);
    void PopScene();
    void ChangeScene(const std::string& sceneName, std::unique_ptr<Scene> scene);



private:
    // 用栈的形式来存储场景
    std::stack<std::unique_ptr<Scene>> m_scenes;
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_sceneCache;
};