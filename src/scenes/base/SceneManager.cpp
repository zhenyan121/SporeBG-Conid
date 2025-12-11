#include "SceneManager.h"

SceneManager::SceneManager(SDL_Renderer* renderer, UIRenderer* uiRenderer) :
    m_renderer(renderer),
    m_uiRenderer(uiRenderer)

{
    m_scene = std::make_unique<GameScene>();
    m_scene->onEnter(renderer, 1600, 900, m_uiRenderer);
}

SceneManager::~SceneManager() {
    while (!m_scenes.empty()) {
        m_scenes.pop();
    }
}

void SceneManager::PushScene(const std::string& sceneName, std::unique_ptr<Scene> scene) {

}



void SceneManager::handleClickCurrent(std::pair<float, float> clickon) {
    auto [screenX, screenY] = clickon;
    m_scene->handleClick(screenX, screenY);
}

void SceneManager::updateCurrent() {
    m_scene->update();
}

void SceneManager::renderCurrent() {
    m_scene->render();
}