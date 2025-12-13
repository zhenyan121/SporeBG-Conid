#pragma once
#include "scenes/base/Scene.h"
#include "ui/managers/MainMenuUIManager.h"
class MainMenuScene : public Scene {
public:
    MainMenuScene();
    ~MainMenuScene() override = default;

    // 实现 Scene 接口
    void onEnter(SDL_Renderer* renderer, int width, int height, UIRenderer* uiRenderer) override;
    void onExit() override;

    void update() override;
    void render() override;
    void handleClick(float x, float y) override;

    void renderMousePosition(float x, float y) override;
private:
    // 其他私有成员变量和方法
    std::unique_ptr<MainMenuUIManager> m_mainMenuUIManager = nullptr;

};